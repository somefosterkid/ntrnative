/* port/gpu tool: textured polygons of captures that look like sprites (colour-0 transparent or alpha formats) against the
 * billboard rule of gpu_poly_is_sprite (g02_spans.c): one line per polygon.
 *   gpu_sprites [-v] DIR|FILE.vcs...
 * Summary per set: textured polys, sprite-format polys, of those flagged by gpu_poly_is_sprite, not flagged. -v: a line each
 * (file idx fmt c0t alpha nv texW texH wmin wmax screen bbox s/t range flag). */
#define _DEFAULT_SOURCE 1 /* strdup under -std=c11 */
#include "g02_spans.h"
#include "gpu.h"
#include "vc_shared.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int cmp(const void *a, const void *b) { return strcmp(*(char *const *)a, *(char *const *)b); }

int main(int argc, char **argv) {
  int verbose = 0;
  char **files = NULL;
  int nf = 0, capf = 0;
  for (int i = 1; i < argc; i++) {
    const char *a = argv[i];
    if (!strcmp(a, "-v")) { verbose = 1; continue; }
    DIR *d = opendir(a);
    if (d) {
      struct dirent *de;
      while ((de = readdir(d))) {
        size_t n = strlen(de->d_name);
        if (n < 5 || strcmp(de->d_name + n - 4, ".vcs")) continue;
        if (nf == capf) files = realloc(files, (capf = capf ? capf * 2 : 256) * sizeof *files);
        files[nf] = malloc(strlen(a) + n + 2);
        sprintf(files[nf++], "%s/%s", a, de->d_name);
      }
      closedir(d);
    } else {
      if (nf == capf) files = realloc(files, (capf = capf ? capf * 2 : 256) * sizeof *files);
      files[nf] = strdup(a);
      nf++;
    }
  }
  if (!nf) { fprintf(stderr, "usage: gpu_sprites [-v] DIR|FILE.vcs...\n"); return 2; }
  qsort(files, (size_t)nf, sizeof *files, cmp);
  VCoreState *s = vcore_create();
  long tot = 0, tex = 0, sprfmt = 0, flagged = 0;
  for (int i = 0; i < nf; i++) {
    char err[512];
    if (vcore_load_vcs(s, files[i], err, sizeof err) != 0) { fprintf(stderr, "load %s: %s\n", files[i], err); continue; }
    vcore_render(s);
    const char *base = strrchr(files[i], '/') ? strrchr(files[i], '/') + 1 : files[i];
    for (u32 pi = 0; pi < s->geo.npoly; pi++) {
      const VC_Poly *p = &s->geo.poly[pi];
      tot++;
      int fmt = (p->texparam >> 26) & 7;
      if (fmt == 0) continue;
      tex++;
      int c0t = (p->texparam >> 29) & 1, alpha = (p->attr >> 16) & 31;
      int hard = fmt == 1 || fmt == 6 || fmt == 7 || fmt == 5 || c0t; /* any format that can have transparent texels */
      if (c0t && (fmt == 1 || fmt == 6 || fmt == 7 || fmt == 5)) c0t = 0; /* the bit means nothing there */
      int spr = gpu_poly_is_sprite(s, p);
      if (hard) sprfmt++;
      if (hard && spr) flagged++;
      if (!verbose) continue;
      int w0 = 1 << 30, w1 = 0, x0 = 9999, x1 = -9999, y0 = 9999, y1 = -9999, sm = 99999, sM = -99999, tm = 99999, tM = -99999;
      for (int v = 0; v < p->nv; v++) {
        const VC_Vertex *V = &s->geo.vtx[p->v[v]];
        if (V->clip[3] < w0) w0 = V->clip[3];
        if (V->clip[3] > w1) w1 = V->clip[3];
        if (V->sx < x0) x0 = V->sx;
        if (V->sx > x1) x1 = V->sx;
        if (V->sy < y0) y0 = V->sy;
        if (V->sy > y1) y1 = V->sy;
        if (V->tex[0] < sm) sm = V->tex[0];
        if (V->tex[0] > sM) sM = V->tex[0];
        if (V->tex[1] < tm) tm = V->tex[1];
        if (V->tex[1] > tM) tM = V->tex[1];
      }
      printf("%s %u fmt%d c0t%d a%d type%d nv%d tex%dx%d w %d %d scr %d,%d %d,%d st %d,%d %d,%d front%d %s\n", base, pi, fmt, c0t, alpha,
             p->type, p->nv, 8 << ((p->texparam >> 20) & 7), 8 << ((p->texparam >> 23) & 7), w0, w1, x0, y0, x1, y1, sm, tm, sM, tM,
             p->front, hard ? (spr ? "SPR" : "NOSPR") : "opaque");
    }
  }
  printf("files %d polys %ld textured %ld hardalpha-format %ld flagged-sprite %ld not-flagged %ld\n", nf, tot, tex, sprfmt, flagged,
         sprfmt - flagged);
  vcore_destroy(s);
  return 0;
}
