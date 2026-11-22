/* port/gpu 2D filter: the C reference of shaders/g11_up.glsl, g11_up_scale2x.glsl, g11_up_mmpx.glsl and pass C's UP program
 * (g07_compose.frag's up_apply, c_planned3). Written from those rules, not from their text: integer only.
 * MMPX: Morgan McGuire and Mara Gagiu, JCGT 2021 (https://casual-effects.com/research/McGuire2021PixelArt/); its rules follow
 * the authors' reference code, Copyright 2020 Morgan McGuire & Mara Gagiu, MIT license (licenses/MMPX-MIT.txt). */
#include "up2d_ref.h"

#include <stdlib.h>
#include <string.h>

enum { ST_IDENTITY = 1, ST_SCALE2X = 2, ST_MMPX = 3 };
#define K3D 0x7F000000u

static int imin(int a, int b) { return a < b ? a : b; }
static int imax(int a, int b) { return a > b ? a : b; }
static int clampi(int v, int lo, int hi) { return imax(lo, imin(hi, v)); }

/* ---- the key: what the filter sees at DS pixel (x, y), y in engine order (0-191 engine A, 192-383 engine B) ---- */
uint32_t up2d_ref_key(const UpRefIn *in, int x, int y) {
  if (y < 192 && in->compose) {
    const uint32_t *pl = in->plan + 2 * (y * 256 + x);
    uint32_t rank = (pl[0] >> 24) & 3;
    if (rank == 0 || !in->have3d) return pl[0] & 0x3F3F3F;
    if (rank == 1) return K3D;
    return (pl[1] & 0x3F3F3F) | (1u << 30);
  }
  return in->src[y * 256 + x] & 0x3F3F3F;
}

/* the neighbour at (dx, dy) clamped within the screen of (x, y) */
static uint32_t keyat(const UpRefIn *in, int x, int y, int dx, int dy) {
  int y0 = y < 192 ? 0 : 192;
  return up2d_ref_key(in, clampi(x + dx, 0, 255), clampi(y + dy, y0, y0 + 191));
}

/* ---- Scale2x (EPX): the four quarters of E from B (up), D (left), F (right), H (down) ---- */
static void scale2x_quarters(const UpRefIn *in, int x, int y, int q[4]) { /* q: -1 D, 0 E, +1 F */
  uint32_t b = keyat(in, x, y, 0, -1), d = keyat(in, x, y, -1, 0), f = keyat(in, x, y, 1, 0), h = keyat(in, x, y, 0, 1);
  q[0] = q[1] = q[2] = q[3] = 0;
  if (b != h && d != f) {
    if (d == b) q[0] = -1;
    if (b == f) q[1] = 1;
    if (d == h) q[2] = -1;
    if (h == f) q[3] = 1;
  }
}

/* ---- MMPX: each pixel's quarters J, K, L, M are E or one of B, D, F, H (positions, compared by key); N >= 3: twice ---- */
enum { MM_PX = 256 * 384 };
typedef struct MmSrc {
  const UpRefIn *in;
  const uint32_t *x1, *k1; /* the first stage's picks and keys (engine order) */
  int stage2, y0;          /* reading the 2x picture of the screen starting at row y0 */
} MmSrc;

static uint32_t mm_luma(uint32_t k) { return k == K3D ? 256 : (k & 63) + ((k >> 8) & 63) + ((k >> 16) & 63); }
static int mm_dx(int c) { return c == 2 ? -1 : c == 3 ? 1 : 0; } /* 0 E, 1 B (up), 2 D (left), 3 F (right), 4 H (down) */
static int mm_dy(int c) { return c == 1 ? -1 : c == 4 ? 1 : 0; }
static uint32_t mm_code(int ox, int oy) { return (uint32_t)(ox + 2) | ((uint32_t)(oy + 2) << 3); }

/* the DS pixel (within the screen) that 2x pixel (X, Y) of the screen (clamped to it) shows after the first stage */
static void mm_pos(const MmSrc *m, int X, int Y, int *sx, int *sy) {
  X = clampi(X, 0, 511), Y = clampi(Y, 0, 383);
  int dx = X >> 1, dy = Y >> 1;
  uint32_t v = (m->x1[(dy + m->y0) * 256 + dx] >> (6 * ((X & 1) + 2 * (Y & 1)))) & 63;
  *sx = clampi(dx + (int)(v & 7) - 2, 0, 255);
  *sy = clampi(dy + (int)((v >> 3) & 7) - 2, 0, 191);
}
static uint32_t mm_src(const MmSrc *m, int x, int y, int dx, int dy) {
  if (!m->stage2) return keyat(m->in, x, y, dx, dy);
  int sx, sy;
  mm_pos(m, x + dx, y + dy, &sx, &sy);
  return m->k1[(sy + m->y0) * 256 + sx];
}

static int eq3any(uint32_t e, uint32_t a, uint32_t b, uint32_t c) { return e == a || e == b || e == c; }
static int eq2all(uint32_t e, uint32_t a, uint32_t b) { return e == a && e == b; }
static int eq3all(uint32_t e, uint32_t a, uint32_t b, uint32_t c) { return e == a && e == b && e == c; }
static int eq4all(uint32_t e, uint32_t a, uint32_t b, uint32_t c, uint32_t d) { return e == a && e == b && e == c && e == d; }
static int ne2(uint32_t e, uint32_t a, uint32_t b) { return e != a && e != b; }
static int ne4(uint32_t e, uint32_t a, uint32_t b, uint32_t c, uint32_t d) { return e != a && e != b && e != c && e != d; }

/* MMPX at (x, y): a DS pixel in engine order (first stage) or a 2x pixel of the screen (second); o: J, K, L, M */
static void mm_run(const MmSrc *m, int x, int y, int o[4]) {
#define S(dx, dy) mm_src(m, x, y, dx, dy)
  uint32_t A = S(-1, -1), B = S(0, -1), C = S(1, -1), D = S(-1, 0), E = S(0, 0), F = S(1, 0), G = S(-1, 1), H = S(0, 1),
           I = S(1, 1);
  int J = 0, K = 0, L = 0, M = 0;
  o[0] = o[1] = o[2] = o[3] = 0;
  if (A == E && B == E && C == E && D == E && F == E && G == E && H == E && I == E) return;
  uint32_t P = S(0, -2), Sx = S(0, 2), Q = S(-2, 0), R = S(2, 0);
  uint32_t Bl = mm_luma(B), Dl = mm_luma(D), El = mm_luma(E), Fl = mm_luma(F), Hl = mm_luma(H);
  /* 1:1 slopes */
  if (D == B && D != H && D != F && (El >= Dl || E == A) && eq3any(E, A, C, G) && (El < Dl || A != D || E != P || E != Q)) J = 2;
  if (B == F && B != D && B != H && (El >= Bl || E == C) && eq3any(E, A, C, I) && (El < Bl || C != B || E != P || E != R)) K = 1;
  if (H == D && H != F && H != B && (El >= Hl || E == G) && eq3any(E, A, G, I) && (El < Hl || G != H || E != Sx || E != Q)) L = 4;
  if (F == H && F != B && F != D && (El >= Fl || E == I) && eq3any(E, C, G, I) && (El < Fl || I != H || E != R || E != Sx)) M = 3;
  /* intersections */
  if (E != F && eq4all(E, C, I, D, Q) && eq2all(F, B, H) && F != S(3, 0)) K = M = 3;
  if (E != D && eq4all(E, A, G, F, R) && eq2all(D, B, H) && D != S(-3, 0)) J = L = 2;
  if (E != H && eq4all(E, G, I, B, P) && eq2all(H, D, F) && H != S(0, 3)) L = M = 4;
  if (E != B && eq4all(E, A, C, H, Sx) && eq2all(B, D, F) && B != S(0, -3)) J = K = 1;
  if (Bl < El && eq4all(E, G, H, I, Sx) && ne4(E, A, D, C, F)) J = K = 1;
  if (Hl < El && eq4all(E, A, B, C, P) && ne4(E, D, G, I, F)) L = M = 4;
  if (Fl < El && eq4all(E, A, D, G, Q) && ne4(E, B, C, I, H)) K = M = 3;
  if (Dl < El && eq4all(E, C, F, I, R) && ne4(E, B, A, G, H)) J = L = 2;
  /* 2:1 slopes */
  if (H != B) {
    if (H != A && H != E && H != C) {
      if (eq3all(H, G, F, R) && ne2(H, D, S(2, -1))) L = M;
      if (eq3all(H, I, D, Q) && ne2(H, F, S(-2, -1))) M = L;
    }
    if (B != I && B != G && B != E) {
      if (eq3all(B, A, F, R) && ne2(B, D, S(2, 1))) J = K;
      if (eq3all(B, C, D, Q) && ne2(B, F, S(-2, 1))) K = J;
    }
  }
  if (F != D) {
    if (D != I && D != E && D != C) {
      if (eq3all(D, A, H, Sx) && ne2(D, B, S(1, 2))) J = L;
      if (eq3all(D, G, B, P) && ne2(D, H, S(1, -2))) L = J;
    }
    if (F != E && F != A && F != G) {
      if (eq3all(F, C, H, Sx) && ne2(F, B, S(-1, 2))) K = M;
      if (eq3all(F, I, B, P) && ne2(F, H, S(-1, -2))) M = K;
    }
  }
#undef S
  o[0] = J, o[1] = K, o[2] = L, o[3] = M;
}

/* in->mm: x1 (the first stage: four 6-bit picks a DS pixel), k1 (the keys), x2 (N >= 3: the 16 4x cells, 3 words) */
void up2d_ref_prepare(UpRefIn *in, int style) {
  if (style != ST_MMPX || in->mm != NULL) return;
  uint32_t *t = calloc((size_t)MM_PX * 5, sizeof *t);
  if (t == NULL) return;
  uint32_t *x1 = t, *k1 = t + MM_PX, *x2 = t + 2 * MM_PX;
  MmSrc m = {in, x1, k1, 0, 0};
  for (int y = 0; y < 384; y++)
    for (int x = 0; x < 256; x++) {
      int o[4];
      mm_run(&m, x, y, o);
      uint32_t v = 0;
      for (int q = 0; q < 4; q++) v |= mm_code(mm_dx(o[q]), mm_dy(o[q])) << (6 * q);
      x1[y * 256 + x] = v;
      k1[y * 256 + x] = up2d_ref_key(in, x, y);
    }
  if (in->scale >= 3) {
    m.stage2 = 1;
    for (int y = 0; y < 384; y++)
      for (int x = 0; x < 256; x++) {
        m.y0 = y < 192 ? 0 : 192;
        int dl = y - m.y0;
        uint32_t *r = x2 + 3 * (y * 256 + x);
        for (int j = 0; j < 2; j++)
          for (int i = 0; i < 2; i++) {
            int X = 2 * x + i, Y = 2 * dl + j, o[4];
            mm_run(&m, X, Y, o);
            for (int q = 0; q < 4; q++) {
              int sx, sy, cell = (2 * j + (q >> 1)) * 4 + 2 * i + (q & 1), b = 6 * cell;
              mm_pos(&m, X + mm_dx(o[q]), Y + mm_dy(o[q]), &sx, &sy);
              uint64_t v = mm_code(sx - x, sy - dl);
              r[b >> 5] |= (uint32_t)(v << (b & 31));
              if ((b & 31) > 26) r[(b >> 5) + 1] |= (uint32_t)(v >> (32 - (b & 31)));
            }
          }
      }
  }
  in->mm = t;
}

void up2d_ref_release(UpRefIn *in) {
  free(in->mm);
  in->mm = NULL;
}

static UpRefPick mmpx_pick(const UpRefIn *in, int dx, int dy, int fx, int fy) {
  UpRefPick p = {0, 0, 0, 0, 0};
  const int N = in->scale;
  uint32_t v;
  if (in->mm == NULL) return p;
  if (N <= 2) {
    int k = (fx >= N ? 1 : 0) + (fy >= N ? 2 : 0);
    v = in->mm[dy * 256 + dx] >> (6 * k);
  } else {
    int cx = (2 * fx >= N) + (2 * fx >= 2 * N) + (2 * fx >= 3 * N), cy = (2 * fy >= N) + (2 * fy >= 2 * N) + (2 * fy >= 3 * N);
    int b = 6 * (cy * 4 + cx);
    const uint32_t *r = in->mm + 2 * MM_PX + 3 * (dy * 256 + dx);
    uint64_t w = r[b >> 5] | ((b >> 5) < 2 ? (uint64_t)r[(b >> 5) + 1] << 32 : 0);
    v = (uint32_t)(w >> (b & 31));
  }
  p.ax = (int8_t)((int)(v & 7) - 2);
  p.ay = (int8_t)((int)((v >> 3) & 7) - 2);
  return p;
}

UpRefPick up2d_ref_pick(const UpRefIn *in, int style, int dx, int dy, int fx, int fy) {
  UpRefPick p = {0, 0, 0, 0, 0};
  if (style == ST_SCALE2X) {
    int q[4], n = in->scale;
    scale2x_quarters(in, dx, dy, q);
    p.ax = (int8_t)q[(fx < n ? 0 : 1) + (fy < n ? 0 : 2)];
  }
  if (style == ST_MMPX) return mmpx_pick(in, dx, dy, fx, fy);
  return p;
}

/* ---- pass C's colour at a DS pixel (c_planned3 / the 1x line), the 3D layer's pixel given ---- */
static int c_r(uint32_t c) { return (int)(c & 63); }
static int c_g(uint32_t c) { return (int)((c >> 8) & 63); }
static int c_b(uint32_t c) { return (int)((c >> 16) & 63); }
static uint32_t c_mk(int r, int g, int b) { return (uint32_t)r | ((uint32_t)g << 8) | ((uint32_t)b << 16); }

static uint32_t blend_alpha(uint32_t c1, uint32_t c2, int eva, int evb) {
  return c_mk(imin(63, (c_r(c1) * eva + c_r(c2) * evb + 8) >> 4), imin(63, (c_g(c1) * eva + c_g(c2) * evb + 8) >> 4),
              imin(63, (c_b(c1) * eva + c_b(c2) * evb + 8) >> 4));
}
static uint32_t blend_bright(uint32_t c, int evy) {
  int r = c_r(c), g = c_g(c), b = c_b(c);
  return c_mk(r + (((63 - r) * evy + 8) >> 4), g + (((63 - g) * evy + 8) >> 4), b + (((63 - b) * evy + 8) >> 4));
}
static uint32_t blend_dark(uint32_t c, int evy) {
  int r = c_r(c), g = c_g(c), b = c_b(c);
  return c_mk(r - ((r * evy + 7) >> 4), g - ((g * evy + 7) >> 4), b - ((b * evy + 7) >> 4));
}
static uint32_t blend_3d(uint32_t c1, uint32_t c2, int a) {
  return c_mk((c_r(c1) * (a + 1) + c_r(c2) * (31 - a) + 16) >> 5, (c_g(c1) * (a + 1) + c_g(c2) * (31 - a) + 16) >> 5,
              (c_b(c1) * (a + 1) + c_b(c2) * (31 - a) + 16) >> 5);
}
static uint32_t bright(const UpRefIn *in, uint32_t px) {
  uint32_t bm = (in->bright >> 14) & 3;
  int f = imin((int)(in->bright & 31), 16);
  if ((bm != 1 && bm != 2) || f == 0) return px;
  int r = c_r(px), g = c_g(px), b = c_b(px);
  if (bm == 1) return c_mk(r + (((63 - r) * f) >> 4), g + (((63 - g) * f) >> 4), b + (((63 - b) * f) >> 4));
  return c_mk(r - ((r * f + 15) >> 4), g - ((g * f + 15) >> 4), b - ((b * f + 15) >> 4));
}

/* the 3D layer at the N x pixel (px, py) of the screen: 0 where transparent (c_3d without SMAA or antialiased edge bits) */
static uint32_t l3d_at(const UpRefIn *in, int px, int py) {
  if (!in->have3d) return 0;
  int W = 256 * in->scale, pos = px + in->hofs * in->scale;
  if (pos >= 2 * W) pos -= 2 * W;
  if (pos >= W) return 0;
  uint32_t t = in->l3d[(size_t)py * W + pos];
  if (((t >> 24) & 31) == 0) return 0;
  return t & 0x1FFFFFFFu;
}

static uint32_t planned3(const UpRefIn *in, const uint32_t *pl, uint32_t l3d) {
  uint32_t c0 = pl[0] & 0xFFFFFF, rank = (pl[0] >> 24) & 3;
  if (rank == 0) return c0;
  int a = (int)((l3d >> 24) & 31);
  if (a == 0) return c0;
  uint32_t t = l3d & 0xFFFFFF, s = pl[1] & 0xFFFFFF, e = (pl[1] >> 24) & 3;
  if (rank == 2) return e == 1 ? bright(in, blend_alpha(s, t, (int)(pl[1] >> 26), (int)(pl[0] >> 26))) : s;
  if (e == 1) return bright(in, blend_3d(t, s, a));
  return bright(in, e == 2 ? blend_bright(t, in->evy) : e == 3 ? blend_dark(t, in->evy) : t);
}

static void v8(uint32_t c, int v[3]) {
  int r = c_r(c), g = c_g(c), b = c_b(c);
  v[0] = (r << 2) | (r >> 4);
  v[1] = (g << 2) | (g >> 4);
  v[2] = (b << 2) | (b >> 4);
}

void up2d_ref_frame(const UpRefIn *in, int style, uint8_t *rgba, UpRefPick *picks, UpRefStats *st) {
  if (style == ST_MMPX && in->mm == NULL) { /* (the caller's in stays as it was) */
    UpRefIn t = *in;
    up2d_ref_prepare(&t, style);
    if (t.mm != NULL) up2d_ref_frame(&t, style, rgba, picks, st);
    up2d_ref_release(&t);
    return;
  }
  const int N = in->scale, W = 256 * N, sh = 192 * N;
  UpRefStats z = {0, 0, 0};
  for (int py = 0; py < 384 * N; py++)
    for (int px = 0; px < W; px++) {
      int top = py < sh, qy = top ? py : py - sh, qx = px;
      int isA = top == (in->atop != 0), dx = qx / N, dy = qy / N;
      int y0 = isA ? 0 : 192;
      UpRefPick k = up2d_ref_pick(in, style, dx, dy + y0, 2 * (qx - dx * N) + 1, 2 * (qy - dy * N) + 1);
      int ax = clampi(dx + k.ax, 0, 255), ay = clampi(dy + k.ay, 0, 191);
      int bx = clampi(dx + k.bx, 0, 255), by = clampi(dy + k.by, 0, 191);
      uint32_t cA, cB;
      if (isA && in->compose) {
        const uint32_t *pa = in->plan + 2 * (ay * 256 + ax), *pb = k.w ? in->plan + 2 * (by * 256 + bx) : pa;
        uint32_t l = (((pa[0] | pb[0]) >> 24) & 3) ? l3d_at(in, qx, qy) : 0;
        cA = planned3(in, pa, l);
        cB = k.w ? planned3(in, pb, l) : cA;
      } else {
        cA = in->src[(ay + y0) * 256 + ax];
        cB = k.w ? in->src[(by + y0) * 256 + bx] : cA;
      }
      int a[3], b[3];
      v8(cA, a);
      v8(cB, b);
      uint8_t *o = rgba + ((size_t)py * W + px) * 4;
      for (int i = 0; i < 3; i++) o[i] = (uint8_t)(k.w ? (a[i] * (64 - k.w) + b[i] * k.w + 32) >> 6 : a[i]);
      o[3] = 255;
      if (picks != NULL) picks[(size_t)py * W + px] = k;
      z.px++;
      if (k.ax || k.ay) z.moved++;
      if (k.w) z.blended++;
    }
  if (st != NULL) *st = z;
}
