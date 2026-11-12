/* port/vcore piece 4: the large bitmap BG and BG mosaic (docs/vcore.md, piece 4). OWNED BY PIECE 4.
 * Spec: GBATEK 25 (DS BG modes: mode 6 "Large Screen Bitmap", BGxCNT size table), 13 (bitmap BGs), 08 (MOSAIC). */
#include "vc_shared.h"

/* Sign-extend the 28-bit reference point BG2X / BG2Y (GBATEK 06/25: 8.8 fixed, bits 0-27, bit 27 the sign). */
static s32 sx28(u32 v) { return (s32)(v << 4) >> 4; }

/* The affine position (8.8) of screen pixel (x, y): reference + y * (PB, PD) + x * (PA, PC). Kept local so this
 * piece's tests do not depend on piece 2's vc_bg_affine_pos; same definition (GBATEK 06). */
static void large_pos(const VCoreState *s, int e, int x, int y, s32 *tx, s32 *ty) {
  s32 pa = (s16)vc_eng16(s, e, VC_REG_BG2PA), pb = (s16)vc_eng16(s, e, VC_REG_BG2PB);
  s32 pc = (s16)vc_eng16(s, e, VC_REG_BG2PC), pd = (s16)vc_eng16(s, e, VC_REG_BG2PD);
  s32 rx = sx28(vc_eng32(s, e, VC_REG_BG2X)), ry = sx28(vc_eng32(s, e, VC_REG_BG2Y));
  *tx = rx + pb * y + pa * x;
  *ty = ry + pd * y + pc * x;
}

/* Mode 6 BG2 (GBATEK 25): a 256-colour rot/scal bitmap, BGxCNT bits 14-15 = 0: 512x1024, 1: 1024x512 (2, 3 are not
 * defined; treated as 1024x512). The data starts at the BG VRAM base (the screen base is NOT used), one byte a
 * pixel, row after row. Index 0 is transparent. BGxCNT bit 13 = wraparound, else outside is transparent. */
void vc_bg_large_line(VCoreState *s, int e, int y, VC_Line *ln) {
  if (e != VC_ENG_A) return; /* the large bitmap exists on engine A only */
  u16 cnt = vc_eng16(s, e, VC_REG_BG2CNT);
  int w = (cnt & 0x4000) ? 1024 : 512, h = (cnt & 0x4000) ? 512 : 1024;
  int wrap = (cnt >> 13) & 1;
  const VC_Region *r = vc_rgn_bg(s, e);
  u32 *dst = ln->bg[2];
  for (int x = 0; x < VC_W; x++) {
    s32 tx, ty;
    large_pos(s, e, x, y, &tx, &ty);
    tx >>= 8;
    ty >>= 8;
    dst[x] = 0;
    if (wrap) {
      tx &= w - 1;
      ty &= h - 1;
    } else if (tx < 0 || tx >= w || ty < 0 || ty >= h) {
      continue;
    }
    u8 idx = vc_rd8(r, (u32)ty * (u32)w + (u32)tx);
    if (idx) dst[x] = VC_PX_OPAQUE | (vc_pal(s, e, 0, idx) & VC_PX_COLOR);
  }
}

/* GBATEK 08: MOSAIC bits 4-7 = BG V size - 1. Every line of a block shows the block's first line. */
int vc_mosaic_bg_y(const VCoreState *s, int e, int n, int y) {
  if (!(vc_eng16(s, e, VC_REG_BG0CNT + 2 * n) & 0x40)) return y;
  int v = ((vc_eng16(s, e, VC_REG_MOSAIC) >> 4) & 15) + 1;
  return y - y % v;
}

/* GBATEK 08: MOSAIC bits 0-3 = BG H size - 1. Pixels 0..H-1 take pixel 0, H..2H-1 pixel H, ... (the upper-left
 * pixel of the block, transparent included). */
void vc_mosaic_bg_line(const VCoreState *s, int e, int n, u32 *px) {
  if (!(vc_eng16(s, e, VC_REG_BG0CNT + 2 * n) & 0x40)) return;
  int h = (vc_eng16(s, e, VC_REG_MOSAIC) & 15) + 1;
  if (h == 1) return;
  for (int x = 0; x < VC_W; x++) px[x] = px[x - x % h];
}
