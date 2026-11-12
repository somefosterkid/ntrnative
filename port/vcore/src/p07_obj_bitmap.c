/* port/vcore piece 7: bitmap OBJs, the OBJ window, OBJ mosaic (docs/vcore.md, piece 7). OWNED BY PIECE 7.
 * Spec: GBATEK 26 (DS Video OBJs: bitmap OBJ mapping), 15 (OAM attributes), 07 (the OBJ window), 08 (MOSAIC). */
#include "vc_shared.h"

/* GBATEK 26 "Bitmap OBJ Mapping (DISPCNT.6,5,22)": bitmap OBJs are 15bit direct colour + alpha flag in bit 15.
 *   2D (bit6 = 0): source bitmap 128 dots wide (bit5 = 0) or 256 (bit5 = 1);
 *       address = (tile & MaskX) * 0x10 + (tile & ~MaskX) * 0x80, MaskX = 0x0F / 0x1F; rows are 256 / 512 bytes apart.
 *   1D (bit6 = 1, bit5 = 0): address = tile * 128 (bit22 = 0) or * 256 (bit22 = 1, engine A only); the source width
 *       is the OBJ's width. Attr 2 bits 12-15 are the OBJ's alpha. */
u32 vc_obj_bitmap_texel(const VCoreState *s, int e, const VC_ObjAttr *o, int u, int v) {
  if (u < 0 || v < 0 || u >= o->w || v >= o->h) return 0;
  u32 dc = vc_eng32(s, e, VC_REG_DISPCNT);
  u32 addr;
  /* oracle (GBATEK: "must be 0"): 1D mapping with bit 5 set draws no bitmap OBJ at all */
  if ((dc & 0x60) == 0x60) return 0;
  if (dc & (1u << 6)) {
    u32 bound = (e == 0 && (dc & (1u << 22))) ? 256 : 128;
    addr = (u32)o->tile * bound + ((u32)v * (u32)o->w + (u32)u) * 2;
  } else {
    u32 maskx = (dc & (1u << 5)) ? 0x1F : 0x0F;
    u32 stride = (dc & (1u << 5)) ? 512 : 256;
    u32 t = (u32)o->tile;
    addr = (t & maskx) * 0x10 + (t & ~maskx) * 0x80 + (u32)v * stride + (u32)u * 2;
  }
  u16 c = vc_rd16(vc_rgn_obj(s, e), addr);
  /* oracle (GBATEK silent): an OBJ alpha of 0 makes the whole bitmap OBJ invisible */
  if (!(c & 0x8000) || !(o->palette & 15)) return 0;
  return VC_PX_OPAQUE | VC_PX_BMPOBJ | ((u32)(o->palette & 15) << VC_PX_ALPHA_SHIFT) | (c & VC_PX_COLOR);
}

/* GBATEK 07 "The OBJ Window": non-transparent dots of OBJ-window OBJs mark the OBJ window; nothing is drawn. */
void vc_obj_window_plot(VCoreState *s, int e, const VC_ObjAttr *o, int x, VC_Line *ln) {
  (void)s; (void)e; (void)o;
  if (x >= 0 && x < VC_W) ln->objwin[x] = 1;
}

/* GBATEK 08: MOSAIC bits 12-15 = OBJ V size - 1; a mosaic row repeats the first row of its block (rows 0..n-1 show
 * row 0, n..2n-1 row n, ...). */
int vc_mosaic_obj_y(const VCoreState *s, int e, int y) {
  int n = ((vc_eng16(s, e, VC_REG_MOSAIC) >> 12) & 15) + 1;
  return y - y % n;
}

/* GBATEK 08: bits 8-11 = OBJ H size - 1. Oracle (probe t2): the mosaic is per OBJ, not on the line buffer: the OBJ's
 * pixel at screen x shows the texel column of the block's first screen pixel (x - x%n), clamped to the OBJ's own
 * left edge; that texel transparent makes the whole block transparent (even where the OBJ's own texel is opaque).
 * Returns the OBJ-relative column (before H flip) to fetch for screen pixel x of an OBJ whose left edge is ox. */
int vc_mosaic_obj_hsize(const VCoreState *s, int e) { return ((vc_eng16(s, e, VC_REG_MOSAIC) >> 8) & 15) + 1; }

int vc_mosaic_obj_x(const VCoreState *s, int e, int x, int ox) {
  int n = ((vc_eng16(s, e, VC_REG_MOSAIC) >> 8) & 15) + 1;
  int c = x - x % n - ox;
  return c < 0 ? 0 : c;
}

/* no line-buffer pass any more (kept for the interface) */
void vc_mosaic_obj_line(const VCoreState *s, int e, VC_Line *ln) { (void)s; (void)e; (void)ln; }
