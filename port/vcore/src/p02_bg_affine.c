/* port/vcore piece 2: affine BGs (docs/vcore.md, piece 2). OWNED BY PIECE 2.
 * Spec: GBATEK 06 (reference point, PA-PD), 04 (BGxCNT, sizes, overflow), 12 (8-bit map entries),
 * 25 (char/screen base with DISPCNT's bits), 18 (palettes). */
#include "vc_shared.h"

/* GBATEK 06: BGxX/Y are 28-bit signed 8.8 (bits 28-31 unused: sign-extend from bit 27);
 * line y starts at X + y*PB, Y + y*PD (the internal registers advance by PB/PD per line, reloaded at vblank),
 * and pixel x adds x*PA, x*PC. PA-PD are signed 8.8 halfwords. Result 24.8 (BG pixels << 8). */
void vc_bg_affine_pos(const VCoreState *s, int e, int n, int x, int y, s32 *tx, s32 *ty) {
  u32 base = (u32)(n - 2) * 0x10;
  s32 pa = (s16)vc_eng16(s, e, VC_REG_BG2PA + base);
  s32 pb = (s16)vc_eng16(s, e, VC_REG_BG2PB + base);
  s32 pc = (s16)vc_eng16(s, e, VC_REG_BG2PC + base);
  s32 pd = (s16)vc_eng16(s, e, VC_REG_BG2PD + base);
  s32 rx = (s32)(vc_eng32(s, e, VC_REG_BG2X + base) << 4) >> 4;
  s32 ry = (s32)(vc_eng32(s, e, VC_REG_BG2Y + base) << 4) >> 4;
  *tx = (s32)((u32)rx + (u32)(pa * x) + (u32)(pb * y));
  *ty = (s32)((u32)ry + (u32)(pc * x) + (u32)(pd * y));
}

void vc_bg_affine_line(VCoreState *s, int e, int n, int y, VC_Line *ln) {
  u32 dispcnt = vc_eng32(s, e, VC_REG_DISPCNT);
  u16 cnt = vc_eng16(s, e, VC_REG_BG0CNT + 2 * n);
  /* GBATEK 25: engine A adds DISPCNT bits 24-26 (char) / 27-29 (screen) x 64 KB; engine B adds 0.
   * Char base is BGxCNT bits 2-5 (x 16 KB), screen base bits 8-12 (x 2 KB). */
  u32 charbase = (u32)((cnt >> 2) & 0xF) * 0x4000;
  u32 scrbase = (u32)((cnt >> 8) & 0x1F) * 0x800;
  if (e == VC_ENG_A) {
    charbase += ((dispcnt >> 24) & 7) * 0x10000;
    scrbase += ((dispcnt >> 27) & 7) * 0x10000;
  }
  int sizebits = 7 + ((cnt >> 14) & 3); /* GBATEK 04: 128 << size dots */
  s32 size = 1 << sizebits;
  int wrap = (cnt >> 13) & 1;
  int tshift = sizebits - 3; /* tiles per row = size / 8 */
  const VC_Region *rgn = vc_rgn_bg(s, e);
  s32 tx, ty, x1, y1;
  vc_bg_affine_pos(s, e, n, 0, y, &tx, &ty);
  vc_bg_affine_pos(s, e, n, 1, y, &x1, &y1);
  s32 dx = x1 - tx, dy = y1 - ty;
  for (int x = 0; x < VC_W; x++) {
    s32 px = tx >> 8, py = ty >> 8; /* arithmetic shift: floor */
    tx += dx;
    ty += dy;
    ln->bg[n][x] = 0;
    if (px < 0 || px >= size || py < 0 || py >= size) {
      if (!wrap) continue;
      px &= size - 1;
      py &= size - 1;
    }
    /* GBATEK 12: one byte per map entry, row-major, tile number 0-255; GBATEK 11: 8bpp tile = 64 bytes */
    u32 tile = vc_rd8(rgn, scrbase + ((u32)(py >> 3) << tshift) + (u32)(px >> 3));
    u32 idx = vc_rd8(rgn, charbase + tile * 64 + (u32)(py & 7) * 8 + (u32)(px & 7));
    if (idx == 0) continue; /* colour 0 transparent */
    ln->bg[n][x] = VC_PX_OPAQUE | (vc_pal(s, e, 0, (int)idx) & VC_PX_COLOR);
  }
  ln->bg_on[n] = 1;
}
