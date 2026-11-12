/* port/gpu: SMAA on the 3D layer at N (Antialiasing=SMAA; docs/gpu.md "Antialiasing: SMAA"). Two full-screen draws
 * between pass P and pass C: the edges from pass R's aux (shaders/g08_smaa_edge.frag) and the blending weights
 * (g08_smaa_weight.frag); pass C does the neighbourhood blending where it reads the 3D layer (g08_smaa.glsl's smaa_blend;
 * 2026-11-02, it was a third pass into a second 3D layer), and g08_read does it into a layer of its own for the readbacks.
 * The weight and blending passes and the two precomputed textures (g08_smaa_area.inc, g08_smaa_search.inc) come from
 * the SMAA reference implementation, github.com/iryoku/smaa (SMAA.hlsl, Textures/AreaTex.h, SearchTex.h), under this
 * licence:
 *
 * Copyright (C) 2013 Jorge Jimenez (jorge@iryoku.com)
 * Copyright (C) 2013 Jose I. Echevarria (joseignacioechevarria@gmail.com)
 * Copyright (C) 2013 Belen Masia (bmasia@unizar.es)
 * Copyright (C) 2013 Fernando Navarro (fernandn@microsoft.com)
 * Copyright (C) 2013 Diego Gutierrez (diegog@unizar.es)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software. As clarification, there is no requirement that the copyright notice and permission be included in binary
 * distributions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
#include "g08_smaa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned char kArea[160 * 560 * 2] = {
#include "g08_smaa_area.inc"
};
static const unsigned char kSearch[64 * 16] = {
#include "g08_smaa_search.inc"
};

struct G08 {
  int w, h;
  GLuint prog[2], tex[4], fbo[2]; /* tex: edges, weights, area, search */
};

static GLuint tex2d(GLenum ifmt, GLenum fmt, GLenum type, int w, int h, GLenum filter, const void *data) {
  GLuint t;
  GL(GenTextures)(1, &t);
  GL(BindTexture)(GL_TEXTURE_2D, t);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)filter);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)filter);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 1);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, (GLint)ifmt, w, h, 0, fmt, type, data);
  GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
  return t;
}

/* a framebuffer with t as its colour; 0 when incomplete */
static GLuint target_fbo(GLuint t) {
  GLuint f;
  GL(GenFramebuffers)(1, &f);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, f);
  GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0);
  GLenum st = GL(CheckFramebufferStatus)(GL_FRAMEBUFFER);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  if (st == GL_FRAMEBUFFER_COMPLETE) return f;
  GL(DeleteFramebuffers)(1, &f);
  return 0;
}

G08 *g08_create(int w, int h, char *err, size_t errlen) {
  G08 *a = calloc(1, sizeof *a);
  if (a == NULL) {
    snprintf(err, errlen, "out of memory");
    return NULL;
  }
  a->w = w;
  a->h = h;
  static const char *const fs[2] = {"g08_smaa_edge.frag", "g08_smaa_weight.frag"};
  for (int i = 0; i < 2; i++)
    if ((a->prog[i] = gpu_gl_program("g00_full.vert", fs[i], err, errlen)) == 0) {
      g08_destroy(a);
      return NULL;
    }
  a->tex[0] = tex2d(GL_RG8, GL_RG, GL_UNSIGNED_BYTE, w, h, GL_LINEAR, NULL);
  a->tex[1] = tex2d(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, w, h, GL_LINEAR, NULL);
  a->tex[2] = tex2d(GL_RG8, GL_RG, GL_UNSIGNED_BYTE, 160, 560, GL_LINEAR, kArea);
  a->tex[3] = tex2d(GL_R8, GL_RED, GL_UNSIGNED_BYTE, 64, 16, GL_LINEAR, kSearch);
  for (int i = 0; i < 2; i++)
    if ((a->fbo[i] = target_fbo(a->tex[i])) == 0) {
      snprintf(err, errlen, "SMAA framebuffer %d incomplete", i);
      g08_destroy(a);
      return NULL;
    }
  /* the uniforms that never change, once (the programs are the context's: a new G08 sets them again) */
  GL(UseProgram)(a->prog[0]);
  GL(Uniform1i)(gpu_gl_uniform(a->prog[0], "uSamp"), 7);
  GL(Uniform1i)(gpu_gl_uniform(a->prog[0], "aLayer"), 0);
  GL(UseProgram)(a->prog[1]);
  GL(Uniform1i)(gpu_gl_uniform(a->prog[1], "aEdges"), 0);
  GL(Uniform1i)(gpu_gl_uniform(a->prog[1], "aArea"), 1);
  GL(Uniform1i)(gpu_gl_uniform(a->prog[1], "aSearch"), 2);
  GL(UseProgram)(0);
  return a;
}

void g08_destroy(G08 *a) {
  if (a == NULL) return;
  for (int i = 0; i < 2; i++) gpu_gl_program_done(a->prog[i]);
  for (int i = 0; i < 4; i++)
    if (a->tex[i]) GL(DeleteTextures)(1, &a->tex[i]);
  for (int i = 0; i < 2; i++)
    if (a->fbo[i]) GL(DeleteFramebuffers)(1, &a->fbo[i]);
  free(a);
}

unsigned g08_weights(const G08 *a) { return a->tex[1]; }

static void unit(int u, GLuint tex) {
  GL(ActiveTexture)(GL_TEXTURE0 + (GLenum)u);
  GL(BindTexture)(GL_TEXTURE_2D, tex);
}

/* pass R's samples on unit 7 (as pass P reads them), layer: pass P's 3D layer; the vertex array bound */
void g08_uni(GpuU *u, int w, int h) {
  const char *e = getenv("BWPORT_SMAA_LUMA"); /* 0..1; 0.05 (the reference's MEDIUM has 0.1 for luma alone: the aux decides here, and 0.1 lost the battle platforms' rims) */
  memset(u, 0, sizeof *u);
  u->aLuma = e != NULL ? (float)atof(e) : 0.05f;
  u->aMetrics[0] = 1.0f / (float)w;
  u->aMetrics[1] = 1.0f / (float)h;
  u->aMetrics[2] = (float)w;
  u->aMetrics[3] = (float)h;
}

void g08_tables(const unsigned char **area, const unsigned char **search) {
  *area = kArea;
  *search = kSearch;
}

void g08_run(G08 *a, GLuint layer) {
  GpuU u;
  g08_uni(&u, a->w, a->h);
  gpu_gl_uniforms(&u);
  GL(Viewport)(0, 0, a->w, a->h);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, a->fbo[0]);
  gpu_gl_invalidate(1);
  GL(UseProgram)(a->prog[0]);
  unit(0, layer);
  GL(DrawArrays)(GL_TRIANGLES, 0, 3);

  GL(BindFramebuffer)(GL_FRAMEBUFFER, a->fbo[1]);
  gpu_gl_invalidate(1);
  GL(UseProgram)(a->prog[1]);
  unit(0, a->tex[0]);
  unit(1, a->tex[2]);
  unit(2, a->tex[3]);
  GL(DrawArrays)(GL_TRIANGLES, 0, 3);
}

int g08_read(G08 *a, GLuint layer, GLuint vao, uint32_t *rgba) {
  char err[512];
  GLuint prog = gpu_gl_program("g00_full.vert", "g08_smaa_blend.frag", err, sizeof err);
  if (prog == 0) return -1;
  GLuint t = tex2d(GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, a->w, a->h, GL_NEAREST, NULL), f = target_fbo(t);
  if (f != 0) {
    GLenum buf = GL_COLOR_ATTACHMENT0;
    GL(BindFramebuffer)(GL_FRAMEBUFFER, f);
    GL(DrawBuffers)(1, &buf);
    GL(Viewport)(0, 0, a->w, a->h);
    GL(UseProgram)(prog);
    GL(BindVertexArray)(vao);
    unit(0, layer);
    unit(1, a->tex[1]);
    GL(Uniform1i)(gpu_gl_uniform(prog, "aLayer"), 0);
    GL(Uniform1i)(gpu_gl_uniform(prog, "aWeights"), 1);
    GL(DrawArrays)(GL_TRIANGLES, 0, 3);
    GL(PixelStorei)(GL_PACK_ALIGNMENT, 4);
    GL(ReadBuffer)(GL_COLOR_ATTACHMENT0);
    GL(ReadPixels)(0, 0, a->w, a->h, GL_RGBA_INTEGER, GL_UNSIGNED_INT, rgba);
    GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
    GL(DeleteFramebuffers)(1, &f);
  }
  GL(DeleteTextures)(1, &t);
  return f != 0 ? 0 : -1;
}
