/* port/gpu: the headless context's private declarations (gpu_egl.c, gpu_sdl.c). G1. */
#ifndef GPU_CTX_H
#define GPU_CTX_H

#include "gpu.h"

enum { GPU_CTX_NONE = 0, GPU_CTX_EGL = 1, GPU_CTX_SDL = 2, GPU_CTX_EAGL = 3 };

struct GpuCtx {
  int kind;
  void *dpy, *ctx, *surf; /* EGLDisplay, EGLContext, EGLSurface (a pbuffer, or none) */
  void *win;              /* SDL_Window, SDL_GLContext in ctx */
  char desc[200];
};

/* each fills c (kind, desc) and makes the context current on this thread; 0 on success, else the reason in err.
 * Without EGL (or SDL) compiled in they fail with "not built in". */
int gpu_ctx_egl_open(struct GpuCtx *c, int api, char *err, size_t errlen);
void gpu_ctx_egl_close(struct GpuCtx *c);
GpuProc gpu_ctx_egl_getproc(const char *name);
int gpu_ctx_sdl_open(struct GpuCtx *c, int api, char *err, size_t errlen);
void gpu_ctx_sdl_close(struct GpuCtx *c);
GpuProc gpu_ctx_sdl_getproc(const char *name);
/* iOS (the Simulator's tools; GPU_HAVE_EAGL, gpu_eagl.m): EAGL's GLES 3.0 without a window, tried first there */
int gpu_ctx_eagl_open(struct GpuCtx *c, int api, char *err, size_t errlen);
void gpu_ctx_eagl_close(struct GpuCtx *c);
GpuProc gpu_ctx_eagl_getproc(const char *name);

#endif
