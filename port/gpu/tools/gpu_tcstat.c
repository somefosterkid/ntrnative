/* port/gpu G12 tool: runs captures, in order, through one texture cache (g12_texcache.c) with TCCHECK and logs the texture mix.
 *   gpu_tcstat [-v] [--nocheck] [--nomips] [--budget MB] DIR|FILE.vcs...
 * DIR = every *.vcs in it, sorted by name (a capture set: consecutive frames, or every Kth). -v prints each frame's TCLOG line.
 * The summary: textures a frame (mean / max) by layer side, the frame set's layer memory (MB, with mips x 4/3: the budget's
 * measure), the cache's arrays at the end, decodes, volatile and over-512 textures, textures sampled from a display capture
 * (their texel bytes on a 16 KB page of a bank that a capture of this set wrote to), TCCHECK's stale count, ms per frame.
 * OWNED BY G12. */
#include "g04_texture.h"
#include "g12_texcache.h"
#include "gpu.h"
#include "vc_shared.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int cmp(const void *a, const void *b) { return strcmp(*(char *const *)a, *(char *const *)b); }

static double now_ms(void) {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t.tv_sec * 1e3 + t.tv_nsec / 1e6;
}

/* the texture pages whose bytes come from a bank in banks (bit b: bank A-D a capture wrote to) */
static u32 captured_pages(const VCoreState *s, u32 banks) {
  const VC_Region *r = &s->rgn[VC_RGN_TEX];
  u32 m = 0;
  if (!banks || !r->npages) return 0;
  for (int p = 0; p < 32; p++) {
    const VC_Page *pg = &r->page[(u32)p % r->npages];
    for (int i = 0; i < pg->n; i++)
      for (int b = 0; b < 4; b++) {
        const u8 *lo = s->vram + vc_bank_offset[b];
        if ((banks >> b & 1) && pg->src[i] >= lo && pg->src[i] < lo + 0x20000) m |= 1u << p;
      }
  }
  return m;
}

int main(int argc, char **argv) {
  int verbose = 0;
  G12Options o = {0};
  o.check = 1;
  o.mips = 1;
  char **files = NULL;
  int nf = 0, capf = 0;
  for (int i = 1; i < argc; i++) {
    const char *a = argv[i];
    if (!strcmp(a, "-v")) { verbose = 1; continue; }
    if (!strcmp(a, "--nocheck")) { o.check = 0; continue; }
    if (!strcmp(a, "--nomips")) { o.mips = 0; continue; } /* no colour fill, memory counted x 1 */
    if (!strcmp(a, "--budget") && i + 1 < argc) { o.budget_mb = (u32)atoi(argv[++i]); continue; }
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
      files[nf] = malloc(strlen(a) + 1);
      strcpy(files[nf++], a);
    }
  }
  if (!nf) {
    fprintf(stderr, "usage: gpu_tcstat [-v] [--nocheck] [--nomips] [--budget MB] DIR|FILE.vcs...\n");
    return 2;
  }
  qsort(files, (size_t)nf, sizeof *files, cmp);
  G12Cache *c = g12_create(&o);
  GpuTexData *td = gpu_texdata_create();
  VCoreState *s = vcore_create();
  u32 banks = 0;
  long frames = 0, sum_tex = 0, max_tex = 0, sum_sz[G12_NBUCKET] = {0}, max_sz[G12_NBUCKET] = {0}, decodes = 0, max_dec = 0;
  long news = 0, changed = 0, kept = 0, stale = 0, checked = 0, maxvol = 0, sumvol = 0, maxbig = 0, maxcap = 0, capframes = 0;
  long maxpend = 0, noroom = 0, evicted = 0, capturing = 0;
  double sum_mb = 0, max_mb = 0, ms = 0, max_ms = 0;
  u64 texels = 0;
  char line[1024];
  G12Stats last;
  memset(&last, 0, sizeof last);
  for (int i = 0; i < nf; i++) {
    char err[512];
    if (vcore_load_vcs(s, files[i], err, sizeof err) != 0) {
      fprintf(stderr, "load %s: %s\n", files[i], err);
      continue;
    }
    u32 cnt = vc_io32(s, VC_REG_DISPCAPCNT);
    if (cnt >> 31) {
      banks |= 1u << ((cnt >> 16) & 3);
      capturing++;
    }
    vcore_render(s);
    gpu_texdata_build(td, s);
    const void *tex, *pal;
    int ntex, npal;
    unsigned ser;
    gpu_texdata_bytes(td, &tex, &ntex, &pal, &npal, &ser);
    G12Frame f;
    double t0 = now_ms();
    g12_frame(c, tex, pal, &s->geo, captured_pages(s, banks), &f);
    double dt = now_ms() - t0;
    const G12Stats *st = &f.st;
    if (verbose) {
      g12_log_line(&f, line, sizeof line);
      printf("%s %s TCCHECK %u checked %u stale\n", strrchr(files[i], '/') ? strrchr(files[i], '/') + 1 : files[i], line, st->checked,
             st->stale);
    }
    frames++;
    sum_tex += st->textures;
    if (st->textures > max_tex) max_tex = st->textures;
    for (int b = 0; b < G12_NBUCKET; b++) {
      sum_sz[b] += st->by_size[b];
      if (st->by_size[b] > max_sz[b]) max_sz[b] = st->by_size[b];
    }
    double mb = st->used_bytes / 1048576.0;
    sum_mb += mb;
    if (mb > max_mb) max_mb = mb;
    decodes += st->decodes;
    if (st->decodes > max_dec) max_dec = st->decodes;
    texels += st->decoded_texels;
    news += st->news;
    changed += st->changed;
    kept += st->kept_pal + st->kept_hash;
    stale += st->stale;
    checked += st->checked;
    sumvol += st->vol;
    if (st->vol > maxvol) maxvol = st->vol;
    if (st->big > maxbig) maxbig = st->big;
    if (st->captured > maxcap) maxcap = st->captured;
    capframes += st->captured > 0;
    if (st->pending > maxpend) maxpend = st->pending;
    noroom += st->noroom;
    evicted += st->evicted;
    if (i > 0) { /* the first frame decodes the whole scene */
      ms += dt;
      if (dt > max_ms) max_ms = dt;
    }
    last = *st;
  }
  if (frames) {
    const G12Stats *st = &last;
    {
      printf("frames %ld: textures/frame mean %.1f max %ld; by side (mean/max) 32: %.1f/%ld 64: %.1f/%ld 128: %.1f/%ld 256: %.1f/%ld "
             "512: %.1f/%ld; over 512: max %ld\n",
             frames, (double)sum_tex / frames, max_tex, (double)sum_sz[0] / frames, max_sz[0], (double)sum_sz[1] / frames, max_sz[1],
             (double)sum_sz[2] / frames, max_sz[2], (double)sum_sz[3] / frames, max_sz[3], (double)sum_sz[4] / frames, max_sz[4],
             maxbig);
      printf("frame set (layers x 4/3): mean %.2f MB max %.2f MB; cache at the end: %u entries, layers %u/%u/%u/%u/%u of %u/%u/%u/%u/%u, "
             "%.2f MB\n",
             sum_mb / frames, max_mb, st->entries, st->layers[0], st->layers[1], st->layers[2], st->layers[3], st->layers[4], st->cap[0],
             st->cap[1], st->cap[2], st->cap[3], st->cap[4], st->bytes / 1048576.0);
      printf("textures created %ld, changed %ld, kept (dirty, equal) %ld; decodes %ld (max %ld a frame, %.1f M texels); pending max %ld, "
             "no room %ld, evicted %ld\n",
             news, changed, kept, decodes, max_dec, texels / 1048576.0, maxpend, noroom, evicted);
      printf("volatile: max %ld a frame (%ld texture-frames); display capture: %ld frames capture (banks %x), sampled from a captured "
             "page: max %ld textures, %ld frames\n",
             maxvol, sumvol, capturing, banks, maxcap, capframes);
      printf("TCCHECK: %ld checked, %ld stale; g12_frame %.3f ms mean, %.3f max (after the first frame%s)\n", checked, stale,
             frames > 1 ? ms / (frames - 1) : 0, max_ms, o.check ? ", with TCCHECK's decodes" : "");
    }
  }
  g12_destroy(c);
  gpu_texdata_destroy(td);
  vcore_destroy(s);
  for (int i = 0; i < nf; i++) free(files[i]);
  free(files);
  return stale != 0;
}
