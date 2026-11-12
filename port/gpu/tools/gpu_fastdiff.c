/* port/gpu: the exact 3D renderer against the fast one (docs/gpu-hwraster.md) on one capture, Metal or GL.
 *
 *   gpu_fastdiff FILE.vcs OUTBASE [--api metal|gl|es] [--scale N] [--aa 1|2] [--time K] [--nopng] [--crop x,y,w,h] [--probe x,y,w,h]
 *
 * --api gl|es: a headless GL context (gpu_ctx_headless, as gpu_render's), the fast pass of src/g10_fast_gl.c (default metal).
 *
 * Writes OUTBASE_exact.png, OUTBASE_fast.png (the N x 3D layers after pass P, the empty part black) and OUTBASE_diff.png (the
 * exact picture darkened; a differing pixel red (colour), yellow (alpha only), magenta (both); brighter = larger), and prints one
 * line: pixels, differing %, by class (edge: the exact pixel or a 4-neighbour has the edge flag or another last polygon; trans: a
 * translucent write on either side; poly: another polygon won the pixel; interp: the same polygon, another colour/depth) and the
 * size of the colour difference (max channel step at 6 bits: 1, 2-3, 4-7, 8+), then the frame's polygons of the kinds the game's
 * captures lack (shadow masks and shadows, wire-frames, lines, dots: vcore_gen.py's scenes have them) and the gaps (pixels with a
 * polygon in Exact and none in Fast: the dots between polygons src/g10_fast.c grow_poly closes). --time K: K more
 * renders of each, the min pass R (S inside) and pass P (E inside) GPU ms of each side. With 4 samples a second line: the pixels with an
 * edge-flagged sample and the "alike" pixels (pass P's fast draw) of each side. */
#include "g00_frame.h"
#include "gpu.h"
#include "vc_shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void to_rgba(const uint32_t *px, uint8_t *rgba, int n) {
  for (int i = 0; i < n; i++) {
    uint32_t c = px[i];
    int a = VC_A5(c) != 0;
    rgba[i * 4 + 0] = (uint8_t)(a ? (VC_R6(c) << 2 | VC_R6(c) >> 4) : 0);
    rgba[i * 4 + 1] = (uint8_t)(a ? (VC_G6(c) << 2 | VC_G6(c) >> 4) : 0);
    rgba[i * 4 + 2] = (uint8_t)(a ? (VC_B6(c) << 2 | VC_B6(c) >> 4) : 0);
    rgba[i * 4 + 3] = 255;
  }
}

int main(int argc, char **argv) {
  const char *in = NULL, *out = NULL;
  int scale = 3, aa = 1, times = 0, png = 1, cx = -1, cy = 0, cw = 0, ch = 0, qx = -1, qy = 0, qw = 1, qh = 1, api = GPU_API_METAL;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--scale") && i + 1 < argc) scale = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--aa") && i + 1 < argc) aa = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--time") && i + 1 < argc) times = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--nopng")) png = 0;
    else if (!strcmp(argv[i], "--api") && i + 1 < argc)
      i++, api = !strcmp(argv[i], "gl") ? GPU_API_GL : !strcmp(argv[i], "es") ? GPU_API_ES : GPU_API_METAL;
    else if (!strcmp(argv[i], "--crop") && i + 1 < argc) sscanf(argv[++i], "%d,%d,%d,%d", &cx, &cy, &cw, &ch);
    else if (!strcmp(argv[i], "--probe") && i + 1 < argc) sscanf(argv[++i], "%d,%d,%d,%d", &qx, &qy, &qw, &qh);
    else if (in == NULL) in = argv[i];
    else out = argv[i];
  }
  if (in == NULL || out == NULL) return 2;
  char err[2048];
  VCoreState *s = vcore_create();
  if (s == NULL || vcore_load_vcs(s, in, err, sizeof err) != 0) {
    fprintf(stderr, "gpu_fastdiff: %s: %s\n", in, s ? err : "out of memory");
    return 1;
  }
  vcore_render(s);
  int nmask = 0, nshadow = 0, nwire = 0, nline = 0, ndot = 0; /* the polygons the 37 captures lack: shadow, wire-frame, line, dot */
  for (u32 i = 0; i < s->geo.npoly && i < VC_MAX_POLYS; i++) {
    const VC_Poly *p = &s->geo.poly[i];
    long long a2 = 0;
    int dot = 1;
    for (int j = 0; j < p->nv; j++) {
      const VC_Vertex *a = &s->geo.vtx[p->v[j]], *b = &s->geo.vtx[p->v[(j + 1) % p->nv]], *v0 = &s->geo.vtx[p->v[0]];
      a2 += (long long)a->sx * b->sy - (long long)b->sx * a->sy;
      if (a->sx != v0->sx || a->sy != v0->sy) dot = 0;
    }
    if (((p->attr >> 4) & 3) == 3) ((p->attr >> 24) & 63) == 0 ? nmask++ : nshadow++;
    if (((p->attr >> 16) & 31) == 0) nwire++;
    if (dot) ndot++;
    else if (a2 == 0) nline++;
  }
  GpuCtx *ctx = api == GPU_API_METAL ? NULL : gpu_ctx_headless(api, err, sizeof err);
  if (api != GPU_API_METAL && ctx == NULL) {
    fprintf(stderr, "gpu_fastdiff: %s\n", err);
    return 1;
  }
  GpuCore *g = gpu_create(ctx ? gpu_ctx_getproc(ctx) : NULL, api, scale, aa, err, sizeof err);
  if (g == NULL) {
    fprintf(stderr, "gpu_fastdiff: %s\n", err);
    return 1;
  }
  gpu_set_timing(g, 1);
  GpuOptions gopt;
  gpu_options_from_env(&gopt);
  gpu_set_options(g, &gopt);
  int W = VC_W * scale, H = VC_H * scale;
  size_t np = (size_t)W * H;
  uint32_t *ex = malloc(np * 4), *hw = malloc(np * 4), *axe = malloc(np * 16), *axh = malloc(np * 16);
  uint8_t *rgba = malloc(np * 4);
  if (!ex || !hw || !axe || !axh || !rgba) return 1;
  gopt.fast = 1;
  gpu_set_options(g, &gopt);
  if (!g->opt.fast) {
    fprintf(stderr, "gpu_fastdiff: this core has no fast renderer: %s\n", gpu_fast_how(g));
    return 1;
  }
  gopt.fast = 0;
  gpu_set_options(g, &gopt);
  int rc = gpu_render_3d(g, s);
  if (rc == 0) gpu_read_3d(g, ex, axe);
  gopt.fast = 1;
  gpu_set_options(g, &gopt);
  int rf = rc == 0 ? gpu_render_3d(g, s) : 0;
  if (rc != 0 || rf != 0) { /* (a GL error comes back negated) */
    fprintf(stderr, "gpu_fastdiff: the %s render failed (%d)\n", rc != 0 ? "exact" : "fast", rc != 0 ? rc : rf);
    return 1;
  }
  gpu_read_3d(g, hw, axh);

  for (int y = qy; qx >= 0 && y < qy + qh; y++) /* --probe: the raster words of both (depth, IDs | flags << 16 | cov << 24, colour, last) */
    for (int x = qx; x < qx + qw; x++) {
      const uint32_t *a = axe + ((size_t)y * W + x) * 4, *b = axh + ((size_t)y * W + x) * 4;
      printf("probe %d,%d: exact %06x %08x %08x %3d | fast %06x %08x %08x %3d\n", x, y, a[0], a[1], a[2], (int)a[3], b[0], b[1], b[2], (int)b[3]);
    }
  long ndiff = 0, nalpha = 0, ncol = 0, cedge = 0, ctrans = 0, cpoly = 0, cinterp = 0, h1 = 0, h2 = 0, h4 = 0, h8 = 0;
  static const int dx[4] = {0, 0, -1, 1}, dy[4] = {-1, 1, 0, 0};
  for (int y = 0; y < H; y++)
    for (int x = 0; x < W; x++) {
      size_t i = (size_t)y * W + x;
      uint32_t a = ex[i], b = hw[i];
      int aa5 = VC_A5(a), ba5 = VC_A5(b);
      int dcol = 0, dalpha = aa5 != ba5;
      if (aa5 != 0 && ba5 != 0) {
        int d = abs((int)VC_R6(a) - (int)VC_R6(b));
        int dg = abs((int)VC_G6(a) - (int)VC_G6(b)), db = abs((int)VC_B6(a) - (int)VC_B6(b));
        if (dg > d) d = dg;
        if (db > d) d = db;
        dcol = d;
      } else if (aa5 != ba5) {
        dcol = 63; /* one side empty */
      }
      uint8_t *o = rgba + i * 4;
      o[0] = (uint8_t)((VC_R6(a) << 2) / 3), o[1] = (uint8_t)((VC_G6(a) << 2) / 3), o[2] = (uint8_t)((VC_B6(a) << 2) / 3), o[3] = 255;
      if (!dcol && !dalpha) continue;
      ndiff++;
      if (dalpha) nalpha++;
      if (dcol) ncol++;
      if (dcol >= 8) h8++;
      else if (dcol >= 4) h4++;
      else if (dcol >= 2) h2++;
      else if (dcol >= 1) h1++;
      /* the class from the exact side's raster words: y = IDs | flags << 16 | cov << 24, w = last polygon */
      const uint32_t *ea = axe + i * 4, *ha = axh + i * 4;
      int edge = (ea[1] >> 16) & 2;
      for (int k = 0; k < 4 && !edge; k++) {
        int nx = x + dx[k], ny = y + dy[k];
        if (nx < 0 || ny < 0 || nx >= W || ny >= H) continue;
        const uint32_t *na = axe + ((size_t)ny * W + nx) * 4;
        if (na[3] != ea[3]) edge = 1;
      }
      int trans = ((ea[1] >> 16) & 4) || ((ha[1] >> 16) & 4);
      if (edge) cedge++;
      else if (trans) ctrans++;
      else if (ea[3] != ha[3]) cpoly++;
      else cinterp++;
      int v = dcol >= 8 ? 255 : 120 + dcol * 16;
      o[0] = (uint8_t)(dcol ? v : 0);
      o[1] = (uint8_t)(dalpha ? v : 0);
      o[2] = (uint8_t)(dalpha && dcol ? v : 0);
    }
  long ngap = 0; /* the pixels Exact covers and Fast leaves empty (a gap between polygons) */
  for (size_t i = 0; i < np; i++) ngap += (int)axe[i * 4 + 3] >= 0 && (int)axh[i * 4 + 3] < 0;
  printf("gpu_fastdiff: %s scale %d aa %d: %zu px, differ %ld (%.2f%%): colour %ld, alpha %ld; edge %ld trans %ld poly %ld interp %ld; "
         "steps 1: %ld 2-3: %ld 4-7: %ld 8+: %ld; polygons %u: shadow masks %d, "
         "shadows %d, wire-frames %d, lines %d, dots %d; gaps %ld\n",
         in, scale, aa, np, ndiff, 100.0 * ndiff / np, ncol, nalpha, cedge, ctrans, cpoly, cinterp, h1, h2, h4, h8, s->geo.npoly,
         nmask, nshadow, nwire, nline, ndot, ngap);
  if (aa == 2) { /* pass P's two draws: the pixels with an edge-flagged sample (bit 30) and the alike ones (bit 31, the fast draw's) */
    long ae = 0, aa_ = 0, be = 0, ba = 0;
    for (size_t i = 0; i < np; i++) {
      ae += (axe[i * 4 + 1] >> 30) & 1, aa_ += axe[i * 4 + 1] >> 31;
      be += (axh[i * 4 + 1] >> 30) & 1, ba += axh[i * 4 + 1] >> 31;
    }
    printf("gpu_fastdiff: edge-flagged pixels exact %.2f%% fast %.2f%%; alike exact %.2f%% fast %.2f%%\n", 100.0 * ae / np,
           100.0 * be / np, 100.0 * aa_ / np, 100.0 * ba / np);
  }
  if (cx >= 0) { /* the crops, 3 x magnified: exact, hw, diff */
    uint8_t *big = malloc((size_t)cw * ch * 9 * 4), *full = malloc(np * 4);
    for (int m = 0; m < 3 && big && full; m++) {
      if (m == 0) to_rgba(ex, full, (int)np);
      else if (m == 1) to_rgba(hw, full, (int)np);
      else memcpy(full, rgba, np * 4);
      for (int y = 0; y < ch * 3; y++)
        for (int x = 0; x < cw * 3; x++)
          memcpy(big + ((size_t)y * cw * 3 + x) * 4, full + ((size_t)(cy + y / 3) * W + cx + x / 3) * 4, 4);
      char path[1024];
      snprintf(path, sizeof path, "%s_crop_%s.png", out, m == 0 ? "exact" : m == 1 ? "fast" : "diff");
      vcore_write_png(path, big, cw * 3, ch * 3);
    }
  }
  if (png) {
    char path[1024];
    snprintf(path, sizeof path, "%s_diff.png", out);
    vcore_write_png(path, rgba, W, H);
    to_rgba(ex, rgba, (int)np);
    snprintf(path, sizeof path, "%s_exact.png", out);
    vcore_write_png(path, rgba, W, H);
    to_rgba(hw, rgba, (int)np);
    snprintf(path, sizeof path, "%s_fast.png", out);
    vcore_write_png(path, rgba, W, H);
  }
  if (times > 0) {
    double rmin[2] = {1e9, 1e9}, pmin[2] = {1e9, 1e9}, cmin[2] = {1e9, 1e9};
    for (int i = 0; i < times; i++)
      for (int m = 0; m < 2; m++) {
        gopt.fast = m;
        gpu_set_options(g, &gopt);
        gpu_render_3d(g, s);
        double u1, r1, p1, c1, t1;
        gpu_last_passes(g, &u1, &r1, &p1);
        gpu_last_times(g, &c1, &t1);
        if (r1 < rmin[m]) rmin[m] = r1;
        if (p1 < pmin[m]) pmin[m] = p1;
        if (c1 < cmin[m]) cmin[m] = c1;
      }
    printf("fasttime: %s scale %d aa %d: exact R+S %.3f P+E %.3f cpu %.3f | fast %.3f P+E %.3f cpu %.3f ms (min of %d)\n", in, scale,
           aa, rmin[0], pmin[0], cmin[0], rmin[1], pmin[1], cmin[1], times);
  }
  return 0;
}
