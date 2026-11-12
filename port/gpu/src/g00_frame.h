/* port/gpu: the frame driver's structures (g00_frame.c), shared with the Metal core (gpu_mtl.m). OWNED BY THE SKELETON (G0). */
#ifndef GPU_G00_FRAME_H
#define GPU_G00_FRAME_H

#include "g02_setup.h"
#include "g08_smaa.h"
#include "g10_fast.h"
#include "gpu_uni.h"

enum { BAND_DS_ROWS = 8, TILE_DS_COLS = 16, TILES_X = VC_W / TILE_DS_COLS };
/* a bin ("tile") is 16 DS columns of a band of 8 DS rows (16 N x 8 N pixels at N x); bin = band * TILES_X + column. The
 * shader's index is (row / uBandRows) * 16 + x / (16 uScale) (shaders/g00_raster.frag) */

struct GpuCore {
  int scale, w, h, nband; /* scale: the grid the spans and samples are on, N k; w, h: the output, 256 N x 192 N */
  int out, k;             /* N, and k: k x k samples per output pixel (1 or 2) */
  GLuint prog_r, prog_rf, prog_p, prog_e, vao; /* prog_rf: pass R for Filter=bilinear, made at its first use (raster_prog) */
  GLuint prog_pf, t_st; /* k = 2: pass P's first draw (g06_post.frag's P_FAST) and the stencil of its pixels (fbo_p's) */
  GLuint t_em[4], fbo_em[4]; /* pass E's DS-resolution maps (N k > 1); k = 2: [2] its first stage, [3] its marks, at w x h */
  GLuint t_poly[GPU_RING], t_band[GPU_RING], t_list[GPU_RING]; /* written and read by one frame in turn (gpu_gl.h, GPU_RING) */
  int ring;
  GLuint t_tex[GPU_RING], t_pal[GPU_RING]; /* G4: a texture build goes into the next pair in turn, as the tables do */
  int tr;                                  /* the pair the frame reads */
  GLuint fbo_r, t_samp, fbo_p, t_out; /* t_samp: pass R's samples, a 2D array of k x k layers at w x h (g00_head.glsl) */
  GpuPrep *own;    /* gpu_render_3d's */
  uint32_t *rb;    /* readback: 4 words per pixel */
  double cpu_ms, total_ms;
  double up_ms, s_ms, r_ms, p_ms; /* timing mode: the uploads, pass S, pass R, pass P (glFinish after each) */
  GpuPassS ps;     /* pass S: the span records on the GPU (g02_setup.c) */
  int timing;
  GpuOptions opt;
  G08 *smaa;   /* Antialiasing=SMAA at N > 1 (g08_smaa.c), else NULL */
  GpuTexData *td, *own_td; /* gpu_use_texdata's for the next call; else own_td, built from s */
  unsigned td_up[GPU_RING]; /* the build in each t_tex / t_pal pair */
  double a_ms; /* timing mode: the SMAA passes */
  double e_ms; /* timing mode: pass E (in p_ms) */
  struct GpuMtl *mtl; /* the Metal core (gpu_mtl.m) when made with GPU_API_METAL, else NULL: the GL half's fields are unused */
  unsigned t_weights; /* Metal: SMAA's weights by texture name (gpu_mtl_texture), 0 without */
  struct GpuFastGL *fgl; /* GL: the fast 3D renderer's programs and data (g10_fast_gl.c), made at its first use */
};

/* k = 2: a sample of pass R as it is stored (RG32UI, shaders/g00_head.glsl) in k = 1's RGBA32UI form, as g_unpack does it (the
 * readbacks: the translucent ID, F_STENCIL and the last polygon are not stored at k = 2: 0) */
static inline void gpu_samp_unpack(uint32_t x, uint32_t y, uint32_t *o) {
  o[0] = x & 0xFFFFFFu;
  o[1] = ((x >> 24) & 63u) | ((y >> 23) & 7u) << 16 | ((y >> 26) & 1u) << 20 | (y >> 27) << 24 | (x & 0xC0000000u);
  o[2] = (y & 63u) | ((y >> 6) & 63u) << 8 | ((y >> 12) & 63u) << 16 | ((y >> 18) & 31u) << 24;
  o[3] = 0;
}

/* the CPU half's result (gpu_prepare) */
struct GpuPrep {
  int scale, nband, nbin;
  GpuSpans sp;
  GpuOptions opt;  /* the CPU half's options (dot, line): the caller's, else the environment's at the first build */
  int opt_set;
  int check;       /* gpu_spans_check: the CPU builds the records too */
  uint32_t *band;  /* nbin x (first, count) */
  uint8_t (*ext)[2]; /* polygon x band: the first and last bin column its rows cover (first > last: none) */
  uint32_t *list;  /* the bands' polygon lists */
  int listcap, nlist;
  double ms;       /* what gpu_prepare took */
  GpuFast fast;    /* opt.fast: the triangles (g10_fast.c) instead of the records and the band lists */
  uint32_t swap_param; /* SWAP_BUFFERS' parameter of the frame prepared (the GL half reads it from here: s->geo may be rewritten by the next geometry run) */
};

/* the uniforms of pass R, E and P for the frame of s prepared in p (the fields each reads; the rest 0) */
void gpu_uni_raster(GpuU *u, const GpuCore *g, const VCoreState *s, const GpuPrep *p);
/* the fast 3D renderer on GL (g10_fast_gl.c): _ok makes its programs at the first call (1: it can draw, else gpu_fast_how says
 * why), _r draws pass R's slot from p's triangles (ring entry ri: the polygon table and textures already uploaded) */
int gpu_gl_fast_ok(GpuCore *g);
void gpu_gl_fast_r(GpuCore *g, const VCoreState *s, const GpuPrep *p, int ri);
void gpu_gl_fast_release(GpuCore *g);
const char *gpu_gl_fast_how(void);
void gpu_uni_edge(GpuU *u, const GpuCore *g, const VCoreState *s);
void gpu_uni_post(GpuU *u, const GpuCore *g, const VCoreState *s);
/* pass E runs this frame (N k > 1, edge marking on) */
int gpu_edgemap_on(const GpuCore *g, const VCoreState *s);

#endif
