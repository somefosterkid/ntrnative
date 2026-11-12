#define _POSIX_C_SOURCE 200809L
/* port/gpu: the span check (G2). OWNED BY G2.
 *
 *   gpu_cover FILE.vcs [-q]
 *
 * The GPU draws the frame with GPU_FLAT (G3, G4 and G5 off: every covered pixel is written opaque white, with its edge
 * flag, its coverage and the polygon that covered it last) and vcore's rasterizer draws the same polygons through a
 * recording plot that writes every pixel it is offered (no depth test). At 1x the two must agree on every pixel: the
 * covered set (alpha), the edge flag, the last polygon, and with antialiasing on the coverage. Exit 1 on a difference. */
#include "gpu.h"
#include "vc_shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int curk, counter;
static int lastk[VC_H][VC_W];

static void rec_plot(VCoreState *s, const VC_Poly *p, int x, int y, u32 depth, u32 col, int edge) {
  (void)p;
  (void)depth;
  (void)col;
  s->f3d.color[y][x] = 0x1F3FFFF;
  s->f3d.depth[y][x] = (u32)++counter;
  s->f3d.flags[y][x] = edge ? VC_F3_EDGE : 0;
  lastk[y][x] = curk;
}

int main(int argc, char **argv) {
  const char *in = NULL;
  int quiet = 0, api = GPU_API_ES;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-q")) quiet = 1;
    else if (in == NULL) in = argv[i];
  }
  if (in == NULL) {
    fprintf(stderr, "usage: gpu_cover FILE.vcs [-q]\n");
    return 2;
  }
  char err[2048];
  VCoreState *s = vcore_create();
  if (s == NULL || vcore_load_vcs(s, in, err, sizeof err) != 0) {
    fprintf(stderr, "gpu_cover: %s: %s\n", in, s ? err : "out of memory");
    return 1;
  }
  vcore_render(s);
  setenv("GPU_FLAT", "1", 1);
  GpuCtx *ctx = gpu_ctx_headless(api, err, sizeof err);
  GpuCore *g = ctx ? gpu_create(gpu_ctx_getproc(ctx), api, 1, 1, err, sizeof err) : NULL;
  if (g == NULL) {
    fprintf(stderr, "gpu_cover: %s\n", err);
    return 1;
  }
  static uint32_t hi[VC_W * VC_H], aux[VC_W * VC_H * 4];
  if (gpu_render_3d(g, s) != 0) {
    fprintf(stderr, "gpu_cover: GL error\n");
    return 1;
  }
  gpu_read_3d(g, hi, aux);
  int aa = (vc_io16(s, VC_REG_DISP3DCNT) >> 4) & 1;
  memset(s->f3d.color, 0, sizeof s->f3d.color);
  memset(s->f3d.depth, 0, sizeof s->f3d.depth);
  memset(s->f3d.flags, 0, sizeof s->f3d.flags);
  memset(s->f3d.coverage, 0, sizeof s->f3d.coverage);
  s->plot = rec_plot;
  for (u32 k = 0; k < s->geo.npoly; k++) {
    curk = (int)k;
    vc_raster_band(s, &s->geo.poly[s->geo.order[k]], 0, VC_H);
  }
  int dcov = 0, dedge = 0, dcv = 0, dlast = 0, first = -1;
  for (int y = 0; y < VC_H; y++)
    for (int x = 0; x < VC_W; x++) {
      int i = y * VC_W + x;
      int vc = s->f3d.depth[y][x] != 0, gc = aux[i * 4 + 3] != 0xFFFFFFFFu;
      int bad = 0;
      if (vc != gc) { dcov++; bad = 1; }
      else if (vc) {
        int ve = (s->f3d.flags[y][x] & VC_F3_EDGE) != 0, ge = ((aux[i * 4 + 1] >> 16) & VC_F3_EDGE) != 0;
        if (ve != ge) { dedge++; bad = 1; }
        if (aa && s->f3d.coverage[y][x] != (aux[i * 4 + 1] >> 24)) { dcv++; bad = 1; }
        if (lastk[y][x] != (int)aux[i * 4 + 3]) { dlast++; bad = 1; }
      }
      if (bad && first < 0) first = i;
    }
  int bad = dcov + dedge + dcv + dlast;
  if (!quiet || bad)
    printf("%s: %s (%u polygons) covered %d edge %d cov %d last %d differ%s", in, bad ? "DIFF" : "same", s->geo.npoly,
           dcov, dedge, dcv, dlast, "");
  if (bad) printf("; first at x %d y %d", first % VC_W, first / VC_W);
  if (!quiet || bad) printf("\n");
  gpu_destroy(g);
  gpu_ctx_destroy(ctx);
  vcore_destroy(s);
  return bad ? 1 : 0;
}
