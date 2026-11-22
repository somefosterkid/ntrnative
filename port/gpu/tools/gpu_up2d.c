/* gpu_up2d FILE.vcs... [--scale N] [--filter2d scale2x|mmpx] [--api es|gl] [--png PREFIX] [-q]
 * The 2D filter's reference compare and invariants (docs: design "How to test it" 2 and 3). For each capture: pass C is
 * drawn Off, Identity and with the style on the GPU (GL / ES), and tools/up2d_ref.c draws the style from pass C's own plan
 * and 1x lines and the 3D layer; then
 *   ref:      GPU style picture == the C reference, every pixel                                        (must be 0 differ)
 *   identity: the Identity picture == Off's                                                           (0)
 *   n1:       at N = 1 the style's picture == Off's (the filter is off at 1x)                          (0)
 *   3d:       N x pixels whose DS pixel and its whole 5 x 5 are "3D on top" equal Off's                (0)
 *   palette:  in 2D-only areas every colour is Off's colour of a DS pixel within the 5 x 5            (0)
 *   moved:    N x pixels where the pick is a neighbour and the picture differs from Off: information
 *   trans:    the reference's picks are translation-invariant: the inputs shifted by one DS pixel (x, then y) give the
 *             same picks one pixel along, away from the screen's borders                               (0)
 * Antialiasing is off (the layer is the plain N x texture). Exit status 1 when a must-be-0 count is not 0. */
#include "g07_compose.h"
#include "gpu.h"
#include "gpu_gl.h"
#include "up2d_ref.h"
#include "vc_shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Counts {
  long ref, ident, n1, d3, pal, trans;
  long moved_diff, moved, blended, checked3d, checkedpal;
} Counts;

static int usage(void) {
  fprintf(stderr, "usage: gpu_up2d FILE.vcs... [--scale N] [--filter2d scale2x|mmpx] [--api es|gl] [--png PREFIX] [-q]\n");
  return 2;
}

static int clampi(int v, int lo, int hi) { return v < lo ? lo : v > hi ? hi : v; }

static int same_px(const uint8_t *a, const uint8_t *b) { return a[0] == b[0] && a[1] == b[1] && a[2] == b[2]; }

/* the first row of the A screen in the output: the top screen when atop, else the bottom */
static int a_row0(const UpRefIn *in) { return in->atop ? 0 : 192 * in->scale; }

static void write_png(const char *prefix, const char *name, const uint8_t *rgba, int w, int h) {
  char p[512];
  snprintf(p, sizeof p, "%s_%s.png", prefix, name);
  vcore_write_png(p, rgba, w, h);
}

/* the 3D-on-top test: effective rank 1 at DS pixel (x, y) of the A screen (a composed A with a 3D layer) */
static int top3d(const UpRefIn *in, int x, int y) {
  return in->compose && in->have3d && ((in->plan[2 * (y * 256 + x)] >> 24) & 3) == 1;
}
static int flat2d(const UpRefIn *in, int x, int y, int isA) { /* the 2D picture alone decides the colour here */
  if (!isA || !in->compose) return 1;
  return !in->have3d || ((in->plan[2 * (y * 256 + x)] >> 24) & 3) == 0;
}

static void invariants(const UpRefIn *in, const uint8_t *off, const uint8_t *up, Counts *c) {
  const int N = in->scale, W = 256 * N;
  for (int half = 0; half < 2; half++) {
    int isA = (half == 0) == (in->atop != 0), row0 = half * 192 * N, y0 = isA ? 0 : 192;
    for (int dy = 0; dy < 192; dy++)
      for (int dx = 0; dx < 256; dx++) {
        int all3d = 0, flat = 1; /* flat: the whole 5 x 5 is 2D only (next to the 3D a quarter may show the 3D: legal) */
        for (int j = -2; j <= 2 && flat; j++)
          for (int i = -2; i <= 2; i++)
            if (!flat2d(in, clampi(dx + i, 0, 255), clampi(dy + j, 0, 191), isA)) {
              flat = 0;
              break;
            }
        if (isA && in->compose && in->have3d) { /* the 3D untouched: the whole 5 x 5 on 3D */
          all3d = 1;
          for (int j = -2; j <= 2 && all3d; j++)
            for (int i = -2; i <= 2; i++)
              if (!top3d(in, clampi(dx + i, 0, 255), clampi(dy + j, 0, 191))) {
                all3d = 0;
                break;
              }
        }
        for (int sy = 0; sy < N; sy++)
          for (int sx = 0; sx < N; sx++) {
            size_t o = ((size_t)(row0 + dy * N + sy) * W + dx * N + sx) * 4;
            if (all3d) {
              c->checked3d++;
              if (!same_px(off + o, up + o)) c->d3++;
            }
            if (flat) { /* the palette: Off's colour of some DS pixel of the 5 x 5 (constant over a flat DS pixel) */
              int ok = 0;
              c->checkedpal++;
              for (int j = -2; j <= 2 && !ok; j++)
                for (int i = -2; i <= 2 && !ok; i++) {
                  int nx = clampi(dx + i, 0, 255), ny = clampi(dy + j, 0, 191);
                  if (!flat2d(in, nx, ny, isA)) continue;
                  size_t on = ((size_t)(row0 + ny * N) * W + nx * N) * 4;
                  ok = same_px(up + o, off + on);
                }
              if (!ok && c->pal++ < 4 && getenv("UP2D_DEBUG"))
                fprintf(stderr, "palette: DS (%d,%d) half %d sub (%d,%d): up %02x%02x%02x off %02x%02x%02x\n", dx, dy, half, sx,
                        sy, up[o], up[o + 1], up[o + 2], off[o], off[o + 1], off[o + 2]);
            }
          }
      }
    (void)y0;
  }
}

/* the reference's picks are translation-invariant: shift the inputs by one DS pixel and compare the picks */
static long translation(const UpRefIn *in, int style) {
  const int N = in->scale;
  uint32_t *plan2 = malloc(sizeof(uint32_t) * 256 * 192 * 2), *src2 = malloc(sizeof(uint32_t) * 256 * 384);
  long bad = 0;
  if (plan2 == NULL || src2 == NULL) return -1;
  for (int dir = 0; dir < 2; dir++) { /* 0: x + 1, 1: y + 1 (within each screen) */
    for (int y = 0; y < 192; y++)
      for (int x = 0; x < 256; x++) {
        int sx = dir == 0 ? clampi(x - 1, 0, 255) : x, sy = dir == 1 ? clampi(y - 1, 0, 191) : y;
        memcpy(plan2 + 2 * (y * 256 + x), in->plan + 2 * (sy * 256 + sx), 8);
        src2[y * 256 + x] = in->src[sy * 256 + sx];
        src2[(y + 192) * 256 + x] = in->src[(sy + 192) * 256 + sx];
      }
    UpRefIn in2 = *in;
    in2.plan = plan2;
    in2.src = src2;
    in2.mm = NULL;
    up2d_ref_prepare(&in2, style);
    const int m = style == G07_UP_MMPX ? 6 : 3; /* a pick reads keys up to m - 1 DS pixels away (MMPX twice: 5) */
    for (int half = 0; half < 2; half++)
      for (int y = m; y < 192 - m; y++)
        for (int x = m; x < 256 - m; x++)
          for (int sy = 0; sy < N; sy++)
            for (int sx = 0; sx < N; sx++) {
              int yy = y + 192 * half, nx = x + (dir == 0), ny = yy + (dir == 1);
              UpRefPick a = up2d_ref_pick(in, style, x, yy, 2 * sx + 1, 2 * sy + 1);
              UpRefPick b = up2d_ref_pick(&in2, style, nx, ny, 2 * sx + 1, 2 * sy + 1);
              if (a.ax != b.ax || a.ay != b.ay || a.bx != b.bx || a.by != b.by || a.w != b.w) bad++;
            }
    up2d_ref_release(&in2);
  }
  free(plan2);
  free(src2);
  return bad;
}

/* one capture at scale N; returns 0 on success, 1 when an invariant failed, 2 on a setup failure */
static int run_one(const char *file, int N, int style, int api, const char *png, int quiet, Counts *tot) {
  char err[2048];
  VCoreState *s = vcore_create();
  if (s == NULL || vcore_load_vcs(s, file, err, sizeof err) != 0) {
    fprintf(stderr, "gpu_up2d: %s: %s\n", file, s ? err : "out of memory");
    return 2;
  }
  vcore_render(s);
  GpuCtx *ctx = gpu_ctx_headless(api, err, sizeof err);
  if (ctx == NULL) {
    fprintf(stderr, "gpu_up2d: %s\n", err);
    return 2;
  }
  GpuCore *g = gpu_create(gpu_ctx_getproc(ctx), api, N, 1, err, sizeof err);
  if (g == NULL) {
    fprintf(stderr, "gpu_up2d: %s\n", err);
    return 2;
  }
  GpuOptions gopt;
  gpu_options_from_env(&gopt);
  gpu_set_options(g, &gopt);
  if (gpu_render_3d(g, s) != 0) return 2;
  const int W = VC_W * N, H = VC_H * N;
  uint32_t *hi = malloc((size_t)W * H * 4);
  uint8_t *off = malloc((size_t)W * H * 8), *idn = malloc((size_t)W * H * 8), *up = malloc((size_t)W * H * 8),
          *ref = malloc((size_t)W * H * 8);
  UpRefPick *picks = malloc((size_t)W * H * 2 * sizeof(UpRefPick));
  static uint32_t plan[VC_W * VC_H * 2], src[VC_W * VC_H * 2];
  if (!hi || !off || !idn || !up || !ref || !picks) return 2;
  gpu_read_3d(g, hi, NULL);

  G07 *c = g07_create(N, err, sizeof err);
  if (c == NULL) {
    fprintf(stderr, "gpu_up2d: %s\n", err);
    return 2;
  }
  VCoreState *s2 = vcore_create();
  if (s2 == NULL || vcore_load_vcs(s2, file, err, sizeof err) != 0) return 2;
  vc_vram_map(s2);
  memcpy(s2->eng_out[VC_ENG_B], s->eng_out[VC_ENG_B], sizeof s->eng_out[0]);
  static VC_Line st[VC_H];
  g07_stage_build(s2, st);
  G07Pack *k = g07_pack_create();
  g07_pack(k, s2, st);
  unsigned t3 = gpu_3d_tex(g);
  int rc = 0;

  g07_set_up2d(c, G07_UP_OFF);
  if (g07_compose_packed(c, s2, k, t3, N, 0) != 0) return 2;
  g07_read_frame(c, off);
  if (!getenv("UP2D_SKIPID")) {
    g07_set_up2d(c, G07_UP_IDENTITY);
    if (g07_compose_packed(c, s2, k, t3, N, 0) != 0) return 2;
  }
  g07_read_frame(c, idn);
  g07_set_up2d(c, style);
  if (g07_compose_packed(c, s2, k, t3, N, 0) != 0) return 2;
  g07_read_frame(c, up);

  G07UpIn gi;
  if (g07_up_inputs(c, s2, k, N, 1, &gi, plan, src) != 0) {
    fprintf(stderr, "gpu_up2d: %s: the plan cannot be read back\n", file);
    return 2;
  }
  UpRefIn in = {N, gi.compose, gi.atop, gi.have3d, gi.hofs, gi.evy, gi.bright, plan, src, hi, NULL};
  UpRefStats stt;
  up2d_ref_prepare(&in, style);
  up2d_ref_frame(&in, style, ref, picks, &stt);

  Counts cn;
  memset(&cn, 0, sizeof cn);
  for (size_t i = 0; i < (size_t)W * H * 2; i++) {
    if (!same_px(up + i * 4, ref + i * 4) && N > 1) {
      if (cn.ref++ < 6 && getenv("UP2D_DEBUG"))
        fprintf(stderr, "ref diff at (%d,%d): up %02x%02x%02x ref %02x%02x%02x off %02x%02x%02x pick %d,%d,%d\n", (int)(i % W),
                (int)(i / W), up[i * 4], up[i * 4 + 1], up[i * 4 + 2], ref[i * 4], ref[i * 4 + 1], ref[i * 4 + 2], off[i * 4],
                off[i * 4 + 1], off[i * 4 + 2], picks[i].ax, picks[i].ay, picks[i].w);
    }
    if (!same_px(idn + i * 4, off + i * 4)) cn.ident++;
    if ((picks[i].ax || picks[i].ay || picks[i].w) && !same_px(up + i * 4, off + i * 4)) cn.moved_diff++;
  }
  long updiff = 0, refoff = 0;
  for (size_t i = 0; i < (size_t)W * H * 2; i++) {
    updiff += !same_px(up + i * 4, off + i * 4);
    refoff += !same_px(ref + i * 4, off + i * 4);
  }
  if (getenv("UP2D_DEBUG")) fprintf(stderr, "up!=off %ld, ref!=off %ld\n", updiff, refoff);
  cn.moved = stt.moved;
  cn.blended = stt.blended;
  if (N == 1) {
    for (size_t i = 0; i < (size_t)W * H * 2; i++)
      if (!same_px(up + i * 4, off + i * 4)) cn.n1++;
  } else {
    invariants(&in, off, up, &cn);
    cn.trans = translation(&in, style);
  }
  long bad = cn.ref + cn.ident + cn.n1 + cn.d3 + cn.pal + (cn.trans > 0 ? cn.trans : 0);
  if (!quiet || bad)
    printf("%s x%d: ref %ld differ, identity %ld, n1 %ld, 3d %ld/%ld, palette %ld/%ld, trans %ld; moved %ld (%ld differ from Off), "
           "blended %ld of %ld%s\n", file, N, cn.ref, cn.ident, cn.n1, cn.d3, cn.checked3d, cn.pal, cn.checkedpal, cn.trans,
           cn.moved, cn.moved_diff, cn.blended, (long)stt.px, bad ? "  FAIL" : "");
  if (png != NULL) {
    write_png(png, "off", off, W, 2 * H);
    write_png(png, "up", up, W, 2 * H);
    write_png(png, "ref", ref, W, 2 * H);
  }
  if (bad) rc = 1;
  tot->ref += cn.ref, tot->ident += cn.ident, tot->n1 += cn.n1, tot->d3 += cn.d3, tot->pal += cn.pal;
  tot->trans += cn.trans > 0 ? cn.trans : 0, tot->moved += cn.moved, tot->moved_diff += cn.moved_diff;
  tot->blended += cn.blended, tot->checked3d += cn.checked3d, tot->checkedpal += cn.checkedpal;
  up2d_ref_release(&in);
  g07_pack_destroy(k);
  g07_destroy(c);
  vcore_destroy(s2);
  gpu_destroy(g);
  gpu_ctx_destroy(ctx);
  vcore_destroy(s);
  free(hi), free(off), free(idn), free(up), free(ref), free(picks);
  return rc;
}

int main(int argc, char **argv) {
  const char *files[4096], *png = NULL;
  int nf = 0, N = 3, style = G07_UP_SCALE2X, api = GPU_API_ES, quiet = 0;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--scale") && i + 1 < argc) N = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--filter2d") && i + 1 < argc) {
      if ((style = g07_up2d_parse(argv[++i])) <= G07_UP_IDENTITY) return usage();
    } else if (!strcmp(argv[i], "--api") && i + 1 < argc) api = !strcmp(argv[++i], "gl") ? GPU_API_GL : GPU_API_ES;
    else if (!strcmp(argv[i], "--png") && i + 1 < argc) png = argv[++i];
    else if (!strcmp(argv[i], "-q")) quiet = 1;
    else if (argv[i][0] == '-' || nf >= 4096) return usage();
    else files[nf++] = argv[i];
  }
  if (nf == 0 || N < 1) return usage();
  Counts tot;
  memset(&tot, 0, sizeof tot);
  int fails = 0, errs = 0;
  for (int i = 0; i < nf; i++) {
    int r = run_one(files[i], N, style, api, png, quiet, &tot);
    if (r == 1) fails++;
    if (r == 2) errs++;
  }
  printf("gpu_up2d: scale %d, %d captures, %d failed, %d errors; ref %ld, identity %ld, n1 %ld, 3d %ld (of %ld px), palette %ld "
         "(of %ld px), trans %ld; moved %ld (%ld differ from Off), blended %ld\n", N, nf, fails, errs, tot.ref, tot.ident,
         tot.n1, tot.d3, tot.checked3d, tot.pal, tot.checkedpal, tot.trans, tot.moved, tot.moved_diff, tot.blended);
  return fails || errs ? 1 : 0;
}
