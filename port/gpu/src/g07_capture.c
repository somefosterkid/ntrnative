/* port/gpu: display capture at N x, the CPU side (include/g07_capture.h, shaders/g07_capture.frag; docs/gpu.md "Display
 * capture"). GL here; a Metal one (g07cap_create_metal) hands its steps to gpu_mtl.m (src/g07_capture_mtl.inc). */
#include "g07_capture.h"
#include "gpu_gl.h"
#include "gpu_mtl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct G07Cap {
  int scale;
  GLuint prog, prog_show, vao, fbo, fbo_read;
  GLuint t_bank[4]; /* 256 N x 256 N R16UI, made on first use */
  GLuint t_1x;      /* 256 x 256 R16UI: source B's bank as the game left it */
  GLuint t_scratch; /* 256 N x 192 N R16UI: the capture before it is copied into its bank */
  GLuint t_none;    /* 1 x 1 RGBA8UI: k3d when there is no 3D layer */
  GpuMtlK *mtl;     /* g07cap_create_metal's: the GL fields are unused */
};

static GLuint make_tex(GLenum ifmt, GLenum fmt, GLenum type, int w, int h) {
  GLuint t;
  GL(GenTextures)(1, &t);
  GL(BindTexture)(GL_TEXTURE_2D, t);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, ifmt, w, h, 0, fmt, type, NULL);
  return t;
}

G07Cap *g07cap_create(int scale, char *err, size_t errlen) {
  if (scale < 2 || scale > GPU_MAX_SCALE) {
    snprintf(err, errlen, "capture scale %d: 2..%d", scale, GPU_MAX_SCALE);
    return NULL;
  }
  G07Cap *k = calloc(1, sizeof *k);
  if (k == NULL) {
    snprintf(err, errlen, "out of memory");
    return NULL;
  }
  k->scale = scale;
  k->prog = gpu_gl_program("g00_full.vert", "g07_capture.frag", err, errlen);
  k->prog_show = k->prog ? gpu_gl_program("g00_full.vert", "g07_capture.frag#1+K_SHOW", err, errlen) : 0;
  if (!k->prog || !k->prog_show) {
    g07cap_destroy(k);
    return NULL;
  }
  GL(GenVertexArrays)(1, &k->vao);
  GL(GenFramebuffers)(1, &k->fbo);
  GL(GenFramebuffers)(1, &k->fbo_read);
  k->t_1x = make_tex(GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT, 256, 256);
  k->t_scratch = make_tex(GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT, 256 * scale, 192 * scale);
  k->t_none = make_tex(GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, 1, 1);
  return k;
}

G07Cap *g07cap_create_metal(int scale, char *err, size_t errlen) {
  if (scale < 2 || scale > GPU_MAX_SCALE) {
    snprintf(err, errlen, "capture scale %d: 2..%d", scale, GPU_MAX_SCALE);
    return NULL;
  }
  G07Cap *k = calloc(1, sizeof *k);
  if (k == NULL || (k->mtl = gpu_mtl_k_create(scale, err, errlen)) == NULL) {
    if (k == NULL) snprintf(err, errlen, "out of memory");
    free(k);
    return NULL;
  }
  k->scale = scale;
  return k;
}

void g07cap_destroy(G07Cap *k) {
  if (k == NULL) return;
  if (k->mtl != NULL) {
    gpu_mtl_k_destroy(k->mtl);
    free(k);
    return;
  }
  gpu_gl_program_done(k->prog);
  gpu_gl_program_done(k->prog_show);
  GLuint tex[] = {k->t_bank[0], k->t_bank[1], k->t_bank[2], k->t_bank[3], k->t_1x, k->t_scratch, k->t_none};
  for (unsigned i = 0; i < sizeof tex / sizeof tex[0]; i++)
    if (tex[i]) GL(DeleteTextures)(1, &tex[i]);
  if (k->fbo) GL(DeleteFramebuffers)(1, &k->fbo);
  if (k->fbo_read) GL(DeleteFramebuffers)(1, &k->fbo_read);
  if (k->vao) GL(DeleteVertexArrays)(1, &k->vao);
  free(k);
}

int g07cap_scale(const G07Cap *k) { return k->scale; }

static GLuint bank_tex(G07Cap *k, int b) {
  if (k->t_bank[b] == 0) k->t_bank[b] = make_tex(GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT, 256 * k->scale, 256 * k->scale);
  return k->t_bank[b];
}

static void tex_unit(GLuint prog, int unit, const char *name, GLuint tex) {
  GL(ActiveTexture)(GL_TEXTURE0 + unit);
  GL(BindTexture)(GL_TEXTURE_2D, tex);
  GL(Uniform1i)(gpu_gl_uniform(prog, name), unit);
}

static int draw_into(G07Cap *k, GLuint tex, int y0, int w, int h) {
  GLenum buf = GL_COLOR_ATTACHMENT0;
  GL(BindFramebuffer)(GL_FRAMEBUFFER, k->fbo);
  GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
  GL(DrawBuffers)(1, &buf);
  GL(Disable)(GL_DEPTH_TEST);
  GL(Disable)(GL_BLEND);
  GL(Disable)(GL_SCISSOR_TEST);
  GL(Viewport)(0, y0, w, h);
  GL(BindVertexArray)(k->vao);
  GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  GLenum e = GL(GetError)();
  return e == GL_NO_ERROR ? 0 : -(int)e;
}

int g07cap_capture(G07Cap *k, uint32_t cnt, uint32_t dispcnt, unsigned texA, unsigned tex3d, int sc3d, const uint8_t *bank1x,
                   const uint32_t rowsB[8]) {
  static const int kH[4] = {128, 64, 128, 192};
  const int N = k->scale, size = (cnt >> 20) & 3, h = kH[size];
  const int bank = (cnt >> 16) & 3, bankB = (dispcnt >> 18) & 3;
  const int woff = (int)((cnt >> 18) & 3) * 64, roff = ((dispcnt >> 16) & 3) == 2 ? 0 : (int)((cnt >> 26) & 3) * 64;
  if (size == 0) return -1; /* 128 x 128: the caller keeps those at 1x */
  if (k->mtl != NULL) {
    GpuK u = {.n = N, .sc3d = tex3d ? sc3d : 0, .cnt = cnt, .roff = roff, .haveB = bank1x != NULL};
    memcpy(u.rows, rowsB, sizeof u.rows);
    return gpu_mtl_k_capture(k->mtl, &u, bank, bankB, texA, tex3d, bank1x, h, woff);
  }
  GLuint dst = bank_tex(k, bank), srcB = bank_tex(k, bankB);
  if (bank1x != NULL) {
    GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 2);
    GL(BindTexture)(GL_TEXTURE_2D, k->t_1x);
    GL(TexSubImage2D)(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RED_INTEGER, GL_UNSIGNED_SHORT, bank1x);
  }
  GL(UseProgram)(k->prog);
  tex_unit(k->prog, 0, "kSrcA", texA);
  tex_unit(k->prog, 1, "k3d", tex3d ? tex3d : k->t_none);
  tex_unit(k->prog, 2, "kBank", srcB);
  tex_unit(k->prog, 3, "kBank1x", k->t_1x);
  GL(Uniform1uiv)(gpu_gl_uniform(k->prog, "kRows"), 8, rowsB);
  GL(Uniform1i)(gpu_gl_uniform(k->prog, "kN"), N);
  GL(Uniform1i)(gpu_gl_uniform(k->prog, "k3dScale"), tex3d ? sc3d : 0);
  GL(Uniform1ui)(gpu_gl_uniform(k->prog, "kCnt"), cnt);
  GL(Uniform1i)(gpu_gl_uniform(k->prog, "kRoff"), roff);
  GL(Uniform1ui)(gpu_gl_uniform(k->prog, "kHaveB"), bank1x != NULL);
  int rc = draw_into(k, k->t_scratch, 0, 256 * N, h * N);
  if (rc != 0) return rc;
  /* into the bank's rows woff .. woff + h - 1, wrapping at 256 */
  GL(BindFramebuffer)(GL_READ_FRAMEBUFFER, k->fbo_read);
  GL(FramebufferTexture2D)(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, k->t_scratch, 0);
  GL(ReadBuffer)(GL_COLOR_ATTACHMENT0);
  GL(BindFramebuffer)(GL_DRAW_FRAMEBUFFER, k->fbo);
  GL(FramebufferTexture2D)(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst, 0);
  GLenum buf = GL_COLOR_ATTACHMENT0;
  GL(DrawBuffers)(1, &buf);
  int first = h < 256 - woff ? h : 256 - woff; /* rows before the wrap */
  GL(BlitFramebuffer)(0, 0, 256 * N, first * N, 0, woff * N, 256 * N, (woff + first) * N, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  if (first < h)
    GL(BlitFramebuffer)(0, first * N, 256 * N, h * N, 0, 0, 256 * N, (h - first) * N, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  GL(BindFramebuffer)(GL_READ_FRAMEBUFFER, 0);
  GL(BindFramebuffer)(GL_DRAW_FRAMEBUFFER, 0);
  GLenum e = GL(GetError)();
  return e == GL_NO_ERROR ? 0 : -(int)e;
}

int g07cap_show(G07Cap *k, unsigned out, int atop, int bank, unsigned bright, const uint32_t rows[8]) {
  const int N = k->scale, y0 = atop ? 0 : 192 * N;
  if (k->mtl != NULL) {
    GpuK u = {.n = N, .bright = bright, .y0 = y0};
    memcpy(u.rows, rows, sizeof u.rows);
    return gpu_mtl_k_show(k->mtl, out, &u, bank);
  }
  GL(UseProgram)(k->prog_show);
  tex_unit(k->prog_show, 0, "kBank", bank_tex(k, bank));
  GL(Uniform1uiv)(gpu_gl_uniform(k->prog_show, "kRows"), 8, rows);
  GL(Uniform1i)(gpu_gl_uniform(k->prog_show, "kN"), N);
  GL(Uniform1ui)(gpu_gl_uniform(k->prog_show, "kBright"), bright);
  GL(Uniform1i)(gpu_gl_uniform(k->prog_show, "kY0"), y0);
  return draw_into(k, out, y0, 256 * N, 192 * N);
}
