/* port/gpu: the frame driver (docs/gpu.md, "The frame"). OWNED BY THE SKELETON (G0).
 *
 * gpu_render_3d: the span records and the polygon table (g02_spans.c), the band lists, the uploads, pass R (the polygon
 * loop, shaders/g00_raster.frag) into the samples (k x k per output pixel: multisampling, docs/gpu.md "Antialiasing"), pass P
 * (shaders/g06_post.frag) into the 3D layer at N, resolving the samples. */
#define _POSIX_C_SOURCE 200809L
#include "g00_frame.h"
#include "g02_setup.h"
#include "g06_post.h"
#include "g08_smaa.h"
#include "g04_texture.h"
#include "gpu_gl.h"
#include "gpu_mtl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now_ms(void) {
  struct timespec ts;
  static int cpu = -1;
  if (cpu < 0) cpu = getenv("GPU_CPUTIME") != NULL; /* timing under load: this process's CPU time (with LP_NUM_THREADS=0) */
  clock_gettime(cpu ? CLOCK_PROCESS_CPUTIME_ID : CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec * 1e3 + (double)ts.tv_nsec / 1e6;
}

static GLuint target(GLenum ifmt, GLenum fmt, GLenum type, int w, int h) {
  GLuint t;
  GL(GenTextures)(1, &t);
  GL(BindTexture)(GL_TEXTURE_2D, t);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, (GLint)ifmt, w, h, 0, fmt, type, NULL);
  return t;
}

int gpu_scale(const GpuCore *g) { return g->out; }
int gpu_samples(const GpuCore *g) { return g->k * g->k; }
unsigned gpu_3d_tex(const GpuCore *g) { return g->t_out; }
unsigned gpu_smaa_weights(const GpuCore *g) { return g->smaa ? g08_weights(g->smaa) : g->t_weights; }
static int sPassSOff; /* gpu_pass_s_off */

void gpu_set_timing(GpuCore *g, int on) { g->timing = on; }
void gpu_use_texdata(GpuCore *g, const GpuTexData *t) { g->td = (GpuTexData *)t; }

static int env_is(const char *name, const char *val) {
  const char *e = getenv(name);
  return e != NULL && strcmp(e, val) == 0;
}

void gpu_options_from_env(GpuOptions *o) {
  o->dot = env_is("BWPORT_GPU_DOT", "fine");
  o->line = env_is("BWPORT_GPU_LINE", "thin");
  o->edge = env_is("BWPORT_GPU_EDGE", "fine");
  o->filter = env_is("BWPORT_GPU_FILTER", "bilinear");
  o->aa = 1;
  o->fast = env_is("BWPORT_RENDERER3D", "fast");
}

void gpu_set_options(GpuCore *g, const GpuOptions *o) {
  g->opt = *o;
  g->opt.aa = g->smaa || g->t_weights ? GPU_AA_SMAA : g->k; /* the samples are the core's */
  g->opt.fast = o->fast && gpu_fast_supported(g);
}

int gpu_fast_supported(GpuCore *g) { return g->mtl != NULL ? gpu_mtl_fast_ok(g) : gpu_gl_fast_ok(g); }

const char *gpu_fast_how(const GpuCore *g) {
  if (g->mtl != NULL) return gpu_mtl_fast_ok((GpuCore *)g) ? "Metal" : "Metal: its pipelines failed (the log says why)";
  return gpu_gl_fast_how();
}

void gpu_prep_set_options(GpuPrep *p, const GpuOptions *o) {
  p->opt = *o;
  p->opt_set = 1;
}

GpuCore *gpu_create(GpuGetProc getproc, int api, int scale, int aa, char *err, size_t errlen) {
  if (scale < 1 || scale > GPU_MAX_SCALE || aa < 1 || aa > GPU_AA_SMAA) {
    snprintf(err, errlen, "scale %d, aa %d: 1..%d, 1, 2 or 3", scale, aa, GPU_MAX_SCALE);
    return NULL;
  }
  const int metal = api == GPU_API_METAL; /* (getproc unused) */
  if (!metal && gpu_gl_load(getproc, api, err, errlen) != 0) return NULL;
  GpuCore *g = calloc(1, sizeof *g);
  if (g == NULL) {
    snprintf(err, errlen, "out of memory");
    return NULL;
  }
  int smaa = aa == GPU_AA_SMAA;
  if (smaa) aa = 1; /* SMAA: one sample per pixel, two passes after pass P and the blending in pass C (none at 1x) */
  g->out = scale;
  g->k = aa;
  g->scale = scale * aa;
  gpu_options_from_env(&g->opt);
  g->opt.aa = aa;
  g->w = VC_W * scale;
  g->h = VC_H * scale;
  g->nband = (VC_H + BAND_DS_ROWS - 1) / BAND_DS_ROWS;
  g->rb = malloc((size_t)g->w * g->h * 16);
  g->own_td = gpu_texdata_create();
  if (metal) {
    if (g->rb == NULL || g->own_td == NULL || gpu_mtl_init(g, smaa && scale > 1, err, errlen) != 0) {
      if (g->rb == NULL || g->own_td == NULL) snprintf(err, errlen, "out of memory");
      gpu_mtl_release(g); /* (what init made) */
      gpu_texdata_destroy(g->own_td);
      free(g->rb);
      free(g);
      return NULL;
    }
    if (g->t_weights) g->opt.aa = GPU_AA_SMAA;
    return g;
  }
  /* one program per k: the samples' state is in registers, indexed by constants (shaders/g00_head.glsl's G_K) */
  int k2 = aa == 2;
  g->prog_r = gpu_gl_program("g00_full.vert", k2 ? "g00_raster.frag#2" : "g00_raster.frag", err, errlen);
  if (g->prog_r) g->prog_p = gpu_gl_program("g00_full.vert", k2 ? "g06_post.frag#2" : "g06_post.frag", err, errlen);
  if (g->prog_p) g->prog_e = gpu_gl_program("g00_full.vert", k2 ? "g06_edgemap.frag#2" : "g06_edgemap.frag", err, errlen);
  if (g->prog_e && k2) g->prog_pf = gpu_gl_program("g00_full.vert", "g06_post.frag#2+P_FAST", err, errlen);
  if (!g->prog_r || !g->prog_p || !g->prog_e || (k2 && !g->prog_pf) || g->rb == NULL || g->own_td == NULL) {
    gpu_destroy(g);
    return NULL;
  }
  GL(GenVertexArrays)(1, &g->vao);
  char serr[512];
  gpu_g02_init(&g->ps, serr, sizeof serr); /* on failure the CPU builds the records */
  int ns = aa * aa;
  GL(GenTextures)(1, &g->t_samp);
  GL(BindTexture)(GL_TEXTURE_2D_ARRAY, g->t_samp);
  GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if (k2) /* RG32UI a sample (shaders/g00_head.glsl) */
    GL(TexImage3D)(GL_TEXTURE_2D_ARRAY, 0, GL_RG32UI, g->w, g->h, ns, 0, GL_RG_INTEGER, GL_UNSIGNED_INT, NULL);
  else
    GL(TexImage3D)(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32UI, g->w, g->h, ns, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
  GL(BindTexture)(GL_TEXTURE_2D_ARRAY, 0);
  g->t_out = target(GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, g->w, g->h);
  GL(GenFramebuffers)(1, &g->fbo_r);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_r);
  for (int i = 0; i < ns; i++) /* 4 attachments at most: GLES 3.0's minimum GL_MAX_DRAW_BUFFERS */
    GL(FramebufferTextureLayer)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i, g->t_samp, 0, i);
  GLenum st = GL(CheckFramebufferStatus)(GL_FRAMEBUFFER);
  GLenum st3 = GL_FRAMEBUFFER_COMPLETE;
  for (int i = 0; i < (k2 ? 4 : 2); i++) {
    /* k = 2: t_em[2], stage 0's, also holds the antialiasing neighbours (R16UI) */
    g->t_em[i] = i == 2 ? target(GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT, g->w, g->h)
                        : target(GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, i >= 2 ? g->w : VC_W, i >= 2 ? g->h : VC_H);
    GL(GenFramebuffers)(1, &g->fbo_em[i]);
    GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_em[i]);
    GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g->t_em[i], 0);
    if (GL(CheckFramebufferStatus)(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) st3 = GL_FRAMEBUFFER_UNSUPPORTED;
  }
  GL(GenFramebuffers)(1, &g->fbo_p);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_p);
  GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g->t_out, 0);
  if (k2) { /* pass P's two draws: the first marks the pixels it wrote, the second draws the rest (GLES 3.0: depth + stencil) */
    GL(GenTextures)(1, &g->t_st);
    GL(BindTexture)(GL_TEXTURE_2D, g->t_st);
    GL(TexStorage2D)(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, g->w, g->h);
    GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, g->t_st, 0);
  }
  GLenum st2 = GL(CheckFramebufferStatus)(GL_FRAMEBUFFER);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  if (st != GL_FRAMEBUFFER_COMPLETE || st2 != GL_FRAMEBUFFER_COMPLETE || st3 != GL_FRAMEBUFFER_COMPLETE) {
    snprintf(err, errlen, "framebuffer incomplete (0x%x, 0x%x)", st, st2);
    gpu_destroy(g);
    return NULL;
  }
  if (smaa && scale > 1) {
    char aerr[512];
    if ((g->smaa = g08_create(g->w, g->h, aerr, sizeof aerr)) == NULL) /* not fatal: drawn without (gpu_smaa_weights 0) */
      fprintf(stderr, "gpu: SMAA: %s; drawn without\n", aerr);
  }
  if (g->smaa) g->opt.aa = GPU_AA_SMAA;
  g->opt.fast = g->opt.fast && gpu_fast_supported(g);
  return g;
}

void gpu_destroy(GpuCore *g) {
  if (g == NULL) return;
  if (g->mtl != NULL) { /* the Metal core's */
    gpu_mtl_release(g);
    gpu_texdata_destroy(g->own_td);
    gpu_prep_destroy(g->own);
    free(g->rb);
    free(g);
    return;
  }
  gpu_gl_fast_release(g);
  gpu_gl_program_done(g->prog_r);
  if (g->prog_rf != g->prog_r) gpu_gl_program_done(g->prog_rf);
  gpu_gl_program_done(g->prog_p);
  gpu_gl_program_done(g->prog_e);
  gpu_gl_program_done(g->prog_pf);
  GLuint tex[] = {g->t_em[0], g->t_em[1], g->t_em[2], g->t_em[3], g->t_samp, g->t_out, g->t_st};
  for (unsigned i = 0; i < sizeof tex / sizeof tex[0]; i++)
    if (tex[i]) gpu_gl_data_forget(tex[i]), GL(DeleteTextures)(1, &tex[i]);
  for (int i = 0; i < GPU_RING; i++) {
    GLuint ring[] = {g->t_poly[i], g->t_band[i], g->t_list[i], g->t_tex[i], g->t_pal[i]};
    for (int j = 0; j < 5; j++)
      if (ring[j]) gpu_gl_data_forget(ring[j]), GL(DeleteTextures)(1, &ring[j]);
  }
  gpu_g06_release();
  g08_destroy(g->smaa);
  gpu_texdata_destroy(g->own_td);
  gpu_g02_release(&g->ps);
  gpu_gl_unpack_release(); /* (pass C uploads through it too: a later upload makes it again) */
  gpu_gl_uniforms_release();
  if (g->fbo_r) GL(DeleteFramebuffers)(1, &g->fbo_r);
  if (g->fbo_p) GL(DeleteFramebuffers)(1, &g->fbo_p);
  for (int i = 0; i < 4; i++)
    if (g->fbo_em[i]) GL(DeleteFramebuffers)(1, &g->fbo_em[i]);
  if (g->vao) GL(DeleteVertexArrays)(1, &g->vao);
  gpu_prep_destroy(g->own);
  free(g->rb);
  free(g);
}

GpuPrep *gpu_prep_create(int scale) {
  if (scale < 1 || scale > GPU_MAX_SCALE * 2) return NULL; /* the N k grid */
  GpuPrep *p = calloc(1, sizeof *p);
  if (p == NULL) return NULL;
  p->scale = scale;
  p->nband = (VC_H + BAND_DS_ROWS - 1) / BAND_DS_ROWS;
  p->nbin = p->nband * TILES_X;
  p->band = calloc((size_t)p->nbin * 2, sizeof *p->band);
  p->ext = calloc((size_t)VC_MAX_POLYS * p->nband, sizeof *p->ext);
  if (p->band == NULL || p->ext == NULL) {
    free(p->band);
    free(p->ext);
    free(p);
    return NULL;
  }
  return p;
}

void gpu_prep_destroy(GpuPrep *p) {
  if (p == NULL) return;
  gpu_spans_free(&p->sp);
  gpu_fast_free(&p->fast);
  free(p->band);
  free(p->ext);
  free(p->list);
  free(p);
}

int gpu_prep_scale(const GpuPrep *p) { return p->scale; }

int gpu_geometry(VCoreState *s) {
  vc_vram_map(s);
  return gpu_geometry_again(s);
}

/* the geometry engine again over the same stream (in-between pictures: the vertices blended, docs/interp.md): the VRAM views are
 * kept, s->geo is cleared first. Touches nothing the GL half of another frame of this state reads. */
int gpu_geometry_again(VCoreState *s) {
  vc_reset_geo(s);
  vc_mtx_reset(s);
  vc_poly_reset(s);
  vc_light_reset(s);
  u32 i = 0; /* vc_3d_frame's loop (p20_frame.c): a vc_3d_geometry() there is requested */
  while (i < s->gx_words) {
    u32 h = s->gx[i++];
    u32 op = h & 0xFF, n = (h >> 8) & 0xFF;
    if (n > s->gx_words - i) n = s->gx_words - i;
    vc_gx_command(s, op, s->gx + i, n);
    i += n;
  }
  return s->geo.swapped;
}

enum { GPU_LIST_BIAS = 1 << 20 }; /* the list's first-record word (shaders/g00_head.glsl) */
/* the bins' lists: each bin (16 DS columns x 8 DS rows) lists, in drawing order, the polygons that have a pixel run in it:
 * a polygon draws a pixel only inside its row's run [xs, xe] (g_pixel), so the union of its runs' columns over the band's
 * rows is exact for the band, and the bins it meets are those columns' */
static int build_bands(GpuPrep *g) {
  int nb = g->nband, rows = BAND_DS_ROWS * g->scale, tw = TILE_DS_COLS * g->scale, total = 0;
  memset(g->band, 0, (size_t)g->nbin * 2 * sizeof *g->band);
  for (int k = 0, f = 0; k < g->sp.npoly; k++) {
    const GpuPolyRec *pr = &g->sp.poly[k];
    uint8_t(*ex)[2] = &g->ext[(size_t)k * nb];
    int r0 = (int)pr->t[1][0], r1 = (int)pr->t[1][1];
    if (r0 > r1) continue;
    int b0 = r0 / rows, b1 = r1 / rows < nb ? r1 / rows : nb - 1; /* the bands it meets: ext holds those only */
    for (int b = b0; b <= b1; b++) ex[b][0] = TILES_X, ex[b][1] = 0;
    GpuMemo *m = !g->sp.records && nb <= GPU_MEMO_BANDS ? g->sp.memo_of[k] : NULL; /* (gpu_spans_build's memo) */
    if (m != NULL && m->hit && m->extok && !g->sp.memo_check) {
      memcpy(ex[b0], m->ext[b0], (size_t)(b1 - b0 + 1) * sizeof *ex);
      f++;
    } else if (!g->sp.records) { /* pass S builds the records: the extents from the polygon's edges (setup entry f) */
      for (int b = b0; b <= b1; b++) {
        int x0, x1, br0 = r0 > b * rows ? r0 : b * rows, br1 = r1 < (b + 1) * rows - 1 ? r1 : (b + 1) * rows - 1;
        gpu_setup_extent(&g->sp.su, f, br0, br1, &x0, &x1);
        int c0 = x0 < 0 ? 0 : x0 / tw, c1 = x1 < 0 ? -1 : x1 / tw;
        if (c1 >= TILES_X) c1 = TILES_X - 1;
        if (c0 <= c1) ex[b][0] = (uint8_t)c0, ex[b][1] = (uint8_t)c1;
      }
      f++;
      if (m != NULL && m->hit && m->extok) g->sp.memo_bad += memcmp(ex[b0], m->ext[b0], (size_t)(b1 - b0 + 1) * sizeof *ex) != 0;
      if (m != NULL) memcpy(m->ext[b0], ex[b0], (size_t)(b1 - b0 + 1) * sizeof *ex), m->extok = 1;
    }
    const GpuSpan *rec = g->sp.records ? &g->sp.span[pr->t[1][2]] : NULL;
    for (int row = r0; rec != NULL && row <= r1; row++, rec++) {
      int b = row / rows;
      if (b >= nb) break;
      float xs = rec->t[0][0], xe = rec->t[0][1];
      if (xs > xe) continue;
      int c0 = (int)xs / tw, c1 = (int)xe / tw;
      if (c0 < 0) c0 = 0;
      if (c1 >= TILES_X) c1 = TILES_X - 1;
      if (c0 < ex[b][0]) ex[b][0] = (uint8_t)c0;
      if (c1 > ex[b][1]) ex[b][1] = (uint8_t)c1;
    }
    for (int b = b0; b <= b1; b++)
      for (int c = ex[b][0]; c <= ex[b][1]; c++) g->band[(b * TILES_X + c) * 2 + 1]++;
  }
  for (int i = 0; i < g->nbin; i++) {
    g->band[i * 2] = (uint32_t)total;
    total += (int)g->band[i * 2 + 1];
    g->band[i * 2 + 1] = 0;
  }
  if (total > g->listcap) {
    uint32_t *n = realloc(g->list, (size_t)total * 4 * sizeof *n);
    if (n == NULL) return -1;
    g->list = n;
    g->listcap = total;
  }
  for (int k = 0; k < g->sp.npoly; k++) {
    const uint8_t(*ex)[2] = (const uint8_t(*)[2]) & g->ext[(size_t)k * nb];
    const GpuPolyRec *pr = &g->sp.poly[k];
    int r0 = (int)pr->t[1][0], r1 = (int)pr->t[1][1];
    if (r0 > r1) continue;
    int b1 = r1 / rows < nb ? r1 / rows : nb - 1;
    for (int b = r0 / rows; b <= b1; b++)
      for (int c = ex[b][0]; c <= ex[b][1]; c++) {
        uint32_t *bn = &g->band[(b * TILES_X + c) * 2];
        uint32_t *e = &g->list[(size_t)(bn[0] + bn[1]++) * 4];
        int rb0 = r0 > b * rows ? r0 : b * rows, rb1 = r1 < (b + 1) * rows - 1 ? r1 : (b + 1) * rows - 1;
        e[0] = (uint32_t)k;
        e[1] = pr->t[0][0];
        e[2] = (uint32_t)((int)pr->t[1][2] - r0 + GPU_LIST_BIAS);
        e[3] = (uint32_t)rb0 | (uint32_t)rb1 << 16;
      }
  }
  return total;
}

void gpu_prepare(GpuPrep *p, const VCoreState *s) {
  double t0 = now_ms();
  if (!p->opt_set) {
    gpu_options_from_env(&p->opt);
    p->opt_set = 1;
  }
  p->sp.dot_block = !p->opt.dot;
  p->sp.line_wide = !p->opt.line;
  p->sp.aa = p->opt.aa == 2 ? 2 : 1; /* GPU_AA_SMAA: one sample */
  p->sp.opt_set = 1;
  p->sp.setup = gpu_g02_env() != 1 && !sPassSOff; /* (pass S off after a failed self-check: the worker builds the records) */
  p->sp.records = !p->sp.setup || p->check;
  p->swap_param = s->geo.swap_param;
  if (p->opt.fast) { /* the fast renderer: the polygon table and the triangles, no records, no band lists */
    p->nlist = gpu_fast_build(&p->fast, &p->sp, s, p->scale) != 0 ? -1 : 0;
    p->ms = now_ms() - t0;
    return;
  }
  gpu_spans_build(&p->sp, s, p->scale);
  p->nlist = build_bands(p);
  if (getenv("GPU_STATS")) {
    double cand = 0, cov = 0;
    int tw = TILE_DS_COLS * p->scale, th = BAND_DS_ROWS * p->scale;
    for (int i = 0; i < p->nbin; i++) cand += (double)p->band[i * 2 + 1] * tw * th;
    for (int i = 0; p->sp.records && i < p->sp.nspan; i++) { float a = p->sp.span[i].t[0][0], b = p->sp.span[i].t[0][1]; if (b >= a) cov += b - a + 1; }
    fprintf(stderr, "stats: scale %d polys %d list %d  pixel-visits %.0f (%.1f/px)  covered %.0f (%.2f/px)\n", p->scale, p->sp.npoly, p->nlist, cand, cand / (256.0 * 192 * p->scale * p->scale), cov, cov / (256.0 * 192 * p->scale * p->scale));
  }
  p->ms = now_ms() - t0;
}

static void tex_unit(GLuint prog, int unit, const char *name, GLuint tex) {
  GL(ActiveTexture)(GL_TEXTURE0 + unit);
  GL(BindTexture)(GL_TEXTURE_2D, tex);
  GL(Uniform1i)(gpu_gl_uniform(prog, name), unit);
}
/* pass R's program: Filter=bilinear where it applies (uFilter, N > 1) has its own (shaders/g04_texture.glsl's G_FILTER) */
static GLuint raster_prog(GpuCore *g) {
  if (!g->opt.filter || g->scale <= g->k) return g->prog_r;
  if (g->prog_rf == 0) {
    char err[512];
    g->prog_rf = gpu_gl_program("g00_full.vert", g->k == 2 ? "g00_raster.frag#2f" : "g00_raster.frag#1f", err, sizeof err);
    if (g->prog_rf == 0) {
      fprintf(stderr, "gpu: %s; textures unfiltered\n", err);
      g->prog_rf = g->prog_r;
    }
  }
  return g->prog_rf;
}

void gpu_uni_raster(GpuU *u, const GpuCore *g, const VCoreState *s, const GpuPrep *p) {
  memset(u, 0, sizeof *u);
  u->uScale = g->scale;
  u->uSnap = g->k;
  u->uBandRows = BAND_DS_ROWS * g->scale;
  u->uTilesX = TILES_X;
  u->uTileW = TILE_DS_COLS * g->scale;
  u->uDisp3d = vc_io16(s, VC_REG_DISP3DCNT);
  u->uSwap = p->swap_param;
  u->uClearColor = vc_io32(s, VC_REG_CLEAR_COLOR);
  u->uClearDepth = vc_io16(s, VC_REG_CLEAR_DEPTH);
  u->uFlat = getenv("GPU_FLAT") != NULL;
  u->uFilter = g->opt.filter != 0;
  u->uAlphaRef = vc_io32(s, VC_REG_ALPHA_TEST_REF) & 0x1F;
  gpu_g06_uni_raster(u, s);
}

void gpu_uni_edge(GpuU *u, const GpuCore *g, const VCoreState *s) {
  memset(u, 0, sizeof *u);
  u->uScale = g->scale;
  u->uSnap = g->k;
  u->uClearColor = vc_io32(s, VC_REG_CLEAR_COLOR);
  u->uEdgeFine = g->opt.edge != 0; /* stage 3's thickening (k = 2) */
  u->uDisp3d = vc_io16(s, VC_REG_DISP3DCNT); /* stage 0's antialiasing neighbours (k = 2) */
}

void gpu_uni_post(GpuU *u, const GpuCore *g, const VCoreState *s) {
  memset(u, 0, sizeof *u);
  u->uScale = g->scale;
  u->uSnap = g->k;
  u->uDisp3d = vc_io16(s, VC_REG_DISP3DCNT);
  gpu_g06_uni_post(u, s, g->opt.edge);
}

int gpu_edgemap_on(const GpuCore *g, const VCoreState *s) { return g->scale > 1 && (vc_io16(s, VC_REG_DISP3DCNT) & 0x20) != 0; }

int gpu_render_3d(GpuCore *g, VCoreState *s) {
  if (g->own == NULL && (g->own = gpu_prep_create(g->scale)) == NULL) return -1;
  gpu_prep_set_options(g->own, &g->opt);
  gpu_prepare(g->own, s);
  return gpu_render_prepared(g, s, g->own);
}

int gpu_render_prepared(GpuCore *g, VCoreState *s, const GpuPrep *p) {
  double t0 = now_ms();
  const GpuTexData *td = g->td;
  g->td = NULL; /* for this call only */
  if (p->scale != g->scale || p->nlist < 0) return -1;
  if (g->mtl != NULL) {
    if (td == NULL) gpu_texdata_build(g->own_td, s), td = g->own_td;
    return gpu_mtl_render(g, s, p, td);
  }
  const int fast = p->opt.fast; /* the fast renderer: the polygon table, the textures and the triangles (g10_fast_gl.c) */
  if (fast && !gpu_gl_fast_ok(g)) return -1;
  int nlist = p->nlist;
  int on_gpu = !fast && p->sp.setup && g->ps.prog != 0; /* pass S builds the records (else the CPU's are uploaded) */
  GPU_PASS(GPU_PASS_UPLOAD, 1);
  int ri = g->ring = (g->ring + 1) % GPU_RING;
  gpu_gl_data_tex(&g->t_poly[ri], GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, p->sp.poly,
                  p->sp.npoly * GPU_POLY_TEXELS);
  if (!fast && !on_gpu && gpu_g02_records(&g->ps, &p->sp, s, p->scale, g->vao) != 0) return -1; /* the CPU's records */
  if (!fast) gpu_gl_data_tex(&g->t_band[ri], GL_RG32UI, GL_RG_INTEGER, GL_UNSIGNED_INT, 8, p->band, p->nbin);
  if (!fast) gpu_gl_data_tex(&g->t_list[ri], GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, p->list, nlist);
  if (td == NULL) gpu_texdata_build(g->own_td, s), td = g->own_td;
  gpu_g04_upload(g->t_tex, g->t_pal, g->td_up, &g->tr, td);
  GPU_PASS(GPU_PASS_UPLOAD, 0);
  if (g->timing) GL(Finish)();
  double t1 = now_ms();

  /* pass R: the polygon loop (pass S first, in its bracket: the records) */
  GPU_PASS(GPU_PASS_R, 1);
  double ts = t1;
  if (on_gpu) {
    /* a complete framebuffer for the draw (iOS: framebuffer 0 is not SDL's, a draw there is GL_INVALID_FRAMEBUFFER_OPERATION
     * even with the rasterizer off) */
    GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_r);
    if (gpu_g02_records(&g->ps, &p->sp, s, p->scale, g->vao) != 0) return -1;
    if (g->timing) GL(Finish)();
    ts = now_ms();
  }
  GLenum bufs[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
  GpuU u;
  if (fast) {
    gpu_gl_fast_r(g, s, p, ri);
  } else {
    GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_r);
    GL(DrawBuffers)(g->k * g->k, bufs);
    gpu_gl_invalidate(g->k * g->k); /* each pass below overwrites its whole target: no tile loads of the old frame */
    GL(Viewport)(0, 0, g->w, g->h);
    GL(Disable)(GL_DEPTH_TEST);
    GL(Disable)(GL_BLEND);
    GLuint pr = raster_prog(g);
    GL(UseProgram)(pr);
    GL(BindVertexArray)(g->vao);
    tex_unit(pr, 0, "uPoly", g->t_poly[ri]);
    tex_unit(pr, 1, "uSpan", g->ps.t_span[g->ps.sring]);
    tex_unit(pr, 2, "uBand", g->t_band[ri]);
    tex_unit(pr, 3, "uList", g->t_list[ri]);
    gpu_g04_bind(pr, 4, g->t_tex[g->tr], g->t_pal[g->tr]);
    gpu_g06_bind_raster(pr, s);
    gpu_uni_raster(&u, g, s, p);
    gpu_gl_uniforms(&u);
    GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  }
  GPU_PASS(GPU_PASS_R, 0);
  if (g->timing) GL(Finish)();
  double t2 = now_ms();

  /* pass E: where the Edge=ds thickening of pass P can find an edge-flagged pixel (N > 1, edge marking on) */
  int edgemap = gpu_edgemap_on(g, s);
  GL(ActiveTexture)(GL_TEXTURE0 + 7); /* the samples, for passes E and P (unit 7: no other pass uses it) */
  GL(BindTexture)(GL_TEXTURE_2D_ARRAY, g->t_samp);
  if (edgemap) {
    GPU_PASS(GPU_PASS_E, 1);
    GL(UseProgram)(g->prog_e);
    GL(Viewport)(0, 0, VC_W, VC_H);
    gpu_uni_edge(&u, g, s);
    /* multisampling: stage 0 per output pixel into t_em[2] (at w x h: a fragment per pixel, not per DS pixel, 2026-11),
     * stage 2 its DS pixels into t_em[0]; then stage 1 as without; then stage 3, the samples' edge marks, into t_em[3] */
    static const int order1[2] = {0, 1}, order2[4] = {0, 2, 1, 3}, out2[4] = {2, 0, 1, 3}, in2[4] = {-1, 2, 0, 2};
    int nst = g->k > 1 ? 4 : 2;
    for (int i = 0; i < nst; i++) {
      int st = g->k > 1 ? order2[i] : order1[i], out = g->k > 1 ? out2[i] : st, in = g->k > 1 ? in2[i] : st - 1;
      GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_em[out]);
      GL(DrawBuffers)(1, bufs);
      gpu_gl_invalidate(1);
      if (out >= 2) GL(Viewport)(0, 0, g->w, g->h);
      else GL(Viewport)(0, 0, VC_W, VC_H);
      GL(Uniform1i)(gpu_gl_uniform(g->prog_e, "uSamp"), 7);
      tex_unit(g->prog_e, 1, "uEm", g->t_em[in >= 0 ? in : out == 1 ? 0 : 1]); /* (stage 0 reads none: any but its target) */
      if (g->k > 1) tex_unit(g->prog_e, 2, "uEdgeMap", g->t_em[st == 3 ? 1 : 3]); /* stage 3 reads it (the others: not their target) */
      u.uStage = st;
      gpu_gl_uniforms(&u);
      GL(DrawArrays)(GL_TRIANGLES, 0, 3);
    }
    GL(Viewport)(0, 0, g->w, g->h);
    GPU_PASS(GPU_PASS_E, 0);
  }

  /* pass P: post, and with multisampling the resolve (one fragment per pixel: the samples that need their neighbours one
   * by one, the rest only the fog; shaders/g06_post.frag). Multisampling: two draws, the fast pixels (prog_pf, which
   * discards the others and marks the stencil where it wrote), then the slow ones alone (the stencil's 0) */
  GPU_PASS(GPU_PASS_P, 1);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_p);
  GL(DrawBuffers)(1, bufs);
  gpu_gl_invalidate(1);
  gpu_uni_post(&u, g, s);
  /* the depth-stencil texture's two parts by name: Apple's software GLES (the Simulator) rejects GL_DEPTH_STENCIL_ATTACHMENT
   * there with GL_INVALID_ENUM, and three such frames in a row turned the GPU path off */
  static const GLenum ds[2] = {GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT};
  if (g->k > 1) {
    static const GLint zero = 0;
    if (gpu_gl.InvalidateFramebuffer != NULL) gpu_gl.InvalidateFramebuffer(GL_FRAMEBUFFER, 2, ds);
    GL(ClearBufferiv)(GL_STENCIL, 0, &zero);
    GL(Enable)(GL_STENCIL_TEST);
  }
  for (int i = g->k > 1 ? 0 : 1; i < 2; i++) {
    GLuint prog = i == 0 ? g->prog_pf : g->prog_p;
    if (g->k > 1) GL(StencilFunc)(i == 0 ? GL_ALWAYS : GL_EQUAL, i == 0 ? 1 : 0, 0xFF);
    if (g->k > 1) GL(StencilOp)(GL_KEEP, GL_KEEP, i == 0 ? GL_REPLACE : GL_KEEP);
    GL(UseProgram)(prog);
    GL(Uniform1i)(gpu_gl_uniform(prog, "uSamp"), 7);
    if (g->k > 1) tex_unit(prog, 3, "uMark", g->t_em[3]); /* read only when pass E ran */
    if (g->k > 1) tex_unit(prog, 4, "uAaNb", g->t_em[2]); /* the same */
    else tex_unit(prog, 2, "uEdgeMap", edgemap ? g->t_em[1] : g->t_em[0]);
    gpu_gl_uniforms(&u);
    GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  }
  if (g->k > 1) { /* the GL defaults again; the stencil is not needed after */
    GL(Disable)(GL_STENCIL_TEST);
    GL(StencilFunc)(GL_ALWAYS, 0, 0xFFFFFFFFu);
    GL(StencilOp)(GL_KEEP, GL_KEEP, GL_KEEP);
    if (gpu_gl.InvalidateFramebuffer != NULL) gpu_gl.InvalidateFramebuffer(GL_FRAMEBUFFER, 2, ds);
  }
  if (g->timing) GL(Finish)();
  double t3 = now_ms();
  if (g->smaa) g08_run(g->smaa, g->t_out); /* in pass P's bracket */
  GPU_PASS(GPU_PASS_P, 0);
  if (g->timing && g->smaa) GL(Finish)();
  double t4 = now_ms();
  g->cpu_ms = p->ms + (t1 - t0);
  g->total_ms = p->ms + (t4 - t0);
  g->up_ms = t1 - t0;
  g->s_ms = ts - t1;
  g->r_ms = t2 - t1; /* pass S inside */
  g->p_ms = t3 - t2;
  g->e_ms = 0; /* (in p_ms: a glFinish after E would add its wait to the E+P readout) */
  g->a_ms = t4 - t3;
  GLenum e = GL(GetError)();
  return e == GL_NO_ERROR ? 0 : -(int)e;
}

/* GL's row 0 is the bottom one; the passes put DS row 0 there (gl_FragCoord.y = row), so the readback's rows are the
 * DS's order, top first */
void gpu_read_3d(GpuCore *g, uint32_t *out, uint32_t *aux) {
  if (g->mtl != NULL) {
    gpu_mtl_read_3d(g, out, aux);
    return;
  }
  int n = g->w * g->h;
  GL(PixelStorei)(GL_PACK_ALIGNMENT, 4);
  if (g->smaa == NULL || g08_read(g->smaa, g->t_out, g->vao, g->rb) != 0) { /* SMAA: its blending, as pass C does it */
    GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_p);
    GL(ReadBuffer)(GL_COLOR_ATTACHMENT0);
    GL(ReadPixels)(0, 0, g->w, g->h, GL_RGBA_INTEGER, GL_UNSIGNED_INT, g->rb);
  }
  for (int i = 0; i < n; i++) {
    const uint32_t *p = &g->rb[i * 4];
    out[i] = (p[0] & 63) | (p[1] & 63) << 8 | (p[2] & 63) << 16 | (p[3] & 31) << 24;
  }
  if (aux != NULL) { /* the first sample's */
    GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_r);
    GL(ReadBuffer)(GL_COLOR_ATTACHMENT0);
    GL(ReadPixels)(0, 0, g->w, g->h, GL_RGBA_INTEGER, GL_UNSIGNED_INT, aux);
    if (g->k == 2) /* r, g, 0, 1 a texel */
      for (int i = 0; i < n; i++) gpu_samp_unpack(aux[i * 4], aux[i * 4 + 1], &aux[i * 4]);
  }
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
}

/* debug (gpu_pxdiff): pass R's colour per pixel (before pass P; the first sample's), r6 | g6 << 8 | b6 << 16 | a5 << 24 */
void gpu_read_3d_raster(GpuCore *g, uint32_t *out) {
  if (g->mtl != NULL) {
    gpu_mtl_read_raster(g, out);
    return;
  }
  int n = g->w * g->h;
  GL(PixelStorei)(GL_PACK_ALIGNMENT, 4);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_r);
  GL(ReadBuffer)(GL_COLOR_ATTACHMENT0);
  GL(ReadPixels)(0, 0, g->w, g->h, GL_RGBA_INTEGER, GL_UNSIGNED_INT, g->rb);
  if (g->k == 2)
    for (int i = 0; i < n; i++) gpu_samp_unpack(g->rb[i * 4], g->rb[i * 4 + 1], &g->rb[i * 4]);
  for (int i = 0; i < n; i++) out[i] = g->rb[i * 4 + 2];
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
}

void gpu_last_times(const GpuCore *g, double *cpu_ms, double *total_ms) {
  if (cpu_ms) *cpu_ms = g->cpu_ms;
  if (total_ms) *total_ms = g->total_ms;
}

void gpu_last_passes(const GpuCore *g, double *up_ms, double *r_ms, double *p_ms) {
  if (up_ms) *up_ms = g->up_ms;
  if (r_ms) *r_ms = g->r_ms;
  if (p_ms) *p_ms = g->p_ms;
}

double gpu_last_smaa_ms(const GpuCore *g) { return g->a_ms; }
double gpu_last_pass_s(const GpuCore *g) { return g->s_ms; }
double gpu_last_pass_e(const GpuCore *g) { return g->e_ms; }

/* pass S off for good (the bridge's self-check found its records wrong on this driver): from now on the worker's preps build the
 * records on the CPU, and a prep made before still gets them (gpu_g02_records builds them on this thread). Set once, read by the
 * worker's gpu_prepare: a prep made just before sees 0 and is served by that path. */
void gpu_pass_s_off(GpuCore *g) {
  sPassSOff = 1;
  if (g->mtl != NULL) {
    gpu_mtl_pass_s_off(g);
    return;
  }
  gpu_gl_program_done(g->ps.prog);
  g->ps.prog = 0;
}

int gpu_spans_check(GpuCore *g, VCoreState *s, void *f) {
  if ((g->mtl != NULL ? !gpu_mtl_pass_s(g) : g->ps.prog == 0) || sPassSOff) {
    fprintf(f, "spans-check: pass S is off\n");
    return -1;
  }
  GpuPrep *p = gpu_prep_create(g->scale);
  if (p == NULL) return -1;
  gpu_prep_set_options(p, &g->opt);
  p->opt.fast = 0; /* (the records are the exact renderer's: with Renderer3D=fast the check had none to compare) */
  p->check = 1;
  gpu_prepare(p, s); /* the setup and the CPU's records */
  int n = -1;
  uint32_t *buf = malloc((size_t)(p->sp.nspan + 1) * GPU_SPAN_TEXELS * 16);
  /* the frame with each way of making the records in turn until one is right (gpu_g02_step: the diagnostics, the next way) */
  if (g->mtl != NULL && buf != NULL && gpu_render_prepared(g, s, p) == 0) { /* one way: the kernel */
    fprintf(f, "pass S: metal\n");
    gpu_mtl_spans_read(g, p->sp.nspan, buf);
    n = gpu_g02_compare(&p->sp, buf, f, 1);
  }
  for (int i = 0; buf != NULL && g->mtl == NULL; i++) {
    g->ps.diag = 1;
    int rc = gpu_render_prepared(g, s, p);
    g->ps.diag = 0;
    if (rc != 0) break;
    if (g->ps.prog == 0) { /* none left: this core builds them (the caller turns pass S off) */
      fprintf(f, "spans-check: pass S is off\n");
      n = 1;
      break;
    }
    fprintf(f, "pass S: %s\n", gpu_g02_mode_name());
    gpu_g02_read(&g->ps, p->sp.nspan, buf);
    n = gpu_g02_compare(&p->sp, buf, f, i == 0);
    if (n <= 0 || gpu_g02_step(&g->ps, &p->sp, buf, f) == GPU_PS_CPU) break;
  }
  free(buf);
  gpu_prep_destroy(p);
  return n;
}
