/* port/vcore piece 16: the rasterizer (docs/vcore.md, piece 16). OWNED BY PIECE 16.
 * Spec: GBATEK 33 (3D overview), 35 (DISP3DCNT, SWAP_BUFFERS bit 1, DISP_1DOT_DEPTH), 43 (POLYGON_ATTR, the depth
 * test), 44 (screen coordinates), 51 (the small-polygon rule, alpha, antialiasing), 52, 54.
 *
 * The vertices arrive at integer screen positions (piece 14). A polygon is filled row by row: every edge gives, on
 * each row it touches, a run of pixels (a Y-major edge one pixel, an X-major edge a run), the leftmost run is the
 * left edge and the rightmost the right edge. GBATEK 51 "Polygon Size": opaque polygons without edge marking and
 * antialiasing (and translucent ones with alpha blending off) leave out their right and bottom edges, every other
 * polygon is drawn whole except for vertical right edges. Attributes run along the edges and across the row with
 * perspective correction by w; the depth is screen-linear z (or perspective w with SWAP_BUFFERS bit 1). */
#include "vc_shared.h"

#include <math.h>
#include <stdlib.h>

typedef VC_SpanAttr RAttr;
typedef VC_SpanSeg REdge; /* ay <= by; for a horizontal edge ax <= bx; ia, ib: the attributes of a and b */

static s64 fdiv(s64 n, s64 d) { /* floor(n / d), d > 0 */
  s64 q = n / d;
  if ((n % d) != 0 && n < 0) q--;
  return q;
}
static int exact_slope(int dx, int dy) { /* measured: dy a power of two (or dx == dy): the hardware's slope is exact */
  return (dy & (dy - 1)) == 0 || dx == dy || dx == -dy;
}
static s64 rdiv(s64 n, s64 d) { return fdiv(2 * n + d, 2 * d); } /* round half up, d > 0 */

static int slope_bits(void) {
  static int n;
  if (!n) { const char *v = getenv("VCN"); n = v ? atoi(v) : 18; }
  return n;
}
static inline double ffloor(double v) { /* floor for |v| < 2^62 (no libm call) */
  long long t = (long long)v;
  double d = (double)t;
  return d > v ? d - 1.0 : d;
}
static inline double floor_d(double v) { return ffloor(v); }
static inline double ceil_d(double v) { return -ffloor(-v); }
static double clampd(double v, double lo, double hi) { return v < lo ? lo : v > hi ? hi : v; }

/* the pixels edge e covers on row y: 0 when it doesn't touch the row */
static int edge_span(const REdge *e, int y, int *lo, int *hi, int *rd) {
  if (y < e->ay || y > e->by) return 0;
  if (e->dy == 0) {
    *lo = e->ax;
    *hi = e->bx;
    *rd = e->bx - 1;
    return 1;
  }
  int adx = e->dx < 0 ? -e->dx : e->dx;
  int k = y - e->ay;
  /* the hardware steps an edge with a slope truncated to VC_SLOPE_BITS fractional bits (measured: where the line
   * passes exactly through a pixel corner the truncation decides which side it falls) */
  s64 S = adx == e->dy ? (1LL << slope_bits()) : (s64)adx * ((1LL << slope_bits()) / e->dy);
  if (adx <= e->dy) { /* Y-major: one pixel per row, the line floored (measured against the oracle) */
    s64 f = (S * k) >> slope_bits();
    if (e->dx >= 0) {
      *lo = *hi = e->ax + (int)f;
      *rd = *lo - 1; /* a right edge is not drawn itself */
    } else {
      *lo = *hi = e->ax - 1 - (int)f;
      *rd = k == 0 ? *lo : *lo - 1; /* an apex's right edge is not dropped (measured) */
    }
    return 1;
  }
  /* X-major (|dx| > dy): one pixel per row like a Y-major edge, but sampled at the row's bottom: round(|dx| (k+1)
   * / dy), counted from the top vertex outwards (measured against the oracle on right-angled probes; GBATEK says
   * nothing) */
  if (y == e->by) return 0; /* measured: the bottom vertex row belongs to the next edge */
  s64 q = (S * (k + 1) + (1LL << (slope_bits() - 1))) >> slope_bits();
  *lo = *hi = e->dx > 0 ? e->ax + (int)q : e->ax - (int)q;
  *rd = *lo - 1;
  return 1;
}

/* X-major edge e, offset of the row's bottom sample from the top vertex: round(|dx| (k+1) / dy) */
static int xmajor_q(const REdge *e, int k) {
  int adx = e->dx < 0 ? -e->dx : e->dx;
  s64 S = (s64)adx * ((1LL << slope_bits()) / e->dy);
  return (int)((S * (k + 1) + (1LL << (slope_bits() - 1))) >> slope_bits());
}
/* measured: an edge pixel run of an X-major edge on row y of a polygon drawn whole (edge marking, AA, translucent):
 * the pixels between the previous row's sample and this row's. Returns 0 on the top and bottom rows. */
static int edge_run(const REdge *e, int y, int *rlo, int *rhi) {
  int adx = e->dx < 0 ? -e->dx : e->dx;
  if (e->dy == 0 || adx <= e->dy || y < e->ay || y >= e->by) return 0;
  int k = y - e->ay, a = k ? xmajor_q(e, k - 1) : 0, b = xmajor_q(e, k);
  if (e->dx > 0) { *rlo = e->ax + a; *rhi = e->ax + b - 1; }
  else { *rlo = e->ax - b; *rhi = e->ax - a - 1; }
  return 1;
}

/* the attributes at fraction f (0..1, screen-linear) between a and b: colour and texture coordinates and w
 * perspective-correct, z linear */
static RAttr lerp_attr(const RAttr *a, const RAttr *b, double f) {
  RAttr r;
  double q = f;
  if (a->w != b->w) {
    double k0 = (1.0 - f) / a->w, k1 = f / b->w;
    q = k1 / (k0 + k1);
    r.w = 1.0 / (k0 + k1);
  } else {
    r.w = a->w;
  }
  for (int i = 0; i < 3; i++) r.c[i] = a->c[i] + (b->c[i] - a->c[i]) * q;
  for (int i = 0; i < 2; i++) r.st[i] = a->st[i] + (b->st[i] - a->st[i]) * q;
  r.z = a->z + (b->z - a->z) * f;
  return r;
}

/* lerp_attr without z (and without w unless needw): the same arithmetic, what the span loop reads */
static inline void lerp_px(const RAttr *a, const RAttr *b, double f, int needw, RAttr *r) {
  double q = f;
  if (a->w != b->w) {
    double k0 = (1.0 - f) / a->w, k1 = f / b->w;
    double sum = k0 + k1;
    q = k1 / sum;
    r->w = needw ? 1.0 / sum : 0.0;
  } else {
    r->w = a->w;
  }
  for (int i = 0; i < 3; i++) r->c[i] = a->c[i] + (b->c[i] - a->c[i]) * q;
  for (int i = 0; i < 2; i++) r->st[i] = a->st[i] + (b->st[i] - a->st[i]) * q;
}

/* edge e at pixel column xe of row y */
static void edge_attr(RAttr *out, const REdge *e, const RAttr *av, int xe, int y, int right) {
  double f;
  int adx = e->dx < 0 ? -e->dx : e->dx;
  /* measured: along an X-major RIGHT edge the
   * colour is the edge's value one pixel past the row's last edge pixel (plane-exact, 62.6 -> 71-73% of flat-triangle
   * pixels right; the per-row residual, +-0.5 px of the span end, is the rounding of the nominal span width: left open).
   * A left X-major edge is sampled at its own pixel (any shift scored worse). */
  double sh = right ? 1.0 : 0.0;
  if (e->dy == 0 || adx > e->dy)
    f = e->dx != 0 ? ((double)(xe - e->ax) + sh) / (double)e->dx : 0.0;
  else
    f = (double)(y - e->ay) / (double)e->dy;
  RAttr *r = out;
  *r = lerp_attr(&av[e->ia], &av[e->ib], clampd(f, 0.0, 1.0));
  for (int i = 0; i < 3; i++) r->c[i] = ffloor(r->c[i] + 1e-6); /* the edge value is an integer (measured) */
}

/* antialiasing coverage 0-31 of an edge pixel (x, y): the part of the pixel the polygon covers */
static int edge_cov(const VC_SpanEdge *e, int x, int y, int left) {
  if (e->dy == 0) return 31;
  double c;
  if (e->dx == 0) {
    double xc = e->ax;
    c = left ? (x + 0.5) - xc : xc - (x - 0.5);
  } else {
    int adx = e->dx < 0 ? -e->dx : e->dx;
    if (adx <= e->dy) {
      double xc = e->ax + (double)(y - e->ay) * e->dx / e->dy;
      c = left ? (x + 0.5) - xc : xc - (x - 0.5);
    } else {
      double yl = e->ay + (double)(x - e->ax) * e->dy / e->dx;
      double d = yl - y;
      c = ((e->dx > 0) == (left != 0)) ? 0.5 + d : 0.5 - d;
    }
  }
  c = clampd(c, 0.0, 1.0);
  int v = (int)(c * 32.0);
  return v > 31 ? 31 : v;
}


/* antialiasing coverage (fitted on oracle probes; GBATEK gives no formula). Row k of an edge: Y-major
 * (|dx| <= dy): the edge's x at the row centre is ax +- S (k + 0.5) with S the slope truncated to 18 fraction bits,
 * nudged by a hair in the direction of travel (exact 45 degree ties); the pixel holding the edge gets coverage
 * floor(32 d) (d = the part of the pixel inside the polygon) when the edge's x at the row top lies in that pixel,
 * then full pixels; with no such pixel the first full pixel is round(x) (a right edge: the last is round(x) - 1).
 * X-major: the exact slope dx / dy, pixel j has coverage floor(32 (0.5 +- (j + 0.5 - x) / sn)) (+ for a left edge)
 * with x a hair above the exact value, cov 0 pixels are not drawn; a down-left right edge's last row is one pixel
 * further right. A vertical or flat edge has coverage 31. */
typedef VC_SpanAA AAEdge; /* drawn from `first` (left) / to `last` (right) */
static s64 ceil_div(s64 n, s64 d) { return -fdiv(-n, d); }
/* X-major coverage of pixel j, left edge or right: integer arithmetic on t = N / (2 adx) */
static int aa_covx(const VC_SpanEdge *e, int k, int j, int left) {
  int adx = e->dx < 0 ? -e->dx : e->dx, sg = e->dx > 0 ? 1 : -1;
  s64 N = (s64)(2 * j + 1 - 2 * e->ax) * e->dy - (s64)sg * adx * (2 * k + 1);
  s64 c = left ? ceil_div(16LL * adx + 16 * N, adx) - 1 : fdiv(16LL * adx - 16 * N, adx);
  return c < 0 ? 0 : c > 31 ? 31 : (int)c;
}
/* the pixel window of a non-flat, non-vertical edge on row y; returns 0 for a flat or vertical edge */
static int aa_edge(const REdge *e, int y, int left, AAEdge *r) {
  if (e->dy == 0 || e->dx == 0) return 0;
  int adx = e->dx < 0 ? -e->dx : e->dx, sg = e->dx > 0 ? 1 : -1;
  int k = y - e->ay;
  r->cov0 = -1;
  r->sh = 0;
  if (adx <= e->dy) { /* Y-major */
    s64 S = adx == e->dy ? (1LL << 18) : (s64)adx * ((1LL << 18) / e->dy);
    s64 Xe = 2 * (((s64)e->ax << 19) + sg * S * (2 * k + 1)) + sg; /* units of 2^-20 */
    s64 Xt = 2 * (((s64)e->ax << 19) + sg * S * 2 * k);
    r->lo = 1;
    r->hi = 0;
    if (left) {
      s64 cp = ceil_div(Xe, 1 << 20);
      if (((cp - 1) << 20) <= Xt && Xt <= (cp << 20)) {
        int cov = (int)((((cp << 20) - Xe) * 32) >> 20);
        if (cov > 31) cov = 31;
        if (cov > 0) { r->first = (int)cp - 1; r->lo = r->hi = (int)cp - 1; r->cov0 = cov; }
        else r->first = (int)cp;
      } else r->first = (int)((Xe + (1 << 19)) >> 20);
    } else {
      s64 pp = Xe >> 20;
      if ((pp << 20) <= Xt && Xt <= ((pp + 1) << 20)) {
        int cov = (int)(((Xe - (pp << 20)) * 32) >> 20);
        if (cov > 31) cov = 31;
        if (cov > 0) { r->last = (int)pp; r->lo = r->hi = (int)pp; r->cov0 = cov; }
        else r->last = (int)pp - 1;
      } else r->last = (int)((Xe + (1 << 19)) >> 20) - 1;
    }
    return 1;
  }
  if (!left && sg < 0 && y == e->by - 1) r->sh = 1;
  const VC_SpanEdge ce = {e->ax, e->ay, e->dx, e->dy};
  /* X-major: scan the window around the row centre x = ax + sg adx (k + 0.5) / dy */
  int xc = (int)fdiv((s64)e->ax * e->dy + (s64)sg * adx * k + (s64)sg * (adx / 2), e->dy) + r->sh;
  int span = adx / e->dy + 3, f = 1 << 30, l = -(1 << 30);
  for (int j = xc - span; j <= xc + span; j++) {
    int c = aa_covx(&ce, k, j - r->sh, left);
    if (c > 0) { if (j < f) f = j; if (j > l) l = j; }
  }
  if (f > l) f = l = xc;
  if (r->sh && l > f) l--; /* measured: that last row's final pixel is not drawn */
  r->lo = xc - span;
  r->hi = xc + span;
  if (left) r->first = f; else r->last = l;
  return 1;
}
static int aa_cov(const VC_SpanEdge *e, int y, int left, const AAEdge *r, int x) {
  if (x < r->lo || x > r->hi) return 31;
  if (r->cov0 >= 0) return r->cov0;
  return aa_covx(e, y - e->ay, x - r->sh, left);
}

/* the coverage of pixel x of a row of an antialiased edge (p21's N x walk calls it) */
int vc_span_aa_cov(const VC_SpanEdge *e, int y, int left, const VC_SpanAA *r, int x) { return aa_cov(e, y, left, r, x); }

int vc_3d_depth_test(const VCoreState *s, const VC_Poly *p, int x, int y, u32 depth) {
  if (x < 0 || x >= VC_W || y < 0 || y >= VC_H) return 0;
  u32 old = s->f3d.depth[y][x];
  if (p->attr & (1u << 14)) { /* equal, with a tolerance of +-0x200 (GBATEK 43) */
    u32 d = depth > old ? depth - old : old - depth;
    return d <= 0x200;
  }
  return depth < old;
}

static u32 clamp_depth(double v) {
  if (v < 0) return 0;
  if (v > 16777215.0) return 0xFFFFFF;
  return (u32)(v + 0.5);
}

/* one pixel with attributes a: colour, texture, then s->plot */
static void emit_pixel(VCoreState *s, const VC_Poly *p, VC_TexCtx *tc, int x, int y, const RAttr *a, int alpha, int edge, int cov) {
  if (x < 0 || x >= VC_W || y < 0 || y >= VC_H) return;
  int wbuf = (s->geo.swap_param & 2) != 0;
  u32 depth = clamp_depth(wbuf ? a->w : a->z);
  int rgb[3];
  /* a->c is the 9-bit value (6 bits + 3 fraction bits); the pixel takes its top 6 bits */
  for (int i = 0; i < 3; i++) rgb[i] = (int)clampd(ffloor((a->c[i] + 1e-6) / 8.0), 0.0, 63.0);
  u32 col = VC_RGB6(rgb[0], rgb[1], rgb[2]) | (u32)alpha << 24;
  int fmt = (p->texparam >> 26) & 7;
  int mode = (p->attr >> 4) & 3;
  s32 ts = 0, tt = 0;
  int needst = fmt != 0 || s->plot != vc_3d_plot;
  if (needst) {
    ts = (s32)ffloor(a->st[0] + 1e-6);
    tt = (s32)ffloor(a->st[1] + 1e-6);
  }
  if (fmt != 0) {
    u32 texel = vc_tex_sample_c(tc, ts, tt);
    col = vc_tex_blend(s, p, col, texel);
  } else if (mode == 2) { /* toon / highlight shade untextured polygons too */
    col = vc_toon_blend(s, p, col, VC_RGB6(63, 63, 63) | 31u << 24);
  }
  if (s->plot != vc_3d_plot) { /* the debug copy; the default plot never reads it (and bands would race on it) */
    s->p16.st[0] = ts;
    s->p16.st[1] = tt;
  }
  u32 before = s->f3d.color[y][x];
  u8 fb = s->f3d.flags[y][x];
  u32 dbefore = s->f3d.depth[y][x];
  s->plot(s, p, x, y, depth, col, edge);
  if (s->f3d.color[y][x] != before || s->f3d.flags[y][x] != fb || s->f3d.depth[y][x] != dbefore)
    s->f3d.coverage[y][x] = (u8)(edge ? cov : 31);
}

int vc_span_dot_visible(const VCoreState *s, const VC_Poly *p) {
  if (p->attr & (1u << 13)) return 1;
  u32 lim = vc_io16(s, VC_REG_DISP_1DOT_DEPTH) & 0x7FFF;
  for (int i = 0; i < p->nv; i++) {
    s32 w = s->geo.vtx[p->v[i]].clip[3];
    if (w < 0 || ((u32)w >> 9) <= lim) return 1;
  }
  return 0;
}

int vc_span_poly(VC_SpanPoly *sp, const VCoreState *s, const VC_Poly *p, const int *pxs, const int *pys, int W, int H) {
  int nv = p->nv;
  if (nv < 1 || nv > VC_POLY_MAXV) return 0;
  const u32 dispcnt = vc_io16(s, VC_REG_DISP3DCNT);
  int wbuf = (s->geo.swap_param & 2) != 0;
  int alpha = (int)((p->attr >> 16) & 31);
  sp->nv = nv;
  sp->W = W;
  sp->H = H;
  sp->wire = alpha == 0;
  sp->aa = (dispcnt >> 4) & 1;
  int emark = (dispcnt >> 5) & 1, blend = (dispcnt >> 3) & 1;
  sp->full = sp->wire || emark || sp->aa || (alpha < 31 && blend);
  int xmin = 1 << 30, xmax = -(1 << 30), ymin = 1 << 30, ymax = -(1 << 30);
  for (int i = 0; i < nv; i++) {
    const VC_Vertex *v = &s->geo.vtx[p->v[i]];
    sp->px[i] = pxs ? pxs[i] : v->sx;
    sp->py[i] = pys ? pys[i] : v->sy;
    double w = v->clip[3] > 0 ? (double)v->clip[3] : 1.0;
    VC_SpanAttr *a = &sp->av[i];
    a->w = w;
    a->z = wbuf ? w : (double)v->sz;
    for (int k = 0; k < 3; k++) {
      int c5 = v->color[k] & 31;
      /* measured by tools/hwfit against the oracle (exact on 55,508 Y-major pixels): a vertex colour is
       * 16*c5+15 (0 for c5 = 0) with 3 fraction bits; GBATEK's 2*c5+(c5>0) is that value >> 3 */
      a->c[k] = c5 ? (double)(16 * c5 + 15) : 0.0;
    }
    a->st[0] = v->tex[0];
    a->st[1] = v->tex[1];
    if (sp->px[i] < xmin) xmin = sp->px[i];
    if (sp->px[i] > xmax) xmax = sp->px[i];
    if (sp->py[i] < ymin) ymin = sp->py[i];
    if (sp->py[i] > ymax) ymax = sp->py[i];
  }
  sp->xmin = xmin;
  sp->xmax = xmax;
  sp->ymin = ymin;
  sp->ymax = ymax;
  sp->ne = 0;
  sp->yend = ymax;
  sp->flatbot = 0;
  /* a 0x0 polygon: one pixel from the first vertex, hidden behind DISP_1DOT_DEPTH unless POLYGON_ATTR bit 13 */
  if (xmin == xmax && ymin == ymax) return 2;

  const int *px = sp->px, *py = sp->py;
  int ne = 0;
  for (int i = 0; i < nv; i++) {
    int j = (i + 1) % nv;
    if (px[i] == px[j] && py[i] == py[j]) continue;
    REdge *e = &sp->ed[ne++];
    int a = i, b = j;
    if (py[a] > py[b] || (py[a] == py[b] && px[a] > px[b])) {
      a = j;
      b = i;
    }
    e->ax = px[a];
    e->ay = py[a];
    e->bx = px[b];
    e->by = py[b];
    e->dx = e->bx - e->ax;
    e->dy = e->by - e->ay;
    e->ia = a;
    e->ib = b;
  }
  sp->ne = ne;
  if (ne == 0) return 0;
  /* measured: a flat bottom edge is never drawn, even when the polygon is drawn whole (translucent, edge-marked, AA) */
  for (int k = 0; k < ne; k++) if (sp->ed[k].dy == 0 && sp->ed[k].ay == ymax && ymax > ymin) sp->flatbot = 1;
  sp->yend = ymax > ymin ? ymax - 1 : ymax;
  return 1;
}

/* edge e's x at the centre of row y (which it touches) as the fraction *n / *d, d > 0: a flat edge's left end, on the bottom
 * vertex's row that vertex's x */
static void edge_mid(const REdge *e, int y, s64 *n, s64 *d) {
  if (e->dy == 0 || y == e->by) {
    *n = e->dy == 0 ? e->ax : e->bx;
    *d = 1;
    return;
  }
  *n = 2 * (s64)e->ax * e->dy + (s64)e->dx * (2 * (y - e->ay) + 1);
  *d = 2 * (s64)e->dy;
}

/* the s64 forms above, for port/gpu's test of its 32-bit forms (port/gpu/tests/t02_spans.c) */
int vc_span_t_edge_span(const VC_SpanSeg *e, int y, int *lo, int *hi, int *rd) { return edge_span(e, y, lo, hi, rd); }
int vc_span_t_xmajor_q(const VC_SpanSeg *e, int k) { return xmajor_q(e, k); }
int vc_span_t_aa_covx(const VC_SpanEdge *e, int k, int j, int left) { return aa_covx(e, k, j, left); }
int vc_span_t_aa_edge(const VC_SpanSeg *e, int y, int left, VC_SpanAA *r) { return aa_edge(e, y, left, r); }
void vc_span_t_edge_mid(const VC_SpanSeg *e, int y, s64 *n, s64 *d) { edge_mid(e, y, n, d); }

int vc_span_row(const VC_SpanPoly *sp, int y, VC_Span *out) {
  if (y < 0 || y >= sp->H) return 0;
  const REdge *ed = sp->ed;
  const RAttr *av = sp->av;
  const int ne = sp->ne, W = sp->W, ymin = sp->ymin, full = sp->full, aa = sp->aa;
  int rlo[VC_SPAN_MAXRUN], rhi[VC_SPAN_MAXRUN], nr = 0;
#define ADDRUN(l, h) \
  do { \
    int _l = (l) < 0 ? 0 : (l), _h = (h) > W - 1 ? W - 1 : (h); \
    if (_l <= _h) { rlo[nr] = _l; rhi[nr] = _h; nr++; } \
  } while (0)
  int Lrun, Llo = 1 << 30, Lhi = 0, Rhi = -(1 << 30), Rlo = 0, le = -1, re = -1, Rrd = 0, Lsteep;
  s64 lmn = 0, lmd = 1, rmn = 0, rmd = 1; /* the left and right edges' x at the row centre, lmn / lmd and rmn / rmd */
  int nonflat = 0, flatrow = 0;
  for (int k = 0; k < ne; k++) if (ed[k].dy == 0 && ed[k].ay == y) flatrow = 1;
  for (int k = 0; k < ne; k++) {
    int lo, hi, rd;
    if (ed[k].dy != 0 && edge_span(&ed[k], y, &lo, &hi, &rd)) nonflat = 1;
  }
  for (int k = 0; k < ne; k++) {
    int lo, hi, rd;
    if (!edge_span(&ed[k], y, &lo, &hi, &rd)) continue;
    if (ed[k].dy != 0 && y == ed[k].by) { /* the vertex row of a passing vertex belongs to the edge that continues */
      int cont = 0;
      for (int j = 0; j < ne; j++) if (ed[j].dy != 0 && ed[j].ay == y) cont = 1;
      if (cont) continue;
    }
    if ((flatrow || y != ymin) && ed[k].dy != 0 && ed[k].ay == y && rd == lo) rd--; /* dropped except at a pointed top */
    if (ed[k].dy == 0 && nonflat) { /* a flat edge only bounds a row no other edge reaches (measured) */
      ADDRUN(lo, hi);
      continue;
    }
    /* which edge bounds the row on the left and which on the right: by the edge's x at the row's centre */
    s64 mn, md;
    edge_mid(&ed[k], y, &mn, &md);
    /* compared exactly (was in double: equal x's reached from different vertices could round apart) */
    s64 cl = mn * lmd - lmn * md, cr = mn * rmd - rmn * md;
    if (le < 0 || cl < 0 || (cl == 0 && lo < Llo)) { lmn = mn; lmd = md; Llo = lo; Lhi = hi; le = k; }
    if (re < 0 || cr > 0 || (cr == 0 && hi > Rhi)) { rmn = mn; rmd = md; Rhi = hi; Rlo = lo; re = k; Rrd = rd; }
    ADDRUN(lo, hi);
  }
  if (le < 0) return 0;
  Lrun = Llo;
  int Rrun = 1 << 30; /* full mode: the right edge's run end replaces its sample */
  if (full) {
    int rl, rh;
    if (edge_run(&ed[le], y, &rl, &rh)) {
      if (rl < Lrun) Lrun = rl;
      ADDRUN(rl, rh);
    }
    if (edge_run(&ed[re], y, &rl, &rh)) {
      Rrun = rh;
      ADDRUN(rl, rh);
    }
  }
  Lsteep = !(ed[le].dx > ed[le].dy);

  VC_Span *r = out;
  r->y = y;
  r->xmax = W - 1;
  r->nrun = 0;
  /* the mask's runs, sorted and merged (insertion sort: there are at most 12) */
  for (int i = 1; i < nr; i++) {
    int l = rlo[i], h = rhi[i], j = i - 1;
    while (j >= 0 && rlo[j] > l) { rlo[j + 1] = rlo[j]; rhi[j + 1] = rhi[j]; j--; }
    rlo[j + 1] = l;
    rhi[j + 1] = h;
  }
  for (int i = 0; i < nr; i++) {
    if (r->nrun > 0 && rlo[i] <= r->run[r->nrun - 1][1] + 1) {
      if (rhi[i] > r->run[r->nrun - 1][1]) r->run[r->nrun - 1][1] = rhi[i];
    } else {
      r->run[r->nrun][0] = rlo[i];
      r->run[r->nrun][1] = rhi[i];
      r->nrun++;
    }
  }
  edge_attr(&r->L, &ed[le], av, Llo, y, 0);
  edge_attr(&r->R, &ed[re], av, Rhi, y, 1);
  if (sp->wire) { /* only the edge pixels, alpha 31; the fraction runs from Llo to Rhi */
    r->flags = VC_SPAN_WIRE;
    r->xs = r->dxs = Llo;
    r->xe = r->dxe = Rhi;
    r->den_f = r->den_z = Rhi > Llo ? Rhi - Llo : 1;
    return 1;
  }

  int xs = Lrun, xe;
  int rvert = ed[re].dx == 0 && ed[re].dy != 0;
  /* measured: an X-major right edge sloping down-right is drawn whole (its run ends before the next row's start);
   * a Y-major (or other) right edge loses its first pixel */
  if (full && !rvert)
    xe = Rhi;
  else
    xe = Rrd;
  if (Rrun != (1 << 30)) xe = Rrun;
  if (xe < Lhi && Lsteep) xe = Lhi; /* a steep left edge's pixels are always drawn (an X-major one's are not: measured) */
  r->flags = 0;
  int dxs = xs, dxe = xe;
  if (aa) {
    r->flags |= VC_SPAN_AA;
    AAEdge al, ar;
    if (aa_edge(&ed[le], y, 1, &al)) { r->flags |= VC_SPAN_AAL; r->al = al; dxs = al.first; }
    if (aa_edge(&ed[re], y, 0, &ar)) { r->flags |= VC_SPAN_AAR; r->ar = ar; dxe = ar.last; }
    if (dxe < dxs) return 0;
  } else if (xe < xs) return 0;
  if (ed[le].dx == 0 && ed[le].dy != 0) r->flags |= VC_SPAN_VERTL;
  if (sp->flatbot && y == sp->yend) r->flags |= VC_SPAN_ALLEDGE;
  r->xs = xs;
  r->xe = xe;
  r->dxs = dxs;
  r->dxe = dxe;
  r->den_f = Rrd - xs + 1 + (rvert ? 0 : 1);
  r->den_z = xe - xs + 2;
  r->Llo = Llo;
  r->Lhi = Lhi;
  r->Rlo = Rlo;
  r->Rhi = Rhi;
  r->le = (VC_SpanEdge){ed[le].ax, ed[le].ay, ed[le].dx, ed[le].dy};
  r->re = (VC_SpanEdge){ed[re].ax, ed[re].ay, ed[re].dx, ed[re].dy};
  return 1;
#undef ADDRUN
}

/* the pixel loop: one row's span, pixel by pixel (the rows' edge walk is vc_span_row's) */
static void span_pixels(VCoreState *s, const VC_Poly *p, VC_TexCtx *tc, const VC_Span *r, int wbuf, int early, int palpha) {
  const int y = r->y, xs = r->xs, xe = r->xe;
  const int flags = r->flags;
  const int wire = (flags & VC_SPAN_WIRE) != 0, aa = (flags & VC_SPAN_AA) != 0;
  const int alledge = (flags & VC_SPAN_ALLEDGE) != 0;
  const double den_f = (double)r->den_f, den_z = (double)r->den_z;
  const RAttr LV = r->L, RV = r->R, *L = &LV, *R = &RV; /* locals: the plot call below makes the compiler reload memory */
  const int nrun = r->nrun, xend = r->dxe < r->xmax ? r->dxe : r->xmax;
  RAttr a2;
#define ADVANCE_RUN   while (x > rend && ri < nrun) {     ri++;     rbeg = ri < nrun ? r->run[ri][0] : 1 << 30;     rend = ri < nrun ? r->run[ri][1] : -1;   }
  int ri = 0, rend = nrun ? r->run[0][1] : -1, rbeg = nrun ? r->run[0][0] : 1 << 30;
  for (int x = r->dxs < 0 ? 0 : r->dxs; x <= xend; x++) {
    if (wire) {
      ADVANCE_RUN;
      if (x < rbeg || x > rend) continue;
    }
    /* measured (hardware vs oracle): along a span the value is floor(L + (R - L) * (x - xl) / den) with
     * den = the number of drawn pixels n (a vertical right edge: exact on 30k colour and 21k texcoord pixels) and
     * n + 1 when the right edge is slanted; colour and texture coordinates alike. Depth keeps the old
     * xr - xl + 2 (not measurable: a probe's vertex z has 4096 depth units per step; p14/p18 score best with it). */
    double f = (double)(x - xs) / den_f;
    if (aa) f = f < 0.0 ? 0.0 : f > 1.0 ? 1.0 : f;
    RAttr a;
    a.z = L->z + (R->z - L->z) * ((double)(x - xs) / den_z);
    if (early) {
      /* a pixel that fails the depth test changes nothing (the plot returns first; shadow polygons excepted):
       * skip its colour, texture and sampling */
      if (wbuf) lerp_px(L, R, f, 1, &a2); /* the depth is w */
      if (!vc_3d_depth_test(s, p, x, y, clamp_depth(wbuf ? a2.w : a.z))) continue;
      if (!wbuf) lerp_px(L, R, f, 0, &a2);
      a2.z = a.z;
      a = a2;
    } else {
      lerp_px(L, R, f, wbuf, &a2);
      a2.z = a.z;
      a = a2;
    }
    /* measured: the last drawn pixel of a row, and every pixel of a flat bottom's last drawn row, are edge pixels */
    ADVANCE_RUN;
    int isedge = (x >= rbeg && x <= rend) || x == xe || alledge;
    int cov = 31;
    if (wire) {
      isedge = 1;
    } else if (aa) {
      if (flags & VC_SPAN_AAL) cov = aa_cov(&r->le, y, 1, &r->al, x);
      if ((flags & VC_SPAN_AAR) && !((flags & VC_SPAN_VERTL) && x == r->le.ax)) { /* measured: a vertical left edge's pixel is full */
        int c2 = aa_cov(&r->re, y, 0, &r->ar, x);
        if (c2 < cov) cov = c2;
      }
      if (cov < 31) isedge = 1;
    } else if (isedge) {
      int inl = x >= r->Llo && x <= r->Lhi, inr = x >= r->Rlo && x <= r->Rhi;
      if (inl) cov = edge_cov(&r->le, x, y, 1);
      if (inr) {
        int c2 = edge_cov(&r->re, x, y, 0);
        if (!inl || c2 < cov) cov = c2;
      }
    }
    emit_pixel(s, p, tc, x, y, &a, palpha, isedge, cov);
  }
#undef ADVANCE_RUN
}

void vc_raster_poly(VCoreState *s, const VC_Poly *p) { vc_raster_band(s, p, 0, VC_H); }

void vc_raster_band(VCoreState *s, const VC_Poly *p, int by0, int by1) {
  VC_SpanPoly sp;
  int kind = vc_span_poly(&sp, s, p, NULL, NULL, VC_W, VC_H);
  if (kind == 0) return;
  if (sp.ymax < by0 || sp.ymin >= by1) return;
  int wbuf = (s->geo.swap_param & 2) != 0;
  int palpha = sp.wire ? 31 : (int)((p->attr >> 16) & 31);
  VC_TexCtx tc;
  if (((p->texparam >> 26) & 7) != 0) vc_tex_ctx_init(&tc, s, p);
  if (kind == 2) {
    if (!vc_span_dot_visible(s, p)) return;
    emit_pixel(s, p, &tc, sp.px[0], sp.py[0], &sp.av[0], palpha, 1, 0);
    return;
  }
  const int early = s->plot == vc_3d_plot && ((p->attr >> 4) & 3) != 3;
  VC_Span r;
  for (int y = sp.ymin < by0 ? by0 : sp.ymin; y <= sp.yend && y < by1; y++) {
    if (!vc_span_row(&sp, y, &r)) continue;
    span_pixels(s, p, &tc, &r, wbuf, early, palpha);
  }
}
