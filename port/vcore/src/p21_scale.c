/* port/vcore piece 21: the 3D at N x the DS resolution on the CPU (docs/vcore.md, "vcore at N x"). OWNED BY PIECE 21.
 *
 * Follows port/gpu's N x rules (docs/gpu.md): the same edge walk (p16's vc_span_poly / vc_span_row) on a grid N times
 * bigger, the same per-pixel steps as p16's emit_pixel, p18's vc_p18_plot and p19's vc_3d_post (copied here for the
 * N x buffers: the 1x code is untouched), the 2D engines composited by p09 at N x (vc_compose_line once per sub-pixel
 * phase of a DS pixel). With the scale forced to 1 the picture equals the 1x core's (the check). */
#include "vc_shared.h"

#include <stdlib.h>

/* ======== the N x frame ======== */

typedef struct VC_NPoly {
  int kind;         /* 0 nothing, 1 walk on the N x grid, 2 walk on the DS grid widened to N (lines, wire-frames), 3 a dot */
  int r0, r1;       /* the rows it can touch (clamped to the screen); r0 > r1: none */
  int xmin, xmax;   /* its columns (cost estimate) */
  int dx, dy, dm;   /* kind 3: the dm x dm block at (dx, dy) */
  int palpha;       /* the alpha of its pixels (wire-frames 31) */
  int m;            /* kind 2: the walk's grid is n / m times the DS grid, its pixels m wide (m = n: the DS grid) */
  VC_SpanPoly sp;   /* kinds 1 and 2 (the grid it is walked on); kind 3: the attributes of vertex 0 */
} VC_NPoly;

typedef struct VC_NFrame {
  int n, w, h;     /* the N x grid the 3D is drawn on (n = scale x aa) */
  int aa, on, ow, oh; /* supersampling k (1, 2, 4); the picture's scale n / k and size (256 on) x (192 on) per screen */
  size_t npx;
  u32 *color, *depth, *out, *src;
  u8 *oid, *tid, *flags, *cov, *mark, *thick; /* thick: Edge=ds thickening (post phase 1) */
  VC_NPoly *poly;
  u32 *pic;        /* the picture: ow x 2 oh RGBA8, the top screen first (k x k box average of the n x grid) */
  int pic_valid, layer_valid; /* the picture is complete; the 3D layer was drawn this frame */
  u32 d3, aref;    /* DISP3DCNT, ALPHA_TEST_REF (this frame's) */
  u8 *fogd;        /* fog density by depth >> 9 (32768 entries), built by the first fog frame */
  u32 fog_key;     /* the registers the table was built for */
  int fog_have;
} VC_NFrame;

enum { M_FINE = 1, M_THICK = 2 };

static void nframe_free(VC_NFrame *F) {
  if (F == NULL) return;
  free(F->color); free(F->depth); free(F->out); free(F->src);
  free(F->oid); free(F->tid); free(F->flags); free(F->cov); free(F->mark); free(F->thick);
  free(F->poly); free(F->pic); free(F->fogd);
  free(F);
}

void vc_p21_destroy(VCoreState *s) {
  nframe_free(s->p21.f);
  s->p21.f = NULL;
}

int vc_p21_active(const VCoreState *s) {
  static int env = -1;
  if (env < 0) {
    const char *e = getenv("BWPORT_VCORE_NPATH");
    env = e != NULL && e[0] == '1';
  }
  return s->p21.scale > 1 || s->p21.force_n || env;
}

void vc_n_picture_done(VCoreState *s) { if (s->p21.f) s->p21.f->pic_valid = 1; }

int vc_n_begin(VCoreState *s) {
  int n = s->p21.scale < 1 ? 1 : s->p21.scale > VC_MAX_SCALE ? VC_MAX_SCALE : s->p21.scale;
  int aa = s->p21.aa < 1 || n % s->p21.aa != 0 ? 1 : s->p21.aa;
  VC_NFrame *F = s->p21.f;
  if (F != NULL && F->n == n && F->aa == aa) {
    F->pic_valid = F->layer_valid = 0;
    return 1;
  }
  nframe_free(F);
  s->p21.f = NULL;
  F = (VC_NFrame *)calloc(1, sizeof *F);
  if (F == NULL) return 0;
  F->n = n;
  F->aa = aa;
  F->on = n / aa;
  F->ow = VC_W * F->on;
  F->oh = VC_H * F->on;
  F->w = VC_W * n;
  F->h = VC_H * n;
  F->npx = (size_t)F->w * F->h;
  F->color = (u32 *)calloc(F->npx, 4);
  F->depth = (u32 *)calloc(F->npx, 4);
  F->out = (u32 *)calloc(F->npx, 4);
  F->oid = (u8 *)calloc(F->npx, 1);
  F->tid = (u8 *)calloc(F->npx, 1);
  F->flags = (u8 *)calloc(F->npx, 1);
  F->cov = (u8 *)calloc(F->npx, 1);
  F->mark = (u8 *)calloc(F->npx, 1);
  F->thick = (u8 *)calloc(F->npx, 1);
  F->poly = (VC_NPoly *)calloc(VC_MAX_POLYS, sizeof(VC_NPoly));
  F->pic = (u32 *)calloc((size_t)F->ow * F->oh * 2, 4);
  if (!F->color || !F->depth || !F->out || !F->oid || !F->tid || !F->flags || !F->cov || !F->mark || !F->thick || !F->poly || !F->pic) {
    nframe_free(F);
    return 0;
  }
  s->p21.f = F;
  return 1;
}

void vc_n_none(VCoreState *s) {
  VC_NFrame *F = s->p21.f;
  memset(F->out, 0, F->npx * 4);
  F->layer_valid = 1;
}

/* ======== the polygons' walks ======== */

/* GBATEK 44 at N times the polygon's viewport (docs/gpu.md "Positions at N x"; port/gpu's gpu_vertex_pos) */
static void vertex_pos(const VC_Poly *poly, const VC_Vertex *v, int n, int *x, int *y) {
  if (n <= 1) {
    *x = v->sx;
    *y = v->sy;
    return;
  }
  int x1 = (int)(poly->viewport & 0xFF), y1 = (int)((poly->viewport >> 8) & 0xFF);
  int x2 = (int)((poly->viewport >> 16) & 0xFF), y2 = (int)(poly->viewport >> 24);
  s64 w = v->clip[3] > 0 ? v->clip[3] : 1;
  s64 vw = (s64)((x2 - x1 + 1) & 0x1FF) * n;
  s64 vh = (s64)((y2 - y1 + 1) & 0xFF) * n;
  *x = (int)(((s64)(v->clip[0] + w) * vw) / (2 * w)) + x1 * n;
  *y = (int)(((s64)(w - v->clip[1]) * vh) / (2 * w)) + (191 - y2) * n;
}

static int all_collinear(const VC_SpanPoly *P) {
  int q = -1;
  for (int i = 1; i < P->nv; i++)
    if (P->px[i] != P->px[0] || P->py[i] != P->py[0]) { q = i; break; }
  if (q < 0) return 1;
  s64 qx = P->px[q] - P->px[0], qy = P->py[q] - P->py[0];
  for (int i = 1; i < P->nv; i++)
    if ((s64)(P->px[i] - P->px[0]) * qy - (s64)(P->py[i] - P->py[0]) * qx != 0) return 0;
  return 1;
}

static void set_dot(VC_NPoly *np, int x, int y, int m, int W, int H) {
  np->kind = 0;
  if (x < 0 || y < 0 || x >= W || y >= H) return;
  np->kind = 3;
  np->dx = x;
  np->dy = y;
  np->dm = m;
}

static void plan_poly(VCoreState *s, VC_NFrame *F, u32 k) {
  VC_NPoly *np = &F->poly[k];
  const int n = F->n, W = F->w, H = F->h;
  np->kind = 0;
  np->r0 = 1;
  np->r1 = 0;
  np->xmin = np->xmax = 0;
  const VC_Poly *p = &s->geo.poly[s->geo.order[k]];
  if (p->nv < 1 || p->nv > VC_POLY_MAXV) return;
  u32 alpha = (p->attr >> 16) & 31;
  np->palpha = alpha == 0 ? 31 : (int)alpha;
  VC_SpanPoly *P = &np->sp;
  int k1 = vc_span_poly(P, s, p, NULL, NULL, VC_W, VC_H); /* the DS grid */
  if (k1 == 0) return;
  if (k1 == 2) { /* all vertices on one DS pixel: an N x N block (or one N x pixel) */
    if (!vc_span_dot_visible(s, p)) return;
    if (n > 1 && s->p21.dot_fine) { /* Dot=fine: one pixel of the picture's grid (k x k samples under antialiasing) */
      int x, y;
      const int kk = F->aa;
      vertex_pos(p, &s->geo.vtx[p->v[0]], n / kk, &x, &y);
      set_dot(np, x * kk, y * kk, kk, W, H);
      if (np->kind == 3 && kk > 1 && (np->dx + kk > W || np->dy + kk > H)) np->kind = 0;
    } else {
      set_dot(np, P->px[0], P->py[0], n, VC_W, VC_H);
      if (np->kind == 3) { np->dx *= n; np->dy *= n; }
    }
  } else if (n > 1 && !s->p21.line_thin && (P->wire || all_collinear(P))) { /* DS-grid lines, N wide */
    np->kind = 2;
    np->m = n;
  } else if (n == 1) {
    np->kind = 1;
  } else {
    int px[VC_POLY_MAXV], py[VC_POLY_MAXV];
    const int kk = (P->wire || all_collinear(P)) ? F->aa : 1; /* Line=thin: a line is one pixel of the picture's grid wide */
    const int g = n / kk;
    for (int i = 0; i < p->nv; i++) vertex_pos(p, &s->geo.vtx[p->v[i]], g, &px[i], &py[i]);
    VC_SpanPoly PN;
    int kn = vc_span_poly(&PN, s, p, px, py, W / kk, H / kk);
    if (kk > 1 && kn == 1) {
      *P = PN;
      np->kind = 2;
      np->m = kk;
      kn = -1;
    }
    if (kn == 0) return;
    if (kn == -1) {
    } else if (kn == 2) { /* one N x pixel and not one DS pixel: a point */
      set_dot(np, px[0] * kk, py[0] * kk, kk, W, H);
    } else {
      *P = PN;
      np->kind = 1;
    }
  }
  if (np->kind == 0) return;
  int ya, yb;
  if (np->kind == 3) {
    ya = np->dy;
    yb = np->dy + np->dm - 1;
    np->xmin = np->dx;
    np->xmax = np->dx + np->dm;
  } else {
    int m = np->kind == 2 ? np->m : 1;
    int y0 = P->ymin < 0 ? 0 : P->ymin, y1 = P->yend > P->H - 1 ? P->H - 1 : P->yend;
    ya = y0 * m;
    yb = y1 * m + m - 1;
    np->xmin = P->xmin * m;
    np->xmax = (P->xmax + 1) * m;
  }
  if (yb > H - 1) yb = H - 1;
  if (np->xmin < 0) np->xmin = 0;
  if (np->xmax > W) np->xmax = W;
  if (ya > yb) { np->kind = 0; return; }
  np->r0 = ya;
  np->r1 = yb;
}

void vc_n_plan(VCoreState *s) {
  VC_NFrame *F = s->p21.f;
  u32 n = s->geo.npoly > VC_MAX_POLYS ? VC_MAX_POLYS : s->geo.npoly;
  for (u32 k = 0; k < n; k++) plan_poly(s, F, k);
  F->layer_valid = 1;
  F->d3 = vc_io16(s, VC_REG_DISP3DCNT);
  F->aref = vc_io32(s, VC_REG_ALPHA_TEST_REF) & 0x1F;
}

void vc_n_plan_strips(VCoreState *s, int nstrip, int *ys) {
  VC_NFrame *F = s->p21.f;
  const int H = F->h;
  double *cost = (double *)calloc((size_t)H + 2, sizeof(double));
  u32 np = s->geo.npoly > VC_MAX_POLYS ? VC_MAX_POLYS : s->geo.npoly;
  if (cost == NULL) {
    for (int i = 0; i <= nstrip; i++) ys[i] = H * i / nstrip;
    return;
  }
  for (u32 k = 0; k < np; k++) {
    const VC_NPoly *q = &F->poly[k];
    if (q->kind == 0) continue;
    double w = (q->xmax > q->xmin ? q->xmax - q->xmin : 0) * 0.5 + 6.0;
    cost[q->r0] += w;
    cost[q->r1 + 1] -= w;
  }
  double tot = 0, acc = 0;
  for (int y = 0; y < H; y++) { acc += cost[y]; cost[y] = acc + 1.0; tot += cost[y]; }
  ys[0] = 0;
  double run = 0;
  int y = 0;
  for (int i = 1; i < nstrip; i++) {
    double target = tot * i / nstrip;
    while (y < H && run + cost[y] <= target) run += cost[y++];
    ys[i] = y;
  }
  ys[nstrip] = H;
  free(cost);
}

/* ======== the clear (p19's vc_3d_clear at N x) ======== */

static inline u32 w6(u32 c5) { return c5 ? c5 * 2 + 1 : 0; }
static inline u32 rgb555_to_rgb6(u32 c) { return VC_RGB6(w6(c & 31), w6((c >> 5) & 31), w6((c >> 10) & 31)); }
static inline u32 depth_expand(u32 x) { return x * 0x200 + ((x + 1) / 0x8000) * 0x1FF; }

/* rows y0..y1-1 of the clear (rows are independent: p20 runs them on the pool, vc_n_clear runs them all) */
void vc_n_clear_rows(VCoreState *s, int y0, int y1) {
  VC_NFrame *F = s->p21.f;
  const int n = F->n, W = F->w;
  u32 d3 = vc_io16(s, VC_REG_DISP3DCNT);
  u32 cc = vc_io32(s, VC_REG_CLEAR_COLOR);
  u32 cd = vc_io16(s, VC_REG_CLEAR_DEPTH) & 0x7FFF;
  u32 off = vc_io16(s, VC_REG_CLRIMAGE_OFFSET);
  u32 id = (cc >> 24) & 0x3F;
  u32 color = rgb555_to_rgb6(cc & 0x7FFF) | (((cc >> 16) & 31) << 24);
  u32 depth = depth_expand(cd);
  u8 fog = (cc >> 15) & 1 ? VC_F3_FOG : 0;
  int bitmap = (d3 >> 14) & 1;
  const VC_Region *tex = &s->rgn[VC_RGN_TEX];
  const size_t i0 = (size_t)y0 * W, cnt = (size_t)(y1 - y0) * W;
  memset(F->oid + i0, (int)id, cnt);
  memset(F->tid + i0, 0xFF, cnt);
  memset(F->cov + i0, 0, cnt);
  if (!bitmap) {
    for (size_t i = i0; i < i0 + cnt; i++) { F->color[i] = color; F->depth[i] = depth; F->out[i] = color; }
    memset(F->flags + i0, fog, cnt);
    return;
  }
  /* the rear-plane bitmap sampled nearest at the DS pixel (x / N, y / N) */
  for (int y = y0; y < y1; y++) {
    u32 by = ((u32)(y / n) + ((off >> 8) & 0xFF)) & 0xFF;
    for (int x = 0; x < W; x++) {
      u32 bx = ((u32)(x / n) + (off & 0xFF)) & 0xFF;
      u32 a = (by * 256 + bx) * 2;
      u32 pc = vc_rd16(tex, 2 * 0x20000 + a), pd = vc_rd16(tex, 3 * 0x20000 + a);
      size_t i = (size_t)y * W + x;
      F->color[i] = F->out[i] = rgb555_to_rgb6(pc & 0x7FFF) | ((pc & 0x8000) ? 31u << 24 : 0);
      F->depth[i] = depth_expand(pd & 0x7FFF);
      F->flags[i] = (pd & 0x8000) ? VC_F3_FOG : 0;
    }
  }
}

void vc_n_clear(VCoreState *s) { vc_n_clear_rows(s, 0, s->p21.f->h); }

/* ======== the pixel (p16's emit_pixel, p18's vc_p18_plot) ======== */

typedef struct Ctx {
  VCoreState *s;
  VC_NFrame *F;
  const VC_Poly *p;
  VC_TexCtx *tc;
  int wbuf, early, palpha, fmt, mode, filter;
  /* antialiasing (k > 1, the walk on the N k grid): the polygon is SHADED once per output pixel, at the pixel's first sample
   * (the attributes of that sample, so an interior pixel gets what the walk at N would give it, the texture stays as sharp as
   * with antialiasing off), and every sample of the pixel that the polygon covers takes that colour: coverage and depth are
   * per sample, the colour per pixel. st/col: the shaded colour of output pixel X (valid when st[X] == *gen). */
  int snap;
  u32 *st, *col, *gen;
} Ctx;

static inline double ffloor(double v) {
  long long t = (long long)v;
  double d = (double)t;
  return d > v ? d - 1.0 : d;
}
static inline double clampd(double v, double lo, double hi) { return v < lo ? lo : v > hi ? hi : v; }
static inline u32 clamp_depth(double v) {
  if (v < 0) return 0;
  if (v > 16777215.0) return 0xFFFFFF;
  return (u32)(v + 0.5);
}

static inline int depth_test(const Ctx *c, size_t i, u32 depth) {
  u32 old = c->F->depth[i];
  if (c->p->attr & (1u << 14)) {
    u32 d = depth > old ? depth - old : old - depth;
    return d <= 0x200;
  }
  return depth < old;
}

static u32 blend(u32 fb, u32 px, u32 a) {
  u32 out = 0;
  for (int ch = 0; ch < 3; ch++) {
    u32 sh = ch * 8;
    u32 pv = (px >> sh) & 0xFF, fv = (fb >> sh) & 0xFF;
    out |= ((pv * (a + 1) + fv * (31 - a)) >> 5) << sh;
  }
  u32 fa = (fb >> 24) & 0x1F;
  return out | ((a > fa ? a : fa) << 24);
}

static void plot_n(const Ctx *c, size_t i, u32 depth, u32 rgb6a5, int edge, int depth_pass) {
  VC_NFrame *F = c->F;
  const VC_Poly *p = c->p;
  u32 a = (rgb6a5 >> 24) & 0x1F;
  u32 aattr = (p->attr >> 16) & 0x1F;
  if (aattr == 0) a = 31;
  if (a == 0) return;
  if (F->d3 & 4) {
    if (a <= F->aref) return;
  }
  u32 id = (p->attr >> 24) & 0x3F;
  u32 fogbit = (p->attr >> 15) & 1;
  u32 rgb = rgb6a5 & 0xFFFFFF;
  u8 *fl = &F->flags[i];
  depth &= 0xFFFFFF;

  if (c->mode == 3) { /* shadow polygons */
    if (id == 0) {
      if (!depth_pass) *fl |= VC_F3_STENCIL;
      return;
    }
    u32 had = *fl & VC_F3_STENCIL;
    *fl &= (u8)~VC_F3_STENCIL;
    if (!had) return;
    u32 cur = (*fl & VC_F3_TRANS) ? F->tid[i] : F->oid[i];
    if (cur == id) return;
    if (!depth_pass) return;
    u32 old = F->color[i];
    if ((*fl & VC_F3_TRANS) && F->tid[i] == id) return;
    if ((F->d3 & 8) && ((old >> 24) & 0x1F) != 0 && a < 31) F->color[i] = blend(old, rgb, a);
    else F->color[i] = rgb | (a << 24);
    if (!(p->attr & 0x8000)) *fl &= (u8)~VC_F3_FOG;
    *fl |= VC_F3_TRANS | VC_F3_WRITTEN;
    F->tid[i] = (u8)id;
    if (p->attr & 0x800) F->depth[i] = depth;
    return;
  }

  if (!depth_pass) return;

  if (a >= 31) {
    F->color[i] = rgb | (31u << 24);
    F->depth[i] = depth;
    F->oid[i] = (u8)id;
    F->tid[i] = 0xFF;
    *fl = (u8)((*fl & VC_F3_STENCIL) | VC_F3_WRITTEN | (fogbit ? VC_F3_FOG : 0) | (edge ? VC_F3_EDGE : 0));
    return;
  }
  if ((*fl & VC_F3_TRANS) && F->tid[i] == id) return;
  u32 old = F->color[i];
  if ((F->d3 & 8) && ((old >> 24) & 0x1F) != 0) F->color[i] = blend(old, rgb, a);
  else F->color[i] = rgb | (a << 24);
  if (!fogbit) *fl &= (u8)~VC_F3_FOG;
  *fl |= VC_F3_TRANS | VC_F3_WRITTEN;
  F->tid[i] = (u8)id;
  if (p->attr & 0x800) F->depth[i] = depth;
}

/* Filter=bilinear (N > 1 only; port/gpu's g4_filtered, the same arithmetic): the four texels around the sample point (the
 * texel centres are at +0.5, the 12.4 coordinate's fraction gives the weights in 1/16 steps), interpolated in the decoded
 * 6-bit colours. A format with a hard alpha (colour-0 transparent, 4x4 mode 0/1, direct bit 15) keeps its edge hard: a
 * transparent nearest texel stays transparent, an opaque one gets alpha 31, and transparent neighbours take no part in
 * the colour; A3I5 / A5I3 interpolate their alpha and weight the colour by it. */
static u32 tex_sample_bilinear(VC_TexCtx *tc, int fmt, s32 ts, s32 tt) {
  u32 nr = vc_tex_sample_c(tc, ts, tt);
  int smooth_a = fmt == 1 || fmt == 6;
  if (!smooth_a && VC_A5(nr) == 0) return nr;
  s32 ux = ts - 8, uy = tt - 8;
  s32 bx = ux >> 4, by = uy >> 4;
  u32 fx = (u32)(ux & 15), fy = (u32)(uy & 15);
  u32 csum[3] = {0, 0, 0}, wsum = 0, asum = 0;
  for (int j = 0; j < 2; j++)
    for (int i = 0; i < 2; i++) {
      u32 w = (i == 0 ? 16 - fx : fx) * (j == 0 ? 16 - fy : fy);
      u32 t = vc_tex_sample_c(tc, (bx + i) * 16, (by + j) * 16);
      u32 ta = VC_A5(t);
      asum += w * ta;
      u32 cw = smooth_a ? w * ta : (ta == 0 ? 0 : w);
      wsum += cw;
      csum[0] += cw * VC_R6(t);
      csum[1] += cw * ((t >> 8) & 0x3F);
      csum[2] += cw * ((t >> 16) & 0x3F);
    }
  if (wsum == 0) return nr;
  u32 o[3];
  for (int k = 0; k < 3; k++) o[k] = (csum[k] + wsum / 2) / wsum;
  u32 oa = smooth_a ? (asum + 128) >> 8 : 31;
  return VC_RGB6(o[0], o[1], o[2]) | (oa << 24);
}

/* (int)clampd(ffloor(v), 0, 63) without ffloor's round trip through a 64-bit integer: the same value for every
 * v, NaN and v >= 2^63 included (ffloor's conversion takes those to INT64_MIN, which clamps to 0) */
static inline int chan6(double v) {
  if (v >= 9223372036854775808.0) return 0;
  return v >= 63.0 ? 63 : v > 0.0 ? (int)v : 0;
}

/* the polygon's colour for the attributes a (vertex colour, texture, toon) */
static inline u32 shade_n(const Ctx *c, const VC_SpanAttr *a, int alpha) {
  int rgb[3];
  for (int k = 0; k < 3; k++) rgb[k] = chan6((a->c[k] + 1e-6) / 8.0);
  u32 col = VC_RGB6(rgb[0], rgb[1], rgb[2]) | (u32)alpha << 24;
  if (c->fmt != 0) {
    s32 ts = (s32)ffloor(a->st[0] + 1e-6), tt = (s32)ffloor(a->st[1] + 1e-6);
    u32 texel = c->filter ? tex_sample_bilinear(c->tc, c->fmt, ts, tt) : vc_tex_sample_c(c->tc, ts, tt);
    col = vc_tex_blend(c->s, c->p, col, texel);
  } else if (c->mode == 2) {
    col = vc_toon_blend(c->s, c->p, col, VC_RGB6(63, 63, 63) | 31u << 24);
  }
  return col;
}

static inline void put_n(const Ctx *c, int x, int y, u32 depth, u32 col, int edge, int cov);

static inline void emit_n(const Ctx *c, int x, int y, const VC_SpanAttr *a, int alpha, int edge, int cov) {
  VC_NFrame *F = c->F;
  if ((unsigned)x >= (unsigned)F->w || (unsigned)y >= (unsigned)F->h) return;
  put_n(c, x, y, clamp_depth(c->wbuf ? a->w : a->z), shade_n(c, a, alpha), edge, cov);
}

static inline void put_n(const Ctx *c, int x, int y, u32 depth, u32 col, int edge, int cov) {
  VC_NFrame *F = c->F;
  size_t i = (size_t)y * F->w + x;
  u32 before = F->color[i], dbefore = F->depth[i];
  u8 fb = F->flags[i];
  plot_n(c, i, depth, col, edge, depth_test(c, i, depth));
  if (F->color[i] != before || F->flags[i] != fb || F->depth[i] != dbefore) F->cov[i] = (u8)(edge ? cov : 31);
}

/* p16's lerp_px: the attributes at fraction f of a span (w only when needed) */
static inline void lerp_px(const VC_SpanAttr *a, const VC_SpanAttr *b, double f, int needw, VC_SpanAttr *r) {
  double q = f;
  if (a->w != b->w) {
    double k0 = (1.0 - f) / a->w, k1 = f / b->w;
    double sum = k0 + k1;
    q = k1 / sum;
    r->w = needw ? 1.0 / sum : 0.0;
  } else {
    r->w = a->w;
  }
  for (int k = 0; k < 3; k++) r->c[k] = a->c[k] + (b->c[k] - a->c[k]) * q;
  for (int k = 0; k < 2; k++) r->st[k] = a->st[k] + (b->st[k] - a->st[k]) * q;
}

/* span_pixels_n for the antialiased walk (the N k grid, z-buffer): the same per-sample rules, with the shading cached per output
 * pixel (Ctx::st) and no attribute interpolation per sample: only the depth, the test, the edge flag and the write. rsn: the
 * span of the output pixel row's first sample row (the shading attributes). */
static void span_pixels_snap(const Ctx *c, const VC_Span *r, int row, const VC_Span *rsn) {
  const VC_NFrame *F = c->F;
  const int flags = r->flags, early = c->early, sn = c->snap;
  const int wire = (flags & VC_SPAN_WIRE) != 0, aa = (flags & VC_SPAN_AA) != 0;
  const int alledge = (flags & VC_SPAN_ALLEDGE) != 0;
  const int xs = r->xs, xe = r->xe;
  const double den_z = (double)r->den_z, z0 = r->L.z, dzdx = den_z != 0.0 ? (r->R.z - r->L.z) / den_z : 0.0;
  const int nrun = r->nrun;
  int xstart = r->dxs, xend = r->dxe;
  if (xstart < 0) xstart = 0;
  if (xend > F->w - 1) xend = F->w - 1;
  int ri = 0, rbeg = nrun ? r->run[0][0] : 1 << 30, rend = nrun ? r->run[0][1] : -1;
  const double fden = rsn->den_f > 0 ? 1.0 / (double)rsn->den_f : 0.0;
  const u32 gen = *c->gen;
  for (int x = xstart; x <= xend; x++) {
    if (wire) {
      while (x > rend && ri < nrun) {
        ri++;
        rbeg = ri < nrun ? r->run[ri][0] : 1 << 30;
        rend = ri < nrun ? r->run[ri][1] : -1;
      }
      if (x < rbeg || x > rend) continue;
    }
    const u32 depth = clamp_depth(z0 + dzdx * (double)(x - xs));
    if (early && !depth_test(c, (size_t)row * F->w + x, depth)) continue;
    while (x > rend && ri < nrun) {
      ri++;
      rbeg = ri < nrun ? r->run[ri][0] : 1 << 30;
      rend = ri < nrun ? r->run[ri][1] : -1;
    }
    int isedge = (x >= rbeg && x <= rend) || (x == xe) || alledge;
    int cov = 31;
    if (wire) {
      isedge = 1;
    } else if (aa) {
      if ((flags & VC_SPAN_AAL) && x >= r->al.lo && x <= r->al.hi) cov = vc_span_aa_cov(&r->le, r->y, 1, &r->al, x);
      if ((flags & VC_SPAN_AAR) && !((flags & VC_SPAN_VERTL) && x == r->le.ax) && x >= r->ar.lo && x <= r->ar.hi) {
        int c2 = vc_span_aa_cov(&r->re, r->y, 0, &r->ar, x);
        if (c2 < cov) cov = c2;
      }
      if (cov < 31) isedge = 1;
    }
    const int X = x / sn;
    if (c->st[X] != gen) { /* shaded once per output pixel, at its first sample */
      VC_SpanAttr as;
      double fs = (double)(X * sn - rsn->xs) * fden;
      lerp_px(&rsn->L, &rsn->R, fs < 0.0 ? 0.0 : fs > 1.0 ? 1.0 : fs, 0, &as);
      c->st[X] = gen;
      c->col[X] = shade_n(c, &as, c->palpha);
    }
    put_n(c, x, row, depth, c->col[X], isedge, cov);
  }
}

/* one row's span: p16's span_pixels with the columns scaled by m (m = N: a span of the DS grid widened to N wide
 * blocks, port/gpu's gpu_span_pack at m; m = 1: the N x grid's own). row: the picture's row; r->y: the walk's row. */
static void span_pixels_n(const Ctx *c, const VC_Span *r, int m, int row, const VC_Span *rsn) {
  if (c->snap > 1 && !c->wbuf && m == 1) {
    span_pixels_snap(c, r, row, rsn);
    return;
  }
  const VC_NFrame *F = c->F;
  const int flags = r->flags, wbuf = c->wbuf, early = c->early;
  const int wire = (flags & VC_SPAN_WIRE) != 0, aa = (flags & VC_SPAN_AA) != 0;
  const int alledge = (flags & VC_SPAN_ALLEDGE) != 0;
  const int xs = r->xs * m, xe_lo = r->xe * m, xe_hi = r->xe * m + m - 1;
  const double den_f = (double)(r->den_f * m), den_z = (double)(r->den_z * m);
  const VC_SpanAttr LV = r->L, RV = r->R, *L = &LV, *R = &RV;
  const int nrun = r->nrun;
  int xstart = r->dxs * m, xend = r->dxe * m + (m - 1);
  if (xstart < 0) xstart = 0;
  if (xend > F->w - 1) xend = F->w - 1;
  VC_SpanAttr a2 = {0};
  const int fast = c->snap > 1 && !wbuf; /* the shading is cached and the depth is linear: no per-sample division */
  const double dzdx = den_z != 0.0 ? (R->z - L->z) / den_z : 0.0;
  int ri = 0, rbeg = nrun ? r->run[0][0] * m : 1 << 30, rend = nrun ? r->run[0][1] * m + m - 1 : -1;
  for (int x = xstart; x <= xend; x++) {
    if (wire) {
      while (x > rend && ri < nrun) {
        ri++;
        rbeg = ri < nrun ? r->run[ri][0] * m : 1 << 30;
        rend = ri < nrun ? r->run[ri][1] * m + m - 1 : -1;
      }
      if (x < rbeg || x > rend) continue;
    }
    double f = fast ? 0.0 : (double)(x - xs) / den_f;
    if (aa) f = f < 0.0 ? 0.0 : f > 1.0 ? 1.0 : f;
    const double az = fast ? L->z + dzdx * (double)(x - xs) : L->z + (R->z - L->z) * ((double)(x - xs) / den_z);
    if (early) {
      size_t i = (size_t)row * F->w + x;
      if (wbuf) lerp_px(L, R, f, 1, &a2);
      if (!depth_test(c, i, clamp_depth(wbuf ? a2.w : az))) continue;
      if (!wbuf && c->snap <= 1) lerp_px(L, R, f, 0, &a2);
    } else if (!(c->snap > 1 && !wbuf)) {
      lerp_px(L, R, f, wbuf, &a2);
    }
    a2.z = az; /*: a2 itself, no copy (`a = a2` right after lerp_px's stores stalled on store forwarding) */
    while (x > rend && ri < nrun) {
      ri++;
      rbeg = ri < nrun ? r->run[ri][0] * m : 1 << 30;
      rend = ri < nrun ? r->run[ri][1] * m + m - 1 : -1;
    }
    int isedge = (x >= rbeg && x <= rend) || (x >= xe_lo && x <= xe_hi) || alledge;
    int cov = 31;
    if (wire) {
      isedge = 1;
    } else if (aa) {
      int xa = m > 1 ? x / m : x; /* the windows are in DS pixels on a widened row */
      /* a pixel outside an edge's window has full coverage: no call */
      if ((flags & VC_SPAN_AAL) && xa >= r->al.lo && xa <= r->al.hi) cov = vc_span_aa_cov(&r->le, r->y, 1, &r->al, xa);
      if ((flags & VC_SPAN_AAR) && !((flags & VC_SPAN_VERTL) && xa == r->le.ax) && xa >= r->ar.lo && xa <= r->ar.hi) {
        int c2 = vc_span_aa_cov(&r->re, r->y, 0, &r->ar, xa);
        if (c2 < cov) cov = c2;
      }
      if (cov < 31) isedge = 1;
    }
    if (c->snap > 1) { /* the colour is the pixel's: shaded once, at the pixel's first sample */
      const int X = x / c->snap;
      if (c->st[X] != *c->gen) {
        VC_SpanAttr as;
        const int xo = X * c->snap;
        double fs = rsn->den_f > 0 ? (double)(xo - rsn->xs) / (double)rsn->den_f : 0.0;
        lerp_px(&rsn->L, &rsn->R, fs < 0.0 ? 0.0 : fs > 1.0 ? 1.0 : fs, 0, &as);
        c->st[X] = *c->gen;
        c->col[X] = shade_n(c, &as, c->palpha);
      }
      put_n(c, x, row, clamp_depth(c->wbuf ? a2.w : a2.z), c->col[X], isedge, cov);
      continue;
    }
    emit_n(c, x, row, &a2, c->palpha, isedge, cov);
  }
}

static void raster_poly(VCoreState *s, VC_NFrame *F, const VC_NPoly *np, u32 k, int y0, int y1, u32 *st, u32 *col, u32 *gen) {
  const VC_Poly *p = &s->geo.poly[s->geo.order[k]];
  Ctx c;
  VC_TexCtx tc;
  c.s = s;
  c.F = F;
  c.p = p;
  c.tc = &tc;
  c.wbuf = (s->geo.swap_param & 2) != 0;
  c.palpha = np->palpha;
  c.fmt = (p->texparam >> 26) & 7;
  c.mode = (p->attr >> 4) & 3;
  c.early = c.mode != 3;
  c.filter = F->n > F->aa && s->p21.filter_bilinear; /* the look options follow Resolution (n / aa), not the supersampling */
  c.snap = np->kind == 1 ? F->aa : 1;
  c.st = st;
  c.col = col;
  c.gen = gen;
  if (c.fmt != 0) vc_tex_ctx_init(&tc, s, p);
  int ya = np->r0 < y0 ? y0 : np->r0, yb = np->r1 > y1 - 1 ? y1 - 1 : np->r1;
  const VC_SpanPoly *P = &np->sp;
  if (np->kind == 3) {
    for (int y = ya; y <= yb; y++)
      for (int x = np->dx; x < np->dx + np->dm; x++) emit_n(&c, x, y, &P->av[0], c.palpha, 1, 0);
    return;
  }
  VC_Span r;
  if (np->kind == 1) {
    VC_Span rsn;
    int srow = -1, sok = 0, orow = -1;
    for (int y = ya; y <= yb; y++) {
      if (!vc_span_row(P, y, &r)) continue;
      const VC_Span *use = &r;
      if (c.snap > 1) {
        const int ys = y - y % c.snap;
        if (ys != srow) { /* the output pixel row's first sample row: its span gives the shading attributes */
          srow = ys;
          if (ys == y) { rsn = r; sok = 1; }
          else sok = vc_span_row(P, ys, &rsn);
        }
        if (sok) use = &rsn;
        if (ys != orow) { orow = ys; ++*c.gen; }
      }
      span_pixels_n(&c, &r, 1, y, use);
    }
    return;
  }
  const int n = np->m;
  int lastds = -1, ok = 0;
  for (int y = ya; y <= yb; y++) { /* kind 2: the walk's row span for each of its m rows */
    int yd = y / n;
    if (yd != lastds) {
      ok = vc_span_row(P, yd, &r);
      lastds = yd;
    }
    if (ok) span_pixels_n(&c, &r, n, y, &r);
  }
}

void vc_n_raster_rows(VCoreState *s, int y0, int y1) {
  VC_NFrame *F = s->p21.f;
  u32 np = s->geo.npoly > VC_MAX_POLYS ? VC_MAX_POLYS : s->geo.npoly;
  u32 st[VC_W * VC_MAX_SCALE / 2 + 1] = {0}, col[VC_W * VC_MAX_SCALE / 2 + 1], gen = 0; /* the shading cache (Ctx::st) */
  for (u32 k = 0; k < np; k++) {
    const VC_NPoly *q = &F->poly[k];
    if (q->kind == 0 || q->r1 < y0 || q->r0 >= y1) continue;
    raster_poly(s, F, q, k, y0, y1, st, col, &gen);
  }
}

/* ======== edge marking, fog, antialiasing (p19's vc_3d_post at N x) ======== */

static void fog_table(VCoreState *s, VC_NFrame *F) {
  u32 key = vc_io16(s, VC_REG_DISP3DCNT) & 0xF00, off = vc_io16(s, VC_REG_FOG_OFFSET);
  u32 sum = 0;
  for (int i = 0; i < 32; i++) sum = sum * 31 + vc_io8(s, VC_REG_FOG_TABLE + i) + 1;
  key = key ^ (off << 12) ^ sum;
  if (F->fogd != NULL && F->fog_have && F->fog_key == key) return;
  if (F->fogd == NULL) F->fogd = (u8 *)malloc(32768);
  if (F->fogd == NULL) return;
  for (u32 d = 0; d < 32768; d++) F->fogd[d] = (u8)vc_p19_fog_density(s, d);
  F->fog_key = key;
  F->fog_have = 1;
}

/* 8 flag bytes at p have none of the bits in mask (skips most of a frame's pixels at memory speed) */
static inline int none8(const u8 *p, u64 mask) {
  u64 w;
  memcpy(&w, p, 8);
  return (w & mask) == 0;
}
#define BYTES(b) ((u64)(b) * 0x0101010101010101ull)

/* The post in phases over rows (the pool runs each phase's rows, vc_n_post runs them all in order; a phase reads
 * only what earlier phases finished, or its own rows, and writes only its own rows, so any split gives the same bytes):
 * 0 the colour copied to out, the marks cleared, the edge test (M_FINE); 1 Edge=ds thickening into F->thick (a marked
 * pixel's run along a row or column, written only where it lands in the phase's rows); 2 the edge colour, fog, AA's copy
 * of out; 3 antialiasing. vc_n_post_begin sets the frame up (the fog table) and returns the number of phases. */
static int post_thick(const VCoreState *s, const VC_NFrame *F) {
  const int T = s->p21.edge_fine ? F->aa : F->n; /* Edge=ds: one DS pixel wide; fine: one pixel of the picture's grid */
  return T;
}

int vc_n_post_begin(VCoreState *s) {
  VC_NFrame *F = s->p21.f;
  u32 d3 = vc_io16(s, VC_REG_DISP3DCNT);
  int aa = (d3 >> 4) & 1, fog = (d3 >> 7) & 1;
  if (fog) fog_table(s, F);
  if (aa && F->src == NULL) F->src = (u32 *)malloc(F->npx * 4);
  return 4;
}

void vc_n_post_rows(VCoreState *s, int phase, int y0, int y1) {
  VC_NFrame *F = s->p21.f;
  const int W = F->w, H = F->h;
  u32 d3 = vc_io16(s, VC_REG_DISP3DCNT);
  int aa = (d3 >> 4) & 1, edge = (d3 >> 5) & 1, fog = (d3 >> 7) & 1, alpha_only = (d3 >> 6) & 1;
  u32 cc = vc_io32(s, VC_REG_CLEAR_COLOR);
  u32 clear_id = (cc >> 24) & 0x3F;
  static const int dxs[4] = {0, 0, -1, 1}, dys[4] = {-1, 1, 0, 0};
  const size_t r0 = (size_t)y0 * W, r1 = (size_t)y1 * W, cnt = r1 - r0;
  const int T = post_thick(s, F);

  if (phase == 0) {
    memcpy(F->out + r0, F->color + r0, cnt * 4);
    memset(F->mark + r0, 0, cnt);
    memset(F->thick + r0, 0, cnt);
    if (!edge) return;
    /* an edge-flagged pixel with a 4-neighbour of another ID and a greater depth (off screen: the rear plane's ID) */
    for (int y = y0; y < y1; y++)
      for (int x = 0; x < W; x++) {
        size_t i = (size_t)y * W + x;
        if ((x & 7) == 0 && x + 8 <= W && none8(F->flags + i, BYTES(VC_F3_EDGE))) { x += 7; continue; }
        if (!(F->flags[i] & VC_F3_EDGE)) continue;
        u32 id = F->oid[i], dep = F->depth[i];
        int hit = 0;
        for (int k = 0; k < 4 && !hit; k++) {
          int nx = x + dxs[k], ny = y + dys[k];
          if (nx < 0 || ny < 0 || nx >= W || ny >= H) hit = clear_id != id;
          else {
            size_t j = (size_t)ny * W + nx;
            hit = F->oid[j] != id && dep < F->depth[j];
          }
        }
        if (hit) F->mark[i] = M_FINE;
      }
    return;
  }

  if (phase == 1) {
    if (!edge || T <= 1) return;
    /* Edge=ds (port/gpu's pass P): also the written pixels of the same ID up to N - 1 pixels from a marked edge pixel
     * along a row or column (the path between them all of that ID): the outline is one DS pixel wide. The marked pixels
     * up to T - 1 rows away are walked too; only the pixels in rows y0..y1-1 are written. */
    int ya = y0 - (T - 1) < 0 ? 0 : y0 - (T - 1), yb = y1 + (T - 1) > H ? H : y1 + (T - 1);
    for (int y = ya; y < yb; y++) {
      const int own = y >= y0 && y < y1;
      for (int x = 0; x < W; x++) {
        size_t i = (size_t)y * W + x;
        if ((x & 7) == 0 && x + 8 <= W && none8(F->mark + i, BYTES(M_FINE))) { x += 7; continue; }
        if (!(F->mark[i] & M_FINE)) continue;
        u32 id = F->oid[i];
        for (int k = 0; k < 4; k++) {
          if (dys[k] == 0 && !own) continue; /* along the row: lands in row y only */
          for (int t = 1; t < T; t++) {
            int nx = x + dxs[k] * t, ny = y + dys[k] * t;
            if (nx < 0 || ny < 0 || nx >= W || ny >= H) break;
            size_t j = (size_t)ny * W + nx;
            if (F->oid[j] != id) break;
            if (ny < y0 || ny >= y1) continue;
            if (F->flags[j] & VC_F3_WRITTEN) F->thick[j] = M_THICK;
          }
        }
      }
    }
    return;
  }

  if (phase == 2) {
    if (edge)
      for (size_t i = r0; i < r1; i++) {
        if ((i & 7) == 0 && i + 8 <= r1 && none8(F->mark + i, BYTES(0xFF)) && none8(F->thick + i, BYTES(0xFF))) { i += 7; continue; }
        if (F->mark[i] | F->thick[i]) {
          u32 ec = rgb555_to_rgb6(vc_io16(s, VC_REG_EDGE_COLOR + (F->oid[i] >> 3) * 2));
          F->out[i] = (ec & 0xFFFFFF) | (31u << 24);
        }
      }
    if (fog) {
      u32 fc = vc_io32(s, VC_REG_FOG_COLOR);
      u32 fcol = rgb555_to_rgb6(fc & 0x7FFF), fa = (fc >> 16) & 31;
      if (F->fogd != NULL)
        for (size_t i = r0; i < r1; i++) {
          if ((i & 7) == 0 && i + 8 <= r1 && none8(F->flags + i, BYTES(VC_F3_FOG))) { i += 7; continue; }
          if (!(F->flags[i] & VC_F3_FOG)) continue;
          u32 D = F->fogd[F->depth[i] >> 9], p = F->out[i];
          u32 a = (fa * D + VC_A5(p) * (128 - D)) >> 7;
          u32 r = VC_R6(p), g = VC_G6(p), b = VC_B6(p);
          if (!alpha_only) {
            r = (VC_R6(fcol) * D + r * (128 - D)) >> 7;
            g = (VC_G6(fcol) * D + g * (128 - D)) >> 7;
            b = (VC_B6(fcol) * D + b * (128 - D)) >> 7;
          }
          F->out[i] = VC_RGB6(r, g, b) | (a << 24);
        }
    }
    if (aa && F->src != NULL) memcpy(F->src + r0, F->out + r0, cnt * 4);
    return;
  }

  if (!aa || F->src == NULL) return;
  for (int y = y0; y < y1; y++)
    for (int x = 0; x < W; x++) {
      size_t i = (size_t)y * W + x;
      if ((x & 7) == 0 && x + 8 <= W && none8(F->flags + i, BYTES(VC_F3_EDGE))) { x += 7; continue; }
      if (!(F->flags[i] & VC_F3_EDGE) || (F->flags[i] & VC_F3_TRANS)) continue;
      u32 a = (F->mark[i] | F->thick[i]) ? 16 : F->cov[i];
      if (a > 31) a = 31;
      u32 p = F->src[i], under = 0, udep = F->depth[i];
      for (int k = 0; k < 4; k++) {
        int nx = x + dxs[k], ny = y + dys[k];
        if (nx < 0 || ny < 0 || nx >= W || ny >= H) continue;
        size_t j = (size_t)ny * W + nx;
        if (F->depth[j] > udep) {
          udep = F->depth[j];
          under = F->src[j];
        }
      }
      u32 ua = VC_A5(under);
      if (ua == 0) {
        F->out[i] = (p & 0xFFFFFF) | (a << 24);
      } else {
        u32 r = ((a + 1) * VC_R6(p) + (31 - a) * VC_R6(under)) / 32;
        u32 g = ((a + 1) * VC_G6(p) + (31 - a) * VC_G6(under)) / 32;
        u32 b = ((a + 1) * VC_B6(p) + (31 - a) * VC_B6(under)) / 32;
        F->out[i] = VC_RGB6(r, g, b) | ((a > ua ? a : ua) << 24);
      }
    }
}

void vc_n_post(VCoreState *s) {
  int np = vc_n_post_begin(s);
  for (int ph = 0; ph < np; ph++) vc_n_post_rows(s, ph, 0, s->p21.f->h);
}

/* ======== the picture ======== */

static inline u32 rgba8(u32 c) {
  u32 r = VC_R6(c), g = VC_G6(c), b = VC_B6(c);
  return ((r << 2) | (r >> 4)) | (((g << 2) | (g >> 4)) << 8) | (((b << 2) | (b >> 4)) << 16) | 0xFF000000u;
}

/* DS row y of a 1x screen (RGB6) as N rows of the picture at region row0 (N = the picture's scale: n / aa) */
static void fill_row(VC_NFrame *F, int region_row0, int y, const u32 *src) {
  const int n = F->on, W = F->ow;
  u32 *d0 = F->pic + (size_t)(region_row0 + y * n) * W;
  for (int x = 0; x < VC_W; x++) {
    u32 v = rgba8(src[x]);
    for (int i = 0; i < n; i++) d0[x * n + i] = v;
  }
  for (int j = 1; j < n; j++) memcpy(d0 + (size_t)j * W, d0, (size_t)W * 4);
}

void vc_n_picture_from_screens(VCoreState *s) {
  VC_NFrame *F = s->p21.f;
  for (int sc = 0; sc < 2; sc++)
    for (int y = 0; y < VC_H; y++) fill_row(F, sc * F->oh, y, s->screen[sc][y]);
  F->pic_valid = 1;
}

/* One row of the 3D layer resolved from the n x grid to the picture's: the k x k samples of each output pixel averaged,
 * premultiplied (colour weighted by its 5-bit alpha, the alpha averaged = the pixel's coverage). Samples that are all alike
 * average to themselves exactly, so an interior (one polygon, shaded once per output pixel) stays what antialiasing off
 * draws; only a geometry edge, where the samples differ, is smoothed. rows: the k sample rows of the n x layer. */
static void resolve_row(u32 *out, const u32 *rows, int W, int ow, int k) {
  const int cnt = k * k, sh = k == 2 ? 2 : 4;
  for (int X = 0; X < ow; X++) {
    const u32 *p0 = rows + (size_t)X * k;
    u32 same = 1, a0 = p0[0];
    for (int r = 0; r < k && same; r++)
      for (int c = 0; c < k; c++)
        if (p0[(size_t)r * W + c] != a0) { same = 0; break; }
    if (same) { out[X] = a0; continue; }
    u32 sa = 0, sr = 0, sg = 0, sb = 0;
    for (int r = 0; r < k; r++)
      for (int c = 0; c < k; c++) {
        u32 px = p0[(size_t)r * W + c], a = VC_A5(px);
        sa += a;
        sr += a * VC_R6(px);
        sg += a * VC_G6(px);
        sb += a * VC_B6(px);
      }
    if (sa == 0) { out[X] = 0; continue; }
    u32 a = (sa + (u32)cnt / 2) >> sh;
    out[X] = VC_RGB6((sr + sa / 2) / sa, (sg + sa / 2) / sa, (sb + sa / 2) / sa) | (a << 24);
  }
}

void vc_n_picture_rows(VCoreState *s, int a_top, int y0, int y1) {
  VC_NFrame *F = s->p21.f;
  const int n = F->on, W = F->ow, k = F->aa, rowA = a_top ? 0 : F->oh, rowB = a_top ? F->oh : 0;
  u32 res[VC_W * VC_MAX_SCALE]; /* the 3D layer's row at the picture's grid (ow <= 256 * 8) */
  const int hofs = vc_eng16(s, VC_ENG_A, VC_REG_BG0HOFS) & 0x1FF;
  const u16 mb = vc_eng16(s, VC_ENG_A, VC_REG_MASTER_BRIGHT);
  const int bm = (mb >> 14) & 3, bright = (bm == 1 || bm == 2) && (mb & 0x1F) != 0;
  for (int y = y0; y < y1; y++) {
    fill_row(F, rowB, y, s->eng_out[VC_ENG_B][y]);
    VC_Line *ln = &s->p20.stage[y];
    if (!ln->bg3d) { /* this line has no 3D layer: its 1x pixels (engine A's display line) scaled */
      fill_row(F, rowA, y, s->eng_out[VC_ENG_A][y]);
      continue;
    }
    for (int j = 0; j < n; j++) {
      const int yn = y * n + j;
      const u32 *row3;
      if (k > 1) { /* resolve first, compose after: the 2D layers are never drawn above the picture's grid */
        resolve_row(res, F->out + (size_t)yn * k * F->w, F->w, W, k);
        row3 = res;
      } else {
        row3 = F->out + (size_t)yn * W;
      }
      u32 *dst = F->pic + (size_t)(rowA + yn) * W;
      for (int i = 0; i < n; i++) {
        /* the 3D layer's pixel under sub-pixel (i, j) of each DS pixel (BG0HOFS scrolls by N times as many pixels) */
        for (int x = 0; x < VC_W; x++) {
          int pos = (x * n + i + hofs * n) % (2 * W);
          u32 px = pos < W ? row3[pos] : 0;
          ln->l3d[x] = VC_A5(px) ? px : 0;
        }
        vc_compose_line(s, VC_ENG_A, y, ln);
        for (int x = 0; x < VC_W; x++) {
          u32 c = ln->out[x];
          if (bright) c = vc_p10_bright(c, mb);
          dst[x * n + i] = rgba8(c);
        }
      }
    }
  }
}

/* ======== the public interface (vcore.h) ======== */

/* the supersampling the n x picture can have: k = 1, 2 or 4 (aa asked for) lowered until n * k <= VC_MAX_SCALE */
int vcore_aa_limit(int n, int aa) {
  int k = aa >= 4 ? 4 : aa >= 2 ? 2 : 1;
  n = n < 1 ? 1 : n;
  while (k > 1 && n * k > VC_MAX_SCALE) k >>= 1;
  return k;
}

/* s->p21.scale is the grid the 3D is drawn on (picture scale x aa); out_want and aa_want are what the caller asked for */
static void apply_scale(VCoreState *s) {
  int n = s->p21.out_want < 1 ? 1 : s->p21.out_want > VC_MAX_SCALE ? VC_MAX_SCALE : s->p21.out_want;
  int k = vcore_aa_limit(n, s->p21.aa_want);
  if (n * k == s->p21.scale && k == s->p21.aa) return;
  s->p21.scale = n * k;
  s->p21.aa = k;
  /* back to the 1x path: the N x frame (its picture still marked valid) goes, or vcore_picture would keep
     returning the last N x picture (Resolution 5 -> 1 kept showing 1280x1920); frees its memory too */
  if (!vc_p21_active(s)) vc_p21_destroy(s);
}
void vcore_set_scale(VCoreState *s, int n) {
  s->p21.out_want = n;
  apply_scale(s);
}
void vcore_set_aa(VCoreState *s, int k) {
  s->p21.aa_want = k;
  apply_scale(s);
}
int vcore_get_scale(const VCoreState *s) { return s->p21.scale < 1 ? 1 : s->p21.scale / (s->p21.aa < 1 ? 1 : s->p21.aa); }
int vcore_get_aa(const VCoreState *s) { return s->p21.aa < 1 ? 1 : s->p21.aa; }
void vcore_set_options(VCoreState *s, int dot_fine, int line_thin, int edge_fine) {
  s->p21.dot_fine = dot_fine != 0;
  s->p21.line_thin = line_thin != 0;
  s->p21.edge_fine = edge_fine != 0;
}
void vcore_set_filter(VCoreState *s, int bilinear) { s->p21.filter_bilinear = bilinear != 0; }
const uint8_t *vcore_picture(const VCoreState *s, int *w, int *h) {
  const VC_NFrame *F = s->p21.f;
  if (F == NULL || !F->pic_valid || !vc_p21_active(s)) return NULL;
  *w = F->ow;
  *h = F->oh * 2;
  return (const uint8_t *)F->pic;
}
const uint32_t *vcore_layer_n(const VCoreState *s, int *w, int *h) {
  const VC_NFrame *F = s->p21.f;
  if (F == NULL || !F->pic_valid || !F->layer_valid) return NULL;
  *w = F->w;
  *h = F->h;
  return F->out;
}
