/* port/gpu G2's unit tests: the packing of a VC_Span into a record (src/g02_spans.c) and the walk's records for a
 * hand-made polygon. A plain main (exit 1 on a failure): build/gpu/gpu_tests. OWNED BY G2. */
#include "g02_spans.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #c); fails++; } } while (0)

static VC_Span base(void) {
  VC_Span v;
  memset(&v, 0, sizeof v);
  v.y = 5;
  v.xs = 10; v.xe = 20; v.dxs = 10; v.dxe = 20; v.den_f = 11; v.den_z = 12; v.xmax = 255;
  v.L.w = v.R.w = 4096; v.L.z = 100; v.R.z = 200;
  v.L.c[0] = 31; v.R.c[0] = 47;
  return v;
}

static void t_plain(void) {
  VC_Span v = base();
  v.nrun = 2;
  v.run[0][0] = 10; v.run[0][1] = 11;
  v.run[1][0] = 19; v.run[1][1] = 20;
  GpuSpan r;
  int ov = 0;
  gpu_span_pack(&r, &v, 1, 0, &ov);
  CHECK(r.t[0][0] == 10 && r.t[0][1] == 20 && r.t[0][2] == 11 && r.t[0][3] == 12);
  CHECK(r.t[1][2] == 100 && r.t[1][3] == 200 && r.t[2][0] == 31 && r.t[3][0] == 47);
  CHECK(r.t[4][3] == 10); /* x0 = vcore's xs */
  CHECK(r.t[5][0] == 10 && r.t[5][1] == 11 && r.t[5][2] == 19 && r.t[5][3] == 20);
  CHECK(r.t[6][0] > r.t[6][1]); /* no third run */
  CHECK(ov == 0);
}

static void t_mask_merge(void) {
  VC_Span v = base();
  v.xe = 15; /* the row's last pixel joins the runs */
  v.nrun = 2;
  v.run[0][0] = 10; v.run[0][1] = 14; /* 14 | 15 | 16..20 merge into one */
  v.run[1][0] = 16; v.run[1][1] = 20;
  GpuSpan r;
  gpu_span_pack(&r, &v, 1, 0, NULL);
  CHECK(r.t[5][0] == 10 && r.t[5][1] == 20 && r.t[5][2] > r.t[5][3]);
}

static void t_overflow(void) {
  VC_Span v = base();
  v.xe = 0;
  v.nrun = 7; /* 7 separate runs + the last pixel's: more than 6 */
  for (int i = 0; i < 7; i++) v.run[i][0] = v.run[i][1] = 10 + 3 * i;
  GpuSpan r;
  int ov = 0;
  gpu_span_pack(&r, &v, 1, 0, &ov);
  CHECK(ov == 1);
  CHECK(r.t[7][2] <= r.t[7][3]); /* the sixth interval is filled */
}

static void t_empty_and_clamp(void) {
  VC_Span v = base();
  v.dxs = 30; v.dxe = 20;
  GpuSpan r;
  gpu_span_pack(&r, &v, 1, 0, NULL);
  CHECK(r.t[0][0] > r.t[0][1]);
  v = base();
  v.dxs = -5; v.dxe = 300; v.xs = -5;
  gpu_span_pack(&r, &v, 1, 0, NULL);
  CHECK(r.t[0][0] == 0 && r.t[0][1] == 255 && r.t[4][3] == -5); /* clamped to the screen, the origin kept */
}

static void t_aa_and_flags(void) {
  VC_Span v = base();
  v.flags = VC_SPAN_AA | VC_SPAN_AAL | VC_SPAN_AAR | VC_SPAN_VERTL | VC_SPAN_ALLEDGE;
  v.le = (VC_SpanEdge){8, 2, 0, 9};
  v.re = (VC_SpanEdge){30, 1, -7, 3};
  v.al = (VC_SpanAA){10, 10, 1, 1, 17, 0};
  v.ar = (VC_SpanAA){0, 20, 15, 25, -1, 1};
  v.nrun = 1; v.run[0][0] = 10; v.run[0][1] = 11;
  GpuSpan r;
  gpu_span_pack(&r, &v, 1, 0, NULL);
  int fl = (int)r.t[4][2];
  CHECK((fl & GPU_ROW_AA) && (fl & GPU_ROW_VERTL) && (fl & GPU_ROW_ALLEDGE) && !(fl & GPU_ROW_WIRE));
  CHECK(r.t[6][0] == 1 && r.t[6][1] == 1 && r.t[6][2] == 18 && r.t[6][3] == 8); /* left: window, cov0 + 1, ax */
  CHECK(r.t[7][0] == 3 && r.t[7][1] == 0 && r.t[7][2] == 9);               /* k = y - ay */
  CHECK(r.t[8][0] == 15 && r.t[8][1] == 25 && r.t[8][2] == 0 + 64 && r.t[8][3] == 30); /* right: X-major, sh */
  CHECK(r.t[9][0] == 4 && r.t[9][1] == -7 && r.t[9][2] == 3);
  /* at m > 1 (a DS-grid row: a line widened by N) the antialiasing windows stay in DS pixels
     (GPU_ROW_DSGRID: the shader reads them at x / N) and the runs are widened */
  gpu_span_pack(&r, &v, 4, GPU_ROW_COV0, NULL);
  fl = (int)r.t[4][2];
  CHECK((fl & GPU_ROW_AA) && (fl & GPU_ROW_DSGRID) && (fl & GPU_ROW_COV0));
  CHECK(r.t[6][0] == 1 && r.t[6][1] == 1 && r.t[6][2] == 18 && r.t[6][3] == 8); /* the same DS-pixel window */
  CHECK(r.t[0][0] == 40 && r.t[0][1] == 83 && r.t[4][3] == 40 && r.t[0][2] == 44);
  CHECK(r.t[5][0] == 40 && r.t[5][1] == 47);
}

/* ---- the 32-bit forms against p16's s64 forms (pass S's arithmetic) ---- */
static int s32_fails;
static void s32_fail(const char *what, const VC_SpanSeg *e, int y, int left) {
  if (s32_fails++ < 10)
    printf("FAIL s32 %s: ax %d ay %d dx %d dy %d y %d left %d\n", what, e->ax, e->ay, e->dx, e->dy, y, left);
}
static VC_SpanSeg seg(int ax, int ay, int dx, int dy) {
  VC_SpanSeg e = {ax, ay, ax + dx, ay + dy, dx, dy, 0, 1};
  return e;
}
static int aa_same(const VC_SpanAA *a, const VC_SpanAA *b, int left) {
  return a->lo == b->lo && a->hi == b->hi && a->cov0 == b->cov0 && a->sh == b->sh && (left ? a->first == b->first : a->last == b->last);
}
/* every form on edge e, rows ay - 1 .. by + 1 */
static void s32_edge(const VC_SpanSeg *e) {
  for (int y = e->ay - 1; y <= e->by + 1; y++) {
    int a0, a1, a2, b0 = 0, b1 = 0, b2 = 0;
    int ra = vc_span_t_edge_span(e, y, &a0, &a1, &a2), rb = gpu_s32_edge_span(e, y, &b0, &b1, &b2);
    if (ra != rb || (ra && (a0 != b0 || a1 != b1 || a2 != b2))) s32_fail("edge_span", e, y, 0);
    if (y < e->ay || y > e->by) continue;
    int adx = e->dx < 0 ? -e->dx : e->dx;
    if (e->dy > 0 && adx > e->dy && y < e->by && vc_span_t_xmajor_q(e, y - e->ay) != gpu_s32_xmajor_q(e, y - e->ay))
      s32_fail("xmajor_q", e, y, 0);
    for (int left = 0; left < 2; left++) {
      VC_SpanAA p, q;
      memset(&p, 0, sizeof p);
      memset(&q, 0, sizeof q);
      int rp = vc_span_t_aa_edge(e, y, left, &p), rq = gpu_s32_aa_edge(e, y, left, &q);
      if (rp != rq || (rp && !aa_same(&p, &q, left))) s32_fail("aa_edge", e, y, left);
      if (rp && p.cov0 < 0) { /* the X-major formula at the window's ends and middle */
        const VC_SpanEdge ce = {e->ax, e->ay, e->dx, e->dy};
        int js[3] = {p.lo - 1, (p.lo + p.hi) / 2, p.hi + 1};
        for (int i = 0; i < 3; i++)
          if (vc_span_t_aa_covx(&ce, y - e->ay, js[i], left) != gpu_s32_aa_covx(&ce, y - e->ay, js[i], left)) s32_fail("aa_covx", e, y, left);
      }
    }
    s64 n64, d64;
    int n32, d32;
    vc_span_t_edge_mid(e, y, &n64, &d64);
    gpu_s32_edge_mid(e, y, &n32, &d32);
    if (n64 != n32 || d64 != d32) s32_fail("edge_mid", e, y, 0);
  }
}
static uint32_t rng = 12345;
static int rnd(int lo, int hi) { /* lo..hi */
  rng = rng * 1664525u + 1013904223u;
  return lo + (int)((rng >> 8) % (uint32_t)(hi - lo + 1));
}
/* every dy in dy0..dy1 with every dx in -2048..2048 (full: all rows; else dx strided); then random edges up to the 16 x grid
 * (|dx| 4096, dy 3072) and random pairs of row-centre x's (with equal ones) */
static void t_s32(int dy0, int dy1, int full) {
  for (int dy = dy0; dy <= dy1; dy++)
    for (int dx = -2048; dx <= 2048; dx += full ? 1 : 61) {
      VC_SpanSeg e = seg(dy & 1 ? 1500 : 3, dx & 1 ? 7 : 1100, dx, dy);
      s32_edge(&e);
    }
  for (int i = 0; i < (full ? 2000000 : 20000); i++) {
    int dy = rnd(0, 3072), dx = rnd(-4096, 4096);
    VC_SpanSeg e = seg(rnd(-64, 4096), rnd(-64, 3072), dx, dy);
    s32_edge(&e);
  }
  for (int i = 0; i < 2000000; i++) {
    int d1 = rnd(1, 6144), d2 = rnd(1, 6144), n1 = rnd(-(1 << 25), 1 << 25), n2;
    if (i & 1) n2 = rnd(-(1 << 25), 1 << 25);
    else { /* the same value: n1 / d1 reduced, scaled to a multiple of it under 6144 */
      int a = n1 < 0 ? -n1 : n1, b = d1;
      while (b) { int t = a % b; a = b; b = t; }
      int dr = d1 / a, m = rnd(1, 6144 / dr);
      d2 = dr * m;
      n2 = n1 / a * m;
    }
    s64 c = (s64)n1 * d2 - (s64)n2 * d1;
    int want = c < 0 ? -1 : c > 0;
    if (gpu_s32_mid_cmp(n1, d1, n2, d2) != want && s32_fails++ < 10) printf("FAIL s32 mid_cmp %d/%d %d/%d\n", n1, d1, n2, d2);
  }
  printf("s32: dy %d..%d%s: %d differ\n", dy0, dy1, full ? " (full)" : "", s32_fails);
  if (s32_fails) fails++;
}

int main(int argc, char **argv) {
  if (argc == 3) { /* gpu_tests DY0 DY1: the 32-bit forms on every edge with dy0 <= dy <= dy1 (a few minutes for the range 0..2048) */
    t_s32(atoi(argv[1]), atoi(argv[2]), 1);
    return fails ? 1 : 0;
  }
  t_s32(0, 160, 0);
  t_plain();
  t_mask_merge();
  t_overflow();
  t_empty_and_clamp();
  t_aa_and_flags();
  printf("gpu_tests: %s (%d failed)\n", fails ? "FAIL" : "PASS", fails);
  return fails ? 1 : 0;
}
