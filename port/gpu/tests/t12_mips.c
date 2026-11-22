/* port/gpu G12's mip tests, test 2 of the texture cache design: mips do not bleed (a white opaque texel among transparent
 * black ones stays white at every level where alpha > 0) and hard-alpha cut-outs keep their coverage. build/gpu/gpu_t12mips.
 * OWNED BY G12. */
#include "g12_tcdecode.h"
#include "g12_tcmips.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #c); fails++; } } while (0)

static u8 L0[512 * 512 * 4], M[512 * 512 * 2];

static size_t cov(const u8 *p, size_t n) {
  size_t c = 0;
  for (size_t i = 0; i < n; i++) c += p[i * 4 + 3] >= 128;
  return c;
}

/* the colour of every texel with alpha > 0 on every level must be exactly (r,g,b) */
static void check_colour(int S, u8 r, u8 g, u8 b, const char *what) {
  const u8 *p = M;
  for (int s = S / 2; s >= 1; s /= 2) {
    for (size_t i = 0; i < (size_t)s * s; i++)
      if (p[i * 4 + 3] && (p[i * 4] != r || p[i * 4 + 1] != g || p[i * 4 + 2] != b)) {
        printf("FAIL %s: level %d texel %zu = %d,%d,%d a%d\n", what, s, i, p[i * 4], p[i * 4 + 1], p[i * 4 + 2], p[i * 4 + 3]);
        fails++;
        return;
      }
    p += (size_t)s * s * 4;
  }
}

/* coverage per level within 1 texel of level 0's share */
/* tol: texels of slack as a share of the level plus a constant (alphas are quantised, level 1 has 5 values, so a dense random
 * cut-out cannot hit the share exactly; the deterministic cases below use 1 texel) */
static void check_cover(int S, const char *what, double tol_share, double tol_const) {
  size_t c0 = cov(L0, (size_t)S * S);
  const u8 *p = M;
  for (int s = S / 2; s >= 1; s /= 2) {
    size_t n = (size_t)s * s, c = cov(p, n);
    double want = (double)c0 * (double)n / ((double)S * S);
    double d = (double)c - want;
    if (d < -(tol_share * (double)n + tol_const) || d > tol_share * (double)n + tol_const) {
      printf("FAIL %s: level %d coverage %zu of %zu, level 0 share wants %.2f\n", what, s, c, n, want);
      fails++;
    }
    p += n * 4;
  }
}

/* a level whose alpha is all equal (a checkerboard's 1/2 everywhere) can only be covered wholly or not at all: the cut-out must
 * be kept (wholly) at every level */
static void check_flat_cover(int S, const char *what) {
  const u8 *p = M;
  for (int s = S / 2; s >= 1; s /= 2) {
    size_t n = (size_t)s * s;
    if (cov(p, n) != n) {
      printf("FAIL %s: level %d coverage %zu of %zu\n", what, s, cov(p, n), n);
      fails++;
    }
    p += n * 4;
  }
}
/* a thin branch: never gone, and never more than 4 texels' worth of the level 0 share per texel it should cover */
static void check_nonzero(int S, const char *what) {
  const u8 *p = M;
  for (int s = S / 2; s >= 1; s /= 2) {
    size_t n = (size_t)s * s;
    if (cov(p, n) == 0) {
      printf("FAIL %s: level %d lost the branch\n", what, s);
      fails++;
    }
    p += n * 4;
  }
}

static void clear(int S, u8 r, u8 g, u8 b) { /* transparent, colour 0 of the palette */
  for (size_t i = 0; i < (size_t)S * S; i++) {
    L0[i * 4] = r;
    L0[i * 4 + 1] = g;
    L0[i * 4 + 2] = b;
    L0[i * 4 + 3] = 0;
  }
}
static void put(int S, int x, int y, u8 r, u8 g, u8 b, u8 a) {
  u8 *p = L0 + ((size_t)y * S + x) * 4;
  p[0] = r; p[1] = g; p[2] = b; p[3] = a;
}

int main(void) {
  CHECK(g12_tc_mips_size(32) == (16 * 16 + 8 * 8 + 4 * 4 + 2 * 2 + 1) * 4);
  for (int S = 32; S <= 512; S *= 2) {
    char w[64];
    /* one white texel in transparent black, with and without the level-0 fill */
    clear(S, 0, 0, 0);
    put(S, 5, 7, 255, 255, 255, 255);
    g12_tc_mips(L0, S, 1, M);
    snprintf(w, sizeof w, "one texel S=%d", S);
    check_colour(S, 255, 255, 255, w);
    g12_tc_fill(L0, S);
    g12_tc_mips(L0, S, 1, M);
    snprintf(w, sizeof w, "one texel filled S=%d", S);
    check_colour(S, 255, 255, 255, w);
    /* hard-alpha checkerboard: 1/2 coverage */
    clear(S, 0, 0, 0);
    for (int y = 0; y < S; y++)
      for (int x = 0; x < S; x++)
        if ((x ^ y) & 1) put(S, x, y, 200, 100, 50, 255);
    g12_tc_fill(L0, S);
    g12_tc_mips(L0, S, 1, M);
    snprintf(w, sizeof w, "checker S=%d", S);
    check_colour(S, 200, 100, 50, w);
    check_flat_cover(S, w);
    /* a 1-texel-wide branch */
    clear(S, 0, 0, 0);
    for (int y = 0; y < S; y++) put(S, 3, y, 10, 90, 20, 255);
    g12_tc_fill(L0, S);
    g12_tc_mips(L0, S, 1, M);
    snprintf(w, sizeof w, "branch S=%d", S);
    check_colour(S, 10, 90, 20, w);
    check_nonzero(S, w);
    /* random hard-alpha cut-outs: coverage kept */
    srand((unsigned)S);
    for (int dens = 10; dens <= 90; dens += 20) {
      clear(S, 0, 0, 0);
      for (int y = 0; y < S; y++)
        for (int x = 0; x < S; x++)
          if (rand() % 100 < dens) put(S, x, y, (u8)(rand() & 255), (u8)(rand() & 255), (u8)(rand() & 255), 255);
      g12_tc_fill(L0, S);
      g12_tc_mips(L0, S, 1, M);
      snprintf(w, sizeof w, "random %d%% S=%d", dens, S);
      check_cover(S, w, 0.2, 3);
    }
    /* soft alpha beside magenta transparent texels: only the opaque colour appears wherever alpha > 0 */
    clear(S, 255, 0, 255);
    for (int y = 0; y < S; y++)
      for (int x = 0; x < S / 2; x++) put(S, x, y, 40, 80, 120, (u8)(1 + (x + y) % 255));
    g12_tc_mips(L0, S, 0, M);
    snprintf(w, sizeof w, "soft S=%d", S);
    check_colour(S, 40, 80, 120, w);
    /* an opaque layer: alpha 255 at every level (no scaling) */
    clear(S, 0, 0, 0);
    for (size_t i = 0; i < (size_t)S * S; i++) L0[i * 4 + 3] = 255;
    g12_tc_mips(L0, S, 1, M);
    {
      int bad = 0;
      for (size_t i = 0; i < g12_tc_mips_size(S) / 4; i++) bad |= M[i * 4 + 3] != 255;
      CHECK(!bad);
    }
  }
  if (fails) printf("t12 mips: %d FAILED\n", fails);
  else printf("t12 mips: ok\n");
  return fails != 0;
}
