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
  p->kcur = 0;
  p->tag[0] = p->tag[1] = p->tag[2] = 0;
  p->ovr = p->sv = 0;
  if (p->mode == VC_IP_RECORD) {
    p->rec.nv = 0;
    p->rec.nl = 0;
    p->rec.swapped = 0;
    p->rec.npoly = 0;
    p->rec.nproj = 0;
    p->rec.ntag = 0;
  }
}

int vc_ip_tags = -1;
int vc_ip_tags_on(void) {
  if (vc_ip_tags < 0) {
    const char *e = getenv("BWPORT_INTERP_TAGS");
    vc_ip_tags = e == NULL || e[0] != '0';
  }
  return vc_ip_tags;
}
void vc_ip_tag(VCoreState *s, const u32 *param) {
  VC_P23 *p = &s->p23;
  if (p->mode != VC_IP_RECORD) return;
  p->tag[0] = param[0], p->tag[1] = param[1], p->tag[2] = param[2];
}

static s32 lerp_s32(s32 a, s32 b, s32 f);
/* APPLY (M4): the tape's texture state back after a list drawn with b's (unless the stream set it since) */
static void state_restore(VCoreState *s) {
  VC_P23 *p = &s->p23;
  if (p->sv & 1) s->p14.texparam = p->svtex;
  if (p->sv & 2) s->p14.pltt = p->svpltt;
  p->sv = 0;
}
/* APPLY (M4) at a list's BEGIN: the texhold bits of vc_ip_state; p14's state is the list's own here (recorded so at its BEGIN) */
static void state_apply(VCoreState *s) {
  VC_P23 *p = &s->p23;
  state_restore(s);
  p->ovr = 0;
  if (p->list < 0 || p->map == NULL || p->texhold == NULL || p->dst == NULL || p->map[p->list] < 0) return;
  const u8 h = p->texhold[p->list];
  if (!(h & (VC_IP_HOLD_STATE | VC_IP_HOLD_ALPHA)) || p->f <= 0) return;
  const VC_IpList *q = &p->dst->l[p->map[p->list]];
  VC_P14 *st = &s->p14;
  if ((h & VC_IP_HOLD_STATE) && p->f >= 2048) { /* the nearer keyframe's texture (the texcoords held with it) */
    if (st->texparam != q->texparam) p->svtex = st->texparam, p->sv |= 1, st->texparam = q->texparam, p->ovr = 1;
    if (st->pltt != q->pltt) p->svpltt = st->pltt, p->sv |= 2, st->pltt = q->pltt, p->ovr = 1;
  }
  if (h & VC_IP_HOLD_ALPHA) {
    const s32 aa = (s32)(st->attr >> 16 & 31);
    s32 al = lerp_s32(aa, (s32)(q->attr >> 16 & 31), p->f);
    if (al < 1) al = 1;
    if (al != aa) st->attr = (st->attr & ~0x001F0000u) | (u32)al << 16, p->ovr = 1;
  }
}

void vc_ip_end(VCoreState *s) {
  s->p23.open = 0;
  if (s->p23.sv) state_restore(s);
}

void vc_ip_begin(VCoreState *s) {
  VC_P23 *p = &s->p23;
  p->list++;
  p->k = 0;
  p->open = 1;
  if (p->mode == VC_IP_APPLY) state_apply(s);
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
  l->geo = l->tex = 2166136261u;
  if (p->tag[0]) l->owner = p->tag[0], l->inst = p->tag[1], l->state = p->tag[2], r->ntag++;
}

static s32 lerp_s32(s32 a, s32 b, s32 f) { return (s32)(a + (((s64)b - a) * f >> 12)); }
static u32 ip_hash(u32 h, u32 x) { return (h ^ x) * 16777619u; }
static void m4_vtx(const s32 *c, const double *m, double *o) { /* a clip-space vertex times a 4x4 (row vector) */
  for (int j = 0; j < 4; j++) o[j] = (double)c[0] * m[j] + (double)c[1] * m[4 + j] + (double)c[2] * m[8 + j] + (double)c[3] * m[12 + j];
}
void vc_ip_moved(const s32 *clip, const double *m, double t, s32 *out) {
  double o[4];
  m4_vtx(clip, m, o);
  for (int c = 0; c < 4; c++) {
    double x = clip[c] + (o[c] - clip[c]) * t;
    x = x < -2147483648.0 ? -2147483648.0 : x > 2147483647.0 ? 2147483647.0 : x;
    out[c] = (s32)floor(x + 0.5);
  }
}

/* a list's identity at its vertex (VC_IpList.proj, geo, tex, mv): the oracle's, RECORD only */
static void ident_vertex(VCoreState *s, VC_IpRec *r, VC_IpList *l) {
  if (l->n == 0) {
    const s32 *m = s->p13.pos.m, *pm = s->p13.proj.m;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 3; j++) l->mv[i * 3 + j] = m[i * 4 + j];
    u32 h = 2166136261u, k = 0;
    for (int i = 0; i < 16; i++) h = ip_hash(h, (u32)pm[i]);
    while (k < r->nproj && r->projh[k] != h) k++;
    if (k == r->nproj && k < VC_IP_NPROJ - 1) memcpy(r->projm[k], pm, sizeof r->projm[k]), r->projh[r->nproj++] = h;
    l->proj = (u8)(k < VC_IP_NPROJ - 1 ? k : VC_IP_NPROJ - 1);
  }
  for (int i = 0; i < 3; i++) l->geo = ip_hash(l->geo, (u32)s->p14.cur[i]);
  l->tex = ip_hash(ip_hash(l->tex, (u16)s->p15.texraw[0]), (u16)s->p15.texraw[1]);
}

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
    ident_vertex(s, r, l);
    l->n++;
    if (v->clip[3] <= 0) l->behind = 1;
  } else if (p->mode == VC_IP_APPLY) {
    vc_ip_blend_list(s, v, v, 1);
  }
}

/* A texture that repeats samples t and t + k x its period alike (the period: its size, twice that mirrored), so a scroll that wrapped
 * between the keyframes is the same small step as the ones before it, not a slide back across a whole period or more: HG/SS's sea
 * moves every water layer by a texture matrix that jumps back by two or three 32-texel (or 16-texel) periods every 12 s, and the
 * in-betweens of that keyframe pair slid each layer across itself (the water flickered for one interval). */
int vc_ip_texwrapon = -1;
int vc_ip_texwrap_on(void) {
  if (vc_ip_texwrapon < 0) {
    const char *e = getenv("BWPORT_INTERP_TEXWRAP");
    vc_ip_texwrapon = e == NULL || e[0] != '0';
  }
  return vc_ip_texwrapon;
}

int vc_ip_tex_step(u32 tpa, u32 tpb, int c, int d) {
  const u32 m = 1u << (16 + c) | 1u << (18 + c) | 7u << (20 + 3 * c); /* the axis' repeat, mirror and size bits */
  if (!vc_ip_texwrap_on() || !(tpa >> (16 + c) & 1) || ((tpa ^ tpb) & m) != 0 || !(tpa >> 26 & 7)) return d;
  const int per = (128 << (tpa >> (20 + 3 * c) & 7)) << (tpa >> (18 + c) & 1); /* 8 << size texels, x 16, x 2 mirrored */
  int r = d % per;
  if (2 * r > per)
    r -= per;
  else if (2 * r < -per)
    r += per;
  return r;
}

int vc_ip_blend_list(VCoreState *s, const VC_Vertex *in, VC_Vertex *out, u32 n) {
  VC_P23 *p = &s->p23;
  if (out != in) memcpy(out, in, n * sizeof *out);
  if (!p->open || p->mode != VC_IP_APPLY) return 1;
  u32 k = p->k;
  p->k += n;
  if (p->list < 0 || p->map == NULL) return 1;
  if (p->map[p->list] == VC_IP_NEW && p->f > 0) { /* new in the newer keyframe: not drawn before it (VC_IP_NEW) */
    for (u32 j = 0; j < n; j++) { /* every vertex right of the view volume (x > w): clipping drops the list's polygons */
      out[j].clip[0] = 2 << 12;
      out[j].clip[1] = out[j].clip[2] = 0;
      out[j].clip[3] = 1 << 12;
    }
    return 0;
  }
  if (VC_IP_IS_SWAP(p->map[p->list]) && p->f >= 2048) { /* another form of a thing b drew (M4): b's lists until the middle */
    for (u32 j = 0; j < n; j++) out[j].clip[0] = 2 << 12, out[j].clip[1] = out[j].clip[2] = 0, out[j].clip[3] = 1 << 12;
    return 0;
  }
  if (VC_IP_IS_ENTER(p->map[p->list]) || VC_IP_IS_SWAP(p->map[p->list])) { /* entering (vc_ip_windows): toward where it was had it stood
                                                                               * still in the world (a VC_IP_SWAP list: with its owner) */
    if (p->enter == NULL || p->f <= 0) return 1;
    const int m0 = p->map[p->list];
    const double *m = p->enter[VC_IP_IS_SWAP(m0) ? -64 - m0 : -16 - m0];
    for (u32 j = 0; j < n; j++) vc_ip_moved(out[j].clip, m, p->f / 4096.0, out[j].clip);
    return 0;
  }
  if (p->map[p->list] < 0) return 1;
  const VC_IpList *dl = &p->dst->l[p->map[p->list]];
  if (k >= dl->n) return 1;
  if (n > dl->n - k) n = dl->n - k;
  const VC_IpVtx *d = &p->dst->v[dl->first + k];
  const s32 f = p->f;
  const int hold = p->texhold != NULL ? p->texhold[p->list] : 0, take = f >= 2048; /* bit 0: s held, bit 1: t held */
  const u32 tpa = p->sv & 1 ? p->svtex : s->p14.texparam; /* this list's texture state (state_apply may have put b's in) */
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
      const int dt = vc_ip_tex_step(tpa, dl->texparam, i, d->tex[i] - v->tex[i]);
      if (hold >> i & 1) {
        if (take) v->tex[i] = d->tex[i];
      } else if (dt > -2048 && dt < 2048)
        v->tex[i] = (s16)lerp_s32(v->tex[i], v->tex[i] + dt, f);
    }
    same &= v->clip[0] == o.clip[0] && v->clip[1] == o.clip[1] && v->clip[2] == o.clip[2] && v->clip[3] == o.clip[3] &&
            v->color[0] == o.color[0] && v->color[1] == o.color[1] && v->color[2] == o.color[2] && v->tex[0] == o.tex[0] &&
            v->tex[1] == o.tex[1];
  }
  return same && !p->ovr; /* (drawn with another texture state or alpha: not the recorded run's polygons) */
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
  if (s->p23.sv) state_restore(s);
  s->p23.ovr = 0;
  s->p23.mode = VC_IP_OFF;
  s->p23.dst = NULL;
  s->p23.map = NULL;
  s->p23.texhold = NULL;
  s->p23.enter = NULL;
  vc_ip_apply_keep(s, NULL, 0);
}

void vc_ip_apply_enter(VCoreState *s, const double (*enter)[16]) { s->p23.enter = enter; }

void vc_ip_apply_keep(VCoreState *s, const VC_IpKeep *keep, int n) {
  s->p23.keep = keep;
  s->p23.nkeep = keep != NULL ? n : 0;
  s->p23.kcur = 0;
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

const int vc_ip_nostep[1] = {0};

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
          if (pstep != vc_ip_nostep && (!pstep || pstep[2 * map[i] + c] != d0)) texhold[i] |= (u8)(1 << c);
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
  if (!vc_ip_state_on()) return; /* (BWPORT_INTERP_STATE=0: M3, the rule above for every list) */
  for (u32 i = 0; i < a->nl; i++) {
    if (map[i] < 0) continue;
    const VC_IpList *p = &a->l[i], *q = &b->l[map[i]];
    if (!p->n || !p->owner || p->owner != q->owner || p->inst != q->inst || VC_IP_TAG_KIND(p->owner) != VC_IP_TAG_SPRITE) continue;
    int d = p->state != q->state; /* one tagged sprite (M4): its image changed, or its texcoords (a cell, a flip): held, else blended */
    for (u32 k = 0; k < p->n && k < q->n && !d; k++)
      d = a->v[p->first + k].tex[0] != b->v[q->first + k].tex[0] || a->v[p->first + k].tex[1] != b->v[q->first + k].tex[1];
    texhold[i] = (u8)((texhold[i] & ~3) | (d ? 3 : 0));
  }
}

/* ---- M4: the texture-bytes test and the state rules (p23_interp.h; INTERP-ARCH 3.2) ---- */

int vc_ip_stateon = -1;
int vc_ip_state_on(void) {
  if (vc_ip_stateon < 0) {
    const char *e = getenv("BWPORT_INTERP_STATE");
    vc_ip_stateon = e == NULL || e[0] != '0';
  }
  return vc_ip_stateon;
}

static u64 hash_bytes(u64 h, const u8 *p, u32 n) {
  u32 i = 0;
  for (; i + 8 <= n; i += 8) {
    u64 w;
    memcpy(&w, p + i, 8);
    h = (h ^ w) * 0x9E3779B97F4A7C15ull;
    h ^= h >> 31;
  }
  for (; i < n; i++) h = (h ^ p[i]) * 0x100000001B3ull;
  return h;
}
/* bytes [o, o + n) of VRAM region r into the hash h, as vc_rd8 reads them (a page's banks OR-ed, an unmapped page 0) */
static u64 rgn_hash(const VC_Region *r, u32 o, u32 n, u64 h) {
  h = (h ^ n) * 0x9E3779B97F4A7C15ull;
  if (r->npages == 0) return h;
  u8 buf[256];
  while (n) {
    const VC_Page *pg = &r->page[(o >> VC_PAGE_SHIFT) % r->npages];
    const u32 po = o & (VC_PAGE_SIZE - 1);
    u32 m = VC_PAGE_SIZE - po;
    if (m > n) m = n;
    if (pg->n == 1) h = hash_bytes(h, pg->src[0] + po, m);
    else if (pg->n == 0) h = (h ^ (0x5A000000u | m)) * 0x9E3779B97F4A7C15ull;
    else
      for (u32 d = 0; d < m;) {
        const u32 c = m - d < (u32)sizeof buf ? m - d : (u32)sizeof buf;
        for (u32 x = 0; x < c; x++) {
          u8 v = 0;
          for (int q = 0; q < pg->n; q++) v |= pg->src[q][po + d + x];
          buf[x] = v;
        }
        h = hash_bytes(h, buf, c);
        d += c;
      }
    o += m;
    n -= m;
  }
  return h;
}
/* the bytes texture state (tp, pltt) samples in s's VRAM (GBATEK 3D texture formats): texels; for 4x4 the index words (slot 1, or 1's
 * second half for slot 2) and the palette as far as its blocks reach (base + index x 4, 4 colours); else the format's palette */
static u32 tex_hash(VCoreState *s, u32 tp, u32 pltt) {
  static const u32 bpp[8] = {0, 8, 2, 4, 8, 2, 8, 16}, pal[8] = {0, 64, 8, 32, 512, 0, 16, 0};
  const u32 fmt = tp >> 26 & 7, w = 8u << (tp >> 20 & 7), h = 8u << (tp >> 23 & 7), a = (tp & 0xFFFF) << 3;
  if (!fmt) return 1;
  const VC_Region *rt = &s->rgn[VC_RGN_TEX];
  u64 x = rgn_hash(rt, a, w * h * bpp[fmt] / 8, 0xcbf29ce484222325ull ^ fmt);
  u32 pn = pal[fmt];
  if (fmt == 5) {
    const u32 ia = 0x20000 + (a & 0x1FFFF) / 2 + (a >= 0x40000 ? 0x10000 : 0), in = w * h / 8;
    x = rgn_hash(rt, ia, in, x);
    u32 mx = 0;
    for (u32 k = 0; k < in; k += 2) {
      const u32 e = vc_rd16(rt, ia + k) & 0x3FFF;
      if (e > mx) mx = e;
    }
    pn = mx * 4 + 8;
  }
  if (pn) x = rgn_hash(&s->rgn[VC_RGN_TEXPAL], fmt == 2 ? pltt << 3 : pltt << 4, pn, x);
  return (u32)(x ^ x >> 32) | 1;
}
typedef struct TexMemo { u32 tp, pltt, h, used; } TexMemo;
enum { IP_TEXMEMO = 512 };
static u32 tex_memo(VCoreState *s, TexMemo *m, u32 tp, u32 pltt) {
  if (!(tp >> 26 & 7)) return 1;
  u32 k = (tp * 2654435761u ^ pltt * 40503u) & (IP_TEXMEMO - 1);
  for (int probe = 0; probe < 16; probe++, k = (k + 1) & (IP_TEXMEMO - 1)) {
    if (!m[k].used) return m[k] = (TexMemo){tp, pltt, tex_hash(s, tp, pltt), 1}, m[k].h;
    if (m[k].tp == tp && m[k].pltt == pltt) return m[k].h;
  }
  return tex_hash(s, tp, pltt);
}

void vc_ip_texsig(VCoreState *s, VC_IpRec *r) {
  vc_vram_map(s);
  TexMemo *memo = (TexMemo *)calloc(IP_TEXMEMO, sizeof *memo);
  for (u32 i = 0; i < r->nl; i++) r->l[i].texh = tex_memo(s, memo, r->l[i].texparam, r->l[i].pltt);
  free(memo);
}

int vc_ip_state(VCoreState *s, const VC_IpRec *a, const VC_IpRec *b, int *map, u8 *texhold, VC_IpKeep *keep, int nkeep) {
  if (!vc_ip_state_on()) return nkeep;
  vc_vram_map(s);
  TexMemo *memo = (TexMemo *)calloc(IP_TEXMEMO, sizeof *memo);
  for (u32 i = 0; i < a->nl; i++) {
    const int j = map[i];
    if (j < 0) continue;
    const VC_IpList *p = &a->l[i], *q = &b->l[j];
    if (!p->n) continue; /* (nothing drawn) */
    if ((p->texparam != q->texparam || p->pltt != q->pltt) && q->texh && tex_memo(s, memo, q->texparam, q->pltt) == q->texh)
      texhold[i] |= VC_IP_HOLD_STATE | 3; /* b's texture is still there: switch at the middle, its texcoords with it */
    const u32 aa = p->attr >> 16 & 31, ab = q->attr >> 16 & 31;
    if (aa != ab && aa && ab) texhold[i] |= VC_IP_HOLD_ALPHA;
  }
  int n = 0, drop = 0;
  for (int k = 0; k < nkeep; k++) {
    const VC_IpList *q = &b->l[keep[k].list];
    if (q->texh && tex_memo(s, memo, q->texparam, q->pltt) != q->texh) { /* its bytes changed: not drawn (gone at once, as before M2) */
      drop += keep[k].half;
      continue;
    }
    keep[n++] = keep[k];
  }
  for (u32 i = 0; i < a->nl && drop; i++) { /* a new form whose old one went: drawn in the whole interval */
    if (!VC_IP_IS_SWAP(map[i])) continue;
    const VC_IpList *p = &a->l[i];
    int other = 0;
    for (int k = 0; k < n && !other; k++)
      other = keep[k].half && b->l[keep[k].list].owner == p->owner && b->l[keep[k].list].inst == p->inst;
    if (!other) map[i] = VC_IP_ENTER(-64 - map[i]);
  }
  free(memo);
  return n;
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
static double clampd(double x, double m) { return x < -m ? -m : x > m ? m : x; }

int vc_ip_list_seen(const VC_IpRec *r, const VC_IpList *l) {
  unsigned all = 63;
  for (u32 j = 0; j < l->n && all != 0; j++) {
    const VC_IpVtx *v = &r->v[l->first + j];
    const s64 x = v->clip[0], y = v->clip[1], z = v->clip[2], w = v->clip[3];
    all &= (unsigned)(x < -w) | (unsigned)(x > w) << 1 | (unsigned)(y < -w) << 2 | (unsigned)(y > w) << 3 | (unsigned)(z < -w) << 4 |
           (unsigned)(z > w) << 5;
  }
  return l->n > 0 && all == 0;
}

int vc_ip_scene_cut(const int *map, int nl, int *unpaired) {
  int n = 0;
  for (int i = 0; i < nl; i++) n += map[i] < 0;
  if (unpaired != NULL) *unpaired = n;
  return n;
}

int vc_ip_cut(const VC_IpRec *a, const int *map, int *unpaired, int *seen) {
  u32 nv = 0, vp = 0;
  int nl = 0, n = 0;
  for (u32 i = 0; i < a->nl; i++) {
    if (!vc_ip_list_seen(a, &a->l[i])) continue;
    nl++;
    nv += a->l[i].n;
    if (map[i] >= 0 || VC_IP_IS_ENTER(map[i])) vp += a->l[i].n; /* (entering: glides with the world) */
    else if (map[i] != VC_IP_NEW) n++;
  }
  if (unpaired != NULL) *unpaired = n;
  if (seen != NULL) *seen = nl;
  return vp * 2 < nv;
}

int vc_ip_cutid = -1;
int vc_ip_cutid_on(void) {
  if (vc_ip_cutid < 0) {
    const char *e = getenv("BWPORT_INTERP_CUTID");
    vc_ip_cutid = e == NULL || e[0] != '0';
  }
  return vc_ip_cutid;
}

int vc_ip_cut_id(const VC_IpRec *a, const VC_IpRec *b, const int *map, const u8 *how, int *unpaired, int *seen) {
  if (!vc_ip_cutid_on() || how == NULL) return vc_ip_cut(a, map, unpaired, seen);
  u64 nv = 0, vp = 0, dn = 0;
  double dsum = 0;
  int nl = 0, n = 0;
  for (u32 i = 0; i < a->nl; i++) {
    const VC_IpList *l = &a->l[i];
    if (!vc_ip_list_seen(a, l)) continue;
    const int m = map[i];
    nl++;
    nv += l->n;
    const int proven = m >= 0 && (how[i] == VC_IP_HOW_ID || how[i] == VC_IP_HOW_TAG);
    if (VC_IP_IS_SWAP(m) || proven) vp += l->n;
    else if (m < 0 && m != VC_IP_NEW && !VC_IP_IS_ENTER(m)) n++; /* (as vc_ip_cut's count: lists left alone) */
    if (!proven) continue;
    const VC_IpList *q = &b->l[m];
    for (u32 k = 0; k < l->n && k < q->n; k++) { /* the pair's screen motion (vc_ip_pair's measure: clamped, in front in both) */
      const VC_IpVtx *u = &a->v[l->first + k], *w = &b->v[q->first + k];
      if (u->clip[3] <= 0 || w->clip[3] <= 0) continue;
      const double ax = clampd(128.0 * u->clip[0] / u->clip[3], 256), ay = clampd(96.0 * u->clip[1] / u->clip[3], 192);
      const double bx = clampd(128.0 * w->clip[0] / w->clip[3], 256), by = clampd(96.0 * w->clip[1] / w->clip[3], 192);
      dsum += sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
      dn++;
    }
  }
  if (unpaired != NULL) *unpaired = n;
  if (seen != NULL) *seen = nl;
  return vp * 2 < nv ? 1 : dn && dsum > 128.0 * (double)dn ? 2 : 0;
}

/* the windows' kept lists in drawing order (VC_IpKeep.after) */
static int keep_cmp(const void *x, const void *y) {
  const VC_IpKeep *p = (const VC_IpKeep *)x, *q = (const VC_IpKeep *)y;
  return p->after != q->after ? (p->after < q->after ? -1 : 1) : p->list - q->list;
}

/* ---- tags (docs/interp-sweep.md M3; INTERP-ARCH 2.3 layer B): the game says which instance a list draws ---- */

typedef struct TagL { u32 owner, inst; int idx; } TagL;
static int tagk_cmp(const TagL *p, const TagL *q) {
  if (p->owner != q->owner) return p->owner < q->owner ? -1 : 1;
  return p->inst != q->inst ? (p->inst < q->inst ? -1 : 1) : 0;
}
static int tagl_cmp(const void *x, const void *y) {
  const TagL *p = (const TagL *)x, *q = (const TagL *)y;
  const int c = tagk_cmp(p, q);
  return c ? c : p->idx - q->idx;
}
/* a's list l and b's m of one (owner, inst) are one thing in two keyframes: the same kind of list (type, vertex count; when the
 * instance drew another number of lists in the other frame, the same shape too), not an overlay (it never glides: it switches at its
 * keyframe) and not a particle whose age went down (its slot holds a new particle) */
static int tag_fits(const VC_IpList *l, const VC_IpList *m, int shape) {
  const u32 kind = VC_IP_TAG_KIND(l->owner);
  if (l->type != m->type || l->n != m->n || (shape && l->geo != m->geo) || kind == VC_IP_TAG_OVERLAY) return 0;
  return kind != VC_IP_TAG_PARTICLE || l->state >= m->state;
}
/* the lists the tags decide when both frames have tags (vc_ip_tags): da[i] / db[j] = 1 for each tagged list with vertices; their pairs
 * ta[i] = b's list or -1, tb[j] = a's or -1, by (owner, inst), an instance's lists in draw order (tag_fits). A decided list with no
 * pair is new or gone: identity and the rest leave it. Returns the pairs. */
static int tag_pair(const VC_IpRec *a, const VC_IpRec *b, int *ta, int *tb, u8 *da, u8 *db) {
  const int na = (int)a->nl, nb = (int)b->nl;
  for (int i = 0; i < na; i++) ta[i] = -1, da[i] = 0;
  for (int j = 0; j < nb; j++) tb[j] = -1, db[j] = 0;
  if (!vc_ip_tags_on() || !a->ntag || !b->ntag) return 0;
  TagL *sa = (TagL *)malloc(sizeof(TagL) * (size_t)(na + nb + 2)), *sb = sa + na + 1;
  int ma = 0, mb = 0, n = 0;
  for (int i = 0; i < na; i++)
    if (a->l[i].owner && a->l[i].n) sa[ma++] = (TagL){a->l[i].owner, a->l[i].inst, i}, da[i] = 1;
  for (int j = 0; j < nb; j++)
    if (b->l[j].owner && b->l[j].n) sb[mb++] = (TagL){b->l[j].owner, b->l[j].inst, j}, db[j] = 1;
  qsort(sa, (size_t)ma, sizeof *sa, tagl_cmp);
  qsort(sb, (size_t)mb, sizeof *sb, tagl_cmp);
  for (int g = 0, y = 0; g < ma;) {
    int ge = g;
    while (ge < ma && !tagk_cmp(&sa[ge], &sa[g])) ge++;
    while (y < mb && tagk_cmp(&sb[y], &sa[g]) < 0) y++;
    int ye = y;
    while (ye < mb && !tagk_cmp(&sb[ye], &sa[g])) ye++;
    const int shape = ge - g != ye - y;
    for (int p = g; p < ge; p++)
      for (int q = y; q < ye; q++) {
        const int i = sa[p].idx, j = sb[q].idx;
        if (tb[j] >= 0 || !tag_fits(&a->l[i], &b->l[j], shape)) continue;
        ta[i] = j, tb[j] = i, n++;
        break;
      }
    g = ge;
    y = ye;
  }
  free(sa);
  return n;
}

/* ---- identity (docs/interp-sweep.md, M1): the pairs a list's shape and matrix prove, before any rule of order or place ---- */

#define IP_ID_QA (6.0 / 4096) /* D's quanta and match tolerances (rotation entries, translation units: vcore_ipsweep's oracle) */
#define IP_ID_QT (1.0 / 16)
#define IP_ID_FAR 12.0           /* world units (a Gen 4 tile is 16): a lone pair further than this from the camera's D is two things */
#define IP_ID_VOTECAND 1024L   /* candidate pairs of one signature (copies x copies) that vote and are all weighed ... */
#define IP_ID_VOTETOTAL 16384L /* ... and of all of them in one keyframe pair; past either, a group's candidates are only those a
                                * plausible motion predicts (ident_pair's rule 3 for many copies) */

typedef struct IdAff { double a[9], t[3]; } IdAff; /* v' = v x a + t (a 4x3 position matrix, 20.12) */
static void id_aff(const s32 *mv, IdAff *m) {
  for (int i = 0; i < 9; i++) m->a[i] = mv[i] / 4096.0;
  for (int i = 0; i < 3; i++) m->t[i] = mv[9 + i] / 4096.0;
}
static int id_inv(const IdAff *m, IdAff *r) {
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
static void id_mul(const IdAff *x, const IdAff *y, IdAff *r) { /* x, then y */
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) r->a[i * 3 + j] = x->a[i * 3] * y->a[j] + x->a[i * 3 + 1] * y->a[3 + j] + x->a[i * 3 + 2] * y->a[6 + j];
  for (int j = 0; j < 3; j++) r->t[j] = x->t[0] * y->a[j] + x->t[1] * y->a[3 + j] + x->t[2] * y->a[6 + j] + y->t[j];
}
static u32 id_bin(const IdAff *d) {
  u32 h = 2166136261u;
  for (int i = 0; i < 9; i++) h = mix(h, (u32)lround(d->a[i] / IP_ID_QA));
  for (int i = 0; i < 3; i++) h = mix(h, (u32)lround(d->t[i] / IP_ID_QT));
  return h;
}
static double id_dist(const IdAff *x, const IdAff *y, double *dt) { /* the largest rotation entry's difference; *dt: translation's */
  double da = 0, t = 0;
  for (int i = 0; i < 9; i++) da = fmax(da, fabs(x->a[i] - y->a[i]));
  for (int i = 0; i < 3; i++) t = fmax(t, fabs(x->t[i] - y->t[i]));
  *dt = t;
  return da;
}
static int id_fits(const IdAff *x, const IdAff *y) {
  double dt, da = id_dist(x, y, &dt);
  return da <= IP_ID_QA && dt <= IP_ID_QT;
}
typedef struct IdBin { u32 h; int votes, last, uniq, pairs; IdAff d; u8 pm; } IdBin; /* uniq: votes of signatures unique in both frames;
                                                                                 * pm: the projections of its pairs (bit VC_IpList.proj) */
/* rule 2's order: votes, then votes of signatures unique in both frames, then the candidate pairs it explains (a view of copies only:
 * the true D maps every copy, a step over one row fewer, and both have a vote from each signature; the M1 review), then the smaller
 * translation, then the hash: no tie is left to the bin table's order */
static int bin_better(const IdBin *p, const IdBin *q) {
  if (p->votes != q->votes) return p->votes > q->votes;
  if (p->uniq != q->uniq) return p->uniq > q->uniq;
  if (p->pairs != q->pairs) return p->pairs > q->pairs;
  const double tp = p->d.t[0] * p->d.t[0] + p->d.t[1] * p->d.t[1] + p->d.t[2] * p->d.t[2];
  const double tq = q->d.t[0] * q->d.t[0] + q->d.t[1] * q->d.t[1] + q->d.t[2] * q->d.t[2];
  if (tp != tq) return tp < tq;
  return p->h < q->h;
}
typedef struct IdCand { int i, j, g, d, p; } IdCand; /* a candidate pair, its signature (group), the chosen D it fits, the plausible one */
typedef struct IdSig { u32 sig; int idx; } IdSig;
enum { IP_ID_MAXD = 64 };
/* what identity measured that the rest's pairs are tested against (pair_rest): the chosen Ds, and the camera's D inverted (a point of
 * the older frame's camera space to the newer's) */
typedef struct IdCtx { IdAff ch[IP_ID_MAXD], icam; int nch, camok, ncand; u8 chpm[IP_ID_MAXD]; } IdCtx; /* ncand: candidates weighed,
                                                                                               * both tiers; chpm: each chosen D's pm */
typedef struct IdBig { int g, ge, y, ye, gid; } IdBig; /* a group of many copies: sa[g, ge) with sb[y, ye) */
typedef struct IdX { double x; int j; } IdX;
static int idx_cmp(const void *p, const void *q) {
  const IdX *u = (const IdX *)p, *v = (const IdX *)q;
  return u->x != v->x ? (u->x < v->x ? -1 : 1) : u->j - v->j;
}
static int idcand_cmp(const void *x, const void *y) {
  const IdCand *p = (const IdCand *)x, *q = (const IdCand *)y;
  if ((p->d < 0) != (q->d < 0)) return p->d < 0 ? 1 : -1;
  if (p->d != q->d) return p->d - q->d;
  return p->i != q->i ? p->i - q->i : p->j - q->j;
}
static int idsig_cmp(const void *x, const void *y) {
  const IdSig *p = (const IdSig *)x, *q = (const IdSig *)y;
  return p->sig != q->sig ? (p->sig < q->sig ? -1 : 1) : p->idx - q->idx;
}
/* a list's signature: its key (render state), its shape in object space and which of the frame's projections drew it */
static u32 id_sig(const VC_IpList *l) { return mix(mix(key_of(l), l->geo), l->proj); }
/* the signature without what a texture's animation changes: the image's place in texture memory and the palette (ident_pair's tier 1) */
static u32 id_look(const VC_IpList *l) {
  VC_IpList m = *l;
  m.texparam &= 0xFFFF0000u;
  m.pltt = 0;
  return mix(mix(key_of(&m), l->geo), l->proj);
}
/* the plausible motion a D is: the chosen D it fits (0 = the camera's), x->nch = none of them but no motion at all (the same matrix in
 * both frames: it stood still relative to the camera, as what the camera follows does: the player, the follower, their shadows), or -1 */
static const IdAff id_one = {{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}};
static int id_plausible(const IdCtx *x, const IdAff *d) {
  for (int q = 0; q < x->nch; q++)
    if (id_fits(&x->ch[q], d)) return q;
  return id_fits(&id_one, d) ? x->nch : -1;
}
/* rule 5 (below): a's list with matrix mva and b's with mvb, one signature, are one thing: their D is plausible, or the place the
 * camera's D gives b's list is within IP_ID_FAR of a's, measured in a's camera space (world units: a billboard's own units are scaled) */
static double id_far(const IdCtx *x, const s32 *mva, const s32 *mvb, int *pq) { /* (0: their D is plausible, or no camera is
                                                                              * known; *pq, or NULL: the plausible D's id_plausible index, else -1) */
  IdAff ma, mb, ia, d;
  id_aff(mva, &ma);
  id_aff(mvb, &mb);
  if (pq != NULL) *pq = -1;
  if (!x->nch || !x->camok || !id_inv(&ma, &ia)) return 0;
  id_mul(&ia, &mb, &d);
  const int q = id_plausible(x, &d);
  if (q >= 0) {
    if (pq != NULL) *pq = q;
    return 0;
  }
  double e = 0;
  for (int k = 0; k < 3; k++) {
    double p = mb.t[0] * x->icam.a[k] + mb.t[1] * x->icam.a[3 + k] + mb.t[2] * x->icam.a[6 + k] + x->icam.t[k] - ma.t[k];
    e += p * p;
  }
  return sqrt(e);
}
static int id_near(const IdCtx *x, const s32 *mva, const s32 *mvb) { return id_far(x, mva, mvb, NULL) <= IP_ID_FAR; }

/* The pairs identity proves between a (the newer keyframe) and b (the older): la[i] = b's list or -1, lb[j] = a's list or -1, and
 * wa[i] = the b list a's list i has its signature in common with but is not (rule 5) or -1; x: what the rest is tested against.
 * Returns the number of pairs. The rules (the sweep's oracle, vcore_ipsweep orc_pair, docs/interp-sweep.md 2026-11-25 / -26):
 * 1. a signature drawn once in each frame is a pair;
 * 2. each candidate pair of a signature (one list of each frame) gives D = inverse(a's matrix) x b's matrix: the motion of the thing in
 *    the world between the frames, the camera's included. One vote per signature and D (quantised); the D with the most votes over
 *    all projections is the camera's (billboards use their own projection with the same camera); another D is a rigid group (the
 *    player with what moves with it) only when two signatures drawn once in both frames vote for it (copies alone, a shadow and its
 *    twin, vote as much for swapping places as for standing still);
 * 3. copies pair through the chosen Ds, the camera's first, in draw order, unless a copy has candidates through two plausible motions
 *    (id_plausible: the chosen Ds and no motion): when the camera moves by the copies' spacing (Run 4x: a tile a keyframe) the
 *    follower's shadow stands where the player's stood in the world and the player's where it stood on the screen, and a row of tiles
 *    the same; nothing in shape and matrix says which, so the signature's lists are left to the rest (rest_pair);
 * 4. a signature with one list left on each side is a pair (it moved on its own), unless rule 3 left it;
 * 5. a pair of 1 or 4 that is not near (id_near: more than IP_ID_FAR world units from where the camera's D puts it, no plausible D) is
 *    two things that look the same (the grass's last image on one tile and the next tile's, the rings of the player and the follower a
 *    phase apart, one house culled and its twin drawn): no pair, and wa marks it.
 * Lists with no vertex (empty BEGIN..END) are left alone.
 * tier 1, after tier 0: the lists left, by id_look (a texture's animation: Platinum's door scene redraws a sprite at its place each
 * keyframe with the next image and palette), through the chosen Ds only. The shape and the matrix are all there is to tell such a list
 * by, so rules 1 and 4 keep a pair only when a plausible motion fits it, and a group is left alone when one of its lists has two
 * plausible candidates, even through one motion (HeartGold's grass over a sprite's feet is a quad of the sprite's shape at its place:
 * the follower whose image changed met the grass first). Rule 2 is not run again. Returns the pairs of both tiers.
 * da / db: the lists the tags decide (tag_pair): they vote in rule 2 as before (the camera's D and the groups' stay what the shapes
 * and matrices say) and are paired by no rule here. */
static int ident_pair(const VC_IpRec *a, const VC_IpRec *b, int *la, int *lb, int *wa, IdCtx *x, int tier, const u8 *da, const u8 *db) {
  int na = (int)a->nl, nb = (int)b->nl;
  IdSig *sa = (IdSig *)malloc(sizeof(IdSig) * (size_t)(na + 1)), *sb = (IdSig *)malloc(sizeof(IdSig) * (size_t)(nb + 1));
  IdAff *ia = (IdAff *)malloc(sizeof(IdAff) * (size_t)(na + 1)), *ab = (IdAff *)malloc(sizeof(IdAff) * (size_t)(nb + 1));
  u8 *inv = (u8 *)calloc((size_t)na + 1, 1), *mvd = (u8 *)calloc((size_t)na + 1, 1); /* mvd: paired by rule 1 or 4 */
  int *pj = (int *)malloc(sizeof(int) * 2 * (size_t)(nb + 1)), *pg = pj + nb + 1;    /* rule 3: b's list's plausible D in group pg */
  int ma = 0, mbs = 0;
  for (int i = 0; i < na; i++) {
    if (!tier) la[i] = wa[i] = -1;
    if (!a->l[i].n || la[i] >= 0 || (tier && da[i])) continue;
    IdAff m;
    id_aff(a->l[i].mv, &m);
    inv[i] = (u8)id_inv(&m, &ia[i]);
    sa[ma++] = (IdSig){tier ? id_look(&a->l[i]) : id_sig(&a->l[i]), i};
  }
  for (int j = 0; j < nb; j++) {
    if (!tier) lb[j] = -1;
    pg[j] = -1;
    if (!b->l[j].n || lb[j] >= 0 || (tier && db[j])) continue;
    id_aff(b->l[j].mv, &ab[j]);
    sb[mbs++] = (IdSig){tier ? id_look(&b->l[j]) : id_sig(&b->l[j]), j};
  }
  qsort(sa, (size_t)ma, sizeof *sa, idsig_cmp);
  qsort(sb, (size_t)mbs, sizeof *sb, idsig_cmp);
  if (!tier) x->ncand = 0;
  int capb = 1024, nbin = 0, nbig = 0;
  long total = 0;
  IdBin *bin = (IdBin *)calloc((size_t)capb, sizeof(IdBin)); /* open addressing on h */
  IdBig *big = (IdBig *)malloc(sizeof(IdBig) * (size_t)(ma + 1));
  int ncand = 0, capc = 1024;
  IdCand *cand = (IdCand *)malloc(sizeof(IdCand) * (size_t)capc);
  IdAff *cd = (IdAff *)malloc(sizeof(IdAff) * (size_t)capc);
  int g = 0, y = 0, gid = 0;
  while (g < ma) { /* the groups: [g, ge) of sa with [y, ye) of sb */
    int ge = g;
    while (ge < ma && sa[ge].sig == sa[g].sig) ge++;
    while (y < mbs && sb[y].sig < sa[g].sig) y++;
    int ye = y;
    while (ye < mbs && sb[ye].sig == sa[g].sig) ye++;
    gid++;
    const int uniq = ge - g == 1 && ye - y == 1;
    if (uniq) { /* rule 1 */
      int i = sa[g].idx, j = sb[y].idx;
      if (a->l[i].n == b->l[j].n && !da[i] && !db[j]) la[i] = j, lb[j] = i, mvd[i] = 1; /* (a 32-bit signature can collide) */
    }
    const long nc = (long)(ge - g) * (ye - y);
    const int weigh = nc > 0 && nc <= IP_ID_VOTECAND && total + nc <= IP_ID_VOTETOTAL;
    if (nc > 0 && !weigh) big[nbig++] = (IdBig){g, ge, y, ye, gid};
    if (weigh) total += nc;
    if (weigh)
      for (int p = g; p < ge; p++)
        for (int q = y; q < ye; q++) {
          int i = sa[p].idx, j = sb[q].idx;
          if (!inv[i] || a->l[i].n != b->l[j].n) continue;
          if (ncand == capc) {
            capc *= 2;
            cand = (IdCand *)realloc(cand, sizeof(IdCand) * (size_t)capc);
            cd = (IdAff *)realloc(cd, sizeof(IdAff) * (size_t)capc);
          }
          id_mul(&ia[i], &ab[j], &cd[ncand]);
          u32 h = id_bin(&cd[ncand]);
          if (nbin * 2 >= capb) { /* grow and rehash */
            IdBin *nw = (IdBin *)calloc((size_t)capb * 2, sizeof(IdBin));
            for (int k = 0; k < capb; k++)
              if (bin[k].votes) {
                u32 s = bin[k].h & (u32)(capb * 2 - 1);
                while (nw[s].votes) s = (s + 1) & (u32)(capb * 2 - 1);
                nw[s] = bin[k];
              }
            free(bin);
            bin = nw;
            capb *= 2;
          }
          u32 s = h & (u32)(capb - 1);
          while (bin[s].votes && bin[s].h != h) s = (s + 1) & (u32)(capb - 1);
          if (!bin[s].votes) bin[s] = (IdBin){h, 0, 0, 0, 0, cd[ncand], 0}, nbin++;
          bin[s].pairs++;
          bin[s].pm |= (u8)(1u << a->l[i].proj);
          if (bin[s].last != gid) bin[s].votes++, bin[s].last = gid;
          if (uniq) bin[s].uniq++;
          x->ncand++;
          if (!da[i] && !db[j]) cand[ncand++] = (IdCand){i, j, gid, -1, -1};
        }
    g = ge;
    y = ye;
  }
  /* rule 2: the chosen Ds, the camera's first */
  if (!tier) x->nch = 0;
  while (!tier) {
    int best = -1;
    for (int k = 0; k < capb; k++) {
      if (bin[k].votes < 2 || bin[k].last < 0) continue;
      if (x->nch && bin[k].uniq < 2) continue;
      if (best < 0 || bin_better(&bin[k], &bin[best])) best = k;
    }
    if (best < 0 || x->nch == IP_ID_MAXD) break;
    int dup = -1;
    for (int c = 0; c < x->nch && dup < 0; c++)
      if (id_fits(&x->ch[c], &bin[best].d)) dup = c;
    if (dup >= 0) x->chpm[dup] |= bin[best].pm;
    else x->chpm[x->nch] = bin[best].pm, x->ch[x->nch++] = bin[best].d;
    bin[best].last = -1;
  }
  if (!tier) x->camok = x->nch && id_inv(&x->ch[0], &x->icam);
  /* rule 3's candidates in a group of many copies (particles: lib/spl draws each as its own list, and those of one emitter move on
   * their own; the M1 review): every pair was weighed (256 copies: 65536 bins and a sort, 10-20 ms a keyframe, pairing nothing).
   * Here only the pairs through a plausible motion (id_plausible: the chosen Ds and no motion), found by the place each predicts for
   * b's list (sorted by x; a D within IP_ID_QA / IP_ID_QT moves the place by at most r); they do not vote. Rule 3 gets the same
   * candidates it would use from the whole set (it skips the others) */
  if (nbig) {
    IdX *xs = (IdX *)malloc(sizeof(IdX) * (size_t)(mbs + 1));
    int *stamp = (int *)malloc(sizeof(int) * (size_t)(nb + 1));
    for (int j = 0; j < nb; j++) stamp[j] = -1;
    static const IdAff one = {{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}};
    for (int k = 0; k < nbig; k++) {
      const IdBig *bg = &big[k];
      const int nq = bg->ye - bg->y;
      for (int q = 0; q < nq; q++) xs[q] = (IdX){ab[sb[bg->y + q].idx].t[0], sb[bg->y + q].idx};
      qsort(xs, (size_t)nq, sizeof *xs, idx_cmp);
      for (int p = bg->g; p < bg->ge; p++) {
        const int i = sa[p].idx;
        if (!inv[i] || da[i]) continue;
        IdAff m, pr, dc;
        id_aff(a->l[i].mv, &m);
        const double r = IP_ID_QA * (fabs(m.t[0]) + fabs(m.t[1]) + fabs(m.t[2])) + IP_ID_QT + 1e-9;
        for (int q = 0; q <= x->nch; q++) {
          id_mul(&m, q < x->nch ? &x->ch[q] : &one, &pr);
          int lo = 0, hi = nq;
          while (lo < hi) {
            const int mid = (lo + hi) / 2;
            if (xs[mid].x < pr.t[0] - r) lo = mid + 1;
            else hi = mid;
          }
          for (int s2 = lo; s2 < nq && xs[s2].x <= pr.t[0] + r; s2++) {
            const int j = xs[s2].j;
            if (stamp[j] == i || a->l[i].n != b->l[j].n || db[j]) continue;
            id_mul(&ia[i], &ab[j], &dc);
            x->ncand++;
            if (id_plausible(x, &dc) < 0) continue;
            stamp[j] = i;
            if (ncand == capc) {
              capc *= 2;
              cand = (IdCand *)realloc(cand, sizeof(IdCand) * (size_t)capc);
              cd = (IdAff *)realloc(cd, sizeof(IdAff) * (size_t)capc);
            }
            cd[ncand] = dc;
            cand[ncand++] = (IdCand){i, j, bg->gid, -1, -1};
          }
        }
      }
    }
    free(xs);
    free(stamp);
  }
  /* rule 3: each candidate's plausible D; a group with a list that has two is left alone (u8 per group in amb) */
  u8 *amb = (u8 *)calloc((size_t)gid + 2, 1);
  for (int c = 0, ri = -1, rg = -1, cp = -1; c < ncand; c++) { /* (a's lists' candidates come in runs, by group) */
    int p = id_plausible(x, &cd[c]), j = cand[c].j;
    cand[c].p = p;
    cand[c].d = p < x->nch ? p : -1;
    if (p < 0) continue;
    if (cand[c].i != ri || cand[c].g != rg) ri = cand[c].i, rg = cand[c].g, cp = -1;
    if (cp >= 0 && (cp != p || tier)) amb[cand[c].g] = 1; /* (tier 1: two candidates at all) */
    cp = p;
    if (pg[j] == cand[c].g && (pj[j] != p || tier)) amb[cand[c].g] = 1;
    pg[j] = cand[c].g, pj[j] = p;
  }
  for (int c = 0; c < ncand; c++)
    if (amb[cand[c].g]) cand[c].d = -1;
  qsort(cand, (size_t)ncand, sizeof *cand, idcand_cmp);
  for (int c = 0; c < ncand && cand[c].d >= 0; c++) {
    int i = cand[c].i, j = cand[c].j;
    if (la[i] >= 0 || lb[j] >= 0) continue;
    la[i] = j, lb[j] = i;
  }
  /* rule 4 */
  g = 0, y = 0, gid = 0;
  while (g < ma) {
    int ge = g, nla = 0, nlb = 0, ii = -1, jj = -1;
    while (ge < ma && sa[ge].sig == sa[g].sig) ge++;
    while (y < mbs && sb[y].sig < sa[g].sig) y++;
    int ye = y;
    while (ye < mbs && sb[ye].sig == sa[g].sig) ye++;
    gid++;
    for (int p = g; p < ge; p++)
      if (la[sa[p].idx] < 0 && !da[sa[p].idx]) nla++, ii = sa[p].idx;
    for (int q = y; q < ye; q++)
      if (lb[sb[q].idx] < 0 && !db[sb[q].idx]) nlb++, jj = sb[q].idx;
    if (nla == 1 && nlb == 1 && !amb[gid] && a->l[ii].n == b->l[jj].n) la[ii] = jj, lb[jj] = ii, mvd[ii] = 1;
    g = ge;
    y = ye;
  }
  /* rule 5 */
  for (int i = 0; i < na; i++) {
    if (!mvd[i] || la[i] < 0) continue;
    IdAff d;
    if (tier ? inv[i] && (id_mul(&ia[i], &ab[la[i]], &d), id_plausible(x, &d) >= 0) : id_near(x, a->l[i].mv, b->l[la[i]].mv)) continue;
    if (!tier) wa[i] = la[i];
    lb[la[i]] = -1, la[i] = -1;
  }
  int n = 0;
  for (int i = 0; i < na; i++) n += la[i] >= 0;
  free(sa), free(sb), free(ia), free(ab), free(inv), free(mvd), free(pj), free(bin), free(cand), free(cd), free(amb), free(big);
  return n;
}

/* ---- the rest (INTERP-ARCH 2.2 step 5, M5): what identity and the tags left, and the pair map ---- */

#define IP_REST_MAXCAND 16384L /* the rest's candidate pairs weighed a round in one keyframe pair (rest_pair) */
#define IP_TAG_JUMP_PX 128.0   /* a tag pair moving further on the screen than half its width (the identity cut's measure) is no pair */
#define IP_REST_JUMP_PX 48.0   /* a rest pair moving further on the screen than this (the order passes' limit before M5) is no pair */

/* BWPORT_INTERP_RESTPX: 1 (default) the rest's pairs are measured on the screen (rest_jumped) and round 2's draw order is bounded by that
 * alone; 0 = M5's rest (round 0 in draw order, no screen limit, every round within IP_ID_FAR) */
int vc_ip_restpxon = -1;
int vc_ip_restpx_on(void) {
  if (vc_ip_restpxon < 0) {
    const char *e = getenv("BWPORT_INTERP_RESTPX");
    vc_ip_restpxon = e == NULL || e[0] != '0';
  }
  return vc_ip_restpxon;
}

static void m4_aff(const IdAff *d, double *r);
static int clip_motion(const VC_IpRec *from, int k, const VC_IpRec *to, const double *d, double *m);

/* the mean distance on the screen (px, 256 x 192) of a's list p, its vertices moved by m (a's clip space to b's; NULL: as they are), from
 * b's list q: the vertices in front in both, clamped half a screen beyond each edge as the cut measures; -1 when there is none */
static double list_px(const VC_IpRec *a, const VC_IpList *p, const VC_IpRec *b, const VC_IpList *q, const double *m, const double *sh) {
  double s = 0;
  int nd = 0;
  for (u32 k = 0; k < p->n && k < q->n; k++) {
    const VC_IpVtx *u = &a->v[p->first + k], *w = &b->v[q->first + k];
    if (u->clip[3] <= 0 || w->clip[3] <= 0) continue;
    s32 o[4];
    const s32 *c = u->clip;
    if (m != NULL) {
      vc_ip_moved(u->clip, m, 1.0, o);
      if (o[3] > 0) c = o;
    }
    const double ax = clampd(128.0 * c[0] / c[3] + (sh ? sh[0] : 0), 256), ay = clampd(96.0 * c[1] / c[3] + (sh ? sh[1] : 0), 192);
    const double bx = clampd(128.0 * w->clip[0] / w->clip[3], 256), by = clampd(96.0 * w->clip[1] / w->clip[3], 192);
    s += sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
    nd++;
  }
  return nd ? s / nd : -1;
}

/* what the rest's pairs are measured against: the world's motion of each projection (pair_rest's cm, a's clip space to b's) and the
 * motion of each plausible D (identity's chosen Ds, then none) in each projection, made when first wanted ([k * (nch + 1) + q]) */
typedef struct RestM {
  double (*cm)[16];
  const int *cmok;
  double (*dm)[16];
  signed char *dmok; /* 0 not made yet, 1 made, -1 none (the projection is not known) */
  double (*sh)[2];   /* a projection with no world's D (no matrices: lists placed on the CPU): the shift on the screen of identity's */
  const int *shok;   /* pairs drawn with it (screen_shift), else none */
} RestM;
static const double *rest_dm(RestM *rm, const IdCtx *x, const VC_IpRec *a, const VC_IpRec *b, int k, int q) {
  const int n = x->nch + 1, c = k * n + q;
  if (rm->dm == NULL) {
    rm->dm = (double (*)[16])malloc(sizeof *rm->dm * (size_t)(VC_IP_NPROJ * n));
    rm->dmok = (signed char *)calloc((size_t)(VC_IP_NPROJ * n), 1);
  }
  if (!rm->dmok[c]) {
    double w4[16];
    m4_aff(q < x->nch ? &x->ch[q] : &id_one, w4);
    rm->dmok[c] = clip_motion(a, k, b, w4, rm->dm[c]) ? 1 : -1;
  }
  return rm->dmok[c] > 0 ? rm->dm[c] : NULL;
}
/* a rest pair, a's list i and b's j, whose matrices moved by identity's plausible D dq (id_far's; -1: none) jumped: it is further on the
 * screen than IP_REST_JUMP_PX plainly, beyond the world's motion and beyond its own D's, all three (the matrix's motion is plausible
 * but the vertices, placed on the CPU under one matrix, are another thing's: the newcomer of a particle run at its oldest's place, a quad
 * that wrapped the screen). *own: its distance from where its D puts it (no D: the smaller of the others), round 0's order; a list
 * with no vertex in front in both is never a jump and is weighed last */
static int rest_jumped(const VC_IpRec *a, int i, const VC_IpRec *b, int j, int dq, RestM *rm, const IdCtx *x, double *own, double *wo) {
  const VC_IpList *p = &a->l[i], *q = &b->l[j];
  const int k = (int)p->proj, known = k < VC_IP_NPROJ && (rm->cmok[k] || rm->shok[k]);
  const double plain = list_px(a, p, b, q, NULL, NULL);
  if (plain < 0) {
    *own = *wo = 1e9;
    return 0;
  }
  const double world = !known ? plain : rm->cmok[k] ? list_px(a, p, b, q, rm->cm[k], NULL) : list_px(a, p, b, q, NULL, rm->sh[k]);
  const double *dm = dq >= 0 && k < VC_IP_NPROJ ? rest_dm(rm, x, a, b, k, dq) : NULL;
  const double o = dm != NULL ? list_px(a, p, b, q, dm, NULL) : fmin(plain, world);
  /* round 0's order: beyond its own D; with none the nearer of the screen and the world's motion, a tie to the world's (a run of
   * copies placed on the CPU under a moving camera, no D: a copy now stands where its neighbour stood) */
  *own = o;
  *wo = dq < 0 ? world : 0; /* (no D fits: zero matrices) */
  return plain > IP_REST_JUMP_PX && world > IP_REST_JUMP_PX && o > IP_REST_JUMP_PX;
}

typedef struct RestL { u32 key; int idx; } RestL;
static int restl_cmp(const void *x, const void *y) {
  const RestL *p = (const RestL *)x, *q = (const RestL *)y;
  return p->key != q->key ? (p->key < q->key ? -1 : 1) : p->idx - q->idx;
}
typedef struct RestC { double e, w; int other, i, j; } RestC; /* (w: round 0's tie, other: another key) */
static int restc_cmp(const void *x, const void *y) {
  const RestC *p = (const RestC *)x, *q = (const RestC *)y;
  if (p->e != q->e) return p->e < q->e ? -1 : 1;
  if (p->w != q->w) return p->w < q->w ? -1 : 1;
  if (p->other != q->other) return p->other - q->other;
  return p->i != q->i ? p->i - q->i : p->j - q->j;
}

/* the rest's looser key: key_of with the texture's address left out (its size, format and flags kept) and, for a format that reads no
 * palette, the palette too: a texture whose image steps through phases at other addresses (HeartGold's rings at the water, untagged) */
static u32 loose_key_of(const VC_IpList *l) {
  const u32 fmt = l->texparam >> 26 & 7; /* (0 no texture and 7 direct colour read no palette) */
  u32 h = 2166136261u;
  h = mix(h, l->attr & ~0x001F0000u);
  h = mix(h, l->texparam & 0xFFFF0000u);
  h = mix(h, fmt == 0 || fmt == 7 ? 0 : l->pltt);
  h = mix(h, l->viewport);
  h = mix(h, l->type);
  return mix(h, l->n);
}

/* The rest: a's and b's lists with vertices that identity and the tags left (la / lb -1, da / db 0: shapes the CPU builds each
 * keyframe, copies that two motions fit, a look that changed past tier 1). Three rounds:
 * 0. within one loose key (loose_key_of: the texture's address left out), the pairs whose motion identity would take (id_far 0: a
 *    chosen D or none), nearest first to where that D puts them on the screen (rest_jumped's own, whole px: a run of quads placed on
 *    the CPU under one matrix moves by one plausible D whatever the pair), then the same key, then in draw order: a sprite whose
 *    animation cell changed while the camera follows it, its old cell drawn by its neighbour now (the test phone's untagged file: the
 *    player blinked at stride 4);
 * 1. within one key (key_of: the render state, the texture, the primitive and the vertex count), 2. within one loose key: as many
 *    lists left in both frames pair in draw order (the order passes' rule before M5: a run of copies, a particle emitter that kept its
 *    count), unequal counts nearest first (id_far: in a's camera space after the camera's D); only near (id_near, rule 5: within
 *    IP_ID_FAR world units: the next tile's grass is another thing), except round 2's draw order, which the screen limit alone bounds:
 *    a thing moving on its own further than 12 units a keyframe (Rock Smash's debris, the follower turning a corner: at stride 4 they
 *    popped) keeps its pair when its loose key has as many lists left in both frames (round 0 has paired what stood still by then).
 * In every round a pair that jumped on the screen (rest_jumped: further than IP_REST_JUMP_PX plainly, beyond the world's motion and
 * beyond its own D; BWPORT_INTERP_RESTPX=0: not measured) is no pair: a plausible D says nothing of vertices placed on the CPU.
 * At most IP_REST_MAXCAND candidates weighed a round (past it: draw order, equal counts only). Pairs written into la / lb; returns how
 * many. */
static int rest_pair(const VC_IpRec *a, const VC_IpRec *b, int *la, int *lb, const u8 *da, const u8 *db, const IdCtx *x, RestM *rm) {
  const int na = (int)a->nl, nb = (int)b->nl, px = vc_ip_restpx_on();
  RestL *ra = (RestL *)malloc(sizeof(RestL) * (size_t)(na + nb + 2)), *rb = ra + na + 1;
  int n = 0, cap = 0;
  RestC *c = NULL;
  for (int round = 0; round < 3; round++) {
    const int loose = round != 1;
    long budget = IP_REST_MAXCAND;
    int ma = 0, mb = 0;
    for (int i = 0; i < na; i++)
      if (la[i] < 0 && !da[i] && a->l[i].n) ra[ma++] = (RestL){loose ? loose_key_of(&a->l[i]) : key_of(&a->l[i]), i};
    for (int j = 0; j < nb; j++)
      if (lb[j] < 0 && !db[j] && b->l[j].n) rb[mb++] = (RestL){loose ? loose_key_of(&b->l[j]) : key_of(&b->l[j]), j};
    qsort(ra, (size_t)ma, sizeof *ra, restl_cmp);
    qsort(rb, (size_t)mb, sizeof *rb, restl_cmp);
    for (int g = 0, y = 0; g < ma;) {
      int ge = g, ye;
      while (ge < ma && ra[ge].key == ra[g].key) ge++;
      while (y < mb && rb[y].key < ra[g].key) y++;
      for (ye = y; ye < mb && rb[ye].key == ra[g].key; ye++) {}
      const long nc = (long)(ge - g) * (ye - y);
      int m = 0, weighed = 0;
      if (nc && nc <= budget && (round == 0 || (ge - g) != (ye - y))) {
        budget -= nc, weighed = 1;
        if (nc > cap) cap = (int)nc, c = (RestC *)realloc(c, sizeof *c * (size_t)cap);
        for (int p = g; p < ge; p++)
          for (int q = y; q < ye; q++) {
            const int i = ra[p].idx, j = rb[q].idx;
            int dq;
            const double e = id_far(x, a->l[i].mv, b->l[j].mv, &dq);
            double own = 0, wo = 0;
            if ((round == 0 ? e != 0 : e > IP_ID_FAR) || (px && rest_jumped(a, i, b, j, dq, rm, x, &own, &wo))) continue;
            c[m].e = round == 0 ? floor(own) : e, c[m].w = round == 0 ? floor(wo) : 0, c[m].other = key_of(&a->l[i]) != key_of(&b->l[j]), c[m].i = i, c[m++].j = j;
          }
        qsort(c, (size_t)m, sizeof *c, restc_cmp);
      }
      if (round == 0 || weighed) {
        for (int k = 0; k < m; k++)
          if (la[c[k].i] < 0 && lb[c[k].j] < 0) la[c[k].i] = c[k].j, lb[c[k].j] = c[k].i, n++;
      } else if (nc && ge - g == ye - y) {
        for (int k = 0; k < ge - g; k++) {
          const int i = ra[g + k].idx, j = rb[y + k].idx;
          int dq;
          double own, wo;
          if ((id_far(x, a->l[i].mv, b->l[j].mv, &dq) <= IP_ID_FAR || (px && round == 2)) && !(px && rest_jumped(a, i, b, j, dq, rm, x, &own, &wo)))
            la[i] = j, lb[j] = i, n++;
        }
      }
      g = ge, y = ye;
    }
  }
  free(c);
  free(ra);
  return n;
}

/* a tag pair (a's list p, b's q) that moved further on the screen than IP_TAG_JUMP_PX, plainly and beyond the world's motion m (a's clip
 * space to b's, or NULL) both: the game moved the instance in one step (Platinum's ledge: NPC shadows and effects drawn far off the
 * screen for one keyframe, then at their place; a slot given to another thing). Measured as the cut measures (clamped half a screen
 * beyond each edge, the vertices in front in both) */
static int tag_jumped(const VC_IpRec *a, const VC_IpList *p, const VC_IpRec *b, const VC_IpList *q, const double *m) {
  const double plain = list_px(a, p, b, q, NULL, NULL);
  return plain > IP_TAG_JUMP_PX && (m == NULL || list_px(a, p, b, q, m, NULL) > IP_TAG_JUMP_PX);
}

static int dbl_cmp(const void *x, const void *y) {
  const double p = *(const double *)x, q = *(const double *)y;
  return p < q ? -1 : p > q;
}

/* the world's shift on the screen (px of 256 x 192, a's place to b's) in projection k when no D says it (zero matrices: lists placed on
 * the CPU): of each axis the median of identity's pairs drawn with it (the mean of each pair's vertices in front in both), at least 3;
 * dx, dy: room for a's lists */
static int screen_shift(const VC_IpRec *a, const VC_IpRec *b, const int *map, int k, double *dx, double *dy, double sh[2]) {
  int n = 0;
  for (u32 i = 0; i < a->nl; i++) {
    const VC_IpList *p = &a->l[i];
    if (map[i] < 0 || (int)p->proj != k) continue;
    const VC_IpList *q = &b->l[map[i]];
    double sx = 0, sy = 0;
    int nd = 0;
    for (u32 v = 0; v < p->n && v < q->n; v++) {
      const s32 *u = a->v[p->first + v].clip, *w = b->v[q->first + v].clip;
      if (u[3] <= 0 || w[3] <= 0) continue;
      sx += 128.0 * w[0] / w[3] - 128.0 * u[0] / u[3], sy += 96.0 * w[1] / w[3] - 96.0 * u[1] / u[3], nd++;
    }
    if (nd) dx[n] = sx / nd, dy[n++] = sy / nd;
  }
  if (n < 3) return 0;
  qsort(dx, (size_t)n, sizeof *dx, dbl_cmp);
  qsort(dy, (size_t)n, sizeof *dy, dbl_cmp);
  sh[0] = dx[n / 2], sh[1] = dy[n / 2];
  return 1;
}

/* the pairs: the tags' (tag_pair), identity's (ident_pair, both tiers), the rest's (rest_pair); the world's D of each projection
 * (st->camp, for the windows) and the tag pairs that jumped (tag_jumped) left out. map / mb come back whole, how (or NULL) as
 * vc_ip_pair_how says */
static void pair_rest(const VC_IpRec *a, const VC_IpRec *b, int *map, int *mb, VC_IpStats *st, u8 *how) {
  const int na = (int)a->nl, nb = (int)b->nl;
  int *wa = (int *)malloc(sizeof(int) * (size_t)(2 * na + 2 * nb + 4)), *ta = wa + na + 1, *tb = ta + na + 1;
  u8 *da = (u8 *)malloc((size_t)(na + nb + 2)), *db = da + na + 1; /* the lists the tags decide (tag_pair) */
  tag_pair(a, b, ta, tb, da, db);
  IdCtx x;
  ident_pair(a, b, map, mb, wa, &x, 0, da, db);
  const int nlock = ident_pair(a, b, map, mb, wa, &x, 1, da, db);
  if (how != NULL)
    for (int i = 0; i < na; i++) how[i] = map[i] >= 0 ? VC_IP_HOW_ID : 0;
  st->camok = x.nch > 0 && x.camok;
  /* the world's D of each projection: the strongest chosen D that some list drawn with it moved by (an ortho layer drawn over a scene
   * of its own has its own; a projection none of whose lists moved by a chosen D has none: vc_ip_windows moves nothing then) */
  for (int k = 0; k < VC_IP_NPROJ && st->camok; k++)
    for (int q = 0; q < x.nch; q++)
      if (x.chpm[q] >> k & 1) {
        st->camproj |= 1 << k;
        memcpy(st->camp[k], x.ch[q].a, 9 * sizeof(double)), memcpy(st->camp[k] + 9, x.ch[q].t, 3 * sizeof(double));
        break;
      }
  double cm[VC_IP_NPROJ][16];
  int cmok[VC_IP_NPROJ];
  for (int k = 0; k < VC_IP_NPROJ; k++) {
    cmok[k] = 0;
    if (!(st->camproj >> k & 1)) continue;
    IdAff d;
    double w4[16];
    memcpy(d.a, st->camp[k], sizeof d.a), memcpy(d.t, st->camp[k] + 9, sizeof d.t);
    m4_aff(&d, w4);
    cmok[k] = clip_motion(a, k, b, w4, cm[k]);
  }
  double sh[VC_IP_NPROJ][2], *sxy = vc_ip_restpx_on() ? (double *)malloc(sizeof(double) * (size_t)(2 * na + 2)) : NULL;
  int shok[VC_IP_NPROJ];
  for (int k = 0; k < VC_IP_NPROJ; k++) shok[k] = sxy != NULL && !cmok[k] && screen_shift(a, b, map, k, sxy, sxy + na + 1, sh[k]);
  free(sxy);
  int ntp = 0;
  for (int i = 0; i < na; i++) {
    const int j = ta[i];
    if (j < 0) continue;
    const int k = a->l[i].proj;
    if (tag_jumped(a, &a->l[i], b, &b->l[j], k < VC_IP_NPROJ && cmok[k] ? cm[k] : NULL)) {
      st->jumps++;
      if (how != NULL) how[i] = VC_IP_HOW_JUMPED;
      continue;
    }
    map[i] = j, mb[j] = i, ntp++;
    if (how != NULL) how[i] = VC_IP_HOW_TAG;
  }
  for (int i = 0; i < na; i++) st->tagdec += da[i];
  RestM rm = {cm, cmok, NULL, NULL, sh, shok};
  const int nrest = rest_pair(a, b, map, mb, da, db, &x, &rm);
  free(rm.dm), free(rm.dmok);
  if (how != NULL)
    for (int i = 0; i < na; i++)
      if (map[i] >= 0 && !how[i]) how[i] = VC_IP_HOW_REST;
  st->ident = nlock;
  st->tagged = ntp;
  st->idcand = x.ncand;
  st->strict = nlock + ntp;
  st->loose = nrest;
  free(wa), free(da);
}

static int win_on(void);
void vc_ip_pair(const VC_IpRec *a, const VC_IpRec *b, int *map, double limit, VC_IpStats *st) { vc_ip_pair_how(a, b, map, NULL, limit, st); }

void vc_ip_pair_how(const VC_IpRec *a, const VC_IpRec *b, int *map, u8 *how, double limit, VC_IpStats *st) {
  (void)limit;
  memset(st, 0, sizeof *st);
  if (how != NULL) memset(how, 0, a->nl);
  const int na = (int)a->nl, nb = (int)b->nl;
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
  int *mb = (int *)malloc(sizeof(int) * (size_t)nb); /* b's list -> a's list */
  for (int j = 0; j < nb; j++) mb[j] = -1;
  pair_rest(a, b, map, mb, st, how);
  /* the behind rule: a pair crossing the camera plane with no vertex in front in both frames has no motion to measure (dropped, it is
   * drawn at a's place); the paired lists' screen motion for the stats, clamped to a band half a screen beyond each edge, a vertex
   * behind the camera in either frame left out (the map's ground pieces reach under the camera, where a tiny w throws the projection
   * thousands of pixels out). The blend itself is linear in clip space, so a vertex crossing w = 0 needs no guard: the clipper runs
   * after it. */
  double dsum = 0;
  u64 dn = 0;
  int all_same = na == nb, used = 0;
  for (int i = 0; i < na; i++) {
    if (map[i] < 0) continue;
    const VC_IpList *p = &a->l[i], *q = &b->l[map[i]];
    double lsum = 0, lmax = 0;
    int ident = 1, flip = 0, nd = 0;
    for (u32 k = 0; k < p->n; k++) {
      const VC_IpVtx *u = &a->v[p->first + k], *w = &b->v[q->first + k];
      if (memcmp(u, w, sizeof *u) != 0) ident = 0;
      if ((u->clip[3] <= 0) != (w->clip[3] <= 0)) flip = 1; /* crosses the camera plane between the frames */
      if (u->clip[3] <= 0 || w->clip[3] <= 0) continue;
      const double ax = clampd(128.0 * u->clip[0] / u->clip[3], 256), ay = clampd(96.0 * u->clip[1] / u->clip[3], 192);
      const double bx = clampd(128.0 * w->clip[0] / w->clip[3], 256), by = clampd(96.0 * w->clip[1] / w->clip[3], 192);
      const double d = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
      lsum += d;
      nd++;
      if (d > lmax) lmax = d;
    }
    if (flip && nd == 0) {
      mb[map[i]] = -1;
      map[i] = -1;
      if (how != NULL) how[i] = VC_IP_HOW_BEHIND;
      st->behind++;
      all_same = 0;
      continue;
    }
    if (!ident) all_same = 0;
    used++;
    dsum += lsum;
    dn += p->n;
    if (lmax > st->disp_max) st->disp_max = lmax;
    st->vt_paired += p->n;
  }
  st->disp_mean = dn ? dsum / (double)dn : 0;
  st->removed = na - used;
  st->added = nb - used;
  u32 vr = 0, va = 0;
  for (int i = 0; i < na; i++)
    if (map[i] < 0) vr += a->l[i].n;
  for (int j = 0; j < nb; j++)
    if (mb[j] < 0) va += b->l[j].n;
  st->vt_removed = vr;
  st->vt_added = va;
  st->same = all_same && used == na && used == nb;
  free(mb);
}

/* ---- the visibility windows (docs/interp-sweep.md, M2; INTERP-ARCH 3.1): every list with no partner, by the projection test ---- */

int vc_ip_win = -1;
static int win_on(void) {
  if (vc_ip_win < 0) {
    const char *e = getenv("BWPORT_INTERP_WINDOWS");
    vc_ip_win = e == NULL || e[0] != '0';
  }
  return vc_ip_win;
}

static const double kOne4[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
static void m4_mul(const double *x, const double *y, double *r) { /* x, then y (row vectors) */
  double t[16];
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) t[i * 4 + j] = x[i * 4] * y[j] + x[i * 4 + 1] * y[4 + j] + x[i * 4 + 2] * y[8 + j] + x[i * 4 + 3] * y[12 + j];
  memcpy(r, t, sizeof t);
}
static int m4_inv(const double *m, double *r) { /* Gauss-Jordan with partial pivoting; 0: singular */
  double a[4][8];
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 8; j++) a[i][j] = j < 4 ? m[i * 4 + j] : (double)(j - 4 == i);
  for (int c = 0; c < 4; c++) {
    int p = c;
    for (int i = c + 1; i < 4; i++)
      if (fabs(a[i][c]) > fabs(a[p][c])) p = i;
    if (fabs(a[p][c]) < 1e-12) return 0;
    for (int j = 0; j < 8 && p != c; j++) {
      const double t = a[c][j];
      a[c][j] = a[p][j], a[p][j] = t;
    }
    const double iv = 1.0 / a[c][c];
    for (int j = 0; j < 8; j++) a[c][j] *= iv;
    for (int i = 0; i < 4; i++) {
      const double k = a[i][c];
      if (i == c || k == 0) continue;
      for (int j = 0; j < 8; j++) a[i][j] -= k * a[c][j];
    }
  }
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) r[i * 4 + j] = a[i][4 + j];
  return 1;
}
static void m4_aff(const IdAff *d, double *r) { /* a motion of camera space (4x3) as a 4x4 */
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) r[i * 4 + j] = d->a[i * 3 + j];
    r[i * 4 + 3] = 0;
  }
  for (int j = 0; j < 3; j++) r[12 + j] = d->t[j];
  r[15] = 1;
}
/* the clip-space motion of a vertex drawn with from's projection k that moves by d (camera space) into the frame to: inverse(P) x d x
 * P', P' to's projection that P is (the same matrix, else the one first used in the same place, else P). 0: P unknown (the bucket of
 * the rest) or singular */
static int clip_motion(const VC_IpRec *from, int k, const VC_IpRec *to, const double *d, double *m) {
  double p[16], q[16], ip[16];
  if (k >= (int)from->nproj) return 0;
  for (int i = 0; i < 16; i++) p[i] = from->projm[k][i] / 4096.0;
  if (!m4_inv(p, ip)) return 0;
  int k2 = -1;
  for (u32 t = 0; t < to->nproj && k2 < 0; t++)
    if (to->projh[t] == from->projh[k]) k2 = (int)t;
  if (k2 < 0 && k < (int)to->nproj) k2 = k;
  if (k2 >= 0)
    for (int i = 0; i < 16; i++) q[i] = to->projm[k2][i] / 4096.0;
  else memcpy(q, p, sizeof q);
  m4_mul(ip, d, m);
  m4_mul(m, q, m);
  return 1;
}
/* vc_ip_list_seen of r's list l with every vertex moved by m */
static int seen_moved(const VC_IpRec *r, const VC_IpList *l, const double *m) {
  unsigned all = 63;
  for (u32 j = 0; j < l->n && all != 0; j++) {
    double o[4];
    m4_vtx(r->v[l->first + j].clip, m, o);
    const double x = o[0], y = o[1], z = o[2], w = o[3];
    all &= (unsigned)(x < -w) | (unsigned)(x > w) << 1 | (unsigned)(y < -w) << 2 | (unsigned)(y > w) << 3 | (unsigned)(z < -w) << 4 |
           (unsigned)(z > w) << 5;
  }
  return l->n > 0 && all == 0;
}
/* the mean screen motion (px, 256 x 192, y up) of r's list l moved by m, over its vertices in front before and after */
static void moved_by(const VC_IpRec *r, const VC_IpList *l, const double *m, float *dx, float *dy) {
  double sx = 0, sy = 0;
  int n = 0;
  for (u32 j = 0; j < l->n; j++) {
    const s32 *c = r->v[l->first + j].clip;
    double o[4];
    m4_vtx(c, m, o);
    if (c[3] <= 0 || o[3] <= 0) continue;
    sx += 128.0 * (o[0] / o[3] - (double)c[0] / c[3]), sy += 96.0 * (o[1] / o[3] - (double)c[1] / c[3]);
    n++;
  }
  *dx = n ? (float)(sx / n) : 0, *dy = n ? (float)(sy / n) : 0;
}
/* the motion now (a to b, as identity's D) of the rigid group b's list j moved with in the pair before: of the b lists whose motion to
 * c fits j's (dp, history bit 0) and that have a partner in a (dn, bit 1), the motion most of them have; 0: none */
static int group_d(const IdAff *dp, const IdAff *dn, const u8 *hok, int nb, int j, IdAff *g) {
  IdAff rep[8];
  int cnt[8], nr = 0, best = -1;
  for (int k = 0; k < nb; k++) {
    if (k == j || hok[k] != 3 || !id_fits(&dp[k], &dp[j])) continue;
    int r = 0;
    while (r < nr && !id_fits(&rep[r], &dn[k])) r++;
    if (r == nr) {
      if (nr == 8) continue;
      rep[nr] = dn[k], cnt[nr++] = 0;
    }
    cnt[r]++;
  }
  for (int r = 0; r < nr; r++)
    if (best < 0 || cnt[r] > cnt[best]) best = r;
  if (best < 0) return 0;
  *g = rep[best];
  return 1;
}

/* a list the tags decided with no partner, l (of a or b): its owner group motion from a's camera space to b's (identity's D: v x g):
 * the D most of the tag pairs of that owner have (ties: the first in a's order); a shadow's (VC_IP_TAG_SHADOW): its sprite's first, the tag
 * pairs of another kind with its inst; an overlay's: none (it switches where it is). 0: no such pair (the world's motion then). memo:
 * the answers so far (nmemo of at most 32) */
typedef struct TagGrp { u32 owner, inst; int ok; IdAff g; } TagGrp;
static int tag_group_d(const VC_IpRec *a, const VC_IpRec *b, const int *map, const VC_IpList *l, TagGrp *memo, int *nmemo, IdAff *g) {
  static const IdAff one = {{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}};
  const u32 kind = VC_IP_TAG_KIND(l->owner);
  if (kind == VC_IP_TAG_OVERLAY) return *g = one, 1;
  for (int q = 0; q < *nmemo; q++)
    if (memo[q].owner == l->owner && (kind != VC_IP_TAG_SHADOW || memo[q].inst == l->inst)) return *g = memo[q].g, memo[q].ok;
  int ok = 0;
  for (int pass = kind == VC_IP_TAG_SHADOW ? 0 : 1; pass < 2 && !ok; pass++) {
    IdAff rep[8];
    int cnt[8], nr = 0, best = -1;
    for (u32 i = 0; i < a->nl; i++) {
      const int j = map[i];
      if (j < 0 || j >= (int)b->nl) continue;
      const VC_IpList *p = &a->l[i], *q = &b->l[j];
      const u32 pk = VC_IP_TAG_KIND(p->owner);
      if (!p->owner || p->owner != q->owner || p->inst != q->inst) continue; /* (a tag pair) */
      if (pass ? p->owner != l->owner : (p->inst != l->inst || pk == VC_IP_TAG_SHADOW || pk == VC_IP_TAG_OVERLAY)) continue;
      IdAff mp, ip, mq, d;
      id_aff(p->mv, &mp);
      if (!id_inv(&mp, &ip)) continue;
      id_aff(q->mv, &mq);
      id_mul(&ip, &mq, &d);
      int r = 0;
      while (r < nr && !id_fits(&rep[r], &d)) r++;
      if (r == nr) {
        if (nr == 8) continue;
        rep[nr] = d, cnt[nr++] = 0;
      }
      cnt[r]++;
    }
    for (int r = 0; r < nr; r++)
      if (best < 0 || cnt[r] > cnt[best]) best = r;
    if (best >= 0) *g = rep[best], ok = 1;
  }
  if (*nmemo < 32) memo[(*nmemo)++] = (TagGrp){l->owner, l->inst, ok, ok ? *g : one};
  return ok;
}

/* M4 (INTERP-ARCH 3.3): the unpaired lists that may be on the screen of each instance (owner, inst; not a particle: a slot's next life
 * is another particle) with such lists in both frames: swa[i] / swb[j] = 1 (zeroed by the caller) */
static void swap_sets(const VC_IpRec *a, const VC_IpRec *b, const int *map, const int *mb, u8 *swa, u8 *swb) {
  const int na = (int)a->nl, nb = (int)b->nl;
  TagL *sa = (TagL *)malloc(sizeof(TagL) * (size_t)(na + nb + 2)), *sb = sa + na + 1;
  int ma = 0, mbn = 0;
  for (int i = 0; i < na; i++) {
    const VC_IpList *l = &a->l[i];
    if (map[i] == -1 && l->owner && l->n && VC_IP_TAG_KIND(l->owner) != VC_IP_TAG_PARTICLE && vc_ip_list_seen(a, l))
      sa[ma++] = (TagL){l->owner, l->inst, i};
  }
  for (int j = 0; j < nb && ma; j++) {
    const VC_IpList *l = &b->l[j];
    if (mb[j] < 0 && l->owner && l->n && VC_IP_TAG_KIND(l->owner) != VC_IP_TAG_PARTICLE && vc_ip_list_seen(b, l))
      sb[mbn++] = (TagL){l->owner, l->inst, j};
  }
  qsort(sa, (size_t)ma, sizeof *sa, tagl_cmp);
  qsort(sb, (size_t)mbn, sizeof *sb, tagl_cmp);
  for (int g = 0, y = 0; g < ma && mbn;) {
    int ge = g;
    while (ge < ma && !tagk_cmp(&sa[ge], &sa[g])) ge++;
    while (y < mbn && tagk_cmp(&sb[y], &sa[g]) < 0) y++;
    int ye = y;
    while (ye < mbn && !tagk_cmp(&sb[ye], &sa[g])) ye++;
    if (ye > y) {
      for (int p = g; p < ge; p++) swa[sa[p].idx] = 1;
      for (int q = y; q < ye; q++) swb[sb[q].idx] = 1;
    }
    g = ge, y = ye;
  }
  free(sa);
}

int vc_ip_windows(const VC_IpRec *a, const VC_IpRec *b, int *map, const VC_IpRec *c, const int *mc, VC_IpStats *st, VC_IpKeep *keep,
                  double (*enter)[16]) {
  st->fresh = st->entering = st->leaving = st->gone = st->swapped = 0;
  if (!win_on()) return 0; /* (BWPORT_INTERP_WINDOWS=0: every list with no partner drawn at a's place, none of b's kept) */
  for (int k = 0; k < VC_IP_NENTER; k++) memcpy(enter[k], kOne4, sizeof kOne4);
  if (a->nl == 0 || b->nl == 0) return 0;
  const int na = (int)a->nl, nb = (int)b->nl;
  /* the world's motion by projection: a's camera space to b's (st->camp), and back; none known: no motion */
  double w4[VC_IP_NPROJ][16], wi4[VC_IP_NPROJ][16];
  int eok[VC_IP_NPROJ];
  for (int k = 0; k < VC_IP_NPROJ; k++) {
    memcpy(w4[k], kOne4, sizeof w4[k]);
    if (st->camproj >> k & 1) {
      IdAff d;
      memcpy(d.a, st->camp[k], sizeof d.a);
      memcpy(d.t, st->camp[k] + 9, sizeof d.t);
      m4_aff(&d, w4[k]);
    }
    if (!m4_inv(w4[k], wi4[k])) memcpy(w4[k], kOne4, sizeof w4[k]), memcpy(wi4[k], kOne4, sizeof wi4[k]);
    if (!(eok[k] = clip_motion(a, k, b, w4[k], enter[k]))) memcpy(enter[k], kOne4, sizeof kOne4);
  }
  int *mb = (int *)malloc(sizeof(int) * (size_t)(nb + 1));
  for (int j = 0; j < nb; j++) mb[j] = -1;
  for (int i = 0; i < na; i++)
    if (map[i] >= 0 && map[i] < nb) mb[map[i]] = i;
  /* a's lists: entering where the game could not have drawn them in b, else new; a list the tags decided by its owner group motion
   * (tag_group_d; its entering motion in enter[VC_IP_NPROJ..]) */
  const int tags = vc_ip_tags_on() && a->ntag && b->ntag;
  /* M4 (vc_ip_state_on; INTERP-ARCH 3.3, 3.5): an instance that changed form switches at the middle (VC_IP_SWAP, VC_IpKeep.half); a
   * particle is never entering or leaving (new or gone: a newborn or a dead one), an overlay never glides (new or gone in place) */
  const int m4 = tags && vc_ip_state_on();
  u8 *swa = NULL, *swb = NULL;
  if (m4) {
    swa = (u8 *)calloc((size_t)(na + nb + 2), 1), swb = swa + na + 1;
    swap_sets(a, b, map, mb, swa, swb);
  }
  TagGrp tmemo[32];
  int ntmemo = 0, nslot = VC_IP_NPROJ;
  for (int i = 0; i < na; i++) {
    const VC_IpList *l = &a->l[i];
    if (map[i] != -1 || !l->n || !vc_ip_list_seen(a, l)) continue;
    IdAff g;
    double g4[16], m[16];
    const u32 kind = VC_IP_TAG_KIND(l->owner);
    if (swa != NULL && swa[i]) { /* moved back as its owner group (an overlay: in place; none known: the world's) */
      int k = l->proj < VC_IP_NPROJ ? l->proj : 0;
      if (kind == VC_IP_TAG_OVERLAY) memcpy(m, kOne4, sizeof m), k = -1;
      else if (tag_group_d(a, b, map, l, tmemo, &ntmemo, &g) && (m4_aff(&g, g4), clip_motion(a, l->proj, b, g4, m))) k = -1;
      if (k < 0) {
        k = VC_IP_NPROJ;
        while (k < nslot && memcmp(enter[k], m, sizeof m)) k++;
        if (k == VC_IP_NENTER) k = l->proj < VC_IP_NPROJ ? l->proj : 0; /* (no slot left: the world's) */
        else memcpy(enter[k], m, sizeof m), nslot += k == nslot;
      }
      map[i] = VC_IP_SWAP(k), st->swapped++;
      continue;
    }
    const int still = m4 && l->owner && (kind == VC_IP_TAG_PARTICLE || kind == VC_IP_TAG_OVERLAY);
    if (tags && l->owner && tag_group_d(a, b, map, l, tmemo, &ntmemo, &g) && (m4_aff(&g, g4), clip_motion(a, l->proj, b, g4, m))) {
      int k = VC_IP_NPROJ;
      while (k < nslot && memcmp(enter[k], m, sizeof m)) k++;
      if (still || seen_moved(a, l, m) || k == VC_IP_NENTER) map[i] = VC_IP_NEW, st->fresh++;
      else memcpy(enter[k], m, sizeof m), nslot += k == nslot, map[i] = VC_IP_ENTER(k), st->entering++;
      continue;
    }
    if (!still && l->proj < VC_IP_NPROJ && eok[l->proj] && !seen_moved(a, l, enter[l->proj])) map[i] = VC_IP_ENTER(l->proj), st->entering++;
    else map[i] = VC_IP_NEW, st->fresh++;
  }
  /* b's lists: kept until a, leaving with the world or gone with their group */
  IdAff *dp = NULL, *dn = NULL;
  u8 *hok = NULL; /* the history (made when a gone list first needs it): bit 0 dp (b to c), bit 1 dn (a to b) */
  IdAff mdp[16], mg[16]; /* group_d's answers by j's motion in the pair before */
  int mok[16], nmemo = 0, nk = 0;
  for (int j = 0; j < nb; j++) {
    const VC_IpList *l = &b->l[j];
    if (mb[j] >= 0 || !l->n || !vc_ip_list_seen(b, l)) continue;
    double m[16], g4[16], gi4[16];
    IdAff g;
    const int tagged = tags && l->owner && tag_group_d(a, b, map, l, tmemo, &ntmemo, &g) && (m4_aff(&g, g4), m4_inv(g4, gi4)) &&
                       clip_motion(b, l->proj, a, gi4, m); /* (its owner group: tag_group_d) */
    const int known = tagged || clip_motion(b, l->proj, a, wi4[l->proj], m);
    if (!known) memcpy(m, kOne4, sizeof m);
    const u32 kind = VC_IP_TAG_KIND(l->owner);
    const int half = swb != NULL && swb[j], still = m4 && l->owner && (kind == VC_IP_TAG_PARTICLE || kind == VC_IP_TAG_OVERLAY);
    if (m4 && l->owner && kind == VC_IP_TAG_OVERLAY) memcpy(m, kOne4, sizeof m);
    const int how = known && !half && !still && !seen_moved(b, l, m) ? VC_IP_LEAVING : VC_IP_GONE;
    if (how == VC_IP_GONE && known && !tagged && c != NULL && mc != NULL) {
      if (hok == NULL) {
        dp = (IdAff *)malloc(sizeof(IdAff) * 2 * (size_t)(nb + 1)), dn = dp + nb + 1;
        hok = (u8 *)calloc((size_t)nb + 1, 1);
        for (int k = 0; k < nb; k++) {
          IdAff mk, ik, mo;
          id_aff(b->l[k].mv, &mk);
          if (!b->l[k].n || !id_inv(&mk, &ik)) continue;
          if (mc[k] >= 0 && mc[k] < (int)c->nl) id_aff(c->l[mc[k]].mv, &mo), id_mul(&ik, &mo, &dp[k]), hok[k] |= 1;
          if (mb[k] >= 0 && (id_aff(a->l[mb[k]].mv, &mo), id_inv(&mo, &ik))) id_mul(&ik, &mk, &dn[k]), hok[k] |= 2;
        }
      }
      if (hok[j] & 1) {
        int q = 0;
        while (q < nmemo && !id_fits(&mdp[q], &dp[j])) q++;
        if (q == nmemo && nmemo < 16) mdp[nmemo] = dp[j], mok[nmemo] = group_d(dp, dn, hok, nb, j, &mg[nmemo]), nmemo++;
        double g4[16], gi4[16];
        if (q < nmemo && mok[q] && (m4_aff(&mg[q], g4), m4_inv(g4, gi4))) clip_motion(b, l->proj, a, gi4, m);
      }
    }
    VC_IpKeep *k = &keep[nk++];
    memset(k, 0, sizeof *k);
    k->list = j;
    k->after = -1;
    for (int q = j - 1; q >= 0 && k->after < 0; q--) k->after = mb[q];
    k->win = how;
    k->half = half;
    memcpy(k->m, m, sizeof m);
    moved_by(b, l, m, &k->dx, &k->dy);
    if (how == VC_IP_LEAVING) st->leaving++;
    else st->gone++;
  }
  if (nk > 1) qsort(keep, (size_t)nk, sizeof *keep, keep_cmp);
  free(mb);
  free(dp);
  free(hok);
  free(swa);
  return nk;
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
              "same=%d dmean=%.2f dmax=%.1f",
          frame, st.swapped_a, st.swapped_b, st.nl_a, st.nl_b, st.nv_a, st.nv_b, cur->npoly, st.strict, st.loose, st.removed,
          st.added, st.jumps, st.behind, st.vt_paired, st.vt_removed, st.vt_added, st.same, st.disp_mean,
          st.disp_max);
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
