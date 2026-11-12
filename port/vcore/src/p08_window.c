/* port/vcore piece 8: windows. GBATEK "LCD I/O Window Feature" (07) and DISPCNT (02).
 * Builds ln->winmask[] (VC_WIN_* bits: BG0-3, OBJ, colour effect) for each pixel of a line. */
#include "vc_shared.h"

/* is coordinate c inside the range [a, b)?  a = bits 8-15, b = bits 0-7 of the register.
 * a < b: a <= c < b.  a > b: the range wraps (c >= a or c < b), as this piece's spec states; a == b: empty. */
static int win_in(int c, int a, int b) {
  if (a == b) return 0;
  if (a < b) return c >= a && c < b;
  return c >= a || c < b;
}

/* Y range [a, b): a per-window flag, set at line a, cleared at line b (b wins when a == b), kept from line to
 * line. GBATEK describes only the in-frame part. At line 0 the oracle's flag is the one the previous frame left,
 * and it behaves as if the event order in time were rotated to start at line 7 (fitted over all a, b < 24 and
 * a, b up to 255 with a backdrop + BLDY probe scene): lines 0..6 events have already happened when
 * line 0 is drawn.  So a > b wraps (flag still set from the previous frame) but only when a is later than b in the
 * order 7, 8 .. 262, 0 .. 6.  Lines beyond 191 count (VBlank lines run the flag too). */
static int win_in_y(int y, int a, int b) {
  int ka = a < 7 ? a + 263 : a, kb = b < 7 ? b + 263 : b;
  int on = a != b && ka > kb;  /* the flag entering line 0 */
  /* events at lines 0..y in time order */
  if (a == b) {
    if (b <= y) on = 0;
    return on;
  }
  if (a <= y && b <= y) on = a > b;
  else if (a <= y) on = 1;
  else if (b <= y) on = 0;
  return on;
}

void vc_window_line(VCoreState *s, int e, int y, VC_Line *ln) {
  u32 dispcnt = vc_eng32(s, e, VC_REG_DISPCNT);
  int w0 = (dispcnt >> 13) & 1, w1 = (dispcnt >> 14) & 1, wo = (dispcnt >> 15) & 1;
  if (!(w0 || w1 || wo)) {
    for (int x = 0; x < VC_W; x++) ln->winmask[x] = 0x3F;
    return;
  }
  u16 winin = vc_eng16(s, e, VC_REG_WININ), winout = vc_eng16(s, e, VC_REG_WINOUT);
  u16 h0 = vc_eng16(s, e, VC_REG_WIN0H), h1 = vc_eng16(s, e, VC_REG_WIN1H);
  u16 v0 = vc_eng16(s, e, VC_REG_WIN0V), v1 = vc_eng16(s, e, VC_REG_WIN1V);
  int y0 = w0 && win_in_y(y, v0 >> 8, v0 & 255);
  int y1 = w1 && win_in_y(y, v1 >> 8, v1 & 255);
  u8 m_w0 = winin & 0x3F, m_w1 = (winin >> 8) & 0x3F, m_obj = (winout >> 8) & 0x3F, m_out = winout & 0x3F;
  for (int x = 0; x < VC_W; x++) {
    u8 m;
    if (y0 && win_in(x, h0 >> 8, h0 & 255)) m = m_w0;
    else if (y1 && win_in(x, h1 >> 8, h1 & 255)) m = m_w1;
    else if (wo && ln->objwin[x]) m = m_obj;
    else m = m_out;
    ln->winmask[x] = m;
  }
}
