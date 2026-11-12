/* port/gpu piece G10 on GL: the fast 3D renderer's pass (docs/gpu-hwraster.md, "GL"), in pass R's place; g10_fast_mtl.inc's pass
 * for GL and GLES. The fragment runs pass R's per-pixel rules on the pixel's state as it is when the fragment runs, which GL gives
 * in one of two ways, chosen at the first use from the context's extensions:
 *   fetch   EXT_shader_framebuffer_fetch (GLES: Apple, Adreno, Mali, PowerVR; desktop: Mesa): the output's old value (inout),
 *           the program "g10_fast.frag#1+G10_FETCH" into the target as a colour attachment, as Metal's programmable blending;
 *   ilock   desktop GL 4.5 with ARB_fragment_shader_interlock (NVIDIA, Intel, Mesa), or iorder with INTEL_fragment_shader_ordering:
 *           the state in an image (load/store), the fragments of one pixel in primitive order; a framebuffer without attachments.
 * Neither (macOS's GL 4.1, a GLES without the extension, AMD's desktop GL without the INTEL extension): the exact renderer draws.
 * The DS fills the top row and the left column of a polygon, not its bottom row or a vertical right edge (the vertices sit on
 * pixel centres: g10_fast.c), which is the top-left rule in DS rows; GL leaves the rule on an edge through a pixel centre to the
 * implementation, so a probe draw at the first use checks it (GL's row 0 is DS row 0 here) and the fast path stays off when it
 * differs. With 4 samples, as on Metal, one fragment per output pixel draws its four samples (g10_fast4.vert/.frag, g10_clear4.frag:
 * the coverage tested in the fragment in integers on the 2N grid, so the tie rule is its own; the vertices from a data texture by
 * gl_VertexID) into the four layers of a target of its own, and g10_pack.frag packs them into pass R's four layers (Metal draws
 * into pass R's layers directly since 2026-11-04, g10_fast4.frag's G10_DIRECT; GLES 3.0 promises four colour attachments only; the probe is
 * for AA off only). Pass E, pass P and SMAA then run as for the exact renderer. */
#include "g00_frame.h"
#include "g04_texture.h"
#include "g06_post.h"
#include "gpu_gl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef GL_READ_WRITE
#define GL_READ_WRITE 0x88BA
#endif
#ifndef GL_FRAMEBUFFER_DEFAULT_WIDTH
#define GL_FRAMEBUFFER_DEFAULT_WIDTH 0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT 0x9311
#endif
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT_ 0x20
#define GL_TEXTURE_FETCH_BARRIER_BIT_ 0x08
#define GL_FRAMEBUFFER_BARRIER_BIT_ 0x400

enum { FAST_NONE, FAST_FETCH, FAST_ILOCK, FAST_IORDER };
enum { FAST_E_UNIT = 6 }; /* the edge functions' texture (pass R's units: 0-5 its tables and textures, 7 the samples, 8 G6's) */

struct GpuFastGL {
  int ok;   /* 1: made; -1: it cannot draw (why) */
  int mode; /* FAST_* */
  GLuint clr, tri, trif, pack; /* trif: Filter=bilinear, made at its first use */
  GLuint vao, vbo[GPU_RING], t_pe[GPU_RING];
  GLuint t_v[GPU_RING];  /* k = 2: the vertices as a data texture (g10_fast4.vert reads a triangle's three) */
  GLuint grid, fbo_grid; /* k = 2: the four samples (RGBA32UI, a 2D array of 4 layers at w x h) and its framebuffer */
  GLuint fbo_img;        /* the image modes: a framebuffer without attachments at the target's size */
};

static char sWhy[200] = "not tried"; /* gpu_fast_how's text */

static int has_ext(const char *name) {
  GLint n = 0;
  GL(GetIntegerv)(GL_NUM_EXTENSIONS, &n);
  for (GLint i = 0; i < n; i++) {
    const char *x = (const char *)GL(GetStringi)(GL_EXTENSIONS, (GLuint)i);
    if (x != NULL && !strcmp(x, name)) return 1;
  }
  return 0;
}

/* the way this context can read a pixel's state in the fragment (FAST_*), the reason in sWhy when none */
static int fast_mode(void) {
  if (has_ext("GL_EXT_shader_framebuffer_fetch")) return FAST_FETCH;
  if (gpu_gl.api == GPU_API_ES) {
    snprintf(sWhy, sizeof sWhy, "this GLES has no EXT_shader_framebuffer_fetch");
    return FAST_NONE;
  }
  int maj = 0, min = 0;
  const char *ver = (const char *)GL(GetString)(GL_VERSION);
  if (ver == NULL || sscanf(ver, "%d.%d", &maj, &min) != 2 || maj * 10 + min < 45) {
    snprintf(sWhy, sizeof sWhy, "OpenGL %s has neither framebuffer fetch nor 4.5 with images and fragment interlock", ver ? ver : "?");
    return FAST_NONE;
  }
  if (gpu_gl.BindImageTexture == NULL || gpu_gl.MemoryBarrier == NULL || gpu_gl.FramebufferParameteri == NULL) {
    snprintf(sWhy, sizeof sWhy, "OpenGL %s: glBindImageTexture, glMemoryBarrier or glFramebufferParameteri missing", ver);
    return FAST_NONE;
  }
  if (has_ext("GL_ARB_fragment_shader_interlock")) return FAST_ILOCK;
  if (has_ext("GL_INTEL_fragment_shader_ordering")) return FAST_IORDER;
  snprintf(sWhy, sizeof sWhy, "this OpenGL has neither framebuffer fetch nor fragment interlock (ARB or INTEL ordering)");
  return FAST_NONE;
}

static const char *const kMode[] = {"none", "framebuffer fetch", "fragment interlock", "fragment ordering"};
static const char *const kTri[] = {"", "g10_fast.frag#1+G10_FETCH", "g10_fast.frag#1+G10_ILOCK", "g10_fast.frag#1+G10_IORDER"};
static const char *const kTriF[] = {"", "g10_fast.frag#1f+G10_FETCH", "g10_fast.frag#1f+G10_ILOCK", "g10_fast.frag#1f+G10_IORDER"};
static const char *const kTri4[] = {"", "g10_fast4.frag#1+G10_FETCH", "g10_fast4.frag#1+G10_ILOCK", "g10_fast4.frag#1+G10_IORDER"};
static const char *const kTriF4[] = {"", "g10_fast4.frag#1f+G10_FETCH", "g10_fast4.frag#1f+G10_ILOCK", "g10_fast4.frag#1f+G10_IORDER"};
enum { FAST_V_UNIT = 9 }; /* k = 2: the vertices' texture */

static GLuint fast_target(GLenum ifmt, int w, int h) {
  GLuint t;
  GL(GenTextures)(1, &t);
  GL(BindTexture)(GL_TEXTURE_2D, t);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  GL(TexStorage2D)(GL_TEXTURE_2D, 1, ifmt, w, h);
  GL(BindTexture)(GL_TEXTURE_2D, 0);
  return t;
}

static void fast_vao(GLuint vao, GLuint vbo) {
  GL(BindVertexArray)(vao);
  GL(BindBuffer)(GL_ARRAY_BUFFER, vbo);
  for (int i = 0; i < 3; i++) {
    GL(EnableVertexAttribArray)((GLuint)i);
    GL(VertexAttribPointer)((GLuint)i, 4, GL_FLOAT, GL_FALSE, GPU_FAST_V * 4, (const void *)(size_t)(i * 16));
  }
}

/* the probe: a square and a triangle with their corners on pixel centres (g10_fast.c's +0.5) into a 16 x 8 target; the DS
 * (and Metal's top-left rule in DS rows) fills the square's pixels x, y in 1..4, the triangle's x + y < 14 with x >= 9, y >= 1
 * (its corners (9, 1), (13, 1), (9, 5): the slanted edge is a right edge). 0 when the rasterizer agrees */
static int fast_probe(struct GpuFastGL *f, GLuint prog, char *got, size_t gotlen) {
  static const float c[9][2] = {{1, 1}, {5, 1}, {5, 5}, {1, 1}, {5, 5}, {1, 5}, {9, 1}, {13, 1}, {9, 5}};
  float v[9 * GPU_FAST_V];
  memset(v, 0, sizeof v);
  for (int i = 0; i < 9; i++) v[i * GPU_FAST_V] = c[i][0] + 0.5f, v[i * GPU_FAST_V + 1] = c[i][1] + 0.5f, v[i * GPU_FAST_V + 3] = 1;
  GLuint t = fast_target(GL_RGBA32UI, 16, 8), fb;
  GL(GenFramebuffers)(1, &fb);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, fb);
  GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0);
  GLenum b0 = GL_COLOR_ATTACHMENT0;
  GL(DrawBuffers)(1, &b0);
  static const GLuint zero[4] = {0, 0, 0, 0};
  GL(ClearBufferuiv)(GL_COLOR, 0, zero);
  GL(Viewport)(0, 0, 16, 8);
  GL(Disable)(GL_DEPTH_TEST);
  GL(Disable)(GL_BLEND);
  GL(UseProgram)(prog);
  GL(Uniform2f)(gpu_gl_uniform(prog, "uGrid"), 16.0f, 8.0f);
  GpuU u;
  memset(&u, 0, sizeof u);
  u.uScale = 1;
  gpu_gl_uniforms(&u);
  fast_vao(f->vao, f->vbo[0]);
  GL(BufferData)(GL_ARRAY_BUFFER, sizeof v, v, GL_STREAM_DRAW);
  GL(DrawArrays)(GL_TRIANGLES, 0, 9);
  uint32_t px[16 * 8 * 4];
  GL(PixelStorei)(GL_PACK_ALIGNMENT, 4);
  GL(ReadBuffer)(GL_COLOR_ATTACHMENT0);
  GL(ReadPixels)(0, 0, 16, 8, GL_RGBA_INTEGER, GL_UNSIGNED_INT, px);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  GL(DeleteFramebuffers)(1, &fb);
  GL(DeleteTextures)(1, &t);
  int bad = 0;
  size_t l = 0;
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 16; x++) {
      int want = x < 8 ? x >= 1 && x <= 4 && y >= 1 && y <= 4 : x >= 9 && y >= 1 && x + y < 14;
      int have = px[(y * 16 + x) * 4 + 3] != 0;
      bad += want != have;
      if (l + 2 < gotlen) got[l++] = have ? '#' : '.';
    }
    if (l + 2 < gotlen) got[l++] = '/';
  }
  got[l] = 0;
  return bad;
}

static void fast_fail(struct GpuFastGL *f, const char *why) {
  f->ok = -1;
  snprintf(sWhy, sizeof sWhy, "%s", why);
  fprintf(stderr, "gpu: the fast 3D renderer on GL: %s; the exact renderer draws\n", why);
}

int gpu_gl_fast_ok(GpuCore *g) {
  if (g->mtl != NULL) return 0;
  if (g->fgl != NULL) return g->fgl->ok > 0;
  struct GpuFastGL *f = g->fgl = calloc(1, sizeof *f);
  if (f == NULL) return 0;
  f->mode = fast_mode();
  if (f->mode == FAST_NONE) {
    fast_fail(f, sWhy);
    return 0;
  }
  char err[1024] = "";
  const int k2 = g->k == 2;
  f->clr = gpu_gl_program("g00_full.vert", k2 ? "g10_clear4.frag" : "g10_clear.frag", err, sizeof err);
  if (f->clr) f->tri = gpu_gl_program(k2 ? "g10_fast4.vert" : "g10_fastgl.vert", k2 ? kTri4[f->mode] : kTri[f->mode], err, sizeof err);
  if (f->tri && k2) f->pack = gpu_gl_program("g00_full.vert", "g10_pack.frag", err, sizeof err);
  GLuint probe = f->tri && !k2 ? gpu_gl_program("g10_fastgl.vert", "g10_clear.frag", err, sizeof err) : 0;
  if (f->tri == 0 || (!k2 && probe == 0) || (k2 && f->pack == 0)) {
    char why[200];
    snprintf(why, sizeof why, "%s: %.150s", kMode[f->mode], err);
    fast_fail(f, why);
    return 0;
  }
  GL(GenVertexArrays)(1, &f->vao);
  GL(GenBuffers)(GPU_RING, f->vbo);
  char got[160] = "";
  int bad = k2 ? 0 : fast_probe(f, probe, got, sizeof got);
  if (!k2) gpu_gl_program_done(probe);
  GL(BindVertexArray)(g->vao);
  GL(BindBuffer)(GL_ARRAY_BUFFER, 0);
  if (bad) {
    char why[200];
    snprintf(why, sizeof why, "the rasterizer fills an edge through a pixel centre otherwise than the DS (%d pixels: %s)", bad, got);
    fast_fail(f, why);
    return 0;
  }
  const int tw = g->w, th = g->h; /* the target: one texel per output pixel (4 samples: one layer each) */
  if (k2) {
    GL(GenTextures)(1, &f->grid);
    GL(BindTexture)(GL_TEXTURE_2D_ARRAY, f->grid);
    GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GL(TexImage3D)(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32UI, tw, th, 4, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
    GL(BindTexture)(GL_TEXTURE_2D_ARRAY, 0);
    GL(GenFramebuffers)(1, &f->fbo_grid);
    GL(BindFramebuffer)(GL_FRAMEBUFFER, f->fbo_grid);
    for (int i = 0; i < 4; i++)
      GL(FramebufferTextureLayer)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i, f->grid, 0, i);
    GLenum st = GL(CheckFramebufferStatus)(GL_FRAMEBUFFER);
    GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
    if (st != GL_FRAMEBUFFER_COMPLETE) {
      fast_fail(f, "the 4-sample grid's framebuffer is incomplete");
      return 0;
    }
  }
  if (f->mode != FAST_FETCH) {
    GL(GenFramebuffers)(1, &f->fbo_img);
    GL(BindFramebuffer)(GL_FRAMEBUFFER, f->fbo_img);
    gpu_gl.FramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, tw);
    gpu_gl.FramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, th);
    GLenum st = GL(CheckFramebufferStatus)(GL_FRAMEBUFFER);
    GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
    if (st != GL_FRAMEBUFFER_COMPLETE) {
      fast_fail(f, "the framebuffer without attachments is incomplete");
      return 0;
    }
  }
  char gerr[256];
  if (gpu_gl_errors("the fast 3D renderer's setup", gerr, sizeof gerr)) {
    fast_fail(f, gerr);
    return 0;
  }
  f->ok = 1;
  snprintf(sWhy, sizeof sWhy, "OpenGL%s: %s", gpu_gl.api == GPU_API_ES ? " ES" : "", kMode[f->mode]);
  fprintf(stderr, "gpu: the fast 3D renderer on GL: %s\n", kMode[f->mode]);
  return 1;
}

const char *gpu_gl_fast_how(void) { return sWhy; }

void gpu_gl_fast_release(GpuCore *g) {
  struct GpuFastGL *f = g->fgl;
  if (f == NULL) return;
  gpu_gl_program_done(f->clr);
  gpu_gl_program_done(f->tri);
  if (f->trif != f->tri) gpu_gl_program_done(f->trif);
  gpu_gl_program_done(f->pack);
  for (int i = 0; i < GPU_RING; i++) {
    if (f->t_pe[i]) gpu_gl_data_forget(f->t_pe[i]), GL(DeleteTextures)(1, &f->t_pe[i]);
    if (f->t_v[i]) gpu_gl_data_forget(f->t_v[i]), GL(DeleteTextures)(1, &f->t_v[i]);
  }
  if (f->vbo[0]) GL(DeleteBuffers)(GPU_RING, f->vbo);
  if (f->vao) GL(DeleteVertexArrays)(1, &f->vao);
  if (f->grid) GL(DeleteTextures)(1, &f->grid);
  if (f->fbo_grid) GL(DeleteFramebuffers)(1, &f->fbo_grid);
  if (f->fbo_img) GL(DeleteFramebuffers)(1, &f->fbo_img);
  free(f);
  g->fgl = NULL;
}

/* the triangles' program and its inputs (u: pass R's uniforms, already in the uniform buffer) */
static void tri_bind(GpuCore *g, const VCoreState *s, GLuint pr, int ri) {
  GL(UseProgram)(pr);
  if (g->k == 2) { /* g10_fast4.vert: the 2N grid's size, the vertices */
    GL(Uniform4f)(gpu_gl_uniform(pr, "hu"), (float)(g->w * 2), (float)(g->h * 2), 0.0f, 0.0f);
    GL(ActiveTexture)(GL_TEXTURE0 + FAST_V_UNIT);
    GL(BindTexture)(GL_TEXTURE_2D, g->fgl->t_v[ri]);
    GL(Uniform1i)(gpu_gl_uniform(pr, "uFastV"), FAST_V_UNIT);
  } else {
    GL(Uniform2f)(gpu_gl_uniform(pr, "uGrid"), (float)g->w, (float)g->h);
  }
  GL(ActiveTexture)(GL_TEXTURE0);
  GL(BindTexture)(GL_TEXTURE_2D, g->t_poly[ri]);
  GL(Uniform1i)(gpu_gl_uniform(pr, "uPoly"), 0);
  GL(ActiveTexture)(GL_TEXTURE0 + FAST_E_UNIT);
  GL(BindTexture)(GL_TEXTURE_2D, g->fgl->t_pe[ri]);
  GL(Uniform1i)(gpu_gl_uniform(pr, "uFastE"), FAST_E_UNIT);
  gpu_g04_bind(pr, 4, g->t_tex[g->tr], g->t_pal[g->tr]);
  gpu_g06_bind_raster(pr, s);
}

void gpu_gl_fast_r(GpuCore *g, const VCoreState *s, const GpuPrep *p, int ri) {
  struct GpuFastGL *f = g->fgl;
  const GpuFast *fv = &p->fast;
  GLuint tri = f->tri;
  if (g->opt.filter && g->scale > g->k) { /* Filter=bilinear where it applies: its own program, as pass R's */
    if (f->trif == 0) {
      char err[512];
      f->trif = g->k == 2 ? gpu_gl_program("g10_fast4.vert", kTriF4[f->mode], err, sizeof err)
                          : gpu_gl_program("g10_fastgl.vert", kTriF[f->mode], err, sizeof err);
      if (f->trif == 0) {
        fprintf(stderr, "gpu: %s; textures unfiltered\n", err);
        f->trif = f->tri;
      }
    }
    tri = f->trif;
  }
  const int k2 = g->k == 2, tw = g->w, th = g->h;
  /* the edge functions (a data texture) and the vertices (ring entry ri: never what a frame in flight reads): attributes, or with
   * 4 samples a data texture, 3 texels a vertex */
  GL(ActiveTexture)(GL_TEXTURE0 + FAST_E_UNIT);
  gpu_gl_data_tex(&f->t_pe[ri], GL_RGBA32F, GL_RGBA, GL_FLOAT, 16, p->sp.npoly > 0 ? fv->pe[0] : NULL,
                  p->sp.npoly * VC_POLY_MAXV);
  if (k2) {
    GL(ActiveTexture)(GL_TEXTURE0 + FAST_V_UNIT);
    gpu_gl_data_tex(&f->t_v[ri], GL_RGBA32F, GL_RGBA, GL_FLOAT, 16, fv->ntri > 0 ? fv->tri : NULL, fv->ntri * (GPU_FAST_V / 4));
  } else {
    fast_vao(f->vao, f->vbo[ri]);
    GL(BufferData)(GL_ARRAY_BUFFER, (GLsizeiptr)((fv->ntri > 0 ? fv->ntri : 1) * GPU_FAST_V * sizeof(float)),
                   fv->ntri > 0 ? fv->tri : NULL, GL_STREAM_DRAW);
  }
  GL(BindVertexArray)(g->vao);
  GpuU u;
  gpu_uni_raster(&u, g, s, p);
  gpu_gl_uniforms(&u);
  /* the rear plane into the target (a colour attachment in every mode) */
  static const GLenum b4[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
  GL(BindFramebuffer)(GL_FRAMEBUFFER, k2 ? f->fbo_grid : g->fbo_r);
  GL(DrawBuffers)(k2 ? 4 : 1, b4);
  gpu_gl_invalidate(k2 ? 4 : 1);
  GL(Viewport)(0, 0, tw, th);
  GL(Disable)(GL_DEPTH_TEST);
  GL(Disable)(GL_BLEND);
  GL(UseProgram)(f->clr);
  gpu_g06_bind_raster(f->clr, s);
  GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  /* the triangles */
  if (fv->ntri > 0) {
    if (f->mode != FAST_FETCH) {
      gpu_gl.MemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT_ | GL_FRAMEBUFFER_BARRIER_BIT_);
      GL(BindFramebuffer)(GL_FRAMEBUFFER, f->fbo_img);
      gpu_gl.BindImageTexture(0, k2 ? f->grid : g->t_samp, 0, k2 ? GL_TRUE : GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32UI);
    }
    tri_bind(g, s, tri, ri);
    GL(BindVertexArray)(k2 ? g->vao : f->vao); /* (4 samples: no attributes, the vertices by gl_VertexID) */
    GL(DrawArrays)(GL_TRIANGLES, 0, fv->ntri);
    GL(BindVertexArray)(g->vao);
    if (f->mode != FAST_FETCH) {
      gpu_gl.MemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT_ | GL_FRAMEBUFFER_BARRIER_BIT_ | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT_);
      gpu_gl.BindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32UI);
    }
  }
  if (k2) { /* the four samples' texels into pass R's four layers */
    GL(BindFramebuffer)(GL_FRAMEBUFFER, g->fbo_r);
    GL(DrawBuffers)(4, b4);
    gpu_gl_invalidate(4);
    GL(Viewport)(0, 0, g->w, g->h);
    GL(UseProgram)(f->pack);
    GL(ActiveTexture)(GL_TEXTURE0 + 1);
    GL(BindTexture)(GL_TEXTURE_2D_ARRAY, f->grid);
    GL(Uniform1i)(gpu_gl_uniform(f->pack, "uHw"), 1);
    GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  }
  GL(BindBuffer)(GL_ARRAY_BUFFER, 0);
  GL(ActiveTexture)(GL_TEXTURE0);
  GL(Viewport)(0, 0, g->w, g->h);
}
