/* port/vcore p23: in-between 3D frames (docs/interp.md). See p23_interp.h. */
#include "vc_shared.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

/* ---- recording and blending, called by the geometry engine (p14) ---- */

void vc_ip_reset(VCoreState *s) {
  VC_P23 *p = &s->p23;
  p->list = -1;
  p->k = 0;
  p->open = 0;
  if (p->mode == VC_IP_RECORD) {
    p->rec.nv = 0;
    p->rec.nl = 0;
    p->rec.swapped = 0;
    p->rec.npoly = 0;
  }
}

void vc_ip_end(VCoreState *s) { s->p23.open = 0; }

void vc_ip_begin(VCoreState *s) {
  VC_P23 *p = &s->p23;
  p->list++;
  p->k = 0;
  p->open = 1;
  if (p->mode != VC_IP_RECORD) return;
  VC_IpRec *r = &p->rec;
  if (r->nl == r->capl) {
    r->capl = r->capl ? r->capl * 2 : 256;
    r->l = (VC_IpList *)realloc(r->l, r->capl * sizeof *r->l);
  }
  VC_IpList *l = &r->l[r->nl++];
  memset(l, 0, sizeof *l);
  l->first = r->nv;
  l->attr = s->p14.attr;
  l->texparam = s->p14.texparam;
  l->pltt = s->p14.pltt;
  l->viewport = (u32)s->p14.vp_x1 | (u32)s->p14.vp_y1 << 8 | (u32)s->p14.vp_x2 << 16 | (u32)s->p14.vp_y2 << 24;
  l->type = (u8)s->p14.type;
}

static s32 lerp_s32(s32 a, s32 b, s32 f) { return (s32)(a + (((s64)b - a) * f >> 12)); }

void vc_ip_vertex(VCoreState *s, VC_Vertex *v) {
  VC_P23 *p = &s->p23;
  if (!p->open) return;
  if (p->mode == VC_IP_RECORD) {
    VC_IpRec *r = &p->rec;
    if (r->nv == r->capv) {
      r->capv = r->capv ? r->capv * 2 : 4096;
      r->v = (VC_IpVtx *)realloc(r->v, r->capv * sizeof *r->v);
    }
    VC_IpVtx *o = &r->v[r->nv++];
    for (int i = 0; i < 4; i++) o->clip[i] = v->clip[i];
    for (int i = 0; i < 3; i++) o->color[i] = v->color[i];
    o->tex[0] = v->tex[0];
    o->tex[1] = v->tex[1];
    VC_IpList *l = &r->l[r->nl - 1];
    l->n++;
    if (v->clip[3] <= 0) l->behind = 1;
  } else if (p->mode == VC_IP_APPLY) {
    vc_ip_blend_list(s, v, v, 1);
  }
}

int vc_ip_blend_list(VCoreState *s, const VC_Vertex *in, VC_Vertex *out, u32 n) {
  VC_P23 *p = &s->p23;
  if (out != in) memcpy(out, in, n * sizeof *out);
  if (!p->open || p->mode != VC_IP_APPLY) return 1;
  u32 k = p->k;
  p->k += n;
  if (p->list < 0 || p->map == NULL || p->map[p->list] < 0) return 1;
  const VC_IpList *dl = &p->dst->l[p->map[p->list]];
  if (k >= dl->n) return 1;
  if (n > dl->n - k) n = dl->n - k;
  const VC_IpVtx *d = &p->dst->v[dl->first + k];
  const s32 f = p->f;
  const int hold = p->texhold != NULL ? p->texhold[p->list] : 0, take = f >= 2048; /* bit 0: s held, bit 1: t held */
  int same = 1;
  for (u32 j = 0; j < n; j++, d++) {
    VC_Vertex *v = &out[j];
    if (v->clip[0] == d->clip[0] && v->clip[1] == d->clip[1] && v->clip[2] == d->clip[2] && v->clip[3] == d->clip[3] &&
        v->color[0] == d->color[0] && v->color[1] == d->color[1] && v->color[2] == d->color[2] && v->tex[0] == d->tex[0] &&
        v->tex[1] == d->tex[1])
      continue; /* the vertex it moves toward is itself: every blend below leaves it (lerp_s32(a, a, f) = a) */
    VC_Vertex o = *v;
    for (int i = 0; i < 4; i++) v->clip[i] = lerp_s32(v->clip[i], d->clip[i], f);
    for (int i = 0; i < 3; i++) v->color[i] = (u8)lerp_s32(v->color[i], d->color[i], f);
    for (int i = 0; i < 2; i++) {
      int dt = d->tex[i] - v->tex[i];
      if (hold >> i & 1) {
        if (take) v->tex[i] = d->tex[i];
      } else if (dt > -2048 && dt < 2048)
        v->tex[i] = (s16)lerp_s32(v->tex[i], d->tex[i], f);
    }
    same &= v->clip[0] == o.clip[0] && v->clip[1] == o.clip[1] && v->clip[2] == o.clip[2] && v->clip[3] == o.clip[3] &&
            v->color[0] == o.color[0] && v->color[1] == o.color[1] && v->color[2] == o.color[2] && v->tex[0] == o.tex[0] &&
            v->tex[1] == o.tex[1];
  }
  return same;
}

void vc_ip_free(VC_IpRec *r) {
  free(r->v);
  free(r->l);
  memset(r, 0, sizeof *r);
}

void vc_ip_take(VCoreState *s, VC_IpRec *into) {
  vc_ip_free(into);
  *into = s->p23.rec;
  memset(&s->p23.rec, 0, sizeof s->p23.rec);
}

static void tape_list_end(VCoreState *s) {
  VC_IpTape *t = &s->p23.tape;
  if (!t->open) return;
  VC_IpTapeList *l = &t->l[t->nl - 1];
  l->op1 = t->nop;
  l->v1 = t->nv;
  l->p1 = s->geo.npoly;
  l->r1 = s->geo.nvtx;
  t->open = 0;
}

void vc_ip_tape_op(VCoreState *s, int op, int b, u32 a) {
  VC_IpTape *t = &s->p23.tape;
  if (op == VC_TAPE_BEGIN || op == VC_TAPE_END) tape_list_end(s);
  if (op == VC_TAPE_SWAP && t->open) t->l[t->nl - 1].swap = 1;
  if (op == VC_TAPE_BEGIN) {
    if (t->nl == t->capl) {
      t->capl = t->capl ? t->capl * 2 : 256;
      t->l = (VC_IpTapeList *)realloc(t->l, t->capl * sizeof *t->l);
    }
    VC_IpTapeList *l = &t->l[t->nl++];
    memset(l, 0, sizeof *l);
    l->op0 = t->nop;
    l->v0 = t->nv;
    l->p0 = s->geo.npoly;
    l->r0 = s->geo.nvtx;
    t->open = 1;
  }
  if (t->nop == t->capop) {
    t->capop = t->capop ? t->capop * 2 : 4096;
    t->op = (VC_IpTapeOp *)realloc(t->op, t->capop * sizeof *t->op);
  }
  VC_IpTapeOp *o = &t->op[t->nop++];
  o->op = (u8)op;
  o->b = (u8)b;
  o->a = a;
}

void vc_ip_tape_vtx(VCoreState *s, const VC_Vertex *v) {
  VC_IpTape *t = &s->p23.tape;
  if (t->nv == t->capv) {
    t->capv = t->capv ? t->capv * 2 : 4096;
    t->v = (VC_Vertex *)realloc(t->v, t->capv * sizeof *t->v);
    t->bv = (VC_Vertex *)realloc(t->bv, t->capv * sizeof *t->bv);
  }
  t->v[t->nv++] = *v;
  vc_ip_tape_op(s, VC_TAPE_VTX, 0, 0);
}

void vc_ip_tape_free(VCoreState *s) {
  free(s->p23.tape.op);
  free(s->p23.tape.v);
  free(s->p23.tape.bv);
  free(s->p23.tape.l);
  free(s->p23.tape.gp);
  free(s->p23.tape.gv);
  memset(&s->p23.tape, 0, sizeof s->p23.tape);
}

void vc_ip_record(VCoreState *s) {
  int saved = s->p23.mode;
  s->p23.mode = VC_IP_RECORD;
  s->p23.tape.nop = s->p23.tape.nv = s->p23.tape.nl = 0;
  s->p23.tape.ok = s->p23.tape.open = 0;
  vc_mtx_reset(s);
  vc_poly_reset(s);
  vc_light_reset(s);
  u32 i = 0;
  while (i < s->gx_words) {
    u32 h = s->gx[i++];
    u32 op = h & 0xFF, n = (h >> 8) & 0xFF;
    if (n > s->gx_words - i) n = s->gx_words - i;
    vc_gx_command(s, op, s->gx + i, n);
    i += n;
  }
  s->p23.rec.swapped = s->geo.swapped != 0;
  s->p23.rec.npoly = s->geo.npoly;
  VC_IpTape *t = &s->p23.tape;
  tape_list_end(s);
  t->gnp = s->geo.npoly < VC_MAX_POLYS ? s->geo.npoly : VC_MAX_POLYS;
  t->gnv = s->geo.nvtx < VC_MAX_VERTS ? s->geo.nvtx : VC_MAX_VERTS;
  if (t->gnp > t->capgp) t->capgp = t->gnp * 2, t->gp = (VC_Poly *)realloc(t->gp, t->capgp * sizeof *t->gp);
  if (t->gnv > t->capgv) t->capgv = t->gnv * 2, t->gv = (VC_Vertex *)realloc(t->gv, t->capgv * sizeof *t->gv);
  if (t->gnp) memcpy(t->gp, s->geo.poly, t->gnp * sizeof *t->gp);
  if (t->gnv) memcpy(t->gv, s->geo.vtx, t->gnv * sizeof *t->gv);
  t->gok = !s->geo.overflow;
  memcpy(s->p23.tape.cur, s->p14.cur, sizeof s->p14.cur);
  memcpy(s->p23.tape.pos_result, s->p14.pos_result, sizeof s->p14.pos_result);
  s->p23.tape.ok = 1;
  s->p23.mode = saved;
}

void vc_ip_apply_begin(VCoreState *s, const VC_IpRec *b, const int *map, const u8 *texhold, s32 f) {
  s->p23.mode = VC_IP_APPLY;
  s->p23.dst = b;
  s->p23.map = map;
  s->p23.texhold = texhold;
  s->p23.f = f;
}

void vc_ip_apply_end(VCoreState *s) {
  s->p23.mode = VC_IP_OFF;
  s->p23.dst = NULL;
  s->p23.map = NULL;
  s->p23.texhold = NULL;
}

/* the list's size on screen (256 wide pixels): the larger side of its bounding box; a vertex behind the camera = huge */
static double list_extent(const VC_IpRec *r, const VC_IpList *l) {
  double x0 = 1e30, x1 = -1e30, y0 = 1e30, y1 = -1e30;
  for (u32 j = 0; j < l->n; j++) {
    const VC_IpVtx *v = &r->v[l->first + j];
    if (v->clip[3] <= 0) return 1e30;
    double x = 128.0 * v->clip[0] / v->clip[3], y = 96.0 * v->clip[1] / v->clip[3];
    if (x < x0) x0 = x;
    if (x > x1) x1 = x;
    if (y < y0) y0 = y;
    if (y > y1) y1 = y;
  }
  return l->n ? (x1 - x0 > y1 - y0 ? x1 - x0 : y1 - y0) : 0;
}

void vc_ip_texhold(const VC_IpRec *a, const VC_IpRec *b, const int *map, u8 *texhold, int thr, double maxpx, const int *pstep,
                   int *nstep) {
  for (u32 i = 0; i < a->nl; i++) {
    texhold[i] = 0;
    if (map[i] < 0) continue;
    const VC_IpList *p = &a->l[i], *q = &b->l[map[i]];
    if (list_extent(a, p) > maxpx || list_extent(b, q) > maxpx) {
      /* a big surface that scrolls keeps its blended scroll, but an axis whose texcoords all jump by one whole multiple of 8 texels
       * steps a cell of an atlas (Black's Route 1 leaves: 128x64 strips scroll s by ~2 texels a keyframe and step t by 16 to the next
       * row, -64 back to the first): that axis alone is held, the other still blends */
      if (thr <= 0 || p->n != q->n || p->n == 0) continue;
      for (int c = 0; c < 2; c++) {
        int d0 = a->v[p->first].tex[c] - b->v[q->first].tex[c], one = 1;
        for (u32 j = 1; j < p->n && one; j++) one = a->v[p->first + j].tex[c] - b->v[q->first + j].tex[c] == d0;
        if (one && d0 != 0 && (d0 >= thr || d0 <= -thr) && d0 % 128 == 0) {
          /* the same step as this list's in the pair before = a steady scroll (HeartGold's water: 8 texels every keyframe): blended */
          if (nstep) nstep[2 * i + c] = d0;
          if (!pstep || pstep[2 * map[i] + c] != d0) texhold[i] |= (u8)(1 << c);
        }
      }
      continue;
    }
    for (u32 j = 0; j < p->n && j < q->n && !texhold[i]; j++)
      for (int c = 0; c < 2; c++) {
        int d = a->v[p->first + j].tex[c] - b->v[q->first + j].tex[c];
        if (d >= thr || d <= -thr) texhold[i] = 3;
      }
  }
}

/* ---- pairing two frames' lists ---- */

static u32 mix(u32 h, u32 x) { return (h ^ x) * 16777619u; }
static u32 key_of(const VC_IpList *l) {
  u32 h = 2166136261u;
  h = mix(h, l->attr & ~0x001F0000u); /* the alpha may fade */
  h = mix(h, l->texparam);
  h = mix(h, l->pltt);
  h = mix(h, l->viewport);
  h = mix(h, l->type);
  return mix(h, l->n);
}
/* a run key: key_of without the texture, for runs of particles whose texture cell steps with their age (a footprint's puff
 * draws a new cell every few frames, so neighbours in the run have different keys) */
static u32 rkey_of(const VC_IpList *l) {
  u32 h = 2166136261u;
  h = mix(h, l->attr & ~0x001F0000u);
  h = mix(h, l->pltt);
  h = mix(h, l->viewport);
  h = mix(h, l->type);
  return mix(h, l->n);
}
static u32 shape_of(const VC_IpList *l) { return mix(mix(2166136261u, l->type), l->n); }
static double clampd(double x, double m) { return x < -m ? -m : x > m ? m : x; }

/* the mean screen distance (px, 256 wide) between two lists' vertices taken in order, measured as the displacement filter in
 * vc_ip_pair does; -1 when no vertex is in front of the camera in both */
static double list_disp(const VC_IpRec *a, const VC_IpList *p, const VC_IpRec *b, const VC_IpList *q) {
  double s = 0;
  int nd = 0;
  for (u32 k = 0; k < p->n && k < q->n; k++) {
    const VC_IpVtx *u = &a->v[p->first + k], *w = &b->v[q->first + k];
    if (u->clip[3] <= 0 || w->clip[3] <= 0) continue;
    double ax = clampd(128.0 * u->clip[0] / u->clip[3], 256), ay = clampd(96.0 * u->clip[1] / u->clip[3], 192);
    double bx = clampd(128.0 * w->clip[0] / w->clip[3], 256), by = clampd(96.0 * w->clip[1] / w->clip[3], 192);
    s += sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
    nd++;
  }
  return nd ? s / nd : -1;
}

int vc_ip_reorder = -1;
#define IP_REORDER_PX 16.0 /* a reordered list may be this far (px, 256 wide) from where the camera motion puts it */
#define IP_REORDER_MAXCAND 250000L

/* the mean screen motion (px, 256 wide) of a's list p to b's list q, vertices in order; 0 when no vertex is in front in both */
static int list_shift(const VC_IpRec *a, const VC_IpList *p, const VC_IpRec *b, const VC_IpList *q, double *dx, double *dy) {
  double sx = 0, sy = 0;
  int nd = 0;
  for (u32 k = 0; k < p->n && k < q->n; k++) {
    const VC_IpVtx *u = &a->v[p->first + k], *w = &b->v[q->first + k];
    if (u->clip[3] <= 0 || w->clip[3] <= 0) continue;
    sx += clampd(128.0 * w->clip[0] / w->clip[3], 256) - clampd(128.0 * u->clip[0] / u->clip[3], 256);
    sy += clampd(96.0 * w->clip[1] / w->clip[3], 192) - clampd(96.0 * u->clip[1] / u->clip[3], 192);
    nd++;
  }
  if (!nd) return 0;
  *dx = sx / nd;
  *dy = sy / nd;
  return 1;
}

typedef struct ReCand {
  double err;
  int i, j;
} ReCand;
static int recand_cmp(const void *x, const void *y) {
  const ReCand *p = (const ReCand *)x, *q = (const ReCand *)y;
  return p->err < q->err ? -1 : p->err > q->err ? 1 : p->i != q->i ? p->i - q->i : p->j - q->j;
}
static int dbl_cmp(const void *x, const void *y) {
  double p = *(const double *)x, q = *(const double *)y;
  return p < q ? -1 : p > q;
}

/* The lists neither the order passes nor the run re-pairing paired, taken by key and position: a draw order that changes between the
 * keyframes (translucent / depth-sorted lists) leaves the same lists in another order, and the LCS pairs only the longest in-order
 * part. The camera motion is the median shift of the lists already paired (the map pieces); a key's lists pair nearest first, a list
 * further than IP_REORDER_PX from where that motion puts it stays alone. Returns the pairs made. */
static int pair_reordered(const VC_IpRec *a, const VC_IpRec *b, const u32 *ka, const u32 *kb, int *map, int *mb) {
  const int na = (int)a->nl, nb = (int)b->nl;
  int ua = 0, ub = 0, np = 0;
  for (int i = 0; i < na; i++) ua += map[i] < 0;
  for (int j = 0; j < nb; j++) ub += mb[j] < 0;
  if (!ua || !ub || (long)ua * ub > IP_REORDER_MAXCAND) return 0;
  double *sx = (double *)malloc(sizeof(double) * 2 * (size_t)(na + 1)), *sy = sx + na + 1;
  int ns = 0;
  for (int i = 0; i < na; i++) {
    double dx, dy;
    if (map[i] >= 0 && list_shift(a, &a->l[i], b, &b->l[map[i]], &dx, &dy)) {
      sx[ns] = dx;
      sy[ns++] = dy;
    }
  }
  double cx = 0, cy = 0;
  if (ns >= 3) {
    qsort(sx, (size_t)ns, sizeof(double), dbl_cmp);
    qsort(sy, (size_t)ns, sizeof(double), dbl_cmp);
    cx = sx[ns / 2];
    cy = sy[ns / 2];
  }
  free(sx);
  ReCand *c = (ReCand *)malloc(sizeof(ReCand) * (size_t)ua * (size_t)ub);
  int nc = 0;
  for (int i = 0; i < na; i++) {
    if (map[i] >= 0) continue;
    for (int j = 0; j < nb; j++) {
      double dx, dy;
      if (mb[j] >= 0 || ka[i] != kb[j] || !list_shift(a, &a->l[i], b, &b->l[j], &dx, &dy)) continue;
      double e = sqrt((dx - cx) * (dx - cx) + (dy - cy) * (dy - cy));
      if (e <= IP_REORDER_PX) {
        c[nc].err = e;
        c[nc].i = i;
        c[nc++].j = j;
      }
    }
  }
  qsort(c, (size_t)nc, sizeof(ReCand), recand_cmp);
  for (int k = 0; k < nc; k++)
    if (map[c[k].i] < 0 && mb[c[k].j] < 0) {
      map[c[k].i] = c[k].j;
      mb[c[k].j] = c[k].i;
      np++;
    }
  free(c);
  /* lists with no vertex in front of the camera in both frames have no position to compare (a battle's 3D scene draws hundreds of
   * them, nothing of them on the screen): same key, in draw order */
  for (int i = 0; i < na; i++) {
    if (map[i] >= 0) continue;
    for (int j = 0; j < nb; j++) {
      double dx, dy;
      if (mb[j] >= 0 || ka[i] != kb[j] || list_shift(a, &a->l[i], b, &b->l[j], &dx, &dy)) continue;
      map[i] = j;
      mb[j] = i;
      np++;
      break;
    }
  }
  return np;
}

int vc_ip_scene_cut(const int *map, int nl, int *unpaired) {
  int n = 0;
  for (int i = 0; i < nl; i++) n += map[i] < 0;
  if (unpaired != NULL) *unpaired = n;
  return n >= 30 && n * 12 >= nl;
}

int vc_ip_repair_runs = -1;
#define IP_RUN_MAXPX 64.0 /* the biggest list (screen px, 256 wide) of a particle run */

/* (The run key is rkey_of, the key without the texture: a particle's texture cell steps with its age. A list left alone costs
 * min(half the limit, half its size): a 32 px puff is not worth a 24 px detour.)
 * A run of consecutive lists with one key is a particle emitter (lib/spl draws each particle as its own 4-vertex quad, the newest
 * first, spl_list.c): a particle born or dead between the keyframes shifts the order, and pairing by order (the passes above) pairs
 * every particle with its neighbour, so in the in-betweens a particle would sweep toward another's place. (Route 1's falling leaves
 * are not such a run: they are four static 12-vertex quad strips whose texture image changes; vcore_ipscan found no mispaired run.)
 * Inside such a run (a's run and the run of b's lists with the same key its partners lie in) the pairing is made again as the
 * order-keeping alignment of least cost: a pair costs its displacement (never more than the limit), a list left alone half the
 * limit (a pair at the limit = two lists left alone). Taken only when it costs less than the pairing by order; a run of map pieces
 * that all move with the camera keeps its pairing (it is already the cheapest). Returns how many of a's lists changed partner. */
static int repair_runs(const VC_IpRec *a, const VC_IpRec *b, const u32 *ka, const u32 *kb, int *map, int *mb, double limit) {
  const int na = (int)a->nl, nb = (int)b->nl;
  const double G = limit * 0.5;
  int changed = 0;
  for (int i0 = 0, i1; i0 < na; i0 = i1) {
    const u32 key = ka[i0];
    for (i1 = i0 + 1; i1 < na && ka[i1] == key; i1++) {
    }
    int j0 = nb, j1 = -1, ok = 1;
    for (int i = i0; i < i1 && ok; i++) {
      if (map[i] < 0) continue;
      if (kb[map[i]] != key) ok = 0;
      if (map[i] < j0) j0 = map[i];
      if (map[i] + 1 > j1) j1 = map[i] + 1;
    }
    if (!ok || j1 < 0) continue;
    while (j0 > 0 && kb[j0 - 1] == key && mb[j0 - 1] < 0) j0--;
    while (j1 < nb && kb[j1] == key && mb[j1] < 0) j1++;
    for (int j = j0; j < j1 && ok; j++)
      if (kb[j] != key || (mb[j] >= 0 && (mb[j] < i0 || mb[j] >= i1))) ok = 0;
    const int m = i1 - i0, n = j1 - j0;
    if (!ok || m + n < 3 || (long)m * n > 65536) continue;
    /* a run of particles is small quads: a run of big lists (Black's battle-intro swirl: nine 34-vertex rings 150-350 px big, each
     * ring one step from the next) is a scene whose order is the pairing, and re-pairing it blended a ring with its neighbour (a gray
     * band in an in-between that is in neither keyframe) */
    for (int i = 0; i < m && ok; i++) ok = list_extent(a, &a->l[i0 + i]) <= IP_RUN_MAXPX;
    for (int j = 0; j < n && ok; j++) ok = list_extent(b, &b->l[j0 + j]) <= IP_RUN_MAXPX;
    if (!ok) continue;
    double *d = (double *)malloc(sizeof(double) * (size_t)m * (size_t)n);
    double *c = (double *)malloc(sizeof(double) * (size_t)(m + 1) * (size_t)(n + 1));
    double *ga = (double *)malloc(sizeof(double) * (size_t)(m + n + 2)), *gb = ga + m + 1; /* what leaving each list alone costs */
    for (int i = 0; i < m; i++) ga[i] = fmin(G, 0.5 * list_extent(a, &a->l[i0 + i]));
    for (int j = 0; j < n; j++) gb[j] = fmin(G, 0.5 * list_extent(b, &b->l[j0 + j]));
    const size_t w = (size_t)n + 1;
    for (int i = 0; i < m; i++)
      for (int j = 0; j < n; j++) {
        double x = list_disp(a, &a->l[i0 + i], b, &b->l[j0 + j]);
        d[i * n + j] = x < 0 || x > limit ? -1 : x;
      }
    double now = 0; /* the pairing by order: a pair beyond the limit is dropped later (two lists alone) */
    for (int i = 0; i < m; i++) {
      int j = map[i0 + i];
      now += j < 0 ? ga[i] : d[i * n + (j - j0)] < 0 ? ga[i] + gb[j - j0] : d[i * n + (j - j0)];
    }
    for (int j = 0; j < n; j++) now += mb[j0 + j] < 0 ? gb[j] : 0;
    for (int i = m; i >= 0; i--)
      for (int j = n; j >= 0; j--) {
        double v;
        if (i == m && j == n) v = 0;
        else if (i == m) v = c[i * w + j + 1] + gb[j];
        else if (j == n) v = c[(i + 1) * w + j] + ga[i];
        else {
          v = c[(i + 1) * w + j] + ga[i];
          if (c[i * w + j + 1] + gb[j] < v) v = c[i * w + j + 1] + gb[j];
          if (d[i * n + j] >= 0 && c[(i + 1) * w + j + 1] + d[i * n + j] < v) v = c[(i + 1) * w + j + 1] + d[i * n + j];
        }
        c[i * w + j] = v;
      }
    if (c[0] < now - 1.0) {
      int *old = (int *)malloc(sizeof(int) * (size_t)m);
      for (int i = 0; i < m; i++) old[i] = map[i0 + i], map[i0 + i] = -1;
      for (int j = 0; j < n; j++) mb[j0 + j] = -1;
      int i = 0, j = 0;
      while (i < m && j < n) {
        double here = c[i * w + j];
        if (d[i * n + j] >= 0 && c[(i + 1) * w + j + 1] + d[i * n + j] <= here + 1e-9) {
          map[i0 + i] = j0 + j;
          mb[j0 + j] = i0 + i;
          i++;
          j++;
        } else if (c[(i + 1) * w + j] + ga[i] <= here + 1e-9) {
          i++;
        } else {
          j++;
        }
      }
      for (int k = 0; k < m; k++) changed += map[i0 + k] != old[k];
      free(old);
    }
    free(d);
    free(c);
    free(ga);
  }
  return changed;
}

void vc_ip_pair(const VC_IpRec *a, const VC_IpRec *b, int *map, double limit, VC_IpStats *st) {
  memset(st, 0, sizeof *st);
  int na = (int)a->nl, nb = (int)b->nl;
  st->nl_a = na;
  st->nl_b = nb;
  st->nv_a = a->nv;
  st->nv_b = b->nv;
  st->swapped_a = (int)a->swapped;
  st->swapped_b = (int)b->swapped;
  for (int i = 0; i < na; i++) map[i] = -1;
  if (na == 0 || nb == 0) {
    st->removed = na;
    st->added = nb;
    st->vt_removed = a->nv;
    st->vt_added = b->nv;
    return;
  }
  u32 *ka = (u32 *)malloc(sizeof(u32) * (size_t)na), *kb = (u32 *)malloc(sizeof(u32) * (size_t)nb);
  for (int i = 0; i < na; i++) ka[i] = key_of(&a->l[i]);
  for (int i = 0; i < nb; i++) kb[i] = key_of(&b->l[i]);
  int *mb = (int *)malloc(sizeof(int) * (size_t)nb); /* b's list -> a's list */
  for (int i = 0; i < nb; i++) mb[i] = -1;
  int pre = 0, suf = 0;
  while (pre < na && pre < nb && ka[pre] == kb[pre]) {
    map[pre] = pre;
    mb[pre] = pre;
    pre++;
  }
  while (suf < na - pre && suf < nb - pre && ka[na - 1 - suf] == kb[nb - 1 - suf]) {
    map[na - 1 - suf] = nb - 1 - suf;
    mb[nb - 1 - suf] = na - 1 - suf;
    suf++;
  }
  int ma = na - pre - suf, mbn = nb - pre - suf;
  if (ma > 0 && mbn > 0 && (long)ma * mbn <= 4000000L) {
    size_t w = (size_t)mbn + 1;
    u16 *dp = (u16 *)calloc((size_t)(ma + 1) * w, sizeof(u16));
    for (int i = ma - 1; i >= 0; i--)
      for (int j = mbn - 1; j >= 0; j--) {
        if (ka[pre + i] == kb[pre + j]) dp[i * w + j] = (u16)(dp[(i + 1) * w + j + 1] + 1);
        else dp[i * w + j] = dp[(i + 1) * w + j] > dp[i * w + j + 1] ? dp[(i + 1) * w + j] : dp[i * w + j + 1];
      }
    int i = 0, j = 0;
    while (i < ma && j < mbn) {
      if (ka[pre + i] == kb[pre + j]) {
        map[pre + i] = pre + j;
        mb[pre + j] = pre + i;
        i++;
        j++;
      } else if (dp[(i + 1) * w + j] >= dp[i * w + j + 1]) {
        i++;
      } else {
        j++;
      }
    }
    free(dp);
  }
  /* second pass, inside the gaps between strict pairs: the same shape with another texture, palette or alpha */
  int ia = 0, ib = 0;
  int strict = 0;
  for (int i = 0; i < na; i++) strict += map[i] >= 0;
  st->strict = strict;
  while (ia <= na) {
    /* the gap ends at the next strict pair (or the end) */
    int ea = ia;
    while (ea < na && map[ea] < 0) ea++;
    int eb = ea < na ? map[ea] : nb;
    int cb = ib;
    for (int x = ia; x < ea; x++) {
      for (int y = cb; y < eb; y++) {
        if (shape_of(&a->l[x]) == shape_of(&b->l[y])) {
          map[x] = y;
          mb[y] = x;
          cb = y + 1;
          st->loose++;
          const VC_IpList *p = &a->l[x], *q = &b->l[y];
          if ((p->attr & ~0x001F0000u) != (q->attr & ~0x001F0000u)) st->ch_attr++;
          else if ((p->attr ^ q->attr) & 0x001F0000u) st->ch_alpha++;
          if (p->texparam != q->texparam) st->ch_tex++;
          if (p->pltt != q->pltt) st->ch_pltt++;
          if (p->viewport != q->viewport) st->ch_viewport++;
          break;
        }
      }
    }
    if (ea >= na) break;
    ia = ea + 1;
    ib = eb + 1;
  }
  if (vc_ip_repair_runs < 0) {
    const char *e = getenv("BWPORT_INTERP_REPAIR");
    /* on by default since the in-between sweep (docs/interp-sweep.md): HeartGold's sand footprints behind a player walking
     * east (a run of identical fading quads whose oldest dies) paired one tile off, 6 A_MISPAIR in-betweens of 12 px -> 0 */
    vc_ip_repair_runs = e == NULL || e[0] != '0';
  }
  if (vc_ip_repair_runs) {
    u32 *ra = (u32 *)malloc(sizeof(u32) * (size_t)na), *rb = (u32 *)malloc(sizeof(u32) * (size_t)nb);
    for (int i = 0; i < na; i++) ra[i] = rkey_of(&a->l[i]);
    for (int i = 0; i < nb; i++) rb[i] = rkey_of(&b->l[i]);
    st->repaired = repair_runs(a, b, ra, rb, map, mb, limit);
    free(ra);
    free(rb);
  }
  if (vc_ip_reorder < 0) {
    const char *e = getenv("BWPORT_INTERP_REORDER");
    vc_ip_reorder = e == NULL || e[0] != '0';
  }
  if (vc_ip_reorder) st->reordered = pair_reordered(a, b, ka, kb, map, mb);
  /* displacement of the paired lists; drop the ones that jump. A list's vertices are measured on the screen clamped to a band one
   * half screen beyond each edge, and a vertex behind the camera in either frame is left out: the map's ground pieces reach under
   * the camera, where a tiny w throws the projection thousands of pixels out or w crosses 0, and dropping such a piece drew it
   * unblended beside its blended neighbours, a line of the clear colour along their shared edge in the in-betweens while walking.
   * The blend itself is linear in clip space, so a vertex crossing w = 0 needs no guard: the clipper runs after it. */
  double dsum = 0;
  u64 dn = 0;
  int all_same = (na == nb);
  for (int i = 0; i < na; i++) {
    if (map[i] < 0) continue;
    const VC_IpList *p = &a->l[i], *q = &b->l[map[i]];
    double lsum = 0, lmax = 0;
    int ident = 1;
    int flip = 0, nd = 0;
    for (u32 k = 0; k < p->n; k++) {
      const VC_IpVtx *u = &a->v[p->first + k], *w = &b->v[q->first + k];
      if (memcmp(u, w, sizeof *u) != 0) ident = 0;
      if ((u->clip[3] <= 0) != (w->clip[3] <= 0)) flip = 1; /* crosses the camera plane between the frames */
      if (u->clip[3] <= 0 || w->clip[3] <= 0) continue;
      double ax = clampd(128.0 * u->clip[0] / u->clip[3], 256), ay = clampd(96.0 * u->clip[1] / u->clip[3], 192);
      double bx = clampd(128.0 * w->clip[0] / w->clip[3], 256), by = clampd(96.0 * w->clip[1] / w->clip[3], 192);
      double d = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
      lsum += d;
      nd++;
      if (d > lmax) lmax = d;
    }
    if (flip && nd == 0) { /* crosses the camera plane with no vertex in front in both frames: nothing measures its motion */
      map[i] = -1;
      st->behind++;
      all_same = 0;
      continue;
    }
    if (!ident) all_same = 0;
    double lmean = nd ? lsum / nd : 0;
    if (lmean > limit) {
      map[i] = -1;
      st->jumps++;
      all_same = 0;
      continue;
    }
    dsum += lsum;
    dn += p->n;
    if (lmax > st->disp_max) st->disp_max = lmax;
    st->vt_paired += p->n;
  }
  st->disp_mean = dn ? dsum / (double)dn : 0;
  int used = 0;
  for (int i = 0; i < na; i++) used += map[i] >= 0;
  /* removed / added: lists with no pairing before the displacement filter */
  int paired_all = st->strict + st->loose + st->reordered;
  st->removed = na - paired_all;
  st->added = nb - paired_all;
  u32 vr = 0, va = 0;
  for (int j = 0; j < nb; j++)
    if (mb[j] < 0) va += b->l[j].n;
  /* a's lists with no partner: those never paired in either pass */
  {
    char *pa = (char *)calloc((size_t)na, 1);
    for (int j = 0; j < nb; j++)
      if (mb[j] >= 0) pa[mb[j]] = 1;
    for (int i = 0; i < na; i++)
      if (!pa[i]) vr += a->l[i].n;
    free(pa);
  }
  st->vt_removed = vr;
  st->vt_added = va;
  st->same = all_same && used == na && st->strict == na;
  free(ka);
  free(kb);
  free(mb);
}

/* ---- BWPORT_INTERP_STATS: the log ---- */

typedef struct Prev2D {
  int have;
  u8 io9[0x70], io9b[0x70];
  u8 oam[0x800];
  u8 pltt[0x800];
} Prev2D;
static Prev2D sPrev;
static VC_IpRec sPrevRec;

static unsigned rd16(const u8 *b, u32 o) { return (unsigned)(b[o] | b[o + 1] << 8); }

void vc_ip_log_frame(VCoreState *s, FILE *fp, unsigned frame) {
  VC_IpRec *cur = &s->p23.rec;
  cur->swapped = s->geo.swapped != 0;
  cur->npoly = s->geo.npoly;
  VC_IpStats st;
  int *map = (int *)malloc(sizeof(int) * (sPrevRec.nl + 1));
  vc_ip_pair(&sPrevRec, cur, map, 48.0, &st);
  free(map);
  fprintf(fp, "f=%u on=%d/%d nl=%d/%d nv=%u/%u npoly=%u str=%d loose=%d rem=%d add=%d jump=%d behind=%d vtp=%u vtr=%u vta=%u "
              "chA=%d chAt=%d chT=%d chP=%d chV=%d same=%d dmean=%.2f dmax=%.1f",
          frame, st.swapped_a, st.swapped_b, st.nl_a, st.nl_b, st.nv_a, st.nv_b, cur->npoly, st.strict, st.loose, st.removed,
          st.added, st.jumps, st.behind, st.vt_paired, st.vt_removed, st.vt_added, st.ch_alpha, st.ch_attr, st.ch_tex, st.ch_pltt,
          st.ch_viewport, st.same, st.disp_mean, st.disp_max);
  /* 2D: what moved since the last frame */
  if (sPrev.have) {
    for (int e = 0; e < 2; e++) {
      const u8 *ia = e ? sPrev.io9b : sPrev.io9, *ib = e ? s->io9b : s->io9;
      int scroll = 0, aff = 0, other = 0;
      long sd = 0;
      for (int n = 0; n < 4; n++) {
        int dx = (int)(rd16(ib, 0x10 + 4 * n) & 0x1FF) - (int)(rd16(ia, 0x10 + 4 * n) & 0x1FF);
        int dy = (int)(rd16(ib, 0x12 + 4 * n) & 0x1FF) - (int)(rd16(ia, 0x12 + 4 * n) & 0x1FF);
        if (dx || dy) scroll |= 1 << n;
        sd += abs(dx) + abs(dy);
      }
      for (u32 o = 0x20; o < 0x40; o += 2)
        if (rd16(ia, o) != rd16(ib, o)) aff |= 1 << ((o - 0x20) / 0x10);
      for (u32 o = 0; o < 0x70; o += 2) {
        if (o >= 0x10 && o < 0x40) continue;
        if (rd16(ia, o) != rd16(ib, o)) other++;
      }
      const u8 *oa = sPrev.oam + e * 0x400, *ob = s->oam + e * 0x400;
      int vis = 0, mv = 0, chg = 0, app = 0, dis = 0, mvmax = 0;
      for (int i = 0; i < 128; i++) {
        unsigned a0 = rd16(oa, (u32)i * 8), a1 = rd16(oa, (u32)i * 8 + 2), a2 = rd16(oa, (u32)i * 8 + 4);
        unsigned b0 = rd16(ob, (u32)i * 8), b1 = rd16(ob, (u32)i * 8 + 2), b2 = rd16(ob, (u32)i * 8 + 4);
        int va = ((a0 >> 8) & 3) != 2 && (a0 | a1 | a2) != 0, vb = ((b0 >> 8) & 3) != 2 && (b0 | b1 | b2) != 0;
        vis += vb;
        if (va && vb) {
          int dx = (int)(b1 & 0x1FF) - (int)(a1 & 0x1FF), dy = (int)(b0 & 0xFF) - (int)(a0 & 0xFF);
          if (dx > 256) dx -= 512;
          if (dx < -256) dx += 512;
          if (dy > 128) dy -= 256;
          if (dy < -128) dy += 256;
          if (dx || dy) {
            mv++;
            int m = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
            if (m > mvmax) mvmax = m;
          }
          if ((a0 & 0xFF00u) != (b0 & 0xFF00u) || (a1 & 0xFE00u) != (b1 & 0xFE00u) || a2 != b2) chg++;
        } else if (vb) {
          app++;
        } else if (va) {
          dis++;
        }
      }
      int pal = 0;
      if (e == 0)
        for (int i = 0; i < 0x400; i += 2)
          if (rd16(sPrev.pltt, (u32)i) != rd16(s->pltt, (u32)i)) pal++;
      if (e == 1)
        for (int i = 0x400; i < 0x800; i += 2)
          if (rd16(sPrev.pltt, (u32)i) != rd16(s->pltt, (u32)i)) pal++;
      fprintf(fp, " | e%d scr=%x sd=%ld aff=%x reg=%d obj=%d mv=%d mvmax=%d chg=%d app=%d dis=%d pal=%d", e, scroll, sd, aff, other,
              vis, mv, mvmax, chg, app, dis, pal);
    }
  }
  fprintf(fp, "\n");
  sPrev.have = 1;
  memcpy(sPrev.io9, s->io9, sizeof sPrev.io9);
  memcpy(sPrev.io9b, s->io9b, sizeof sPrev.io9b);
  memcpy(sPrev.oam, s->oam, sizeof sPrev.oam);
  memcpy(sPrev.pltt, s->pltt, sizeof sPrev.pltt);
  vc_ip_free(&sPrevRec);
  sPrevRec = *cur;
  memset(cur, 0, sizeof *cur);
}
