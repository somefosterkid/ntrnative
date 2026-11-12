/* port/gpu: the headless context's renderer and limits, and whether the shaders build (docs/gpu.md). SKELETON + G1.
 *   gpu_info [--api es|gl] [--scale N]        $GPU_API, $GPU_CTX=egl|sdl, $GPU_EGL=surfaceless|device|default as in
 *                                              gpu_render (src/gpu_egl.c)
 * Exit 0 when the context, the function table and the shaders all work. "fast 3D": whether Renderer3D=fast can draw on it
 * (docs/gpu-hwraster.md, "GL"), how, or why not. */
#include "gpu_gl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GLint geti(GLenum e) {
  GLint v = 0;
  GL(GetIntegerv)(e, &v);
  return v;
}

int main(int argc, char **argv) {
  int api = GPU_API_ES, scale = 1;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--api") && i + 1 < argc) api = !strcmp(argv[++i], "gl") ? GPU_API_GL : GPU_API_ES;
    else if (!strcmp(argv[i], "--scale") && i + 1 < argc) scale = atoi(argv[++i]);
    else {
      fprintf(stderr, "usage: gpu_info [--api es|gl] [--scale N]\n");
      return 2;
    }
  }
  char err[2048];
  GpuCtx *c = gpu_ctx_headless(api, err, sizeof err);
  if (c == NULL) {
    printf("gpu_info: %s\n", err);
    return 1;
  }
  printf("context   %s\n", gpu_ctx_desc(c));
  GpuCore *g = gpu_create(gpu_ctx_getproc(c), api, scale, 1, err, sizeof err);
  if (g == NULL) {
    printf("shaders   FAILED: %s\n", err);
    gpu_ctx_destroy(c);
    return 1;
  }
  printf("api       %s (GLSL header %s)\n", gpu_gl.api == GPU_API_ES ? "OpenGL ES 3.0" : "OpenGL 3.3 core",
         gpu_gl.api == GPU_API_ES ? "#version 300 es" : "#version 330 core");
  printf("renderer  %s\nversion   %s\nGLSL      %s\n", (const char *)GL(GetString)(GL_RENDERER),
         (const char *)GL(GetString)(GL_VERSION), (const char *)GL(GetString)(GL_SHADING_LANGUAGE_VERSION));
  printf("limits    max texture %d, draw buffers %d, colour attachments %d, texture units %d, uniform block %d\n",
         geti(GL_MAX_TEXTURE_SIZE), geti(GL_MAX_DRAW_BUFFERS), geti(GL_MAX_COLOR_ATTACHMENTS),
         geti(GL_MAX_TEXTURE_IMAGE_UNITS), geti(GL_MAX_UNIFORM_BLOCK_SIZE));
  int need = 256 * scale > 192 * scale ? 256 * scale : 192 * scale;
  if (need > geti(GL_MAX_TEXTURE_SIZE) || geti(GL_MAX_DRAW_BUFFERS) < 4)
    printf("WARNING   scale %d needs a %d texture and 4 draw buffers\n", scale, need);
  if (gpu_gl_errors("after gpu_create", err, sizeof err)) printf("GL errors %s\n", err);
  printf("shaders   OK (scale %d)\n", gpu_scale(g));
  int fast = gpu_fast_supported(g); /* the fast 3D renderer: its programs and the edge-rule probe (src/g10_fast_gl.c) */
  printf("fast 3D   %s: %s\n", fast ? "yes" : "no", gpu_fast_how(g));
  if (gpu_gl_errors("after the fast 3D renderer's setup", err, sizeof err)) printf("GL errors %s\n", err);
  gpu_destroy(g);
  gpu_ctx_destroy(c);
  return 0;
}
