/* vcore_ipsweep: Smooth motion's in-between artifact scorer (docs/interp-sweep.md), offline over every-frame captures.
 *   vcore_ipsweep [--thr PX] [--events] A.vcs B.vcs ...   (in frame order, every frame: tools/caps.py get PLAY A-B/1)
 * Replays the live path (port/bridge/ip_common.h IpGather): a capture whose 3D equals the last keyframe's is a repeat, a new
 * keyframe (KEY) is paired with the one before (OLD) by vc_ip_pair and vc_ip_texhold (8 texels, 64 px), the gap is the
 * frame distance clamped to 1..2 ticks, and an in-between at f = k / (slots per gap) draws KEY's lists blended toward OLD's by
 * 1 - f. A 120 Hz display has 2 slots per tick (first in-between f = 1 / (2 gap)), 60 Hz one (gap 2 only: f = .5).
 * Scored per keyframe pair and list (screen px, 256 x 192; "ref" = without --oracle the median motion of the live pairs around it, as
 * before M5; with --oracle the motion of its tag pair for a list the tags pair (the game's truth), else the median motion of the
 * oracle's pairs around it = the world (m5fix: identity's pairs are not measured against themselves, the oracle's identity being the
 * live pairing's rules)):
 *   A_MISPAIR    a paired list whose partner is not the same-key list of OLD that moves with ref (err = the two motions' distance;
 *                with --oracle only lists the tags pair: a live partner other than the tag's, which another same-key list fits better)
 *   A_OFFMOTION  a paired list moving > thr away from ref with no better same-key candidate (own motion: NPCs; or a loose mispair;
 *                with --oracle only lists the tags pair)
 *   AL_MISPAIR / AL_OFFMOTION   (--oracle) the same for every other list, measured against the old ref (the live pairs around it): the
 *                column the A_ classes are without --oracle, for the lists that have no truth of their own
 *   B_TEXSTEP    a blended texcoord axis that changes >= 8 texels, not one uniform step (an atlas mix the hold misses; err texels)
 *   B_TEXSCROLL  a blended axis that changes >= 8 texels by one uniform step (a fast scroll: right to blend; info)
 *   B_HELD       an axis held by lv3's big-list rule (err = the step in texels); B_HELDRUN: the same list was held by the same step
 *                in the pair before too (a steady scroll in 8-texel steps would be stepped instead of glided: the suspect class)
 *   B_WRAP       a blended axis of a texture that repeats alike in both keyframes whose step (vc_ip_tex_step's, the blend's) is over half
 *                its period: the in-betweens slide the texture back across itself (a scroll that wrapped: HG/SS's sea every 12 s; err =
 *                the slide beyond the shortest step, texels). B_TEXSTEP / B_TEXSCROLL take the blend's steps too
 *   C_MISSED     a list of KEY with no partner although OLD has a same-key list where ref puts it (drawn at KEY's place: leads by |ref|)
 *   C_APPEAR     a list of KEY with no partner and no candidate (new: drawn at KEY's place for the whole interval)
 *   C_VANISH     a list of OLD with no partner (gone for the whole interval, one keyframe early; err = its size)
 *   C_NEW        a list of KEY marked new (VC_IP_NEW: vc_ip_windows): not drawn before its keyframe, as without Smooth motion (info: err = what
 *                drawing it at KEY's place would have been off, |ref|; vis 0)
 *   C_BLINK      a C_NEW list whose key vanished (C_VANISH) where ref puts it in the pair before: a list the game draws every other
 *                keyframe would be drawn only at its keyframes (err = its size, in every in-between)
 *   C_KEPT       a list of OLD with no partner drawn until KEY (vc_ip_windows' gone), as without Smooth motion (info: err = the motion it is
 *                drawn with, px; vis 0); C_VANISH is then the rest
 *   C_EDGE       a list with no partner that vc_ip_windows moves by the projection: one of KEY entering the screen (blended toward
 *                its place in OLD had it stood still in the world) or one of OLD leaving it (kept, moved so to KEY); err = its motion's
 *                distance from ref (where C_APPEAR / C_VANISH showed |ref| or its size)
 *   E_SPEED      the camera's px per tick changes by > max(1, half) between consecutive pairs (judder), E_CUT a cut pair
 * Every event: vis120 / vis60 = err times (1 - first in-between f) at that rate (0 when the rate shows no in-between).
 * Output: with --events one line per event (`EV class frame oldframe list key n cx cy size err vis120 vis60 info`), then `STAT ...` and one
 * `SUM class count vis>=1 vis>=3 maxvis120 frame list` per class.
 * --oracle adds the list identity of docs/interp-sweep.md (the comment before orc_pair) and its classes: O_DISAGREE, O_MISSED, V_NONE,
 * V_TWICE, V_STALE, P_NONE, P_TWICE, P_CROSS (the table there), an `ORACLE` line, `OA` lines (each A_ event judged right / wrong /
 * unknown by it) and with --pairs `ORC` lines; --orcdump FRAME prints the oracle's view of the pair whose KEY is FRAME. Without it the
 * output is the same as before. */
#include "vc_shared.h"
#include "vcore.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIMIT 48.0
/* --time: the live path's time a keyframe pair (vc_ip_pair_how, vc_ip_windows, vc_ip_texhold, vc_ip_state, vc_ip_cut_id: what IpGather's
 * worker runs past recording and the texture hashes), its median, p95 and max in a TIME line */
static int timing;
static double *tus;
static int ntus, captus;
static double now_us(void) {
  struct timespec t;
  timespec_get(&t, TIME_UTC);
  return (double)t.tv_sec * 1e6 + (double)t.tv_nsec / 1e3;
}
#define TEXHOLD 128 /* ip_common.h IP_TEXHOLD */
#define TEXHOLD_PX 64.0
/* an axis' period (1/16 texel) when the texture repeats on it alike in both keyframes' state (TEXIMAGE_PARAM's repeat, mirror and size
 * bits; mirrored = twice the size), else 0 */
static int tex_period(u32 a, u32 b, int c) {
  const u32 x = a ^ b;
  if (!(a >> 26 & 7) || !(a >> (16 + c) & 1) || (x >> (16 + c) & 1) || (x >> (18 + c) & 1) || (x >> (20 + 3 * c) & 7)) return 0;
  return (8 << (a >> (20 + 3 * c) & 7)) * 16 * (1 + (int)(a >> (18 + c) & 1));
}
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
  double hw, hh;  /* half its width and height */
} Box;
static Box box_of(const VC_IpRec *r, const VC_IpList *l) {
  Box b = {0, 0, 0, 1, 0, 0, 0};
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
  b.hw = (x1 - x0) / 2, b.hh = (y1 - y0) / 2;
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
/* motion with every vertex in front of the camera in both, clamped half a screen beyond the edges (vc_ip_pair's tag jump measure):
 * a list drawn far off the screen in one frame moves to the band's edge, where motion leaves it out */
static int motion_c(const VC_IpRec *a, const VC_IpList *p, const VC_IpRec *b, const VC_IpList *q, double *dx, double *dy) {
  double sx = 0, sy = 0;
  int nd = 0;
  for (u32 k = 0; k < p->n && k < q->n; k++) {
    const VC_IpVtx *u = &a->v[p->first + k], *w = &b->v[q->first + k];
    if (u->clip[3] <= 0 || w->clip[3] <= 0) continue;
    sx += cl(128.0 * u->clip[0] / u->clip[3], 256) - cl(128.0 * w->clip[0] / w->clip[3], 256);
    sy += cl(96.0 * w->clip[1] / w->clip[3], 192) - cl(96.0 * u->clip[1] / u->clip[3], 192);
    nd++;
  }
  *dx = nd ? sx / nd : 0;
  *dy = nd ? sy / nd : 0;
  return nd;
}
/* motion's measure for a list moved by a clip-space matrix (vc_ip_windows): KEY - OLD on the screen (y down), r's list l being KEY's
 * moved to OLD (old = 0: an entering list) or OLD's moved to KEY (old = 1: a kept one) */
static int motion_m(const VC_IpRec *r, const VC_IpList *l, const double *m, int old, double *dx, double *dy) {
  double sx = 0, sy = 0;
  int nd = 0;
  for (u32 k = 0; k < l->n; k++) {
    const s32 *c = r->v[l->first + k].clip;
    double o[4];
    for (int j = 0; j < 4; j++) o[j] = (double)c[0] * m[j] + (double)c[1] * m[4 + j] + (double)c[2] * m[8 + j] + (double)c[3] * m[12 + j];
    if (c[3] <= 0 || o[3] <= 0) continue;
    double ux = 128.0 * c[0] / c[3], uy = 96.0 * c[1] / c[3], wx = 128.0 * o[0] / o[3], wy = 96.0 * o[1] / o[3];
    if (fabs(ux) > 256 || fabs(uy) > 192 || fabs(wx) > 256 || fabs(wy) > 192) continue;
    const double mx = cl(wx, 256) - cl(ux, 256), my = cl(uy, 192) - cl(wy, 192); /* (moved - own, y down) */
    sx += old ? mx : -mx, sy += old ? my : -my;
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

enum { A_MISPAIR, A_OFFMOTION, B_TEXSTEP, B_TEXSCROLL, B_HELD, B_HELDRUN, B_WRAP, C_MISSED, C_APPEAR, C_VANISH, C_NEW, C_BLINK, C_KEPT, C_EDGE, E_SPEED, E_CUT,
       AL_MISPAIR, AL_OFFMOTION, O_DISAGREE, O_MISSED, V_NONE, V_TWICE, V_STALE, P_NONE, P_TWICE, P_CROSS, NCLS }; /* (AL_, O_, V_, P_: --oracle only) */
static const char *cls_name[NCLS] = {"A_MISPAIR", "A_OFFMOTION", "B_TEXSTEP", "B_TEXSCROLL", "B_HELD", "B_HELDRUN", "B_WRAP", "C_MISSED",
                                     "C_APPEAR",  "C_VANISH",    "C_NEW",     "C_BLINK",     "C_KEPT",   "C_EDGE",  "E_SPEED", "E_CUT",
                                     "AL_MISPAIR", "AL_OFFMOTION",
                                     "O_DISAGREE", "O_MISSED",   "V_NONE",    "V_TWICE",     "V_STALE",
                                     "P_NONE",    "P_TWICE",     "P_CROSS"};
typedef struct Gone { u32 key; double cx, cy; } Gone; /* a C_VANISH list of the pair before: its key and place in that pair's OLD */
static Gone *gone[2];
static int ngone[2], capgone;
static double gonegx, gonegy; /* that pair's camera motion */
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
  const int info_only = c == C_NEW || c == C_KEPT;
  double v120 = info_only ? 0 : err * v120f, v60 = info_only ? 0 : err * v60f;
  if (c == B_TEXSTEP || c == B_TEXSCROLL || c == B_HELD || c == B_HELDRUN || c == B_WRAP) v120 = v60 = err; /* texels: every in-between */
  sum[c].n++;
  sum[c].v1 += v120 >= 1;
  sum[c].v3 += v120 >= 3;
  if (v120 > sum[c].maxv || sum[c].n == 1) {
    sum[c].maxv = v120;
    sum[c].frame = frame;
    sum[c].list = list;
  }
  if (events && (v120 >= 1 || c >= E_SPEED || info_only))
    printf("EV %s %u %u %d %08x %u %.1f %.1f %.1f %.2f %.2f %.2f %s\n", cls_name[c], frame, oldframe, list, l ? keyx(l) : 0, l ? l->n : 0,
           b ? b->cx : 0, b ? b->cy : 0, b ? b->size : 0, err, v120, v60, info ? info : "-");
}

/* ---- the oracle (--oracle): list identity from shape and matrix beside the live pairing (docs/interp-sweep.md, "Oracle") ----
 * A list's signature is (key, the hash of its object-space vertices, its projection's index); copies of one signature are told apart
 * by D = inverse(mv_key) x mv_old (row vectors: mv_old = mv_key x D), one D for everything that stands still in the world. Steps:
 * 1 a signature with one list in each frame is a pair (uniq); 2 every candidate pair of a signature votes once for its D (rotation
 * to 6 / 4096, translation to 1 / 16 unit), the most voted D per projection is the camera's and the others with 2 votes or more are
 * rigid groups; 3 copies pair through those Ds, the camera's first, each list once, in draw order (static); what is left of a
 * signature pairs when one list is left on each side (moved), stays unpaired when several are (ambig), or has no candidate (new). */
static int oracle;        /* --oracle */
static unsigned orcdump; /* --orcdump FRAME: the oracle's view of the pair whose KEY is FRAME, list by list */
enum { O_NONE, O_UNIQ, O_STATIC, O_MOVED, O_AMBIG, O_NEW, O_EMPTY, O_LOOK, O_TAG, NOCLS };
static const char *ocls_name[NOCLS] = {"-", "uniq", "static", "moved", "ambig", "new", "empty", "look", "tag"};
/* ORACLEP (M5's measure, vc_ip_pair_how's how): per way a list was paired (identity, a tag, the rest) its pairs and the oracle's verdict
 * on them (right, wrong, unknown, unknown on the screen, unknown off the camera's motion by > thr), and the lists whose pair was dropped
 * (a tag pair that jumped, the camera plane) with those of them the oracle pairs; with --pairs an ORC DROP line for each such list */
enum { NHOW = VC_IP_HOW_BEHIND + 1 };
static unsigned long long wp[NHOW][7];
static const char *wname[NHOW] = {"none", "id", "tag", "rest", "jumped", "behind"};
static unsigned long long otagged; /* keyframe pairs with tags in both frames ("tag" is printed only then: untagged output as before) */
typedef struct Aff { double a[9], t[3]; } Aff; /* v' = v x a + t */
static void aff_of(const s32 *mv, Aff *m) {
  for (int i = 0; i < 9; i++) m->a[i] = mv[i] / 4096.0;
  for (int i = 0; i < 3; i++) m->t[i] = mv[9 + i] / 4096.0;
}
static int aff_inv(const Aff *m, Aff *r) {
  const double *a = m->a;
  double c0 = a[4] * a[8] - a[5] * a[7], c1 = a[5] * a[6] - a[3] * a[8], c2 = a[3] * a[7] - a[4] * a[6];
  double det = a[0] * c0 + a[1] * c1 + a[2] * c2;
  if (fabs(det) < 1e-12) return 0;
  double *b = r->a, id = 1.0 / det;
  b[0] = c0 * id, b[1] = (a[2] * a[7] - a[1] * a[8]) * id, b[2] = (a[1] * a[5] - a[2] * a[4]) * id;
  b[3] = c1 * id, b[4] = (a[0] * a[8] - a[2] * a[6]) * id, b[5] = (a[2] * a[3] - a[0] * a[5]) * id;
  b[6] = c2 * id, b[7] = (a[1] * a[6] - a[0] * a[7]) * id, b[8] = (a[0] * a[4] - a[1] * a[3]) * id;
  for (int j = 0; j < 3; j++) r->t[j] = -(m->t[0] * b[j] + m->t[1] * b[3 + j] + m->t[2] * b[6 + j]);
  return 1;
}
static void aff_mul(const Aff *x, const Aff *y, Aff *r) { /* x, then y */
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) r->a[i * 3 + j] = x->a[i * 3] * y->a[j] + x->a[i * 3 + 1] * y->a[3 + j] + x->a[i * 3 + 2] * y->a[6 + j];
  for (int j = 0; j < 3; j++) r->t[j] = x->t[0] * y->a[j] + x->t[1] * y->a[3 + j] + x->t[2] * y->a[6 + j] + y->t[j];
}
#define ORC_QA (6.0 / 4096) /* D's quanta and match tolerances */
#define ORC_QT (1.0 / 16)
#define ORC_FAR 12.0 /* world units (a Gen 4 tile is 16) */
static u32 aff_bin(const Aff *d) {
  u32 h = 2166136261u;
  for (int i = 0; i < 9; i++) h = mixk(h, (u32)lround(d->a[i] / ORC_QA));
  for (int i = 0; i < 3; i++) h = mixk(h, (u32)lround(d->t[i] / ORC_QT));
  return h;
}
static double aff_dist(const Aff *x, const Aff *y, double *dt) { /* the largest rotation entry's difference; *dt: translation's */
  double da = 0, t = 0;
  for (int i = 0; i < 9; i++) da = fmax(da, fabs(x->a[i] - y->a[i]));
  for (int i = 0; i < 3; i++) t = fmax(t, fabs(x->t[i] - y->t[i]));
  *dt = t;
  return da;
}
typedef struct OBin { u32 h; int proj, votes, last, uniq, pairs; Aff d; } OBin; /* uniq: votes of signatures unique in both frames */
/* step 2's order (p23_interp.c bin_better): votes, then votes of signatures unique in both frames, then the candidate pairs it explains
 * (a grid of copies: the true D maps every copy, one step over a row fewer), then the smaller translation, then the hash */
static int obin_better(const OBin *p, const OBin *q) {
  if (p->votes != q->votes) return p->votes > q->votes;
  if (p->uniq != q->uniq) return p->uniq > q->uniq;
  if (p->pairs != q->pairs) return p->pairs > q->pairs;
  const double tp = p->d.t[0] * p->d.t[0] + p->d.t[1] * p->d.t[1] + p->d.t[2] * p->d.t[2];
  const double tq = q->d.t[0] * q->d.t[0] + q->d.t[1] * q->d.t[1] + q->d.t[2] * q->d.t[2];
  if (tp != tq) return tp < tq;
  return p->h < q->h;
}
typedef struct OCand { int i, j, d, g; } OCand; /* a candidate pair, the chosen D it fits (-1 none), its signature's group */
static int ocand_cmp(const void *x, const void *y) {
  const OCand *a = (const OCand *)x, *b = (const OCand *)y;
  if ((a->d < 0) != (b->d < 0)) return a->d < 0 ? 1 : -1;
  if (a->d != b->d) return a->d - b->d;
  return a->i != b->i ? a->i - b->i : a->j - b->j;
}
typedef struct OSig { u32 sig; int idx; } OSig;
static int osig_cmp(const void *x, const void *y) {
  const OSig *a = (const OSig *)x, *b = (const OSig *)y;
  return a->sig != b->sig ? (a->sig < b->sig ? -1 : 1) : a->idx - b->idx;
}
static u32 osig(const VC_IpList *l) { return mixk(mixk(keyx(l), l->geo), l->proj); }
static u32 olook(const VC_IpList *l) { /* without the texture image's address and the palette: what a texture's animation changes */
  VC_IpList m = *l;
  m.texparam &= 0xFFFF0000u;
  m.pltt = 0;
  return mixk(mixk(keyx(&m), l->geo), l->proj);
}
/* a D's plausible motion: the first chosen D it fits, nch: none of them but no motion at all (it stood still relative to the camera:
 * the player, the follower, their shadows while the camera follows them), -1 */
static int orc_plaus(const OBin *ch, int nch, const Aff *d) {
  static const Aff one = {{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}};
  double dt;
  for (int q = 0; q < nch; q++)
    if (aff_dist(&ch[q].d, d, &dt) <= ORC_QA && dt <= ORC_QT) return q;
  return aff_dist(&one, d, &dt) <= ORC_QA && dt <= ORC_QT ? nch : -1;
}
static struct {
  unsigned long long cls[NOCLS], gone, far, wfar, ds, disagree, missed, ptp, pfp, twod;
  double rmax, tmax;
} ost;
/* the tags (docs/interp-sweep.md M3) when both frames have some: every tagged list with vertices is decided (dk / dob = 1); KEY's list
 * pairs with OLD's of its (owner, inst), the instance's lists in draw order, of the same type and vertex count (and shape when the
 * instance drew another number of lists in the other frame); an overlay (kind 6) never; a particle (kind 3) whose age went down is a
 * new particle in its slot. Written apart from the live pass (p23_interp.c tag_pair); the tags are the truth here whatever
 * BWPORT_INTERP_TAGS says. Returns the pairs */
static int orc_tags(const VC_IpRec *k, const VC_IpRec *o, int *tko, int *tok, u8 *dk, u8 *dob) {
  const int on = k->ntag && o->ntag;
  int n = 0;
  for (u32 i = 0; i < k->nl; i++) tko[i] = -1, dk[i] = on && k->l[i].owner && k->l[i].n;
  for (u32 j = 0; j < o->nl; j++) tok[j] = -1, dob[j] = on && o->l[j].owner && o->l[j].n;
  for (u32 i = 0; i < k->nl; i++) {
    const VC_IpList *p = &k->l[i];
    if (!dk[i] || p->owner >> 24 == 6) continue;
    int ck = 0, co = 0;
    for (u32 q = 0; q < k->nl; q++) ck += dk[q] && k->l[q].owner == p->owner && k->l[q].inst == p->inst;
    for (u32 j = 0; j < o->nl; j++) co += dob[j] && o->l[j].owner == p->owner && o->l[j].inst == p->inst;
    for (u32 j = 0; j < o->nl; j++) {
      const VC_IpList *q = &o->l[j];
      if (!dob[j] || tok[j] >= 0 || q->owner != p->owner || q->inst != p->inst || q->type != p->type || q->n != p->n) continue;
      if ((ck != co && q->geo != p->geo) || (p->owner >> 24 == 3 && p->state < q->state)) continue;
      tko[i] = (int)j, tok[j] = (int)i, n++;
      break;
    }
  }
  return n;
}
/* the oracle's pairs of KEY's lists (k) with OLD's (o): ko[i] = OLD's list or -1, ok_o[j] = KEY's list or -1, cls[i] = O_*; ch[*pnch]
 * the chosen Ds (at most 64). Returns the number of chosen Ds.
 * tier 1, after tier 0 (O_LOOK, 2026-11-26): the lists left, by olook (Platinum's door scene redraws a sprite at its place each
 * keyframe with the next image and palette), through tier 0's Ds: a pair only through a plausible motion (orc_plaus), and a group whose
 * list has two plausible candidates, even through one motion, is left (HeartGold's grass over a sprite's feet: the sprite's shape at its
 * place, another palette).
 * dk / dob: the lists the tags decide (orc_tags): they vote in step 2 and no step pairs them. */
static int orc_pair(const VC_IpRec *k, const VC_IpRec *o, int *ko, int *ok_o, u8 *cls, int tier, OBin *ch, int *pnch, const u8 *dk,
                    const u8 *dob) {
  int nk = (int)k->nl, no = (int)o->nl;
  OSig *sk = (OSig *)malloc(sizeof(OSig) * (size_t)(nk + 1)), *so = (OSig *)malloc(sizeof(OSig) * (size_t)(no + 1));
  Aff *ik = (Aff *)malloc(sizeof(Aff) * (size_t)(nk + 1)), *ao = (Aff *)malloc(sizeof(Aff) * (size_t)(no + 1));
  u8 *invok = (u8 *)calloc((size_t)nk + 1, 1);
  int mk = 0, mo = 0;
  for (int i = 0; i < nk; i++) {
    if (!tier) ko[i] = -1, cls[i] = k->l[i].n ? O_NONE : O_EMPTY;
    if (!k->l[i].n || ko[i] >= 0 || (tier && dk[i])) continue;
    Aff m;
    aff_of(k->l[i].mv, &m);
    invok[i] = (u8)aff_inv(&m, &ik[i]);
    sk[mk++] = (OSig){tier ? olook(&k->l[i]) : osig(&k->l[i]), i};
  }
  for (int j = 0; j < no; j++) {
    if (!tier) ok_o[j] = -1;
    if (!o->l[j].n || ok_o[j] >= 0 || (tier && dob[j])) continue;
    aff_of(o->l[j].mv, &ao[j]);
    so[mo++] = (OSig){tier ? olook(&o->l[j]) : osig(&o->l[j]), j};
  }
  u8 *c0 = (u8 *)malloc((size_t)nk + 1); /* the classes before this tier */
  memcpy(c0, cls, (size_t)nk);
  qsort(sk, (size_t)mk, sizeof *sk, osig_cmp);
  qsort(so, (size_t)mo, sizeof *so, osig_cmp);
  /* the groups: [ga, gb) in sk with [ha, hb) in so */
  int capb = 1024, nb = 0;
  OBin *bin = (OBin *)calloc((size_t)capb, sizeof(OBin)); /* open addressing on h */
  int ncand = 0, capc = 4096;
  OCand *cand = (OCand *)malloc(sizeof(OCand) * (size_t)capc);
  Aff *cd = (Aff *)malloc(sizeof(Aff) * (size_t)capc);
  int g = 0, x = 0, gid = 0;
  while (g < mk) {
    int ge = g;
    while (ge < mk && sk[ge].sig == sk[g].sig) ge++;
    while (x < mo && so[x].sig < sk[g].sig) x++;
    int xe = x;
    while (xe < mo && so[xe].sig == sk[g].sig) xe++;
    gid++;
    if (ge - g == 1 && xe - x == 1) { /* step 1 */
      int i = sk[g].idx, j = so[x].idx;
      if (!dk[i] && !dob[j]) ko[i] = j, ok_o[j] = i, cls[i] = tier ? O_LOOK : O_UNIQ;
    }
    if (xe > x && (long)(ge - g) * (xe - x) <= 1L << 16)
      for (int a = g; a < ge; a++)
        for (int b = x; b < xe; b++) {
          int i = sk[a].idx, j = so[b].idx;
          if (!invok[i]) continue;
          if (ncand == capc) {
            capc *= 2;
            cand = (OCand *)realloc(cand, sizeof(OCand) * (size_t)capc);
            cd = (Aff *)realloc(cd, sizeof(Aff) * (size_t)capc);
          }
          aff_mul(&ik[i], &ao[j], &cd[ncand]);
          u32 h = aff_bin(&cd[ncand]);
          if (nb * 2 >= capb) { /* grow and rehash */
            OBin *nbn = (OBin *)calloc((size_t)capb * 2, sizeof(OBin));
            for (int q = 0; q < capb; q++)
              if (bin[q].votes) {
                u32 p = bin[q].h & (u32)(capb * 2 - 1);
                while (nbn[p].votes) p = (p + 1) & (u32)(capb * 2 - 1);
                nbn[p] = bin[q];
              }
            free(bin);
            bin = nbn;
            capb *= 2;
          }
          u32 p = h & (u32)(capb - 1);
          while (bin[p].votes && bin[p].h != h) p = (p + 1) & (u32)(capb - 1);
          if (!bin[p].votes) bin[p] = (OBin){h, k->l[i].proj, 0, 0, 0, 0, cd[ncand]}, nb++;
          bin[p].pairs++;
          if (bin[p].last != gid) bin[p].votes++, bin[p].last = gid;
          if (ge - g == 1 && xe - x == 1) bin[p].uniq++;
          if (!dk[i] && !dob[j]) cand[ncand++] = (OCand){i, j, -1, gid};
        }
    g = ge;
    x = xe;
  }
  /* step 2: the chosen Ds: per projection the most voted (the camera's), then every other bin that two signatures unique in both
   * frames voted for (a rigid group with two anchors: copies alone, a shadow and its twin, vote as much for swapping places as for
   * standing still, and one lone coincidence is enough to make such a D), by votes */
  int nch = tier ? *pnch : 0, capch = 64;
  while (!tier) {
    int best = -1;
    for (int q = 0; q < capb; q++) {
      if (bin[q].votes < 2 || bin[q].last < 0) continue;
      if (nch && bin[q].uniq < 2) continue;
      if (best < 0 || obin_better(&bin[q], &bin[best])) best = q;
    }
    if (best < 0 || nch == capch) break;
    int dup = 0;
    for (int c = 0; c < nch && !dup; c++) {
      double dt, da = aff_dist(&ch[c].d, &bin[best].d, &dt);
      dup = da <= ORC_QA && dt <= ORC_QT;
    }
    if (!dup) ch[nch++] = bin[best];
    bin[best].last = -1;
  }
  *pnch = nch;
  /* each candidate's D: the first chosen one it fits. Its plausible motion (orc_plaus): that one, or none at all (nch). A group whose
   * list has candidates through two plausible motions is not identified (2026-11-26: at Run 4x the camera moves a tile a keyframe, the
   * follower's shadow stands where the player's stood in the world and the player's where it stood on the screen; the camera's D first
   * paired them crosswise, the screen's D is the truth there and the camera's for a row of tiles) */
  u8 *amb = (u8 *)calloc((size_t)gid + 2, 1);
  int *pj = (int *)malloc(sizeof(int) * 2 * (size_t)(no + 1)), *pg = pj + no + 1;
  for (int j = 0; j < no; j++) pg[j] = -1;
  for (int c = 0, ri = -1, rg = -1, cp = -1; c < ncand; c++) {
    int p = orc_plaus(ch, nch, &cd[c]), j = cand[c].j;
    cand[c].d = p < nch ? p : -1;
    if (p < 0) continue;
    if (cand[c].i != ri || cand[c].g != rg) ri = cand[c].i, rg = cand[c].g, cp = -1;
    if (cp >= 0 && (cp != p || tier)) amb[cand[c].g] = 1;
    cp = p;
    if (pg[j] == cand[c].g && (pj[j] != p || tier)) amb[cand[c].g] = 1;
    pg[j] = cand[c].g, pj[j] = p;
  }
  for (int c = 0; c < ncand; c++)
    if (amb[cand[c].g]) cand[c].d = -1;
  free(pj);
  qsort(cand, (size_t)ncand, sizeof *cand, ocand_cmp);
  /* step 3: copies through the chosen Ds (uniq pairs are taken already) */
  for (int c = 0; c < ncand && cand[c].d >= 0; c++) {
    int i = cand[c].i, j = cand[c].j;
    if (ko[i] >= 0 || ok_o[j] >= 0) continue;
    ko[i] = j, ok_o[j] = i, cls[i] = tier ? O_LOOK : O_STATIC;
  }
  /* what is left of each signature */
  g = 0, x = 0, gid = 0;
  while (g < mk) {
    int ge = g, la = 0, lb = 0, ia = -1, jb = -1;
    while (ge < mk && sk[ge].sig == sk[g].sig) ge++;
    while (x < mo && so[x].sig < sk[g].sig) x++;
    int xe = x;
    while (xe < mo && so[xe].sig == sk[g].sig) xe++;
    gid++;
    if (amb[gid] && !tier)
      for (int a = g; a < ge; a++) ost.twod += ko[sk[a].idx] < 0 && !dk[sk[a].idx];
    for (int a = g; a < ge; a++)
      if (ko[sk[a].idx] < 0 && !dk[sk[a].idx]) la++, ia = sk[a].idx;
    for (int b = x; b < xe; b++)
      if (ok_o[so[b].idx] < 0 && !dob[so[b].idx]) lb++, jb = so[b].idx;
    if (la == 1 && lb == 1 && !amb[gid]) ko[ia] = jb, ok_o[jb] = ia, cls[ia] = tier ? O_LOOK : O_MOVED;
    else if (!tier)
      for (int a = g; a < ge; a++)
        if (ko[sk[a].idx] < 0 && !dk[sk[a].idx]) cls[sk[a].idx] = lb ? O_AMBIG : O_NEW;
    g = ge;
    x = xe;
  }
  /* a unique or last copy that jumps in the world is not the same object (the grass's last image of one tile and the next tile's
   * in the same image; one house culled and its twin drawn; the rings of the player and the follower a phase apart): no plausible D
   * fits it (a chosen one, or none: it stood still relative to the camera) and it is more than ORC_FAR world units from where the
   * camera's D puts it, measured in KEY's camera space (2026-11-26: before, in the list's own units, and a billboard's are scaled: the
   * rings' 15-unit swap measured 8) */
  Aff icam;
  int camok = nch && aff_inv(&ch[0].d, &icam);
  for (int i = 0; i < nk && tier; i++) { /* tier 1: a plausible motion or no pair */
    if (cls[i] != O_LOOK || c0[i] == O_LOOK) continue;
    Aff d;
    if (invok[i] && (aff_mul(&ik[i], &ao[ko[i]], &d), orc_plaus(ch, nch, &d) >= 0)) continue;
    ok_o[ko[i]] = -1, ko[i] = -1, cls[i] = c0[i];
  }
  for (int i = 0; i < nk && !tier; i++) {
    if ((cls[i] != O_UNIQ && cls[i] != O_MOVED) || !invok[i] || !camok) continue;
    Aff d;
    aff_mul(&ik[i], &ao[ko[i]], &d);
    if (orc_plaus(ch, nch, &d) >= 0) continue;
    const Aff *m = &ao[ko[i]];
    double e = 0;
    for (int c = 0; c < 3; c++) {
      double p = m->t[0] * icam.a[c] + m->t[1] * icam.a[3 + c] + m->t[2] * icam.a[6 + c] + icam.t[c] - k->l[i].mv[9 + c] / 4096.0;
      e += p * p;
    }
    if (e <= ORC_FAR * ORC_FAR) continue;
    ok_o[ko[i]] = -1, ko[i] = -1, cls[i] = O_NEW;
    ost.wfar++;
  }
  /* residuals of the static pairs against their D */
  for (int i = 0; i < nk && !tier; i++) {
    if (cls[i] != O_STATIC && cls[i] != O_UNIQ) continue;
    Aff d;
    if (!invok[i]) continue;
    aff_mul(&ik[i], &ao[ko[i]], &d);
    for (int q = 0; q < nch; q++) {
      double dt, da = aff_dist(&ch[q].d, &d, &dt);
      if (da <= ORC_QA && dt <= ORC_QT) {
        ost.rmax = fmax(ost.rmax, da * 4096);
        ost.tmax = fmax(ost.tmax, dt * 4096);
        break;
      }
    }
  }
  free(sk), free(so), free(ik), free(ao), free(invok), free(bin), free(cand), free(cd), free(amb), free(c0);
  return nch;
}

/* the texture and palette bytes a list samples (vc_ip_draw_kept draws an OLD list with OLD's texparam / pltt in KEY's VRAM) */
#define OTEX (512 * 1024)
#define OPAL (96 * 1024)
static void tex_range(u32 tp, u32 pltt, u32 r[3][2]) { /* {texels, 4x4 indices, palette}: offset, bytes */
  u32 fmt = tp >> 26 & 7, w = 8u << (tp >> 20 & 7), h = 8u << (tp >> 23 & 7), a = (tp & 0xFFFF) << 3;
  static const u32 bpp[8] = {0, 8, 2, 4, 8, 2, 8, 16}, pal[8] = {0, 64, 8, 32, 512, 512, 16, 0};
  memset(r, 0, sizeof(u32) * 6);
  if (!fmt) return;
  r[0][0] = a, r[0][1] = w * h * bpp[fmt] / 8;
  if (fmt == 5) r[1][0] = 0x20000 + (a & 0x1FFFF) / 2 + (a >= 0x40000 ? 0x10000 : 0), r[1][1] = w * h / 8;
  r[2][0] = fmt == 2 ? pltt << 3 : pltt << 4, r[2][1] = pal[fmt];
}
static int tex_changed(const u8 *ta, const u8 *pa, const u8 *tb, const u8 *pb, const VC_IpList *l) {
  u32 r[3][2];
  tex_range(l->texparam, l->pltt, r);
  for (int q = 0; q < 3; q++) {
    const u8 *x = q < 2 ? ta : pa, *y = q < 2 ? tb : pb;
    u32 lim = q < 2 ? OTEX : OPAL, o = r[q][0], n = r[q][1];
    if (o >= lim) continue;
    if (o + n > lim) n = lim - o;
    if (n && memcmp(x + o, y + o, n)) return 1;
  }
  return 0;
}
static void vram_snap(VCoreState *s, u8 *tex, u8 *pal) {
  vc_vram_map(s);
  for (u32 a = 0; a < OTEX; a++) tex[a] = vc_rd8(&s->rgn[VC_RGN_TEX], a);
  for (u32 a = 0; a < OPAL; a++) pal[a] = vc_rd8(&s->rgn[VC_RGN_TEXPAL], a);
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
    else if (!strcmp(argv[i], "--time")) timing = 1;
    else if (!strcmp(argv[i], "--pairs")) pairlog = 1;
    else if (!strcmp(argv[i], "--oracle")) oracle = 1;
    else if (!strcmp(argv[i], "--orcdump") && i + 1 < argc) oracle = 1, orcdump = (unsigned)atoi(argv[++i]);
    else files[nf++] = argv[i];
  }
  if (nf < 2) {
    fprintf(stderr, "usage: vcore_ipsweep [--thr PX] [--events] A.vcs B.vcs ...\n");
    return 2;
  }
  VCoreState *s = vcore_create();
  char err[256];
  VC_IpRec key, old, older; /* (older: the keyframe before old, vc_ip_keep's pair before) */
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  memset(&older, 0, sizeof older);
  int *pmap = NULL, cappm = 0, mapok = 0, pmapok = 0; /* the pair before's map (old to older), as IpGather keeps it */
  VC_IpKeep *keep = NULL;
  int *kept = NULL;
  int have = 0;
  unsigned kf = 0, of = 0;
  int *pstep = NULL; /* the pair before's cell steps (vc_ip_texhold) */
  unsigned long long nmove = 0;
  double campath = 0;
  int unp_max = 0;
  unsigned long long nscene = 0, nkey = 0, nrep = 0, npair = 0, ncut = 0, lists = 0, lpaired = 0, gaps[4] = {0, 0, 0, 0};
  int *map = NULL, *mb = NULL, cap = 0, capb = 0;
  u8 *th = NULL, *how = NULL; /* (how: vc_ip_pair_how's, M4's cut, ORACLEP) */
  int *heldd = NULL, *heldp = NULL, capheld = 0, heldn = 0; /* per OLD list: the held step (axis * 65536 + delta) of the pair before */
  double pvx = 0, pvy = 0;
  int pmoving = 0;
  int *ko = NULL, *oko = NULL, *ptk = NULL; /* --oracle: KEY's / OLD's oracle partners, OLD's lists a proxy took */
  u8 *ocls = NULL;
  int *tko = NULL, *tok = NULL; /* ... the tags' pairs (orc_tags) and the lists they decide */
  u8 *tdk = NULL, *tdo = NULL;
  u8 *vtk = NULL, *vpk = NULL, *vto = NULL, *vpo = NULL; /* --oracle: KEY's and OLD's texture and palette VRAM */
  if (oracle) {
    vtk = (u8 *)calloc(OTEX, 1), vto = (u8 *)calloc(OTEX, 1);
    vpk = (u8 *)calloc(OPAL, 1), vpo = (u8 *)calloc(OPAL, 1);
  }
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
    if (vc_ip_state_on()) vc_ip_texsig(s, &cur); /* (as IpGather: M4's texture-bytes test) */
    if (mapok) {
      if ((int)key.nl + 1 > cappm) {
        cappm = (int)key.nl * 2 + 64;
        pmap = (int *)realloc(pmap, sizeof(int) * (size_t)cappm);
      }
      memcpy(pmap, map, sizeof(int) * key.nl);
    }
    pmapok = mapok;
    mapok = 0;
    vc_ip_free(&older);
    older = old;
    old = key;
    of = kf;
    key = cur;
    kf = fr;
    nkey++;
    if (oracle) {
      u8 *t = vto;
      vto = vtk, vtk = t, t = vpo, vpo = vpk, vpk = t;
      vram_snap(s, vtk, vpk);
    }
    int hadold = have;
    have = 1;
    if (!hadold || !key.swapped || !old.swapped || key.nl == 0 || old.nl == 0) {
      pmoving = 0;
      heldn = 0;
      ngone[0] = 0;
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
      how = (u8 *)realloc(how, (size_t)cap);
      ko = (int *)realloc(ko, sizeof(int) * (size_t)cap);
      ocls = (u8 *)realloc(ocls, (size_t)cap);
      tko = (int *)realloc(tko, sizeof(int) * (size_t)cap);
      tdk = (u8 *)realloc(tdk, (size_t)cap);
    }
    if ((int)old.nl + 1 > capb) {
      capb = (int)old.nl * 2 + 64;
      mb = (int *)realloc(mb, sizeof(int) * (size_t)capb);
      keep = (VC_IpKeep *)realloc(keep, sizeof *keep * (size_t)capb);
      kept = (int *)realloc(kept, sizeof(int) * (size_t)capb);
      oko = (int *)realloc(oko, sizeof(int) * (size_t)capb);
      tok = (int *)realloc(tok, sizeof(int) * (size_t)capb);
      tdo = (u8 *)realloc(tdo, (size_t)capb);
      ptk = (int *)realloc(ptk, sizeof(int) * (size_t)capb);
    }
    VC_IpStats st;
    const double t0 = timing ? now_us() : 0;
    vc_ip_pair_how(&key, &old, map, how, LIMIT, &st);
    mapok = 1;
    double enter[VC_IP_NENTER][16];
    int nkeep = vc_ip_windows(&key, &old, map, pmapok ? &older : NULL, pmapok ? pmap : NULL, &st, keep, enter);
    { /* the cell hold and M4's state rules before the cut, as IpGather */
      int *stk = (int *)calloc(2 * (size_t)key.nl + 2, sizeof(int));
      vc_ip_texhold(&key, &old, map, th, hold_thr, TEXHOLD_PX, pstep ? pstep : vc_ip_nostep, stk);
      free(pstep);
      pstep = stk;
      nkeep = vc_ip_state(s, &key, &old, map, th, keep, nkeep);
    }
    const double t1 = timing ? now_us() : 0;
    for (u32 j = 0; j < old.nl; j++) kept[j] = 0;
    for (int k = 0; k < nkeep; k++) kept[keep[k].list] = k + 1;
    if (oracle) {
      OBin och[64];
      int nds = 0;
      const int tagged = key.ntag && old.ntag, ntp = orc_tags(&key, &old, tko, tok, tdk, tdo);
      orc_pair(&key, &old, ko, oko, ocls, 0, och, &nds, tdk, tdo);
      orc_pair(&key, &old, ko, oko, ocls, 1, och, &nds, tdk, tdo);
      for (u32 i = 0; i < key.nl; i++)
        if (tdk[i]) ko[i] = tko[i], ocls[i] = tko[i] >= 0 ? O_TAG : O_NEW;
      for (u32 j = 0; j < old.nl; j++)
        if (tdo[j]) oko[j] = tok[j];
      otagged += (unsigned long long)tagged;
      ost.ds += (unsigned long long)nds;
      int oc[NOCLS] = {0}, gone_o = 0;
      for (u32 i = 0; i < key.nl; i++) ost.cls[ocls[i]]++, oc[ocls[i]]++;
      if (orcdump == kf) {
        for (u32 i = 0; i < key.nl; i++)
          printf("OK %u n %u key %08x geo %08x proj %u cls %s oracle %d live %d t %d %d %d\n", i, key.l[i].n, keyx(&key.l[i]), key.l[i].geo, key.l[i].proj,
                 ocls_name[ocls[i]], ko[i], map[i], key.l[i].mv[9], key.l[i].mv[10], key.l[i].mv[11]);
        for (u32 j = 0; j < old.nl; j++)
          printf("OO %u n %u key %08x geo %08x proj %u oracle %d kept %d t %d %d %d\n", j, old.l[j].n, keyx(&old.l[j]), old.l[j].geo, old.l[j].proj,
                 oko[j], kept[j], old.l[j].mv[9], old.l[j].mv[10], old.l[j].mv[11]);
      }
      for (u32 j = 0; j < old.nl; j++) gone_o += old.l[j].n && oko[j] < 0;
      ost.gone += (unsigned long long)gone_o;
      if (pairlog)
        printf("ORC %u %u uniq %d static %d moved %d ambig %d new %d empty %d gone %d ds %d", kf, of, oc[O_UNIQ], oc[O_STATIC], oc[O_MOVED],
               oc[O_AMBIG], oc[O_NEW], oc[O_EMPTY], gone_o, nds);
      if (pairlog && tagged) printf(" tag %d", ntp);
      if (pairlog) printf("\n");
    }
    const double t2 = timing ? now_us() : 0;
    int unp = 0, unps = 0, cut = vc_ip_cut_id(&key, &old, map, how, &unps, NULL); /* (unps: those on the screen; the cut counts only them) */
    if (timing) {
      if (ntus == captus) captus = captus ? 2 * captus : 1024, tus = (double *)realloc(tus, sizeof *tus * (size_t)captus);
      tus[ntus++] = (t1 - t0) + (now_us() - t2);
    }
    vc_ip_scene_cut(map, (int)key.nl, &unp);
    if (unp > unp_max) unp_max = unp;
    if (pairlog)
      printf("PAIR %u %u nl %d/%d nv %u/%u strict %d loose %d removed %d added %d jumps %d vtpaired %u vtrem %u vtadd %u unpaired %d onscreen %d new %d kept %d\n", kf,
             of, st.nl_a, st.nl_b, key.nv, old.nv, st.strict, st.loose, st.removed, st.added, st.jumps, st.vt_paired, st.vt_removed,
             st.vt_added, unp, unps, st.fresh, nkeep);
    lists += key.nl;
    for (u32 i = 0; i < old.nl; i++) mb[i] = -1;
    for (u32 i = 0; i < key.nl; i++)
      if (map[i] >= 0) {
        lpaired++;
        mb[map[i]] = (int)i;
      }
    if (cut) { /* a cut: the new frame at once, no in-between (vc_ip_cut, as ip_common.h IpGather) */
      ncut++;
      nscene += cut == 2;
      v120f = v60f = 0;
      ev(E_CUT, kf, -1, NULL, NULL, 0, cut == 2 ? "scene" : "cut");
      pmoving = 0;
      heldn = 0;
      ngone[0] = 0;
      free(pstep);
      pstep = NULL;
      continue;
    }
    npair++;
    Box *bk = (Box *)malloc(sizeof(Box) * key.nl), *bo = (Box *)malloc(sizeof(Box) * old.nl);
    double *mx = (double *)malloc(sizeof(double) * key.nl), *my = (double *)malloc(sizeof(double) * key.nl);
    double *smx = (double *)calloc(key.nl + 1, sizeof(double)), *smy = (double *)calloc(key.nl + 1, sizeof(double));
    int *good = (int *)calloc(key.nl, sizeof(int)), *ofar = (int *)calloc(key.nl + 1, sizeof(int));
    int *lgood = (int *)calloc(key.nl + 1, sizeof(int)); /* the live pairs that are ref's samples without --oracle */
    for (u32 i = 0; i < key.nl; i++) bk[i] = box_of(&key, &key.l[i]);
    for (u32 j = 0; j < old.nl; j++) bo[j] = box_of(&old, &old.l[j]);
    double *tx = (double *)malloc(sizeof(double) * (key.nl + 1)), *ty = (double *)malloc(sizeof(double) * (key.nl + 1));
    int ng = 0;
    for (u32 i = 0; i < key.nl; i++) {
      mx[i] = my[i] = 0;
      if (map[i] >= 0)
        lgood[i] = motion(&key, &key.l[i], &old, &old.l[map[i]], &mx[i], &my[i]) && bk[i].front && bo[map[i]].front && bk[i].vis &&
                   key.l[i].n >= 3;
      /* ref's samples: with --oracle the oracle's pairs and their motion (M5: identity as the truth), else the live pairs */
      const int j = oracle ? ko[i] : map[i];
      if (j >= 0 && motion(&key, &key.l[i], &old, &old.l[j], &smx[i], &smy[i]) && bk[i].front && bo[j].front && bk[i].vis &&
          key.l[i].n >= 3) {
        good[i] = 1;
        tx[ng] = smx[i];
        ty[ng] = smy[i];
        ng++;
      }
    }
    double gx = median(tx, ng), gy = median(ty, ng);
    if (fabs(gx) + fabs(gy) > 0.5) nmove++;
    campath += fabs(gx) + fabs(gy);
    /* ref at a point: the median motion of the samples within 64 px (at least 3), else the global one */
    double *nx = tx, *ny = ty;
#define REF(px, py, self, rx, ry)                                                                         \
  do {                                                                                                    \
    int nn = 0;                                                                                           \
    for (u32 q = 0; q < key.nl; q++)                                                                      \
      if (good[q] && (int)q != (self) && fabs(bk[q].cx - (px)) < 64 && fabs(bk[q].cy - (py)) < 64) {      \
        nx[nn] = smx[q];                                                                                  \
        ny[nn] = smy[q];                                                                                  \
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
    /* the old ref (before M5, and without --oracle): the median motion of the live pairs within 64 px (at least 3), else of all of them;
     * with --oracle the A_ classes of a list without a tag's truth take it (AL_) */
    double *lx = (double *)malloc(sizeof(double) * (key.nl + 1)), *ly = (double *)malloc(sizeof(double) * (key.nl + 1));
    int nl0 = 0;
    for (u32 q = 0; q < key.nl; q++)
      if (lgood[q]) lx[nl0] = mx[q], ly[nl0++] = my[q];
    const double lgx = median(lx, nl0), lgy = median(ly, nl0);
#define REFL(px, py, self, rx, ry)                                                                        \
  do {                                                                                                    \
    int nn = 0;                                                                                           \
    for (u32 q = 0; q < key.nl; q++)                                                                      \
      if (lgood[q] && (int)q != (self) && fabs(bk[q].cx - (px)) < 64 && fabs(bk[q].cy - (py)) < 64) {    \
        lx[nn] = mx[q];                                                                                   \
        ly[nn] = my[q];                                                                                   \
        nn++;                                                                                             \
      }                                                                                                   \
    if (nn >= 3) {                                                                                        \
      rx = median(lx, nn);                                                                                \
      ry = median(ly, nn);                                                                                \
    } else {                                                                                              \
      rx = lgx;                                                                                           \
      ry = lgy;                                                                                           \
    }                                                                                                     \
  } while (0)
    /* the oracle's pairs that jump (more than the limit from the motion of the oracle's other pairs there, measured with every vertex
     * in front clamped half a screen beyond the edges, as the live code measures: one far off the screen counts): two copies, one
     * culled and one drawn, or a thing the game moved in one step (Platinum's ledge): not the same object to the in-betweens */
    for (u32 i = 0; i < key.nl && oracle; i++) {
      const int j = ko[i];
      if (j < 0 || !bk[i].front || !bo[j].front || (!bk[i].vis && !bo[j].vis)) continue;
      double rx, ry, ox, oy;
      REF(bk[i].cx, bk[i].cy, (int)i, rx, ry);
      motion_c(&key, &key.l[i], &old, &old.l[j], &ox, &oy);
      ofar[i] = hypot(ox - rx, oy - ry) > LIMIT;
    }
    char info[160];
    for (u32 i = 0; i < key.nl; i++) {
      const VC_IpList *p = &key.l[i];
      u32 kk = keyx(p);
      if (!bk[i].vis || !bk[i].front) continue;
      double rx, ry;
      REF(bk[i].cx, bk[i].cy, (int)i, rx, ry);
      /* a list the tags pair (the game's truth): its tag pair's motion. Identity's pairs are measured against their surroundings like
       * any other list: the oracle's identity is the live pairing's rules, and a list measured against its own pair is always right */
      const int truth = oracle && ocls[i] == O_TAG && good[i] && !ofar[i];
      if (truth) rx = smx[i], ry = smy[i];
      if (map[i] >= 0) {
        const VC_IpList *q = &old.l[map[i]];
        /* the A_ classes: with --oracle a list without a tag's truth is measured against the old ref (the live pairs around it) as
         * AL_MISPAIR / AL_OFFMOTION, a column of its own (independent of the oracle's identity; a pairing that moves a whole area
         * moves it too) */
        double arx = rx, ary = ry;
        int cmis = A_MISPAIR, coff = A_OFFMOTION;
        if (oracle && !truth) {
          REFL(bk[i].cx, bk[i].cy, (int)i, arx, ary);
          cmis = AL_MISPAIR, coff = AL_OFFMOTION;
        }
        double dev = hypot(mx[i] - arx, my[i] - ary);
        if (dev > thr && bo[map[i]].front) {
          /* the same-key list of OLD that moves most like ref */
          double best = 1e30, bx = 0, by = 0;
          int bj = -1;
          for (u32 j = 0; j < old.nl; j++) {
            if ((int)j == map[i] || keyx(&old.l[j]) != kk || !bo[j].front) continue;
            if (fabs(bo[j].cx + arx - bk[i].cx) > 64 || fabs(bo[j].cy + ary - bk[i].cy) > 64) continue;
            double dx, dy;
            if (!motion(&key, p, &old, &old.l[j], &dx, &dy)) continue;
            double d = hypot(dx - arx, dy - ary);
            if (d < best) {
              best = d;
              bj = (int)j;
              bx = dx;
              by = dy;
            }
          }
          if (bj >= 0 && best < dev - thr) {
            snprintf(info, sizeof info, "partner=%d best=%d dev=%.1f bestdev=%.1f ref=%.1f,%.1f nl=%u/%u", map[i], bj, dev, best, arx, ary,
                     key.nl, old.nl);
            ev(cmis, kf, (int)i, p, &bk[i], hypot(mx[i] - bx, my[i] - by), info);
            if (oracle) printf("OA %s %u %u %s %s\n", cls_name[cmis], kf, i, ocls_name[ocls[i]], ko[i] < 0 ? "unknown" : ko[i] == map[i] ? "right" : "wrong");
          } else if (keyx(q) == kk || bk[i].size <= LOOSE_MAXPX) { /* a loose-paired overlay is no motion of its own */
            snprintf(info, sizeof info, "partner=%d dev=%.1f ref=%.1f,%.1f loose=%d", map[i], dev, arx, ary, keyx(q) != kk);
            ev(coff, kf, (int)i, p, &bk[i], dev, info);
            if (oracle) printf("OA %s %u %u %s %s\n", cls_name[coff], kf, i, ocls_name[ocls[i]], ko[i] < 0 ? "unknown" : ko[i] == map[i] ? "right" : "wrong");
          }
        }
        /* texcoords */
        if (p->n == q->n && p->n > 0) {
          for (int c = 0; c < 2; c++) {
            /* d0: the raw step (the hold's); b0, mxd, uni: the steps the blend takes (KEY minus OLD); slide: how far one of them goes
             * beyond the shortest step of an axis that repeats alike in both keyframes (per: its period, 1/16 texel; 0 = none) */
            const int per = tex_period(p->texparam, q->texparam, c);
            int d0 = key.v[p->first].tex[c] - old.v[q->first].tex[c], b0 = 0, uni = 1, mxd = 0, slide = 0;
            for (u32 j = 0; j < p->n; j++) {
              const int raw = key.v[p->first + j].tex[c] - old.v[q->first + j].tex[c];
              int d = -vc_ip_tex_step(p->texparam, q->texparam, c, -raw);
              if (j == 0) b0 = d;
              uni &= d == b0;
              if (per && d > -2048 && d < 2048) {
                int sh = raw % per;
                sh -= 2 * sh > per ? per : 2 * sh < -per ? -per : 0;
                if (abs(d) - abs(sh) > slide) slide = abs(d) - abs(sh);
              }
              if (d < 0) d = -d;
              if (d > mxd) mxd = d;
            }
            int held = th[i] >> c & 1;
            /* (one tagged sprite's hold and a texture state switching at the middle are not the big-list rule's) */
            const int byrule = !(th[i] & VC_IP_HOLD_STATE) &&
                               !(p->owner && p->owner == q->owner && p->inst == q->inst && VC_IP_TAG_KIND(p->owner) == VC_IP_TAG_SPRITE);
            if (held) {
              if (bk[i].size > TEXHOLD_PX && byrule) {
                int code = (c + 1) * 65536 + d0;
                int run = map[i] < heldn && heldp[map[i]] == code;
                snprintf(info, sizeof info, "axis=%c d=%d", "st"[c], d0 / 16);
                ev(run ? B_HELDRUN : B_HELD, kf, (int)i, p, &bk[i], fabs(d0 / 16.0), info);
              }
            } else if (slide > 0) {
              snprintf(info, sizeof info, "axis=%c d0=%d period=%d", "st"[c], b0 / 16, per / 16);
              ev(B_WRAP, kf, (int)i, p, &bk[i], slide / 16.0, info);
            } else if (mxd >= TEXHOLD && mxd < 2048) {
              snprintf(info, sizeof info, "axis=%c d0=%d max=%d", "st"[c], b0 / 16, mxd / 16);
              ev(uni ? B_TEXSCROLL : B_TEXSTEP, kf, (int)i, p, &bk[i], mxd / 16.0, info);
            }
          }
        }
      } else {
        /* unpaired: drawn at KEY's place in every in-between, or (VC_IP_NEW) not before KEY, or (VC_IP_ENTER) blended toward its place
         * in OLD had it stood still in the world */
        if (VC_IP_IS_ENTER(map[i])) {
          double ex, ey;
          motion_m(&key, p, enter[-16 - map[i]], 0, &ex, &ey);
          snprintf(info, sizeof info, "enter drawn=%.1f,%.1f ref=%.1f,%.1f nl=%u/%u", ex, ey, rx, ry, key.nl, old.nl);
          ev(C_EDGE, kf, (int)i, p, &bk[i], hypot(ex - rx, ey - ry), info);
          continue;
        }
        if (VC_IP_IS_SWAP(map[i])) { /* (M4) the new form of a thing drawn in OLD: from the middle of the in-betweens on */
          snprintf(info, sizeof info, "swap ref=%.1f,%.1f nl=%u/%u", rx, ry, key.nl, old.nl);
          ev(C_NEW, kf, (int)i, p, &bk[i], hypot(rx, ry), info);
          continue;
        }
        if (map[i] == VC_IP_NEW) {
          int blink = 0;
          for (int g = 0; g < ngone[0] && !blink; g++)
            blink = gone[0][g].key == kk && fabs(gone[0][g].cx + gonegx + rx - bk[i].cx) <= 8 && fabs(gone[0][g].cy + gonegy + ry - bk[i].cy) <= 8;
          snprintf(info, sizeof info, "ref=%.1f,%.1f nl=%u/%u", rx, ry, key.nl, old.nl);
          if (blink) {
            double sv120 = v120f, sv60 = v60f; /* missing for the whole interval, as C_VANISH */
            v120f = v120f > 0;
            v60f = v60f > 0;
            ev(C_BLINK, kf, (int)i, p, &bk[i], bk[i].size, info);
            v120f = sv120;
            v60f = sv60;
          } else
            ev(C_NEW, kf, (int)i, p, &bk[i], hypot(rx, ry), info);
          continue;
        }
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
    if (oracle) /* ORACLEP: each way's pairs judged by the oracle, the drops */
      for (u32 i = 0; i < key.nl; i++) {
        const int w = how[i] < NHOW ? how[i] : 0;
        if (map[i] >= 0) {
          const int wrong = ko[i] >= 0 ? ko[i] != map[i] : oko[map[i]] >= 0 && oko[map[i]] != (int)i;
          wp[w][0]++, wp[w][1] += ko[i] >= 0 && !wrong, wp[w][2] += wrong;
          if (ko[i] < 0 && !wrong) {
            wp[w][3]++;
            if (bk[i].vis && bk[i].front) {
              double rx, ry;
              REF(bk[i].cx, bk[i].cy, (int)i, rx, ry);
              wp[w][4]++, wp[w][5] += hypot(mx[i] - rx, my[i] - ry) > thr;
            }
          }
        } else if (w == VC_IP_HOW_JUMPED || w == VC_IP_HOW_BEHIND) {
          wp[w][0]++, wp[w][6] += ko[i] >= 0;
          if (pairlog)
            printf("ORC DROP %u %u %s owner %08x inst %u n %u at %.0f,%.0f size %.0f oracle %d map %d\n", kf, i, wname[w], key.l[i].owner,
                   key.l[i].inst, key.l[i].n, bk[i].cx, bk[i].cy, bk[i].size, ko[i], map[i]);
        }
      }
    if (oracle) { /* the oracle's checks: its pairs against the live ones, and "never both, never neither" (docs/interp-sweep.md) */
      for (u32 i = 0; i < key.nl; i++) {
        int j = ko[i];
        if (j < 0) { /* a live pair of a list the oracle pairs with nothing, onto an OLD list the oracle pairs with another */
          if (map[i] >= 0 && oko[map[i]] >= 0 && bk[i].vis && bk[i].front && bo[map[i]].front) {
            ost.disagree++;
            snprintf(info, sizeof info, "live=%d its=%d ocls=%s", map[i], oko[map[i]], ocls_name[ocls[i]]);
            ev(O_DISAGREE, kf, (int)i, &key.l[i], &bk[i], hypot(bk[i].cx - bk[oko[map[i]]].cx, bk[i].cy - bk[oko[map[i]]].cy), info);
          }
          continue;
        }
        if (!bk[i].front || !bo[j].front || (!bk[i].vis && !bo[j].vis)) continue;
        double rx, ry, ox, oy;
        REF(bk[i].cx, bk[i].cy, (int)i, rx, ry);
        motion(&key, &key.l[i], &old, &old.l[j], &ox, &oy);
        if (ofar[i]) { /* the oracle's pair jumps (ofar) */
          ost.far++;
          continue;
        }
        if (map[i] >= 0 && map[i] != j) {
          ost.disagree++;
          snprintf(info, sizeof info, "live=%d oracle=%d ocls=%s ref=%.1f,%.1f", map[i], j, ocls_name[ocls[i]], rx, ry);
          ev(O_DISAGREE, kf, (int)i, &key.l[i], &bk[i], hypot(mx[i] - ox, my[i] - oy), info);
        } else if (map[i] < 0) {
          ost.missed++;
          snprintf(info, sizeof info, "oracle=%d ocls=%s new=%d kept=%d", j, ocls_name[ocls[i]], map[i] == VC_IP_NEW, kept[j] != 0);
          ev(O_MISSED, kf, (int)i, &key.l[i], &bk[i], hypot(ox, oy), info);
        }
        /* how many times the in-betweens draw it: KEY's list unless it waits for KEY, OLD's list when kept */
        int dk = map[i] != VC_IP_NEW, dj = kept[j] != 0 && !keep[kept[j] - 1].half; /* (a half: one form in each half, M4) */
        int mis = map[i] >= 0 && map[i] != j && hypot(bo[map[i]].cx - bo[j].cx, bo[map[i]].cy - bo[j].cy) > fmax(4, bo[j].size / 2);
        if ((dk && dj) || ((!dk || mis) && !dj)) {
          double sv120 = v120f, sv60 = v60f; /* for the whole interval */
          v120f = v120f > 0, v60f = v60f > 0;
          snprintf(info, sizeof info, "oracle=%d live=%d kept=%d mis=%d ocls=%s", j, map[i], dj, mis, ocls_name[ocls[i]]);
          ev(dk && dj ? V_TWICE : V_NONE, kf, (int)i, &key.l[i], &bk[i], bk[i].size, info);
          v120f = sv120, v60f = sv60;
        }
      }
      /* the proxy for a thing whose form changed (no signature in common): a KEY list the oracle calls new and an OLD list it pairs
       * with nothing, of one primitive type, vertex count (3 to 8) and palette (a paletted texture's; 2026-11-26: on the test phone's
       * recording 29 of 29 V_NONE matched a splash, a ring or the grass born at the player's feet with the player's or the follower's
       * sprite of another palette, whose new animation frame the oracle does not pair and the live pairing does), at most 64 px, within max(8 px, half their size) of each other
       * after the camera's motion there, at most twice the other's size; the live partner if it is one, else the nearest, in KEY's order */
      for (u32 j = 0; j < old.nl; j++) ptk[j] = 0;
      for (u32 i = 0; i < key.nl; i++) {
        const VC_IpList *p = &key.l[i];
        if (p->n < 3 || p->n > 8 || bk[i].size > 64 || !bk[i].vis || !bk[i].front || (ocls[i] != O_NEW && ko[i] < 0 && ocls[i] != O_AMBIG) ||
            tdk[i])
          continue; /* (sprite-like lists only: a screen-sized overlay is near everything; the tags' lists are known) */
        int truth = ko[i];
        const u32 pfmt = p->texparam >> 26 & 7;
        double rx, ry;
        REF(bk[i].cx, bk[i].cy, (int)i, rx, ry);
        for (int pass = 0; pass < 2; pass++) { /* 0: the proxy's precision on lists the oracle pairs; 1: the proxy itself */
          if ((pass == 0) != (truth >= 0)) continue;
          double best = 1e30;
          int bj = -1;
          for (int live = pass; live >= 0 && bj < 0; live--) /* the proxy itself: the live partner first when it is a candidate */
            for (u32 j = live ? (u32)(map[i] >= 0 ? map[i] : 0) : 0; j < old.nl; j++) {
              const VC_IpList *q = &old.l[j];
              if (live && (map[i] < 0 || (int)j != map[i])) break;
              if (q->n != p->n || q->type != p->type || !bo[j].vis || !bo[j].front || (pass == 1 && (oko[j] >= 0 || ptk[j] || tdo[j]))) continue;
              if (pfmt != 0 && pfmt != 7 && q->pltt != p->pltt) continue; /* a paletted sprite keeps its palette (2026-11-26) */
              double lo = fmin(bk[i].size, bo[j].size), hi = fmax(bk[i].size, bo[j].size);
              double d = hypot(bo[j].cx + rx - bk[i].cx, bo[j].cy + ry - bk[i].cy);
              if (hi > 2 * lo + 2 || d > fmax(8, hi / 2) || d >= best) continue;
              best = d, bj = (int)j;
            }
          if (bj < 0) continue;
          if (pass == 0) {
            if (bj == truth) ost.ptp++;
            else ost.pfp++;
            continue;
          }
          if (ocls[i] == O_AMBIG) continue;
          ptk[bj] = 1;
          int dk = map[i] != VC_IP_NEW, dj = kept[bj] != 0 && !keep[kept[bj] - 1].half;
          if (dk == dj) {
            double sv120 = v120f, sv60 = v60f;
            v120f = v120f > 0, v60f = v60f > 0;
            snprintf(info, sizeof info, "proxy=%d off=%.1f live=%d kept=%d", bj, best, map[i], dj);
            ev(dk ? V_TWICE : V_NONE, kf, (int)i, p, &bk[i], bk[i].size, info);
            v120f = sv120, v60f = sv60;
          }
        }
      }
      /* the per-palette census (as scratch/ipform's who.c): sprite-like quads (4 vertices, at most 64 px, on the screen) by palette.
       * A palette with as many such lists in OLD as in KEY should be drawn that many times in the in-betweens (KEY's lists that do not
       * wait for KEY plus OLD's kept ones): fewer is P_NONE, more P_TWICE; a KEY list paired with an OLD list of another palette is
       * P_CROSS (a sprite blended from an effect's place, or the other way). Needs no identity: what the oracle misses in HG/SS. The
       * lists the tags decide (tdk / tdo) are left out: their identity is known, the oracle's V_ judges them, and a particle's palette
       * animation or its births and deaths at one place would read as P_CROSS / P_TWICE here (Gen 5's battles, 2026-11-26) */
      {
        int *sk = (int *)malloc(sizeof(int) * (key.nl + 1)), *so2 = (int *)malloc(sizeof(int) * (old.nl + 1)), nsk = 0, nso = 0;
        for (u32 i = 0; i < key.nl; i++)
          if (key.l[i].n == 4 && bk[i].size <= 64 && bk[i].vis && bk[i].front && !tdk[i]) sk[nsk++] = (int)i;
        /* OLD's on KEY's screen: its box moved by the motion around it (one thing crossing the screen's edge between the frames counts
         * in both or in neither, as the pair a sprite at the edge and its partner just off it did in Gen 5's grass, 2026-11-26) */
        for (u32 j = 0; j < old.nl; j++) {
          if (old.l[j].n != 4 || bo[j].size > 64 || !bo[j].front || tdo[j]) continue;
          double rx, ry;
          REF(bo[j].cx, bo[j].cy, -1, rx, ry);
          if (fabs(bo[j].cx + rx - 128) <= 128 + bo[j].hw && fabs(bo[j].cy + ry - 96) <= 96 + bo[j].hh) so2[nso++] = (int)j;
        }
        for (int a = 0; a < nsk; a++) {
          int i = sk[a], first = 1, ck = 0, co = 0, dr = 0;
          u32 pl = key.l[i].pltt;
          for (int b = 0; b < a && first; b++) first = key.l[sk[b]].pltt != pl;
          if (!first) continue;
          for (int b = a; b < nsk; b++)
            if (key.l[sk[b]].pltt == pl) ck++, dr += map[sk[b]] != VC_IP_NEW;
          for (int b = 0; b < nso; b++)
            if (old.l[so2[b]].pltt == pl) co++, dr += kept[so2[b]] != 0 && !keep[kept[so2[b]] - 1].half;
          if (ck == co && dr != ck) {
            double sv120 = v120f, sv60 = v60f;
            v120f = v120f > 0, v60f = v60f > 0;
            snprintf(info, sizeof info, "pltt=%04x lists=%d drawn=%d", pl, ck, dr);
            ev(dr < ck ? P_NONE : P_TWICE, kf, i, &key.l[i], &bk[i], bk[i].size, info);
            v120f = sv120, v60f = sv60;
          }
        }
        for (int a = 0; a < nsk; a++) {
          int i = sk[a];
          if (map[i] < 0 || old.l[map[i]].pltt == key.l[i].pltt || old.l[map[i]].n != 4 || bo[map[i]].size > 64) continue;
          snprintf(info, sizeof info, "pltt=%04x partner=%d its=%04x", key.l[i].pltt, map[i], old.l[map[i]].pltt);
          ev(P_CROSS, kf, i, &key.l[i], &bk[i], hypot(mx[i], my[i]), info);
        }
        free(sk), free(so2);
      }
      /* (M4) a pair switching to OLD's texture state at the middle draws it in KEY's VRAM too */
      for (u32 i = 0; i < key.nl; i++) {
        if (map[i] < 0 || !(th[i] & VC_IP_HOLD_STATE) || !tex_changed(vto, vpo, vtk, vpk, &old.l[map[i]])) continue;
        double sv120 = v120f, sv60 = v60f;
        v120f = v120f > 0, v60f = v60f > 0;
        snprintf(info, sizeof info, "state tex=%08x pltt=%04x", old.l[map[i]].texparam, old.l[map[i]].pltt);
        ev(V_STALE, kf, (int)i, &key.l[i], &bk[i], bk[i].size, info);
        v120f = sv120, v60f = sv60;
      }
      /* a kept list is drawn with OLD's texture state in KEY's VRAM: wrong where those bytes changed */
      for (int k = 0; k < nkeep; k++) {
        int j = keep[k].list;
        if (!tex_changed(vto, vpo, vtk, vpk, &old.l[j])) continue;
        double sv120 = v120f, sv60 = v60f;
        v120f = v120f > 0, v60f = v60f > 0;
        snprintf(info, sizeof info, "kept tex=%08x pltt=%04x", old.l[j].texparam, old.l[j].pltt);
        ev(V_STALE, kf, j, &old.l[j], &bo[j], bo[j].size, info);
        v120f = sv120, v60f = sv60;
      }
    }
    ngone[1] = 0;
    for (u32 j = 0; j < old.nl; j++) {
      if (mb[j] >= 0 || !bo[j].vis || !bo[j].front) continue;
      if (bo[j].cx < 0 || bo[j].cx > 256 || bo[j].cy < 0 || bo[j].cy > 192) continue; /* mostly off the screen */
      if (kept[j] && keep[kept[j] - 1].win == VC_IP_LEAVING) { /* leaving the screen with the world (vc_ip_windows) */
        double ex, ey, rx, ry;
        motion_m(&old, &old.l[j], keep[kept[j] - 1].m, 1, &ex, &ey);
        REF(bo[j].cx + ex, bo[j].cy + ey, -1, rx, ry);
        snprintf(info, sizeof info, "leave drawn=%.1f,%.1f ref=%.1f,%.1f nl=%u/%u", ex, ey, rx, ry, key.nl, old.nl);
        ev(C_EDGE, kf, (int)j, &old.l[j], &bo[j], hypot(ex - rx, ey - ry), info);
      } else if (kept[j]) { /* drawn until KEY (vc_ip_keep): not gone */
        const VC_IpKeep *k = &keep[kept[j] - 1];
        snprintf(info, sizeof info, "motion=%.1f,%.1f after=%d nl=%u/%u%s", k->dx, k->dy, k->after, key.nl, old.nl, k->half ? " half" : "");
        ev(C_KEPT, kf, (int)j, &old.l[j], &bo[j], hypot(k->dx, k->dy), info);
      } else {
        snprintf(info, sizeof info, "nl=%u/%u", key.nl, old.nl);
        double sv120 = v120f, sv60 = v60f; /* gone for the whole interval: its size, in every in-between */
        v120f = v120f > 0;
        v60f = v60f > 0;
        ev(C_VANISH, kf, (int)j, &old.l[j], &bo[j], bo[j].size, info);
        v120f = sv120;
        v60f = sv60;
      }
      if (ngone[1] == capgone) {
        capgone = capgone * 2 + 64;
        gone[0] = (Gone *)realloc(gone[0], sizeof(Gone) * (size_t)capgone);
        gone[1] = (Gone *)realloc(gone[1], sizeof(Gone) * (size_t)capgone);
      }
      gone[1][ngone[1]++] = (Gone){keyx(&old.l[j]), bo[j].cx, bo[j].cy};
    }
    { /* this pair's vanished lists for the next pair's C_BLINK */
      Gone *t = gone[0];
      gone[0] = gone[1], gone[1] = t, ngone[0] = ngone[1], gonegx = gx, gonegy = gy;
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
    free(smx), free(smy), free(ofar), free(lgood), free(lx), free(ly);
    free(tx);
    free(ty);
  }
  if (timing && ntus) {
    qsort(tus, (size_t)ntus, sizeof *tus, cmpd);
    printf("TIME pairs %d median %.1f p95 %.1f max %.1f us\n", ntus, tus[ntus / 2], tus[(int)(ntus * 0.95)], tus[ntus - 1]);
  }
  printf("STAT keyframes %llu repeats %llu pairs %llu cuts %llu scene %llu unpaired_max %d lists %llu paired %.2f%% gaps1 %llu gaps2 %llu gaps3+ %llu moving %llu campath %.0f\n",
         nkey, nrep, npair, ncut, nscene, unp_max, lists, lists ? 100.0 * lpaired / lists : 0, gaps[1], gaps[2], gaps[3], nmove, campath);
  if (oracle) {
    unsigned long long n = 0;
    for (int c = 1; c < NOCLS; c++) n += c == O_EMPTY ? 0 : ost.cls[c];
    printf("ORACLE lists %llu", n);
    for (int c = 1; c < NOCLS; c++)
      if (c != O_TAG || otagged) printf(" %s %llu", ocls_name[c], ost.cls[c]);
    if (otagged) printf(" tagpairs %llu", otagged);
    printf(" twod %llu", ost.twod);
    printf(" gone %llu wfar %llu far %llu ds_per_pair %.1f disagree %llu missed %llu dres_rot %.1f dres_trans %.1f proxy_tp %llu proxy_fp %llu\n", ost.gone,
           ost.wfar, ost.far, npair + ncut ? (double)ost.ds / (double)(npair + ncut) : 0, ost.disagree, ost.missed, ost.rmax, ost.tmax, ost.ptp, ost.pfp);
    for (int w = 0; w < NHOW; w++)
      printf("ORACLEP %s pairs %llu right %llu wrong %llu unknown %llu useen %llu uoff %llu oracle %llu\n", wname[w], wp[w][0], wp[w][1], wp[w][2],
             wp[w][3], wp[w][4], wp[w][5], wp[w][6]);
  }
  for (int c = 0; c < (oracle ? NCLS : E_CUT + 1); c++)
    if ((c != C_EDGE && c != B_WRAP) || sum[c].n) /* (none with BWPORT_INTERP_WINDOWS=0, or no wrap blended: the output as before) */
      printf("SUM %s %llu %llu %llu %.2f %u %d\n", cls_name[c], sum[c].n, sum[c].v1, sum[c].v3, sum[c].maxv, sum[c].frame, sum[c].list);
  return 0;
}
