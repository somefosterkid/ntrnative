#define _POSIX_C_SOURCE 200809L
/* port/gpu: the crack check (G9). OWNED BY G9.
 *
 *   gpu_crack FILE.vcs [N] [--png OUT.png] [-q]
 *
 * Polygon cracks at N x: the pixels no polygon covers (GPU_FLAT: pass R's "last polygon" is none) inside the frame's silhouette.
 * The silhouette is vcore's 1x coverage (the GPU at 1x covers exactly vcore's pixels, gpu_cover) eroded by two DS pixels,
 * so the outline's own N x edge pixels are not counted. A mesh free of T-junctions must give 0. With --png the covered
 * pixels are dark green, the cracks red. Prints one line: silhouette size in N x pixels and the crack count. */
#include "gpu.h"
#include "vc_shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { ER = 2 }; /* the silhouette's erosion in DS pixels */

static int render(GpuCtx *ctx, VCoreState *s, int api, int n, uint32_t *aux, char *err, size_t el) {
  GpuCore *g = gpu_create(gpu_ctx_getproc(ctx), api, n, 1, err, el);
  if (g == NULL) return -1;
  if (gpu_render_3d(g, s) != 0) {
    snprintf(err, el, "GL error");
    gpu_destroy(g);
    return -1;
  }
  uint32_t *hi = malloc((size_t)VC_W * VC_H * n * n * 4);
  gpu_read_3d(g, hi, aux);
  free(hi);
  gpu_destroy(g);
  return 0;
}

int main(int argc, char **argv) {
  const char *in = NULL, *png = NULL;
  int n = 4, quiet = 0, api = GPU_API_ES, pos = 0;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-q")) quiet = 1;
    else if (!strcmp(argv[i], "--png") && i + 1 < argc) png = argv[++i];
    else if (pos++ == 0) in = argv[i];
    else n = atoi(argv[i]);
  }
  if (in == NULL || n < 1) {
    fprintf(stderr, "usage: gpu_crack FILE.vcs [N] [--png OUT.png] [-q]\n");
    return 2;
  }
  char err[2048];
  VCoreState *s = vcore_create();
  if (s == NULL || vcore_load_vcs(s, in, err, sizeof err) != 0) {
    fprintf(stderr, "gpu_crack: %s: %s\n", in, s ? err : "out of memory");
    return 1;
  }
  vcore_render(s);
  /* backdrops (a polygon whose bounding box is a third of the screen or more) would hide every crack: leave them out */
  int skipbig = getenv("GPU_CRACK_ALL") == NULL, dropped = 0;
  if (skipbig) {
    u32 m = 0;
    for (u32 k = 0; k < s->geo.npoly; k++) {
      const VC_Poly *p = &s->geo.poly[s->geo.order[k]];
      int x0 = 1 << 20, x1 = -(1 << 20), y0 = x0, y1 = x1;
      for (int i = 0; i < p->nv; i++) {
        const VC_Vertex *v = &s->geo.vtx[p->v[i]];
        if (v->sx < x0) x0 = v->sx;
        if (v->sx > x1) x1 = v->sx;
        if (v->sy < y0) y0 = v->sy;
        if (v->sy > y1) y1 = v->sy;
      }
      if ((long)(x1 - x0) * (y1 - y0) * 3 >= (long)VC_W * VC_H) { dropped++; continue; }
      s->geo.order[m++] = s->geo.order[k];
    }
    s->geo.npoly = m; /* NOTE: order[] indexes poly[]; npoly counts the drawn ones from here on */
  }
  setenv("GPU_FLAT", "1", 1); /* pass R writes every covered pixel (no depth test, no alpha test): "last" = covered */
  GpuCtx *ctx = gpu_ctx_headless(api, err, sizeof err);
  if (ctx == NULL) {
    fprintf(stderr, "gpu_crack: %s\n", err);
    return 1;
  }
  uint32_t *a1 = malloc((size_t)VC_W * VC_H * 16), *an = malloc((size_t)VC_W * VC_H * n * n * 16);
  if (render(ctx, s, api, 1, a1, err, sizeof err) != 0 || render(ctx, s, api, n, an, err, sizeof err) != 0) {
    fprintf(stderr, "gpu_crack: %s\n", err);
    return 1;
  }
  int W = VC_W * n, H = VC_H * n;
  long sil = 0, bad = 0;
  uint8_t *pic = png ? calloc((size_t)W * H, 4) : NULL;
  for (int y = 0; y < H; y++)
    for (int x = 0; x < W; x++) {
      int cov = an[((size_t)y * W + x) * 4 + 3] != 0xFFFFFFFFu;
      int dx = x / n, dy = y / n, in_sil = dx >= ER && dy >= ER && dx < VC_W - ER && dy < VC_H - ER;
      for (int j = -ER; j <= ER && in_sil; j++)
        for (int i = -ER; i <= ER; i++)
          if (a1[((dy + j) * VC_W + dx + i) * 4 + 3] == 0xFFFFFFFFu) in_sil = 0;
      sil += in_sil;
      if (in_sil && !cov && bad++ < 8 && !quiet) printf("  crack at N x (%d, %d)\n", x, y);
      if (pic) {
        size_t o = ((size_t)y * W + x) * 4;
        pic[o + 3] = 255;
        if (in_sil && !cov) pic[o] = 255;
        else if (cov) pic[o + 1] = 70;
      }
    }
  if (!quiet || bad) printf("%s N=%d: silhouette %ld N x px (%d backdrops left out), cracks %ld\n", in, n, sil, dropped, bad);
  if (pic) {
    vcore_write_png(png, pic, W, H);
  }
  return bad ? 1 : 0;
}
