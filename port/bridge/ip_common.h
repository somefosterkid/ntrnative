/* port/bridge/ip_common.h: what the vcore and gpu bridges share for in-between pictures (docs/interp.md): the recorded
 * keyframes, the pairing, the counters and BWPORT_INTERP_REPORT. Included by each bridge (static: one copy each; only the
 * renderer in force runs). Needs vc_shared.h and <stdio.h>, <stdlib.h>, <string.h>, <time.h>. */
#ifndef BW_IP_COMMON_H
#define BW_IP_COMMON_H

/* a paired list whose texcoords change by this much (1/16 texel: 8 texels) between two keyframes steps a sprite's atlas cell; water and
 * conveyor scrolls move < 4 texels per keyframe (the histogram in BWPORT_INTERP_REPORT) */
#define IP_TEXHOLD 128
#define IP_TEXHOLD_PX 64.0 /* ... and only a list this small on screen (a sprite; the battle's big floor scrolls 8+ texels per keyframe) */
/* BWPORT_INTERP_TEXHOLD=N: the threshold in 1/16 texel (0 = the old behaviour: every texcoord blended; for before/after tests) */
static int IpTexHoldThr(void) {
  static int thr = -1;
  if (thr < 0) {
    const char *e = getenv("BWPORT_INTERP_TEXHOLD");
    thr = e != NULL && e[0] != 0 ? atoi(e) : IP_TEXHOLD;
    if (thr == 0) thr = 1 << 30;
  }
  return thr;
}
static struct {
  VC_IpRec key, old, spare;
  int *map;
  u8 *texhold; /* per key list: its texcoords step (vc_ip_texhold), not blended */
  size_t mapcap;
  int have, haveOld, valid;
  long long serial, keyNs, oldNs;
} sIp;
static struct {
  unsigned long long calls, keyframes, repeats, pairs, nopair, cut, mids, midsShown, midsLate, midsNone, nextMid, nextPlain;
  unsigned long long lists, listsPaired, verts, vertsPaired, gapClamped;
  unsigned long long recNs, midNs, plainNs, wholeNs, glNs;
  unsigned long long waits, waitNs, waitMax, waitCpuNs; /* gpu path: the GL thread waited for a mid's CPU half (InterpTexture), how often and how long */
  unsigned long long glCpuNs, glWallNs; /* gpu path: the GL thread's CPU time and the wall time since the first in-between (InterpTexture) */
  unsigned long long texHeld;
  unsigned long long texHist[12]; /* paired lists by their largest texcoord change between the keyframes: 0, <1, <2, <4, <8, <16, <32, <64, <128, more texels */
} sIc;
static unsigned long long sIpVerifyN, sIpVerifyBad; /* BWPORT_INTERP_VERIFY (vcore bridge) */
#define IC(x, n) __atomic_fetch_add(&sIc.x, (unsigned long long)(n), __ATOMIC_RELAXED)

static long long NowNs(void) {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return (long long)t.tv_sec * 1000000000ll + t.tv_nsec;
}

static int IpSame(const VC_IpRec *a, const VC_IpRec *b) {
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


/* BWPORT_INTERP_CHARLOG=FILE: per picture the in-between code makes (a blend at f, or the plain keyframe), one line per
 * small list (bounding box <= 64 x 64 px): where it is on the 256 x 192 screen and the texel rectangle it shows, after the same blend
 * the geometry engine applies (vc_ip_vertex). Lines: "P pic call keyframe idx f blend" then "L list n attr texparam pltt mapped x0 y0
 * x1 y1 s0 t0 s1 t1" (pixels, texels). The metric tool (C:\stress\jt81\metric.py) reads it. */
static FILE *sCl;
static int sClInit;
static unsigned long long sClPic;
static s32 IpLerp(s32 a, s32 b, s32 f) { return (s32)(a + (((s64)b - a) * f >> 12)); }
typedef struct IpBox {
  double x0, y0, x1, y1;
  int s0, t0, s1, t1;
  int ok;
} IpBox;
static void IpBoxAdd(IpBox *b, const VC_IpVtx *v) {
  if (v->clip[3] <= 0) {
    b->ok = 0;
    return;
  }
  double x = 128.0 + 128.0 * v->clip[0] / v->clip[3], y = 96.0 - 96.0 * v->clip[1] / v->clip[3];
  if (x < b->x0) b->x0 = x;
  if (x > b->x1) b->x1 = x;
  if (y < b->y0) b->y0 = y;
  if (y > b->y1) b->y1 = y;
  if (v->tex[0] < b->s0) b->s0 = v->tex[0];
  if (v->tex[0] > b->s1) b->s1 = v->tex[0];
  if (v->tex[1] < b->t0) b->t0 = v->tex[1];
  if (v->tex[1] > b->t1) b->t1 = v->tex[1];
}
static void IpBoxInit(IpBox *b) {
  b->x0 = b->y0 = 1e9;
  b->x1 = b->y1 = -1e9;
  b->s0 = b->t0 = 1 << 30;
  b->s1 = b->t1 = -(1 << 30);
  b->ok = 1;
}
static void IpBoxPrint(const IpBox *b) {
  fprintf(sCl, " %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f", b->x0, b->y0, b->x1, b->y1, b->s0 / 16.0, b->t0 / 16.0, b->s1 / 16.0, b->t1 / 16.0);
}
/* the list's vertex as the geometry engine's blend (vc_ip_vertex, APPLY) makes it */
static VC_IpVtx IpShown(const VC_IpVtx *a, const VC_IpVtx *d, s32 b, int texhold) {
  VC_IpVtx v = *a;
  for (int c = 0; c < 4; c++) v.clip[c] = IpLerp(a->clip[c], d->clip[c], b);
  if (texhold) {
    if (b >= 2048) v.tex[0] = d->tex[0], v.tex[1] = d->tex[1];
    return v;
  }
  for (int c = 0; c < 2; c++) {
    int dt = d->tex[c] - a->tex[c];
    if (dt > -2048 && dt < 2048) v.tex[c] = (s16)IpLerp(a->tex[c], d->tex[c], b);
  }
  return v;
}
static void IpMetric(int idx, double f, int blend) {
  if (!sClInit) {
    sClInit = 1;
    const char *e = getenv("BWPORT_INTERP_CHARLOG");
    if (e != NULL && e[0] != 0) sCl = fopen(e, "w");
  }
  if (sCl == NULL || !sIp.have) return;
  s32 b = blend ? (s32)((1.0 - f) * 4096.0 + 0.5) : 0;
  if (b < 0) b = 0;
  if (b > 4096) b = 4096;
  fprintf(sCl, "P %llu %llu %llu %d %.4f %d %d\n", sClPic++, sIc.calls, sIc.keyframes, idx, f, blend, sIp.valid);
  const VC_IpRec *k = &sIp.key;
  for (u32 i = 0; i < k->nl; i++) {
    const VC_IpList *l = &k->l[i];
    if (l->n == 0) continue;
    int m = sIp.valid && blend ? sIp.map[i] : -1;
    const VC_IpList *dl = m >= 0 ? &sIp.old.l[m] : NULL;
    int hold = m >= 0 && sIp.texhold != NULL && sIp.texhold[i];
    IpBox sh, kb, ob;
    IpBoxInit(&sh);
    IpBoxInit(&kb);
    IpBoxInit(&ob);
    for (u32 j = 0; j < l->n; j++) {
      const VC_IpVtx *kv = &k->v[l->first + j];
      IpBoxAdd(&kb, kv);
      if (dl != NULL && j < dl->n) {
        const VC_IpVtx *d = &sIp.old.v[dl->first + j];
        IpBoxAdd(&ob, d);
        VC_IpVtx v = IpShown(kv, d, b, hold);
        IpBoxAdd(&sh, &v);
      } else {
        IpBoxAdd(&sh, kv);
      }
    }
    if (!sh.ok || sh.x1 - sh.x0 > 64 || sh.y1 - sh.y0 > 64 || sh.x1 < -8 || sh.x0 > 264 || sh.y1 < -8 || sh.y0 > 200) continue;
    fprintf(sCl, "L %u %u %x %x %x %d", i, l->n, l->attr, l->texparam, l->pltt, m >= 0);
    IpBoxPrint(&sh);
    if (m >= 0 && ob.ok && kb.ok) {
      IpBoxPrint(&ob);
      IpBoxPrint(&kb);
    }
    fprintf(sCl, "\n");
  }
  if ((sClPic & 63) == 0) fflush(sCl);
}

/* BWPORT_INTERP_REPORT=FILE: the counters, rewritten every 120 calls and at exit */
static void IpReport(void) {
  static const char *path;
  static int init;
  if (!init) {
    init = 1;
    path = getenv("BWPORT_INTERP_REPORT");
  }
  if (path == NULL || path[0] == 0 || sIc.calls == 0) return; /* the other bridge never ran */
  FILE *fp = fopen(path, "w");
  if (fp == NULL) return;
  unsigned long long c = sIc.calls ? sIc.calls : 1;
  fprintf(fp, "calls %llu keyframes %llu repeats %llu pairs-valid %llu no-pair(first/3Doff) %llu cut %llu gap-clamped %llu\n", sIc.calls,
          sIc.keyframes, sIc.repeats, sIc.pairs, sIc.nopair, sIc.cut, sIc.gapClamped);
  fprintf(fp, "lists %llu paired %llu (%.1f%%) vertices %llu paired %llu (%.1f%%)\n", sIc.lists, sIc.listsPaired,
          sIc.lists ? 100.0 * sIc.listsPaired / sIc.lists : 0.0, sIc.verts, sIc.vertsPaired,
          sIc.verts ? 100.0 * sIc.vertsPaired / sIc.verts : 0.0);
  fprintf(fp, "mids-drawn %llu shown %llu late %llu none %llu ; next-slot pictures: in-between %llu plain %llu\n", sIc.mids,
          sIc.midsShown, sIc.midsLate, sIc.midsNone, sIc.nextMid, sIc.nextPlain);
  fprintf(fp, "ms per call: whole %.2f record %.2f ; per in-between render %.2f ; per plain render %.2f\n", sIc.wholeNs / 1e6 / c,
          sIc.recNs / 1e6 / c, sIc.mids + sIc.nextMid ? sIc.midNs / 1e6 / (sIc.mids + sIc.nextMid) : 0.0,
          sIc.nextPlain ? sIc.plainNs / 1e6 / sIc.nextPlain : 0.0);
  fprintf(fp, "lists with texcoords held (a cell step: >= 8 texels, <= 64 px big): %llu\n", sIc.texHeld);
  fprintf(fp, "paired lists by max texcoord change (texels) 0/<1/<2/<4/<8/<16/<32/<64/<128/more:");
  for (int i = 0; i < 10; i++) fprintf(fp, " %llu", sIc.texHist[i]);
  fprintf(fp, "\n");
  if (sIpVerifyN) fprintf(fp, "verify: %llu reused renders compared with the full render, %llu differ\n", sIpVerifyN, sIpVerifyBad);
  if (sIc.glNs) fprintf(fp, "gpu path: GL half per in-between %.2f ms (the CPU half is the render cost above)\n", sIc.glNs / 1e6 / (sIc.midsShown ? sIc.midsShown : 1));
  if (sIc.glWallNs)
    fprintf(fp, "gpu path: waits for a mid %llu, %.2f ms per second (CPU %.2f), longest %.2f ms ; GL thread CPU %.1f ms per second\n",
            sIc.waits, sIc.waitNs * 1e3 / sIc.glWallNs, sIc.waitCpuNs * 1e3 / sIc.glWallNs, sIc.waitMax / 1e6, sIc.glCpuNs * 1e3 / sIc.glWallNs);
  fclose(fp);
}

/* the first half of an in-between call: record the call's vertices, tell a keyframe from a repeat, pair a new keyframe with
 * the previous one, and return the gap (ns) between the two keyframes (clamped to 1..2 game ticks) */
static long long IpGather(VCoreState *s, long long serial, long long nominal, long long tick) {
  long long tA = NowNs();
  IC(calls, 1);
  if (serial != sIp.serial) { /* the present timeline started again: no keyframes known */
    sIp.have = sIp.haveOld = sIp.valid = 0;
    sIp.serial = serial;
  }
  s->p23.rec = sIp.spare;
  memset(&sIp.spare, 0, sizeof sIp.spare);
  vc_ip_record(s);
  VC_IpRec cur = s->p23.rec;
  memset(&s->p23.rec, 0, sizeof s->p23.rec);
  if (!sIp.have || !IpSame(&cur, &sIp.key)) { /* a new keyframe */
    VC_IpRec recycled = sIp.old;
    sIp.old = sIp.key;
    sIp.haveOld = sIp.have;
    sIp.key = cur;
    sIp.have = 1;
    sIp.spare = recycled;
    sIp.oldNs = sIp.keyNs;
    sIp.keyNs = nominal;
    sIp.valid = 0;
    IC(keyframes, 1);
    if (sIp.haveOld && sIp.key.swapped && sIp.old.swapped && sIp.key.nl > 0 && sIp.old.nl > 0) {
      if (sIp.mapcap < sIp.key.nl + 1) {
        sIp.mapcap = sIp.key.nl * 2 + 64;
        sIp.map = (int *)realloc(sIp.map, sIp.mapcap * sizeof(int));
        sIp.texhold = (u8 *)realloc(sIp.texhold, sIp.mapcap);
      }
      VC_IpStats st;
      vc_ip_pair(&sIp.key, &sIp.old, sIp.map, 48.0, &st);
      IC(lists, sIp.key.nl);
      IC(verts, sIp.key.nv);
      IC(vertsPaired, st.vt_paired);
      for (u32 i = 0; i < sIp.key.nl; i++) IC(listsPaired, sIp.map[i] >= 0);
      vc_ip_texhold(&sIp.key, &sIp.old, sIp.map, sIp.texhold, IpTexHoldThr(), IP_TEXHOLD_PX);
      for (u32 i = 0; i < sIp.key.nl; i++) {
        if (sIp.map[i] < 0) continue;
        IC(texHeld, sIp.texhold[i]);
        const VC_IpList *p = &sIp.key.l[i], *q = &sIp.old.l[sIp.map[i]];
        int mx = 0;
        for (u32 j = 0; j < p->n && j < q->n; j++)
          for (int c = 0; c < 2; c++) {
            int d = sIp.key.v[p->first + j].tex[c] - sIp.old.v[q->first + j].tex[c];
            if (d < 0) d = -d;
            if (d > mx) mx = d;
          }
        int bk = mx == 0 ? 0 : 1;
        for (int t = 16; t <= 2048 && mx >= t; t *= 2) bk++;
        IC(texHist[bk], 1);
      }
      /* a cut (fewer than half the vertices pair) shows the new frame at once, as without interpolation */
      if (st.vt_paired * 2 >= sIp.key.nv) {
        sIp.valid = 1;
        IC(pairs, 1);
      } else {
        IC(cut, 1);
      }
    } else {
      IC(nopair, 1);
    }
  } else {
    sIp.spare = cur;
    IC(repeats, 1);
  }
  long long tB = NowNs();
  IC(recNs, tB - tA);
  long long gap = sIp.keyNs - sIp.oldNs;
  if (gap < tick) gap = tick;
  if (gap > 2 * tick) { /* the scene was still: its first motion is spread over two game ticks */
    gap = 2 * tick;
    IC(gapClamped, 1);
  }
  return gap;
}

#endif
