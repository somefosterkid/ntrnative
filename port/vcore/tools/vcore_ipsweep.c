/* vcore_ipsweep: Smooth motion's in-between artifact scorer (docs/interp-sweep.md), offline over every-frame captures.
 *   vcore_ipsweep [--thr PX] [--events] [--repair 0|1] A.vcs B.vcs ...   (in frame order, every frame: tools/caps.py get PLAY A-B/1;
 *                                                                          --repair: vc_ip_repair_runs, default BWPORT_INTERP_REPAIR)
 * Replays the live path (port/bridge/ip_common.h IpGather): a capture whose 3D equals the last keyframe's is a repeat, a new
 * keyframe (KEY) is paired with the one before (OLD) by vc_ip_pair (48 px) and vc_ip_texhold (8 texels, 64 px), the gap is the
 * frame distance clamped to 1..2 ticks, and an in-between at f = k / (slots per gap) draws KEY's lists blended toward OLD's by
 * 1 - f. A 120 Hz display has 2 slots per tick (first in-between f = 1 / (2 gap)), 60 Hz one (gap 2 only: f = .5).
 * Scored per keyframe pair and list (screen px, 256 x 192; "ref" = the median motion of the paired lists around it = the camera):
 *   A_MISPAIR    a paired list whose partner is not the same-key list of OLD that moves with ref (err = the two motions' distance)
 *   A_OFFMOTION  a paired list moving > thr away from ref with no better same-key candidate (own motion: NPCs; or a loose mispair)
 *   B_TEXSTEP    a blended texcoord axis that changes >= 8 texels, not one uniform step (an atlas mix the hold misses; err texels)
 *   B_TEXSCROLL  a blended axis that changes >= 8 texels by one uniform step (a fast scroll: right to blend; info)
 *   B_HELD       an axis held by lv3's big-list rule (err = the step in texels); B_HELDRUN: the same list was held by the same step
 *                in the pair before too (a steady scroll in 8-texel steps would be stepped instead of glided: the suspect class)
 *   C_MISSED     a list of KEY with no partner although OLD has a same-key list where ref puts it (drawn at KEY's place: leads by |ref|)
 *   C_APPEAR     a list of KEY with no partner and no candidate (new: drawn at KEY's place for the whole interval)
 *   C_VANISH     a list of OLD with no partner (gone for the whole interval, one keyframe early; err = its size)
 *   E_SPEED      the camera's px per tick changes by > max(1, half) between consecutive pairs (judder), E_CUT a cut pair
 * Every event: vis120 / vis60 = err times (1 - first in-between f) at that rate (0 when the rate shows no in-between).
 * Output: with --events one line per event (`EV class frame oldframe list key n cx cy size err vis120 vis60 info`), then `STAT ...` and one
 * `SUM class count vis>=1 vis>=3 maxvis120 frame list` per class. */
#include "vc_shared.h"
#include "vcore.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT 48.0
#define TEXHOLD 128 /* ip_common.h IP_TEXHOLD */
#define TEXHOLD_PX 64.0
#define LOOSE_MAXPX 512.0 /* A_OFFMOTION is not scored for a loose-paired list (shape only) bigger than this */
static int hold_thr = TEXHOLD; /* BWPORT_INTERP_TEXHOLD like the live code (0 = no hold: before/after) */

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

typedef struct Box {
  double cx, cy, size;
  int front, vis; /* every vertex in front of the camera; the box meets the screen */
} Box;
static Box box_of(const VC_IpRec *r, const VC_IpList *l) {
  Box b = {0, 0, 0, 1, 0};
  double x0 = 1e30, x1 = -1e30, y0 = 1e30, y1 = -1e30;
  int nf = 0;
  for (u32 k = 0; k < l->n; k++) {
    const VC_IpVtx *v = &r->v[l->first + k];
    if (v->clip[3] <= 0) {
      b.front = 0;
      continue;
    }
    double x = cl(128.0 + 128.0 * v->clip[0] / v->clip[3], 1024), y = cl(96.0 - 96.0 * v->clip[1] / v->clip[3], 1024);
    x0 = x < x0 ? x : x0;
    x1 = x > x1 ? x : x1;
    y0 = y < y0 ? y : y0;
    y1 = y > y1 ? y : y1;
    nf++;
  }
  if (!nf) return b;
  b.cx = (x0 + x1) / 2;
  b.cy = (y0 + y1) / 2;
  b.size = x1 - x0 > y1 - y0 ? x1 - x0 : y1 - y0;
  b.vis = x1 >= 0 && x0 <= 256 && y1 >= 0 && y0 <= 192;
  return b;
}
/* the mean screen motion (KEY - OLD) of two lists' vertices in order, the vertices within half a screen of it in both (vc_ip_pair
 * clamps there); 0 = none */
static int motion(const VC_IpRec *a, const VC_IpList *p, const VC_IpRec *b, const VC_IpList *q, double *dx, double *dy) {
  double sx = 0, sy = 0;
  int nd = 0;
  for (u32 k = 0; k < p->n && k < q->n; k++) {
    const VC_IpVtx *u = &a->v[p->first + k], *w = &b->v[q->first + k];
    if (u->clip[3] <= 0 || w->clip[3] <= 0) continue;
    double ux = 128.0 * u->clip[0] / u->clip[3], uy = 96.0 * u->clip[1] / u->clip[3];
    double wx = 128.0 * w->clip[0] / w->clip[3], wy = 96.0 * w->clip[1] / w->clip[3];
    if (fabs(ux) > 256 || fabs(uy) > 192 || fabs(wx) > 256 || fabs(wy) > 192) continue; /* far off the screen: clamped there */
    sx += cl(128.0 * u->clip[0] / u->clip[3], 256) - cl(128.0 * w->clip[0] / w->clip[3], 256);
    sy += cl(96.0 * w->clip[1] / w->clip[3], 192) - cl(96.0 * u->clip[1] / u->clip[3], 192);
    nd++;
  }
  *dx = nd ? sx / nd : 0;
  *dy = nd ? sy / nd : 0;
  return nd;
}
static int cmpd(const void *a, const void *b) {
  double x = *(const double *)a, y = *(const double *)b;
  return x < y ? -1 : x > y;
}
static double median(double *v, int n) {
  if (n <= 0) return 0;
  qsort(v, (size_t)n, sizeof *v, cmpd);
  return n & 1 ? v[n / 2] : (v[n / 2 - 1] + v[n / 2]) / 2;
}

enum { A_MISPAIR, A_OFFMOTION, B_TEXSTEP, B_TEXSCROLL, B_HELD, B_HELDRUN, C_MISSED, C_APPEAR, C_VANISH, E_SPEED, E_CUT, NCLS };
static const char *cls_name[NCLS] = {"A_MISPAIR", "A_OFFMOTION", "B_TEXSTEP", "B_TEXSCROLL", "B_HELD",  "B_HELDRUN",
                                     "C_MISSED",  "C_APPEAR",    "C_VANISH",  "E_SPEED",     "E_CUT"};
static struct {
  unsigned long long n, v1, v3;
  double maxv;
  unsigned frame;
  int list;
} sum[NCLS];
static int events, pairlog; /* --pairs: one `PAIR` line per keyframe pair */
static double v120f, v60f;
static unsigned oldframe; /* 1 - the first in-between's f at 120 / 60 Hz for the current gap (0 = no in-between) */

static void ev(int c, unsigned frame, int list, const VC_IpList *l, const Box *b, double err, const char *info) {
  double v120 = err * v120f, v60 = err * v60f;
  if (c == B_TEXSTEP || c == B_TEXSCROLL || c == B_HELD || c == B_HELDRUN) v120 = v60 = err; /* texels: shown in every in-between */
  sum[c].n++;
  sum[c].v1 += v120 >= 1;
  sum[c].v3 += v120 >= 3;
  if (v120 > sum[c].maxv || sum[c].n == 1) {
    sum[c].maxv = v120;
    sum[c].frame = frame;
    sum[c].list = list;
  }
  if (events && (v120 >= 1 || c >= E_SPEED))
    printf("EV %s %u %u %d %08x %u %.1f %.1f %.1f %.2f %.2f %.2f %s\n", cls_name[c], frame, oldframe, list, l ? keyx(l) : 0, l ? l->n : 0,
           b ? b->cx : 0, b ? b->cy : 0, b ? b->size : 0, err, v120, v60, info ? info : "-");
}

static unsigned frame_of(const char *path) { /* .../f002780.vcs -> 2780 */
  const char *s = strrchr(path, '/');
  s = s ? s + 1 : path;
  while (*s && (*s < '0' || *s > '9')) s++;
  return (unsigned)strtoul(s, NULL, 10);
}

int main(int argc, char **argv) {
  const char *eh = getenv("BWPORT_INTERP_TEXHOLD");
  if (eh != NULL && eh[0] != 0) hold_thr = atoi(eh) ? atoi(eh) : 1 << 30;
  double thr = 6.0;
  const char **files = (const char **)malloc(sizeof(char *) * (size_t)argc);
  int nf = 0;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--thr") && i + 1 < argc) thr = atof(argv[++i]);
    else if (!strcmp(argv[i], "--events")) events = 1;
    else if (!strcmp(argv[i], "--pairs")) pairlog = 1;
    else if (!strcmp(argv[i], "--repair") && i + 1 < argc) vc_ip_repair_runs = atoi(argv[++i]);
    else files[nf++] = argv[i];
  }
  if (nf < 2) {
    fprintf(stderr, "usage: vcore_ipsweep [--thr PX] [--events] [--repair 0|1] A.vcs B.vcs ...\n");
    return 2;
  }
  VCoreState *s = vcore_create();
  char err[256];
  VC_IpRec key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  int have = 0;
  unsigned kf = 0, of = 0;
  int *pstep = NULL; /* the pair before's cell steps (vc_ip_texhold) */
  unsigned long long nmove = 0;
  double campath = 0;
  int unp_max = 0;
  unsigned long long nscene = 0, nkey = 0, nrep = 0, npair = 0, ncut = 0, lists = 0, lpaired = 0, gaps[4] = {0, 0, 0, 0};
  int *map = NULL, *mb = NULL, cap = 0, capb = 0;
  u8 *th = NULL;
  int *heldd = NULL, *heldp = NULL, capheld = 0, heldn = 0; /* per OLD list: the held step (axis * 65536 + delta) of the pair before */
  double pvx = 0, pvy = 0;
  int pmoving = 0;
  for (int f = 0; f < nf; f++) {
    if (vcore_load_vcs(s, files[f], err, sizeof err)) {
      fprintf(stderr, "vcore_ipsweep: %s: %s\n", files[f], err);
      continue;
    }
    VC_IpRec cur;
    memset(&cur, 0, sizeof cur);
    vc_ip_record(s);
    vc_ip_take(s, &cur);
    unsigned fr = frame_of(files[f]);
    if (have && same_rec(&cur, &key)) {
      nrep++;
      vc_ip_free(&cur);
      continue;
    }
    vc_ip_free(&old);
    old = key;
    of = kf;
    key = cur;
    kf = fr;
    nkey++;
    int hadold = have;
    have = 1;
    if (!hadold || !key.swapped || !old.swapped || key.nl == 0 || old.nl == 0) {
      pmoving = 0;
      heldn = 0;
      free(pstep);
      pstep = NULL;
      continue;
    }
    oldframe = of;
    int gap = (int)(kf - of);
    gaps[gap < 1 ? 0 : gap > 3 ? 3 : gap]++;
    if (gap < 1) gap = 1;
    if (gap > 2) gap = 2;
    v120f = 1.0 - 1.0 / (2.0 * gap);
    v60f = gap == 2 ? 0.5 : 0.0;
    if ((int)key.nl + 1 > cap) {
      cap = (int)key.nl * 2 + 64;
      map = (int *)realloc(map, sizeof(int) * (size_t)cap);
      th = (u8 *)realloc(th, (size_t)cap);
    }
    if ((int)old.nl + 1 > capb) {
      capb = (int)old.nl * 2 + 64;
      mb = (int *)realloc(mb, sizeof(int) * (size_t)capb);
    }
    VC_IpStats st;
    vc_ip_pair(&key, &old, map, LIMIT, &st);
    int unp = 0, scene = vc_ip_scene_cut(map, (int)key.nl, &unp);
    if (unp > unp_max) unp_max = unp;
    if (pairlog)
      printf("PAIR %u %u nl %d/%d nv %u/%u strict %d loose %d reorder %d removed %d added %d jumps %d vtpaired %u vtrem %u vtadd %u unpaired %d\n", kf,
             of, st.nl_a, st.nl_b, key.nv, old.nv, st.strict, st.loose, st.reordered, st.removed, st.added, st.jumps, st.vt_paired, st.vt_removed,
             st.vt_added, unp);
    int *stk = (int *)calloc(2 * (size_t)key.nl + 2, sizeof(int));
    vc_ip_texhold(&key, &old, map, th, hold_thr, TEXHOLD_PX, pstep, stk);
    free(pstep);
    pstep = stk;
    lists += key.nl;
    for (u32 i = 0; i < old.nl; i++) mb[i] = -1;
    for (u32 i = 0; i < key.nl; i++)
      if (map[i] >= 0) {
        lpaired++;
        mb[map[i]] = (int)i;
      }
    if (st.vt_paired * 2 < key.nv || scene) { /* a cut: the new frame at once, no in-between (scene: ip_common.h IpGather) */
      ncut++;
      nscene += scene && st.vt_paired * 2 >= key.nv;
      v120f = v60f = 0;
      ev(E_CUT, kf, -1, NULL, NULL, 0, scene ? "scene" : "cut");
      pmoving = 0;
      heldn = 0;
      free(pstep);
      pstep = NULL;
      continue;
    }
    npair++;
    Box *bk = (Box *)malloc(sizeof(Box) * key.nl), *bo = (Box *)malloc(sizeof(Box) * old.nl);
    double *mx = (double *)malloc(sizeof(double) * key.nl), *my = (double *)malloc(sizeof(double) * key.nl);
    int *good = (int *)calloc(key.nl, sizeof(int));
    for (u32 i = 0; i < key.nl; i++) bk[i] = box_of(&key, &key.l[i]);
    for (u32 j = 0; j < old.nl; j++) bo[j] = box_of(&old, &old.l[j]);
    double *tx = (double *)malloc(sizeof(double) * (key.nl + 1)), *ty = (double *)malloc(sizeof(double) * (key.nl + 1));
    int ng = 0;
    for (u32 i = 0; i < key.nl; i++) {
      mx[i] = my[i] = 0;
      if (map[i] < 0) continue;
      if (motion(&key, &key.l[i], &old, &old.l[map[i]], &mx[i], &my[i]) && bk[i].front && bo[map[i]].front && bk[i].vis &&
          key.l[i].n >= 3) {
        good[i] = 1;
        tx[ng] = mx[i];
        ty[ng] = my[i];
        ng++;
      }
    }
    double gx = median(tx, ng), gy = median(ty, ng);
    if (fabs(gx) + fabs(gy) > 0.5) nmove++;
    campath += fabs(gx) + fabs(gy);
    /* ref at a point: the median motion of the good pairs within 64 px (at least 3), else the global one */
    double *nx = tx, *ny = ty;
#define REF(px, py, self, rx, ry)                                                                         \
  do {                                                                                                    \
    int nn = 0;                                                                                           \
    for (u32 q = 0; q < key.nl; q++)                                                                      \
      if (good[q] && (int)q != (self) && fabs(bk[q].cx - (px)) < 64 && fabs(bk[q].cy - (py)) < 64) {      \
        nx[nn] = mx[q];                                                                                   \
        ny[nn] = my[q];                                                                                   \
        nn++;                                                                                             \
      }                                                                                                   \
    if (nn >= 3) {                                                                                        \
      rx = median(nx, nn);                                                                                \
      ry = median(ny, nn);                                                                                \
    } else {                                                                                              \
      rx = gx;                                                                                            \
      ry = gy;                                                                                            \
    }                                                                                                     \
  } while (0)
    char info[160];
    for (u32 i = 0; i < key.nl; i++) {
      const VC_IpList *p = &key.l[i];
      u32 kk = keyx(p);
      if (!bk[i].vis || !bk[i].front) continue;
      double rx, ry;
      REF(bk[i].cx, bk[i].cy, (int)i, rx, ry);
      if (map[i] >= 0) {
        const VC_IpList *q = &old.l[map[i]];
        double dev = hypot(mx[i] - rx, my[i] - ry);
        if (dev > thr && bo[map[i]].front) {
          /* the same-key list of OLD that moves most like ref */
          double best = 1e30, bx = 0, by = 0;
          int bj = -1;
          for (u32 j = 0; j < old.nl; j++) {
            if ((int)j == map[i] || keyx(&old.l[j]) != kk || !bo[j].front) continue;
            if (fabs(bo[j].cx + rx - bk[i].cx) > 64 || fabs(bo[j].cy + ry - bk[i].cy) > 64) continue;
            double dx, dy;
            if (!motion(&key, p, &old, &old.l[j], &dx, &dy)) continue;
            double d = hypot(dx - rx, dy - ry);
            if (d < best) {
              best = d;
              bj = (int)j;
              bx = dx;
              by = dy;
            }
          }
          if (bj >= 0 && best < dev - thr) {
            snprintf(info, sizeof info, "partner=%d best=%d dev=%.1f bestdev=%.1f ref=%.1f,%.1f nl=%u/%u", map[i], bj, dev, best, rx, ry,
                     key.nl, old.nl);
            ev(A_MISPAIR, kf, (int)i, p, &bk[i], hypot(mx[i] - bx, my[i] - by), info);
          } else if (keyx(q) == kk || bk[i].size <= LOOSE_MAXPX) { /* a loose-paired overlay is no motion of its own */
            snprintf(info, sizeof info, "partner=%d dev=%.1f ref=%.1f,%.1f loose=%d", map[i], dev, rx, ry, keyx(q) != kk);
            ev(A_OFFMOTION, kf, (int)i, p, &bk[i], dev, info);
          }
        }
        /* texcoords */
        if (p->n == q->n && p->n > 0) {
          for (int c = 0; c < 2; c++) {
            int d0 = key.v[p->first].tex[c] - old.v[q->first].tex[c], uni = 1, mxd = 0;
            for (u32 j = 0; j < p->n; j++) {
              int d = key.v[p->first + j].tex[c] - old.v[q->first + j].tex[c];
              uni &= d == d0;
              if (d < 0) d = -d;
              if (d > mxd) mxd = d;
            }
            int held = th[i] >> c & 1;
            if (held) {
              if (bk[i].size > TEXHOLD_PX) {
                int code = (c + 1) * 65536 + d0;
                int run = map[i] < heldn && heldp[map[i]] == code;
                snprintf(info, sizeof info, "axis=%c d=%d", "st"[c], d0 / 16);
                ev(run ? B_HELDRUN : B_HELD, kf, (int)i, p, &bk[i], fabs(d0 / 16.0), info);
              }
            } else if (mxd >= TEXHOLD && mxd < 2048) {
              snprintf(info, sizeof info, "axis=%c d0=%d max=%d", "st"[c], d0 / 16, mxd / 16);
              ev(uni ? B_TEXSCROLL : B_TEXSTEP, kf, (int)i, p, &bk[i], mxd / 16.0, info);
            }
          }
        }
      } else {
        /* unpaired: drawn at KEY's place in every in-between */
        double best = 1e30;
        int bj = -1;
        for (u32 j = 0; j < old.nl; j++) {
          if (keyx(&old.l[j]) != kk || !bo[j].front) continue;
          double d = hypot(bo[j].cx + rx - bk[i].cx, bo[j].cy + ry - bk[i].cy);
          if (d < best) {
            best = d;
            bj = (int)j;
          }
        }
        double tol = thr / 2 > 3 ? thr / 2 : 3;
        if (bj >= 0 && best <= tol) {
          snprintf(info, sizeof info, "cand=%d candpartner=%d off=%.1f ref=%.1f,%.1f jumps=%d behind=%d", bj, mb[bj], best, rx, ry, st.jumps,
                   st.behind);
          ev(C_MISSED, kf, (int)i, p, &bk[i], hypot(rx, ry), info);
        } else {
          snprintf(info, sizeof info, "ref=%.1f,%.1f nl=%u/%u", rx, ry, key.nl, old.nl);
          ev(C_APPEAR, kf, (int)i, p, &bk[i], hypot(rx, ry), info);
        }
      }
    }
    for (u32 j = 0; j < old.nl; j++) {
      if (mb[j] >= 0 || !bo[j].vis || !bo[j].front) continue;
      if (bo[j].cx < 0 || bo[j].cx > 256 || bo[j].cy < 0 || bo[j].cy > 192) continue; /* mostly off the screen */
      snprintf(info, sizeof info, "nl=%u/%u", key.nl, old.nl);
      double sv120 = v120f, sv60 = v60f; /* gone for the whole interval: its size, in every in-between */
      v120f = v120f > 0;
      v60f = v60f > 0;
      ev(C_VANISH, kf, (int)j, &old.l[j], &bo[j], bo[j].size, info);
      v120f = sv120;
      v60f = sv60;
    }
    /* timing: the camera's px per tick */
    double vx = gx / gap, vy = gy / gap, sp = hypot(vx, vy), psp = hypot(pvx, pvy);
    int moving = ng >= 5 && sp > 0.5;
    if (moving && pmoving) {
      double dv = hypot(vx - pvx, vy - pvy), lim = (sp > psp ? sp : psp) / 2;
      if (dv > (lim > 1 ? lim : 1)) {
        snprintf(info, sizeof info, "v=%.2f,%.2f prev=%.2f,%.2f gap=%d", vx, vy, pvx, pvy, (int)(kf - of));
        double sv = v120f;
        v120f = 1;
        ev(E_SPEED, kf, -1, NULL, NULL, dv, info);
        v120f = sv;
      }
    }
    pvx = vx;
    pvy = vy;
    pmoving = moving;
    /* this pair's held steps, by KEY list (the next pair's OLD) */
    if ((int)key.nl > capheld) {
      capheld = (int)key.nl * 2 + 64;
      heldd = (int *)realloc(heldd, sizeof(int) * (size_t)capheld);
      heldp = (int *)realloc(heldp, sizeof(int) * (size_t)capheld);
    }
    for (u32 i = 0; i < key.nl; i++) {
      heldd[i] = 0;
      if (map[i] < 0 || !th[i] || bk[i].size <= TEXHOLD_PX) continue;
      const VC_IpList *p = &key.l[i], *q = &old.l[map[i]];
      int c = th[i] & 1 ? 0 : 1;
      heldd[i] = (c + 1) * 65536 + (key.v[p->first].tex[c] - old.v[q->first].tex[c]);
    }
    int *t = heldd;
    heldd = heldp;
    heldp = t;
    heldn = (int)key.nl;
    free(bk);
    free(bo);
    free(mx);
    free(my);
    free(good);
    free(tx);
    free(ty);
  }
  printf("STAT keyframes %llu repeats %llu pairs %llu cuts %llu scene %llu unpaired_max %d lists %llu paired %.2f%% gaps1 %llu gaps2 %llu gaps3+ %llu moving %llu campath %.0f\n",
         nkey, nrep, npair, ncut, nscene, unp_max, lists, lists ? 100.0 * lpaired / lists : 0, gaps[1], gaps[2], gaps[3], nmove, campath);
  for (int c = 0; c < NCLS; c++)
    printf("SUM %s %llu %llu %llu %.2f %u %d\n", cls_name[c], sum[c].n, sum[c].v1, sum[c].v3, sum[c].maxv, sum[c].frame, sum[c].list);
  return 0;
}
