/* port/vcore piece 17: textures (docs/vcore.md, piece 17). OWNED BY PIECE 17.
 * Follows GBATEK 47 (TEXIMAGE_PARAM, PLTT_BASE), 48 (the 7 formats), 50 (blending). */
#include "vc_shared.h"

/* 5-bit colour channel -> 6 bits: "IF X>0 THEN X=X*2+1" (GBATEK 50) */
static inline u32 w6(u32 c) { return c ? c * 2 + 1 : 0; }

static inline u32 rgb555_to_px(u32 c, u32 alpha) {
  return VC_RGB6(w6(c & 31), w6((c >> 5) & 31), w6((c >> 10) & 31)) | (alpha << 24);
}

/* the texel coordinate on one axis after clamp / repeat / flip (GBATEK 47 bits 16-19) */
static inline int tex_axis(int v, int size, int repeat, int flip) {
  if (!repeat) return v < 0 ? 0 : (v >= size ? size - 1 : v);
  int m = v & (size - 1);
  if (flip && (v & size)) m = size - 1 - m;
  return m;
}

/* the texture palette address space is 128 KB (slots 0-7) and the address wraps there (oracle: a 4x4 block's palette
 * offset can run past it); slots 6 and 7 hold no VRAM, a read there gives colour 0, black (the oracle) */
static inline u16 pal_rd16(const VC_Region *pal, u32 a) {
  a &= 0x1FFFF;
  if (a >= ((u32)pal->npages << VC_PAGE_SHIFT)) return 0;
  return vc_rd16(pal, a);
}

static u32 tex_sample_slow(const VCoreState *s, const VC_Poly *p, s32 ts, s32 tt) {
  u32 tp = p->texparam;
  int fmt = (tp >> 26) & 7;
  if (fmt == 0) return 0;
  int sw = 8 << ((tp >> 20) & 7), sh = 8 << ((tp >> 23) & 7);
  int x = tex_axis(ts >> 4, sw, (tp >> 16) & 1, (tp >> 18) & 1);
  int y = tex_axis(tt >> 4, sh, (tp >> 17) & 1, (tp >> 19) & 1);
  u32 base = (tp & 0xFFFF) * 8;
  u32 i = (u32)y * (u32)sw + (u32)x;
  u32 pb = p->pltt & 0x1FFF;
  const VC_Region *tex = &s->rgn[VC_RGN_TEX], *pal = &s->rgn[VC_RGN_TEXPAL];
  int t0 = (tp >> 29) & 1;
  u32 idx, a;
  switch (fmt) {
  case 1: /* A3I5 */
    idx = vc_rd8(tex, base + i);
    a = idx >> 5;
    a = a * 4 + a / 2;
    return rgb555_to_px(pal_rd16(pal, pb * 16 + (idx & 31) * 2), a);
  case 2: /* 4-colour, palette base in 8-byte steps */
    idx = (vc_rd8(tex, base + (i >> 2)) >> ((i & 3) * 2)) & 3;
    return rgb555_to_px(pal_rd16(pal, pb * 8 + idx * 2), (t0 && idx == 0) ? 0 : 31);
  case 3: /* 16-colour */
    idx = (vc_rd8(tex, base + (i >> 1)) >> ((i & 1) * 4)) & 15;
    return rgb555_to_px(pal_rd16(pal, pb * 16 + idx * 2), (t0 && idx == 0) ? 0 : 31);
  case 4: /* 256-colour */
    idx = vc_rd8(tex, base + i);
    return rgb555_to_px(pal_rd16(pal, pb * 16 + idx * 2), (t0 && idx == 0) ? 0 : 31);
  case 5: { /* 4x4 compressed (GBATEK 48) */
    u32 blk = (u32)(y >> 2) * (u32)(sw >> 2) + (u32)(x >> 2);
    u32 row = vc_rd8(tex, base + blk * 4 + (u32)(y & 3));
    u32 tx = (row >> ((x & 3) * 2)) & 3;
    /* index data in slot 1: slot0 offset / 2, slot 2 offset / 2 + 0x10000 */
    /* oracle: slots 0 and 1 (and 2 and 3) map alike: the index data is at (base within its 128 KB slot) / 2, plus
     * 64 KB for slots 2 and 3 (GBATEK only defines slots 0 and 2) */
    u32 ia = ((base & 0x1FFFF) >> 1) + ((base & 0x40000) ? 0x10000 : 0);
    u32 pi = vc_rd16(tex, 0x20000 + ia + blk * 2);
    u32 pofs = pb * 16 + (pi & 0x3FFF) * 4;
    u32 mode = pi >> 14;
    u32 c0 = pal_rd16(pal, pofs), c1 = pal_rd16(pal, pofs + 2);
    switch (mode) {
    case 0:
      if (tx == 3) return 0;
      return rgb555_to_px(pal_rd16(pal, pofs + tx * 2), 31);
    case 2:
      return rgb555_to_px(pal_rd16(pal, pofs + tx * 2), 31);
    default: { /* modes 1 and 3: two colours and their blends, per 5-bit channel */
      u32 w0, w1, sft;
      if (tx < 2) return rgb555_to_px(tx ? c1 : c0, 31);
      if (mode == 1) {
        if (tx == 3) return 0;
        w0 = 1; w1 = 1; sft = 1;
      } else if (tx == 2) { w0 = 5; w1 = 3; sft = 3; }
      else { w0 = 3; w1 = 5; sft = 3; }
      u32 c = 0;
      for (int k = 0; k < 3; k++) {
        u32 a0 = (c0 >> (5 * k)) & 31, a1 = (c1 >> (5 * k)) & 31;
        c |= (((a0 * w0 + a1 * w1) >> sft) & 31) << (5 * k);
      }
      return rgb555_to_px(c, 31);
    }
    }
  }
  case 6: /* A5I3 */
    idx = vc_rd8(tex, base + i);
    return rgb555_to_px(pal_rd16(pal, pb * 16 + (idx & 7) * 2), idx >> 3);
  default: { /* 7 direct */
    u32 c = vc_rd16(tex, base + i * 2);
    return rgb555_to_px(c, (c & 0x8000) ? 31 : 0);
  }
  }
}


/* ---- the per-polygon fast path: the same texel as tex_sample_slow, bytes read without the region's
 * modulo and loops, palette entries converted once per index ---- */
static inline u32 trd8(const VC_TexCtx *c, u32 a) {
  const VC_Page *pg = &c->tex->page[(a >> VC_PAGE_SHIFT) & c->tmask];
  u32 o = a & (VC_PAGE_SIZE - 1), v = 0;
  for (int i = 0; i < pg->n; i++) v |= pg->src[i][o];
  return v;
}
static inline u32 trd16(const VC_TexCtx *c, u32 a) {
  a &= ~1u;
  const VC_Page *pg = &c->tex->page[(a >> VC_PAGE_SHIFT) & c->tmask];
  u32 o = a & (VC_PAGE_SIZE - 1), v = 0;
  for (int i = 0; i < pg->n; i++) v |= pg->src[i][o] | (u32)pg->src[i][o + 1] << 8;
  return v;
}
static inline u32 prd16(const VC_TexCtx *c, u32 a) {
  a &= 0x1FFFE;
  if (a >= c->palsz) return 0;
  const VC_Page *pg = &c->pal->page[a >> VC_PAGE_SHIFT];
  u32 o = a & (VC_PAGE_SIZE - 1), v = 0;
  for (int i = 0; i < pg->n; i++) v |= pg->src[i][o] | (u32)pg->src[i][o + 1] << 8;
  return v;
}

void vc_tex_ctx_init(VC_TexCtx *c, const VCoreState *s, const VC_Poly *p) {
  u32 tp = p->texparam;
  c->s = s;
  c->p = p;
  c->fmt = (tp >> 26) & 7;
  c->sw = 8u << ((tp >> 20) & 7);
  c->sh = 8u << ((tp >> 23) & 7);
  c->rx = (tp >> 16) & 1;
  c->ry = (tp >> 17) & 1;
  c->fx = (tp >> 18) & 1;
  c->fy = (tp >> 19) & 1;
  c->t0 = (tp >> 29) & 1;
  c->base = (tp & 0xFFFF) * 8;
  c->pb = p->pltt & 0x1FFF;
  c->tex = &s->rgn[VC_RGN_TEX];
  c->pal = &s->rgn[VC_RGN_TEXPAL];
  c->palsz = c->pal->npages << VC_PAGE_SHIFT;
  c->tmask = c->tex->npages - 1;
  c->fast = c->tex->npages != 0 && (c->tex->npages & (c->tex->npages - 1)) == 0 && c->pal->npages != 0 &&
            c->palsz <= 0x20000;
  c->lx = -1;
  c->lblk = 0xFFFFFFFFu;
  c->last = 0;
  size_t n = c->fmt == 2 ? 4 : c->fmt == 3 ? 16 : 256;
  if (c->fmt == 2 || c->fmt == 3 || c->fmt == 1 || c->fmt == 4 || c->fmt == 6) memset(c->pc, 0xFF, n * sizeof c->pc[0]);
}

u32 vc_tex_sample_c(VC_TexCtx *c, s32 ts, s32 tt) {
  if (c->fmt == 0) return 0;
  if (!c->fast) return tex_sample_slow(c->s, c->p, ts, tt);
  u32 sw = c->sw;
  int x = tex_axis(ts >> 4, (int)sw, c->rx, c->fx);
  int y = tex_axis(tt >> 4, (int)c->sh, c->ry, c->fy);
  if (x == c->lx && y == c->ly) return c->last;
  c->lx = x;
  c->ly = y;
  u32 base = c->base, pb = c->pb, i = (u32)y * sw + (u32)x, idx, a, r;
  switch (c->fmt) {
  case 1: /* A3I5 */
    idx = trd8(c, base + i);
    if (c->pc[idx] == 0xFFFFFFFFu) {
      a = idx >> 5;
      a = a * 4 + a / 2;
      c->pc[idx] = rgb555_to_px(prd16(c, pb * 16 + (idx & 31) * 2), a);
    }
    r = c->pc[idx];
    break;
  case 2:
    idx = (trd8(c, base + (i >> 2)) >> ((i & 3) * 2)) & 3;
    if (c->pc[idx] == 0xFFFFFFFFu)
      c->pc[idx] = rgb555_to_px(prd16(c, pb * 8 + idx * 2), (c->t0 && idx == 0) ? 0 : 31);
    r = c->pc[idx];
    break;
  case 3:
    idx = (trd8(c, base + (i >> 1)) >> ((i & 1) * 4)) & 15;
    if (c->pc[idx] == 0xFFFFFFFFu)
      c->pc[idx] = rgb555_to_px(prd16(c, pb * 16 + idx * 2), (c->t0 && idx == 0) ? 0 : 31);
    r = c->pc[idx];
    break;
  case 4:
    idx = trd8(c, base + i);
    if (c->pc[idx] == 0xFFFFFFFFu)
      c->pc[idx] = rgb555_to_px(prd16(c, pb * 16 + idx * 2), (c->t0 && idx == 0) ? 0 : 31);
    r = c->pc[idx];
    break;
  case 6: /* A5I3 */
    idx = trd8(c, base + i);
    if (c->pc[idx] == 0xFFFFFFFFu) c->pc[idx] = rgb555_to_px(prd16(c, pb * 16 + (idx & 7) * 2), idx >> 3);
    r = c->pc[idx];
    break;
  case 7: {
    u32 v = trd16(c, base + i * 2);
    r = rgb555_to_px(v, (v & 0x8000) ? 31 : 0);
    break;
  }
  default: { /* 5: 4x4 compressed: a block's four colours are built once (the slow path's rules) */
    u32 blk = (u32)(y >> 2) * (sw >> 2) + (u32)(x >> 2);
    if (blk != c->lblk) {
      c->lblk = blk;
      u32 ia = ((base & 0x1FFFF) >> 1) + ((base & 0x40000) ? 0x10000 : 0);
      u32 pi = trd16(c, 0x20000 + ia + blk * 2);
      u32 pofs = pb * 16 + (pi & 0x3FFF) * 4;
      u32 mode = pi >> 14;
      u32 c0 = prd16(c, pofs), c1 = prd16(c, pofs + 2);
      if (mode == 0 || mode == 2) {
        c->bc[0] = rgb555_to_px(c0, 31);
        c->bc[1] = rgb555_to_px(c1, 31);
        c->bc[2] = rgb555_to_px(prd16(c, pofs + 4), 31);
        c->bc[3] = mode == 0 ? 0 : rgb555_to_px(prd16(c, pofs + 6), 31);
      } else {
        u32 c2 = 0, c3 = 0;
        u32 q0 = 5, q1 = 3, r0 = 3, r1 = 5;
        if (mode == 1) {
          for (int k = 0; k < 3; k++) {
            u32 a0 = (c0 >> (5 * k)) & 31, a1 = (c1 >> (5 * k)) & 31;
            c2 |= (((a0 + a1) >> 1) & 31) << (5 * k);
          }
        } else {
          for (int k = 0; k < 3; k++) {
            u32 a0 = (c0 >> (5 * k)) & 31, a1 = (c1 >> (5 * k)) & 31;
            c2 |= (((a0 * q0 + a1 * q1) >> 3) & 31) << (5 * k);
            c3 |= (((a0 * r0 + a1 * r1) >> 3) & 31) << (5 * k);
          }
        }
        c->bc[0] = rgb555_to_px(c0, 31);
        c->bc[1] = rgb555_to_px(c1, 31);
        c->bc[2] = rgb555_to_px(c2, 31);
        c->bc[3] = mode == 1 ? 0 : rgb555_to_px(c3, 31);
      }
    }
    r = c->bc[(trd8(c, base + blk * 4 + (u32)(y & 3)) >> ((x & 3) * 2)) & 3];
    break;
  }
  }
  c->last = r;
  return r;
}

u32 vc_tex_sample(const VCoreState *s, const VC_Poly *p, s32 ts, s32 tt) {
  return tex_sample_slow(s, p, ts, tt);
}

/* GBATEK 50: modulation, decal, (toon / highlight: piece 19); shadow polygons modulate like mode 0 */
u32 vc_tex_blend(const VCoreState *s, const VC_Poly *p, u32 vcolor, u32 texel) {
  int fmt = (p->texparam >> 26) & 7;
  if (fmt == 0 || !(vc_io16(s, VC_REG_DISP3DCNT) & 1)) return vcolor;
  int mode = (p->attr >> 4) & 3;
  if (mode == 2) return vc_toon_blend(s, p, vcolor, texel);
  u32 av = VC_A5(vcolor), at = VC_A5(texel), o = 0;
  if (mode == 1 || mode == 3) { /* decal; shadow polygons texture like decal (oracle) */
    for (int k = 0; k < 3; k++) {
      u32 v = (vcolor >> (8 * k)) & 0x3F, t = (texel >> (8 * k)) & 0x3F, r;
      if (at == 0) r = v;
      else if (at == 31) r = t;
      else r = (t * at + v * (31 - at)) / 32; /* oracle: /32 with 31-At, GBATEK's /64 is wrong */
      o |= r << (8 * k);
    }
    return o | (av << 24);
  }
  for (int k = 0; k < 3; k++) {
    u32 v = (vcolor >> (8 * k)) & 0x3F, t = (texel >> (8 * k)) & 0x3F;
    o |= (((t + 1) * (v + 1) - 1) / 64) << (8 * k);
  }
  /* alpha is 5 bits: the same formula at that width (/32) */
  return o | ((((at + 1) * (av + 1) - 1) / 32) << 24);
}
