/* port/gpu piece G12: the mip chain of a cached layer (g12_tcmips.h). OWNED BY G12. */
#include "g12_tcmips.h"

#include "g12_tcdecode.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

size_t g12_tc_mips_size(int S) {
  size_t n = 0;
  for (int s = S / 2; s >= 1; s /= 2) n += (size_t)s * s * 4;
  return n;
}

static int cmp_desc(const void *a, const void *b) {
  float x = *(const float *)a, y = *(const float *)b;
  return x < y ? 1 : x > y ? -1 : 0;
}

static size_t cover(const u8 *a4, size_t n, double k) {
  size_t c = 0;
  for (size_t i = 0; i < n; i++)
    if ((double)a4[i * 4 + 3] * k >= 127.5) c++;
  return c;
}

void g12_tc_mips(const u8 *level0, int S, int hard, u8 *out) {
  size_t n0 = (size_t)S * S;
  double share0 = 0;
  if (hard) {
    size_t c = 0;
    for (size_t i = 0; i < n0; i++) c += level0[i * 4 + 3] >= 128;
    share0 = (double)c / (double)n0;
  }
  /* cur / ca: the unscaled chain's previous level (RGB bytes and the alpha as a float 0..255, which keeps a one-texel
   * branch's 1/512 alive down to 1 x 1: what the next level averages) */
  u8 *cur = (u8 *)malloc(n0 * 4), *nxt = (u8 *)malloc(n0 + 4);
  float *ca = (float *)malloc(n0 * sizeof(float)), *na = (float *)malloc(n0 / 4 * sizeof(float) + 4);
  if (!cur || !nxt || !ca || !na) {
    free(cur);
    free(nxt);
    free(ca);
    free(na);
    return;
  }
  memcpy(cur, level0, n0 * 4);
  for (size_t i = 0; i < n0; i++) ca[i] = level0[i * 4 + 3];
  u8 *dst = out;
  for (int s = S / 2, ps = S; s >= 1; ps = s, s /= 2) {
    for (int y = 0; y < s; y++)
      for (int x = 0; x < s; x++) {
        double as = 0, cs[3] = {0, 0, 0};
        for (int d = 0; d < 4; d++) {
          size_t pi = (size_t)(2 * y + (d >> 1)) * ps + (size_t)(2 * x + (d & 1));
          as += ca[pi];
          for (int k = 0; k < 3; k++) cs[k] += (double)cur[pi * 4 + k] * ca[pi];
        }
        size_t qi = (size_t)y * s + x;
        u8 *q = nxt + qi * 4;
        na[qi] = (float)(as / 4);
        if (as > 0)
          for (int k = 0; k < 3; k++) q[k] = (u8)(cs[k] / as + 0.5);
        else memset(q, 0, 3);
        q[3] = as > 0 ? 255 : 0; /* (only "has a colour" for the fill) */
      }
    g12_tc_fill(nxt, s);
    memcpy(cur, nxt, (size_t)s * s * 4);
    memcpy(ca, na, (size_t)s * s * sizeof(float));
    size_t n = (size_t)s * s;
    memcpy(dst, nxt, n * 4);
    for (size_t i = 0; i < n; i++) dst[i * 4 + 3] = (u8)(na[i] + 0.5f);
    if (hard && share0 > 0) {
      size_t target = (size_t)(share0 * (double)n + 0.5);
      if (target < 1) target = 1; /* a cut-out never vanishes */
      if (target > n) target = n;
      if (cover(dst, n, 1) != target) {
        /* count(k) is a step function (the alphas are quantised: a 2 x 2 mean takes 5 values at level 1): sort the alphas,
         * take the achievable count nearest the target (ties: the larger; never 0 for a texture with coverage), and k in the middle (log scale) of the range
         * that gives it, so a level that is opaque stays at alpha 255 and over-covered levels shrink back. */
        float *sa = (float *)malloc(n * sizeof(float));
        if (sa) {
          for (size_t i = 0; i < n; i++) sa[i] = na[i];
          qsort(sa, n, sizeof(float), cmp_desc);
          size_t best = (size_t)-1, bd = (size_t)-1;
          for (size_t c = 0; c <= n; c++) {
            if (c && c < n && sa[c - 1] == sa[c]) continue;
            if (c && !sa[c - 1]) continue;
            if (!c && target) continue; /* never lose a cut-out entirely */
            size_t d = c > target ? c - target : target - c;
            if (d <= bd) bd = d, best = c;
          }
          double kl = best ? 127.5 / sa[best - 1] : 1.0 / 512;
          double ku = best < n && sa[best] ? 127.5 / sa[best] : 512;
          double k = sqrt(kl * ku);
          for (size_t i = 0; i < n; i++) {
            double a = (double)na[i] * k + 0.5;
            dst[i * 4 + 3] = (u8)(a > 255 ? 255 : a);
          }
          free(sa);
        }
      }
    }
    dst += n * 4;
  }
  free(cur);
  free(nxt);
  free(ca);
  free(na);
}
