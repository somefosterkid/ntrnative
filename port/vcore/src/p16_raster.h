/* port/vcore piece 16: the rasterizer (docs/vcore.md, piece 16). OWNED BY PIECE 16: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P16_H
#define VC_P16_H

#include "vc_types.h"

typedef struct VC_P16 {
  s32 st[2]; /* the texture coordinates (12.4) of the pixel being plotted: set just before each s->plot call
              * (for the unit tests and debugging) */
} VC_P16;

struct VCoreState;
/* vc_raster_poly restricted to the rows y0 <= y < y1 (the bands of the threaded frame: rows are independent, so a
 * band writes only its own rows of s->f3d and nothing else when s->plot is the default) */
void vc_raster_band(struct VCoreState *s, const VC_Poly *p, int y0, int y1);


/* ---- the row setup (piece G2 split it from the pixel loop: port/gpu builds its span records from it) ----
 * vc_span_poly() turns a polygon and its vertex positions on a W x H screen (VC_W x VC_H for vcore; 256 N x 192 N for
 * the GPU path: the same rules as if the DS's screen were N times bigger) into the edges and the vertex attributes;
 * vc_span_row() then gives one row's VC_Span: the run of pixels, the attribute ends, the edge-flag runs. vcore's
 * own pixel loop (vc_raster_band) consumes exactly this struct. */
typedef struct VC_SpanAttr {
  double w, z, c[3], st[2]; /* clip w, depth (z, or w with SWAP_BUFFERS bit 1), colour 9-bit (16 c5 + 15), s, t (12.4) */
} VC_SpanAttr;
typedef struct VC_SpanSeg { /* a polygon edge, a above b (ay <= by; a horizontal edge ax <= bx) */
  int ax, ay, bx, by, dx, dy;
  int ia, ib; /* the attribute indices of a and b */
} VC_SpanSeg;
typedef struct VC_SpanEdge { /* what the coverage needs of an edge */
  int ax, ay, dx, dy;
} VC_SpanEdge;
typedef struct VC_SpanAA { /* the antialiasing window of an edge on a row (p16's aa_edge) */
  int first, last; /* drawn from `first` (left edge) / to `last` (right edge) */
  int lo, hi;      /* the pixels with their own coverage; others are full */
  int cov0;        /* >= 0: that coverage for the pixel lo == hi (Y-major); -1: the X-major formula */
  int sh;          /* a down-left right edge's last row: the formula's x is shifted by 1 */
} VC_SpanAA;

enum {
  VC_SPAN_MAXRUN = VC_POLY_MAXV + 2,
  VC_SPAN_WIRE = 1,    /* wire-frame: only the mask's pixels (alpha 31) */
  VC_SPAN_AA = 2,      /* antialiasing: the coverage comes from al/ar */
  VC_SPAN_ALLEDGE = 4, /* every pixel is an edge pixel (the last row of a flat-bottomed polygon) */
  VC_SPAN_AAL = 8,     /* al is valid */
  VC_SPAN_AAR = 16,    /* ar is valid */
  VC_SPAN_VERTL = 32   /* the left edge is vertical (its pixel has full coverage in AA mode) */
};

typedef struct VC_Span {
  int y, flags;
  int xs;       /* the origin of the attribute fractions: f = (x - xs) / den_f */
  int xe;       /* the last pixel before antialiasing (an edge pixel); depth's fraction is (x - xs) / den_z */
  int dxs, dxe; /* the pixels drawn, inclusive, not clamped to the screen (dxe < dxs: none) */
  int den_f, den_z;
  int xmax;                    /* the screen's last column */
  int Llo, Lhi, Rlo, Rhi;      /* the left and right edges' pixel runs (the coverage of an edge pixel in them) */
  VC_SpanEdge le, re;          /* the left and right edges */
  VC_SpanAA al, ar;
  int nrun;                    /* the edge-flag mask: sorted disjoint runs, clamped to the screen */
  int run[VC_SPAN_MAXRUN][2];
  VC_SpanAttr L, R;            /* the attributes at the row's two ends */
} VC_Span;

typedef struct VC_SpanPoly {
  int nv, ne, W, H;
  int wire, full, aa;
  int ymin, ymax, yend, xmin, xmax;
  int flatbot; /* a flat bottom edge (never drawn, even in a polygon drawn whole) */
  int px[VC_POLY_MAXV], py[VC_POLY_MAXV];
  VC_SpanAttr av[VC_POLY_MAXV];
  VC_SpanSeg ed[VC_POLY_MAXV];
} VC_SpanPoly;

/* px, py: the vertex positions (NULL: the vertices' sx, sy). Returns 0 when nothing can be drawn, 1 for a polygon
 * with edges (rows ymin..yend), 2 for a polygon whose vertices all lie on one pixel (the 1-dot rule). */
int vc_span_poly(VC_SpanPoly *sp, const struct VCoreState *s, const VC_Poly *p, const int *px, const int *py, int W, int H);
/* row y of the polygon: 1 and *r filled when the row draws something */
int vc_span_row(const VC_SpanPoly *sp, int y, VC_Span *r);
/* the coverage 0-31 of pixel x on row y of an edge whose window is r (VC_SpanAA); left: the left edge */
int vc_span_aa_cov(const VC_SpanEdge *e, int y, int left, const VC_SpanAA *r, int x);
/* p16's s64 row arithmetic, for port/gpu's test of its 32-bit forms (port/gpu/tests/t02_spans.c) */
int vc_span_t_edge_span(const VC_SpanSeg *e, int y, int *lo, int *hi, int *rd);
int vc_span_t_xmajor_q(const VC_SpanSeg *e, int k);
int vc_span_t_aa_covx(const VC_SpanEdge *e, int k, int j, int left);
int vc_span_t_aa_edge(const VC_SpanSeg *e, int y, int left, VC_SpanAA *r);
void vc_span_t_edge_mid(const VC_SpanSeg *e, int y, s64 *n, s64 *d);
/* DISP_1DOT_DEPTH and POLYGON_ATTR bit 13: a one-pixel polygon is drawn */
int vc_span_dot_visible(const struct VCoreState *s, const VC_Poly *p);

#endif
