/* port/gpu piece G2: the span records (docs/gpu.md, "The data", G2). OWNED BY G2; the record layout below is fixed (the
 * shaders read it: shaders/g00_head.glsl, g03_pixel.glsl).
 *
 * One record per polygon and covered row (rows of the N x screen), 12 texels of 4 floats (on the GPU: texel 12 r + t of the span
 * texture holds record r's texel t, g02_setup.h; pass S builds them there, this file on the CPU). Built from vcore p16's own row
 * setup (vc_span_poly / vc_span_row, p16_raster.h) on the N x grid:
 *   t0: xs, xe       the run of pixels the row draws (inclusive, clamped to the screen; xs > xe: none)
 *       den_f, den_z the denominators of the attribute fraction (x - x0) / den_f and of depth's (x - x0) / den_z
 *   t1: L.w, R.w, L.z, R.z            w (clip w, 20.12 as a number) and depth (z 24-bit, or w with SWAP_BUFFERS bit 1)
 *   t2: L.c0, L.c1, L.c2, L.s         the row's left end: colour 9-bit (16 c5 + 15, 0 for 0) and s (12.4)
 *   t3: R.c0, R.c1, R.c2, R.s         the right end
 *   t4: L.t, R.t, row flags (GPU_ROW_*), x0 (the fractions' origin: vcore's xs, before the clamp)
 *   t5: the edge-flag mask: pixels lo1, hi1, lo2, hi2 of the merged runs (p16's emask, plus the row's last pixel); an
 *       empty run is lo > hi
 *   without antialiasing: t6 = lo3, hi3, lo4, hi4 and t7 = lo5, hi5, lo6, hi6 (more runs of the mask; t8, t9 unused);
 *   with antialiasing (GPU_ROW_AA), the two edges' coverage windows (p16's aa_edge) as
 *   t6: left lo, hi, cov0 + 1 (0: the X-major formula), ax        t7: left k, dx, dy, unused
 *   t8: right lo, hi, (cov0 + 1) + 64 sh, ax                      t9: right k, dx, dy, unused
 *   (k = the row - ay of the edge; a pixel outside lo..hi has full coverage; cov0 >= 0 is that pixel's coverage)
 *   t10: low parts (value - (float)value of the doubles above) of L.z, R.z; the inner run lo, hi
 *   t11: low parts of L.t, R.t, L.s, R.s
 *   The inner run: pixels after t5's first run and before its second (so in none of the mask's runs, which are sorted), and
 *   with GPU_ROW_AA out of both windows of t6 and t8 (on the N grid: a DS-grid window lo .. hi is pixels lo m .. hi m + m - 1);
 *   a window that lies inside it keeps its left part. Its pixels have no edge flag and full coverage: pass R's edge step reads
 *   no more of the record for them (lo > hi: none).
 * The skeleton filled t0-t5 by a plain pixel-centre rule; G2 replaced it with vcore p16's edge walk (exact at 1x).
 */
#ifndef GPU_G02_SPANS_H
#define GPU_G02_SPANS_H

#include "vc_shared.h"

enum { GPU_SPAN_TEXELS = 12, GPU_POLY_TEXELS = 2 };
enum {
  GPU_ROW_ALLEDGE = 1, /* every pixel of the row is an edge pixel (a flat-bottomed polygon's last row, a 1-dot polygon) */
  GPU_ROW_WIRE = 2,    /* wire-frame: only the mask's pixels are drawn */
  GPU_ROW_AA = 4,      /* t6-t9 hold the antialiasing windows */
  GPU_ROW_VERTL = 8,   /* the left edge is vertical (antialiasing: its pixel has full coverage on the right edge's side) */
  GPU_ROW_COV0 = 16,   /* the pixels' coverage is 0 (a 1-dot polygon) */
  GPU_ROW_DSGRID = 32, /* with GPU_ROW_AA: the windows are on the DS grid (a line walked at N > 1): coverage of pixel x / N */
  GPU_ROW_MASK2 = 64   /* without AA: the edge-flag mask has more than 2 runs, t6 and t7 hold them (else not read) */
};
enum { GPU_ROW_M_SHIFT = 8 }; /* row flags >> 8 = the row's widening m (a DS-grid or N-grid span widened to m-wide blocks), 0 = none */
/* the polygon table's flags (t0.w) */
enum { GPU_PF_TRANS = 1, GPU_PF_FRONT = 2, GPU_PF_WBUF = 4, GPU_PF_WIRE = 8, GPU_PF_WHOLE = 16 };

typedef struct GpuSpan {
  float t[GPU_SPAN_TEXELS][4];
} GpuSpan;

/* t0: attr, texparam, pltt, flags; t1: first row, last row, first record, polygon alpha (wire-frame: 31) */
typedef struct GpuPolyRec {
  uint32_t t[GPU_POLY_TEXELS][4];
} GpuPolyRec;

/* pass S's input (shaders/g02_setup.vert builds the records from it on the GPU; docs/gpu-cost.md P1). Per polygon with
 * records, in record order, a find entry (first record, its first setup texel) and its setup, RGBA32UI texels:
 *   h0: flags (GPU_SU_*, m << 8, ne << 16, nv << 20), first record, records, W | H << 16 (the walk's grid)
 *   h1: ymin, yend, y0 (the first row walked; a dot: its row), the dot's x (ints)
 *   h2: the edges' vertex indices, ia | ib << 4 a byte each: edges 0-3 in x, 4-7 in y, 8-9 in z
 *   ne texels: an edge's ax, ay, bx, by (p16's VC_SpanSeg)
 *   nv x 2 texels: a vertex's w, z, s, t and c0, c1, c2, 0 (p16's VC_SpanAttr: whole numbers) */
enum { GPU_SU_DOT = 1, GPU_SU_WIRE = 2, GPU_SU_FULL = 4, GPU_SU_AA = 8, GPU_SU_FLATBOT = 16 };
typedef struct GpuSetup {
  uint32_t (*tx)[4];
  int ntx, captx;
  uint32_t (*find)[2];
  int nfind, capfind;
} GpuSetup;

/* a polygon's last setup build (the memo, by its polygon RAM index): its inputs and what the build made of them. A polygon whose
 * inputs equal its entry's (and the build's own inputs the memo's) gets the entry's rows and setup again, at its place */
enum { GPU_MEMO_KEY = 3 + 9 * VC_POLY_MAXV, GPU_MEMO_TX = 3 + 3 * VC_POLY_MAXV, GPU_MEMO_BANDS = 24 };
typedef struct GpuMemo {
  uint32_t key[GPU_MEMO_KEY]; /* nv, attr, viewport, then each vertex's clip x y z w, sx, sy, sz, colour, texcoords */
  uint32_t r0, r1, nrec;      /* its rows (t1.x, t1.y) and the records it counts */
  uint32_t ntx;               /* its setup's texels (0: none) */
  uint32_t tx[GPU_MEMO_TX][4]; /* the setup, h0's first record 0 */
  uint8_t ext[GPU_MEMO_BANDS][2]; /* gpu_prepare's band extents of its rows' bands (extok) */
  uint8_t nkey, valid, hit, extok; /* hit: the last build took it (made it: 0) */
} GpuMemo;

typedef struct GpuSpans {
  GpuSpan *span;
  int nspan, capspan; /* records: nspan counts them also when only the setup is built */
  int records;        /* build the records on the CPU (else only count them) */
  int setup;          /* build pass S's setup */
  GpuSetup su;
  GpuPolyRec poly[VC_MAX_POLYS]; /* in drawing order (s->geo.order) */
  int npoly;
  int dot_block;      /* N > 1: a 1-dot polygon is an N x N block (the DS look), else one N x pixel */
  int line_wide;      /* N > 1: lines (zero-area polygons) and wire-frames are drawn on the DS grid, N wide */
  int mask_overflow;  /* rows whose edge-flag mask had more runs than the record holds (last frame) */
  int aa;             /* supersampling k (scale = N k; 1, 2, 4): Dot=fine and Line=thin are one pixel of the N grid, k x k samples */
  int opt_set;        /* dot_block and line_wide are set (else the first build reads them from the environment) */
  GpuMemo *memo;      /* the memo (setup builds only: records off): capmemo entries */
  int capmemo;
  uint32_t memo_in[8]; /* the build's own inputs the entries were made with */
  GpuMemo *memo_of[VC_MAX_POLYS]; /* in drawing order: polygon k's entry (NULL: none) */
  int memo_check;     /* BWPORT_GPU_MEMOCHECK=1: a polygon that matches its entry is built anyway and compared */
  int memo_off;       /* BWPORT_GPU_MEMO=0: no memo */
  int memo_env;       /* the two are read */
  unsigned memo_builds, memo_polys, memo_hits, memo_bad; /* counts for the check's log (every 600 builds) */
} GpuSpans;

/* the records of every polygon of s->geo in drawing order at scale n: rows 0..192n-1, columns 0..256n-1. The options
 * dot_block and line_wide come from $BWPORT_GPU_DOT (block|fine) and $BWPORT_GPU_LINE (wide|thin) at the first build
 * unless the caller set them with opt_set = 1. */
void gpu_spans_build(GpuSpans *sp, const VCoreState *s, int scale);
void gpu_spans_free(GpuSpans *sp);
/* vertex v of polygon `poly`: its position on the N x screen: N = 1 vcore's sx, sy; else the viewport transform of its
 * clip coordinates at N times the polygon's own viewport (VC_Poly.viewport, kept by vcore p14: docs/gpu.md, G9) */
void gpu_vertex_pos(const VCoreState *s, const VC_Poly *poly, const VC_Vertex *v, int scale, int *x, int *y);
/* one VC_Span packed into a record at horizontal scale m (m > 1: the DS-grid span widened to m-wide blocks) */
void gpu_span_pack(GpuSpan *r, const VC_Span *v, int m, int extra_row_flags, int *overflow);

/* setup entry f's pixels over rows r0..r1 (of the N x grid; inside its records' rows) lie in x0..x1: the edges' x range there,
 * a few pixels wider (the band lists without the records) */
void gpu_setup_extent(const GpuSetup *su, int f, int r0, int r1, int *x0, int *x1);
/* p16's row arithmetic in 32-bit integers, as pass S computes it (g02_spans.c; checked against p16's s64 forms by tests/t02_spans.c) */
int gpu_s32_edge_span(const VC_SpanSeg *e, int y, int *lo, int *hi, int *rd);
int gpu_s32_xmajor_q(const VC_SpanSeg *e, int k);
int gpu_s32_aa_covx(const VC_SpanEdge *e, int k, int j, int left);
int gpu_s32_aa_edge(const VC_SpanSeg *e, int y, int left, VC_SpanAA *r);
void gpu_s32_edge_mid(const VC_SpanSeg *e, int y, int *n, int *d);
int gpu_s32_mid_cmp(int n1, int d1, int n2, int d2);

#endif
