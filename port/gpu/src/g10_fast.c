/* port/gpu piece G10: the fast 3D renderer's triangles (g10_fast.h; docs/gpu-hwraster.md). Built from vcore's geometry like
 * gpu_spans_build's polygon table, with no span records: the GPU's rasterizer finds the pixels. */
#include "g10_fast.h"
#include "p16_raster.h"

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static int grow(float **buf, int *cap, int need) {
  if (*cap >= need) return 0;
  int n = need + need / 2;
  float *o = realloc(*buf, (size_t)n * GPU_FAST_V * sizeof(float));
  if (o == NULL) return -1;
  *buf = o;
  *cap = n;
  return 0;
}

/* one triangle vertex: at (x, y) on the grid with vertex v's attributes */
static void vert(float *o, float x, float y, const VC_Vertex *v, int k, int flags) {
  o[0] = x;
  o[1] = y;
  o[2] = (float)v->sz;
  o[3] = (float)(v->clip[3] > 0 ? v->clip[3] : 1);
  for (int c = 0; c < 3; c++) {
    int c5 = v->color[c] & 31;
    o[4 + c] = (float)(c5 ? 16 * c5 + 15 : 0);
  }
  o[7] = (float)v->tex[0];
  o[8] = (float)v->tex[1];
  o[9] = (float)k;
  o[10] = (float)flags;
  o[11] = 0.0f;
}

/* a filled polygon's outline grown outwards, with AA off (gx, gy: its vertices, shifted by half a pixel as the fan's). The GPU
 * takes a pixel when its centre is inside; the DS (p16) takes an X-major edge's whole run in a row and a left edge's pixel from
 * its floored x, so where two polygons' edges nearly meet (vertices the clipper made on each side, a vertex mid-way along
 * another's edge) the DS leaves no gap and the GPU left single pixels between them: dots along the battle intro's mesh (the
 * encounter effect), a few in the field. So each top or left edge (those whose pixel centres on the line are drawn already,
 * so no pixel on the line itself is added) moves out along its minor axis, 7/8 of a pixel if X-major, 63/64 if Y-major, and the
 * vertices go to where the moved edges meet (gpu_fast_build clips the moved triangles to the polygon's box widened by the grow,
 * clip_tri: the meeting point of a sharp corner is far out, of an edge-on polygon's anywhere along its line; it was pulled back to
 * two pixels until 2026-11-24, which cut the grow short beside a sharp corner and left the gap to a neighbour's corner a single
 * pixel at the corner itself, its sliver's vertex: lost to another GPU's rounding). These amounts close every gap in the
 * intro's captures with the least change against Exact over the 37 (gpu_fastdiff, AA off, with g10_fast.frag's part for the
 * grown pixels: 11.23 -> 11.22 %); moving every edge by 1/2 also drew a row or column more along right and bottom edges. With 4
 * samples there are no gaps (the samples are on the 2N grid): left as it was. Y-major was 1/2 until 2026-11-19: a left edge's
 * pixel is the DS's floored x, so with 1/2 a polygon's left silhouette (no neighbour on that side) lost its column wherever the
 * edge's x had a fraction over 1/2: dark vertical slivers on Platinum's town ground (the black below the 3D layer). Just under 1
 * takes the floored column and never the one left of it. */
static void grow_poly(const int *px, const int *py, int nv, int cw, double *gx, double *gy) {
  double A[VC_POLY_MAXV], B[VC_POLY_MAXV], G[VC_POLY_MAXV];
  for (int i = 0; i < nv; i++) {
    int j = (i + 1) % nv, dx = px[j] - px[i], dy = py[j] - py[i], m = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    double s = m == 0 ? 0 : (cw ? 1.0 : -1.0) / m;
    A[i] = -dy * s, B[i] = dx * s;
    /* top or left: a Y-major edge by the sign of A (a left edge), an X-major one by B (a top edge). A's sign on an X-major edge is
     * only its slope's: a top edge sloping up to the right was "top" and one sloping down was not (and a bottom edge sloping the
     * other way was grown), so a sloped edge shared by two polygons left single pixels between them, exactly where the line
     * passes through a pixel's corner (Black's battle backdrop at 1x: 3850 black pixels over 676 captures, 10501 at 3x) */
    int xmajor = abs(dx) > abs(dy);
    G[i] = m == 0 || !(xmajor ? B[i] > 0 : A[i] > 0) ? 0 : xmajor ? 0.875 : 0.984375;

  }
  for (int i = 0; i < nv; i++) {
    int h = (i + nv - 1) % nv; /* edge h into vertex i, edge i out of it: A d.x + B d.y = -G on both */
    double dx = 0, dy = 0, det = A[h] * B[i] - A[i] * B[h];
    if (det > 1e-3 || det < -1e-3) {
      dx = (-G[h] * B[i] + G[i] * B[h]) / det;
      dy = (-A[h] * G[i] + A[i] * G[h]) / det;
    } else { /* (nearly) one line, or a vertex on a zero-length edge: out along the edge that has a length */
      int e = A[i] != 0 || B[i] != 0 ? i : h;
      double n2 = A[e] * A[e] + B[e] * B[e], g = G[h] > G[i] ? G[h] : G[i];
      if (n2 > 0) dx = -g * A[e] / n2, dy = -g * B[e] / n2;
    }
    gx[i] = px[i] + 0.5 + dx, gy[i] = py[i] + 0.5 + dy;
  }
}

/* fan triangle o's three vertices moved to (x0, y0) .. (x2, y2), their attributes taken from the triangle's own planes (screen-
 * linear z, 1 / w and the others over w, as the GPU interpolates them), so its pixels inside keep their values */
static void move_tri(float *o, double x0, double y0, double x1, double y1, double x2, double y2) {
  const double q[3][2] = {{x0, y0}, {x1, y1}, {x2, y2}};
  const float *a = o, *b = o + GPU_FAST_V, *c = o + 2 * GPU_FAST_V;
  double e1x = b[0] - a[0], e1y = b[1] - a[1], e2x = c[0] - a[0], e2y = c[1] - a[1], D = e1x * e2y - e1y * e2x;
  float n[3][9];
  for (int t = 0; t < 3; t++) {
    const float *r = o + t * GPU_FAST_V;
    for (int i = 0; i < 9; i++) n[t][i] = r[i];
    n[t][0] = (float)q[t][0], n[t][1] = (float)q[t][1];
    if (D == 0) continue;
    double ux = q[t][0] - a[0], uy = q[t][1] - a[1];
    double lb = (ux * e2y - uy * e2x) / D, lc = (e1x * uy - e1y * ux) / D, la = 1 - lb - lc;
    double iw = la / a[3] + lb / b[3] + lc / c[3];
    if (!(iw > 1e-12)) continue; /* beyond the horizon: the vertex's own attributes */
    n[t][2] = (float)(la * a[2] + lb * b[2] + lc * c[2]);
    n[t][3] = (float)(1 / iw);
    for (int i = 4; i < 9; i++) n[t][i] = (float)((la * a[i] / a[3] + lb * b[i] / b[3] + lc * c[i] / c[3]) / iw);
  }
  for (int t = 0; t < 3; t++)
    for (int i = 0; i < 9; i++) o[t * GPU_FAST_V + i] = n[t][i];
}

/* the attributes at (x, y) of triangle t's planes (z and 1 / w screen-linear, the others over w, as the GPU interpolates them; t's
 * first vertex's beyond the horizon), into the vertex n */
static void tri_at(const float *t, double x, double y, float *n) {
  const float *a = t, *b = t + GPU_FAST_V, *c = t + 2 * GPU_FAST_V;
  double e1x = b[0] - a[0], e1y = b[1] - a[1], e2x = c[0] - a[0], e2y = c[1] - a[1], D = e1x * e2y - e1y * e2x;
  memcpy(n, a, sizeof(float) * GPU_FAST_V);
  n[0] = (float)x, n[1] = (float)y;
  if (D == 0) return;
  double ux = x - a[0], uy = y - a[1];
  double lb = (ux * e2y - uy * e2x) / D, lc = (e1x * uy - e1y * ux) / D, la = 1 - lb - lc;
  double iw = la / a[3] + lb / b[3] + lc / c[3];
  if (!(iw > 1e-12)) return;
  n[2] = (float)(la * a[2] + lb * b[2] + lc * c[2]);
  n[3] = (float)(1 / iw);
  for (int i = 4; i < 9; i++) n[i] = (float)((la * a[i] / a[3] + lb * b[i] / b[3] + lc * c[i] / c[3]) / iw);
}

/* fan triangle o, moved by move_tri (t: as it was), clipped to the box x0 .. x1, y0 .. y1: its part inside as a fan of up to 5
 * triangles at o, their attributes from t's planes; the number of triangles. A grown pixel is never farther from its polygon than
 * the grow, so none lies outside the polygon's box widened by it; but the moved edges of an edge-on polygon (all its corners on
 * nearly one line) meet far out along that line, and its grown pixels drew a line of its colour there. A crossing is computed from
 * the edge's two ends in one order, so the fan's neighbour triangles cut the edge they share at the same point (no gap between) */
static int clip_tri(float *o, const float *t, double x0, double y0, double x1, double y1) {
  double p[8][2], q[8][2];
  int n = 3, out = 0;
  for (int i = 0; i < 3; i++) {
    p[i][0] = o[i * GPU_FAST_V], p[i][1] = o[i * GPU_FAST_V + 1];
    out |= p[i][0] < x0 || p[i][0] > x1 || p[i][1] < y0 || p[i][1] > y1;
  }
  if (!out) return 1;
  for (int s = 0; s < 4; s++) { /* one side at a time (Sutherland-Hodgman): x >= x0, x <= x1, y >= y0, y <= y1 */
    int ax = s >> 1, m = 0;
    double lim = s == 0 ? x0 : s == 1 ? x1 : s == 2 ? y0 : y1, sg = s & 1 ? -1 : 1;
    for (int i = 0; i < n; i++) {
      const double *a = p[i], *b = p[(i + 1) % n];
      int ia = (a[ax] - lim) * sg >= 0, ib = (b[ax] - lim) * sg >= 0;
      if (ia) q[m][0] = a[0], q[m][1] = a[1], m++;
      if (ia != ib) {
        const double *u = a[0] < b[0] || (a[0] == b[0] && a[1] < b[1]) ? a : b, *v = u == a ? b : a;
        q[m][ax] = lim, q[m][1 - ax] = u[1 - ax] + (v[1 - ax] - u[1 - ax]) * (lim - u[ax]) / (v[ax] - u[ax]), m++;
      }
    }
    n = m;
    memcpy(p, q, sizeof(double) * 2 * (size_t)m);
  }
  float w[8][GPU_FAST_V];
  for (int i = 0; i < n; i++) tri_at(t, p[i][0], p[i][1], w[i]);
  for (int i = 1; i + 1 < n; i++) {
    memcpy(o, w[0], sizeof w[0]);
    memcpy(o + GPU_FAST_V, w[i], sizeof w[0]);
    memcpy(o + 2 * GPU_FAST_V, w[i + 1], sizeof w[0]);
    o += 3 * GPU_FAST_V;
  }
  return n < 3 ? 0 : n - 2;
}

/* a band m wide from grid point a to b (vertex attributes va, vb), its pixels every one an edge pixel: a parallelogram over the
 * columns (X-major) or rows (Y-major) a .. b, m more at the far end; corners on pixel boundaries (no half-pixel shift). Only its
 * part from major coordinate lo to hi (grid coordinates, [lo, hi); INT_MIN, INT_MAX: all of it), its attributes as the whole
 * band's there */
static void band_part(GpuFast *f, int ax, int ay, const VC_Vertex *va, int bx, int by, const VC_Vertex *vb, int m, int k,
                      int flags, int lo, int hi) {
  if (abs(bx - ax) >= abs(by - ay) ? bx < ax : by < ay) {
    int t = ax;
    ax = bx, bx = t, t = ay, ay = by, by = t;
    const VC_Vertex *tv = va;
    va = vb, vb = tv;
  }
  float c[4][2];
  if (bx - ax >= abs(by - ay)) {
    c[0][0] = (float)ax, c[0][1] = (float)ay, c[1][0] = (float)(bx + m), c[1][1] = (float)by;
    c[2][0] = (float)(bx + m), c[2][1] = (float)(by + m), c[3][0] = (float)ax, c[3][1] = (float)(ay + m);
  } else {
    c[0][0] = (float)ax, c[0][1] = (float)ay, c[1][0] = (float)(ax + m), c[1][1] = (float)ay;
    c[2][0] = (float)(bx + m), c[2][1] = (float)(by + m), c[3][0] = (float)bx, c[3][1] = (float)(by + m);
  }
  const VC_Vertex *cv[4] = {va, vb, vb, va};
  if (bx - ax < abs(by - ay)) cv[1] = va, cv[3] = vb;
  static const int tri[6] = {0, 1, 2, 0, 2, 3};
  float *o = f->tri + (size_t)f->ntri * GPU_FAST_V;
  for (int i = 0; i < 6; i++) vert(o + i * GPU_FAST_V, c[tri[i]][0], c[tri[i]][1], cv[tri[i]], k, flags);
  f->ntri += 6;
  int xm = bx - ax >= abs(by - ay);
  double P0 = xm ? ax : ay, P1 = (xm ? bx : by) + m;
  if ((lo > P0 || hi < P1) && P1 > P0) { /* the corners moved along the band's two long sides, the attributes from its planes */
    double t0 = lo > P0 ? (lo - P0) / (P1 - P0) : 0.0, t1 = hi < P1 ? (hi - P0) / (P1 - P0) : 1.0, n[4][2];
    const int A0 = 0, A1 = xm ? 1 : 3, B0 = xm ? 3 : 1, B1 = 2; /* side A from corner A0 to A1, side B from B0 to B1 */
    for (int d = 0; d < 2; d++) {
      n[A0][d] = c[A0][d] + t0 * (c[A1][d] - c[A0][d]), n[A1][d] = c[A0][d] + t1 * (c[A1][d] - c[A0][d]);
      n[B0][d] = c[B0][d] + t0 * (c[B1][d] - c[B0][d]), n[B1][d] = c[B0][d] + t1 * (c[B1][d] - c[B0][d]);
    }
    move_tri(o, n[0][0], n[0][1], n[1][0], n[1][1], n[2][0], n[2][1]);
    move_tri(o + 3 * GPU_FAST_V, n[0][0], n[0][1], n[2][0], n[2][1], n[3][0], n[3][1]);
  }
}

static void band(GpuFast *f, int ax, int ay, const VC_Vertex *va, int bx, int by, const VC_Vertex *vb, int m, int k, int flags) {
  band_part(f, ax, ay, va, bx, by, vb, m, k, flags, INT_MIN, INT_MAX);
}

/* The seams between filled polygons (2026-11-24). The DS's spans are inclusive: along an edge two neighbours share, both draw the
 * edge's pixels (the later one wins), and a right edge's pixel of coverage 0 is left to the neighbour (p16 ends the span before
 * it). The GPU gives a pixel on a shared edge to exactly one of the two (the top-left rule), so a rule that leaves a pixel to "the
 * neighbour" leaves it to nobody: g10_fast.frag's coverage-0 skip did, and the grows only partly made up for it (black holes in
 * Black's town on fans of sliver triangles, 47-183 a resolution; HeartGold's T-junctions, the field's edge marking makes each a
 * dash). So each filled polygon's edge is sorted here: inner when another filled polygon's edge runs along it for half its length
 * or more (within 1.5 grid pixels, interiors on opposite sides, overlapping a pixel or more, the same depth there within twice
 * the two planes' slopes: a polygon passing in front never counts), else a silhouette. g10_fast.frag takes the coverage from the
 * silhouette edges alone (pe[3]): an inner edge's pixel is drawn whole, never left out. Where a neighbour's corner lies outside an
 * edge it runs along (a T-junction, two edges overlapping in part) the gap between them gets a sliver drawn as grown pixels (with
 * 4 samples as grown samples: g10_fast4.frag, GPU_FAST_SLIVER). The grows stay: without them on inner edges Cherrygrove's captures had 10-32 gaps a resolution, the 4 samples' 20-29. */
enum { SEAM_IV = 6, SEAM_INS = 2 * VC_POLY_MAXV, SEAM_HASH = 13, SEAM_CELL = 16, SEAM_MINC = 24 }; /* inner parts kept an edge,
  * corners a polygon; the tested pairs' hash, bits; the grid's cell, DS pixels (SEAM_MINC grid pixels at least) */
typedef struct SeamPoly {
  int nv, cw, nins;      /* nv 0: not a filled polygon */
  int px[VC_POLY_MAXV], py[VC_POLY_MAXV];
  float d[VC_POLY_MAXV]; /* depth, linear on the screen: z, or 2^36 / w with w-buffering */
  float slope;           /* the depth's change a grid pixel, |d/dx| + |d/dy| (the plane of its fan triangle of most area) */
  int niv[VC_POLY_MAXV];
  float iv[VC_POLY_MAXV][SEAM_IV][2]; /* edge i's parts along a neighbour's edge (its parameter, 0..1) */
  int ins_e[SEAM_INS], ins_x[SEAM_INS], ins_y[SEAM_INS];
  float ins_t[SEAM_INS]; /* neighbours' corners outside edge ins_e, at its parameter ins_t (its slivers) */
} SeamPoly;
typedef struct GpuFastSeams {
  SeamPoly poly[VC_MAX_POLYS];
  uint32_t hkey[1 << SEAM_HASH]; /* the pairs tested this frame (fast_seams: each once): lo VC_MAX_POLYS VC_POLY_MAXV + hi, where hgen is gen */
  uint16_t hgen[1 << SEAM_HASH], gen;
  int nhash;
  float ux[VC_MAX_POLYS * VC_POLY_MAXV], uy[VC_MAX_POLYS * VC_POLY_MAXV], len[VC_MAX_POLYS * VC_POLY_MAXV]; /* each edge's direction, length */
  struct SeamKey { float r; int b, e, s; } *key; /* a cell's edges by angle and offset; s: the side its interior lies on */
  int capkey;
  int *cell; /* the grid: cell c's keys are key[cell[c] .. cell[c + 1]) */
  struct SeamReg { int c; struct SeamKey k; } *reg; /* (cell, key) pairs as found */
  int capreg;
  struct SeamKey *big; /* a crowded cell's keys sorted */
  int capbig;
} GpuFastSeams;

static void seam_iv(SeamPoly *q, int i, double lo, double hi) {
  if (hi < lo) {
    double t = lo;
    lo = hi, hi = t;
  }
  lo = lo < 0 ? 0 : lo, hi = hi > 1 ? 1 : hi;
  float l = (float)lo, h = (float)hi;
  int m = 0;
  for (int j = 0; j < q->niv[i]; j++) /* (the union: the parts it meets merged into it, the others kept) */
    if (l <= q->iv[i][j][1] && h >= q->iv[i][j][0]) {
      if (q->iv[i][j][0] < l) l = q->iv[i][j][0];
      if (q->iv[i][j][1] > h) h = q->iv[i][j][1];
    } else
      q->iv[i][m][0] = q->iv[i][j][0], q->iv[i][m][1] = q->iv[i][j][1], m++;
  if (m < SEAM_IV) q->iv[i][m][0] = l, q->iv[i][m][1] = h, m++;
  q->niv[i] = m;
}

static void seam_corner(SeamPoly *q, int i, double t, int x, int y) {
  for (int j = 0; j < q->nins; j++)
    if (q->ins_e[j] == i && q->ins_x[j] == x && q->ins_y[j] == y) return;
  if (q->nins >= SEAM_INS) return; /* (room for the slivers, gpu_fast_build; a cap of VC_POLY_MAXV - nv until 2026-11-24 kept
                                    * the corners met first, so the result hung on the order the grid met the pairs) */
  q->ins_e[q->nins] = i, q->ins_t[q->nins] = (float)t, q->ins_x[q->nins] = x, q->ins_y[q->nins] = y, q->nins++;
}

/* edges e and f (k VC_POLY_MAXV + i): along each other? */
static void seam_pair(GpuFastSeams *S, int e, int f) {
  SeamPoly *P = &S->poly[e / VC_POLY_MAXV], *Q = &S->poly[f / VC_POLY_MAXV];
  int i = e % VC_POLY_MAXV, j = f % VC_POLY_MAXV, i2 = (i + 1) % P->nv, j2 = (j + 1) % Q->nv;
  double ux = S->ux[e], uy = S->uy[e], vx = S->ux[f], vy = S->uy[f];
  if ((P->cw == Q->cw ? 1 : -1) * (ux * vx + uy * vy) > -0.98) return; /* the interiors on opposite sides, the edges about parallel */
  double ax = P->px[i], ay = P->py[i], bx = P->px[i2], by = P->py[i2], cx = Q->px[j], cy = Q->py[j], dx = Q->px[j2], dy = Q->py[j2];
  double hc = (cx - ax) * uy - (cy - ay) * ux, hd = (dx - ax) * uy - (dy - ay) * ux;
  if ((hc > 1.5 && hd > 1.5) || (hc < -1.5 && hd < -1.5)) return; /* f wholly to one side of e's line */
  double L = S->len[e], M = S->len[f];
  double tc = (cx - ax) * ux + (cy - ay) * uy, td = (dx - ax) * ux + (dy - ay) * uy;
  double lo = fmax(0, fmin(tc, td)), hi = fmin(L, fmax(tc, td));
  if (hi - lo < 1.0) return;
  double hlo = hc + (hd - hc) * (lo - tc) / (td - tc), hhi = hc + (hd - hc) * (hi - tc) / (td - tc);
  if (fabs(hlo) > 1.5 || fabs(hhi) > 1.5) return;
  double tm = (lo + hi) / 2, fm = (tm - tc) / (td - tc);
  double zp = P->d[i] + (P->d[i2] - P->d[i]) * tm / L, zq = Q->d[j] + (Q->d[j2] - Q->d[j]) * fm;
  if (fabs(zp - zq) > 64 + 2 * (P->slope + Q->slope)) return;
  seam_iv(P, i, lo / L, hi / L);
  seam_iv(Q, j, (lo - tc) / (td - tc), (hi - tc) / (td - tc));
  /* a corner outside the edge (a gap between them; inside, the two overlap) gets a sliver (gpu_fast_build) */
  int ca = cx == ax && cy == ay, cb = cx == bx && cy == by, da = dx == ax && dy == ay, db = dx == bx && dy == by;
  double sp = P->cw ? 1 : -1, sq = Q->cw ? 1 : -1;
  double ha = (ax - cx) * vy - (ay - cy) * vx, hb = (bx - cx) * vy - (by - cy) * vx;
  if (tc > 0 && tc < L && !ca && !cb && sp * hc > 1.0 / 64) seam_corner(P, i, tc / L, Q->px[j], Q->py[j]);
  if (td > 0 && td < L && !da && !db && sp * hd > 1.0 / 64) seam_corner(P, i, td / L, Q->px[j2], Q->py[j2]);
  double ta = (ax - cx) * vx + (ay - cy) * vy, tb = (bx - cx) * vx + (by - cy) * vy;
  if (ta > 0 && ta < M && !ca && !da && sq * ha > 1.0 / 64) seam_corner(Q, j, ta / M, P->px[i], P->py[i]);
  if (tb > 0 && tb < M && !cb && !db && sq * hb > 1.0 / 64) seam_corner(Q, j, tb / M, P->px[i2], P->py[i2]);
}

/* every filled polygon's edges (S->poly, nv > 0) against the others' near it, through a grid of SEAM_CELL DS pixels; -1 when out of
 * memory. The result does not depend on the order the pairs are met (seam_iv's union, seam_corner's set) */
static int fast_seams(GpuFastSeams *S, int n, int scale) {
  const int cs = SEAM_CELL * scale < SEAM_MINC ? SEAM_MINC : SEAM_CELL * scale, gw = VC_W * scale / cs + 1, gh = VC_H * scale / cs + 1, nc = gw * gh;
  if (S->cell == NULL && (S->cell = malloc(sizeof(int) * (size_t)((VC_W * 8 / SEAM_MINC + 1) * (VC_H * 8 / SEAM_MINC + 1) + 2))) == NULL)
    return -1; /* (the most cells, at any scale) */
  memset(S->cell, 0, sizeof(int) * (size_t)(nc + 2));
  /* each edge in the cells its box (2 grid pixels wider) meets, with its sort key there: the angle's bin (32 over pi, of 3.6 to 7.1
   * degrees; bin 0 again as 32, the offset negated, for the wrap), the cell centre's offset from its line, its interior's side */
  int nr = 0;
  const double ics = 1.0 / cs;
  for (int k = 0; k < n; k++) {
    const SeamPoly *q = &S->poly[k];
    for (int i = 0; i < q->nv; i++) {
      int j = (i + 1) % q->nv, e = k * VC_POLY_MAXV + i;
      double ax = q->px[i], ay = q->py[i], dx = q->px[j] - ax, dy = q->py[j] - ay, l = hypot(dx, dy);
      if (l == 0) continue;
      S->ux[e] = (float)(dx / l), S->uy[e] = (float)(dy / l), S->len[e] = (float)l;
      double fx = dy < 0 || (dy == 0 && dx < 0) ? -dx : dx, fy = dy < 0 || (dy == 0 && dx < 0) ? -dy : dy; /* (the line's way, y >= 0) */
      float th = (float)(1 - fx / (fabs(fx) + fy)); /* its pseudo-angle, 0 .. 2 (monotonic in the angle, no atan2) */
      float fl = S->uy[e] < 0 || (S->uy[e] == 0 && S->ux[e] < 0) ? -1.0f : 1.0f;
      int b = (int)(th * 16), sd = (q->cw ? 1 : -1) * (int)fl > 0;
      b = b > 31 ? 31 : b;
      double nx = -S->uy[e] * fl, ny = S->ux[e] * fl;
      double lx = dx < 0 ? ax + dx : ax, hx = dx < 0 ? ax : ax + dx, ly = dy < 0 ? ay + dy : ay, hy = dy < 0 ? ay : ay + dy;
      int x0 = (int)((lx - 2) * ics + 1) - 1, x1 = (int)((hx + 2) * ics + 1) - 1, y0 = (int)((ly - 2) * ics + 1) - 1, y1 = (int)((hy + 2) * ics + 1) - 1;
      x0 = x0 < 0 ? 0 : x0, y0 = y0 < 0 ? 0 : y0, x1 = x1 < gw ? x1 : gw - 1, y1 = y1 < gh ? y1 : gh - 1;
      int nb = (x1 - x0 + 1) * (y1 - y0 + 1);
      if (S->capreg < nr + nb) {
        int cap = (nr + nb) * 2;
        void *o = realloc(S->reg, sizeof *S->reg * (size_t)cap);
        if (o == NULL) return -1;
        S->reg = o;
        S->capreg = cap;
      }
      for (int cy = y0; cy <= y1; cy++)
        for (int cx = x0; cx <= x1; cx++) {
          int c = cy * gw + cx;
          float r = (float)(((cx + 0.5) * cs - ax) * nx + ((cy + 0.5) * cs - ay) * ny);
          S->reg[nr].c = c, S->reg[nr].k.r = r, S->reg[nr].k.b = b, S->reg[nr].k.e = e, S->reg[nr++].k.s = sd;
          S->cell[c + 1]++;
        }
    }
  }
  for (int c = 0; c < nc; c++) S->cell[c + 1] += S->cell[c];
  if (S->capkey < nr) {
    void *o = realloc(S->key, sizeof *S->key * (size_t)nr);
    if (o == NULL) return -1;
    S->key = o;
    S->capkey = nr;
  }
  for (int a = 0; a < nr; a++) S->key[S->cell[S->reg[a].c]++] = S->reg[a].k;
  for (int c = nc; c > 0; c--) S->cell[c] = S->cell[c - 1]; /* the fill moved each start to its end */
  S->cell[0] = 0;
  /* in each cell by bin, then offset: a pair is tested only in one bin or the next, its interiors on opposite sides, the offsets
   * within 1.5 + 0.07 cs (an edge 1.5 from the other's line, its angle up to a bin off, 0.1, at the cell's corner); each pair once */
  const float win = (float)(1.5 + cs * 0.07);
  struct SeamKey Z[512];
  if (++S->gen == 0) memset(S->hgen, 0, sizeof S->hgen), S->gen = 1; /* (the pairs tested: a new generation each frame) */
  S->nhash = 0;
  for (int c = 0; c < nc; c++) {
    int nk = S->cell[c + 1] - S->cell[c];
    if (nk < 2) continue;
    const struct SeamKey *K = S->key + S->cell[c];
    struct SeamKey *T = Z;
    if (nk > 512) {
      if (S->capbig < nk) {
        void *o = realloc(S->big, sizeof *S->big * (size_t)nk);
        if (o == NULL) return -1;
        S->big = o;
        S->capbig = nk;
      }
      T = S->big;
    }
    int st[33] = {0}, s0[33];
    for (int a = 0; a < nk; a++) st[K[a].b + 1]++;
    for (int b = 1; b < 33; b++) st[b] += st[b - 1];
    memcpy(s0, st, sizeof s0);
    for (int a = 0; a < nk; a++) { /* (counting by bin, insertion by offset: few a bin) */
      int z = st[K[a].b]++;
      for (; z > s0[K[a].b] && T[z - 1].r > K[a].r; z--) T[z] = T[z - 1];
      T[z] = K[a];
    }
    for (int a = 0, z0 = 0; a < nk; a++) { /* z0: bin b + 1's first offset within the window below a's */
      int b = T[a].b, s = T[a].s, wrap = b == 31, e = T[a].e;
      float ra = T[a].r, lo1 = wrap ? -ra - win : ra - win, hi1 = wrap ? -ra + win : ra + win;
      int z1 = s0[0], z1e = s0[1]; /* (bin 0, for bin 31: its offsets from the other way, so -r) */
      if (!wrap) {
        while (z0 < nk && (T[z0].b < b + 1 || (T[z0].b == b + 1 && T[z0].r < lo1))) z0++;
        z1 = z0, z1e = nk;
      } else
        while (z1 < z1e && T[z1].r < lo1) z1++;
      for (int pass = 0; pass < 2; pass++)
        for (int z = pass ? z1 : a + 1; z < (pass ? z1e : nk) && T[z].b == (pass ? (wrap ? 0 : b + 1) : b) && T[z].r <= (pass ? hi1 : ra + win); z++) {
          if ((T[z].s == s) != (pass && wrap)) continue;
          int f = T[z].e;
          if (e / VC_POLY_MAXV == f / VC_POLY_MAXV) continue;
          int lo = e < f ? e : f, hi = e < f ? f : e;
          uint32_t key = (uint32_t)lo * (VC_MAX_POLYS * VC_POLY_MAXV) + (uint32_t)hi, h = (key * 2654435761u) >> (32 - SEAM_HASH);
          while (S->hgen[h] == S->gen && S->hkey[h] != key) h = (h + 1) & ((1u << SEAM_HASH) - 1);
          if (S->hgen[h] == S->gen) continue; /* (tested in another cell) */
          if (++S->nhash < (1 << SEAM_HASH) - 1) S->hgen[h] = S->gen, S->hkey[h] = key; /* (full: tested again, harmlessly) */
          seam_pair(S, lo, hi);
        }
    }
  }
  return 0;
}

/* vertex o at (x, y) on edge a -> b at its parameter t: the attributes as the GPU interpolates them along the edge (z linear, the
 * others over w) */
static void vert_at(float *o, float x, float y, const VC_Vertex *a, const VC_Vertex *b, double t, int k, int flags) {
  float oa[GPU_FAST_V], ob[GPU_FAST_V];
  vert(oa, 0, 0, a, k, flags);
  vert(ob, 0, 0, b, k, flags);
  double iw = (1 - t) / oa[3] + t / ob[3];
  vert(o, x, y, a, k, flags);
  o[2] = (float)((1 - t) * oa[2] + t * ob[2]);
  o[3] = (float)(1 / iw);
  for (int i = 4; i < 9; i++) o[i] = (float)(((1 - t) * oa[i] / oa[3] + t * ob[i] / ob[3]) / iw);
}

int gpu_fast_build(GpuFast *f, GpuSpans *sp, const VCoreState *s, int scale) {
  const VC_GeomOut *g = &s->geo;
  u32 n = g->npoly > VC_MAX_POLYS ? VC_MAX_POLYS : g->npoly;
  u32 d3 = vc_io16(s, VC_REG_DISP3DCNT);
  int blend = (d3 >> 3) & 1, aa = (d3 >> 4) & 1, emark = (d3 >> 5) & 1;
  const int kk = sp->aa > 1 && scale % sp->aa == 0 ? sp->aa : 1; /* samples per N pixel across: the looks act on the N grid */
  /* a first size: 8 fan triangles or 10 bands (6 vertices each) a polygon (each polygon grows it for what it may write, below) */
  if (grow(&f->tri, &f->captri, (int)n * VC_POLY_MAXV * 6) != 0) return -1;
  if (f->cappe < (int)n * VC_POLY_MAXV) {
    void *o = realloc(f->pe, (size_t)VC_MAX_POLYS * VC_POLY_MAXV * sizeof *f->pe);
    if (o == NULL) return -1;
    f->pe = o;
    f->cappe = VC_MAX_POLYS * VC_POLY_MAXV;
  }
  if (f->seams == NULL && (f->seams = calloc(1, sizeof *f->seams)) == NULL) return -1;
  /* test aids: BWPORT_FAST_SEAM bit 0 the slivers, bit 1 the inner edges' rule (pe[3]): default 3, 0 none (the seams' A/B);
   * BWPORT_FAST_JITTER=1 every vertex nudged by 1/256 to 1/64 of an output pixel (a hash of its position: another GPU's rounding of
   * the same input); =D (2..256) by a quarter of 1/D pixel up to 1/D (D 16, 8, 4, 2: the sweep where the seam fix stops closing gaps).
   * Read once into one word (seam | jitter << 2 | D << 3), stored whole: the worker and the GL thread both build */
  static int aids = -1;
  int ai = __atomic_load_n(&aids, __ATOMIC_RELAXED);
  if (ai < 0) {
    const char *e = getenv("BWPORT_FAST_SEAM"), *j = getenv("BWPORT_FAST_JITTER");
    const int jj = j != NULL ? atoi(j) : 0;
    ai = (e != NULL ? atoi(e) & 3 : 3) | (jj != 0) << 2 | (jj >= 2 ? (jj > 256 ? 256 : jj) : 0) << 3;
    __atomic_store_n(&aids, ai, __ATOMIC_RELAXED);
  }
  const int seam = ai & 3, jitter = (ai >> 2) & 1, jdiv = ai >> 3;
  GpuFastSeams *S = f->seams;
  const int wbuf = (g->swap_param & 2) != 0;
  for (u32 k = 0; k < n; k++) { /* the filled polygons on the grid, as the loop below sorts them */
    const VC_Poly *p = &g->poly[g->order[k]];
    SeamPoly *q = &S->poly[k];
    q->nv = q->nins = 0;
    if (p->nv < 3 || p->nv > VC_POLY_MAXV || ((p->attr >> 16) & 31) == 0) continue;
    long long ds2 = 0, a2 = 0;
    int dot = 1;
    for (int i = 0; i < p->nv; i++) {
      const VC_Vertex *a = &g->vtx[p->v[i]], *b = &g->vtx[p->v[(i + 1) % p->nv]];
      ds2 += (long long)a->sx * b->sy - (long long)b->sx * a->sy;
      if (a->sx != g->vtx[p->v[0]].sx || a->sy != g->vtx[p->v[0]].sy) dot = 0;
      gpu_vertex_pos(s, p, a, scale, &q->px[i], &q->py[i]);
      q->d[i] = wbuf ? (float)(68719476736.0 / (a->clip[3] > 0 ? a->clip[3] : 1)) : (float)a->sz;
    }
    for (int i = 0; i < p->nv; i++) {
      int j = (i + 1) % p->nv;
      a2 += (long long)q->px[i] * q->py[j] - (long long)q->px[j] * q->py[i];
    }
    if (dot || ds2 == 0 || a2 == 0) continue;
    q->nv = p->nv;
    q->cw = a2 > 0;
    double best = 0; /* the depth's plane through the fan triangle of most area: its gradient (a sliver seen edge-on: steep) */
    q->slope = 0;
    for (int i = 1; i + 1 < p->nv; i++) {
      double x1 = q->px[i] - q->px[0], y1 = q->py[i] - q->py[0], x2 = q->px[i + 1] - q->px[0], y2 = q->py[i + 1] - q->py[0], D = x1 * y2 - x2 * y1;
      if (fabs(D) <= best) continue;
      double z1 = q->d[i] - q->d[0], z2 = q->d[i + 1] - q->d[0];
      best = fabs(D);
      q->slope = (float)(fabs((z1 * y2 - z2 * y1) / D) + fabs((x1 * z2 - x2 * z1) / D));
    }
    for (int i = 0; i < p->nv; i++) q->niv[i] = 0;
  }
  if ((seam & 3) != 0 && fast_seams(S, (int)n, scale) != 0) return -1;
  f->ntri = 0;
  sp->npoly = (int)n;
  sp->nspan = 0;
  for (u32 k = 0; k < n; k++) {
    const VC_Poly *p = &g->poly[g->order[k]];
    GpuPolyRec *pr = &sp->poly[k];
    memset(pr, 0, sizeof *pr);
    u32 alpha = (p->attr >> 16) & 31;
    u32 fl = (p->translucent ? GPU_PF_TRANS : 0) | (p->front ? GPU_PF_FRONT : 0) |
             ((g->swap_param & 2) ? GPU_PF_WBUF : 0) | (alpha == 0 ? GPU_PF_WIRE : 0);
    if (alpha == 0 || emark || aa || (alpha < 31 && blend)) fl |= GPU_PF_WHOLE;
    if (gpu_poly_is_sprite(s, p)) fl |= GPU_PF_SPRITE;
    pr->t[0][0] = p->attr;
    pr->t[0][1] = p->texparam;
    pr->t[0][2] = p->pltt;
    pr->t[0][3] = fl;
    pr->t[1][0] = 1; /* no rows */
    pr->t[1][3] = alpha == 0 ? 31 : alpha;
    gpu_poly_st_range(s, p, pr->t[2]);
    if (p->nv < 1 || p->nv > VC_POLY_MAXV) continue;
    if (sp->planes) { /* G12's filtered sampling: the s/t range over the vertices (12.4) for the repeat-axis clamp; no planes (the
                       * fast renderer's gradients are its fragments', shaders/g12_texcache.glsl G12_HW) */
      int16_t *r = sp->range[k];
      memset(sp->plane[k], 0, sizeof sp->plane[k]);
      for (int i = 0; i < p->nv; i++)
        for (int c = 0; c < 2; c++) {
          int16_t t = (int16_t)g->vtx[p->v[i]].tex[c];
          if (i == 0 || t < r[c * 2]) r[c * 2] = t;
          if (i == 0 || t > r[c * 2 + 1]) r[c * 2 + 1] = t;
        }
    }
    const VC_Vertex *v[VC_POLY_MAXV];
    int dot = 1;
    long long ds2 = 0; /* twice the signed area on the DS grid */
    for (int i = 0; i < p->nv; i++) {
      v[i] = &g->vtx[p->v[i]];
      if (v[i]->sx != v[0]->sx || v[i]->sy != v[0]->sy) dot = 0;
    }
    for (int i = 0; i < p->nv; i++) {
      const VC_Vertex *a = v[i], *b = v[(i + 1) % p->nv];
      ds2 += (long long)a->sx * b->sy - (long long)b->sx * a->sy;
    }
    /* the dot and the line branches write up to VC_POLY_MAXV bands (band_part: 6 vertices each) without a check of their own; a
     * filled polygon grows for its own below, and may write more than its share of the reservation above (5 pieces a clipped fan
     * triangle, the slivers: up to 180 vertices) */
    if (grow(&f->tri, &f->captri, f->ntri + 6 * VC_POLY_MAXV) != 0) return -1;
    if (dot) { /* all on one DS pixel: an N x N block (Dot=block) or one N pixel (Dot=fine), coverage 0, an edge pixel (p16) */
      if (!vc_span_dot_visible(s, p)) continue;
      int x = v[0]->sx * scale, y = v[0]->sy * scale, m = scale;
      if (scale > 1 && !sp->dot_block) {
        gpu_vertex_pos(s, p, v[0], scale / kk, &x, &y);
        x *= kk, y *= kk, m = kk;
      }
      band(f, x, y, v[0], x, y, v[0], m, (int)k, GPU_FAST_ALLEDGE | GPU_FAST_COV0);
      continue;
    }
    int px[VC_POLY_MAXV], py[VC_POLY_MAXV];
    for (int i = 0; i < p->nv; i++) gpu_vertex_pos(s, p, v[i], scale, &px[i], &py[i]);
    long long a2 = 0;
    for (int i = 0; i < p->nv; i++) {
      int j = (i + 1) % p->nv;
      a2 += (long long)px[i] * py[j] - (long long)px[j] * py[i];
    }
    if (alpha == 0 || ds2 == 0 || a2 == 0) { /* a wire-frame's edges, a line: bands one DS pixel or one N pixel wide */
      int m = kk, wide = scale > 1 && sp->line_wide && (alpha == 0 || ds2 == 0);
      if (wide) m = scale;
      int qx[VC_POLY_MAXV], qy[VC_POLY_MAXV];
      for (int i = 0; i < p->nv; i++) {
        if (wide) qx[i] = v[i]->sx * scale, qy[i] = v[i]->sy * scale;
        else if (kk > 1) gpu_vertex_pos(s, p, v[i], scale / kk, &qx[i], &qy[i]), qx[i] *= kk, qy[i] *= kk;
        else qx[i] = px[i], qy[i] = py[i];
      }
      if (alpha == 0 && ds2 != 0) {
        for (int i = 0; i < p->nv; i++) {
          int j = (i + 1) % p->nv;
          band(f, qx[i], qy[i], v[i], qx[j], qy[j], v[j], m, (int)k, GPU_FAST_ALLEDGE);
        }
      } else {
        /* collinear: each edge's part of the line that no edge before it has, its values from that edge. vc_span_row takes a
         * row's left and right edge from the first edge on it in the polygon's order, so a line whose edges go back and forth
         * (Platinum's door outline: 65 -> 77 -> 74 -> 62) has each row's depth and colour from that edge, not from the two far
         * ends (Fast drew the door's black lines in the wall's colour, 2026-11-21) */
        int xmaj = 0, elo[VC_POLY_MAXV], ehi[VC_POLY_MAXV], nb = 0; /* nb: bands so far, at most VC_POLY_MAXV (the buffer's) */
        for (int i = 1; i < p->nv; i++) xmaj |= abs(qx[i] - qx[0]) >= abs(qy[i] - qy[0]) && qx[i] != qx[0];
        for (int i = 0; i < p->nv; i++) {
          int j = (i + 1) % p->nv, a = xmaj ? qx[i] : qy[i], b = xmaj ? qx[j] : qy[j];
          elo[i] = a < b ? a : b, ehi[i] = (a < b ? b : a) + m;
          int plo[VC_POLY_MAXV], phi[VC_POLY_MAXV], np = 1;
          plo[0] = elo[i], phi[0] = ehi[i];
          for (int e = 0; e < i && np > 0; e++) { /* less the earlier edges' parts (at most i + 1 pieces) */
            int tlo[VC_POLY_MAXV], thi[VC_POLY_MAXV], nn = 0;
            for (int q = 0; q < np; q++) {
              if (ehi[e] <= plo[q] || elo[e] >= phi[q]) { tlo[nn] = plo[q], thi[nn++] = phi[q]; continue; }
              if (plo[q] < elo[e]) tlo[nn] = plo[q], thi[nn++] = elo[e];
              if (ehi[e] < phi[q]) tlo[nn] = ehi[e], thi[nn++] = phi[q];
            }
            np = nn;
            for (int q = 0; q < nn; q++) plo[q] = tlo[q], phi[q] = thi[q];
          }
          /* an edge along the other axis (a zero-length one; a near-collinear one on the N grid) is drawn whole or not at all */
          int emaj = abs(qx[j] - qx[i]) >= abs(qy[j] - qy[i]);
          for (int q = 0; q < np; q++) {
            int whole = plo[q] == elo[i] && phi[q] == ehi[i];
            if ((!whole && emaj != xmaj) || nb == VC_POLY_MAXV) continue;
            nb++;

            band_part(f, qx[i], qy[i], v[i], qx[j], qy[j], v[j], m, (int)k, GPU_FAST_ALLEDGE, whole ? INT_MIN : plo[q],
                      whole ? INT_MAX : phi[q]);
          }
        }
      }

      continue;
    }
    /* a filled polygon: a fan, and its edge functions (the inner ones flagged, fast_seams); the interior is on the right of A->B (y
     * down) when a2 > 0 */
    int cw = a2 > 0, nq = p->nv;
    unsigned inner = 0;
    const SeamPoly *sq = &S->poly[k];
    for (int i = 0; i < nq && sq->nv > 0 && (seam & 2); i++) { /* inner: along a neighbour's edge for half its length or more */
      double len = 0;
      for (int j = 0; j < sq->niv[i]; j++) len += sq->iv[i][j][1] - sq->iv[i][j][0];
      if (len >= 0.5) inner |= 1u << i;
    }
    for (int i = 0; i < nq; i++) { /* its edges' functions, positive inside, 1 a pixel along the minor axis */
      int j = (i + 1) % nq;
      int dx = px[j] - px[i], dy = py[j] - py[i], m = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
      float *e = f->pe[k * VC_POLY_MAXV + i];
      if (m == 0) {
        /* two vertices on one point: never near. A flat top's row is edge pixels on the DS (p16's flat edge's run, as a flat
         * bottom's last row): with AA off, the coverage at the DS pixel's centre (g10_fast.frag) gives its row 31, with 4 samples
         * g10_fast4.frag's coverage leaves flat edges out. It was never near until 2026-11-19 (fk2 AA off, fk3 4 samples): a top
         * clipped at the screen's top row then had no edge marks there, only scattered dark specks */
        e[0] = e[1] = e[3] = 0, e[2] = 1e9f;
        continue;
      }
      float sg = (cw ? 1.0f : -1.0f) / (float)m, ax = (float)px[i] + 0.5f, ay = (float)py[i] + 0.5f;
      e[0] = -(float)dy * sg, e[1] = (float)dx * sg, e[2] = ((float)dy * ax - (float)dx * ay) * sg, e[3] = (inner >> i) & 1u ? 1.0f : 0.0f;
      /* a right or bottom edge: the rasterizer leaves out the pixels on it (top-left rule), so its last pixel inside is up to 1
       * away (a top or left edge's first, under 1): the fragment's d < 1 takes both */
      if (!(e[0] > 0 || (e[0] == 0 && e[1] > 0))) e[2] -= 1.0f / 1024;
    }
    double gx[VC_POLY_MAXV], gy[VC_POLY_MAXV];
    if (kk == 1) grow_poly(px, py, nq, cw, gx, gy);
    /* the polygon's box: a grown pixel or sample lies in it widened by the grow (under a pixel: clip_tri, g10_fast4.vert) */
    int bx0 = px[0], by0 = py[0], bx1 = px[0], by1 = py[0];
    for (int i = 1; i < nq; i++) {
      bx0 = px[i] < bx0 ? px[i] : bx0, bx1 = px[i] > bx1 ? px[i] : bx1;
      by0 = py[i] < by0 ? py[i] : by0, by1 = py[i] > by1 ? py[i] : by1;
    }
    /* (with 4 samples for g10_fast4.vert, on vertices 0 and 1 of each fan triangle: x + 4096 y, exact in a float. Each clamped to
     * 0..4095, which loses no sample: the 2N grid is at most 4096 x 3072 (Resolution 8), a box edge below 0 or past the grid takes
     * every sample on that side as 0 or 4095 does (g10_fast4.frag tests sample 2x + 1 against 2 lo and 2 hi + 2). An offset of
     * 1024 for off-screen corners left x only up to 3071: at 7x and 8x the grown samples right of it were lost) */
    float blo = (float)((bx0 < 0 ? 0 : bx0 > 4095 ? 4095 : bx0) + 4096 * (by0 < 0 ? 0 : by0 > 4095 ? 4095 : by0));
    float bhi = (float)((bx1 < 0 ? 0 : bx1 > 4095 ? 4095 : bx1) + 4096 * (by1 < 0 ? 0 : by1 > 4095 ? 4095 : by1));
    const double gw = 0.984375; /* the grow, at most (grow_poly) */
    /* each fan triangle in up to 5 once clipped, the slivers (one a corner) */
    if (grow(&f->tri, &f->captri, f->ntri + 3 * (5 * (nq - 2) + SEAM_INS)) != 0) return -1;
    float *o = f->tri + (size_t)f->ntri * GPU_FAST_V;
    for (int i = 1; i + 1 < nq; i++) {
      int fl = nq << 4 | i << 8; /* the triangle's edges: polygon edges i - 1, i, i + 1 (g10_fast.vert) */
      vert(o, (float)px[0] + 0.5f, (float)py[0] + 0.5f, v[0], (int)k, fl);
      vert(o + GPU_FAST_V, (float)px[i] + 0.5f, (float)py[i] + 0.5f, v[i], (int)k, fl);
      vert(o + 2 * GPU_FAST_V, (float)px[i + 1] + 0.5f, (float)py[i + 1] + 0.5f, v[i + 1], (int)k, fl);
      int nt = 1;
      if (kk == 1) {
        float t[3 * GPU_FAST_V];
        memcpy(t, o, sizeof t);
        move_tri(o, gx[0], gy[0], gx[i], gy[i], gx[i + 1], gy[i + 1]);
        nt = clip_tri(o, t, bx0 + 0.5 - gw, by0 + 0.5 - gw, bx1 + 0.5 + gw, by1 + 0.5 + gw);
      } else
        o[11] = blo, o[GPU_FAST_V + 11] = bhi;
      o += 3 * nt * GPU_FAST_V;
      f->ntri += 3 * nt;
    }
    /* the slivers: where neighbours' corners lie outside edge i (fast_seams), the gap between the edge and them, a fan from its
     * first vertex over the corners in order to its second. Its pixels are outside the polygon's edges, so g10_fast.frag takes them
     * as grown pixels (edge i is the triangle's vE1): depth + 256 at the edge, the values just inside it; they lose to any polygon's
     * own pixel at that depth and fill only what nothing draws. With 4 samples (on the 2N grid) g10_fast4.frag takes a sliver's
     * samples (GPU_FAST_SLIVER) as grown ones the same way, and draws no grown samples around it */
    for (int i = 0; i < nq && (seam & 1); i++) {
      int j = (i + 1) % nq, fl = nq << 4 | i << 8 | GPU_FAST_SLIVER, last = -1;
      float a0[GPU_FAST_V], pv[GPU_FAST_V];
      vert(a0, (float)px[i] + 0.5f, (float)py[i] + 0.5f, v[i], (int)k, fl);
      for (double t0 = 0;;) { /* edge i's corners in order */
        int b = -1;
        for (int c = 0; c < sq->nins; c++)
          if (sq->ins_e[c] == i && sq->ins_t[c] > t0 && (b < 0 || sq->ins_t[c] < sq->ins_t[b])) b = c;
        float cv[GPU_FAST_V];
        if (b < 0 && last < 0) break;
        if (b < 0) vert(cv, (float)px[j] + 0.5f, (float)py[j] + 0.5f, v[j], (int)k, fl);
        else vert_at(cv, (float)sq->ins_x[b] + 0.5f, (float)sq->ins_y[b] + 0.5f, v[i], v[j], sq->ins_t[b], (int)k, fl);
        if (last >= 0) {
          memcpy(o, a0, sizeof a0);
          memcpy(o + GPU_FAST_V, pv, sizeof pv);
          memcpy(o + 2 * GPU_FAST_V, cv, sizeof cv);
          o += 3 * GPU_FAST_V;
          f->ntri += 3;
        }
        if (b < 0) break;
        memcpy(pv, cv, sizeof cv);
        last = b;
        t0 = sq->ins_t[b];
      }
    }
  }
  if (jitter) /* (the same position the same nudge, as a GPU rounds the same input the same way) */
    for (int i = 0; i < f->ntri; i++) {
      float *o = f->tri + (size_t)i * GPU_FAST_V;
      uint32_t hx, hy;
      memcpy(&hx, &o[0], 4);
      memcpy(&hy, &o[1], 4);
      uint64_t h = ((uint64_t)hx << 32 | hy) * 0x9E3779B97F4A7C15ull;
      h ^= h >> 29, h *= 0xBF58476D1CE4E5B9ull, h ^= h >> 32;
      double r0 = jdiv ? 0.25 / jdiv : 1.0 / 256, r1 = jdiv ? 1.0 / jdiv : 1.0 / 64;
      double r = (r0 + (double)(h & 0xFFFF) / 65536.0 * (r1 - r0)) * kk, an = (double)((h >> 16) & 0xFFFF) / 65536.0 * 6.283185307179586;
      o[0] += (float)(r * cos(an)), o[1] += (float)(r * sin(an));
    }
  return 0;
}

void gpu_fast_free(GpuFast *f) {
  free(f->tri);
  free(f->pe);
  if (f->seams != NULL) free(f->seams->cell), free(f->seams->key), free(f->seams->reg), free(f->seams->big), free(f->seams);
  memset(f, 0, sizeof *f);
}
