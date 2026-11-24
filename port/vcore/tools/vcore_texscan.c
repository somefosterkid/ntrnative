/* vcore_texscan: how a scene's big textured lists change their TEXTURE over consecutive captures (.vcs), offline. For each capture
 * and each distinct (texparam, palette) of the lists that pass the filter (by default the quad strips of 12+ vertices; --tp HEX picks
 * one texparam), the texture is decoded (vc_tex_sample over every texel) and compared with the same texture in the capture before:
 * the texels that differ, and the shift (dx, dy, wrapping) that maps the earlier image onto this one with the fewest differences.
 * Route 1's falling leaves (Black) are strips whose geometry never changes: the leaves move inside the texture (lv2, vcore_ipscan).
 *
 *   vcore_texscan [--tp HEX] [--minv N] [--shift R] [--minpx TEXELS] A.vcs B.vcs ...
 * One line per capture and texture: "fNNNNNN tp pltt id changed-texels best-dx best-dy differ-at-best"; id counts the distinct images
 * seen (a repeat of an earlier image keeps its id). A summary line per texture: captures, changes, the gaps between changes. */
#include "vc_shared.h"
#include "vcore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXT 64
typedef struct {
  u32 tp, pltt, w, h;
  u32 *img, *prev;
  int have;
  unsigned long long hash[4096];
  int nhash;
  int changes, caps, lastchange, gaps[64], done;
} T;

static unsigned long long fnv(const u32 *p, size_t n) {
  unsigned long long h = 1469598103934665603ull;
  for (size_t i = 0; i < n; i++) h = (h ^ p[i]) * 1099511628211ull;
  return h;
}

static int frame_of(const char *path) {
  const char *b = strrchr(path, '/');
  b = b ? b + 1 : path;
  while (*b && (*b < '0' || *b > '9')) b++;
  return atoi(b);
}

int main(int argc, char **argv) {
  u32 want = 0;
  int minv = 12, R = 16, nf = 0, minpx = 0;
  const char **files = (const char **)malloc(sizeof(char *) * (size_t)argc);
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--tp") && i + 1 < argc) want = (u32)strtoul(argv[++i], NULL, 16);
    else if (!strcmp(argv[i], "--minv") && i + 1 < argc) minv = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--shift") && i + 1 < argc) R = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--minpx") && i + 1 < argc) minpx = atoi(argv[++i]);
    else files[nf++] = argv[i];
  }
  if (nf < 1) {
    fprintf(stderr, "usage: vcore_texscan [--tp HEX] [--minv N] [--shift R] [--minpx TEXELS] A.vcs B.vcs ...\n");
    return 2;
  }
  T t[MAXT];
  int nt = 0;
  memset(t, 0, sizeof t);
  char err[256];
  for (int f = 0; f < nf; f++) {
    VCoreState *s = vcore_create();
    if (vcore_load_vcs(s, files[f], err, sizeof err)) {
      fprintf(stderr, "vcore_texscan: %s: %s\n", files[f], err);
      vcore_destroy(s);
      continue;
    }
    vc_ip_record(s);
    VC_IpRec rec;
    memset(&rec, 0, sizeof rec);
    vc_ip_take(s, &rec);
    vc_vram_map(s);
    int fr = frame_of(files[f]);
    for (u32 li = 0; li < rec.nl; li++) {
      const VC_IpList *l = &rec.l[li];
      u32 tp = l->texparam;
      if (((tp >> 26) & 7) == 0) continue;
      if (want ? (tp & 0x3FFFFFFF) != (want & 0x3FFFFFFF) : (int)l->n < minv) continue;
      if ((8 << ((tp >> 20) & 7)) * (8 << ((tp >> 23) & 7)) < minpx) continue;
      int k;
      for (k = 0; k < nt; k++)
        if (t[k].tp == tp && t[k].pltt == l->pltt) break;
      if (k == nt) {
        if (nt == MAXT) continue;
        t[k].tp = tp, t[k].pltt = l->pltt;
        t[k].w = 8u << ((tp >> 20) & 7), t[k].h = 8u << ((tp >> 23) & 7);
        t[k].img = (u32 *)calloc(t[k].w * t[k].h, 4);
        t[k].prev = (u32 *)calloc(t[k].w * t[k].h, 4);
        t[k].lastchange = -1;
        nt++;
      }
      T *x = &t[k];
      if (x->caps && x->done == fr) continue; /* already done for this capture (another list with the texture) */
      VC_Poly p;
      memset(&p, 0, sizeof p);
      p.texparam = tp & ~(3u << 16) & ~(3u << 18); /* no repeat/flip: the texel grid as stored */
      p.pltt = l->pltt;
      for (u32 y = 0; y < x->h; y++)
        for (u32 xx = 0; xx < x->w; xx++) x->img[y * x->w + xx] = vc_tex_sample(s, &p, (s32)xx * 16, (s32)y * 16);
      unsigned long long h = fnv(x->img, x->w * x->h);
      int id;
      for (id = 0; id < x->nhash; id++)
        if (x->hash[id] == h) break;
      if (id == x->nhash && x->nhash < 4096) x->hash[x->nhash++] = h;
      int changed = 0, bdx = 0, bdy = 0, best = -1;
      if (x->have) {
        for (u32 i = 0; i < x->w * x->h; i++) changed += x->img[i] != x->prev[i];
        if (changed)
          for (int dy = -R; dy <= R; dy++)
            for (int dx = -R; dx <= R; dx++) {
              int d = 0;
              for (u32 y = 0; y < x->h && (best < 0 || d < best); y++)
                for (u32 xx = 0; xx < x->w; xx++) {
                  u32 sy = (y - dy + x->h * 4) % x->h, sx = (xx - dx + x->w * 4) % x->w;
                  d += x->img[y * x->w + xx] != x->prev[sy * x->w + sx];
                }
              if (best < 0 || d < best) best = d, bdx = dx, bdy = dy;
            }
        else
          best = 0;
        if (changed) {
          if (x->changes && x->lastchange >= 0 && fr - x->lastchange < 64) x->gaps[fr - x->lastchange]++;
          x->changes++;
        }
      }
      printf("f%06d tp %08x pltt %04x list %u id %d changed %d shift %d %d differ %d\n", fr, tp, l->pltt, li, id, changed, bdx, bdy,
             best);
      if (changed || !x->have) x->lastchange = fr;
      x->have = 1;
      x->caps++;
      memcpy(x->prev, x->img, x->w * x->h * 4);
      x->done = fr;
    }
    vc_ip_free(&rec);
    vcore_destroy(s);
  }
  for (int k = 0; k < nt; k++) {
    printf("texture tp %08x pltt %04x %ux%u: captures %d changes %d distinct %d gaps(frames:count)", t[k].tp, t[k].pltt, t[k].w, t[k].h,
           t[k].caps, t[k].changes, t[k].nhash);
    for (int g = 1; g < 64; g++)
      if (t[k].gaps[g]) printf(" %d:%d", g, t[k].gaps[g]);
    printf("\n");
  }
  return 0;
}
