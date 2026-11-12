/* port/gpu: the Metal core's side of the frame driver and of pass C (gpu_mtl.m, Apple only; docs/gpu-backends.md, M1-M2). The
 * CPU half (gpu_prepare, the texture build, pass C's pack) is the GL core's; g00_frame.c and g07_compose.c hand a core made with
 * GPU_API_METAL to these. A build without GPU_HAVE_METAL gets the stubs below: gpu_create(GPU_API_METAL) fails there. */
#ifndef GPU_MTL_H
#define GPU_MTL_H

#include "g00_frame.h"

/* the generated MSL (gpu_msl.c from port/gpu/msl, tools/gpu_msl.py): name as gpu_gl_program's fs ("g00_raster.frag#2") */
typedef struct GpuMslSrc {
  const char *name, *text;
} GpuMslSrc;
extern const GpuMslSrc gpu_msl_src[];

typedef struct GpuMtlC GpuMtlC; /* pass C's Metal side (G07's) */

/* test-only ablations (the maintainers' notes): a pass taken out or faked so a perf run measures the frame without it. Off (0) by
 * default; the bridge hands the mask in from the cfgscript's ablate= key. The pictures are wrong while one is on. */
enum {
  GPU_ABL_R = 1,     /* pass R draws no polygon (every bin's list empty; Fast: no triangle): the rear plane and the layers' stores only */
  GPU_ABL_E = 2,     /* no pass E (pass P reads the last edge map) */
  GPU_ABL_PSLOW = 4, /* pass P without its slow-pixel draw (4 samples: those pixels keep the target's old contents) */
  GPU_ABL_P = 8,     /* no pass P (the 3D output stays the last frame's) */
  GPU_ABL_C = 16     /* no pass C (the composed picture stays the last frame's) */
};

#ifdef GPU_HAVE_METAL
void gpu_mtl_ablate(unsigned mask); /* GPU_ABL_* bits, for every core in the process */
/* g's targets and pipelines (g's common fields are set); smaa: SMAA at N > 1. 0, else -1 with the reason in err */
int gpu_mtl_init(GpuCore *g, int smaa, char *err, size_t errlen);
void gpu_mtl_release(GpuCore *g);
/* gpu_render_prepared's GL half: the uploads and passes S, R, E, P and SMAA into the frame's command buffer (timing mode: one
 * command buffer a pass, committed and waited for, its GPU time in g's fields) */
int gpu_mtl_render(GpuCore *g, VCoreState *s, const GpuPrep *p, const GpuTexData *td);
void gpu_mtl_read_3d(GpuCore *g, uint32_t *out, uint32_t *aux);
void gpu_mtl_read_raster(GpuCore *g, uint32_t *out);
/* the span records the last frame's pass S wrote (n x GPU_SPAN_TEXELS x 4 words) */
void gpu_mtl_spans_read(GpuCore *g, int n, uint32_t *out);
int gpu_mtl_pass_s(const GpuCore *g); /* 1 when this core builds the records (pass S's kernel is there) */
void gpu_mtl_pass_s_off(GpuCore *g);
int gpu_mtl_fast_ok(GpuCore *g); /* the fast 3D renderer's pipelines are made (at the first call): it can draw */

GpuMtlC *gpu_mtl_c_create(int scale, char *err, size_t errlen);
void gpu_mtl_c_destroy(GpuMtlC *c);
/* pass C from G07Pack's stage (VC_H x VC_W x 4 words) and src (VC_H x 2 x VC_W words), pack serial (not uploaded again when it is
 * the last one's), plan: the plan made from the stage first (else the last one's is used), the 3D layer and SMAA's weights by
 * texture name (gpu_mtl_texture), the uniforms */
int gpu_mtl_compose(GpuMtlC *c, const uint32_t *stage, const uint32_t *src, unsigned serial, int plan, unsigned tex3d, unsigned smaa,
                    const GpuU *u);
unsigned gpu_mtl_c_upload_3d(GpuMtlC *c, const uint8_t *rgba8ui, int w, int h);
unsigned gpu_mtl_c_frame_tex(const GpuMtlC *c);
void gpu_mtl_c_read(GpuMtlC *c, uint8_t *rgba);
double gpu_mtl_c_gpu_ms(const GpuMtlC *c); /* the last pass C's command buffer GPU ms, once it is done (0 before) */
#else
static inline void gpu_mtl_ablate(unsigned mask) { (void)mask; }
static inline int gpu_mtl_init(GpuCore *g, int smaa, char *err, size_t errlen) {
  (void)g, (void)smaa;
  snprintf(err, errlen, "no Metal core in this build");
  return -1;
}
static inline void gpu_mtl_release(GpuCore *g) { (void)g; }
static inline int gpu_mtl_render(GpuCore *g, VCoreState *s, const GpuPrep *p, const GpuTexData *td) {
  (void)g, (void)s, (void)p, (void)td;
  return -1;
}
static inline void gpu_mtl_read_3d(GpuCore *g, uint32_t *out, uint32_t *aux) { (void)g, (void)out, (void)aux; }
static inline void gpu_mtl_read_raster(GpuCore *g, uint32_t *out) { (void)g, (void)out; }
static inline void gpu_mtl_spans_read(GpuCore *g, int n, uint32_t *out) { (void)g, (void)n, (void)out; }
static inline int gpu_mtl_pass_s(const GpuCore *g) { return (void)g, 0; }
static inline void gpu_mtl_pass_s_off(GpuCore *g) { (void)g; }
static inline int gpu_mtl_fast_ok(GpuCore *g) { return (void)g, 0; }
static inline GpuMtlC *gpu_mtl_c_create(int scale, char *err, size_t errlen) {
  (void)scale;
  snprintf(err, errlen, "no Metal core in this build");
  return NULL;
}
static inline void gpu_mtl_c_destroy(GpuMtlC *c) { (void)c; }
static inline int gpu_mtl_compose(GpuMtlC *c, const uint32_t *stage, const uint32_t *src, unsigned serial, int plan,
                                  unsigned tex3d, unsigned smaa, const GpuU *u) {
  (void)c, (void)stage, (void)src, (void)serial, (void)plan, (void)tex3d, (void)smaa, (void)u;
  return -1;
}
static inline unsigned gpu_mtl_c_upload_3d(GpuMtlC *c, const uint8_t *rgba8ui, int w, int h) {
  (void)c, (void)rgba8ui, (void)w, (void)h;
  return 0;
}
static inline unsigned gpu_mtl_c_frame_tex(const GpuMtlC *c) { return (void)c, 0u; }
static inline void gpu_mtl_c_read(GpuMtlC *c, uint8_t *rgba) { (void)c, (void)rgba; }
static inline double gpu_mtl_c_gpu_ms(const GpuMtlC *c) { return (void)c, 0.0; }
#endif

#endif
