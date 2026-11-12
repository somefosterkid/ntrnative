/* hwfit_raster: the candidate-model harness for the 3D rasterizer's colour interpolation (tools/hwfit/README.md).
 *
 * Reads D/probes.bin (hwfit.py pack: one triangle per probe, its screen vertices and the oracle's pixels), enumerates
 * parameter combinations of an interpolation model, renders every probe's triangle colours with each candidate and
 * scores the pixels against the oracle's. Coverage (which pixels the polygon fills, each row's first and last pixel)
 * is taken from the oracle: this tool fits the arithmetic of the colours, not the edge rules.
 *
 *   hwfit_raster DIR [--n N] [--top K] [--steep] [--set name=v[,name=v..]] [--misses K] [--list]
 *     --steep   only probes whose edges are all Y-major (|dx| < |dy|)
 *     --set     pin parameters (everything else varies over its table); --list prints the parameters
 *     --misses  after the table, K mismatching pixels of the best candidate and the per-width row statistics
 *     MISSW=w   (environment) only misses on rows w pixels wide (99: 16 and wider)
 *
 * The model, per colour channel (values are 9-bit fixed point; the output is value >> 3, floored):
 *   vertex value  vbase(c5)
 *   edge value on row y = interp(vA, vB, y - ya, yb - ya), E fractional bits, rounded em (Y-major edges; with xmaj,
 *                         X-major edges step by x instead)
 *   span value at x     = form sf over (L, R, num = x - xl + numS, den = xr - xl + spanD)
 *       sf 0 exact rational, floored at the end
 *       sf 1 slope: s = rnd((R - L) << K / den), v = L << K + s * num
 *       sf 2 factor: f = rnd(num << K / den), v = L + ((R - L) * f) >> K
 *   which edge is the left one: lr 0 by the exact x on the row, 1 by the triangle's geometry
 * Build: gcc -O2 -fopenmp -o hwfit_raster hwfit_raster.c
 */
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int64_t s64;
typedef struct Vtx { int sx, sy, z, w, c[3]; } Vtx;
typedef struct Row { int y, xl, xr; const Vtx *La, *Lb, *Ra, *Rb; } Row;
typedef struct Probe {
  int id, x0, y0, w, h;
  Vtx v[3];
  uint32_t *pix; /* w*h: r6 | g6 << 8 | b6 << 16 | a5 << 24, 0 = not drawn */
  Row *rows[2];  /* by lr */
  int nrows[2], steep;
} Probe;

static Probe *probes;
static int nprobes;

typedef struct Param { int base, E, em, spanD, numS, sf, K, sm, lr, xmaj; } Param;
typedef struct Dim { const char *name; int off; int n; int vals[12]; } Dim;
#define OFF(f) ((int)__builtin_offsetof(Param, f))
static const Dim dims[] = {
    {"base", OFF(base), 5, {0, 1, 2, 3, 4}},
    {"E", OFF(E), 7, {0, 1, 2, 3, 4, 6, 8}},
    {"em", OFF(em), 3, {0, 1, 2}},
    {"spanD", OFF(spanD), 4, {0, 1, 2, 3}},
    {"numS", OFF(numS), 3, {-1, 0, 1}},
    {"sf", OFF(sf), 3, {0, 1, 2}},
    {"K", OFF(K), 8, {6, 8, 10, 12, 14, 16, 20, 24}},
    {"sm", OFF(sm), 4, {0, 1, 2, 3}},
    {"lr", OFF(lr), 2, {0, 1}},
    {"xmaj", OFF(xmaj), 2, {0, 1}},
};
#define NDIM ((int)(sizeof dims / sizeof dims[0]))
static int pinned[NDIM];

static s64 fdivs(s64 n, s64 d) { s64 q = n / d; if ((n % d) != 0 && ((n < 0) != (d < 0))) q--; return q; }
static s64 rdivm(s64 n, s64 d, int m) { /* d > 0; m: 0 floor 1 nearest 2 ceil 3 toward zero */
  if (m == 0) return fdivs(n, d);
  if (m == 1) return fdivs(2 * n + d, 2 * d);
  if (m == 2) return -fdivs(-n, d);
  return n / d;
}

static s64 vbase(const Param *p, int c) {
  switch (p->base) {
  case 0: return c ? 16 * c + 15 : 0;
  case 1: return 16 * c + 15;
  case 2: return 16 * c;
  case 3: return (s64)(c * 2 + (c > 0)) << 3;
  default: return c ? 16 * c + 8 : 0;
  }
}

static double edge_x(const Vtx *a, const Vtx *b, int y) {
  if (b->sy == a->sy) return a->sx;
  return a->sx + (double)(b->sx - a->sx) * (y - a->sy) / (b->sy - a->sy);
}

/* each probe's rows: first/last drawn pixel and the edges that bound it, for both left/right assignments */
static void prep(Probe *pr) {
  const Vtx *v[3] = {&pr->v[0], &pr->v[1], &pr->v[2]};
  for (int i = 0; i < 3; i++)
    for (int j = i + 1; j < 3; j++)
      if (v[j]->sy < v[i]->sy || (v[j]->sy == v[i]->sy && v[j]->sx < v[i]->sx)) { const Vtx *t = v[i]; v[i] = v[j]; v[j] = t; }
  const Vtx *T = v[0], *M = v[1], *B = v[2];
  pr->steep = 1;
  /* a zero-area triangle (collinear vertices) is a line: no left/right edge to fit */
  long cross = (long)(M->sx - T->sx) * (B->sy - T->sy) - (long)(B->sx - T->sx) * (M->sy - T->sy);
  if (cross == 0) { pr->steep = 0; pr->rows[0] = pr->rows[1] = calloc(1, sizeof(Row)); pr->nrows[0] = pr->nrows[1] = 0; return; }
  for (int i = 0; i < 3; i++) {
    const Vtx *a = &pr->v[i], *b = &pr->v[(i + 1) % 3];
    int dx = abs(a->sx - b->sx), dy = abs(a->sy - b->sy);
    if (dy != 0 && dx >= dy) pr->steep = 0;
  }
  /* geometry: is the long edge T-B on the left? (M right of it) */
  double xm = edge_x(T, B, M->sy);
  int geo_long_left = M->sx > xm ? 1 : M->sx < xm ? 0 : -1;
  for (int lr = 0; lr < 2; lr++) {
    pr->rows[lr] = calloc(pr->h, sizeof(Row));
    pr->nrows[lr] = 0;
    for (int yy = 0; yy < pr->h; yy++) {
      int y = pr->y0 + yy, xl = -1, xr = -1;
      for (int xx = 0; xx < pr->w; xx++)
        if (pr->pix[yy * pr->w + xx]) { if (xl < 0) xl = xx; xr = xx; }
      if (xl < 0 || T->sy == B->sy) continue;
      const Vtx *e0a = T, *e0b = B, *e1a, *e1b;
      if (T->sy == M->sy || y >= M->sy) { e1a = M; e1b = B; } else { e1a = T; e1b = M; }
      if (e1a->sy == e1b->sy) continue;
      double x0 = edge_x(e0a, e0b, y), x1 = edge_x(e1a, e1b, y);
      int long_left = x0 <= x1;
      if (lr == 1 && geo_long_left >= 0) long_left = geo_long_left;
      Row *r = &pr->rows[lr][pr->nrows[lr]++];
      r->y = y; r->xl = xl + pr->x0; r->xr = xr + pr->x0;
      if (long_left) { r->La = e0a; r->Lb = e0b; r->Ra = e1a; r->Rb = e1b; }
      else { r->La = e1a; r->Lb = e1b; r->Ra = e0a; r->Rb = e0b; }
    }
  }
}

static s64 edge_val(const Param *p, const Vtx *a, const Vtx *b, int ch, int y, int xpix) {
  s64 A = vbase(p, a->c[ch]), B = vbase(p, b->c[ch]);
  int dy = b->sy - a->sy, dx = b->sx - a->sx, adx = abs(dx);
  s64 num = y - a->sy, den = dy;
  if (p->xmaj && adx >= dy && dx != 0) {
    num = dx > 0 ? xpix - a->sx : a->sx - xpix;
    if (num < 0) num = 0;
    if (num > adx) num = adx;
    den = adx;
  }
  if (den <= 0) return A << p->E;
  return rdivm((A * den + (B - A) * num) << p->E, den, p->em);
}

/* the span value (E fractional bits; floored to the output by the caller) */
static s64 span_val(const Param *p, s64 L, s64 R, int num, int den) {
  if (den <= 0) return L;
  if (num < 0) num = 0;
  switch (p->sf) {
  case 0: return fdivs(L * den + (R - L) * num, den);
  case 1: {
    s64 s = rdivm((R - L) << p->K, den, p->sm);
    return ((L << p->K) + s * num) >> p->K;
  }
  default: {
    s64 f = rdivm((s64)num << p->K, den, p->sm);
    return L + (((R - L) * f) >> p->K);
  }
  }
}

typedef struct Score { s64 ch[3], all, tot; } Score;
static int g_verbose, g_hist, g_missw = -1;
static s64 hist[11][2]; /* rows by width bucket (1..7, 8-15, 16-31, 32+): [wrong, right] */

static void score_param(const Param *p, int n, int steep, Score *sc) {
  memset(sc, 0, sizeof *sc);
  for (int i = 0; i < n; i++) {
    Probe *pr = &probes[i];
    if (steep && !pr->steep) continue;
    for (int ri = 0; ri < pr->nrows[p->lr]; ri++) {
      const Row *r = &pr->rows[p->lr][ri];
      int yy = r->y - pr->y0;
      s64 Lv[3], Rv[3];
      for (int ch = 0; ch < 3; ch++) {
        Lv[ch] = edge_val(p, r->La, r->Lb, ch, r->y, r->xl);
        Rv[ch] = edge_val(p, r->Ra, r->Rb, ch, r->y, r->xr);
      }
      int rowok = 1, wd = r->xr - r->xl + 1;
      for (int x = r->xl; x <= r->xr; x++) {
        uint32_t o = pr->pix[yy * pr->w + (x - pr->x0)];
        if (!o) continue;
        int ok = 1;
        for (int ch = 0; ch < 3; ch++) {
          s64 val = span_val(p, Lv[ch], Rv[ch], x - r->xl + p->numS, r->xr - r->xl + p->spanD);
          val = fdivs(val, (1LL << p->E) * 8);
          int e = (o >> (8 * ch)) & 63;
          int g = val < 0 ? 0 : val > 63 ? 63 : (int)val;
          if (g == e) sc->ch[ch]++;
          else {
            ok = 0;
            if (g_verbose > 0 && (g_missw < 0 || g_missw == wd || (g_missw == 99 && wd >= 16))) {
              g_verbose--;
              printf("  miss probe %d y %d x %d (xl %d xr %d) ch %d: oracle %d model %d  L %.3f R %.3f\n", pr->id, r->y, x, r->xl, r->xr, ch, e, g,
                     Lv[ch] / 8.0 / (1 << p->E), Rv[ch] / 8.0 / (1 << p->E));
            }
          }
        }
        sc->all += ok;
        sc->tot++;
        if (!ok) rowok = 0;
      }
      if (g_hist) hist[wd < 8 ? wd : wd < 16 ? 8 : wd < 32 ? 9 : 10][rowok]++;
    }
  }
}

static void print_param(const Param *p) {
  for (int d = 0; d < NDIM; d++) printf("%s=%d ", dims[d].name, *(const int *)((const char *)p + dims[d].off));
}

static void load(const char *dir) {
  char path[512];
  snprintf(path, sizeof path, "%s/probes.bin", dir);
  FILE *f = fopen(path, "rb");
  if (!f) { perror(path); exit(1); }
  int32_t n;
  if (fread(&n, 4, 1, f) != 1) exit(1);
  probes = calloc(n, sizeof(Probe));
  for (int i = 0; i < n; i++) {
    int32_t h[7];
    if (fread(h, 4, 7, f) != 7) { n = i; break; }
    Probe *pr = &probes[i];
    pr->id = h[0]; pr->x0 = h[2]; pr->y0 = h[3]; pr->w = h[4]; pr->h = h[5];
    for (int k = 0; k < 3; k++) {
      int32_t q[7];
      if (fread(q, 4, 7, f) != 7) exit(1);
      pr->v[k] = (Vtx){q[0], q[1], q[2], q[3], {q[4], q[5], q[6]}};
    }
    pr->pix = malloc((size_t)pr->w * pr->h * 4);
    if (fread(pr->pix, 4, (size_t)pr->w * pr->h, f) != (size_t)pr->w * pr->h) exit(1);
    prep(pr);
  }
  nprobes = n;
  fclose(f);
}

int main(int argc, char **argv) {
  if (argc < 2) { fprintf(stderr, "usage: hwfit_raster DIR [--n N] [--top K] [--steep] [--set a=1,b=2] [--misses K] [--list]\n"); return 1; }
  int n = 1 << 30, top = 15, steep = 0, nverbose = 0, pinval[NDIM];
  memset(pinned, 0, sizeof pinned);
  for (int i = 2; i < argc; i++) {
    if (!strcmp(argv[i], "--n") && i + 1 < argc) n = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--top") && i + 1 < argc) top = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--steep")) steep = 1;
    else if (!strcmp(argv[i], "--misses") && i + 1 < argc) nverbose = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--list")) {
      for (int d = 0; d < NDIM; d++) { printf("%s:", dims[d].name); for (int k = 0; k < dims[d].n; k++) printf(" %d", dims[d].vals[k]); printf("\n"); }
      return 0;
    } else if (!strcmp(argv[i], "--set") && i + 1 < argc) {
      char *s = strdup(argv[++i]);
      for (char *tok = strtok(s, ","); tok; tok = strtok(NULL, ",")) {
        char *eq = strchr(tok, '=');
        if (!eq) continue;
        *eq = 0;
        for (int d = 0; d < NDIM; d++) if (!strcmp(dims[d].name, tok)) { pinned[d] = 1; pinval[d] = atoi(eq + 1); }
      }
    }
  }
  if (getenv("MISSW")) g_missw = atoi(getenv("MISSW"));
  load(argv[1]);
  if (n > nprobes) n = nprobes;
  /* the dimensions that vary: sf = 0 ignores K and sm; skipped duplicates */
  size_t total = 1;
  int nv[NDIM];
  for (int d = 0; d < NDIM; d++) { nv[d] = pinned[d] ? 1 : dims[d].n; total *= nv[d]; }
  typedef struct { Param p; Score s; int skip; } Res;
  Res *res = calloc(total, sizeof(Res));
  #pragma omp parallel for schedule(dynamic, 4)
  for (long idx = 0; idx < (long)total; idx++) {
    long k = idx;
    Param p;
    int first = 1;
    for (int d = 0; d < NDIM; d++) {
      int ix = k % nv[d];
      k /= nv[d];
      int v = pinned[d] ? pinval[d] : dims[d].vals[ix];
      if (ix != 0 && !pinned[d]) first = 0;
      *(int *)((char *)&p + dims[d].off) = v;
      if (!strcmp(dims[d].name, "sf") ) {}
    }
    /* sf 0 ignores K and sm: keep only their first values */
    if (p.sf == 0) {
      int kfirst = pinned[6] ? 1 : (p.K == dims[6].vals[0]), smfirst = pinned[7] ? 1 : (p.sm == dims[7].vals[0]);
      if (!kfirst || !smfirst) { res[idx].skip = 1; continue; }
    }
    (void)first;
    res[idx].p = p;
    score_param(&p, n, steep, &res[idx].s);
  }
  for (int t = 0; t < top; t++) {
    long best = -1;
    for (long i = 0; i < (long)total; i++) if (!res[i].skip && res[i].s.tot && (best < 0 || res[i].s.all > res[best].s.all)) best = i;
    if (best < 0) break;
    Res *r = &res[best];
    printf("%6.2f%% all (%6.2f %6.2f %6.2f per channel) of %lld px  ", 100.0 * r->s.all / r->s.tot, 100.0 * r->s.ch[0] / r->s.tot,
           100.0 * r->s.ch[1] / r->s.tot, 100.0 * r->s.ch[2] / r->s.tot, (long long)r->s.tot);
    print_param(&r->p);
    printf("\n");
    if (t == 0 && nverbose) {
      Score sc;
      g_verbose = nverbose; g_hist = 1;
      score_param(&r->p, n, steep, &sc);
      g_verbose = 0; g_hist = 0;
      printf("rows by width, right/total:");
      for (int b = 1; b < 11; b++)
        printf(" [%s] %lld/%lld", b < 8 ? (char[]){(char)('0' + b), 0} : b == 8 ? "8-15" : b == 9 ? "16-31" : "32+", (long long)hist[b][1],
               (long long)(hist[b][0] + hist[b][1]));
      printf("\n");
    }
    r->s.all = -1;
  }
  return 0;
}
