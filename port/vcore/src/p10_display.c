/* port/vcore piece 10: master brightness and the display modes (docs/vcore.md, piece 10). OWNED BY PIECE 10.
 * Follows GBATEK: DS Video Stuff (MASTER_BRIGHT), LCD I/O Display Control (forced blank, display modes),
 * DS Power Control (POWCNT1 bits 1 and 9). */
#include "vc_shared.h"

/* MASTER_BRIGHT on one 6-bit channel: up New = Old + (63-Old)*F/16, down New = Old - Old*F/16. GBATEK does not say
 * how the division rounds; the oracle (checked on 63 and 38 at F = 8 and 5) rounds the up step down and the amount
 * subtracted going down up, so a white screen at F = 8 down is 63 - 32 = 31. */
static inline u32 p10_chan(u32 v, int mode, u32 f) {
  if (mode == 1) return v + (((63 - v) * f) >> 4);
  return v - ((v * f + 15) >> 4);
}

u32 vc_p10_bright(u32 px, u16 mb) {
  int mode = (mb >> 14) & 3;
  u32 f = mb & 0x1F;
  if (f > 16) f = 16;
  if (mode != 1 && mode != 2) return px; /* 0 disabled, 3 reserved */
  return VC_RGB6(p10_chan(VC_R6(px), mode, f), p10_chan(VC_G6(px), mode, f), p10_chan(VC_B6(px), mode, f));
}

/* display mode 2: line y of a bank (256 BGR555 pixels per line, row y at byte y*512); NULL (bank not in LCDC mode) = 0 */
void vc_p10_vram_line(const u8 *bank, int y, u32 *dst) {
  int x;
  for (x = 0; x < VC_W; x++) {
    const u8 *p = bank ? bank + (y * VC_W + x) * 2 : NULL;
    dst[x] = vc_555_to_rgb6(p ? (u16)(p[0] | p[1] << 8) : 0);
  }
}

void vc_display_line(VCoreState *s, int e, int y, const VC_Line *ln, u32 *dst) {
  u32 dispcnt = vc_eng32(s, e, VC_REG_DISPCNT);
  u16 powcnt = vc_io16(s, VC_REG_POWCNT1);
  int on = (powcnt >> (e ? 9 : 1)) & 1;
  u32 white = VC_RGB6(63, 63, 63);
  int x;

  /* Found by tabulating the oracle over POWCNT1 x display mode x forced blank x MASTER_BRIGHT (GBATEK says none of
   * this; the oracle wins): display mode 0 is white and MASTER_BRIGHT does not touch it. Mode 2 shows the VRAM
   * bank and mode 3 black even when forced blank is set or the engine is powered off. Only mode 1 (engine B: the
   * only mode besides 0) depends on power and forced blank: A off shows black, A forced blank white; B off or forced
   * blank white. Those whites and blacks do go through MASTER_BRIGHT. */
  int mode = (dispcnt >> 16) & (e ? 1 : 3);
  if (mode == 0) {
    for (x = 0; x < VC_W; x++) dst[x] = white;
    return;
  }
  if (mode == 1) {
    if (!on && !e) for (x = 0; x < VC_W; x++) dst[x] = 0;
    else if (!on || (dispcnt & 0x80)) for (x = 0; x < VC_W; x++) dst[x] = white;
    else for (x = 0; x < VC_W; x++) dst[x] = ln->out[x];
  } else if (mode == 2) {
    vc_p10_vram_line(vc_vram_lcdc(s, (dispcnt >> 18) & 3), y, dst);
  } else { /* main memory display: no data */
    for (x = 0; x < VC_W; x++) dst[x] = 0;
  }
  {
    u16 mb = vc_eng16(s, e, VC_REG_MASTER_BRIGHT);
    int bm = (mb >> 14) & 3;
    if ((bm == 1 || bm == 2) && (mb & 0x1F) != 0) /* a factor of 0 (or mode 0 / 3) leaves every pixel as it is */
      for (x = 0; x < VC_W; x++) dst[x] = vc_p10_bright(dst[x], mb);
  }
}
