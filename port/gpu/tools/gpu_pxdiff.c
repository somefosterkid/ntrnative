#define _POSIX_C_SOURCE 200809L
/* port/gpu: per-pixel differences between vcore's 3D layer and the GPU's at 1x, with the attributes (ga62).
 *
 *   gpu_pxdiff FILE.vcs [-n MAX] [-p X,Y]
 *
 * For each pixel that differs (the 3D rule: colour at 6 bits where alpha > 0, alpha at 5 bits): both colours, the
 * depth/flags/coverage of the raster stage (vcore's f3d vs pass R's aux), the polygon that last changed the pixel on
 * each side, and vcore's attributes of the GPU's polygon at that pixel (colour 9 bit, st, w, z, the span's ends).
 * With -p X,Y: that pixel only, for every polygon covering it. */
#include "gpu.h"
#include "vc_shared.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gpu_read_3d_raster(GpuCore *g, uint32_t *out);
static int curk, vlast[VC_H][VC_W];
static u32 vpre[VC_H][VC_W];

static void wrap_plot(VCoreState *s, const VC_Poly *p, int x, int y, u32 depth, u32 col, int edge) {
  u32 b = s->f3d.color[y][x], d = s->f3d.depth[y][x];
  u8 f = s->f3d.flags[y][x];
  vc_3d_plot(s, p, x, y, depth, col, edge);
  if (s->f3d.color[y][x] != b || s->f3d.depth[y][x] != d || s->f3d.flags[y][x] != f) vlast[y][x] = curk;
}

typedef struct { double w, z, c[3], st[2]; } At;

static double fl(double v) { return floor(v); }

/* vcore's lerp for polygon p at (x, y): returns 1 and prints when covered */
static int attrs(VCoreState *s, const VC_Poly *p, int x, int y, int verbose, At *out) {
  VC_SpanPoly sp;
  int kind = vc_span_poly(&sp, s, p, NULL, NULL, VC_W, VC_H);
  if (kind != 1) return 0;
  if (y < sp.ymin || y > sp.yend) return 0;
  VC_Span r;
  if (!vc_span_row(&sp, y, &r)) return 0;
  int xend = r.dxe < r.xmax ? r.dxe : r.xmax;
  if (x < r.dxs || x > xend) return 0;
  int wbuf = (s->geo.swap_param & 2) != 0;
  double f = (double)(x - r.xs) / (double)r.den_f;
  if (r.flags & VC_SPAN_AA) f = f < 0 ? 0 : f > 1 ? 1 : f;
  At a;
  double q = f;
  const VC_SpanAttr *L = &r.L, *R = &r.R;
  if (L->w != R->w) {
    double k0 = (1.0 - f) / L->w, k1 = f / R->w, sum = k0 + k1;
    q = k1 / sum;
    a.w = 1.0 / sum;
  } else a.w = L->w;
  for (int i = 0; i < 3; i++) a.c[i] = L->c[i] + (R->c[i] - L->c[i]) * q;
  for (int i = 0; i < 2; i++) a.st[i] = L->st[i] + (R->st[i] - L->st[i]) * q;
  a.z = L->z + (R->z - L->z) * ((double)(x - r.xs) / (double)r.den_z);
  if (out) *out = a;
  if (verbose) {
    printf("      xs %d xe %d dxs %d dxe %d den_f %d den_z %d f %.9f q %.9f persp %d\n", r.xs, r.xe, r.dxs, r.dxe, r.den_f,
           r.den_z, f, q, L->w != R->w);
    printf("      L c %.4f %.4f %.4f st %.4f %.4f w %.4f z %.2f\n", L->c[0], L->c[1], L->c[2], L->st[0], L->st[1], L->w, L->z);
    printf("      R c %.4f %.4f %.4f st %.4f %.4f w %.4f z %.2f\n", R->c[0], R->c[1], R->c[2], R->st[0], R->st[1], R->w, R->z);
    printf("      a c %.6f %.6f %.6f (6b %.0f %.0f %.0f) st %.6f %.6f w %.6f z %.4f depth %.0f\n", a.c[0], a.c[1], a.c[2],
           fl((a.c[0] + 1e-6) / 8), fl((a.c[1] + 1e-6) / 8), fl((a.c[2] + 1e-6) / 8), a.st[0], a.st[1], a.w, a.z,
           wbuf ? a.w : a.z);
  }
  return 1;
}

static void polyinfo(const VCoreState *s, int k) {
  const VC_Poly *p = &s->geo.poly[s->geo.order[k]];
  printf("poly %d idx %u nv %d attr %08x tex %08x pltt %04x fmt %d mode %d alpha %d trans %d\n", k, p->index, p->nv, p->attr,
         p->texparam, p->pltt, (p->texparam >> 26) & 7, (p->attr >> 4) & 3, (p->attr >> 16) & 31, p->translucent);
}

int main(int argc, char **argv) {
  const char *in = NULL;
  int maxn = 40, px = -1, py = -1;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-n") && i + 1 < argc) maxn = atoi(argv[++i]);
    else if (!strcmp(argv[i], "-p") && i + 1 < argc) sscanf(argv[++i], "%d,%d", &px, &py);
    else in = argv[i];
  }
  if (!in) return 2;
  char err[2048];
  VCoreState *s = vcore_create();
  if (s == NULL || vcore_load_vcs(s, in, err, sizeof err) != 0) {
    fprintf(stderr, "gpu_pxdiff: %s\n", err);
    return 1;
  }
  vcore_render(s);
  printf("DISP3DCNT %04x CLEAR_COLOR %08x CLEAR_DEPTH %08x FOG_COLOR %08x FOG_OFFSET %04x SWAP %u\n", vc_io16(s, VC_REG_DISP3DCNT),
         vc_io32(s, VC_REG_CLEAR_COLOR), vc_io32(s, VC_REG_CLEAR_DEPTH), vc_io32(s, VC_REG_FOG_COLOR),
         vc_io16(s, VC_REG_FOG_OFFSET), s->geo.swap_param);
  printf("FOG_TABLE");
  for (int i = 0; i < 32; i++) printf(" %d", vc_io8(s, VC_REG_FOG_TABLE + i));
  printf("\n");
  static u32 vout[VC_H][VC_W];
  memcpy(vout, s->f3d.out, sizeof vout);
  GpuCtx *ctx = gpu_ctx_headless(GPU_API_ES, err, sizeof err);
  GpuCore *g = ctx ? gpu_create(gpu_ctx_getproc(ctx), GPU_API_ES, 1, 1, err, sizeof err) : NULL;
  if (!g) {
    fprintf(stderr, "gpu_pxdiff: %s\n", err);
    return 1;
  }
  static uint32_t gout[VC_W * VC_H], aux[VC_W * VC_H * 4];
  if (gpu_render_3d(g, s) != 0) return 1;
  gpu_read_3d(g, gout, aux);
  static uint32_t gpre[VC_W * VC_H];
  gpu_read_3d_raster(g, gpre);
  /* vcore's raster stage again, with the last-polygon record */
  vc_3d_clear(s);
  memset(vlast, 0xFF, sizeof vlast);
  s->plot = wrap_plot;
  for (u32 k = 0; k < s->geo.npoly; k++) {
    curk = (int)k;
    vc_raster_band(s, &s->geo.poly[s->geo.order[k]], 0, VC_H);
  }
  memcpy(vpre, s->f3d.color, sizeof vpre);
  if (px >= 0) {
    int i = py * VC_W + px;
    printf("pixel %d,%d: vcore out %08x pre %08x depth %06x flags %02x cov %d last %d | gpu out %08x depth %06x flags %02x cov %d last %d\n",
           px, py, vout[py][px], vpre[py][px], s->f3d.depth[py][px], s->f3d.flags[py][px], s->f3d.coverage[py][px],
           vlast[py][px], gout[i], aux[i * 4], (aux[i * 4 + 1] >> 16) & 0xFF, aux[i * 4 + 1] >> 24, (int)aux[i * 4 + 3]);
    for (u32 k = 0; k < s->geo.npoly; k++) {
      const VC_Poly *p = &s->geo.poly[s->geo.order[k]];
      At a;
      if (attrs(s, p, px, py, 0, &a)) {
        polyinfo(s, (int)k);
        attrs(s, p, px, py, 1, &a);
      }
    }
    return 0;
  }
  int dpre = 0, nd = 0, shown = 0, dnone = 0, ddepth = 0, dflags = 0, dcov = 0, dlast = 0, dpost = 0;
  for (int y = 0; y < VC_H; y++)
    for (int x = 0; x < VC_W; x++) {
      int i = y * VC_W + x;
      u32 v = vout[y][x];
      /* gout: RGB6 | a5 << 24 like f3d.out */
      u32 gg = gout[i];
      int va = (v >> 24) & 31, ga = (gg >> 24) & 31;
      int same = va == ga && (va == 0 || (v & 0x3FFFF) == (gg & 0x3FFFF));
      if (same) continue;
      nd++;
      int rd = s->f3d.depth[y][x] != aux[i * 4];
      int rf = (s->f3d.flags[y][x]) != ((aux[i * 4 + 1] >> 16) & 0xFF);
      int rc = s->f3d.coverage[y][x] != (aux[i * 4 + 1] >> 24);
      int rl = vlast[y][x] != (int)aux[i * 4 + 3];
      ddepth += rd;
      dflags += rf;
      dcov += rc;
      dlast += rl;
      int rp = (vpre[y][x] & 0x1FFFFFF) != (gpre[i] & 0x1FFFFFF) && ((vpre[y][x] >> 24) & 31) != 0;
      dpre += rp;
      if (!rd && !rf && !rc && !rl && !rp) dnone++;
      if (shown >= maxn) continue;
      shown++;
      printf("--- x %d y %d: vcore %08x (pre %08x) gpu %08x (pre %08x) | depth v %06x g %06x flags v %02x g %02x cov v %d g %d last v %d g %d\n",
             x, y, v, vpre[y][x], gg, gpre[i], s->f3d.depth[y][x], aux[i * 4], s->f3d.flags[y][x], (aux[i * 4 + 1] >> 16) & 0xFF,
             s->f3d.coverage[y][x], aux[i * 4 + 1] >> 24, vlast[y][x], (int)aux[i * 4 + 3]);
      int kk[2] = {vlast[y][x], (int)aux[i * 4 + 3]};
      for (int t = 0; t < 2; t++) {
        if (kk[t] < 0 || kk[t] >= (int)s->geo.npoly) continue;
        if (t == 1 && kk[1] == kk[0]) continue;
        printf("  %s: ", t ? "gpu last" : "vcore last");
        polyinfo(s, kk[t]);
        attrs(s, &s->geo.poly[s->geo.order[kk[t]]], x, y, 1, NULL);
      }
    }
  (void)dpost;
  printf("%s: %d px differ; raster-stage differences among them: depth %d flags %d cov %d last %d pre-colour %d; none (post only) %d\n",
         in, nd, ddepth, dflags, dcov, dlast, dpre, dnone);
  return 0;
}
