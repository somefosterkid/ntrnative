/* vcore_ipscan: Smooth motion's pairing (vc_ip_pair, docs/interp.md) over consecutive captures, offline, with the run re-pairing
 * (vc_ip_repair_runs) off and on.
 *   vcore_ipscan [--limit PX] [--log FILE] A.vcs B.vcs ...      (in frame order: BWPORT_VCAPTURE=DIR,F0-F1/1, tools/caps.py)
 * A capture whose 3D equals the last keyframe's is a repeat (the field draws its 3D every second frame); each new keyframe is paired
 * with the one before, as IpGather (port/bridge/ip_common.h) does, once per mode. Prints per mode: keyframe pairs, cuts, lists and
 * vertices paired, the motion of the paired vertices (mean, max), and for the RUN QUADS (4-vertex lists at most 64 px big with a
 * neighbour of the same key: particles) the displacement to the paired quad against the nearest quad of that key in the old
 * keyframe; "wrong" = the paired one is > 2 px and > 2x as far as the nearest. --log FILE: per keyframe pair the run quads
 * ("K pair file lists-new lists-old"; per list "L list n type attr texparam pltt cx cy size partner0 partner1"; per run quad
 * "Q list attr texparam pltt cx cy size partner0 disp0 partner1 disp1 nearest"; with IPSCAN_VERTS=1 the vertices of lists of <= 16
 * vertices, "V list k x y w s t"). */
#include "vc_shared.h"
#include "vcore.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static u32 mixk(u32 h, u32 x) { return (h ^ x) * 16777619u; }
static u32 keyx(const VC_IpList *l) { /* vc_ip_pair's key_of */
  u32 h = 2166136261u;
  h = mixk(h, l->attr & ~0x001F0000u);
  h = mixk(h, l->texparam);
  h = mixk(h, l->pltt);
  h = mixk(h, l->viewport);
  h = mixk(h, l->type);
  return mixk(h, l->n);
}
static double cl(double x, double m) { return x < -m ? -m : x > m ? m : x; }
static double disp(const VC_IpRec *a, const VC_IpList *p, const VC_IpRec *b, const VC_IpList *q) {
  double s = 0;
  int nd = 0;
  for (u32 k = 0; k < p->n && k < q->n; k++) {
    const VC_IpVtx *u = &a->v[p->first + k], *w = &b->v[q->first + k];
    if (u->clip[3] <= 0 || w->clip[3] <= 0) continue;
    double ax = cl(128.0 * u->clip[0] / u->clip[3], 256), ay = cl(96.0 * u->clip[1] / u->clip[3], 192);
    double bx = cl(128.0 * w->clip[0] / w->clip[3], 256), by = cl(96.0 * w->clip[1] / w->clip[3], 192);
    s += sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
    nd++;
  }
  return nd ? s / nd : -1;
}
/* centre (screen px, 0..256 x 0..192) and size; 0 = a vertex behind the camera */
static int box(const VC_IpRec *r, const VC_IpList *l, double *cx, double *cy, double *sz) {
  double x0 = 1e30, x1 = -1e30, y0 = 1e30, y1 = -1e30;
  for (u32 k = 0; k < l->n; k++) {
    const VC_IpVtx *v = &r->v[l->first + k];
    if (v->clip[3] <= 0) return 0;
    double x = 128.0 + 128.0 * v->clip[0] / v->clip[3], y = 96.0 - 96.0 * v->clip[1] / v->clip[3];
    if (x < x0) x0 = x;
    if (x > x1) x1 = x;
    if (y < y0) y0 = y;
    if (y > y1) y1 = y;
  }
  *cx = (x0 + x1) / 2;
  *cy = (y0 + y1) / 2;
  *sz = x1 - x0 > y1 - y0 ? x1 - x0 : y1 - y0;
  return l->n > 0;
}
static int same_rec(const VC_IpRec *a, const VC_IpRec *b) { /* IpSame */
  if (a->nl != b->nl || a->nv != b->nv || a->swapped != b->swapped) return 0;
  for (u32 i = 0; i < a->nl; i++) {
    const VC_IpList *p = &a->l[i], *q = &b->l[i];
    if (p->n != q->n || p->attr != q->attr || p->texparam != q->texparam || p->pltt != q->pltt || p->viewport != q->viewport ||
        p->type != q->type)
      return 0;
  }
  for (u32 i = 0; i < a->nv; i++) {
    const VC_IpVtx *p = &a->v[i], *q = &b->v[i];
    if (memcmp(p->clip, q->clip, sizeof p->clip) || p->tex[0] != q->tex[0] || p->tex[1] != q->tex[1] || memcmp(p->color, q->color, 3))
      return 0;
  }
  return 1;
}

typedef struct M {
  unsigned long long pairs, cuts, lists, lpaired, verts, vpaired, repaired;
  double dsum, dmax;
  unsigned long long dn;
  unsigned long long q, qpaired, qwrong;
  double qsum, qmax;
} M;

static unsigned long long cellsteps, cellblend, cellblend_old;
int main(int argc, char **argv) {
  double limit = 48.0;
  const char *logp = NULL;
  int nf = 0;
  const char **files = (const char **)malloc(sizeof(char *) * (size_t)argc);
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--limit") && i + 1 < argc) limit = atof(argv[++i]);
    else if (!strcmp(argv[i], "--log") && i + 1 < argc) logp = argv[++i];
    else files[nf++] = argv[i];
  }
  if (nf < 2) {
    fprintf(stderr, "usage: vcore_ipscan [--limit PX] [--log FILE] A.vcs B.vcs ...\n");
    return 2;
  }
  FILE *lg = logp ? fopen(logp, "w") : NULL;
  VC_IpRec key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  int have = 0, keyframes = 0, repeats = 0;
  M m[2];
  memset(m, 0, sizeof m);
  int *map[2] = {NULL, NULL};
  u32 *ka = NULL, *kb = NULL;
  size_t cap = 0;
  char err[256];
  for (int f = 0; f < nf; f++) {
    VCoreState *s = vcore_create();
    if (vcore_load_vcs(s, files[f], err, sizeof err)) {
      fprintf(stderr, "vcore_ipscan: %s: %s\n", files[f], err);
      vcore_destroy(s);
      continue;
    }
    VC_IpRec cur;
    memset(&cur, 0, sizeof cur);
    vc_ip_record(s);
    vc_ip_take(s, &cur);
    vcore_destroy(s);
    if (have && same_rec(&cur, &key)) {
      vc_ip_free(&cur);
      repeats++;
      continue;
    }
    vc_ip_free(&old);
    old = key;
    key = cur;
    int haveOld = have;
    have = 1;
    keyframes++;
    if (!haveOld || !key.swapped || !old.swapped || key.nl == 0 || old.nl == 0) continue;
    size_t need = (key.nl > old.nl ? key.nl : old.nl) + 1;
    if (need > cap) {
      cap = need * 2;
      for (int k = 0; k < 2; k++) map[k] = (int *)realloc(map[k], cap * sizeof(int));
      ka = (u32 *)realloc(ka, cap * sizeof(u32));
      kb = (u32 *)realloc(kb, cap * sizeof(u32));
    }
    VC_IpStats st[2];
    for (int k = 0; k < 2; k++) {
      vc_ip_repair_runs = k;
      vc_ip_pair(&key, &old, map[k], limit, &st[k]);
      M *x = &m[k];
      if (st[k].vt_paired * 2 >= key.nv) x->pairs++;
      else x->cuts++;
      x->lists += key.nl;
      for (u32 i = 0; i < key.nl; i++) x->lpaired += map[k][i] >= 0;
      x->verts += key.nv;
      x->vpaired += st[k].vt_paired;
      x->repaired += (unsigned long long)st[k].repaired;
      x->dsum += st[k].disp_mean * st[k].vt_paired;
      x->dn += st[k].vt_paired;
      if (st[k].disp_max > x->dmax) x->dmax = st[k].disp_max;
    }
    { /* cell steps: a paired list's axis whose texcoords jump by 8..127 texels (the range the blend would sweep through) */
      static u8 *th;
      static size_t thn;
      if (thn < key.nl + 1) thn = key.nl * 2 + 64, th = (u8 *)realloc(th, thn);
      vc_ip_texhold(&key, &old, map[0], th, 128, 64.0, NULL, NULL);
      for (u32 i = 0; i < key.nl; i++) {
        if (map[0][i] < 0) continue;
        const VC_IpList *p = &key.l[i], *q = &old.l[map[0][i]];
        for (int c = 0; c < 2; c++) {
          int step = 0;
          for (u32 j = 0; j < p->n && j < q->n; j++) {
            int d = abs(key.v[p->first + j].tex[c] - old.v[q->first + j].tex[c]);
            if (d >= 128 && d < 2048) step = 1;
          }
          if (!step) continue;
          double sz = -1, cx, cy;
          box(&key, p, &cx, &cy, &sz);
          cellsteps++;
          if (!(th[i] >> c & 1)) cellblend++;
          if (sz > 64.0 || !(th[i] >> c & 1)) cellblend_old++; /* the rule before: a list over 64 px never held */
        }
      }
    }
    for (u32 i = 0; i < key.nl; i++) ka[i] = keyx(&key.l[i]);
    for (u32 j = 0; j < old.nl; j++) kb[j] = keyx(&old.l[j]);
    if (lg) fprintf(lg, "K %d %s %u %u\n", keyframes - 1, files[f], key.nl, old.nl);
    for (u32 i = 0; lg && i < key.nl; i++) {
      const VC_IpList *p = &key.l[i];
      double cx = 0, cy = 0, sz = -1;
      box(&key, p, &cx, &cy, &sz);
      fprintf(lg, "L %u %u %u %x %x %x %.1f %.1f %.1f %d %d\n", i, p->n, p->type, p->attr, p->texparam, p->pltt, cx, cy, sz, map[0][i], map[1][i]);
      if (map[0][i] >= 0 && old.l[map[0][i]].n == p->n) { /* T: a paired list's texcoord change (texels): the largest, and vertex 0's */
        const VC_IpList *q = &old.l[map[0][i]];
        int mx = 0;
        for (u32 k = 0; k < p->n; k++)
          for (int c = 0; c < 2; c++) {
            int d = abs(key.v[p->first + k].tex[c] - old.v[q->first + k].tex[c]);
            if (d > mx) mx = d;
          }
        if (mx) fprintf(lg, "T %u %u %x %.1f %.2f %.2f %.2f\n", i, p->n, p->texparam, sz, mx / 16.0,
                        (key.v[p->first].tex[0] - old.v[q->first].tex[0]) / 16.0, (key.v[p->first].tex[1] - old.v[q->first].tex[1]) / 16.0);
      }
      for (u32 k = 0; getenv("IPSCAN_VERTS") && p->n <= 16 && k < p->n; k++) {
        const VC_IpVtx *v = &key.v[p->first + k];
        double w = v->clip[3] > 0 ? v->clip[3] : 1;
        fprintf(lg, "V %u %u %.1f %.1f %d %.1f %.1f\n", i, k, 128.0 + 128.0 * v->clip[0] / w, 96.0 - 96.0 * v->clip[1] / w, v->clip[3], v->tex[0] / 16.0, v->tex[1] / 16.0);
      }
    }
    for (u32 i = 0; i < key.nl; i++) {
      const VC_IpList *p = &key.l[i];
      if (p->n != 4) continue;
      if (!((i > 0 && ka[i - 1] == ka[i]) || (i + 1 < key.nl && ka[i + 1] == ka[i]))) continue;
      double cx, cy, sz;
      if (!box(&key, p, &cx, &cy, &sz) || sz > 64 || cx < -8 || cx > 264 || cy < -8 || cy > 200) continue;
      double near = -1;
      for (u32 j = 0; j < old.nl; j++) {
        if (kb[j] != ka[i]) continue;
        double d = disp(&key, p, &old, &old.l[j]);
        if (d >= 0 && (near < 0 || d < near)) near = d;
      }
      double dd[2];
      for (int k = 0; k < 2; k++) {
        M *x = &m[k];
        x->q++;
        dd[k] = -1;
        int j = map[k][i];
        if (j < 0) continue;
        dd[k] = disp(&key, p, &old, &old.l[j]);
        x->qpaired++;
        x->qsum += dd[k];
        if (dd[k] > x->qmax) x->qmax = dd[k];
        if (near >= 0 && dd[k] > near + 2 && dd[k] > 2 * near) x->qwrong++;
      }
      if (lg)
        fprintf(lg, "Q %u %x %x %x %.1f %.1f %.1f %d %.2f %d %.2f %.2f\n", i, p->attr, p->texparam, p->pltt, cx, cy, sz, map[0][i], dd[0],
                map[1][i], dd[1], near);
    }
  }
  printf("captures %d keyframes %d repeats %d\n", nf, keyframes, repeats);
  for (int k = 0; k < 2; k++) {
    M *x = &m[k];
    if (k == 0)
      printf("cell steps (an axis jumping 8..127 texels between keyframes) %llu: left blended (a strip of two cells) %llu, by the rule "
             "before (lists over 64 px never held) %llu\n", cellsteps, cellblend, cellblend_old);
    printf("repair %s: pairs %llu cuts %llu repaired-lists %llu | lists %llu paired %.2f%% | vertices %llu paired %.2f%% | motion mean %.3f max %.1f px"
           " | run quads %llu paired %llu wrong %llu (%.1f%%) disp mean %.2f max %.1f px\n",
           k ? "on " : "off", x->pairs, x->cuts, x->repaired, x->lists, x->lists ? 100.0 * x->lpaired / x->lists : 0.0, x->verts,
           x->verts ? 100.0 * x->vpaired / x->verts : 0.0, x->dn ? x->dsum / x->dn : 0.0, x->dmax, x->q, x->qpaired, x->qwrong,
           x->qpaired ? 100.0 * x->qwrong / x->qpaired : 0.0, x->qpaired ? x->qsum / x->qpaired : 0.0, x->qmax);
  }
  if (lg) fclose(lg);
  return 0;
}
