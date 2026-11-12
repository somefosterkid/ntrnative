/* port/vcore piece 1: text BGs (docs/vcore.md, piece 1). OWNED BY PIECE 1.
 * Written from GBATEK: 04 (BGxCNT, map sizes), 05 (scroll), 11 (tile data), 12 (text map entries), 18 (colour 0
 * transparent), 25 (char/screen base, DS extended palette slots), 27 (extended palettes), 24 (DISPCNT bits). */
#include "vc_shared.h"

/* The text BG map entry address for BG pixel (px, py): GBATEK 04: the map is 1, 2 or 4 screen blocks of 32x32
 * entries (2 KB each), SC0 at base, SC1 at +2K, SC2 at +4K, SC3 at +6K. Size 1 = SC0 SC1 side by side, size 2 =
 * SC0 over SC1, size 3 = SC0 SC1 / SC2 SC3. */
static u32 text_map_addr(u32 base, int size, int px, int py) {
  u32 blk = 0;
  if (size == 1) blk = (u32)(px >> 8) & 1;
  else if (size == 2) blk = (u32)(py >> 8) & 1;
  else if (size == 3) blk = ((u32)(px >> 8) & 1) | (((u32)(py >> 8) & 1) << 1);
  return base + blk * 0x800 + ((u32)((py >> 3) & 31) * 32 + (u32)((px >> 3) & 31)) * 2;
}

void vc_bg_text_line(VCoreState *s, int e, int n, int y, VC_Line *ln) {
  u32 *dst = ln->bg[n];
  const u16 cnt = vc_eng16(s, e, VC_REG_BG0CNT + 2 * n);
  const u32 dispcnt = vc_eng32(s, e, VC_REG_DISPCNT);
  const int hofs = vc_eng16(s, e, VC_REG_BG0HOFS + 4 * n) & 0x1FF; /* GBATEK 05: bits 0-8 */
  const int vofs = vc_eng16(s, e, VC_REG_BG0VOFS + 4 * n) & 0x1FF;
  /* GBATEK 25: engine A adds DISPCNT bits 24-26 * 64K to the char base and bits 27-29 * 64K to the screen base;
   * engine B has none. BGxCNT bits 2-5 are the char base in 16K units. */
  u32 char_base = ((u32)(cnt >> 2) & 0xF) * 0x4000;
  u32 scr_base = ((u32)(cnt >> 8) & 0x1F) * 0x800;
  if (e == VC_ENG_A) {
    char_base += ((dispcnt >> 24) & 7) * 0x10000;
    scr_base += ((dispcnt >> 27) & 7) * 0x10000;
  }
  const int bpp8 = (cnt >> 7) & 1;
  const int size = (cnt >> 14) & 3;
  const int extpal = (int)((dispcnt >> 30) & 1) && bpp8; /* GBATEK 27: extended palettes only for 256-colour tiles */
  /* slot: BGn uses slot n; BG0 / BG1 pick slot 2 / 3 by BGxCNT bit 13 (GBATEK 25) */
  int slot = n;
  if (n < 2 && ((cnt >> 13) & 1)) slot = n + 2;
  const VC_Region *bg = vc_rgn_bg(s, e);
  const VC_Region *ext = vc_rgn_bgext(s, e);

  const int width_mask = (size & 1) ? 511 : 255;
  const int height_mask = (size & 2) ? 511 : 255;
  const int py = (y + vofs) & height_mask;
  const int ty = py & 7;

  int cached_col = -1;
  u16 ent = 0;
  u32 row0 = 0, row1 = 0; /* the tile row's 4 (4bpp) or 8 (8bpp) bytes, low byte first */
  for (int x = 0; x < VC_W; x++) {
    int px = (x + hofs) & width_mask;
    int newtile = (px >> 3) != cached_col;
    if (newtile) {
      cached_col = px >> 3;
      ent = vc_rd16(bg, text_map_addr(scr_base, size, px, py));
    }
    u32 tile = ent & 0x3FF;
    int fx = px & 7, fy = ty;
    if (ent & 0x0400) fx = 7 - fx; /* GBATEK 12: bit 10 H flip, 11 V flip */
    if (ent & 0x0800) fy = 7 - fy;
    int pal = (ent >> 12) & 15;
    if (newtile) {
      /* the row sits in one page: 4-byte aligned reads (a flip changes fy, so fetch the flipped row) */
      if (bpp8) {
        u32 a = char_base + tile * 64 + (u32)fy * 8;
        row0 = vc_rd32(bg, a);
        row1 = vc_rd32(bg, a + 4);
      } else {
        row0 = vc_rd32(bg, char_base + tile * 32 + (u32)fy * 4);
      }
    }
    u32 col = 0;
    if (bpp8) {
      u32 idx = ((fx < 4 ? row0 : row1) >> ((fx & 3) * 8)) & 0xFF;
      if (idx) {
        if (extpal) col = vc_rd16(ext, (u32)slot * 0x2000 + (u32)pal * 512 + idx * 2);
        else col = vc_pal(s, e, 0, (int)idx);
        col = VC_PX_OPAQUE | (col & VC_PX_COLOR);
      }
    } else {
      u32 b = (row0 >> ((fx >> 1) * 8)) & 0xFF;
      u32 idx = (fx & 1) ? (b >> 4) : (b & 15); /* GBATEK 11: low nibble = left dot */
      if (idx) col = VC_PX_OPAQUE | (vc_pal(s, e, 0, pal * 16 + (int)idx) & VC_PX_COLOR);
    }
    dst[x] = col;
  }
}
