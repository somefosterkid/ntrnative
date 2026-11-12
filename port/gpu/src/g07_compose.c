/* port/gpu G7: engine A's compositor at N x, the CPU side (docs/gpu.md, G7; shaders/g07_compose.frag). OWNED BY G7.
 *
 * Per frame: the 2D layers of engine A (vcore, 1x) packed into one RGBA32UI texture, the 1x lines of what the shader
 * does not compose (engine A in display modes 0/2/3, forced blank or powered off; engine B) into an R32UI texture (g07_pack:
 * the port's worker does it, and a frame's in-between pictures reuse the upload),
 * the registers as uniforms (vcore applies them per frame, not per line), one full-screen pass into an RGBA8 texture. */
#include "g07_compose.h"
#include "gpu_gl.h"
#include "gpu_mtl.h"
#include "vc_shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct G07 {
  int scale, w, h; /* scale: the grid pass C draws on (N); w, h: the output (256 N x 384 N) */
  GLuint prog, vao, fbo, t_out, t_stage, t_src, t_3d, t_none; /* t_none: 1 x 1 RGBA8, cSmaa's when unused */
  GLuint prog_plan, fbo_plan, t_plan; /* the plan (g07_compose.frag's C_PLAN), 256 x 192 RG32UI */
  unsigned plan_serial;               /* the pack the plan is made from (0: none) and the uniforms it read */
  uint32_t plan_key[8];
  int sc3d;
  VC_Line *own;    /* g07_stage_build's lines when the caller gives none */
  G07Pack *pack;   /* g07_compose's */
  unsigned up;     /* the pack in t_stage / t_src (0: none) */
  uint8_t *rb;
  GpuMtlC *mtl; /* g07_create_metal's: the GL fields are unused */
};

struct G07Pack {
  uint32_t stage[VC_H * VC_W * 4];
  uint32_t src[VC_H * 2 * VC_W]; /* engine A's lines (when not composed), then engine B's */
  unsigned serial;               /* this pack's number */
  unsigned aserial;              /* the number of the pack engine A's stage was made in (reuse bit 0: an earlier one's) */
};

G07Pack *g07_pack_create(void) { return calloc(1, sizeof(G07Pack)); }
void g07_pack_destroy(G07Pack *k) { free(k); }

/* engine A composed on the GPU only in display mode 1, powered on and not forced blank (vc_display_line) */
static int composed(VCoreState *s) {
  u32 dispcnt = vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT);
  int mode = (dispcnt >> 16) & 3, on = (vc_io16(s, VC_REG_POWCNT1) >> 1) & 1;
  return mode == 1 && on && !(dispcnt & 0x80);
}

void g07_pack(G07Pack *k, VCoreState *s, const VC_Line *stage) { g07_pack_reuse(k, s, stage, NULL, 0); }

void g07_pack_reuse(G07Pack *k, VCoreState *s, const VC_Line *stage, const G07Pack *from, int reuse) {
  static unsigned serial; /* the packs' count over all G07Packs (atomic: two workers may pack) */
  int compose = composed(s);
  if (reuse & 1) {
    memcpy(k->stage, from->stage, sizeof k->stage);
    if (!compose) memcpy(k->src, from->src, sizeof k->src / 2);
  }
  if (reuse & 2) memcpy(&k->src[VC_H * VC_W], &from->src[VC_H * VC_W], sizeof k->src / 2);
  for (int y = 0; y < VC_H; y++) {
    if (!(reuse & 2)) memcpy(&k->src[(size_t)(VC_H + y) * VC_W], s->eng_out[VC_ENG_B][y], VC_W * 4);
    if (reuse & 1) continue;
    const VC_Line *ln = &stage[y];
    uint32_t *o = &k->stage[(size_t)y * VC_W * 4];
    uint32_t bgon = (uint32_t)(ln->bg_on[0] | ln->bg_on[1] << 1 | ln->bg_on[2] << 2 | ln->bg_on[3] << 3);
    for (int x = 0; x < VC_W; x++) {
      o[x * 4 + 0] = ln->obj[x];
      o[x * 4 + 1] = (ln->bg[0][x] & 0xFFFFu) | (ln->bg[1][x] & 0xFFFFu) << 16;
      o[x * 4 + 2] = (ln->bg[2][x] & 0xFFFFu) | (ln->bg[3][x] & 0xFFFFu) << 16;
      o[x * 4 + 3] = ln->winmask[x] | bgon << 8 | (uint32_t)(ln->bg3d != 0) << 12;
    }
    if (!compose) vc_display_line(s, VC_ENG_A, y, ln, &k->src[(size_t)y * VC_W]);
  }
  k->serial = __atomic_add_fetch(&serial, 1, __ATOMIC_RELAXED);
  k->aserial = (reuse & 1) ? from->aserial : k->serial;
}

int g07_pack_differs(const G07Pack *a, const G07Pack *b, VCoreState *s, int mask) {
  int d = 0;
  if ((mask & 1) && (memcmp(a->stage, b->stage, sizeof a->stage) != 0 || (!composed(s) && memcmp(a->src, b->src, sizeof a->src / 2) != 0)))
    d |= 1;
  if ((mask & 2) && memcmp(&a->src[VC_H * VC_W], &b->src[VC_H * VC_W], sizeof a->src / 2) != 0) d |= 2;
  return d;
}

static GLuint make_tex(GLenum ifmt, int w, int h) {
  GLuint t;
  GL(GenTextures)(1, &t);
  GL(BindTexture)(GL_TEXTURE_2D, t);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  (void)ifmt; (void)w; (void)h;
  return t;
}

G07 *g07_create(int scale, char *err, size_t errlen) {
  if (scale < 1 || scale > GPU_MAX_SCALE) {
    snprintf(err, errlen, "scale %d: 1..%d", scale, GPU_MAX_SCALE);
    return NULL;
  }
  G07 *c = calloc(1, sizeof *c);
  if (c == NULL) {
    snprintf(err, errlen, "out of memory");
    return NULL;
  }
  c->scale = scale;
  c->w = VC_W * scale;
  c->h = VC_H * 2 * scale;
  c->rb = malloc((size_t)c->w * c->h * 4);
  c->prog = gpu_gl_program("g00_full.vert", "g07_compose.frag", err, errlen);
  c->prog_plan = c->prog ? gpu_gl_program("g00_full.vert", "g07_compose.frag#1+C_PLAN", err, errlen) : 0;
  if (!c->rb || !c->prog || !c->prog_plan) {
    g07_destroy(c);
    return NULL;
  }
  GL(GenVertexArrays)(1, &c->vao);
  c->t_out = make_tex(GL_RGBA8, c->w, c->h);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA8, c->w, c->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  c->t_stage = make_tex(GL_RGBA32UI, VC_W, VC_H);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA32UI, VC_W, VC_H, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
  c->t_src = make_tex(GL_R32UI, VC_W, VC_H * 2);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_R32UI, VC_W, VC_H * 2, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
  c->t_none = make_tex(GL_RGBA8, 1, 1);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  c->t_plan = make_tex(GL_RG32UI, VC_W, VC_H);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RG32UI, VC_W, VC_H, 0, GL_RG_INTEGER, GL_UNSIGNED_INT, NULL);
  GL(GenFramebuffers)(1, &c->fbo);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo);
  GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, c->t_out, 0);
  GLenum st = GL(CheckFramebufferStatus)(GL_FRAMEBUFFER);
  GL(GenFramebuffers)(1, &c->fbo_plan);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo_plan);
  GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, c->t_plan, 0);
  if (st == GL_FRAMEBUFFER_COMPLETE) st = GL(CheckFramebufferStatus)(GL_FRAMEBUFFER);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  if (st != GL_FRAMEBUFFER_COMPLETE) {
    snprintf(err, errlen, "G7 framebuffer incomplete (0x%x)", st);
    g07_destroy(c);
    return NULL;
  }
  return c;
}

G07 *g07_create_metal(int scale, char *err, size_t errlen) {
  if (scale < 1 || scale > GPU_MAX_SCALE) {
    snprintf(err, errlen, "scale %d: 1..%d", scale, GPU_MAX_SCALE);
    return NULL;
  }
  G07 *c = calloc(1, sizeof *c);
  if (c == NULL || (c->mtl = gpu_mtl_c_create(scale, err, errlen)) == NULL) {
    free(c);
    return NULL;
  }
  c->scale = scale;
  c->w = VC_W * scale;
  c->h = VC_H * 2 * scale;
  return c;
}

void g07_destroy(G07 *c) {
  if (c == NULL) return;
  if (c->mtl != NULL) {
    gpu_mtl_c_destroy(c->mtl);
    free(c->own);
    g07_pack_destroy(c->pack);
    free(c);
    return;
  }
  gpu_gl_program_done(c->prog);
  gpu_gl_program_done(c->prog_plan);
  GLuint tex[] = {c->t_out, c->t_stage, c->t_src, c->t_3d, c->t_none, c->t_plan};
  for (unsigned i = 0; i < sizeof tex / sizeof tex[0]; i++)
    if (tex[i]) GL(DeleteTextures)(1, &tex[i]);
  if (c->fbo) GL(DeleteFramebuffers)(1, &c->fbo);
  if (c->fbo_plan) GL(DeleteFramebuffers)(1, &c->fbo_plan);
  if (c->vao) GL(DeleteVertexArrays)(1, &c->vao);
  free(c->own);
  g07_pack_destroy(c->pack);
  free(c->rb);
  free(c);
}

/* vc_render_line_a1 (p20_frame.c): OBJs, BGs (the 3D BG only marked; vc_bg_line also reads s->f3d.out for it, which is
 * not used), windows */
void g07_stage_build(VCoreState *s, VC_Line *stage) {
  for (int y = 0; y < VC_H; y++) {
    VC_Line *ln = &stage[y];
    memset(ln, 0, sizeof *ln);
    memset(ln->winmask, 0x3F, sizeof ln->winmask);
    vc_obj_line(s, VC_ENG_A, y, ln);
    for (int n = 0; n < 4; n++) vc_bg_line(s, VC_ENG_A, n, y, ln);
    vc_window_line(s, VC_ENG_A, y, ln);
  }
}

unsigned g07_upload_3d(G07 *c, const uint32_t *l3d, int sc) {
  size_t n = (size_t)VC_W * sc * VC_H * sc;
  uint8_t *b = malloc(n * 4);
  if (b == NULL) return 0;
  for (size_t i = 0; i < n; i++) {
    uint32_t p = l3d[i];
    b[i * 4 + 0] = (uint8_t)(p & 63);
    b[i * 4 + 1] = (uint8_t)((p >> 8) & 63);
    b[i * 4 + 2] = (uint8_t)((p >> 16) & 63);
    b[i * 4 + 3] = (uint8_t)((p >> 24) & 31);
  }
  if (c->mtl != NULL) {
    unsigned t = gpu_mtl_c_upload_3d(c->mtl, b, VC_W * sc, VC_H * sc);
    free(b);
    c->sc3d = sc;
    return t;
  }
  if (c->t_3d == 0) c->t_3d = make_tex(GL_RGBA8UI, 0, 0);
  GL(BindTexture)(GL_TEXTURE_2D, c->t_3d);
  GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA8UI, VC_W * sc, VC_H * sc, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, b);
  free(b);
  c->sc3d = sc;
  return c->t_3d;
}

static void tex_unit(GLuint prog, int unit, const char *name, GLuint tex) {
  GL(ActiveTexture)(GL_TEXTURE0 + unit);
  GL(BindTexture)(GL_TEXTURE_2D, tex);
  GL(Uniform1i)(gpu_gl_uniform(prog, name), unit);
}

/* pass C's uniforms for the frame of s (have3d, smaa: a 3D layer is given, with SMAA's weights) */
static void uni_compose(GpuU *u, VCoreState *s, int scale, int sc, int have3d, int smaa) {
  u32 dispcnt = vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT);
  u16 bld = vc_eng16(s, VC_ENG_A, VC_REG_BLDCNT), alpha = vc_eng16(s, VC_ENG_A, VC_REG_BLDALPHA);
  uint32_t prios = 0;
  for (int n = 0; n < 4; n++) prios |= (uint32_t)(vc_eng16(s, VC_ENG_A, VC_REG_BG0CNT + 2 * n) & 3) << (2 * n);
  int eva = alpha & 0x1F, evb = (alpha >> 8) & 0x1F, evy = vc_eng16(s, VC_ENG_A, VC_REG_BLDY) & 0x1F;
  memset(u, 0, sizeof *u);
  u->cSmaaOn = have3d && smaa;
  u->cScale = scale;
  u->cScale3d = sc;
  u->cHave3d = have3d != 0;
  u->cCompose = (uint32_t)composed(s);
  u->cATop = (vc_io16(s, VC_REG_POWCNT1) >> 15) & 1;
  u->cObjOn = (dispcnt >> 12) & 1;
  u->cPrios = prios;
  u->cBld = bld;
  u->cEva = eva > 16 ? 16 : eva;
  u->cEvb = evb > 16 ? 16 : evb;
  u->cEvy = evy > 16 ? 16 : evy;
  u->cBackdrop = vc_555_to_rgb6(vc_pal(s, VC_ENG_A, 0, 0));
  u->cHofs = vc_eng16(s, VC_ENG_A, VC_REG_BG0HOFS) & 0x1FF;
  u->cBright = vc_eng16(s, VC_ENG_A, VC_REG_MASTER_BRIGHT);
}

int g07_compose(G07 *c, VCoreState *s, const VC_Line *stage, unsigned tex3d, int sc, unsigned smaa) {
  if (stage == NULL) {
    if (c->own == NULL) c->own = malloc(sizeof(VC_Line) * VC_H);
    if (c->own == NULL) return -1;
    g07_stage_build(s, c->own);
    stage = c->own;
  }
  if (c->pack == NULL && (c->pack = g07_pack_create()) == NULL) return -1;
  g07_pack(c->pack, s, stage);
  return g07_compose_packed(c, s, c->pack, tex3d, sc, smaa);
}

/* 1 when the plan is to be made (again) for engine A's stage of pack serial aserial and the uniforms it reads; engine A
 * composed only */
static int plan_stale(G07 *c, const GpuU *u, unsigned serial) {
  if (!u->cCompose) return 0;
  uint32_t key[8] = {u->cObjOn, u->cPrios, u->cBld, (uint32_t)u->cEva, (uint32_t)u->cEvb, (uint32_t)u->cEvy, u->cBackdrop,
                     u->cBright};
  if (serial == c->plan_serial && !memcmp(key, c->plan_key, sizeof key)) return 0;
  c->plan_serial = serial;
  memcpy(c->plan_key, key, sizeof key);
  return 1;
}

int g07_compose_packed(G07 *c, VCoreState *s, const G07Pack *k, unsigned tex3d, int sc, unsigned smaa) {
  GpuU u;
  uni_compose(&u, s, c->scale, sc, tex3d != 0, smaa != 0);
  int plan = plan_stale(c, &u, k->aserial);
  if (c->mtl != NULL) return gpu_mtl_compose(c->mtl, k->stage, k->src, k->serial, plan, tex3d, smaa, &u);
  if (k->serial != c->up) { /* (the textures keep their size: sub-image uploads) */
    GPU_PASS(GPU_PASS_UPLOAD, 1);
    /* through one unpack buffer (docs/gpu-cost.md P3): the driver converts it to the textures' layout on the GPU, not
     * on this thread as it does client memory */
    const size_t nstage = sizeof k->stage, nsrc = sizeof k->src;
    const void *stage = k->stage, *src = k->src;
    unsigned char *m = gpu_gl_unpack_map(nstage + nsrc);
    if (m != NULL) {
      memcpy(m, k->stage, nstage);
      memcpy(m + nstage, k->src, nsrc);
      if (gpu_gl_unpack_unmap() == 0) stage = (const void *)0, src = (const void *)nstage;
    }
    GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
    GL(BindTexture)(GL_TEXTURE_2D, c->t_stage);
    GL(TexSubImage2D)(GL_TEXTURE_2D, 0, 0, 0, VC_W, VC_H, GL_RGBA_INTEGER, GL_UNSIGNED_INT, stage);
    GL(BindTexture)(GL_TEXTURE_2D, c->t_src);
    GL(TexSubImage2D)(GL_TEXTURE_2D, 0, 0, 0, VC_W, VC_H * 2, GL_RED_INTEGER, GL_UNSIGNED_INT, src);
    gpu_gl_unpack_done();
    GPU_PASS(GPU_PASS_UPLOAD, 0);
    c->up = k->serial;
  }

  GLenum buf = GL_COLOR_ATTACHMENT0;
  GPU_PASS(GPU_PASS_C, 1);
  GL(Disable)(GL_DEPTH_TEST);
  GL(Disable)(GL_BLEND);
  GL(BindVertexArray)(c->vao);
  if (plan) { /* the plan from the 2D layers, at 1x (a frame's in-betweens reuse it) */
    GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo_plan);
    GL(DrawBuffers)(1, &buf);
    gpu_gl_invalidate(1);
    GL(Viewport)(0, 0, VC_W, VC_H);
    GL(UseProgram)(c->prog_plan);
    tex_unit(c->prog_plan, 0, "cStage", c->t_stage);
    gpu_gl_uniforms(&u);
    GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  }
  GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo);
  GL(DrawBuffers)(1, &buf);
  gpu_gl_invalidate(1); /* overwritten whole */
  GL(Viewport)(0, 0, VC_W * c->scale, VC_H * 2 * c->scale);
  GL(UseProgram)(c->prog);
  tex_unit(c->prog, 0, "cPlan", c->t_plan);
  tex_unit(c->prog, 1, "cSrc1x", c->t_src);
  tex_unit(c->prog, 2, "c3d", tex3d ? tex3d : c->t_stage); /* a bound texture even when unused */
  tex_unit(c->prog, 3, "cSmaa", tex3d && smaa ? smaa : c->t_none);
  gpu_gl_uniforms(&u);
  GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  GPU_PASS(GPU_PASS_C, 0);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  GLenum e = GL(GetError)();
  return e == GL_NO_ERROR ? 0 : -(int)e;
}

unsigned g07_frame_tex(const G07 *c) { return c->mtl != NULL ? gpu_mtl_c_frame_tex(c->mtl) : c->t_out; }
double g07_gpu_ms(const G07 *c) { return c->mtl != NULL ? gpu_mtl_c_gpu_ms(c->mtl) : 0; }

void g07_read_frame(G07 *c, uint8_t *rgba) {
  if (c->mtl != NULL) {
    gpu_mtl_c_read(c->mtl, rgba);
    return;
  }
  GL(PixelStorei)(GL_PACK_ALIGNMENT, 4);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo);
  GL(ReadBuffer)(GL_COLOR_ATTACHMENT0);
  GL(ReadPixels)(0, 0, c->w, c->h, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
}
