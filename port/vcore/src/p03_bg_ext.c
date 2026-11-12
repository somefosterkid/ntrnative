/* port/vcore piece 3: extended BGs (affine 16-bit map, 256-colour and direct-colour bitmaps). OWNED BY PIECE 3.
 * Spec: GBATEK "DS Video BG Modes / Control" (extended affine selection by BGxCNT bits 7 and 2, base addresses,
 * sizes, direct colour alpha bit), "LCD I/O BG Rotation/Scaling" (reference point and PA-PD), "LCD VRAM BG Screen
 * Data Format" (map entry bits), "DS Video Extended Palettes" (slot n of BGn, colour 0 transparent).
 *
 * The texel position is computed here (X + y*PB + x*PA, Y + y*PD + x*PC, 24.8) the way vc_bg_affine_pos (piece 2)
 * defines it, so this piece's tests do not depend on piece 2's body. */
#include "vc_shared.h"

static void p03_pos(const VCoreState *s, int e, int n, int y, s32 *x0, s32 *y0, s32 *pa, s32 *pc) {
  u32 base = VC_REG_BG2PA + 0x10u * (u32)(n - 2);
  u32 rx = VC_REG_BG2X + 0x10u * (u32)(n - 2);
  s32 refx = (s32)(vc_eng32(s, e, rx) << 4) >> 4; /* 28-bit signed 19.8 */
  s32 refy = (s32)(vc_eng32(s, e, rx + 4) << 4) >> 4;
  s32 pb = (s16)vc_eng16(s, e, base + 2);
  s32 pd = (s16)vc_eng16(s, e, base + 6);
  *pa = (s16)vc_eng16(s, e, base);
  *pc = (s16)vc_eng16(s, e, base + 4);
  *x0 = refx + y * pb;
  *y0 = refy + y * pd;
}

void vc_bg_ext_line(VCoreState *s, int e, int n, int y, VC_Line *ln) {
  u32 *dst = ln->bg[n];
  u32 dispcnt = vc_eng32(s, e, VC_REG_DISPCNT);
  u32 cnt = vc_eng16(s, e, VC_REG_BG0CNT + 2u * (u32)n);
  int size = (cnt >> 14) & 3;
  int wrap = (cnt >> 13) & 1;
  const VC_Region *vr = vc_rgn_bg(s, e);
  s32 px, py, pa, pc;
  p03_pos(s, e, n, y, &px, &py, &pa, &pc);

  if (cnt & 0x80) {
    /* bitmap: base = BGxCNT bits 8-12 * 16 KB (no DISPCNT part); 128x128, 256x256, 512x256, 512x512 */
    static const int bw[4] = {128, 256, 512, 512}, bh[4] = {128, 256, 256, 512};
    int w = bw[size], h = bh[size];
    int direct = (cnt >> 2) & 1;
    u32 base = ((cnt >> 8) & 31) * 0x4000u;
    for (int x = 0; x < VC_W; x++, px += pa, py += pc) {
      int tx = px >> 8, ty = py >> 8;
      dst[x] = 0;
      if (tx < 0 || tx >= w || ty < 0 || ty >= h) {
        if (!wrap) continue;
        tx &= w - 1;
        ty &= h - 1;
      }
      if (direct) {
        u16 c = vc_rd16(vr, base + ((u32)ty * (u32)w + (u32)tx) * 2);
        if (c & 0x8000) dst[x] = VC_PX_OPAQUE | (c & VC_PX_COLOR);
      } else {
        u8 i = vc_rd8(vr, base + (u32)ty * (u32)w + (u32)tx);
        if (i) dst[x] = VC_PX_OPAQUE | (vc_pal(s, e, 0, i) & VC_PX_COLOR);
      }
    }
    return;
  }

  /* affine map with 16-bit text-style entries, 256-colour tiles: size 128 << size pixels square */
  int dim = 128 << size;
  int tiles = dim >> 3;
  u32 cbase = ((cnt >> 2) & 15) * 0x4000u;
  u32 sbase = ((cnt >> 8) & 31) * 0x800u;
  if (e == 0) {
    cbase += ((dispcnt >> 24) & 7) * 0x10000u;
    sbase += ((dispcnt >> 27) & 7) * 0x10000u;
  }
  int extpal = (dispcnt >> 30) & 1;
  const VC_Region *pr = vc_rgn_bgext(s, e);
  for (int x = 0; x < VC_W; x++, px += pa, py += pc) {
    int tx = px >> 8, ty = py >> 8;
    dst[x] = 0;
    if (tx < 0 || tx >= dim || ty < 0 || ty >= dim) {
      if (!wrap) continue;
      tx &= dim - 1;
      ty &= dim - 1;
    }
    u16 ent = vc_rd16(vr, sbase + (u32)((ty >> 3) * tiles + (tx >> 3)) * 2);
    int fx = tx & 7, fy = ty & 7;
    if (ent & 0x400) fx = 7 - fx;
    if (ent & 0x800) fy = 7 - fy;
    u8 i = vc_rd8(vr, cbase + (u32)(ent & 0x3FF) * 64u + (u32)fy * 8u + (u32)fx);
    if (!i) continue;
    u16 c;
    if (extpal) c = vc_rd16(pr, (u32)n * 0x2000u + (u32)(ent >> 12) * 512u + (u32)i * 2u);
    else c = vc_pal(s, e, 0, i);
    dst[x] = VC_PX_OPAQUE | (c & VC_PX_COLOR);
  }
}
