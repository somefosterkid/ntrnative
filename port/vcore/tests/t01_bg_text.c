/* port/vcore piece 1's unit tests: text BGs (docs/vcore.md, piece 1). OWNED BY PIECE 1.
 * Run: build/vcore/vcore_tests p01
 * Expected values are worked by hand from GBATEK 04 (BGxCNT, map sizes), 05 (scroll), 11 (tile data), 12 (map
 * entries), 18 (colour 0), 24/25 (char/screen base), 27 (extended palettes). */
#include "vct.h"

#include <stdlib.h>

#define PX(c) (VC_PX_OPAQUE | (c))
enum { BGV_SIZE = 0x80000 };

/* engine A / B BG VRAM (512 KB / 128 KB as the engine sees it) and the BG extended palette (32 KB) */
static u8 V[BGV_SIZE];
static u8 X[0x8000];

static void put8(u32 a, u8 v) { V[a] = v; }
static void put16(u32 a, u16 v) { V[a] = (u8)v; V[a + 1] = (u8)(v >> 8); }
static void putx(u32 a, u16 v) { X[a] = (u8)v; X[a + 1] = (u8)(v >> 8); }

/* a state with engine e in display mode 1, BG mode 0, and the VRAM mapped flat */
static VCoreState *mk(int e) {
  VCoreState *s = vct_state();
  memset(V, 0, sizeof V);
  memset(X, 0, sizeof X);
  vct_eng32(s, e, VC_REG_DISPCNT, 0x00010100);
  vct_map_flat(s, e ? VC_RGN_BBG : VC_RGN_ABG, V, e ? 0x20000 : BGV_SIZE);
  vct_map_flat(s, e ? VC_RGN_BBGEXT : VC_RGN_ABGEXT, X, sizeof X);
  return s;
}
static void cnt(VCoreState *s, int e, int n, u16 v) { vct_eng16(s, e, VC_REG_BG0CNT + 2 * n, v); }
static void scroll(VCoreState *s, int e, int n, u16 h, u16 v) {
  vct_eng16(s, e, VC_REG_BG0HOFS + 4 * n, h);
  vct_eng16(s, e, VC_REG_BG0VOFS + 4 * n, v);
}
static void dispcnt(VCoreState *s, int e, u32 v) { vct_eng32(s, e, VC_REG_DISPCNT, v); }
static VC_Line *draw(VCoreState *s, int e, int n, int y) {
  memset(&s->line, 0, sizeof s->line);
  vc_bg_text_line(s, e, n, y, &s->line);
  return &s->line;
}
/* a 4bpp tile (32 bytes at tile * 32) filled with palette index idx */
static void solid4(int tile, int idx) {
  for (int i = 0; i < 32; i++) put8((u32)tile * 32 + (u32)i, (u8)(idx * 0x11));
}

/* ---- 4bpp tiles and palette numbers ---- */
VCT_TEST(p01_4bpp_palette_numbers) {
  VCoreState *s = mk(VC_ENG_A);
  cnt(s, 0, 0, 0x0100); /* char base 0, screen base 1 (0x800), 256x256, 4bpp */
  /* tile 1, row 0: bytes 21 43 65 07 -> the low nibble is the LEFT dot: idx 1 2 3 4 5 6 7 0 (GBATEK 11) */
  put8(0x20, 0x21); put8(0x21, 0x43); put8(0x22, 0x65); put8(0x23, 0x07);
  put16(0x800, 0x3001); /* map (0,0): tile 1, palette 3 (GBATEK 12) */
  for (int i = 1; i < 8; i++) vct_pal(s, 0, 0, 3 * 16 + i, (u16)(0x0100 + i));
  vct_pal(s, 0, 0, 1 * 16 + 1, 0x7FFF); /* palette 1 is not the one chosen */
  VC_Line *ln = draw(s, 0, 0, 0);
  for (int i = 0; i < 7; i++) VCT_EQ(ln->bg[0][i], PX(0x0100 + i + 1));
  VCT_EQ(ln->bg[0][7], 0); /* idx 0 */
  VCT_EQ(ln->bg[0][8], 0); /* tile 0 (blank) */
  ln = draw(s, 0, 0, 1);   /* row 1 of tile 1 is blank */
  for (int i = 0; i < 8; i++) VCT_EQ(ln->bg[0][i], 0);
}

/* ---- 8bpp tiles: the byte is the palette index, the map's palette number is unused ---- */
VCT_TEST(p01_8bpp) {
  VCoreState *s = mk(VC_ENG_A);
  cnt(s, 0, 1, 0x0180); /* BG1, 256 colours, screen base 1 */
  put8(2 * 64 + 0, 5); put8(2 * 64 + 1, 0); put8(2 * 64 + 2, 200); put8(2 * 64 + 7, 9);
  put8(2 * 64 + 8, 7); /* row 1 */
  put16(0x800, 0x7002); /* tile 2, palette number 7: ignored without extended palettes */
  vct_pal(s, 0, 0, 5, 0x1234); vct_pal(s, 0, 0, 200, 0x4321); vct_pal(s, 0, 0, 9, 0x0042);
  vct_pal(s, 0, 0, 7 * 16 + 5, 0x7FFF); vct_pal(s, 0, 0, 7, 0x0777);
  VC_Line *ln = draw(s, 0, 1, 0);
  VCT_EQ(ln->bg[1][0], PX(0x1234));
  VCT_EQ(ln->bg[1][1], 0);
  VCT_EQ(ln->bg[1][2], PX(0x4321));
  VCT_EQ(ln->bg[1][3], 0);
  VCT_EQ(ln->bg[1][7], PX(0x0042));
  ln = draw(s, 0, 1, 1);
  VCT_EQ(ln->bg[1][0], PX(0x0777));
  VCT_EQ(ln->bg[1][1], 0);
}

/* ---- the four map sizes: 2 KB screen blocks SC0..SC3 (GBATEK 04) ---- */
static void sizes_setup(VCoreState *s, int size) {
  cnt(s, 0, 0, (u16)(0x0100 | size << 14)); /* screen base 1: SC0 0x800, SC1 0x1000, SC2 0x1800, SC3 0x2000 */
  for (int k = 1; k <= 4; k++) {
    solid4(k, k);
    vct_pal(s, 0, 0, k, (u16)(0x1000 + k));
  }
  put16(0x800, 1); put16(0x1000, 2); put16(0x1800, 3); put16(0x2000, 4); /* entry (0,0) of SC0..SC3 */
}
VCT_TEST(p01_map_size_0_256x256) {
  VCoreState *s = mk(VC_ENG_A);
  sizes_setup(s, 0);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1001));
  scroll(s, 0, 0, 256, 256); /* 256 & 0x1FF = 256, but the 256x256 screen wraps around (GBATEK 04) */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1001));
}
VCT_TEST(p01_map_size_1_512x256) {
  VCoreState *s = mk(VC_ENG_A);
  sizes_setup(s, 1);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1001));
  scroll(s, 0, 0, 256, 0); /* SC1 is to the right of SC0 */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1002));
  scroll(s, 0, 0, 0, 256); /* 256 high: the vertical scroll wraps back to SC0 */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1001));
  scroll(s, 0, 0, 255, 0); /* x=0 is pixel 255 (SC0 col 31, blank), x=1 is pixel 256 (SC1) */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], 0);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][1], PX(0x1002));
}
VCT_TEST(p01_map_size_2_256x512) {
  VCoreState *s = mk(VC_ENG_A);
  sizes_setup(s, 2);
  scroll(s, 0, 0, 0, 256); /* SC1 is below SC0 */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1002));
  scroll(s, 0, 0, 256, 0); /* 256 wide: wraps to SC0 */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1001));
  scroll(s, 0, 0, 0, 250); /* line 6 is background row 256 (SC1 row 0), line 5 is row 255 (SC0 row 31, blank) */
  VCT_EQ(draw(s, 0, 0, 5)->bg[0][0], 0);
  VCT_EQ(draw(s, 0, 0, 6)->bg[0][0], PX(0x1002));
}
VCT_TEST(p01_map_size_3_512x512) {
  VCoreState *s = mk(VC_ENG_A);
  sizes_setup(s, 3);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1001));
  scroll(s, 0, 0, 256, 0);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1002)); /* SC1 right of SC0 */
  scroll(s, 0, 0, 0, 256);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1003)); /* SC2 below SC0 */
  scroll(s, 0, 0, 256, 256);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1004)); /* SC3 */
  scroll(s, 0, 0, 504, 0);                        /* x=8 is pixel 512 = 0 again: SC0 */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][8], PX(0x1001));
}
VCT_TEST(p01_scroll_9bit_wrap) {
  VCoreState *s = mk(VC_ENG_A);
  sizes_setup(s, 3);
  put16(0x1000 + 2 * 31, 4); /* SC1 col 31 = tile 4 (solid idx 4) */
  scroll(s, 0, 0, 511, 0);
  /* x=0 -> pixel 511 (SC1 col 31, tile 4); x=1 -> pixel 0 (SC0 col 0, tile 1) */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1004));
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][1], PX(0x1001));
  /* bits 9-15 of the offset are not used: 0xFE00 acts as 0, 0x205 as 5 */
  scroll(s, 0, 0, 0xFE00, 0xFE00);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1001));
  scroll(s, 0, 0, 0x0205, 0);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x1001)); /* pixel 5 is still inside tile col 0 */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][3], 0); /* pixel 8 is tile col 1: entry 0 = tile 0, blank */
}
VCT_TEST(p01_scroll_pixels) {
  VCoreState *s = mk(VC_ENG_A);
  cnt(s, 0, 0, 0x0100);
  /* tile 1: row 0 = idx 1..8 (bytes 21 43 65 87), row 7 = idx 2 3 */
  put8(0x20, 0x21); put8(0x21, 0x43); put8(0x22, 0x65); put8(0x23, 0x87);
  put8(0x20 + 28, 0x32);
  for (int i = 1; i <= 8; i++) vct_pal(s, 0, 0, i, (u16)(0x0200 + i));
  put16(0x800, 1);               /* map (0,0) = tile 1 */
  put16(0x800 + 2 * 31, 1);      /* map (31,0) = tile 1 */
  put16(0x800 + 2 * 32 * 31, 1); /* map (0,31) = tile 1 */
  scroll(s, 0, 0, 5, 0);         /* x=0 shows pixel 5: idx 6; x=2 pixel 7: idx 8; x=3 pixel 8: blank */
  VC_Line *ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], PX(0x0206));
  VCT_EQ(ln->bg[0][2], PX(0x0208));
  VCT_EQ(ln->bg[0][3], 0);
  scroll(s, 0, 0, 255, 0); /* x=0 pixel 255 = tile col 31 pixel 7: idx 8; x=1 pixel 0: idx 1 */
  ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], PX(0x0208));
  VCT_EQ(ln->bg[0][1], PX(0x0201));
  scroll(s, 0, 0, 0, 0x1FF); /* y=0 shows row 255 (the 256 high BG wraps: map row 31, tile row 7: idx 2 3) */
  ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], PX(0x0202));
  VCT_EQ(ln->bg[0][1], PX(0x0203));
  ln = draw(s, 0, 0, 1); /* y=1 shows row 0 */
  VCT_EQ(ln->bg[0][0], PX(0x0201));
  scroll(s, 0, 0, 0, 7); /* y=0 -> row 7 of tile row 0 */
  ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], PX(0x0202));
}

/* ---- flips (map entry bits 10, 11) ---- */
VCT_TEST(p01_flips) {
  VCoreState *s = mk(VC_ENG_A);
  cnt(s, 0, 0, 0x0100);
  put8(0x20, 0x21); put8(0x21, 0x43); put8(0x22, 0x65); put8(0x23, 0x87); /* row 0: idx 1..8 */
  put8(0x20 + 28, 0x32);                                                   /* row 7: idx 2 3 0 ... */
  for (int i = 1; i <= 8; i++) vct_pal(s, 0, 0, i, (u16)(0x0300 + i));
  put16(0x800, 0x0401); /* H flip */
  put16(0x802, 0x0801); /* V flip */
  put16(0x804, 0x0C01); /* both */
  put16(0x806, 0x0001); /* none */
  VC_Line *ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], PX(0x0308)); /* H flip: x0 = idx 8, x7 = idx 1 */
  VCT_EQ(ln->bg[0][7], PX(0x0301));
  VCT_EQ(ln->bg[0][8], PX(0x0302)); /* V flip: line 0 = tile row 7 (idx 2 3 at x0 x1) */
  VCT_EQ(ln->bg[0][9], PX(0x0303));
  VCT_EQ(ln->bg[0][10], 0);
  VCT_EQ(ln->bg[0][16 + 7], PX(0x0302)); /* both: x7 = idx 2, x6 = idx 3 */
  VCT_EQ(ln->bg[0][16 + 6], PX(0x0303));
  VCT_EQ(ln->bg[0][16], 0);
  VCT_EQ(ln->bg[0][24], PX(0x0301)); /* none */
  VCT_EQ(ln->bg[0][31], PX(0x0308));
  ln = draw(s, 0, 0, 7); /* V flip: line 7 shows tile row 0 */
  VCT_EQ(ln->bg[0][8], PX(0x0301));
  VCT_EQ(ln->bg[0][15], PX(0x0308));
}
VCT_TEST(p01_flips_8bpp) {
  VCoreState *s = mk(VC_ENG_A);
  cnt(s, 0, 0, 0x0180);
  for (int i = 0; i < 8; i++) put8(64 + (u32)i, (u8)(10 + i));       /* tile 1 row 0 */
  for (int i = 0; i < 8; i++) put8(64 + 56 + (u32)i, (u8)(20 + i));  /* row 7 */
  for (int i = 10; i < 28; i++) vct_pal(s, 0, 0, i, (u16)(0x0400 + i));
  put16(0x800, 0x0401); put16(0x802, 0x0801);
  VC_Line *ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], PX(0x0400 + 17));
  VCT_EQ(ln->bg[0][7], PX(0x0400 + 10));
  VCT_EQ(ln->bg[0][8], PX(0x0400 + 20));
  VCT_EQ(ln->bg[0][15], PX(0x0400 + 27));
}

/* ---- DISPCNT's char base (bits 24-26) and screen base (bits 27-29), engine A only (GBATEK 25) ---- */
VCT_TEST(p01_dispcnt_bases_engine_a) {
  VCoreState *s = mk(VC_ENG_A);
  /* char base = BGxCNT 1 * 16K + DISPCNT 2 * 64K = 0x24000; screen base = 3 * 2K + 3 * 64K = 0x31800 */
  dispcnt(s, 0, 0x00010100 | (2u << 24) | (3u << 27));
  cnt(s, 0, 0, (u16)((1 << 2) | (3 << 8)));
  put8(0x24000 + 0x20, 0x51); /* tile 1: idx 1 5 */
  put16(0x31800, 1);
  vct_pal(s, 0, 0, 1, 0x0A01); vct_pal(s, 0, 0, 5, 0x0A05);
  VC_Line *ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], PX(0x0A01));
  VCT_EQ(ln->bg[0][1], PX(0x0A05));
  /* the highest: DISPCNT 7 (448K) + BGxCNT 15 (240K) = 688K wraps in the 512 KB region */
  dispcnt(s, 0, 0x00010100 | (7u << 24) | (7u << 27));
  cnt(s, 0, 0, (u16)((15 << 2) | (31 << 8)));
  u32 cb = (7 * 0x10000 + 15 * 0x4000) & 0x7FFFF, sb = (7 * 0x10000 + 31 * 0x800) & 0x7FFFF;
  put8(cb + 0x20, 0x07);
  put16(sb, 1);
  vct_pal(s, 0, 0, 7, 0x0A07);
  ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], PX(0x0A07));
}
VCT_TEST(p01_bgcnt_char_base_bits_2_to_5) {
  /* BGxCNT bits 2-5 are the char base (bits 4-5 are the DS extension) */
  VCoreState *s = mk(VC_ENG_A);
  cnt(s, 0, 0, (u16)((0xA << 2) | (1 << 8)));
  put8(0xA * 0x4000 + 0x20, 0x03);
  put16(0x800, 1);
  vct_pal(s, 0, 0, 3, 0x0B03);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x0B03));
  /* BGxCNT bit 6 is the mosaic flag and must not move the base */
  cnt(s, 0, 0, (u16)((0xA << 2) | (1 << 8) | 0x40));
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x0B03));
}
VCT_TEST(p01_engine_b_no_dispcnt_bases) {
  VCoreState *s = mk(VC_ENG_B);
  /* the bits are set in engine B's DISPCNT but have no effect: char base 1 * 16K, screen base 2 * 2K */
  dispcnt(s, 1, 0x00010100 | (2u << 24) | (3u << 27));
  cnt(s, 1, 0, (u16)((1 << 2) | (2 << 8)));
  put8(0x4000 + 0x20, 0x21);
  put16(0x1000, 1);
  vct_pal(s, 1, 0, 1, 0x0C01); vct_pal(s, 1, 0, 2, 0x0C02);
  VC_Line *ln = draw(s, 1, 0, 0);
  VCT_EQ(ln->bg[0][0], PX(0x0C01));
  VCT_EQ(ln->bg[0][1], PX(0x0C02));
  VCT_EQ(ln->bg[0][2], 0);
  /* engine B uses its own palette (B BG at 0x400) */
  vct_pal(s, 0, 0, 1, 0x7FFF);
  VCT_EQ(draw(s, 1, 0, 0)->bg[0][0], PX(0x0C01));
}

/* ---- colour 0 is transparent whatever the palette holds (GBATEK 18) ---- */
VCT_TEST(p01_color0_transparent) {
  VCoreState *s = mk(VC_ENG_A);
  cnt(s, 0, 0, 0x0100);
  put8(0x20, 0x10); /* idx 0 then idx 1 */
  put16(0x800, 0x2001);
  vct_pal(s, 0, 0, 0, 0x7FFF);
  vct_pal(s, 0, 0, 2 * 16, 0x7FFF);     /* palette 2 colour 0 */
  vct_pal(s, 0, 0, 2 * 16 + 1, 0x0000); /* black, but idx 1: visible */
  VC_Line *ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], 0);
  VCT_EQ(ln->bg[0][1], PX(0x0000));
  cnt(s, 0, 0, 0x0180); /* 8bpp: idx 0 is transparent too */
  put8(64, 0); put8(65, 1);
  put16(0x800, 1);
  vct_pal(s, 0, 0, 1, 0x0000);
  ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], 0);
  VCT_EQ(ln->bg[0][1], PX(0));
  vct_pal(s, 0, 0, 1, 0x8123); /* the colour's bit 15 is not used */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][1], PX(0x0123));
}

/* ---- the tile number has 10 bits ---- */
VCT_TEST(p01_tile_number_10_bits) {
  VCoreState *s = mk(VC_ENG_A);
  cnt(s, 0, 0, (u16)(16 << 8)); /* 4bpp, screen base 16 = 0x8000 */
  put8(1023 * 32, 0x05);        /* tile 1023 at 0x7FE0 */
  put16(0x8000, 0x03FF);        /* tile 1023, no flip, palette 0 */
  put16(0x8002, 0x07FF);        /* the same with H flip: bit 10 is the flip, not a tile bit */
  vct_pal(s, 0, 0, 5, 0x0D05);
  VC_Line *ln = draw(s, 0, 0, 0);
  VCT_EQ(ln->bg[0][0], PX(0x0D05));
  VCT_EQ(ln->bg[0][1], 0);
  VCT_EQ(ln->bg[0][8 + 7], PX(0x0D05));
  cnt(s, 0, 0, (u16)(0x0080 | (2 << 2) | (1 << 8))); /* 8bpp: tile 1023 at 1023 * 64 from char base 2 = 0x8000 */
  put8(0x8000 + 1023 * 64 + 3, 0x66);
  put16(0x800, 0x03FF);
  vct_pal(s, 0, 0, 0x66, 0x0D66);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][3], PX(0x0D66));
}

/* ---- extended palettes: DISPCNT bit 30, 256-colour tiles with 16-bit map entries (GBATEK 27, 25) ---- */
VCT_TEST(p01_extpal_slots) {
  VCoreState *s = mk(VC_ENG_A);
  dispcnt(s, 0, 0x00010100 | (1u << 30));
  cnt(s, 0, 0, 0x0180); /* BG0: slot 0 (BGxCNT bit 13 clear); tile 1 idx 5 at x0, palette number 2 */
  put8(64, 5);
  put16(0x800, 0x2001);
  putx(0 * 0x2000 + 2 * 512 + 5 * 2, 0x0E10);
  putx(1 * 0x2000 + 2 * 512 + 5 * 2, 0x0E11);
  putx(2 * 0x2000 + 2 * 512 + 5 * 2, 0x0E12);
  putx(3 * 0x2000 + 2 * 512 + 5 * 2, 0x0E13);
  vct_pal(s, 0, 0, 5, 0x7FFF); /* the standard palette is not used */
  vct_pal(s, 0, 0, 2 * 16 + 5, 0x7FFF);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x0E10));
  cnt(s, 0, 0, 0x2180); /* BG0 + bit 13: slot 2 */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x0E12));
  cnt(s, 0, 1, 0x0180); /* BG1: slot 1, with bit 13: slot 3 */
  VCT_EQ(draw(s, 0, 1, 0)->bg[1][0], PX(0x0E11));
  cnt(s, 0, 1, 0x2180);
  VCT_EQ(draw(s, 0, 1, 0)->bg[1][0], PX(0x0E13));
  cnt(s, 0, 2, 0x2180); /* BG2, BG3: slots 2, 3 (bit 13 is their overflow flag, no slot select) */
  VCT_EQ(draw(s, 0, 2, 0)->bg[2][0], PX(0x0E12));
  cnt(s, 0, 3, 0x2180);
  VCT_EQ(draw(s, 0, 3, 0)->bg[3][0], PX(0x0E13));
  put8(64, 0); /* idx 0 stays transparent */
  putx(2 * 0x2000 + 2 * 512, 0x7FFF);
  cnt(s, 0, 2, 0x0180);
  VCT_EQ(draw(s, 0, 2, 0)->bg[2][0], 0);
}
VCT_TEST(p01_extpal_off_and_4bpp) {
  VCoreState *s = mk(VC_ENG_A);
  putx(5 * 2, 0x0F55);
  put16(0x800, 1);
  /* extended palettes enabled, but 16-colour tiles still use the standard palette (GBATEK 27) */
  dispcnt(s, 0, 0x00010100 | (1u << 30));
  cnt(s, 0, 0, 0x0100);
  put8(0x20, 0x05);
  vct_pal(s, 0, 0, 5, 0x0F05);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x0F05));
  dispcnt(s, 0, 0x00010100); /* 8bpp with the bit clear: the standard palette */
  cnt(s, 0, 0, 0x0180);
  put8(64, 5);
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x0F05));
  dispcnt(s, 0, 0x00010100 | (1u << 30)); /* ... and with it set: the extended slot 0 */
  VCT_EQ(draw(s, 0, 0, 0)->bg[0][0], PX(0x0F55));
}
VCT_TEST(p01_extpal_engine_b) {
  VCoreState *s = mk(VC_ENG_B);
  dispcnt(s, 1, 0x00010100 | (1u << 30));
  cnt(s, 1, 1, 0x2180); /* BG1 + bit 13: slot 3 */
  put8(64, 7);
  put16(0x800, 0xF001); /* palette 15 */
  putx(3 * 0x2000 + 15 * 512 + 7 * 2, 0x0F77);
  VCT_EQ(draw(s, 1, 1, 0)->bg[1][0], PX(0x0F77));
}

/* ---- scrolling is a shift of the unscrolled picture ---- */
static u32 rs;
static u32 rnd(void) { rs = rs * 1664525u + 1013904223u; return rs >> 8; }
static void random_fill(VCoreState *s, int e) {
  for (u32 i = 0; i < BGV_SIZE; i++) V[i] = (u8)(rnd() & rnd());
  for (u32 i = 0; i < sizeof X; i++) X[i] = (u8)(rnd() & rnd());
  for (int i = 0; i < 256; i++) vct_pal(s, e, 0, i, (u16)(rnd() & 0x7FFF));
}
VCT_TEST(p01_scroll_is_a_shifted_picture) {
  VCoreState *s = mk(VC_ENG_A);
  rs = 7;
  random_fill(s, 0);
  static u32 plain[256][256];
  cnt(s, 0, 2, 0x0904); /* 256x256, 4bpp, char base 1, screen base 9 */
  scroll(s, 0, 2, 0, 0);
  for (int y = 0; y < 192; y++) memcpy(plain[y], draw(s, 0, 2, y)->bg[2], sizeof plain[y]);
  scroll(s, 0, 2, 37, 201);
  int checked = 0;
  for (int y = 0; y < 192; y++) {
    int py = (y + 201) & 255;
    if (py >= 192) continue;
    VC_Line *ln = draw(s, 0, 2, y);
    for (int x = 0; x < 256; x++) {
      VCT_EQ(ln->bg[2][x], plain[py][(x + 37) & 255]);
      checked++;
    }
  }
  VCT_CHECK(checked > 256 * 100);
}

/* ---- synthetic scenes for the oracle (VCT_EMIT=DIR): random tiles and maps in an otherwise plain frame ---- */
static void emit_scene(const char *name, int e, int n, u16 bgcnt, u16 hofs, u16 vofs, u32 dispcnt_extra, int ext) {
  VCoreState *s = vct_state();
  rs = 12345 + (u32)n * 77 + bgcnt;
  memset(V, 0, sizeof V);
  memset(X, 0, sizeof X);
  random_fill(s, e);
  vct_io16(s, VC_REG_POWCNT1, 0x820F);
  vct_eng32(s, e, VC_REG_DISPCNT, 0x00010000 | (1u << (8 + n)) | dispcnt_extra);
  vct_pal(s, e, 0, 0, 0x2800); /* a backdrop that shows through transparent pixels */
  cnt(s, e, n, bgcnt);
  scroll(s, e, n, hofs, vofs);
  if (e == VC_ENG_A) {
    memcpy(s->vram, V, BGV_SIZE); /* banks A-D are contiguous: MST 1, offsets 0-3 */
    vct_io8(s, VC_REG_VRAMCNT_A + 0, 0x81);
    vct_io8(s, VC_REG_VRAMCNT_A + 1, 0x89);
    vct_io8(s, VC_REG_VRAMCNT_A + 2, 0x91);
    vct_io8(s, VC_REG_VRAMCNT_A + 3, 0x99);
    if (ext) { /* bank E (64 KB) as the BG extended palette: MST 4 */
      memcpy(vc_bank(s, VC_BANK_E), X, sizeof X);
      vct_io8(s, VC_REG_VRAMCNT_A + 4, 0x84);
    }
  } else {
    memcpy(vc_bank(s, VC_BANK_C), V, 0x20000); /* bank C MST 4 = engine B BG */
    vct_io8(s, VC_REG_VRAMCNT_A + 2, 0x84);
    if (ext) { /* bank H (32 KB) MST 2 = engine B BG extended palette */
      memcpy(vc_bank(s, VC_BANK_H), X, sizeof X);
      vct_io8(s, 0x248, 0x82);
    }
  }
  vct_emit(s, name);
  /* the same state through the real code: each line draws only its own BG, with enough opaque pixels */
  vct_map_flat(s, e ? VC_RGN_BBG : VC_RGN_ABG, V, e ? 0x20000 : BGV_SIZE);
  vct_map_flat(s, e ? VC_RGN_BBGEXT : VC_RGN_ABGEXT, X, sizeof X);
  int opaque = 0;
  for (int y = 0; y < 192; y++) {
    VC_Line *ln = draw(s, e, n, y);
    for (int x = 0; x < 256; x++) opaque += (ln->bg[n][x] & VC_PX_OPAQUE) != 0;
    for (int o = 0; o < 4; o++)
      if (o != n)
        for (int x = 0; x < 256; x++)
          if (ln->bg[o][x]) { vct_fail(__FILE__, __LINE__, "%s: drew BG%d", name, o); return; }
  }
  if (opaque < 192 * 256 / 4) vct_fail(__FILE__, __LINE__, "%s: only %d opaque pixels", name, opaque);
}
VCT_TEST(p01_scene_4bpp) { emit_scene("p01_4bpp", 0, 0, 0x0100, 0, 0, 0, 0); }
VCT_TEST(p01_scene_4bpp_bg1_scroll) { emit_scene("p01_4bpp_bg1_scroll", 0, 1, 0x0904, 13, 77, 0, 0); }
VCT_TEST(p01_scene_8bpp_bg2) { emit_scene("p01_8bpp_bg2", 0, 2, 0x0A8C, 3, 5, 0, 0); }
VCT_TEST(p01_scene_size1) { emit_scene("p01_size1", 0, 0, 0x4508, 100, 9, 0, 0); }
VCT_TEST(p01_scene_size2) { emit_scene("p01_size2", 0, 3, 0x8B0C, 7, 300, 0, 0); }
VCT_TEST(p01_scene_size3_wrap) { emit_scene("p01_size3_wrap", 0, 0, 0xC784, 0x1F5, 0x1E3, 0, 0); }
VCT_TEST(p01_scene_size3_8bpp) { emit_scene("p01_size3_8bpp", 0, 1, 0xCD90, 250, 250, 0, 0); }
VCT_TEST(p01_scene_base_a) { emit_scene("p01_base_a", 0, 0, 0x0D08, 0, 0, (3u << 24) | (2u << 27), 0); }
VCT_TEST(p01_scene_base_a_high) { emit_scene("p01_base_a_high", 0, 2, 0x1D3C, 11, 3, (4u << 24) | (5u << 27), 0); }
VCT_TEST(p01_scene_engine_b) { emit_scene("p01_engine_b", 1, 0, 0x4308, 20, 30, 0, 0); }
VCT_TEST(p01_scene_engine_b_ignores_bases) { emit_scene("p01_engine_b_ignores_bases", 1, 1, 0x0A04, 1, 2, (3u << 24) | (3u << 27), 0); }
VCT_TEST(p01_scene_engine_b_8bpp) { emit_scene("p01_engine_b_8bpp", 1, 2, 0x0194, 0, 0, 0, 0); }
VCT_TEST(p01_scene_extpal_bg0) { emit_scene("p01_extpal_bg0", 0, 0, 0x0188, 0, 0, 1u << 30, 1); }
VCT_TEST(p01_scene_extpal_bg0_slot2) { emit_scene("p01_extpal_bg0_slot2", 0, 0, 0x2188, 5, 6, 1u << 30, 1); }
VCT_TEST(p01_scene_extpal_bg1_slot3) { emit_scene("p01_extpal_bg1_slot3", 0, 1, 0x6988, 0, 0, 1u << 30, 1); }
VCT_TEST(p01_scene_extpal_bg2) { emit_scene("p01_extpal_bg2", 0, 2, 0x0988, 0, 0, 1u << 30, 1); }
VCT_TEST(p01_scene_extpal_bg3) { emit_scene("p01_extpal_bg3", 0, 3, 0x4988, 0, 0, 1u << 30, 1); }
VCT_TEST(p01_scene_extpal_engine_b) { emit_scene("p01_extpal_engine_b", 1, 1, 0x2988, 0, 0, 1u << 30, 1); }
VCT_TEST(p01_scene_extpal_4bpp_ignored) { emit_scene("p01_extpal_4bpp_ignored", 0, 0, 0x0104, 0, 0, 1u << 30, 1); }
