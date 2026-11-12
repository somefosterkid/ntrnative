/* port/gpu pass S: the span records built on the GPU from the polygons' setup (docs/gpu-cost.md P1; g02_setup.c,
 * shaders/g02_setup.vert), and the span texture pass R reads. OWNED BY G2. */
#ifndef GPU_G02_SETUP_H
#define GPU_G02_SETUP_H

#include "g02_spans.h"
#include "gpu_gl.h"

#include <stdio.h>

/* the span texture: RGBA32UI, 1024 wide, record r's texel t (float bits) at texel 12 r + t (shaders/g00_head.glsl g_span) */
typedef struct GpuPassS {
  GLuint prog;   /* 0: the records come from the CPU */
  GLuint tfb;    /* pass S's transform-feedback buffer, then the copy's source */
  size_t tfcap;  /* its bytes */
  GLuint t_setup[GPU_RING], t_find[GPU_RING], t_span[GPU_RING], fbo_read;
  int ring;      /* the t_setup / t_find this frame's pass S writes and reads */
  int sring;     /* the t_span this frame wrote and pass R reads: 0 when pass S copies it on the GPU, the ring when the CPU's
                  records are uploaded (an upload into the texture the GPU still reads would wait for it) */
  int maxrows;   /* GL_MAX_TEXTURE_SIZE: the span texture's rows at most */
  GLuint prog_f, t_frag[4], fbo_f, fbo_s; /* GPU_PS_FRAG: its program, the four targets (three bands each), its framebuffers */
  GLuint query;  /* the self-check's GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN */
  int diag;      /* set by the self-check: the next pass S counts what transform feedback wrote */
  GpuSpans *cpu; /* the records built here when the prep built only the setup and this core runs no pass S */
  uint32_t *tmp; /* the readback's rows */
  size_t tmpcap;
} GpuPassS;

/* how pass S makes the records (gpu_g02_step goes down the list when the self-check finds them wrong; one choice for the
 * process, read and set on the GL thread; BWPORT_GPU_PASSS=tf|chunk|range|frag|cpu picks the first):
 * TF one transform-feedback draw and one copy into the span texture; CHUNK the same session in draws of 1024 points; RANGE
 * chunks of 2048 records, each bound at its own offset in its own session; FRAG a fragment shader (four targets, three
 * draws, then one draw that interleaves them into the span texture); CPU the CPU builder (gpu_pass_s_off). Apart from
 * those, the copy can go up in pieces of 32 rows (when the buffer was right and the texture was not). */
enum { GPU_PS_TF, GPU_PS_CHUNK, GPU_PS_RANGE, GPU_PS_FRAG, GPU_PS_CPU };
int gpu_g02_mode(void);
const char *gpu_g02_mode_name(void);
/* after a self-check found sp's records wrong (gpu: the span texture's, read back): a few lines on f (what transform feedback
 * wrote, the first wrong record in its buffer and in the texture, the GL limits) and the next way to try. Returns the new
 * mode (GPU_PS_CPU: none left). */
int gpu_g02_step(GpuPassS *ps, const GpuSpans *sp, const uint32_t *gpu, FILE *f);

/* BWPORT_GPU_SPANS: 0 pass S (the default); 1 "cpu": the prep builds the records (the CPU builder, on the worker); 2 "nopass":
 * the core runs no pass S, so the GL thread builds the records (what a core whose pass S failed does; tested on purpose) */
int gpu_g02_env(void);
/* pass S's program (once per core). -1 when it failed or is switched off: this core takes the CPU's records from then on */
int gpu_g02_init(GpuPassS *ps, char *err, size_t errlen);
void gpu_g02_release(GpuPassS *ps);
/* sp's records into the span texture (bound to GL_TEXTURE_2D on the active unit): built by pass S from sp's setup when this
 * core runs it, else sp's own records, else (sp has only the setup) built here from s at scale. Pass S draws into the bound
 * framebuffer, which must be complete (nothing is written: the rasterizer is off); the buffer bindings are left at 0. -1
 * when they do not fit (more rows than maxrows) or memory ran out. */
int gpu_g02_records(GpuPassS *ps, const GpuSpans *sp, const VCoreState *s, int scale, GLuint vao);
/* the span texture's first n records read back as GpuSpan (n x GPU_SPAN_TEXELS x 4 words) */
void gpu_g02_read(GpuPassS *ps, int n, uint32_t *out);
/* the GPU's records against the CPU's (sp->span), field by field: a count per field and the first ten (and three of the low
 * parts; detail 0: only the summary line) on f. Returns the number of integer fields that differ (the attributes are
 * reported apart: hi exact, lo within an ulp). */
int gpu_g02_compare(const GpuSpans *sp, const uint32_t *gpu, FILE *f, int detail);

#endif
