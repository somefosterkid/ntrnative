/* port/gpu: a headless GL context for the tools and tests (docs/gpu.md, "Testing by CPU"). OWNED BY THE SKELETON (+ G1).
 *
 * gpu_ctx_headless tries, in order: EGL on Mesa's surfaceless platform (no X server, no window; llvmpipe here), EGL on
 * a platform device (eglQueryDevicesEXT), EGL on the default display (a pbuffer when the context can't go current
 * without a surface), then an SDL hidden window (gpu_sdl.c) for a host without EGL. The reasons of the failed tries
 * are joined in err.
 *   GPU_API=es|gl   the API of the context (overrides the argument): ES 3.0, or desktop GL 3.3 core (EGL_OPENGL_API)
 *   GPU_CTX=egl|sdl, GPU_EGL=surfaceless|device|default   only that try (for testing the fallbacks) */
#include "gpu_ctx.h"
#include "gpu_gl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef GPU_HAVE_EGL
#include <EGL/egl.h>
#include <EGL/eglext.h>

static const char *egl_error_name(EGLint e) {
  switch (e) {
  case EGL_SUCCESS: return "EGL_SUCCESS";
  case EGL_NOT_INITIALIZED: return "EGL_NOT_INITIALIZED";
  case EGL_BAD_ACCESS: return "EGL_BAD_ACCESS";
  case EGL_BAD_ALLOC: return "EGL_BAD_ALLOC";
  case EGL_BAD_ATTRIBUTE: return "EGL_BAD_ATTRIBUTE";
  case EGL_BAD_CONFIG: return "EGL_BAD_CONFIG";
  case EGL_BAD_CONTEXT: return "EGL_BAD_CONTEXT";
  case EGL_BAD_CURRENT_SURFACE: return "EGL_BAD_CURRENT_SURFACE";
  case EGL_BAD_DISPLAY: return "EGL_BAD_DISPLAY";
  case EGL_BAD_MATCH: return "EGL_BAD_MATCH";
  case EGL_BAD_NATIVE_PIXMAP: return "EGL_BAD_NATIVE_PIXMAP";
  case EGL_BAD_NATIVE_WINDOW: return "EGL_BAD_NATIVE_WINDOW";
  case EGL_BAD_PARAMETER: return "EGL_BAD_PARAMETER";
  case EGL_BAD_SURFACE: return "EGL_BAD_SURFACE";
  case EGL_CONTEXT_LOST: return "EGL_CONTEXT_LOST";
  default: return "EGL error";
  }
}

#define FAIL(...)                                                                                                     \
  do {                                                                                                                \
    snprintf(err, errlen, __VA_ARGS__);                                                                               \
    goto fail;                                                                                                        \
  } while (0)

/* one display, one context: 0 on success (c filled), else the reason */
static int try_display(struct GpuCtx *c, EGLDisplay dpy, const char *how, int es, char *err, size_t errlen) {
  EGLint ma = 0, mi = 0;
  EGLContext ctx = EGL_NO_CONTEXT;
  EGLSurface surf = EGL_NO_SURFACE;
  if (dpy == EGL_NO_DISPLAY) FAIL("%s: no display (%s)", how, egl_error_name(eglGetError()));
  if (!eglInitialize(dpy, &ma, &mi)) FAIL("%s: eglInitialize failed (%s)", how, egl_error_name(eglGetError()));
  if (!eglBindAPI(es ? EGL_OPENGL_ES_API : EGL_OPENGL_API))
    FAIL("%s: eglBindAPI(%s) failed (%s)", how, es ? "ES" : "GL", egl_error_name(eglGetError()));
  EGLConfig cfg = NULL;
  EGLint n = 0;
  int pbuffer_cfg = 1;
  EGLint cattr[] = {EGL_RENDERABLE_TYPE, es ? EGL_OPENGL_ES3_BIT : EGL_OPENGL_BIT, EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                    EGL_NONE};
  if (!eglChooseConfig(dpy, cattr, &cfg, 1, &n) || n < 1) {
    pbuffer_cfg = 0; /* the context may still go current without a surface */
    cattr[2] = EGL_NONE;
    if (!eglChooseConfig(dpy, cattr, &cfg, 1, &n) || n < 1)
      FAIL("%s: no %s 3 config (%s)", how, es ? "GLES" : "GL", egl_error_name(eglGetError()));
  }
  EGLint es_attr[] = {EGL_CONTEXT_MAJOR_VERSION, 3, EGL_CONTEXT_MINOR_VERSION, 0, EGL_NONE};
  EGLint gl_attr[] = {EGL_CONTEXT_MAJOR_VERSION, 3, EGL_CONTEXT_MINOR_VERSION, 3, EGL_CONTEXT_OPENGL_PROFILE_MASK,
                      EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT, EGL_NONE};
  ctx = eglCreateContext(dpy, cfg, EGL_NO_CONTEXT, es ? es_attr : gl_attr);
  if (ctx == EGL_NO_CONTEXT)
    FAIL("%s: no %s context (%s)", how, es ? "GLES 3.0" : "GL 3.3 core", egl_error_name(eglGetError()));
  const char *mode = "surfaceless";
  if (!eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, ctx)) {
    EGLint e1 = eglGetError();
    if (!pbuffer_cfg) FAIL("%s: eglMakeCurrent without a surface failed (%s), no pbuffer config", how, egl_error_name(e1));
    EGLint pattr[] = {EGL_WIDTH, 64, EGL_HEIGHT, 64, EGL_NONE};
    surf = eglCreatePbufferSurface(dpy, cfg, pattr);
    if (surf == EGL_NO_SURFACE || !eglMakeCurrent(dpy, surf, surf, ctx))
      FAIL("%s: eglMakeCurrent failed without a surface (%s) and with a pbuffer (%s)", how, egl_error_name(e1),
           egl_error_name(eglGetError()));
    mode = "pbuffer";
  }
  c->kind = GPU_CTX_EGL;
  c->dpy = dpy;
  c->ctx = ctx;
  c->surf = surf;
  if (surf != EGL_NO_SURFACE) snprintf(c->desc, sizeof c->desc, "EGL %d.%d %s, %s (%s)", ma, mi, how, mode, eglQueryString(dpy, EGL_VENDOR));
  else snprintf(c->desc, sizeof c->desc, "EGL %d.%d %s (%s)", ma, mi, how, eglQueryString(dpy, EGL_VENDOR));
  return 0;
fail:
  if (surf != EGL_NO_SURFACE) eglDestroySurface(dpy, surf);
  if (ctx != EGL_NO_CONTEXT) eglDestroyContext(dpy, ctx);
  if (dpy != EGL_NO_DISPLAY) eglTerminate(dpy);
  return -1;
}

int gpu_ctx_egl_open(struct GpuCtx *c, int api, char *err, size_t errlen) {
  int es = api == GPU_API_ES;
  const char *only = getenv("GPU_EGL");
  char log[600] = "", one[240];
  PFNEGLGETPLATFORMDISPLAYEXTPROC gpd = (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
  const char *exts = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS); /* client extensions (EGL 1.5 / EGL_EXT_client_extensions) */
  if (exts == NULL) exts = "";
  /* 1: Mesa's surfaceless platform */
  if (only == NULL || !strcmp(only, "surfaceless")) {
    if (gpd != NULL && strstr(exts, "EGL_MESA_platform_surfaceless")) {
      if (try_display(c, gpd(EGL_PLATFORM_SURFACELESS_MESA, EGL_DEFAULT_DISPLAY, NULL), "surfaceless", es, one, sizeof one) == 0)
        return 0;
      snprintf(log + strlen(log), sizeof log - strlen(log), "%s; ", one);
    } else {
      snprintf(log + strlen(log), sizeof log - strlen(log), "surfaceless: no EGL_MESA_platform_surfaceless; ");
    }
  }
  /* 2: a platform device (the GPU itself, with no display server) */
  if (only == NULL || !strcmp(only, "device")) {
    PFNEGLQUERYDEVICESEXTPROC qd = (PFNEGLQUERYDEVICESEXTPROC)eglGetProcAddress("eglQueryDevicesEXT");
    EGLDeviceEXT devs[8];
    EGLint nd = 0;
    if (gpd != NULL && qd != NULL && strstr(exts, "EGL_EXT_platform_device") && qd(8, devs, &nd) && nd > 0) {
      for (int i = 0; i < nd; i++) {
        char how[32];
        snprintf(how, sizeof how, "device %d", i);
        if (try_display(c, gpd(EGL_PLATFORM_DEVICE_EXT, devs[i], NULL), how, es, one, sizeof one) == 0) return 0;
        snprintf(log + strlen(log), sizeof log - strlen(log), "%s; ", one);
      }
    } else {
      snprintf(log + strlen(log), sizeof log - strlen(log), "device: none; ");
    }
  }
  /* 3: the default display (X11, Wayland, GBM, whatever EGL picks), with a pbuffer if it must */
  if (only == NULL || !strcmp(only, "default")) {
    if (try_display(c, eglGetDisplay(EGL_DEFAULT_DISPLAY), "default display", es, one, sizeof one) == 0) return 0;
    snprintf(log + strlen(log), sizeof log - strlen(log), "%s; ", one);
  }
  snprintf(err, errlen, "EGL: %s", log);
  return -1;
}

void gpu_ctx_egl_close(struct GpuCtx *c) {
  EGLDisplay d = (EGLDisplay)c->dpy;
  eglMakeCurrent(d, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  if (c->surf != NULL) eglDestroySurface(d, (EGLSurface)c->surf);
  eglDestroyContext(d, (EGLContext)c->ctx);
  eglTerminate(d);
}

GpuProc gpu_ctx_egl_getproc(const char *name) { return (GpuProc)eglGetProcAddress(name); }

#else /* no EGL: the mingw build, iOS */

int gpu_ctx_egl_open(struct GpuCtx *c, int api, char *err, size_t errlen) {
  snprintf(err, errlen, "EGL: not built in");
  return -1;
}
void gpu_ctx_egl_close(struct GpuCtx *c) {}
GpuProc gpu_ctx_egl_getproc(const char *name) { return NULL; }

#endif

/* ---- the public interface: EGL first, then SDL ---- */

GpuCtx *gpu_ctx_headless(int api, char *err, size_t errlen) {
  const char *e = getenv("GPU_API"), *only = getenv("GPU_CTX");
  if (e != NULL && e[0]) api = !strcmp(e, "gl") ? GPU_API_GL : GPU_API_ES;
  GpuCtx *c = calloc(1, sizeof *c);
  if (c == NULL) {
    snprintf(err, errlen, "out of memory");
    return NULL;
  }
  char e1[700] = "", e2[400] = "";
#ifdef GPU_HAVE_EAGL
  if (gpu_ctx_eagl_open(c, api, e2, sizeof e2) == 0) return c;
#endif
  if ((only == NULL || strcmp(only, "sdl")) && gpu_ctx_egl_open(c, api, e1, sizeof e1) == 0) return c;
  if ((only == NULL || !strcmp(only, "sdl")) && gpu_ctx_sdl_open(c, api, e2, sizeof e2) == 0) return c;
  snprintf(err, errlen, "no headless GL context: %s%s%s", e1, e1[0] && e2[0] ? " / " : "", e2);
  free(c);
  return NULL;
}

void gpu_ctx_destroy(GpuCtx *c) {
  if (c == NULL) return;
  gpu_gl_programs_forget();
  if (c->kind == GPU_CTX_EGL) gpu_ctx_egl_close(c);
#ifdef GPU_HAVE_EAGL
  else if (c->kind == GPU_CTX_EAGL) gpu_ctx_eagl_close(c);
#endif
  else if (c->kind == GPU_CTX_SDL) gpu_ctx_sdl_close(c);
  free(c);
}

GpuGetProc gpu_ctx_getproc(const GpuCtx *c) {
  if (c == NULL) return NULL;
#ifdef GPU_HAVE_EAGL
  if (c->kind == GPU_CTX_EAGL) return gpu_ctx_eagl_getproc;
#endif
  return c->kind == GPU_CTX_SDL ? gpu_ctx_sdl_getproc : gpu_ctx_egl_getproc;
}

const char *gpu_ctx_desc(const GpuCtx *c) { return c ? c->desc : ""; }
