/* port/gpu: the headless context through an SDL hidden window, for a host without EGL (docs/gpu.md, G1). The exe's own
 * context is libntr's; this one is for the tools and tests where EGL is missing (win64, macOS). Built when SDL2 is
 * found (GPU_HAVE_SDL). Mesa llvmpipe here reaches it with SDL_VIDEODRIVER=offscreen (GPU_CTX=sdl to try). */
#include "gpu_ctx.h"

#include <stdio.h>

#ifdef GPU_HAVE_SDL
#include <SDL.h>

int gpu_ctx_sdl_open(struct GpuCtx *c, int api, char *err, size_t errlen) {
  int es = api == GPU_API_ES;
  if (!SDL_WasInit(SDL_INIT_VIDEO) && SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
    snprintf(err, errlen, "SDL: video init failed: %s", SDL_GetError());
    return -1;
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, es ? SDL_GL_CONTEXT_PROFILE_ES : SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, es ? 0 : 3);
  SDL_Window *w = SDL_CreateWindow("gpu", 0, 0, 64, 64, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
  if (w == NULL) {
    snprintf(err, errlen, "SDL: hidden window failed: %s", SDL_GetError());
    return -1;
  }
  SDL_GLContext gc = SDL_GL_CreateContext(w);
  if (gc == NULL) {
    snprintf(err, errlen, "SDL: %s context failed: %s", es ? "GLES 3.0" : "GL 3.3 core", SDL_GetError());
    SDL_DestroyWindow(w);
    return -1;
  }
  c->kind = GPU_CTX_SDL;
  c->win = w;
  c->ctx = gc;
  snprintf(c->desc, sizeof c->desc, "SDL hidden window (%s)", SDL_GetCurrentVideoDriver());
  return 0;
}

void gpu_ctx_sdl_close(struct GpuCtx *c) {
  SDL_GL_DeleteContext((SDL_GLContext)c->ctx);
  SDL_DestroyWindow((SDL_Window *)c->win);
}

GpuProc gpu_ctx_sdl_getproc(const char *name) { return (GpuProc)SDL_GL_GetProcAddress(name); }

#else

int gpu_ctx_sdl_open(struct GpuCtx *c, int api, char *err, size_t errlen) {
  snprintf(err, errlen, "SDL: not built in");
  return -1;
}
void gpu_ctx_sdl_close(struct GpuCtx *c) {}
GpuProc gpu_ctx_sdl_getproc(const char *name) { return NULL; }

#endif
