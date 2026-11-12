/* port/gpu piece G2: the span records (docs/gpu.md, G2). OWNED BY G2.
 *
 * vcore p16's own edge walk (p16_raster.c: vc_span_poly, vc_span_row), run on the N x grid, packed into the records of
 * g02_spans.h. At N = 1 the rows are vcore's exactly: the same code produced them. At N > 1 the polygon's vertices are
 * the viewport transform of its clip coordinates at N times the viewport (gpu_vertex_pos) and the walk follows the
 * same rules on the bigger grid (the dropped right and bottom edges are one N x pixel). Two kinds of polygon keep the
 * DS look (options, see below): a 1-dot polygon (all its vertices on one DS pixel) is an N x N block, and lines
 * (zero-area polygons) and wire-frames are walked on the DS grid and widened to N, so they are N wide.
 *   BWPORT_GPU_DOT  = block (default) | fine     BWPORT_GPU_LINE = wide (default) | thin */
#include "g02_spans.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gpu_vertex_pos(const VCoreState *s, const VC_Poly *poly, const VC_Vertex *v, int scale, int *x, int *y) {
  if (scale <= 1) {
    *x = v->sx;
    *y = v->sy;
    return;
  }
  /* GBATEK 44: screen_x = (x + w) * width / 2w + x1 (vcore p14's vc_p14_viewport), on a screen N times bigger */
  (void)s;
  int x1 = (int)(poly->viewport & 0xFF), y1 = (int)((poly->viewport >> 8) & 0xFF);
  int x2 = (int)((poly->viewport >> 16) & 0xFF), y2 = (int)(poly->viewport >> 24);
  int64_t w = v->clip[3] > 0 ? v->clip[3] : 1;
  int64_t vw = (int64_t)((x2 - x1 + 1) & 0x1FF) * scale;
  int64_t vh = (int64_t)((y2 - y1 + 1) & 0xFF) * scale;
  *x = (int)(((int64_t)(v->clip[0] + w) * vw) / (2 * w)) + x1 * scale;
  *y = (int)(((int64_t)(w - v->clip[1]) * vh) / (2 * w)) + (191 - y2) * scale;
}

static GpuSpan *new_span(GpuSpans *sp) {
  if (sp->nspan == sp->capspan) {
    int cap = sp->capspan ? sp->capspan * 2 : 65536;
    GpuSpan *n = realloc(sp->span, (size_t)cap * sizeof *n);
    if (n == NULL) return NULL;
    sp->span = n;
    sp->capspan = cap;
  }
  GpuSpan *r = &sp->span[sp->nspan++];
  memset(r, 0, sizeof *r);
  return r;
}

typedef struct Iv {
  int lo, hi;
} Iv;

/* the row's edge-flag mask at horizontal scale m: p16's runs plus the row's last pixel, sorted and merged */
static int build_mask(const VC_Span *v, int m, Iv *out) {
  Iv t[VC_SPAN_MAXRUN + 1];
  int n = 0, xmax = (v->xmax + 1) * m - 1;
  for (int i = 0; i < v->nrun; i++) {
    t[n].lo = v->run[i][0] * m;
    t[n].hi = v->run[i][1] * m + m - 1;
    n++;
  }
  if (v->xe >= 0 && v->xe <= v->xmax) { /* x == xe is an edge pixel too */
    t[n].lo = v->xe * m;
    t[n].hi = v->xe * m + m - 1;
    n++;
  }
  for (int i = 1; i < n; i++) {
    Iv x = t[i];
    int j = i - 1;
    while (j >= 0 && t[j].lo > x.lo) { t[j + 1] = t[j]; j--; }
    t[j + 1] = x;
  }
  int k = 0;
  for (int i = 0; i < n; i++) {
    if (t[i].hi > xmax) t[i].hi = xmax;
    if (t[i].lo > t[i].hi) continue;
    if (k > 0 && t[i].lo <= out[k - 1].hi + 1) {
      if (t[i].hi > out[k - 1].hi) out[k - 1].hi = t[i].hi;
    } else {
      out[k++] = t[i];
    }
  }
  return k;
}

static float lo_part(double v) { return (float)(v - (double)(float)v); }

/* the inner run (t10.zw, g02_spans.h): lo .. hi with the antialiasing window w (DS pixels w[0] .. w[1] when mm > 1) cut away; a
 * window inside the run keeps its left part (shaders/g02_setup.vert s_inner_cut) */
static void inner_cut(float r[2], const float w[2], float mm) {
  if (w[0] > w[1]) return;
  float a = w[0] * mm, b = w[1] * mm + mm - 1.0f;
  if (b < r[0] || a > r[1]) return;
  if (a <= r[0]) r[0] = b + 1.0f;
  else r[1] = a - 1.0f;
}

void gpu_span_pack(GpuSpan *r, const VC_Span *v, int m, int extra, int *overflow) {
  memset(r, 0, sizeof *r);
  int xmax = (v->xmax + 1) * m - 1;
  int dxs = v->dxs * m, dxe = v->dxe * m + (m - 1);
  if (dxs < 0) dxs = 0;
  if (dxe > xmax) dxe = xmax;
  if (dxe < dxs) { /* nothing on this row */
    r->t[0][0] = 1;
    return;
  }
  r->t[0][0] = (float)dxs;
  r->t[0][1] = (float)dxe;
  r->t[0][2] = (float)(v->den_f * m);
  r->t[0][3] = (float)(v->den_z * m);
  const VC_SpanAttr *L = &v->L, *R = &v->R;
  r->t[1][0] = (float)L->w;
  r->t[1][1] = (float)R->w;
  r->t[1][2] = (float)L->z;
  r->t[1][3] = (float)R->z;
  for (int i = 0; i < 3; i++) {
    r->t[2][i] = (float)L->c[i]; /* an edge's colour is a whole value (p16's edge_attr) */
    r->t[3][i] = (float)R->c[i];
  }
  r->t[2][3] = (float)L->st[0];
  r->t[3][3] = (float)R->st[0];
  r->t[4][0] = (float)L->st[1];
  r->t[4][1] = (float)R->st[1];
  r->t[10][0] = lo_part(L->z);
  r->t[10][1] = lo_part(R->z);
  r->t[11][0] = lo_part(L->st[1]);
  r->t[11][1] = lo_part(R->st[1]);
  r->t[11][2] = lo_part(L->st[0]);
  r->t[11][3] = lo_part(R->st[0]);
  int fl = extra;
  if (v->flags & VC_SPAN_ALLEDGE) fl |= GPU_ROW_ALLEDGE;
  if (v->flags & VC_SPAN_WIRE) fl |= GPU_ROW_WIRE;
  if (v->flags & VC_SPAN_VERTL) fl |= GPU_ROW_VERTL;
  int aa = (v->flags & VC_SPAN_AA) != 0;
  if (aa) fl |= GPU_ROW_AA;
  if (aa && m > 1) fl |= GPU_ROW_DSGRID; /* the windows are in DS pixels: the shader divides x by N */
  if (m > 1) fl |= m << GPU_ROW_M_SHIFT; /* the widening: the shader shades a widened row per sample, and reads DSGRID windows at x / m */
  r->t[4][2] = (float)fl;
  r->t[4][3] = (float)(v->xs * m);

  Iv mk[VC_SPAN_MAXRUN + 1];
  int n = build_mask(v, m, mk), cap = aa ? 2 : 6;
  if (!aa && n > 2) r->t[4][2] = (float)(fl |= GPU_ROW_MASK2);
  if (n > cap) {
    if (overflow) (*overflow)++;
    n = cap;
  }
  for (int i = 0; i < cap; i++) {
    float *t = i < 2 ? r->t[5] + 2 * i : r->t[6 + (i - 2) / 2] + 2 * ((i - 2) & 1);
    t[0] = (float)(i < n ? mk[i].lo : 1);
    t[1] = (float)(i < n ? mk[i].hi : 0);
  }
  if (aa) {
    const VC_SpanAA *a[2] = {&v->al, &v->ar};
    const VC_SpanEdge *e[2] = {&v->le, &v->re};
    int valid[2] = {(v->flags & VC_SPAN_AAL) != 0, (v->flags & VC_SPAN_AAR) != 0};
    for (int s = 0; s < 2; s++) {
      float *ta = r->t[6 + s * 2], *tb = r->t[7 + s * 2];
      ta[0] = (float)(valid[s] ? a[s]->lo : 1);
      ta[1] = (float)(valid[s] ? a[s]->hi : 0);
      ta[2] = (float)(valid[s] ? a[s]->cov0 + 1 + 64 * a[s]->sh : 0);
      ta[3] = (float)e[s]->ax;
      tb[0] = (float)(v->y - e[s]->ay);
      tb[1] = (float)e[s]->dx;
      tb[2] = (float)e[s]->dy;
    }
  }
  float *in = r->t[10] + 2; /* the inner run: between the mask's first two runs, out of both windows */
  in[0] = r->t[5][1] + 1.0f;
  in[1] = r->t[5][2] - 1.0f;
  if (aa) {
    inner_cut(in, r->t[6], m > 1 ? (float)m : 1.0f);
    inner_cut(in, r->t[8], m > 1 ? (float)m : 1.0f);
  }
}

/* ---- p16's row arithmetic in 32-bit integers: pass S's (shaders/g02_setup.vert, GLSL ES 3.00 has no 64-bit integers); each is
 * the GLSL line for line. tests/t02_spans.c checks them against p16's s64 forms (vc_span_t_*) over the whole range. The
 * slope has 18 fraction bits (p16's VCN knob is not followed). Bounds, with |dx| <= 4096, dy <= 3072 (the 16 x grid):
 * a slope step S |k| <= |dx| 2^18 <= 2^30 within the edge's rows (k <= dy), so S k needs no split. */

static int fdiv32(int n, int d) { /* floor(n / d), d > 0 */
  return n >= 0 ? (int)((uint32_t)n / (uint32_t)d) : -(int)(((uint32_t)-n + (uint32_t)d - 1) / (uint32_t)d);
}

int gpu_s32_edge_span(const VC_SpanSeg *e, int y, int *lo, int *hi, int *rd) {
  if (y < e->ay || y > e->by) return 0;
  if (e->dy == 0) {
    *lo = e->ax;
    *hi = e->bx;
    *rd = e->bx - 1;
    return 1;
  }
  uint32_t adx = (uint32_t)(e->dx < 0 ? -e->dx : e->dx), dy = (uint32_t)e->dy, k = (uint32_t)(y - e->ay);
  uint32_t S = adx == dy ? 1u << 18 : adx * ((1u << 18) / dy);
  if (adx <= dy) {
    int f = (int)((S * k) >> 18);
    if (e->dx >= 0) {
      *lo = *hi = e->ax + f;
      *rd = *lo - 1;
    } else {
      *lo = *hi = e->ax - 1 - f;
      *rd = k == 0 ? *lo : *lo - 1;
    }
    return 1;
  }
  if (y == e->by) return 0;
  int q = (int)((S * (k + 1) + (1u << 17)) >> 18);
  *lo = *hi = e->dx > 0 ? e->ax + q : e->ax - q;
  *rd = *lo - 1;
  return 1;
}

int gpu_s32_xmajor_q(const VC_SpanSeg *e, int k) {
  uint32_t adx = (uint32_t)(e->dx < 0 ? -e->dx : e->dx);
  uint32_t S = adx * ((1u << 18) / (uint32_t)e->dy);
  return (int)((S * (uint32_t)(k + 1) + (1u << 17)) >> 18);
}

/* ceil(16 N / |dx|): left c = 15 + it, right c = 16 - it (p16's ceil_div(16 adx + 16 N, adx) - 1, fdiv(16 adx - 16 N, adx)) */
int gpu_s32_aa_covx(const VC_SpanEdge *e, int k, int j, int left) {
  int adx = e->dx < 0 ? -e->dx : e->dx, sg = e->dx > 0 ? 1 : -1;
  int F = 16 * ((2 * j + 1 - 2 * e->ax) * e->dy - sg * adx * (2 * k + 1));
  int q = F >= 0 ? (int)(((uint32_t)F + (uint32_t)adx - 1) / (uint32_t)adx) : -(int)((uint32_t)-F / (uint32_t)adx);
  int c = left ? 15 + q : 16 - q;
  return c < 0 ? 0 : c > 31 ? 31 : c;
}

/* p16's aa_edge. Y-major: the row centre's x Xe = ax 2^20 + sg (2 S (2k + 1) + 1) and the row top's Xt = ax 2^20 + sg 4 S k
 * (units of 2^-20) are kept relative to ax as a whole part and a fraction of 2^20. X-major: the scan for the first and last
 * pixel of positive coverage is closed-form: coverage grows with the pixel on a left edge (c > 0 <=> 16 N > -15 |dx|) and
 * falls on a right edge (c > 0 <=> 16 N <= 15 |dx|), N linear in the pixel. */
int gpu_s32_aa_edge(const VC_SpanSeg *e, int y, int left, VC_SpanAA *r) {
  if (e->dy == 0 || e->dx == 0) return 0;
  int adx = e->dx < 0 ? -e->dx : e->dx, sg = e->dx > 0 ? 1 : -1, dy = e->dy;
  int k = y - e->ay;
  r->cov0 = -1;
  r->sh = 0;
  if (adx <= dy) {
    uint32_t S = adx == dy ? 1u << 18 : (uint32_t)adx * ((1u << 18) / (uint32_t)dy);
    uint32_t A = S * (uint32_t)(2 * k + 1), B = S * (uint32_t)k; /* Xe - ax 2^20 = sg (2A + 1), Xt - ax 2^20 = sg 4B */
    int ui = (int)(A >> 19), uf = (int)(((A & 0x7FFFFu) << 1) | 1u);
    int ti = (int)(B >> 18), tf = (int)((B & 0x3FFFFu) << 2);
    int fl = sg > 0 ? ui : -ui - 1, fr = sg > 0 ? uf : (1 << 20) - uf; /* Xe = (ax + fl) 2^20 + fr, 0 < fr < 2^20 */
    int xl = sg > 0 || tf == 0 ? sg * ti : -ti - 1, xr = sg > 0 || tf == 0 ? tf : (1 << 20) - tf; /* Xt likewise, 0 <= xr */
    int xc = xl + (xr != 0);                                                                       /* ceil(Xt / 2^20) - ax */
    int rnd = e->ax + fl + (fr >= (1 << 19));                                                      /* (Xe + 2^19) >> 20 */
    r->lo = 1;
    r->hi = 0;
    if (left) {
      int cp = fl + 1; /* ceil(Xe / 2^20) - ax */
      if (cp - 1 <= xl && xc <= cp) {
        int cov = ((1 << 20) - fr) >> 15;
        if (cov > 0) { r->first = e->ax + cp - 1; r->lo = r->hi = r->first; r->cov0 = cov; }
        else r->first = e->ax + cp;
      } else r->first = rnd;
    } else {
      int pp = fl;
      if (pp <= xl && xc <= pp + 1) {
        int cov = fr >> 15;
        if (cov > 0) { r->last = e->ax + pp; r->lo = r->hi = r->last; r->cov0 = cov; }
        else r->last = e->ax + pp - 1;
      } else r->last = rnd - 1;
    }
    return 1;
  }
  if (!left && sg < 0 && y == e->by - 1) r->sh = 1;
  int xc = fdiv32(e->ax * dy + sg * adx * k + sg * (adx / 2), dy) + r->sh;
  int span = adx / dy + 3, f, l;
  int M = 32 * e->ax * dy - 16 * dy + 16 * sg * adx * (2 * k + 1); /* 16 N(j) = 32 dy j - M */
  if (left) {
    int jmin = fdiv32(M - 15 * adx, 32 * dy) + 1; /* the first j with 32 dy j - M > -15 adx */
    if (jmin <= xc + span) { f = jmin > xc - span ? jmin : xc - span; l = xc + span; }
    else f = l = xc;
  } else {
    int jmax = fdiv32(M + 15 * adx, 32 * dy) + r->sh; /* the last j with 32 dy (j - sh) - M <= 15 adx */
    if (jmax >= xc - span) { f = xc - span; l = jmax < xc + span ? jmax : xc + span; }
    else f = l = xc;
  }
  if (r->sh && l > f) l--;
  r->lo = xc - span;
  r->hi = xc + span;
  if (left) r->first = f; else r->last = l;
  return 1;
}

/* edge e's x at the centre of row y as n / d (p16's edge_mid); the sign of n1 / d1 - n2 / d2 by the whole parts, then the
 * remainders crossed (each < d <= 2 dy: products < 2^26) */
void gpu_s32_edge_mid(const VC_SpanSeg *e, int y, int *n, int *d) {
  if (e->dy == 0 || y == e->by) {
    *n = e->dy == 0 ? e->ax : e->bx;
    *d = 1;
    return;
  }
  *n = 2 * e->ax * e->dy + e->dx * (2 * (y - e->ay) + 1);
  *d = 2 * e->dy;
}
int gpu_s32_mid_cmp(int n1, int d1, int n2, int d2) {
  int q1 = fdiv32(n1, d1), q2 = fdiv32(n2, d2);
  if (q1 != q2) return q1 < q2 ? -1 : 1;
  int a = (n1 - q1 * d1) * d2, b = (n2 - q2 * d2) * d1;
  return a < b ? -1 : a > b ? 1 : 0;
}

static int env_is(const char *name, const char *val, int dflt) {
  const char *e = getenv(name);
  return e == NULL ? dflt : !strcmp(e, val);
}

static int all_collinear(const VC_SpanPoly *P) {
  int q = -1;
  for (int i = 1; i < P->nv; i++)
    if (P->px[i] != P->px[0] || P->py[i] != P->py[0]) { q = i; break; }
  if (q < 0) return 1;
  int64_t qx = P->px[q] - P->px[0], qy = P->py[q] - P->py[0];
  for (int i = 1; i < P->nv; i++)
    if ((int64_t)(P->px[i] - P->px[0]) * qy - (int64_t)(P->py[i] - P->py[0]) * qx != 0) return 0;
  return 1;
}

static void set_rows(GpuPolyRec *pr, int r0, int r1, int first) {
  pr->t[1][0] = (uint32_t)r0;
  pr->t[1][1] = (uint32_t)r1;
  pr->t[1][2] = (uint32_t)first;
}

static uint32_t (*su_grow(GpuSetup *su, int n))[4] {
  if (su->ntx + n > su->captx) {
    int cap = su->captx ? su->captx * 2 : 16384;
    while (cap < su->ntx + n) cap *= 2;
    uint32_t(*t)[4] = realloc(su->tx, (size_t)cap * sizeof *t);
    if (t == NULL) return NULL;
    su->tx = t;
    su->captx = cap;
  }
  if (su->nfind == su->capfind) {
    int cap = su->capfind ? su->capfind * 2 : 1024;
    uint32_t(*f)[2] = realloc(su->find, (size_t)cap * sizeof *f);
    if (f == NULL) return NULL;
    su->find = f;
    su->capfind = cap;
  }
  uint32_t(*t)[4] = &su->tx[su->ntx];
  su->ntx += n;
  return t;
}

/* pass S's setup of P's records first .. first + n - 1 (g02_spans.h, GpuSetup): a walk from row y0 (records of rows y0 + i / m),
 * or (dot) the 1-dot polygon at (x, y0). -1: out of memory */
static int setup_poly(GpuSpans *sp, const VC_SpanPoly *P, int dot, int m, int first, int n, int y0, int x) {
  GpuSetup *su = &sp->su;
  int ne = dot ? 0 : P->ne, nv = dot ? 1 : P->nv, at = su->ntx;
  uint32_t(*t)[4] = su_grow(su, 3 + ne + 2 * nv);
  if (t == NULL) return -1;
  su->find[su->nfind][0] = (uint32_t)first;
  su->find[su->nfind][1] = (uint32_t)at;
  su->nfind++;
  uint32_t fl = (dot ? GPU_SU_DOT : 0) | (P->wire ? GPU_SU_WIRE : 0) | (P->full ? GPU_SU_FULL : 0) | (P->aa ? GPU_SU_AA : 0) |
                (P->flatbot ? GPU_SU_FLATBOT : 0) | (uint32_t)m << 8 | (uint32_t)ne << 16 | (uint32_t)nv << 20;
  memset(t, 0, (size_t)(3 + ne + 2 * nv) * sizeof *t);
  t[0][0] = fl;
  t[0][1] = (uint32_t)first;
  t[0][2] = (uint32_t)n;
  t[0][3] = (uint32_t)P->W | (uint32_t)P->H << 16;
  t[1][0] = (uint32_t)P->ymin;
  t[1][1] = (uint32_t)P->yend;
  t[1][2] = (uint32_t)y0;
  t[1][3] = (uint32_t)x;
  for (int i = 0; i < ne; i++) {
    const VC_SpanSeg *e = &P->ed[i];
    t[2][i >> 2] |= (uint32_t)(e->ia | e->ib << 4) << (8 * (i & 3));
    t[3 + i][0] = (uint32_t)e->ax;
    t[3 + i][1] = (uint32_t)e->ay;
    t[3 + i][2] = (uint32_t)e->bx;
    t[3 + i][3] = (uint32_t)e->by;
  }
  for (int i = 0; i < nv; i++) {
    const VC_SpanAttr *a = &P->av[i];
    uint32_t *v0 = t[3 + ne + 2 * i], *v1 = t[4 + ne + 2 * i];
    v0[0] = (uint32_t)(int32_t)a->w; /* whole numbers (vc_span_poly) */
    v0[1] = (uint32_t)(int32_t)a->z;
    v0[2] = (uint32_t)(int32_t)a->st[0];
    v0[3] = (uint32_t)(int32_t)a->st[1];
    for (int c = 0; c < 3; c++) v1[c] = (uint32_t)(int32_t)a->c[c];
  }
  return 0;
}

void gpu_setup_extent(const GpuSetup *su, int f, int r0, int r1, int *x0, int *x1) {
  const uint32_t(*t)[4] = (const uint32_t(*)[4])su->tx[su->find[f][1]];
  uint32_t fl = t[0][0];
  int m = (int)((fl >> 8) & 255), ne = (int)((fl >> 16) & 15), lo = 1 << 30, hi = -(1 << 30);
  if (fl & GPU_SU_DOT) {
    lo = hi = (int)t[1][3];
  } else {
    int Y0 = r0 / m, Y1 = r1 / m;
    for (int i = 0; i < ne; i++) {
      int ax = (int)t[3 + i][0], ay = (int)t[3 + i][1], bx = (int)t[3 + i][2], by = (int)t[3 + i][3], dx = bx - ax, dy = by - ay;
      if (by < Y0 || ay > Y1) continue;
      if (dy == 0) {
        if (ax < lo) lo = ax;
        if (bx > hi) hi = bx;
        continue;
      }
      /* rows Y0..Y1 of the edge lie between its x at the first row's top and the last row's bottom */
      int ya = ay > Y0 ? ay : Y0, yb = by < Y1 + 1 ? by : Y1 + 1;
      for (int j = 0; j < 2; j++) {
        int y = j ? yb : ya, xl, xh;
        if (y == ay || y == by) { /* an end: the division below is exact there */
          xl = xh = y == ay ? ax : bx;
        } else {
          int64_t n = (int64_t)dx * (y - ay), q = n >= 0 ? n / dy : -((-n + dy - 1) / dy);
          xl = ax + (int)q;
          xh = xl + (n % dy != 0);
        }
        if (xl < lo) lo = xl;
        if (xh > hi) hi = xh;
      }
    }
    lo -= 3; /* the walk's pixels are within a pixel of the edges, the antialiasing windows within two */
    hi += 3;
  }
  *x0 = lo * m;
  *x1 = hi * m + m - 1;
}

/* the walk's rows of P at horizontal scale m and vertical replication m (m = 1: the grid of P itself) */
static void walk(GpuSpans *sp, const VC_SpanPoly *P, int m, int H, GpuPolyRec *pr) {
  int y0 = P->ymin < 0 ? 0 : P->ymin, y1 = P->yend;
  if (y1 > P->H - 1) y1 = P->H - 1;
  if (y0 > y1) return;
  int r0 = y0 * m, r1 = y1 * m + m - 1;
  if (r1 > H - 1) r1 = H - 1;
  if (r0 > r1) return;
  int first = sp->nspan;
  if (sp->setup && setup_poly(sp, P, 0, m, first, r1 - r0 + 1, y0, 0) != 0) return; /* out of memory: no rows */
  if (!sp->records) {
    sp->nspan += r1 - r0 + 1;
    set_rows(pr, r0, r1, first);
    return;
  }
  VC_Span v;
  for (int y = y0; y <= y1; y++) {
    int ok = vc_span_row(P, y, &v);
    for (int j = 0; j < m && y * m + j <= r1; j++) {
      GpuSpan *r = new_span(sp);
      if (r == NULL) { /* out of memory: the rows so far */
        set_rows(pr, r0, y * m + j - 1, first);
        return;
      }
      if (ok) gpu_span_pack(r, &v, m, 0, &sp->mask_overflow);
      else r->t[0][0] = 1;
    }
  }
  set_rows(pr, r0, r1, first);
}

/* a 1-dot polygon: the pixel (x, y) of P's grid, m x m wide */
static void dot(GpuSpans *sp, const VC_SpanPoly *P, int x, int y, int m, GpuPolyRec *pr) {
  if (x < 0 || y < 0 || x >= P->W || y >= P->H) return;
  VC_Span v;
  memset(&v, 0, sizeof v);
  v.y = y;
  v.xs = v.xe = v.dxs = v.dxe = x;
  v.den_f = v.den_z = 1;
  v.xmax = P->W - 1;
  v.nrun = 1;
  v.run[0][0] = v.run[0][1] = x;
  v.L = v.R = P->av[0];
  int first = sp->nspan;
  if (sp->setup && setup_poly(sp, P, 1, m, first, m, y, x) != 0) return;
  if (!sp->records) {
    sp->nspan += m;
    set_rows(pr, y * m, y * m + m - 1, first);
    return;
  }
  for (int j = 0; j < m; j++) {
    GpuSpan *r = new_span(sp);
    if (r == NULL) return;
    gpu_span_pack(r, &v, m, GPU_ROW_ALLEDGE | GPU_ROW_COV0, &sp->mask_overflow);
  }
  set_rows(pr, y * m, y * m + m - 1, first);
}

static void one_poly(GpuSpans *sp, const VCoreState *s, const VC_Poly *p, int scale, GpuPolyRec *pr) {
  int W = VC_W * scale, H = VC_H * scale;
  /* supersampling k (scale = N k): the look options act on the N grid, as vcore p21 plans a polygon: Dot=fine is one N pixel
   * (a k x k block of samples), Line=thin a line one N pixel wide (the N grid's walk widened by k) */
  const int kk = scale % sp->aa == 0 ? sp->aa : 1;
  pr->t[1][0] = 1; /* no rows */
  pr->t[1][1] = 0;
  VC_SpanPoly P1;
  int k1 = vc_span_poly(&P1, s, p, NULL, NULL, VC_W, VC_H); /* the DS grid */
  if (k1 == 0) return;
  if (k1 == 2) { /* all vertices on one DS pixel */
    if (!vc_span_dot_visible(s, p)) return;
    if (scale > 1 && !sp->dot_block) {
      int x, y;
      VC_SpanPoly PN = P1;
      gpu_vertex_pos(s, p, &s->geo.vtx[p->v[0]], scale / kk, &x, &y);
      PN.W = W / kk;
      PN.H = H / kk;
      dot(sp, &PN, x, y, kk, pr);
    } else {
      dot(sp, &P1, P1.px[0], P1.py[0], scale, pr);
    }
    return;
  }
  if (scale > 1 && sp->line_wide && (P1.wire || all_collinear(&P1))) { /* DS-grid lines, N wide */
    walk(sp, &P1, scale, H, pr);
    return;
  }
  if (scale == 1) {
    walk(sp, &P1, 1, H, pr);
    return;
  }
  const int kl = (P1.wire || all_collinear(&P1)) ? kk : 1; /* Line=thin: a line is one pixel of the N grid wide */
  const int g = scale / kl;
  int px[VC_POLY_MAXV], py[VC_POLY_MAXV];
  for (int i = 0; i < p->nv; i++) gpu_vertex_pos(s, p, &s->geo.vtx[p->v[i]], g, &px[i], &py[i]);
  VC_SpanPoly PN;
  int kn = vc_span_poly(&PN, s, p, px, py, W / kl, H / kl);
  if (kn == 0) return;
  if (kn == 2) { /* collapsed to one pixel of the grid (and not one DS pixel): a point */
    dot(sp, &PN, px[0], py[0], kl, pr);
    return;
  }
  walk(sp, &PN, kl, H, pr);
}

/* the memo (g02_spans.h, GpuMemo): polygon p's inputs to one_poly in k (returns the words) */
static int memo_key(const VCoreState *s, const VC_Poly *p, uint32_t *k) {
  int n = 0;
  k[n++] = p->nv;
  k[n++] = p->attr;
  k[n++] = p->viewport;
  for (int i = 0; i < p->nv; i++) {
    const VC_Vertex *v = &s->geo.vtx[p->v[i]];
    for (int c = 0; c < 4; c++) k[n++] = (uint32_t)v->clip[c];
    k[n++] = (uint32_t)v->sx;
    k[n++] = (uint32_t)v->sy;
    k[n++] = (uint32_t)v->sz;
    k[n++] = (uint32_t)v->color[0] | (uint32_t)v->color[1] << 8 | (uint32_t)v->color[2] << 16;
    k[n++] = (uint32_t)(uint16_t)v->tex[0] | (uint32_t)(uint16_t)v->tex[1] << 16;
  }
  return n;
}

/* entry m's rows and setup for the polygon at drawing place pr, as one_poly would build them. 0: out of memory (nothing done) */
static int memo_apply(GpuSpans *sp, const GpuMemo *m, GpuPolyRec *pr) {
  int first = sp->nspan;
  if (m->ntx) {
    GpuSetup *su = &sp->su;
    int at = su->ntx;
    uint32_t(*t)[4] = su_grow(su, (int)m->ntx);
    if (t == NULL) return 0;
    su->find[su->nfind][0] = (uint32_t)first;
    su->find[su->nfind][1] = (uint32_t)at;
    su->nfind++;
    memcpy(t, m->tx, m->ntx * sizeof *t);
    t[0][1] = (uint32_t)first;
  }
  pr->t[1][0] = m->r0;
  pr->t[1][1] = m->r1;
  pr->t[1][2] = m->r0 <= m->r1 ? (uint32_t)first : 0; /* (set_rows: the first record; no rows leave it 0) */
  sp->nspan += (int)m->nrec;
  return 1;
}

/* the memo for a build of n polygons with its own inputs in[]: 1 when it is used (entries for polygon RAM indices 0..n-1) */
static int memo_ready(GpuSpans *sp, const uint32_t in[8], int n) {
  if (sp->records || !sp->setup || sp->memo_off) return 0;
  if (n > sp->capmemo) {
    int cap = n < 512 ? 512 : n;
    GpuMemo *m = realloc(sp->memo, (size_t)cap * sizeof *m);
    if (m == NULL) return 0;
    for (int i = sp->capmemo; i < cap; i++) m[i].valid = 0;
    sp->memo = m;
    sp->capmemo = cap;
  }
  if (memcmp(sp->memo_in, in, sizeof sp->memo_in) != 0) {
    for (int i = 0; i < sp->capmemo; i++) sp->memo[i].valid = 0;
    memcpy(sp->memo_in, in, sizeof sp->memo_in);
  }
  return 1;
}

/* entry m from the build of the polygon just done: its inputs key[0..nk), rows in pr, records and setup texels from ns, nt on */
static void memo_store(GpuSpans *sp, GpuMemo *m, const uint32_t *key, int nk, const GpuPolyRec *pr, int ns, int nt) {
  int ntx = sp->su.ntx - nt;
  m->valid = ntx <= GPU_MEMO_TX; /* (always: 3 + ne + 2 nv texels) */
  m->hit = m->extok = 0;
  if (!m->valid) return;
  memcpy(m->key, key, (size_t)nk * sizeof *key);
  m->nkey = (uint8_t)nk;
  m->r0 = pr->t[1][0];
  m->r1 = pr->t[1][1];
  m->nrec = (uint32_t)(sp->nspan - ns);
  m->ntx = (uint32_t)ntx;
  memcpy(m->tx, sp->su.tx[nt], (size_t)ntx * sizeof *m->tx);
  if (ntx) m->tx[0][1] = 0;
}

/* the check: the polygon built at pr (records ns.., texels nt..) against entry m's */
static int memo_differs(const GpuSpans *sp, const GpuMemo *m, const GpuPolyRec *pr, int ns, int nt) {
  int ntx = sp->su.ntx - nt;
  if (pr->t[1][0] != m->r0 || pr->t[1][1] != m->r1 || (uint32_t)(sp->nspan - ns) != m->nrec || (uint32_t)ntx != m->ntx) return 1;
  if (m->r0 <= m->r1 && pr->t[1][2] != (uint32_t)ns) return 1;
  for (int i = 0; i < ntx; i++)
    for (int c = 0; c < 4; c++)
      if (sp->su.tx[nt + i][c] != (i == 0 && c == 1 ? (uint32_t)ns : m->tx[i][c])) return 1;
  return 0;
}

void gpu_spans_build(GpuSpans *sp, const VCoreState *s, int scale) {
  const VC_GeomOut *g = &s->geo;
  u32 n = g->npoly > VC_MAX_POLYS ? VC_MAX_POLYS : g->npoly;
  u32 d3 = vc_io16(s, VC_REG_DISP3DCNT);
  int blend = (d3 >> 3) & 1, aa = (d3 >> 4) & 1, emark = (d3 >> 5) & 1;
  if (!sp->opt_set) {
    sp->dot_block = env_is("BWPORT_GPU_DOT", "fine", 0) ? 0 : 1;
    sp->line_wide = env_is("BWPORT_GPU_LINE", "thin", 0) ? 0 : 1;
    sp->opt_set = 1;
  }
  if (sp->aa < 1) sp->aa = 1;
  if (!sp->setup) sp->records = 1;
  sp->nspan = 0;
  sp->su.ntx = sp->su.nfind = 0;
  sp->mask_overflow = 0;
  sp->npoly = (int)n;
  if (!sp->memo_env) {
    const char *e = getenv("BWPORT_GPU_MEMOCHECK");
    sp->memo_check = e != NULL && e[0] == '1';
    e = getenv("BWPORT_GPU_MEMO");
    sp->memo_off = e != NULL && e[0] == '0';
    sp->memo_env = 1;
  }
  const uint32_t in[8] = {(uint32_t)scale, (uint32_t)sp->aa, (uint32_t)sp->dot_block, (uint32_t)sp->line_wide, d3 & 0x38,
                          g->swap_param & 2, vc_io16(s, VC_REG_DISP_1DOT_DEPTH) & 0x7FFFu, 1};
  const int memo = memo_ready(sp, in, (int)n);
  for (u32 k = 0; k < n; k++) {
    const VC_Poly *p = &g->poly[g->order[k]];
    GpuPolyRec *pr = &sp->poly[k];
    memset(pr, 0, sizeof *pr);
    u32 alpha = (p->attr >> 16) & 31;
    u32 fl = (p->translucent ? GPU_PF_TRANS : 0) | (p->front ? GPU_PF_FRONT : 0) |
             ((g->swap_param & 2) ? GPU_PF_WBUF : 0) | (alpha == 0 ? GPU_PF_WIRE : 0);
    if (alpha == 0 || emark || aa || (alpha < 31 && blend)) fl |= GPU_PF_WHOLE;
    pr->t[0][0] = p->attr;
    pr->t[0][1] = p->texparam;
    pr->t[0][2] = p->pltt;
    pr->t[0][3] = fl;
    pr->t[1][3] = alpha == 0 ? 31 : alpha;
    sp->memo_of[k] = NULL;
    if (p->nv < 1 || p->nv > VC_POLY_MAXV) {
      pr->t[1][0] = 1;
      continue;
    }
    if (!memo) {
      one_poly(sp, s, p, scale, pr);
      continue;
    }
    GpuMemo *m = &sp->memo[g->order[k]];
    uint32_t key[GPU_MEMO_KEY];
    int nk = memo_key(s, p, key), ns = sp->nspan, nt = sp->su.ntx;
    sp->memo_of[k] = m;
    sp->memo_polys++;
    if (m->valid && m->nkey == nk && memcmp(m->key, key, (size_t)nk * sizeof *key) == 0) {
      sp->memo_hits++;
      if (!sp->memo_check && memo_apply(sp, m, pr)) {
        m->hit = 1;
        continue;
      }
      if (sp->memo_check) {
        one_poly(sp, s, p, scale, pr);
        sp->memo_bad += memo_differs(sp, m, pr, ns, nt);
        m->hit = 1;
        continue;
      }
    }
    one_poly(sp, s, p, scale, pr);
    memo_store(sp, m, key, nk, pr, ns, nt);
  }
  if (sp->memo_check && ++sp->memo_builds % 600 == 0) {
    fprintf(stderr, "bwport: gpu: MEMOCHECK: %u polygons, %u from the memo, %u differ\n", sp->memo_polys, sp->memo_hits, sp->memo_bad);
    sp->memo_polys = sp->memo_hits = 0;
  }
}

void gpu_spans_free(GpuSpans *sp) {
  free(sp->memo);
  sp->memo = NULL;
  sp->capmemo = 0;
  free(sp->span);
  sp->span = NULL;
  sp->nspan = sp->capspan = 0;
  free(sp->su.tx);
  free(sp->su.find);
  memset(&sp->su, 0, sizeof sp->su);
}
