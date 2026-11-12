/* port/gpu: one .vcs frame through the GPU path (docs/gpu.md, "Testing by CPU"). OWNED BY THE SKELETON.
 *
 *   gpu_render FILE.vcs OUT.png [--3d L.png] [--scale N] [--hi H.png] [--dump-vram V.bin] [--api es|gl|metal] [--time K] [-q]
 *
 * The command line of the oracle (<ref>/oracle/README) so that vcore_check can use it as one: `vcore_check pNN
 * --oracle gpu=build/gpu/gpu_render` compares vcore with the GPU path on a piece's scenes.
 * vcore renders the frame first (its geometry is the GPU path's input; its 2D draws the screens), then the GPU draws
 * the 3D layer at N x in a headless context (EGL, llvmpipe here).
 * OUT.png: both screens (256x384, top first) with engine A recomposited by vcore around the GPU's 3D layer at 1x.
 * --3d: the GPU's 3D layer at 1x as vcore_render --3d writes it (colour 6 -> 8 bits, alpha 0-31 -> 0-255); at N > 1 the
 *       N x layer box-downsampled (each channel's mean over the N x N block, rounded).
 * --hi: the N x layer itself. --time K: K more renders, the mean CPU and total milliseconds per frame (with --compose gpu:
 *       K more pass C draws too, each with glFinish). --gap MS: MS of sleep between the timed renders (the game's cadence: the
 *       GPU idles between frames and clocks down; back-to-back renders run it at its top clock, docs/gpu-hwraster.md).
 * --ablate MASK: the Metal core's test-only ablations (gpu_mtl.h GPU_ABL_*: 1 R, 2 E, 4 P's slow draw, 8 P, 16 C; the picture is
 *       wrong), as the cfgscript's ablate= key, so a pass's share can be timed here.
 * --spans gpu|cpu|nopass: who builds the span records (BWPORT_GPU_SPANS; default gpu: pass S; nopass: the GL thread). --spans-check: pass S's records read
 *       back and diffed against the CPU builder's field by field (exit 1 when an integer field differs).
 * --api metal (a build with the Metal core, macOS): the Metal core, no GL context; --time's milliseconds are then the command
 *       buffers' GPU time per pass (each pass committed and waited for, as GL's glFinish after each).
 * --time K with --compose gpu also times pass C alone, K times as a frame's picture (the pack, its upload and the plan each
 *       time) and K times as an in-between (the same pack: the draw only), then both again after the 3D passes in one command
 *       buffer ("frame": as the game draws a picture): the wall ms, and under Metal the GPU ms. */
#define _POSIX_C_SOURCE 200809L /* setenv, usleep (c_std=c11 hides them on Linux) */
#define _DEFAULT_SOURCE
#include "g07_compose.h"
#include "gpu.h"
#include "gpu_gl.h"
#include "gpu_mtl.h" /* gpu_mtl_ablate (a no-op without the Metal core) */
#include "vc_shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifndef GPU_HAVE_METAL
#define gpu_mtl_commit(wait) ((void)(wait)) /* only the Metal core has one (gpu.h); metal is 0 without it */
#endif

static void to_rgba(const uint32_t *px, uint8_t *rgba, int n) {
  for (int i = 0; i < n; i++) {
    uint32_t c = px[i];
    rgba[i * 4 + 0] = (uint8_t)(VC_R6(c) << 2 | VC_R6(c) >> 4);
    rgba[i * 4 + 1] = (uint8_t)(VC_G6(c) << 2 | VC_G6(c) >> 4);
    rgba[i * 4 + 2] = (uint8_t)(VC_B6(c) << 2 | VC_B6(c) >> 4);
    rgba[i * 4 + 3] = (uint8_t)(VC_A5(c) * 255 / 31);
  }
}

static void downsample(const uint32_t *hi, int n, uint32_t *lo) {
  const char *pt = getenv("GPU_DOWN"); /* GPU_DOWN=point: the block's top-left pixel (the DS sample point), not the mean */
  if (pt != NULL && !strcmp(pt, "point")) {
    for (int y = 0; y < VC_H; y++)
      for (int x = 0; x < VC_W; x++) lo[y * VC_W + x] = hi[(size_t)(y * n) * VC_W * n + x * n];
    return;
  }
  for (int y = 0; y < VC_H; y++)
    for (int x = 0; x < VC_W; x++) {
      uint32_t sum[4] = {0, 0, 0, 0};
      for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++) {
          uint32_t c = hi[(size_t)(y * n + j) * VC_W * n + x * n + i];
          sum[0] += VC_R6(c), sum[1] += VC_G6(c), sum[2] += VC_B6(c), sum[3] += VC_A5(c);
        }
      uint32_t d = (uint32_t)(n * n), r = (sum[0] + d / 2) / d, g = (sum[1] + d / 2) / d, b = (sum[2] + d / 2) / d;
      lo[y * VC_W + x] = VC_RGB6(r, g, b) | ((sum[3] + d / 2) / d) << 24;
    }
}

static double wall_ms(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec * 1e3 + (double)ts.tv_nsec / 1e6;
}

static int usage(void) {
  fprintf(stderr, "usage: gpu_render FILE.vcs OUT.png [--3d L.png] [--scale N] [--hi H.png] [--dump-vram V.bin] "
                  "[--api es|gl|metal] [--time K] [--gap MS] [--ablate MASK] [--compose gpu|vcore] [--3d-from gpu|vcore] [-q]\n");
  return 2;
}

int main(int argc, char **argv) {
  const char *in = NULL, *out = NULL, *out3d = NULL, *outhi = NULL, *outv = NULL;
  int scale = 1, aa = 1, quiet = 0, api = GPU_API_ES, times = 0, spans_check = 0, gap = 0, ablate = 0;
  /* G7: --compose gpu = engine A composited by the GPU (pass C), the screens at N x; --3d-from vcore = pass C's 3D
   * input is vcore's own 1x layer (the 1x byte-exact check). $GPU_COMPOSE / $GPU_3D_FROM set the defaults (vcore_check
   * runs the tool with a fixed command line). */
  const char *ev = getenv("GPU_COMPOSE"), *e3 = getenv("GPU_3D_FROM");
  int compose_gpu = ev != NULL && !strcmp(ev, "gpu"), from_vcore = e3 != NULL && !strcmp(e3, "vcore");
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--3d") && i + 1 < argc) out3d = argv[++i];
    else if (!strcmp(argv[i], "--hi") && i + 1 < argc) outhi = argv[++i];
    else if (!strcmp(argv[i], "--dump-vram") && i + 1 < argc) outv = argv[++i];
    else if (!strcmp(argv[i], "--scale") && i + 1 < argc) scale = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--aa") && i + 1 < argc) aa = !strcmp(argv[i + 1], "smaa") ? GPU_AA_SMAA : atoi(argv[i + 1]), i++;
    else if (!strcmp(argv[i], "--time") && i + 1 < argc) times = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--gap") && i + 1 < argc) gap = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--ablate") && i + 1 < argc) ablate = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--api") && i + 1 < argc)
      i++, api = !strcmp(argv[i], "gl") ? GPU_API_GL : !strcmp(argv[i], "metal") ? GPU_API_METAL : GPU_API_ES;
    else if (!strcmp(argv[i], "--compose") && i + 1 < argc) compose_gpu = !strcmp(argv[++i], "gpu");
    else if (!strcmp(argv[i], "--3d-from") && i + 1 < argc) from_vcore = !strcmp(argv[++i], "vcore");
    else if (!strcmp(argv[i], "--spans") && i + 1 < argc) setenv("BWPORT_GPU_SPANS", argv[++i], 1);
    else if (!strcmp(argv[i], "--spans-check")) spans_check = 1;
    else if (!strcmp(argv[i], "-q")) quiet = 1;
    else if (argv[i][0] == '-') return usage();
    else if (in == NULL) in = argv[i];
    else if (out == NULL) out = argv[i];
    else return usage();
  }
  if (in == NULL || out == NULL) return usage();
  /* --aa 2: multisampling, 2 x 2 samples per pixel (docs/gpu.md "Antialiasing"); the 3D layer (--3d, --hi) stays at N */
  aa = aa == GPU_AA_SMAA ? aa : aa >= 2 ? 2 : 1; /* --aa smaa: SMAA after pass P (N > 1) */
  const int out_scale = scale;
  char err[2048];
  VCoreState *s = vcore_create();
  if (s == NULL || vcore_load_vcs(s, in, err, sizeof err) != 0) {
    fprintf(stderr, "gpu_render: %s: %s\n", in, s ? err : "out of memory");
    return 1;
  }
  vcore_render(s); /* vcore's frame: the geometry the GPU draws, the 2D around it */

  const int metal = api == GPU_API_METAL;
  GpuCtx *ctx = metal ? NULL : gpu_ctx_headless(api, err, sizeof err);
  if (ctx == NULL && !metal) {
    fprintf(stderr, "gpu_render: %s\n", err);
    return 1;
  }
  GpuCore *g = gpu_create(metal ? NULL : gpu_ctx_getproc(ctx), api, scale, aa, err, sizeof err);
  const char *desc = metal ? "Metal" : gpu_ctx_desc(ctx);
  if (g == NULL) {
    fprintf(stderr, "gpu_render: %s\n", err);
    return 1;
  }
  gpu_set_timing(g, 1);
  if (metal && ablate) gpu_mtl_ablate((unsigned)ablate);
  GpuOptions gopt;
  gpu_options_from_env(&gopt);
  gpu_set_options(g, &gopt);
  static uint32_t vc3d[VC_W * VC_H];
  memcpy(vc3d, s->f3d.out, sizeof vc3d); /* vcore's own 3D layer (G7's 1x input, --3d-from vcore) */
  int rc = gpu_render_3d(g, s);
  if (rc != 0) {
    fprintf(stderr, "gpu_render: GL error 0x%x\n", -rc);
    return 1;
  }
  int W = VC_W * scale, H = VC_H * scale;
  uint32_t *hi = malloc((size_t)W * H * 4), *lo = malloc(VC_W * VC_H * 4);
  uint8_t *rgba = malloc((size_t)W * H * 4 > VC_W * VC_H * 8 ? (size_t)W * H * 4 : VC_W * VC_H * 8);
  if (hi == NULL || lo == NULL || rgba == NULL) return 1;
  gpu_read_3d(g, hi, NULL);
  if (spans_check) {
    int nd = gpu_spans_check(g, s, stdout);
    if (nd != 0) rc = 1;
  }
  if (times > 0) {
    double c = 0, t = 0, tmin = 1e9, u = 0, r = 0, po = 0, rmin = 1e9, pmin = 1e9, cmin = 1e9, umin = 1e9, smin = 1e9, sp = 0,
           sa = 0, amin = 1e9, se = 0, emin = 1e9;
    for (int i = 0; i < times; i++) {
      double c1, t1, u1, r1, p1;
      if (gap > 0) usleep((useconds_t)gap * 1000);
      gpu_render_3d(g, s);
      gpu_last_times(g, &c1, &t1);
      gpu_last_passes(g, &u1, &r1, &p1);
      double s1 = gpu_last_pass_s(g);
      c += c1;
      t += t1;
      u += u1;
      r += r1;
      po += p1;
      sp += s1;
      if (t1 < tmin) tmin = t1;
      if (r1 < rmin) rmin = r1;
      if (p1 < pmin) pmin = p1;
      if (c1 < cmin) cmin = c1;
      if (u1 < umin) umin = u1;
      if (s1 < smin) smin = s1;
      double e1 = gpu_last_pass_e(g);
      se += e1;
      if (e1 < emin) emin = e1;
      double a1 = gpu_last_smaa_ms(g);
      sa += a1;
      if (a1 < amin) amin = a1;
    }
    printf("gpu_render: scale %d, %u polygons: %.2f ms CPU (spans, bands, uploads; min %.2f), %.2f ms per frame (min %.2f); "
           "GL: uploads %.2f (min %.2f), S %.2f (min %.2f), R with S %.2f (min %.2f), P %.2f (min %.2f) (%s)\n", scale, s->geo.npoly,
           c / times, cmin, t / times, tmin, u / times, umin, sp / times, smin, r / times, rmin, po / times, pmin,
           desc);
    printf("gpu_render: E %.2f (min %.2f), in P\n", se / times, emin);
    if (aa == GPU_AA_SMAA) printf("gpu_render: SMAA %.2f (min %.2f)\n", sa / times, amin);
  }
  if (outhi != NULL) {
    to_rgba(hi, rgba, W * H);
    if (vcore_write_png(outhi, rgba, W, H) != 0) rc = 1;
  }
  if (scale == 1) memcpy(lo, hi, VC_W * VC_H * 4);
  else downsample(hi, scale, lo);
  if (out3d != NULL) {
    to_rgba(lo, rgba, VC_W * VC_H);
    if (vcore_write_png(out3d, rgba, VC_W, VC_H) != 0) rc = 1;
  }
  if (compose_gpu) {
    /* pass C: engine A composited on the GPU around the 3D layer (the GPU's at N x, or vcore's at 1x); both screens */
    G07 *c = metal ? g07_create_metal(out_scale, err, sizeof err) : g07_create(out_scale, err, sizeof err);
    if (c == NULL) {
      fprintf(stderr, "gpu_render: %s\n", err);
      return 1;
    }
    /* the GPU's layer: the core's own texture, as the game's bridge passes it (with SMAA pass C blends it: hi is blended
     * already; with 4 samples its alpha carries the edges' empty part, which the readback drops) */
    unsigned t3 = from_vcore ? g07_upload_3d(c, vc3d, 1) : gpu_3d_tex(g), smaa = from_vcore ? 0 : gpu_smaa_weights(g);
    /* a frame that captures wrote VRAM during vcore_render (and vcore's screens are then the ones shown): a second,
     * never-rendered state gives the 2D layers the VRAM they were drawn from, with engine B's picture from s */
    VCoreState *s2 = vcore_create();
    if (s2 == NULL || vcore_load_vcs(s2, in, err, sizeof err) != 0) return 1;
    vc_vram_map(s2);
    memcpy(s2->eng_out[VC_ENG_B], s->eng_out[VC_ENG_B], sizeof s->eng_out[0]);
    int r7 = g07_compose(c, s2, NULL, t3, from_vcore ? 1 : scale, smaa);
    if (r7 != 0) {
      fprintf(stderr, "gpu_render: compose: GL error 0x%x\n", -r7);
      return 1;
    }
    if (times > 0) { /* --time with --compose gpu: pass C alone (the pack, its uploads, the plan, the draw), glFinish after
                      * each; then as an in-between picture (the frame's pack again: no upload, no plan) */
      VC_Line *st = malloc(sizeof(VC_Line) * VC_H); /* the 2D layers as the worker leaves them */
      G07Pack *k = g07_pack_create();
      if (st == NULL || k == NULL) return 1;
      g07_stage_build(s2, st);
      g07_pack(k, s2, st);
      for (int mode = 0; mode < 4; mode++) { /* 2, 3: a whole picture's (the 3D passes, then pass C), one command buffer */
        if (mode == 2) gpu_set_timing(g, 0);
        if (metal) gpu_mtl_commit(1);
        else GL(Finish)();
        double sum = 0, cmin = 1e9, gsum = 0, gmin = 1e9;
        for (int i = 0; i < times; i++) {
          if (gap > 0) usleep((useconds_t)gap * 1000);
          double t0 = wall_ms();
          if (!(mode & 1)) g07_pack(k, s2, st); /* a frame's picture: a new pack (uploaded, its plan made) */
          if (mode >= 2) gpu_render_3d(g, s);
          g07_compose_packed(c, s2, k, t3, from_vcore ? 1 : scale, smaa);
          if (metal) gpu_mtl_commit(1);
          else GL(Finish)();
          double t = wall_ms() - t0;
          sum += t;
          if (t < cmin) cmin = t;
          double gt = g07_gpu_ms(c);
          gsum += gt;
          if (gt < gmin) gmin = gt;
        }
        static const char *const whats[] = {"compose", "compose in-between", "frame", "frame in-between"};
        const char *what = whats[mode];
        printf("gpu_render: %s %.2f (min %.2f)\n", what, sum / times, cmin);
        if (metal) printf("gpu_render: %s GPU %.3f (min %.3f)\n", what, gsum / times, gmin);
      }
      g07_pack_destroy(k);
      free(st);
    }
    vcore_destroy(s2);
    uint8_t *fr = malloc((size_t)W * H * 8);
    if (fr == NULL) return 1;
    g07_read_frame(c, fr);
    if (vcore_write_png(out, fr, VC_W * out_scale, VC_H * 2 * out_scale) != 0) rc = 1;
    free(fr);
    g07_destroy(c);
  } else {
  /* the screens: vcore's engine A again, around the GPU's 3D layer (p20's vc_render_line and its screen choice) */
  memcpy(s->f3d.out, lo, sizeof s->f3d.out);
  for (int y = 0; y < VC_H; y++) vc_render_line(s, VC_ENG_A, y);
  int a_top = (vc_io16(s, VC_REG_POWCNT1) >> 15) & 1;
  memcpy(s->screen[a_top ? VC_SCREEN_TOP : VC_SCREEN_BOTTOM], s->eng_out[VC_ENG_A], sizeof s->eng_out[0]);
  vcore_frame_rgba(s, rgba);
  if (vcore_write_png(out, rgba, VC_W, VC_H * 2) != 0) rc = 1;
  }
  if (outv != NULL) {
    FILE *f = fopen(outv, "wb");
    if (f == NULL || fwrite(s->vram, 1, VC_VRAM_SIZE, f) != VC_VRAM_SIZE) rc = 1;
    if (f != NULL) fclose(f);
  }
  if (!quiet) printf("gpu_render: %s -> %s (scale %d, %u polygons, %s)\n", in, out, scale, s->geo.npoly, desc);
  gpu_destroy(g);
  if (ctx != NULL) gpu_ctx_destroy(ctx);
  vcore_destroy(s);
  free(hi);
  free(lo);
  free(rgba);
  return rc;
}
