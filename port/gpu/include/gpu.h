/* port/gpu: the DS 3D layer (and engine A's compositing) at N x the DS resolution on the GPU, GLES 3.0 / GL 3.3 core
 * (docs/gpu.md). The CPU half is port/vcore's (geometry, sort, the edge walk); the GPU half draws the pixels.
 * OWNED BY THE SKELETON.
 *
 * A GL context must be current on the calling thread for every gpu_* call but gpu_ctx_* (the tools make a headless
 * one with gpu_ctx_headless; the port's bridge uses libntr's).
 */
#ifndef GPU_H
#define GPU_H

#include <stddef.h>
#include <stdint.h>

#include "vcore.h"

#ifdef __cplusplus
extern "C" {
#endif

/* a GL function by name (eglGetProcAddress, SDL_GL_GetProcAddress) */
typedef void (*GpuProc)(void);
typedef GpuProc (*GpuGetProc)(const char *name);

/* OpenGL ES 3.0 (#version 300 es), desktop GL 3.3 core (#version 330 core), or Metal (Apple, a build with GPU_HAVE_METAL: the
 * MSL generated from the same GLSL, src/gpu_mtl.m; no GL context, getproc unused) */
enum { GPU_API_ES = 0, GPU_API_GL = 1, GPU_API_METAL = 2 };
enum { GPU_MAX_SCALE = 8 };

typedef struct GpuCore GpuCore;

/* G9: the N x look (they do nothing at N = 1). dot: a 1-dot polygon (particles) is an N x N block (0, the DS look) or one
 * N x pixel (1); line: lines and wire-frames are N wide (0) or one N x pixel (1); edge: edge marking is N wide, one DS
 * pixel (0) or one N x pixel (1); filter: texture sampling nearest, the DS look (0) or bilinear on the decoded texels,
 * the alpha rules kept (1). The defaults of a new GpuCore / GpuPrep come from the environment (BWPORT_GPU_DOT=block|fine,
 * BWPORT_GPU_LINE=wide|thin, BWPORT_GPU_EDGE=ds|fine, BWPORT_GPU_FILTER=nearest|bilinear), else all 0. */
typedef struct GpuOptions {
  int dot, line, edge, filter;
  int aa; /* multisampling k (1 or 2: k x k samples per output pixel, on the N k grid the spans are built on): dot, line and edge
          * act on the N grid, a polygon is shaded once per output pixel (docs/gpu.md "Antialiasing"); a GpuCore's is its own.
          * GPU_AA_SMAA: one sample per pixel and SMAA on the 3D layer after pass P (N > 1; docs/gpu.md "Antialiasing: SMAA") */
  int fast; /* the fast 3D renderer (docs/gpu-hwraster.md: the GPU's own rasterizer, close to the DS, not exact): no span records,
             * triangles (prep and core; a core without it draws exact: gpu_fast_supported). BWPORT_RENDERER3D=fast|exact */
} GpuOptions;
enum { GPU_AA_SMAA = 3 };
void gpu_options_from_env(GpuOptions *o);
/* dot and line act in the CPU half (the prep), edge and filter in the GL half (the core): set both, per frame is fine */
void gpu_set_options(GpuCore *g, const GpuOptions *o);
int gpu_fast_supported(GpuCore *g); /* the core draws fast (Metal; GL with framebuffer fetch or fragment interlock: g10_fast_gl.c) */
/* how it draws fast, or why it cannot ("Metal", "OpenGL ES: framebuffer fetch", "this GLES has no ..."): after gpu_fast_supported */
const char *gpu_fast_how(const GpuCore *g);

/* loads the GL functions, compiles the shaders, makes the render targets for scale N (1..8) with aa x aa samples per pixel
 * (aa 1 or 2; the targets stay at N), or aa = GPU_AA_SMAA: one sample and SMAA (off at N = 1). NULL on failure, with the
 * reason in err. */
GpuCore *gpu_create(GpuGetProc getproc, int api, int scale, int aa, char *err, size_t errlen);
void gpu_destroy(GpuCore *g);
int gpu_scale(const GpuCore *g);   /* N */
int gpu_samples(const GpuCore *g); /* samples per pixel: 1 or 4 */

/* vcore's geometry for the frame in s (VRAM views, the GX3D stream through the geometry engine, the sort at
 * SWAP_BUFFERS): fills s->geo. 1 when the stream had a SWAP_BUFFERS (a frame to draw), else 0. Not needed after
 * vcore_render(s), which has done it. */
int gpu_geometry(VCoreState *s);
/* gpu_geometry again for the same state (in-between pictures): no VRAM mapping, s->geo cleared first */
int gpu_geometry_again(VCoreState *s);

/* the CPU half of drawing the 3D layer of s at N x, apart from the GL half: the span records (vcore p16's edge walk
 * on the N x grid), the polygon table, the bands' lists. It reads only s (s->geo as gpu_geometry or vcore_render left
 * it, the 3D registers) and its own GpuPrep, so a worker thread runs it with the geometry while the GL thread draws the
 * frame before: one GpuPrep per frame in flight. No GL context is needed. */
typedef struct GpuPrep GpuPrep;
GpuPrep *gpu_prep_create(int scale); /* scale: the grid, N k (1..2 GPU_MAX_SCALE) */
void gpu_prep_destroy(GpuPrep *p);
int gpu_prep_scale(const GpuPrep *p);
void gpu_prepare(GpuPrep *p, const VCoreState *s);
void gpu_prep_set_options(GpuPrep *p, const GpuOptions *o);

/* the texture slots and palettes of s's VRAM flattened for the GL half (any thread, s's VRAM map as vc_vram_map or
 * gpu_geometry left it): the port's worker builds one per frame, which its in-between pictures share */
typedef struct GpuTexData GpuTexData;
GpuTexData *gpu_texdata_create(void);
void gpu_texdata_destroy(GpuTexData *t);
void gpu_texdata_build(GpuTexData *t, const VCoreState *s);
/* the next gpu_render_prepared / gpu_render_3d takes the textures from t, built from the s it is given, and uploads nothing
 * when that build is up already (NULL, the default: it flattens s itself). Good for one call. */
void gpu_use_texdata(GpuCore *g, const GpuTexData *t);

/* the GL half: uploads p's records and the textures from s's VRAM, runs pass R and pass P. p must be prepared from
 * the same s (the same frame) at g's grid (N k) with options.aa = k. 0 on success, else -(GL error). */
int gpu_render_prepared(GpuCore *g, VCoreState *s, const GpuPrep *p);

/* draws the 3D layer of s (s->geo as gpu_geometry or vcore_render left it, the 3D registers, VRAM) at N x on the GPU:
 * gpu_prepare into g's own GpuPrep, then gpu_render_prepared. 0 on success. */
int gpu_render_3d(GpuCore *g, VCoreState *s);

/* the 3D layer just drawn, (256 N) x (192 N) pixels, top row first, each RGB6 | alpha5 << 24 like vcore's
 * s->f3d.out. aux (or NULL): pass R's texel of each pixel's first sample, 4 words (depth, IDs|flags|coverage, colour, polygon). */
void gpu_read_3d(GpuCore *g, uint32_t *out, uint32_t *aux);

/* the 3D layer just drawn as a texture, without a readback: GL_RGBA8UI, (256 N) x (192 N), DS row 0 at GL row 0, texel =
 * (r6, g6, b6, a5): pass C's input (g07_compose with sc = N). Owned by g; the next gpu_render_3d overwrites it. */
unsigned gpu_3d_tex(const GpuCore *g);
/* SMAA's blending weights for that layer (RGBA8, the same size), 0 without SMAA: pass C blends with them (g07_compose) */
unsigned gpu_smaa_weights(const GpuCore *g); /* (also 0 when SMAA was asked for and could not be set up: drawn without) */

/* on: gpu_render_3d ends with glFinish so that gpu_last_times measures the GPU too (the tools); off (default): it
 * returns when the commands are queued (the bridge) */
void gpu_set_timing(GpuCore *g, int on);

/* the per-pass hook (the port's timing readout): fn(pass, 1) before a pass's GL calls and fn(pass, 0) after them, on
 * the GL thread; NULL (the default) calls nothing. GPU_PASS_UPLOAD brackets the data and texture uploads (twice a frame:
 * gpu_render_prepared's, g07_compose's), the others one pass each (E only when it runs). The port's bridge brackets
 * each with a GL timer query. */
enum { GPU_PASS_UPLOAD, GPU_PASS_R, GPU_PASS_E, GPU_PASS_P, GPU_PASS_C, GPU_PASS_N };
typedef void (*GpuPassHook)(int pass, int begin);
void gpu_set_pass_hook(GpuPassHook fn);

/* milliseconds of the last gpu_render_3d: CPU side (spans, bands, uploads) and the whole call with glFinish */
void gpu_last_times(const GpuCore *g, double *cpu_ms, double *total_ms);
/* timing mode: the GL half's phases of the last call (glFinish after each): uploads, pass R (pass S inside it), pass P */
void gpu_last_passes(const GpuCore *g, double *up_ms, double *r_ms, double *p_ms);
double gpu_last_smaa_ms(const GpuCore *g); /* timing mode: the SMAA passes (0 without) */
/* timing mode: pass S of the last call (the span records built on the GPU; inside gpu_last_passes' R and GPU_PASS_R's bracket
 * for the hook), 0 when the CPU built them */
double gpu_last_pass_s(const GpuCore *g);
/* timing mode: pass E of the last call (inside gpu_last_passes' P), 0 when it did not run; the Metal core's only (GL: 0) */
double gpu_last_pass_e(const GpuCore *g);
/* the tools' --spans-check and the bridge's self-check: s's records built by pass S, read back and diffed against the CPU
 * builder's field by field (a report on f, FILE *). While they differ, pass S's next way of making them is tried on the same
 * frame (src/g02_setup.h GPU_PS_*, with diagnostics on f). Returns the number of integer fields that differ with the last
 * way tried (> 0: none was right, call gpu_pass_s_off), -1 when pass S is off. */
int gpu_spans_check(GpuCore *g, VCoreState *s, void *f);
/* pass S off for good: the CPU builds the span records (after a failed self-check) */
void gpu_pass_s_off(GpuCore *g);

/* ---- the Metal core (GPU_API_METAL). A texture name (gpu_3d_tex, gpu_smaa_weights, g07_frame_tex, g07_upload_3d) stands for an
 * id<MTLTexture>, as a GL name for a GL texture. The passes of a frame (gpu_render_prepared, gpu_render_3d, g07_compose*) go into
 * one command buffer, the frame's, made on first use; the caller commits it (the readbacks commit and wait themselves). ---- */
void *gpu_mtl_texture(unsigned name);       /* the id<MTLTexture> (__bridge), NULL for 0 or a name that is gone */
void *gpu_mtl_device(void);                 /* the id<MTLDevice> the core uses */
void *gpu_mtl_command_buffer(void);         /* the frame's id<MTLCommandBuffer>: a present can encode into it before the commit */
void gpu_mtl_commit(int wait);              /* commits the frame's command buffer (nothing when none); wait: until the GPU is done */
/* the device and queue to use (id<MTLDevice>, id<MTLCommandQueue>, __bridge): before the first gpu_create(GPU_API_METAL), else the
 * system's default device and a queue of the core's own */
void gpu_mtl_use(void *device, void *queue);

/* ---- a headless context for the tools (EGL; Mesa's surfaceless platform first) ---- */
typedef struct GpuCtx GpuCtx;
GpuCtx *gpu_ctx_headless(int api, char *err, size_t errlen);
void gpu_ctx_destroy(GpuCtx *c);
GpuGetProc gpu_ctx_getproc(const GpuCtx *c);
const char *gpu_ctx_desc(const GpuCtx *c); /* "EGL 1.5 surfaceless (Mesa Project)" */

#ifdef __cplusplus
}
#endif

#endif
