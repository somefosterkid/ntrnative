/* port/vcore piece 18: translucency, sorting, polygon IDs, shadows (docs/vcore.md, piece 18). OWNED BY PIECE 18.
 * Spec: GBATEK 43 (POLYGON_ATTR), 46 (shadow polygons), 51 (alpha blending, fog flag), 35 (DISP3DCNT, ALPHA_TEST_REF,
 * SWAP_BUFFERS), 47 (texture formats), 33 (overview). */
#include "vc_shared.h"

#include <string.h>

#define P18_SORT_OPAQUE_BY_Y 1 /* docs/vcore.md: opaque first sorted by Y (stable); 0 = as sent */

/* sort key: MEASURED against the oracle (GBATEK says only "sorted by Y"): ascending lowest-vertex Y (ymax) first, ties by
 * the highest vertex (ymin), then the order sent */
static u64 sort_key(const VC_GeomOut *g, u32 i) {
  return ((u64)(u32)g->poly[i].ymax << 32) | (u32)g->poly[i].ymin;
}

/* a stable sort of ord[0..n) by sort_key: insertion sorts of runs of 16, then merges (the left one first on a tie), so the
 * order is the one stable order, as the insertion sort over the whole range gave it, without its n * n moves */
static void sort_by_y(const VC_GeomOut *g, u16 *ord, u32 n) {
  enum { RUN = 16 };
  u64 key[VC_MAX_POLYS];
  u16 tmp[VC_MAX_POLYS];
  for (u32 i = 0; i < n; i++) key[ord[i]] = sort_key(g, ord[i]);
  for (u32 r = 0; r < n; r += RUN) {
    u32 e = r + RUN < n ? r + RUN : n;
    for (u32 i = r + 1; i < e; i++) {
      u16 v = ord[i];
      u32 j;
      for (j = i; j > r && key[ord[j - 1]] > key[v]; j--) ord[j] = ord[j - 1];
      ord[j] = v;
    }
  }
  u16 *src = ord, *dst = tmp;
  for (u32 w = RUN; w < n; w *= 2) {
    for (u32 lo = 0; lo < n; lo += 2 * w) {
      u32 mid = lo + w < n ? lo + w : n, hi = lo + 2 * w < n ? lo + 2 * w : n, a = lo, b = mid, o = lo;
      while (a < mid && b < hi) dst[o++] = key[src[b]] < key[src[a]] ? src[b++] : src[a++];
      while (a < mid) dst[o++] = src[a++];
      while (b < hi) dst[o++] = src[b++];
    }
    u16 *t = src;
    src = dst;
    dst = t;
  }
  if (src != ord) memcpy(ord, src, n * sizeof *ord);
}

static u32 attr_alpha(const VC_Poly *p) { return (p->attr >> 16) & 0x1F; }
static u32 attr_id(const VC_Poly *p) { return (p->attr >> 24) & 0x3F; }
static u32 attr_mode(const VC_Poly *p) { return (p->attr >> 4) & 3; }

/* GBATEK 43: alpha 1..30 is translucent (0 = wire-frame, 31 = solid); GBATEK 47: texture formats 1 (A3I5) and 6
 * (A5I3) are the translucent ones (bits 26-28 of TEXIMAGE_PARAM) while textures are enabled (DISP3DCNT bit 0). */
int vc_p18_is_translucent(const VCoreState *s, const VC_Poly *p) {
  u32 a = attr_alpha(p);
  if (a >= 1 && a <= 30) return 1;
  if (vc_io16(s, VC_REG_DISP3DCNT) & 1) {
    u32 fmt = (p->texparam >> 26) & 7;
    if (fmt == 1 || fmt == 6) return 1;
  }
  return 0;
}

/* GBATEK 35 SWAP_BUFFERS: bit 0 manual Y-sorting of translucent polygons, bit 1 w-buffering. GBATEK 46: translucent
 * polygons are always rendered last, even with auto-sort disabled. */
void vc_cmd_swap(VCoreState *s, const u32 *param, u32 nparam) {
  VC_GeomOut *g = &s->geo;
  u32 n = g->npoly, k = 0, i;
  g->swap_param = nparam ? (param[0] & 3) : 0;
  g->swapped = 1;
  if (s->p23.mode == VC_IP_RECORD) vc_ip_tape_op(s, VC_TAPE_SWAP, 0, g->swap_param);
  if (n > VC_MAX_POLYS) n = VC_MAX_POLYS;
  for (i = 0; i < n; i++) g->poly[i].translucent = (u8)vc_p18_is_translucent(s, &g->poly[i]);
  /* opaque pass */
  for (i = 0; i < n; i++)
    if (!g->poly[i].translucent) g->order[k++] = (u16)i;
  if (P18_SORT_OPAQUE_BY_Y) sort_by_y(g, g->order, k);
  /* translucent pass */
  u32 t0 = k;
  for (i = 0; i < n; i++)
    if (g->poly[i].translucent) g->order[k++] = (u16)i;
  if (!(g->swap_param & 1)) sort_by_y(g, g->order + t0, k - t0);
}

/* GBATEK 51: FrameBuf = (Poly*(A+1) + FrameBuf*(31-A)) / 32 per channel, alpha = max. */
static u32 blend(u32 fb, u32 px, u32 a) {
  u32 out = 0;
  for (int c = 0; c < 3; c++) {
    u32 sh = c * 8;
    u32 pv = (px >> sh) & 0xFF, fv = (fb >> sh) & 0xFF;
    out |= ((pv * (a + 1) + fv * (31 - a)) >> 5) << sh;
  }
  u32 fa = (fb >> 24) & 0x1F;
  return out | ((a > fa ? a : fa) << 24);
}

void vc_p18_plot(VCoreState *s, const VC_Poly *p, int x, int y, u32 depth, u32 rgb6a5, int edge, int depth_pass) {
  VC_Frame3D *f = &s->f3d;
  if ((unsigned)x >= VC_W || (unsigned)y >= VC_H) return;
  u32 a = (rgb6a5 >> 24) & 0x1F;
  u32 aattr = attr_alpha(p);
  if (aattr == 0) a = 31; /* GBATEK 43: wire-frames are rendered with a fixed alpha of 31 */
  /* GBATEK 51 / 35: alpha 0 pixels are never rendered; with the alpha test, only alpha > ALPHA_TEST_REF */
  if (a == 0) return;
  if (vc_io16(s, VC_REG_DISP3DCNT) & 4) {
    if (a <= (vc_io32(s, VC_REG_ALPHA_TEST_REF) & 0x1F)) return;
  }
  u32 id = attr_id(p);
  u32 fogbit = (p->attr >> 15) & 1;
  u32 rgb = rgb6a5 & 0xFFFFFF;
  u8 *fl = &f->flags[y][x];
  depth &= 0xFFFFFF;

  if (attr_mode(p) == 3) { /* shadow polygons, GBATEK 46 */
    if (id == 0) {         /* step 1, the mask: stencil is set where the depth test fails; nothing is drawn */
      if (!depth_pass) *fl |= VC_F3_STENCIL;
      return;
    }
    /* step 2: drawn only where the stencil is clear (it is reset after being checked), where the polygon ID differs
     * from the attribute buffer's, and where the depth test passes */
    /* MEASURED against the oracle (GBATEK 46 says "only if the stencil bits are zero", but the oracle draws a step-2
     * shadow only where step 1 set the stencil): the oracle wins */
    u32 had = *fl & VC_F3_STENCIL;
    *fl &= (u8)~VC_F3_STENCIL;
    if (!had) return;
    u32 cur = (*fl & VC_F3_TRANS) ? f->trans_id[y][x] : f->opaque_id[y][x];
    if (cur == id) return;
    if (!depth_pass) return;
    /* drawn as a translucent pixel; the old fog flag is ANDed with the polygon's */
    {
      u32 old = f->color[y][x];
      if ((*fl & VC_F3_TRANS) && f->trans_id[y][x] == id) return;
      if ((vc_io16(s, VC_REG_DISP3DCNT) & 8) && ((old >> 24) & 0x1F) != 0 && a < 31)
        f->color[y][x] = blend(old, rgb, a);
      else
        f->color[y][x] = rgb | (a << 24);
      if (!(p->attr & 0x8000)) *fl &= (u8)~VC_F3_FOG;
      *fl |= VC_F3_TRANS | VC_F3_WRITTEN;
      f->trans_id[y][x] = (u8)id;
      if (p->attr & 0x800) f->depth[y][x] = depth;
    }
    return;
  }

  if (!depth_pass) return;

  if (a >= 31) { /* opaque: colour, depth, ID, fog flag replaced */
    f->color[y][x] = rgb | (31u << 24);
    f->depth[y][x] = depth;
    f->opaque_id[y][x] = (u8)id;
    f->trans_id[y][x] = 0xFF;
    *fl = (u8)((*fl & VC_F3_STENCIL) | VC_F3_WRITTEN | (fogbit ? VC_F3_FOG : 0) | (edge ? VC_F3_EDGE : 0));
    return;
  }

  /* translucent: a translucent polygon does not draw over a pixel the same polygon ID wrote translucently */
  if ((*fl & VC_F3_TRANS) && f->trans_id[y][x] == id) return;
  u32 old = f->color[y][x];
  if ((vc_io16(s, VC_REG_DISP3DCNT) & 8) && ((old >> 24) & 0x1F) != 0)
    f->color[y][x] = blend(old, rgb, a);
  else
    f->color[y][x] = rgb | (a << 24);
  if (!fogbit) *fl &= (u8)~VC_F3_FOG; /* GBATEK 51: fog flag ANDed for translucent pixels */
  *fl |= VC_F3_TRANS | VC_F3_WRITTEN;
  f->trans_id[y][x] = (u8)id;
  if (p->attr & 0x800) f->depth[y][x] = depth;
}

void vc_3d_plot(VCoreState *s, const VC_Poly *p, int x, int y, u32 depth, u32 rgb6a5, int edge) {
  int pass = ((unsigned)x < VC_W && (unsigned)y < VC_H) ? vc_3d_depth_test(s, p, x, y, depth) : 0;
  vc_p18_plot(s, p, x, y, depth, rgb6a5, edge, pass);
}
