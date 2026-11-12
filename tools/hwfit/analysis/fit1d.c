/* fit1d: fit the colour rule along a Y-major edge from tabulated oracle observations (dy A5 B5 t value).
 * Families: S = slope accumulation (slope = round(delta << k / dy), v = A<<k + off + t*slope, floored),
 *           F = factor (fac = round(t << FB / dy), v = A + delta*fac >> FB, then + off).
 * Run separately for rising (B > A) and falling (B < A) channels. gcc -O2 -o fit1d fit1d.c */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int64_t s64;
typedef struct { int dy, a, b, t, v; } Obs;
static Obs *obs;
static int n;
static s64 fdivs(s64 x, s64 d) { s64 q = x / d; if ((x % d) != 0 && ((x < 0) != (d < 0))) q--; return q; }
static s64 rdiv(s64 x, s64 d, int m) { /* d > 0; 0 floor, 1 ceil, 2 nearest(half up), 3 toward zero */
  switch (m) {
  case 0: return fdivs(x, d);
  case 1: return -fdivs(-x, d);
  case 2: return fdivs(2 * x + d, 2 * d);
  default: return x / d;
  }
}
static int c6(int c) { return c * 2 + (c > 0); }
typedef struct { int fam, k, m, fm, from, off16, base; } M;
static int model(const M *m, const Obs *o) {
  s64 A = m->base == 0 ? c6(o->a) : o->a * 2, B = m->base == 0 ? c6(o->b) : o->b * 2;
  s64 d = B - A, v;
  int k = m->k;
  if (m->fam == 0) {
    s64 slope = rdiv(d << k, o->dy, m->m);
    if (m->from == 0) v = (A << k) + (s64)m->off16 * (1LL << k) / 16 + o->t * slope;
    else v = (B << k) - (o->dy - o->t) * slope + (s64)m->off16 * (1LL << k) / 16;
    return (int)fdivs(v, 1LL << k);
  } else {
    s64 t = m->from == 0 ? o->t : o->dy - o->t;
    s64 fac = rdiv(t << k, o->dy, m->fm);
    s64 prod = d * fac;
    s64 r = m->m == 0 ? fdivs(prod, 1LL << k) : m->m == 1 ? -fdivs(-prod, 1LL << k) : fdivs(2 * prod + (1LL << k), 2LL << k);
    v = m->from == 0 ? A + r : B - r;
    s64 s = v * 16 + m->off16;
    return (int)fdivs(s, 16);
  }
}
int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "r");
  int rising = atoi(argv[2]);
  obs = malloc(sizeof(Obs) * 100000);
  Obs o;
  while (fscanf(f, "%d %d %d %d %d", &o.dy, &o.a, &o.b, &o.t, &o.v) == 5) {
    if (rising > 0 && !(o.b > o.a)) continue;
    if (rising < 0 && !(o.b < o.a)) continue;
    if (rising == 0) continue;
    if (o.t == 0) continue; /* the apex row is the vertex */
    obs[n++] = o;
  }
  printf("%d obs\n", n);
  typedef struct { M m; int ok; } R;
  R best[10];
  memset(best, 0, sizeof best);
  for (int fam = 0; fam < 2; fam++)
    for (int k = 0; k <= 16; k++)
      for (int m1 = 0; m1 < (fam ? 3 : 4); m1++)
        for (int fm = 0; fm < (fam ? 3 : 1); fm++)
          for (int from = 0; from < 2; from++)
            for (int off16 = (fam ? -16 : 0); off16 <= 32; off16++)
              for (int base = 0; base < 2; base++) {
                M m = {fam, k, m1, fm, from, off16, base};
                int ok = 0;
                for (int i = 0; i < n; i++) ok += model(&m, &obs[i]) == obs[i].v;
                int w = 0;
                for (int j = 1; j < 10; j++) if (best[j].ok < best[w].ok) w = j;
                if (ok > best[w].ok) { best[w].m = m; best[w].ok = ok; }
              }
  for (int j = 0; j < 10; j++) {
    int bi = 0;
    for (int i = 1; i < 10; i++) if (best[i].ok > best[bi].ok) bi = i;
    M *m = &best[bi].m;
    printf("%6.2f%%  fam=%d k=%d m=%d fm=%d from=%d off16=%d base=%d\n", 100.0 * best[bi].ok / n, m->fam, m->k, m->m, m->fm, m->from,
           m->off16, m->base);
    best[bi].ok = -1;
  }
  return 0;
}
