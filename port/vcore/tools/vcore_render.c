/* vcore_render: renders a .vcs frame with port/vcore (docs/vcore.md).
 *   vcore_render FILE.vcs OUT.png [--3d OUT3D.png] [--libntr OUTL.png] [--geo OUT.txt] [--bank X OUT.png]
 *                [--dump-vram OUT.bin] [--info] [--bench N] [--scale N] [--aa K] [--dot fine] [--line thin] [--edge fine]
 * --scale N: the 3D at N x (1..8, p21): OUT.png is then 256 N x 384 N.
 * OUT.png: both screens, 256x384 RGBA, top screen first (the compare tool's layout: tools/vcore_compare.py).
 * --3d: the 3D layer (s->f3d.out, alpha 0-31 widened to 0-255) as 256x192. --libntr: the file's own LSHT picture.
 * --geo: the geometry engine's output as text (docs/vcore.md, "The geometry dump"), for pieces 13-15 to compare
 * numbers with the oracle's. --bank X: VRAM bank X (A-I) after the frame (capture's destination) as a 256-wide
 * BGR555 picture. --dump-vram: banks A-I after the frame, 0xA4000 bytes in the .vcs order (the oracle's
 * --dump-vram: compare the two for capture). --info: print the file's INFO chunk. OWNED BY THE SKELETON (piece 20 may add options). */
#include "vc_shared.h"
#include "vcore.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* the geometry dump: one line per polygon in drawing order, then one per vertex */
static int write_geo(const VCoreState *s, const char *path) {
  FILE *f = fopen(path, "w");
  if (f == NULL) return -1;
  const VC_GeomOut *g = &s->geo;
  fprintf(f, "# vcore geometry: polygons %u vertices %u swapped %u swap_param %u overflow %u\n", g->npoly, g->nvtx,
          g->swapped, g->swap_param, g->overflow);
  for (u32 k = 0; k < g->npoly; k++) {
    const VC_Poly *p = &g->poly[g->order[k]];
    fprintf(f, "poly %u index %u nv %u attr %08x tex %08x pltt %04x front %u trans %u y %d %d\n", k, p->index, p->nv,
            p->attr, p->texparam, p->pltt, p->front, p->translucent, p->ymin, p->ymax);
    for (int i = 0; i < p->nv && i < VC_POLY_MAXV; i++) {
      const VC_Vertex *v = &g->vtx[p->v[i] % VC_MAX_VERTS];
      fprintf(f, "  v %d %d z %06x w %d rgb %u %u %u st %d %d clip %d %d %d %d\n", v->sx, v->sy, v->sz & 0xFFFFFF,
              v->clip[3], v->color[0], v->color[1], v->color[2], v->tex[0], v->tex[1], v->clip[0], v->clip[1],
              v->clip[2], v->clip[3]);
    }
  }
  return fclose(f) == 0 ? 0 : -1;
}

/* bank b as a 256-wide picture of BGR555 pixels (bit 15 ignored) */
static int write_bank(VCoreState *s, int b, const char *path, uint8_t *rgba) {
  const u8 *m = vc_bank(s, b);
  int h = (int)(vc_bank_size[b] / 512);
  for (int i = 0; i < 256 * h; i++) {
    u16 c = (u16)(m[i * 2] | m[i * 2 + 1] << 8);
    u32 r = c & 0x1F, g = (c >> 5) & 0x1F, bl = (c >> 10) & 0x1F;
    rgba[i * 4 + 0] = (uint8_t)(r << 3 | r >> 2);
    rgba[i * 4 + 1] = (uint8_t)(g << 3 | g >> 2);
    rgba[i * 4 + 2] = (uint8_t)(bl << 3 | bl >> 2);
    rgba[i * 4 + 3] = 255;
  }
  return vcore_write_png(path, rgba, 256, h);
}

static int usage(void) {
  fprintf(stderr, "usage: vcore_render FILE.vcs OUT.png [--3d OUT3D.png] [--libntr OUTL.png] [--geo OUT.txt] "
                  "[--bank X OUT.png] [--dump-vram OUT.bin] [--info] [--bench N]\n");
  return 2;
}

int main(int argc, char **argv) {
  const char *in = NULL, *out = NULL, *out3d = NULL, *outl = NULL, *outg = NULL, *outb = NULL, *outv = NULL;
  int info = 0, bank = -1, bench = 0, scale = 1, aa = 1, dot_fine = 0, line_thin = 0, edge_fine = 0;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--3d") && i + 1 < argc) out3d = argv[++i];
    else if (!strcmp(argv[i], "--libntr") && i + 1 < argc) outl = argv[++i];
    else if (!strcmp(argv[i], "--geo") && i + 1 < argc) outg = argv[++i];
    else if (!strcmp(argv[i], "--dump-vram") && i + 1 < argc) outv = argv[++i];
    else if (!strcmp(argv[i], "--bank") && i + 2 < argc && argv[i + 1][0] >= 'A' && argv[i + 1][0] <= 'I') {
      bank = argv[i + 1][0] - 'A';
      outb = argv[i + 2];
      i += 2;
    } else if (!strcmp(argv[i], "--bench") && i + 1 < argc) bench = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--seq")) vc_threads = 0;
    else if (!strcmp(argv[i], "--scale") && i + 1 < argc) scale = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--aa") && i + 1 < argc) aa = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--dot") && i + 1 < argc) dot_fine = !strcmp(argv[++i], "fine");
    else if (!strcmp(argv[i], "--line") && i + 1 < argc) line_thin = !strcmp(argv[++i], "thin");
    else if (!strcmp(argv[i], "--edge") && i + 1 < argc) edge_fine = !strcmp(argv[++i], "fine");
    else if (!strcmp(argv[i], "--info")) info = 1;
    else if (argv[i][0] == '-' && argv[i][1]) return usage();
    else if (in == NULL) in = argv[i];
    else if (out == NULL) out = argv[i];
    else return usage();
  }
  if (in == NULL || (out == NULL && !info && !bench && !outl && !outg && !outb && !outv)) return usage();
  VCoreState *s = vcore_create();
  if (s == NULL) return 1;
  char err[256];
  if (vcore_load_vcs(s, in, err, sizeof err) != 0) {
    fprintf(stderr, "vcore_render: %s: %s\n", in, err);
    return 1;
  }
  vcore_set_scale(s, scale);
  vcore_set_aa(s, aa); /* --aa K: the 3D at N K x, the picture the K x K box average, N x again (K is lowered when N K > 8) */
  vcore_set_options(s, dot_fine, line_thin, edge_fine);
  if (info) printf("%s", s->info ? s->info : "(no INFO)\n");
  if (bench > 0) {
    /* --bench N: render the frame N times, print the milliseconds per frame by stage (the rest = the work reset,
     * capture and the line-independent bookkeeping) */
    vcore_render(s); /* warm up */
    memset(vc_stage_ns, 0, sizeof vc_stage_ns);
    memset(vc_nwall_ns, 0, sizeof vc_nwall_ns);
    struct timespec a, b;
    clock_t c0 = clock();
    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < bench; i++) vcore_render(s);
    timespec_get(&b, TIME_UTC);
    double tot = ((double)(b.tv_sec - a.tv_sec) * 1e9 + (double)(b.tv_nsec - a.tv_nsec)) / bench / 1e6, sum = 0;
    static const char *const nm[VC_NSTAGE] = {"vram_map", "3d_geometry", "3d_clear", "3d_raster", "3d_post", "2d_A", "2d_B"};
    double cpu = (double)(clock() - c0) / CLOCKS_PER_SEC * 1e3 / bench; /* all threads' CPU time */
    printf("bench %s: %.3f ms/frame (cpu %.3f);", in, tot, cpu);
    for (int k = 0; k < VC_NSTAGE; k++) {
      printf(" %s %.3f", nm[k], vc_stage_ns[k] / bench / 1e6);
      sum += vc_stage_ns[k] / bench / 1e6;
    }
    printf(" other %.3f\n", tot - sum);
    if (scale > 1 || aa > 1) {
      static const char *const wn[VC_NWALL] = {"plan", "clear", "raster", "post", "picture"};
      printf("  N x wall ms of the 3D thread / picture:");
      for (int k = 0; k < VC_NWALL; k++) printf(" %s %.3f", wn[k], vc_nwall_ns[k] / bench / 1e6);
      printf("\n");
    }
  }
  vcore_render(s);
  int rc = 0;
  uint8_t *rgba = (uint8_t *)malloc(256 * 512 * 4);
  if (outg != NULL && write_geo(s, outg) != 0) rc = 1;
  if (outb != NULL && write_bank(s, bank, outb, rgba) != 0) rc = 1;
  if (outv != NULL) {
    FILE *f = fopen(outv, "wb");
    if (f == NULL || fwrite(s->vram, 1, sizeof s->vram, f) != sizeof s->vram) rc = 1;
    if (f != NULL && fclose(f) != 0) rc = 1;
  }
  if (out != NULL) {
    int pw = 0, ph = 0;
    const uint8_t *pic = vcore_picture(s, &pw, &ph);
    if (pic == NULL) {
      vcore_frame_rgba(s, rgba);
      pic = rgba;
      pw = 256;
      ph = 384;
    }
    if (vcore_write_png(out, pic, pw, ph) != 0) {
      fprintf(stderr, "vcore_render: can't write %s\n", out);
      rc = 1;
    }
  }
  int lw = 0, lh = 0;
  const u32 *layer = vcore_layer_n(s, &lw, &lh);
  if (out3d != NULL && layer != NULL) { /* the N x layer, as gpu_render --hi writes it */
    uint8_t *big = (uint8_t *)malloc((size_t)lw * lh * 4);
    for (int i = 0; i < lw * lh; i++) {
      u32 c = layer[i];
      big[i * 4 + 0] = (uint8_t)(VC_R6(c) << 2 | VC_R6(c) >> 4);
      big[i * 4 + 1] = (uint8_t)(VC_G6(c) << 2 | VC_G6(c) >> 4);
      big[i * 4 + 2] = (uint8_t)(VC_B6(c) << 2 | VC_B6(c) >> 4);
      big[i * 4 + 3] = (uint8_t)(VC_A5(c) * 255 / 31);
    }
    if (vcore_write_png(out3d, big, lw, lh) != 0) rc = 1;
    free(big);
  } else if (out3d != NULL) {
    for (int i = 0; i < VC_W * VC_H; i++) {
      u32 c = s->f3d.out[i / VC_W][i % VC_W];
      rgba[i * 4 + 0] = (uint8_t)(VC_R6(c) << 2 | VC_R6(c) >> 4);
      rgba[i * 4 + 1] = (uint8_t)(VC_G6(c) << 2 | VC_G6(c) >> 4);
      rgba[i * 4 + 2] = (uint8_t)(VC_B6(c) << 2 | VC_B6(c) >> 4);
      rgba[i * 4 + 3] = (uint8_t)(VC_A5(c) * 255 / 31);
    }
    if (vcore_write_png(out3d, rgba, 256, 192) != 0) rc = 1;
  }
  if (outl != NULL) {
    if (vcore_libntr_shot(s) == NULL) {
      fprintf(stderr, "vcore_render: %s has no LSHT chunk\n", in);
      rc = 1;
    } else if (vcore_write_png(outl, vcore_libntr_shot(s), 256, 384) != 0) {
      rc = 1;
    }
  }
  free(rgba);
  vcore_destroy(s);
  return rc;
}
