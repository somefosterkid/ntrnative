/* port/vcore piece 14: vertices, polygon assembly, clipping, viewport (docs/vcore.md, piece 14). OWNED BY PIECE 14.
 * Spec: GBATEK 44 (vertex commands, primitive types, clipping), 43 (POLYGON_ATTR), 35 (VIEWPORT, DISP_1DOT_DEPTH),
 * 33 (the two RAMs), 53 (POS_TEST/BOX_TEST). */
#include "vc_shared.h"

/* ---- vertices ---- */

static s32 sext(u32 v, int bits) {
  u32 m = 1u << (bits - 1);
  v &= (1u << bits) - 1;
  return (s32)((v ^ m) - m);
}

/* GBATEK 44, "Notes on VTX commands": (xx, yy, zz, ww) = (x, y, z, 1.0) * ClipMatrix */
static void transform(VCoreState *s, const s32 pos[3], s32 out[4]) {
  s32 in[4] = {pos[0], pos[1], pos[2], 0x1000};
  vc_mtx_apply(vc_mtx_clip(s), in, out);
}

/* GBATEK 44 ("screen_x = (xx+ww)*viewport_width / (2*ww) + viewport_x1"; the viewport's y counts from the bottom
 * (GBATEK 35), so the screen y runs from the top edge y = 191 - y2 downwards) and the 24-bit depth z-buffer: z in
 * [-w, w] -> 0..0xFFFE00 ((z/w * 0x4000 + 0x3FFF) * 0x200; the 15-bit depth the clear register's range 0..7FFF
 * counts in, times 0x200). */
void vc_p14_viewport(VC_Vertex *v, int x1, int y1, int x2, int y2) {
  s64 w = v->clip[3];
  if (w <= 0) w = 1;
  s64 vw = (x2 - x1 + 1) & 0x1FF, vh = (y2 - y1 + 1) & 0xFF;
  v->sx = (s32)(((s64)(v->clip[0] + w) * vw) / (2 * w)) + x1;
  v->sy = (s32)(((s64)(w - v->clip[1]) * vh) / (2 * w)) + (191 - y2);
  s64 z = (((s64)v->clip[2] * 0x4000) / w + 0x3FFF) * 0x200;
  if (z < 0) z = 0;
  if (z > 0xFFFFFF) z = 0xFFFFFF;
  v->sz = (s32)z;
}

/* ---- facing ---- */

/* The sign of the determinant of (x, y, w) of the three vertices: with w > 0 it is the sign of the screen area
 * (positive = anti-clockwise in the y-up view space = front, GBATEK 44: "normally arranged anti-clockwise"). */
int vc_p14_facing(const VC_Vertex *a, const VC_Vertex *b, const VC_Vertex *c) {
  s64 m[3][3] = {{a->clip[0], a->clip[1], a->clip[3]},
                 {b->clip[0], b->clip[1], b->clip[3]},
                 {c->clip[0], c->clip[1], c->clip[3]}};
  s64 mx = 0;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      s64 q = m[i][j] < 0 ? -m[i][j] : m[i][j];
      if (q > mx) mx = q;
    }
  int sh = 0; /* keep every product below 2^62 in the sum */
  while ((mx >> sh) >= (1 << 20)) sh++;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) m[i][j] = m[i][j] >= 0 ? (m[i][j] >> sh) : -((-m[i][j]) >> sh);
  s64 det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
            m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
  return det > 0 ? 1 : det < 0 ? -1 : 0;
}

/* ---- clipping ---- */

/* the distance of a vertex inside plane p (>= 0 = inside): planes 0/1 x >= -w / x <= w, 2/3 y, 4/5 z */
static s64 plane_dist(const VC_Vertex *v, int p) {
  s64 c = v->clip[p >> 1], w = v->clip[3];
  return (p & 1) ? w - c : w + c;
}

static s32 lerp32(s32 a, s32 b, s64 num, s64 den) {
  return (s32)(a + (s64)(b - a) * num / den);
}

/* the point where edge a->b meets plane p; da, db the distances of its ends (of different sign) */
static void intersect(const VC_Vertex *a, const VC_Vertex *b, s64 da, s64 db, int plane, VC_Vertex *o) {
  s64 num = da, den = da - db;
  memset(o, 0, sizeof *o);
  for (int i = 0; i < 4; i++) o->clip[i] = lerp32(a->clip[i], b->clip[i], num, den);
  for (int i = 0; i < 3; i++) o->color[i] = (u8)lerp32(a->color[i], b->color[i], num, den);
  for (int i = 0; i < 2; i++) o->tex[i] = (s16)lerp32(a->tex[i], b->tex[i], num, den);
  /* the new vertex lies exactly on its plane (the per-coordinate roundings above would leave it a unit off,
   * and a polygon clipped to the screen edge would then stop one dot short) */
  o->clip[plane >> 1] = (plane & 1) ? o->clip[3] : -o->clip[3];
  o->clipped = 1;
}

enum { CLIP_MAX = 16 };

/* The planes are clipped in the order near, far, left, right, bottom, top (oracle: the order matters because a
 * polygon that is not planar in clip space, a quad with unequal depths, is cut along chords made by the earlier planes;
 * x and y first gave quads that cross the near plane and several side planes edges 30 dots off; 400 probes, 20962
 * differing dots before, 917 after; triangles are planar and did not tell). Plane index: 2k left/bottom/near, 2k+1
 * right/top/far, k = x, y, z. */
static const int clip_order[6] = {4, 5, 0, 1, 2, 3};

int vc_p14_clip(const VC_Vertex *in, int n, int far_ok, VC_Vertex *out) {
  VC_Vertex a[CLIP_MAX], b[CLIP_MAX];
  if (n > 10) n = 10;
  /* GBATEK 43 bit 12: a polygon that crosses the far plane is hidden unless the bit is set */
  if (!far_ok)
    for (int i = 0; i < n; i++)
      if (plane_dist(&in[i], 5) < 0) return 0;
  const VC_Vertex *src = in;
  VC_Vertex *dst = a;
  for (int oi = 0; oi < 6; oi++) {
    int p = clip_order[oi];
    int outside = 0; /* every vertex inside the plane: its pass would give them back as they are */
    for (int i = 0; i < n && !outside; i++) outside = plane_dist(&src[i], p) < 0;
    if (!outside) {
      if (n < 3) return 0;
      continue;
    }
    int m = 0;
    for (int i = 0, j = n - 1; i < n; j = i++) {
      const VC_Vertex *cur = &src[i], *prev = &src[j];
      s64 dc = plane_dist(cur, p), dp = plane_dist(prev, p);
      if (dc >= 0) {
        if (dp < 0 && m < CLIP_MAX) intersect(prev, cur, dp, dc, p, &dst[m++]);
        if (m < CLIP_MAX) dst[m++] = *cur;
      } else if (dp >= 0 && m < CLIP_MAX) {
        intersect(prev, cur, dp, dc, p, &dst[m++]);
      }
    }
    n = m;
    src = dst;
    dst = dst == a ? b : a;
    if (n < 3) return 0;
  }
  if (n > 10) n = 10; /* the polygon RAM holds 10 vertices */
  for (int i = 0; i < n; i++) out[i] = src[i];
  return n;
}

/* ---- polygon assembly ---- */

void vc_poly_reset(VCoreState *s) {
  VC_P14 *p = &s->p14;
  memset(p, 0, sizeof *p);
  p->vp_x2 = 255;
  p->vp_y2 = 191;
  p->type = 0;
  vc_ip_reset(s);
  s->geo.nvtx = 0;
  s->geo.npoly = 0;
  s->geo.overflow = 0;
  s->geo.swapped = 0;
  for (u32 i = 0; i < VC_MAX_POLYS; i++) s->geo.order[i] = (u16)i;
}

/* one polygon from n pending vertices (the order of a polygon's vertices: GBATEK 44's diagrams) */
static void emit_polygon(VCoreState *s, VC_P14Pend *pv[4], int n) {
  VC_P14 *st = &s->p14;
  VC_GeomOut *g = &s->geo;
  if (g->npoly >= VC_MAX_POLYS) {
    g->overflow = 1;
    return;
  }

  /* culling by facing (POLYGON_ATTR bits 6 back / 7 front); a degenerate polygon (a line segment) is always drawn */
  int face = vc_p14_facing(&pv[0]->v, &pv[1]->v, &pv[2]->v);
  int front = face >= 0;
  if (face > 0 && !(st->attr & 0x80)) return;
  if (face < 0 && !(st->attr & 0x40)) return;

  VC_Vertex cv[4], ov[10];
  int need_clip = 0;
  for (int i = 0; i < n; i++) {
    const s32 *c = pv[i]->v.clip;
    s64 w = c[3];
    for (int k = 0; k < 3; k++)
      if (c[k] < -w || c[k] > w) need_clip = 1;
  }
  int cnt = n;
  u32 vpkey = (u32)st->vp_x1 | (u32)st->vp_y1 << 8 | (u32)st->vp_x2 << 16 | (u32)st->vp_y2 << 24;
  if (need_clip) {
    for (int i = 0; i < n; i++) cv[i] = pv[i]->v;
    cnt = vc_p14_clip(cv, n, (st->attr >> 12) & 1, ov);
    if (cnt < 3) return;
    for (int i = 0; i < cnt; i++) vc_p14_viewport(&ov[i], st->vp_x1, st->vp_y1, st->vp_x2, st->vp_y2);
  } else {
    for (int i = 0; i < n; i++) {
      VC_P14Pend *q = pv[i];
      if (!q->vpok || q->vpkey != vpkey) {
        vc_p14_viewport(&q->v, st->vp_x1, st->vp_y1, st->vp_x2, st->vp_y2);
        q->vpok = 1;
        q->vpkey = vpkey;
      }
      ov[i] = q->v;
    }
  }
  /* which pending vertex each output vertex is (NULL: made by clipping). Oracle: a clipped polygon keeps the
   * unclipped corners it still has as the same vertex RAM entries and adds only the new ones (a quad strip's third
   * quad clipped at the side: the oracle has 11 vertices where giving the whole polygon fresh ones makes 13). */
  VC_P14Pend *src[10];
  int ram[10];
  int fresh = 0; /* vertex RAM entries to add */
  for (int i = 0; i < cnt; i++) {
    src[i] = NULL;
    if (!need_clip) src[i] = pv[i];
    if (!src[i]) fresh++;
  }
  for (int k = 0; k < n; k++) {
    int used = 0;
    for (int i = 0; i < cnt; i++) used |= src[i] == pv[k];
    if (used && pv[k]->ram < 0) fresh++;
  }
  /* A clipped polygon writes all its output vertices as new entries and registers none of them, so the next polygon
   * of a strip writes its shared corners again (oracle: a clipped triangle strip of 4 vertices has 7 entries; 278 of
   * 300 random strip scenes match exactly, the rest involve polygons this piece drops or keeps differently). */
  if (g->nvtx + (u32)fresh > VC_MAX_VERTS) {
    g->overflow = 1;
    return;
  }

  /* extent, and the 1-dot rule (GBATEK 35: a 0x0-dot polygon is shown only when bit 13 is set and some vertex has
   * w <= DISP_1DOT_DEPTH, a 12.3 number: w is 20.12) */
  s32 ymin = ov[0].sy, ymax = ov[0].sy, xmin = ov[0].sx, xmax = ov[0].sx;
  for (int i = 1; i < cnt; i++) {
    if (ov[i].sy < ymin) ymin = ov[i].sy;
    if (ov[i].sy > ymax) ymax = ov[i].sy;
    if (ov[i].sx < xmin) xmin = ov[i].sx;
    if (ov[i].sx > xmax) xmax = ov[i].sx;
  }
  if (xmin == xmax && ymin == ymax) {
    u32 depth = vc_io16(s, VC_REG_DISP_1DOT_DEPTH) & 0x7FFF;
    int near = 0;
    for (int i = 0; i < cnt; i++)
      if ((u32)ov[i].clip[3] >> 9 <= depth) near = 1;
    if (!((st->attr >> 13) & 1) || !near) return;
  }

  /* commit: new vertex RAM entries in the order the vertices arrived, clipped ones as made */
  if (!need_clip)
    for (int r = 0; r < 4; r++)
      for (int k = 0; k < n; k++)
        if (pv[k] == &st->ring[r] && pv[k]->ram < 0) pv[k]->ram = (int)g->nvtx++;
  for (int i = 0; i < cnt; i++) {
    if (!src[i]) {
      ram[i] = (int)g->nvtx++;
    } else {
      if (src[i]->ram >= 0) {
        ram[i] = src[i]->ram;
      } else if (need_clip) {
        ram[i] = (int)g->nvtx++;
      } else {
        ram[i] = src[i]->ram = (int)g->nvtx++;
      }
    }
    g->vtx[ram[i]] = ov[i];
  }
  VC_Poly *po = &g->poly[g->npoly];
  memset(po, 0, sizeof *po);
  for (int i = 0; i < cnt; i++) po->v[i] = (u16)ram[i];
  po->nv = (u8)cnt;
  po->front = (u8)front;
  po->type = (u8)st->type;
  po->attr = st->attr;
  po->texparam = st->texparam;
  po->pltt = st->pltt;
  po->viewport = (u32)st->vp_x1 | (u32)st->vp_y1 << 8 | (u32)st->vp_x2 << 16 | (u32)st->vp_y2 << 24;
  po->ymin = ymin;
  po->ymax = ymax;
  po->index = (u16)g->npoly;
  g->order[g->npoly] = (u16)g->npoly;
  g->npoly++;
}

static void assemble(VCoreState *s, const VC_P14Pend *in);

/* a vertex arrives: the position is in st->cur */
static void new_vertex(VCoreState *s) {
  VC_P14 *st = &s->p14;
  if (!st->in_list) return; /* GBATEK 44: VTX commands only between BEGIN and END */
  VC_P14Pend nv;
  memset(&nv, 0, sizeof nv);
  nv.ram = -1;
  s32 pos[4] = {st->cur[0], st->cur[1], st->cur[2], 0x1000};
  vc_vertex_attrs(s, pos, &nv.v);
  transform(s, st->cur, nv.v.clip);
  nv.v.clipped = 0;
  if (s->p23.mode) {
    if (s->p23.mode == VC_IP_RECORD) vc_ip_tape_vtx(s, &nv.v);
    vc_ip_vertex(s, &nv.v);
  }
  assemble(s, &nv);
}

/* the vertex into the list's ring, and the polygon it completes */
static void assemble(VCoreState *s, const VC_P14Pend *in) {
  VC_P14 *st = &s->p14;
  VC_P14Pend nv = *in;
  const int ringmax = 4; /* the list's last four vertices, newest at the end */
  if (st->count < ringmax) {
    st->ring[st->count] = nv;
  } else {
    for (int i = 0; i < ringmax - 1; i++) st->ring[i] = st->ring[i + 1];
    st->ring[ringmax - 1] = nv;
  }
  st->count++;
  VC_P14Pend *r = st->ring;
  VC_P14Pend *q[4];
  switch (st->type) {
  case 0: /* separate triangles */
    if (st->count == 3) {
      q[0] = &r[0]; q[1] = &r[1]; q[2] = &r[2];
      emit_polygon(s, q, 3);
      st->count = 0;
    }
    break;
  case 1: /* separate quads: v0 v1 v2 v3 anti-clockwise */
    if (st->count == 4) {
      q[0] = &r[0]; q[1] = &r[1]; q[2] = &r[2]; q[3] = &r[3];
      emit_polygon(s, q, 4);
      st->count = 0;
    }
    break;
  case 2: /* triangle strip: each second triangle has swapped winding, so its first two vertices swap */
    if (st->count >= 3) {
      VC_P14Pend *a = &r[st->count >= 4 ? 1 : 0], *b = a + 1, *c = a + 2;
      if (st->polyn & 1) { q[0] = b; q[1] = a; } else { q[0] = a; q[1] = b; }
      q[2] = c;
      emit_polygon(s, q, 3);
      st->polyn++;
    }
    break;
  case 3: /* quad strip: v0 v1 v2 v3 -> (v0 v1 v3 v2), then every two vertices another quad */
    if (st->count >= 4 && !(st->count & 1)) {
      VC_P14Pend *a = &r[0];
      q[0] = a; q[1] = a + 1; q[2] = a + 3; q[3] = a + 2;
      emit_polygon(s, q, 4);
    }
    break;
  }
}

void vc_cmd_vtx(VCoreState *s, u32 op, const u32 *param, u32 nparam) {
  VC_P14 *st = &s->p14;
  u32 p0 = nparam > 0 ? param[0] : 0, p1 = nparam > 1 ? param[1] : 0;
  switch (op) {
  case VC_OP_VTX_16: /* 16-bit signed, 12-bit fraction */
    st->cur[0] = sext(p0, 16);
    st->cur[1] = sext(p0 >> 16, 16);
    st->cur[2] = sext(p1, 16);
    break;
  case VC_OP_VTX_10: /* 10-bit signed, 6-bit fraction: shifted to 12 */
    st->cur[0] = sext(p0, 10) << 6;
    st->cur[1] = sext(p0 >> 10, 10) << 6;
    st->cur[2] = sext(p0 >> 20, 10) << 6;
    break;
  case VC_OP_VTX_XY:
    st->cur[0] = sext(p0, 16);
    st->cur[1] = sext(p0 >> 16, 16);
    break;
  case VC_OP_VTX_XZ:
    st->cur[0] = sext(p0, 16);
    st->cur[2] = sext(p0 >> 16, 16);
    break;
  case VC_OP_VTX_YZ:
    st->cur[1] = sext(p0, 16);
    st->cur[2] = sext(p0 >> 16, 16);
    break;
  case VC_OP_VTX_DIFF: /* signed 10-bit, 9-bit fraction / 8 = the raw value in 12-bit fraction units; 16-bit sum */
    st->cur[0] = sext((u32)(st->cur[0] + sext(p0, 10)), 16);
    st->cur[1] = sext((u32)(st->cur[1] + sext(p0 >> 10, 10)), 16);
    st->cur[2] = sext((u32)(st->cur[2] + sext(p0 >> 20, 10)), 16);
    break;
  default: return;
  }
  new_vertex(s);
}

void vc_cmd_poly(VCoreState *s, u32 op, const u32 *param, u32 nparam) {
  VC_P14 *st = &s->p14;
  u32 p0 = nparam > 0 ? param[0] : 0;
  switch (op) {
  case VC_OP_POLYGON_ATTR: st->attr_pending = p0; break; /* GBATEK 43: takes effect at the next BEGIN_VTXS */
  case VC_OP_TEXIMAGE_PARAM:
    st->texparam = p0;
    if (s->p23.mode == VC_IP_RECORD) vc_ip_tape_op(s, VC_TAPE_TEX, 0, p0);
    break;
  case VC_OP_PLTT_BASE:
    st->pltt = p0 & 0x1FFF;
    if (s->p23.mode == VC_IP_RECORD) vc_ip_tape_op(s, VC_TAPE_PLTT, 0, st->pltt);
    break;
  case VC_OP_BEGIN_VTXS:
    st->attr = st->attr_pending;
    st->type = (int)(p0 & 3);
    st->in_list = 1;
    st->count = 0;
    st->polyn = 0;
    if (s->p23.mode == VC_IP_RECORD) vc_ip_tape_op(s, VC_TAPE_BEGIN, st->type, st->attr);
    if (s->p23.mode) vc_ip_begin(s);
    break;
  case VC_OP_END_VTXS: /* GBATEK 44: no effect on the hardware beyond the list's end */
    st->in_list = 0;
    if (s->p23.mode == VC_IP_RECORD) vc_ip_tape_op(s, VC_TAPE_END, 0, 0);
    if (s->p23.mode) vc_ip_end(s);
    break;
  case VC_OP_VIEWPORT:
    st->vp_x1 = (int)(p0 & 0xFF);
    st->vp_y1 = (int)((p0 >> 8) & 0xFF);
    st->vp_x2 = (int)((p0 >> 16) & 0xFF);
    st->vp_y2 = (int)((p0 >> 24) & 0xFF);
    if (s->p23.mode == VC_IP_RECORD) vc_ip_tape_op(s, VC_TAPE_VIEWPORT, 0, p0);
    break;
  case VC_OP_POS_TEST: { /* GBATEK 53: (x, y, z, 1) * clip matrix; it overwrites the current vertex */
    st->cur[0] = sext(p0, 16);
    st->cur[1] = sext(p0 >> 16, 16);
    st->cur[2] = sext(nparam > 1 ? param[1] : 0, 16);
    transform(s, st->cur, st->pos_result);
    break;
  }
  case VC_OP_BOX_TEST: break; /* no picture: the result goes to GXSTAT, which the frame does not read */
  default: break;
  }
}

/* p23's replay (vc_geo_replay in p23_interp.h): the polygon assembly of the last recorded stream again, each vertex through the
 * blend in force, as new_vertex and vc_cmd_poly do it in a full run; the commands that touch nothing here are not on the tape.
 * A list whose vertices the blend leaves as they are makes the polygons the recorded run made of it, so they are copied from
 * that run (vertex RAM indices moved by where the list starts now) when they fit and the run did not overflow */
static void replay_op(VCoreState *s, const VC_IpTapeOp *o, const VC_Vertex **v) {
  VC_P14 *st = &s->p14;
  switch (o->op) {
  case VC_TAPE_BEGIN:
    st->attr = st->attr_pending = o->a;
    st->type = o->b;
    st->in_list = 1;
    st->count = 0;
    st->polyn = 0;
    if (s->p23.mode) vc_ip_begin(s);
    break;
  case VC_TAPE_END:
    st->in_list = 0;
    if (s->p23.mode) vc_ip_end(s);
    break;
  case VC_TAPE_TEX: st->texparam = o->a; break;
  case VC_TAPE_PLTT: st->pltt = o->a; break;
  case VC_TAPE_VIEWPORT:
    st->vp_x1 = (int)(o->a & 0xFF);
    st->vp_y1 = (int)((o->a >> 8) & 0xFF);
    st->vp_x2 = (int)((o->a >> 16) & 0xFF);
    st->vp_y2 = (int)((o->a >> 24) & 0xFF);
    break;
  case VC_TAPE_VTX: { /* the vertex already blended (vc_geo_replay) */
    VC_P14Pend nv;
    memset(&nv, 0, sizeof nv);
    nv.ram = -1;
    nv.v = *(*v)++;
    assemble(s, &nv);
    break;
  }
  case VC_TAPE_SWAP: vc_cmd_swap(s, &o->a, 1); break;
  }
}

int vc_geo_replay(VCoreState *s) {
  VC_IpTape *t = &s->p23.tape;
  if (!t->ok) return -1;
  VC_GeomOut *g = &s->geo;
  vc_reset_geo(s);
  vc_poly_reset(s);
  u32 i = 0, li = 0;
  t->copied = 0;
  while (i < t->nop) {
    const VC_IpTapeOp *o = &t->op[i];
    if (o->op != VC_TAPE_BEGIN || li >= t->nl) {
      const VC_Vertex *v = NULL; /* (no VTX outside a list) */
      replay_op(s, o, &v);
      i++;
      continue;
    }
    const VC_IpTapeList *l = &t->l[li++];
    replay_op(s, o, NULL); /* the BEGIN */
    int same = vc_ip_blend_list(s, &t->v[l->v0], &t->bv[l->v0], l->v1 - l->v0);
    u32 np = l->p1 - l->p0, nr = l->r1 - l->r0;
    if (same && t->gok && !l->swap && g->npoly + np <= VC_MAX_POLYS && g->nvtx + nr <= VC_MAX_VERTS) {
      u32 dr = g->nvtx - l->r0;
      memcpy(&g->vtx[g->nvtx], &t->gv[l->r0], nr * sizeof *g->vtx);
      for (u32 k = l->p0; k < l->p1; k++) {
        VC_Poly *po = &g->poly[g->npoly];
        *po = t->gp[k];
        for (int n = 0; n < po->nv; n++) po->v[n] = (u16)(po->v[n] + dr);
        po->index = (u16)g->npoly;
        g->order[g->npoly] = (u16)g->npoly;
        g->npoly++;
      }
      g->nvtx += nr;
      t->copied++;
      for (u32 k = l->op0 + 1; k < l->op1; k++) /* the list's state changes still hold after it */
        if (t->op[k].op != VC_TAPE_VTX) replay_op(s, &t->op[k], NULL);
    } else {
      const VC_Vertex *v = &t->bv[l->v0];
      for (u32 k = l->op0 + 1; k < l->op1; k++) replay_op(s, &t->op[k], &v);
    }
    i = l->op1;
  }
  memcpy(s->p14.cur, t->cur, sizeof s->p14.cur);
  memcpy(s->p14.pos_result, t->pos_result, sizeof s->p14.pos_result);
  return g->swapped;
}
