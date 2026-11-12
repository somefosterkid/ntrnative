/* port/vcore piece 5: normal OBJs and the OAM loop (docs/vcore.md, piece 5). OWNED BY PIECE 5.
 * Follows GBATEK 14/15 (OBJ overview, OAM attributes), 17/26 (tile mapping, DS OBJ tile mapping, priority, vertical
 * wrap), 27 (OBJ extended palettes), 18 (palettes).
 *
 * Conventions: VC_ObjAttr.x is the 9-bit X sign-extended (-256..255); VC_ObjAttr.y is the raw 8-bit Y (0..255): an OBJ
 * covers screen line ly when ((ly - y) & 255) < h, so it wraps from the bottom to the top of the screen (GBATEK 26,
 * "OBJ Vertical Wrap": on the DS it shows at both ends).
 */
#include "vc_shared.h"

/* GBATEK 15: shape (attr 0 bits 14-15) x size (attr 1 bits 14-15) -> width, height. Shape 3 is prohibited. */
static const u8 obj_dim[3][4][2] = {
  {{8, 8}, {16, 16}, {32, 32}, {64, 64}},
  {{16, 8}, {32, 8}, {32, 16}, {64, 32}},
  {{8, 16}, {8, 32}, {16, 32}, {32, 64}},
};

void vc_obj_decode(const VCoreState *s, int e, int i, VC_ObjAttr *o) {
  const u8 *oam = vc_oam(s, e) + (i & 127) * 8;
  memset(o, 0, sizeof *o);
  o->index = i;
  for (int k = 0; k < 3; k++) o->attr[k] = (u16)(oam[k * 2] | oam[k * 2 + 1] << 8);
  u16 a0 = o->attr[0], a1 = o->attr[1], a2 = o->attr[2];
  int shape = a0 >> 14, size = a1 >> 14;
  if (shape < 3) {
    o->w = obj_dim[shape][size][0];
    o->h = obj_dim[shape][size][1];
  }
  o->y = a0 & 0xFF;
  o->x = a1 & 0x1FF;
  if (o->x >= 256) o->x -= 512;
  o->affine = (a0 >> 8) & 1;
  if (o->affine) {
    o->dbl = (a0 >> 9) & 1;
    o->affine_param = (a1 >> 9) & 31;
  } else {
    o->hidden = (a0 >> 9) & 1;
    o->hflip = (a1 >> 12) & 1;
    o->vflip = (a1 >> 13) & 1;
  }
  o->mode = (a0 >> 10) & 3;
  o->mosaic = (a0 >> 12) & 1;
  o->bpp8 = (a0 >> 13) & 1;
  o->tile = a2 & 0x3FF;
  o->prio = (a2 >> 10) & 3;
  o->palette = a2 >> 12;
}

/* GBATEK 26 "OBJ Tile Mapping": DISPCNT bit 4 = 1D, bits 20-21 the 1D boundary 32/64/128/256 bytes; 2D: 32 tiles a
 * row (GBATEK 17), tile numbers in 32-byte units; an 8bpp tile takes two units. */
u32 vc_obj_tile_texel(const VCoreState *s, int e, const VC_ObjAttr *o, int u, int v) {
  if (u < 0 || v < 0 || u >= o->w || v >= o->h) return 0;
  u32 dispcnt = vc_eng32(s, e, VC_REG_DISPCNT);
  int tx = u >> 3, ty = v >> 3;
  u32 tsize = o->bpp8 ? 64 : 32;
  u32 addr;
  if (dispcnt & (1u << 4)) {
    u32 bound = 32u << ((dispcnt >> 20) & 3);
    addr = (u32)o->tile * bound + (u32)(ty * (o->w >> 3) + tx) * tsize;
  } else {
    addr = ((u32)o->tile + (u32)ty * 32) * 32 + (u32)tx * tsize;
  }
  const VC_Region *r = vc_rgn_obj(s, e);
  int px = u & 7, py = v & 7;
  u32 idx;
  if (o->bpp8) {
    idx = vc_rd8(r, addr + (u32)py * 8 + (u32)px);
  } else {
    idx = vc_rd8(r, addr + (u32)py * 4 + (u32)(px >> 1));
    idx = (px & 1) ? idx >> 4 : idx & 15;
  }
  if (idx == 0) return 0;
  u16 col;
  if (o->bpp8) {
    if (dispcnt & (1u << 31)) /* GBATEK 27: 16 palettes x 256 colours */
      col = vc_rd16(vc_rgn_objext(s, e), ((u32)o->palette * 256 + idx) * 2);
    else
      col = vc_pal(s, e, 1, (int)idx);
  } else {
    col = vc_pal(s, e, 1, o->palette * 16 + (int)idx);
  }
  return VC_PX_OPAQUE | (col & VC_PX_COLOR);
}

void vc_obj_plot(VCoreState *s, int e, const VC_ObjAttr *o, int x, u32 px, VC_Line *ln) {
  if (x < 0 || x >= VC_W) return;
  if (o->mode == 2) { /* OBJ window: no colour */
    vc_obj_window_plot(s, e, o, x, ln);
    return;
  }
  /* GBATEK 26 "DS OBJ Priority": priority first, then the OAM number (the lower, the earlier drawn, wins) */
  u32 cur = ln->obj[x];
  if (cur && VC_PX_OBJPRIO(cur) <= (u32)o->prio) return;
  px |= (u32)o->prio << VC_PX_OBJPRIO_SHIFT;
  if (o->mode == 1) px |= VC_PX_SEMI;
  if (o->mosaic) px |= VC_PX_MOSAIC;
  ln->obj[x] = px;
}

void vc_obj_line(VCoreState *s, int e, int y, VC_Line *ln) {
  memset(ln->obj, 0, sizeof ln->obj);
  memset(ln->objwin, 0, sizeof ln->objwin);
  if (!(vc_eng32(s, e, VC_REG_DISPCNT) & (1u << 12))) return; /* bit 23 (H-blank free) has no effect here */
  const u8 *oam = vc_oam(s, e);
  for (int i = 0; i < 128; i++) {
    /* quick reject (same as the tests below, on the raw words): shape 3 has no size; a hidden OBJ; a normal OBJ
     * whose rows do not include this line */
    u32 a0 = (u32)(oam[i * 8] | oam[i * 8 + 1] << 8), a1 = (u32)(oam[i * 8 + 2] | oam[i * 8 + 3] << 8);
    u32 shape = a0 >> 14;
    if (shape == 3) continue;
    if (!((a0 >> 8) & 1)) {
      if ((a0 >> 9) & 1) continue;
      if ((u32)((y - (int)(a0 & 0xFF)) & 255) >= obj_dim[shape][a1 >> 14][1]) continue;
    }
    VC_ObjAttr o;
    vc_obj_decode(s, e, i, &o);
    if (o.hidden || o.w == 0) continue;
    int ly = y;
    if (o.affine) {
      vc_obj_affine_line(s, e, &o, ly, ln);
      continue;
    }
    int v = (ly - o.y) & 255;
    if (v >= o.h) continue;
    if (o.mosaic) { /* oracle: row = (y - y%size) - top, clamped to 0; V flip after (p07) */
      v = (vc_mosaic_obj_y(s, e, y) - o.y) & 255;
      if (v >= o.h) v = 0;
    }
    if (o.vflip) v = o.h - 1 - v;
    for (int i2 = 0; i2 < o.w; i2++) {
      int x = o.x + i2;
      if (x < 0 || x >= VC_W) continue;
      int i3 = o.mosaic ? vc_mosaic_obj_x(s, e, x, o.x) : i2; /* oracle: per-OBJ H mosaic (p07) */
      int u = o.hflip ? o.w - 1 - i3 : i3;
      u32 px = (o.mode == 3) ? vc_obj_bitmap_texel(s, e, &o, u, v) : vc_obj_tile_texel(s, e, &o, u, v);
      if (px) vc_obj_plot(s, e, &o, x, px, ln);
    }
  }
}
