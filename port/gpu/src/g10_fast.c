/* port/gpu piece G10: the fast 3D renderer's triangles (g10_fast.h; docs/gpu-hwraster.md). Built from vcore's geometry like
 * gpu_spans_build's polygon table, with no span records: the GPU's rasterizer finds the pixels. */
#include "g10_fast.h"
#include "p16_raster.h"

#include <limits.h>
#include <math.h>
#include <stdlib.h>
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
 * vertices go to where the moved edges meet (at most two pixels away at a sharp corner). These amounts close every gap in the
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
    double l2 = dx * dx + dy * dy;
    if (l2 > 4) dx *= 2 / sqrt(l2), dy *= 2 / sqrt(l2);
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

int gpu_fast_build(GpuFast *f, GpuSpans *sp, const VCoreState *s, int scale) {
  const VC_GeomOut *g = &s->geo;
  u32 n = g->npoly > VC_MAX_POLYS ? VC_MAX_POLYS : g->npoly;
  u32 d3 = vc_io16(s, VC_REG_DISP3DCNT);
  int blend = (d3 >> 3) & 1, aa = (d3 >> 4) & 1, emark = (d3 >> 5) & 1;
  const int kk = sp->aa > 1 && scale % sp->aa == 0 ? sp->aa : 1; /* samples per N pixel across: the looks act on the N grid */
  /* at most 8 fan triangles or 10 bands (6 vertices each) a polygon */
  if (grow(&f->tri, &f->captri, (int)n * VC_POLY_MAXV * 6) != 0) return -1;
  if (f->cappe < (int)n * VC_POLY_MAXV) {
    void *o = realloc(f->pe, (size_t)VC_MAX_POLYS * VC_POLY_MAXV * sizeof *f->pe);
    if (o == NULL) return -1;
    f->pe = o;
    f->cappe = VC_MAX_POLYS * VC_POLY_MAXV;
  }
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
    /* a filled polygon: a fan, and its edge functions; the interior is on the right of A->B (y down) when a2 > 0 */
    int cw = a2 > 0;
    for (int i = 0; i < p->nv; i++) { /* its edges' functions, positive inside, 1 a pixel along the minor axis */
      int j = (i + 1) % p->nv;
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
      e[0] = -(float)dy * sg, e[1] = (float)dx * sg, e[2] = ((float)dy * ax - (float)dx * ay) * sg, e[3] = 0;
      /* a right or bottom edge: the rasterizer leaves out the pixels on it (top-left rule), so its last pixel inside is up to 1
       * away (a top or left edge's first, under 1): the fragment's d < 1 takes both */
      if (!(e[0] > 0 || (e[0] == 0 && e[1] > 0))) e[2] -= 1.0f / 1024;
    }
    double gx[VC_POLY_MAXV], gy[VC_POLY_MAXV];
    if (kk == 1) grow_poly(px, py, p->nv, cw, gx, gy);
    float *o = f->tri + (size_t)f->ntri * GPU_FAST_V;
    for (int i = 1; i + 1 < p->nv; i++) {
      int fl = p->nv << 4 | i << 8; /* the triangle's edges: polygon edges i - 1, i, i + 1 (g10_fast.vert) */
      vert(o, (float)px[0] + 0.5f, (float)py[0] + 0.5f, v[0], (int)k, fl);
      vert(o + GPU_FAST_V, (float)px[i] + 0.5f, (float)py[i] + 0.5f, v[i], (int)k, fl);
      vert(o + 2 * GPU_FAST_V, (float)px[i + 1] + 0.5f, (float)py[i + 1] + 0.5f, v[i + 1], (int)k, fl);
      if (kk == 1) move_tri(o, gx[0], gy[0], gx[i], gy[i], gx[i + 1], gy[i + 1]);
      o += 3 * GPU_FAST_V;
      f->ntri += 3;
    }
  }
  return 0;
}

void gpu_fast_free(GpuFast *f) {
  free(f->tri);
  free(f->pe);
  memset(f, 0, sizeof *f);
}
