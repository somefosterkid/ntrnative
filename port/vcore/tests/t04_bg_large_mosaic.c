/* port/vcore piece 4's unit tests: the large bitmap BG and BG mosaic (docs/vcore.md, piece 4). OWNED BY PIECE 4.
 * Expected values are worked by hand from GBATEK 25 (large bitmap), 08 (MOSAIC), 06 (affine parameters).
 * Run: build/vcore/vcore_tests p04 */
#include "vct.h"

#include <string.h>

#define BIG (512 * 1024)
static u8 g_big[BIG]; /* the engine A BG VRAM as a test sees it */

static VCoreState *large_state(u16 bg2cnt) {
  VCoreState *s = vct_state();
  memset(g_big, 0, sizeof g_big);
  vct_map_flat(s, VC_RGN_ABG, g_big, BIG);
  vct_io32(s, VC_REG_DISPCNT, 0x00010406); /* mode 6, BG2 on, graphics display */
  vct_eng16(s, VC_ENG_A, VC_REG_BG2CNT, bg2cnt);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PA, 0x100);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PD, 0x100);
  for (int i = 1; i < 256; i++) vct_pal(s, VC_ENG_A, 0, i, (u16)(i * 37 & 0x7FFF));
  return s;
}
#define PX(i) (VC_PX_OPAQUE | ((i) * 37 & 0x7FFF))

static VC_Line g_ln;

VCT_TEST(p04_large_512x1024_stride) {
  /* BGxCNT bits 14-15 = 0: 512 pixels a row, 1024 rows */
  VCoreState *s = large_state(0x0000);
  g_big[0] = 1;
  g_big[255] = 2;
  g_big[256] = 3;               /* x = 256 is off screen */
  g_big[512 * 1 + 0] = 4;       /* row 1 */
  g_big[512 * 7 + 9] = 5;       /* row 7, x 9 */
  memset(&g_ln, 0, sizeof g_ln);
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][0], PX(1));
  VCT_EQ(g_ln.bg[2][255], PX(2));
  VCT_EQ(g_ln.bg[2][1], 0);
  vc_bg_large_line(s, VC_ENG_A, 1, &g_ln);
  VCT_EQ(g_ln.bg[2][0], PX(4));
  vc_bg_large_line(s, VC_ENG_A, 7, &g_ln);
  VCT_EQ(g_ln.bg[2][9], PX(5));
  VCT_EQ(g_ln.bg[2][8], 0);
}

VCT_TEST(p04_large_1024x512_stride) {
  /* bit 14 = 1: 1024 pixels a row, 512 rows */
  VCoreState *s = large_state(0x4000);
  g_big[1024 * 3 + 10] = 6; /* row 3, x 10 */
  g_big[600] = 7;           /* row 0, x 600 */
  vc_bg_large_line(s, VC_ENG_A, 3, &g_ln);
  VCT_EQ(g_ln.bg[2][10], PX(6));
  /* reference point X = 600 (8.8): screen x 0 shows bitmap x 600 */
  vct_eng32(s, VC_ENG_A, VC_REG_BG2X, 600 << 8);
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][0], PX(7));
}

VCT_TEST(p04_large_last_row_and_outside) {
  /* 512x1024 without wraparound: row 1023 is the last; row 1024 is outside = transparent */
  VCoreState *s = large_state(0x0000);
  g_big[1023 * 512 + 0] = 9;
  vct_eng32(s, VC_ENG_A, VC_REG_BG2Y, 1023 << 8);
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][0], PX(9));
  vc_bg_large_line(s, VC_ENG_A, 1, &g_ln); /* ty = 1024 */
  for (int x = 0; x < VC_W; x++) VCT_EQ(g_ln.bg[2][x], 0);
  /* X = -1: bitmap x -1 is outside, x = 1 on screen shows bitmap x 0 */
  vct_eng32(s, VC_ENG_A, VC_REG_BG2Y, 0);
  vct_eng32(s, VC_ENG_A, VC_REG_BG2X, 0x0FFFFF00); /* -1.0 in 28-bit 8.8 */
  g_big[0] = 8;
  g_big[511] = 3;
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][0], 0);
  VCT_EQ(g_ln.bg[2][1], PX(8));
}

VCT_TEST(p04_large_wraparound) {
  /* BGxCNT bit 13: -1 wraps to 511 (512 wide) and 1023 (rows) */
  VCoreState *s = large_state(0x2000);
  g_big[511] = 3;
  g_big[1023 * 512 + 511] = 4;
  g_big[0] = 8;
  vct_eng32(s, VC_ENG_A, VC_REG_BG2X, 0x0FFFFF00);
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][0], PX(3)); /* x -1 -> 511, row 0 */
  VCT_EQ(g_ln.bg[2][1], PX(8));
  vct_eng32(s, VC_ENG_A, VC_REG_BG2Y, 0x0FFFFF00); /* Y = -1 -> row 1023 */
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][0], PX(4));
  /* 1024x512 wraps columns at 1024 and rows at 512 */
  s = large_state(0x6000);
  g_big[1023] = 5;
  g_big[511 * 1024] = 6;
  vct_eng32(s, VC_ENG_A, VC_REG_BG2X, 0x0FFFFF00);
  vct_eng32(s, VC_ENG_A, VC_REG_BG2Y, 0x0FFFFF00);
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][1], PX(6)); /* x 0, row 511 */
  vct_eng32(s, VC_ENG_A, VC_REG_BG2Y, 0);
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][0], PX(5)); /* x 1023, row 0 */
}

VCT_TEST(p04_large_scale_and_rotate) {
  VCoreState *s = large_state(0x0000);
  for (int i = 0; i < 40; i++) g_big[i] = (u8)(i + 1);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PA, 0x200); /* 2x step: screen x -> bitmap 2x */
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][3], PX(7)); /* bitmap x 6 = index 7 */
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PA, 0x080); /* 0.5: x 5 -> 2.5 -> 2 */
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][5], PX(3));
  /* 90 degree rotation: PA = 0, PB = 1.0 (per line), PC = 1.0 (per pixel), PD = 0: screen (x, y) = bitmap (y, x) */
  g_big[512 * 5 + 2] = 77; /* bitmap (2, 5) */
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PA, 0);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PB, 0x100);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PC, 0x100);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PD, 0);
  vc_bg_large_line(s, VC_ENG_A, 2, &g_ln); /* line 2: bitmap x = 2, bitmap y = x */
  VCT_EQ(g_ln.bg[2][5], PX(77));
}

VCT_TEST(p04_large_transparent_index_and_engine_b) {
  VCoreState *s = large_state(0x0000);
  g_big[4] = 0; /* index 0: transparent whatever palette entry 0 holds */
  vct_pal(s, VC_ENG_A, 0, 0, 0x7FFF);
  g_big[5] = 200;
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][4], 0);
  VCT_EQ(g_ln.bg[2][5], PX(200));
  /* bit 15 of a palette entry is not part of the colour */
  vct_pal(s, VC_ENG_A, 0, 200, 0xFFFF);
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][5], VC_PX_OPAQUE | 0x7FFF);
  /* engine B has no large bitmap */
  memset(&g_ln, 0, sizeof g_ln);
  vc_bg_large_line(s, VC_ENG_B, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][5], 0);
}

VCT_TEST(p04_large_ignores_screen_base) {
  /* GBATEK 25: the screen base is NOT used for the large bitmap */
  VCoreState *s = large_state(0x0000 | (5 << 8));
  g_big[0] = 1;
  vc_bg_large_line(s, VC_ENG_A, 0, &g_ln);
  VCT_EQ(g_ln.bg[2][0], PX(1));
}

/* ---- mosaic ---- */
VCT_TEST(p04_mosaic_y_blocks) {
  VCoreState *s = vct_state();
  vct_eng16(s, VC_ENG_A, VC_REG_BG2CNT, 0x0040);
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x0030); /* V size 3 + 1 = 4 */
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 0), 0);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 3), 0);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 4), 4);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 7), 4);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 191), 188);
  /* size 16 (nibble 15) */
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x00F0);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 35), 32);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 47), 32);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 48), 48);
  /* size 1 (nibble 0): no change; the H and OBJ sizes are not V */
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0xFF0F);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 35), 35);
}

VCT_TEST(p04_mosaic_off_without_bgcnt_bit6) {
  VCoreState *s = vct_state();
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x00FF);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2CNT, 0x0040);
  /* BG1 and BG3 have bit 6 clear: their lines are untouched */
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 1, 35), 35);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 3, 35), 35);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 35), 32);
  u32 px[VC_W];
  for (int x = 0; x < VC_W; x++) px[x] = VC_PX_OPAQUE | (u32)x;
  vc_mosaic_bg_line(s, VC_ENG_A, 1, px);
  for (int x = 0; x < VC_W; x++) VCT_EQ(px[x], VC_PX_OPAQUE | (u32)x);
  /* all-zero MOSAIC register (sizes 1x1) leaves a flagged BG alone */
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0);
  vc_mosaic_bg_line(s, VC_ENG_A, 2, px);
  for (int x = 0; x < VC_W; x++) VCT_EQ(px[x], VC_PX_OPAQUE | (u32)x);
}

VCT_TEST(p04_mosaic_h_repeats_first_pixel) {
  VCoreState *s = vct_state();
  vct_eng16(s, VC_ENG_A, VC_REG_BG0CNT, 0x0040);
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x0002); /* H size 2 + 1 = 3 */
  u32 px[VC_W];
  for (int x = 0; x < VC_W; x++) px[x] = VC_PX_OPAQUE | (u32)(x + 1);
  vc_mosaic_bg_line(s, VC_ENG_A, 0, px);
  VCT_EQ(px[0], VC_PX_OPAQUE | 1);
  VCT_EQ(px[1], VC_PX_OPAQUE | 1);
  VCT_EQ(px[2], VC_PX_OPAQUE | 1);
  VCT_EQ(px[3], VC_PX_OPAQUE | 4);
  VCT_EQ(px[5], VC_PX_OPAQUE | 4);
  VCT_EQ(px[254], VC_PX_OPAQUE | 253); /* 254 - 254 % 3 = 252 -> value 253 */
  VCT_EQ(px[255], VC_PX_OPAQUE | 256); /* 255 % 3 == 0: its own block */
  /* GBATEK example: nibble 5 = 6 wide: pixels 0-5 as 0, 6-11 as 6, 12-17 as 12 */
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x0005);
  for (int x = 0; x < VC_W; x++) px[x] = VC_PX_OPAQUE | (u32)(x + 1);
  vc_mosaic_bg_line(s, VC_ENG_A, 0, px);
  VCT_EQ(px[5], VC_PX_OPAQUE | 1);
  VCT_EQ(px[6], VC_PX_OPAQUE | 7);
  VCT_EQ(px[11], VC_PX_OPAQUE | 7);
  VCT_EQ(px[12], VC_PX_OPAQUE | 13);
  VCT_EQ(px[17], VC_PX_OPAQUE | 13);
  /* size 16 */
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x000F);
  for (int x = 0; x < VC_W; x++) px[x] = VC_PX_OPAQUE | (u32)(x + 1);
  vc_mosaic_bg_line(s, VC_ENG_A, 0, px);
  VCT_EQ(px[15], VC_PX_OPAQUE | 1);
  VCT_EQ(px[16], VC_PX_OPAQUE | 17);
  VCT_EQ(px[255], VC_PX_OPAQUE | 241);
}

VCT_TEST(p04_mosaic_transparent_first_pixel) {
  VCoreState *s = vct_state();
  vct_eng16(s, VC_ENG_A, VC_REG_BG1CNT, 0x0040);
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x0003); /* 4 wide */
  u32 px[VC_W];
  for (int x = 0; x < VC_W; x++) px[x] = VC_PX_OPAQUE | (u32)(x + 1);
  px[0] = 0;                     /* block 0: first pixel transparent: the whole block is */
  px[8] = VC_PX_OPAQUE | 0x123;  /* block 2 starts opaque */
  px[9] = 0;
  px[12] = 0;                    /* block 3 transparent start, rest opaque */
  vc_mosaic_bg_line(s, VC_ENG_A, 1, px);
  for (int x = 0; x < 4; x++) VCT_EQ(px[x], 0);
  for (int x = 8; x < 12; x++) VCT_EQ(px[x], VC_PX_OPAQUE | 0x123);
  for (int x = 12; x < 16; x++) VCT_EQ(px[x], 0);
  VCT_EQ(px[4], VC_PX_OPAQUE | 5);
  VCT_EQ(px[7], VC_PX_OPAQUE | 5);
}

VCT_TEST(p04_mosaic_each_bg_and_engine_b) {
  /* each of BG0-3 reads its own BGxCNT bit 6; text and affine BGs are treated alike; engine B uses its registers */
  VCoreState *s = vct_state();
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x0001);
  vct_eng16(s, VC_ENG_B, VC_REG_MOSAIC, 0x0003);
  vct_eng16(s, VC_ENG_A, VC_REG_BG0CNT, 0x0040);
  vct_eng16(s, VC_ENG_A, VC_REG_BG3CNT, 0x0040);
  vct_eng16(s, VC_ENG_B, VC_REG_BG2CNT, 0x0040);
  for (int n = 0; n < 4; n++) {
    u32 px[VC_W];
    for (int x = 0; x < VC_W; x++) px[x] = VC_PX_OPAQUE | (u32)(x + 1);
    vc_mosaic_bg_line(s, VC_ENG_A, n, px);
    int on = (n == 0 || n == 3);
    VCT_EQ(px[1], on ? (VC_PX_OPAQUE | 1) : (VC_PX_OPAQUE | 2));
    VCT_EQ(px[2], VC_PX_OPAQUE | 3);
    VCT_EQ(px[3], on ? (VC_PX_OPAQUE | 3) : (VC_PX_OPAQUE | 4));
  }
  u32 px[VC_W];
  for (int x = 0; x < VC_W; x++) px[x] = VC_PX_OPAQUE | (u32)(x + 1);
  vc_mosaic_bg_line(s, VC_ENG_B, 2, px);
  VCT_EQ(px[3], VC_PX_OPAQUE | 1);
  VCT_EQ(px[4], VC_PX_OPAQUE | 5);
  vct_eng16(s, VC_ENG_B, VC_REG_MOSAIC, 0x0030);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_B, 2, 7), 4);
  VCT_EQ(vc_mosaic_bg_y(s, VC_ENG_A, 2, 7), 7); /* A BG2 has no bit 6 */
}

/* ---- synthetic scenes for the oracle (VCT_EMIT=DIR) ---- */
static void fill_pattern(int w, int h) {
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
      int v = (x >> 3) * 7 + (y >> 3) * 13 + ((x ^ y) & 3);
      g_big[y * w + x] = (u8)(v % 255 + 1);
      if ((x + y) % 61 == 0) g_big[y * w + x] = 0; /* sprinkled transparent pixels */
    }
}
static void emit_scene(VCoreState *s, const char *name) {
  memcpy(vc_bank(s, VC_BANK_A), g_big, 0x20000);
  memcpy(vc_bank(s, VC_BANK_B), g_big + 0x20000, 0x20000);
  memcpy(vc_bank(s, VC_BANK_C), g_big + 0x40000, 0x20000);
  memcpy(vc_bank(s, VC_BANK_D), g_big + 0x60000, 0x20000);
  vct_io8(s, VC_REG_VRAMCNT_A + 0, 0x81); /* A: BG at 0 */
  vct_io8(s, VC_REG_VRAMCNT_A + 1, 0x89); /* B: BG at 128 KB */
  vct_io8(s, VC_REG_VRAMCNT_A + 2, 0x91); /* C: 256 KB */
  vct_io8(s, VC_REG_VRAMCNT_A + 3, 0x99); /* D: 384 KB */
  for (int i = 0; i < 256; i++) vct_pal(s, VC_ENG_A, 0, i, (u16)((i * 2654435761u >> 12) & 0x7FFF));
  vct_emit(s, name);
}

VCT_TEST(p04_scenes) {
  VCoreState *s = large_state(0x0000);
  fill_pattern(512, 1024);
  vct_eng32(s, VC_ENG_A, VC_REG_BG2Y, 300 << 8);
  emit_scene(s, "p04_large_512x1024");
  s = large_state(0x4000);
  fill_pattern(1024, 512);
  vct_eng32(s, VC_ENG_A, VC_REG_BG2X, 700 << 8);
  emit_scene(s, "p04_large_1024x512");
  s = large_state(0x2000); /* wraparound with X, Y negative */
  fill_pattern(512, 1024);
  vct_eng32(s, VC_ENG_A, VC_REG_BG2X, 0x0FFFFE00);
  vct_eng32(s, VC_ENG_A, VC_REG_BG2Y, 0x0FFFFC00);
  emit_scene(s, "p04_large_wrap");
  s = large_state(0x0000); /* without wrap, negative reference: transparent border */
  fill_pattern(512, 1024);
  vct_eng32(s, VC_ENG_A, VC_REG_BG2X, 0x0FFFFB00);
  vct_eng32(s, VC_ENG_A, VC_REG_BG2Y, 0x0FFFFD00);
  emit_scene(s, "p04_large_outside");
  s = large_state(0x0000); /* scaled 1.5x and sheared */
  fill_pattern(512, 1024);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PA, 0x180);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PB, 0x040);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PC, 0xFFC0);
  vct_eng16(s, VC_ENG_A, VC_REG_BG2PD, 0x0C0);
  emit_scene(s, "p04_large_affine");
  s = large_state(0x0040); /* mosaic 6 x 4 on the large bitmap */
  fill_pattern(512, 1024);
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x0035);
  emit_scene(s, "p04_large_mosaic_6x4");
  s = large_state(0x0040); /* mosaic 16 x 16 */
  fill_pattern(512, 1024);
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x00FF);
  emit_scene(s, "p04_large_mosaic_16x16");
  s = large_state(0x0000); /* mosaic registers set but BG bit 6 clear: no mosaic */
  fill_pattern(512, 1024);
  vct_eng16(s, VC_ENG_A, VC_REG_MOSAIC, 0x00FF);
  emit_scene(s, "p04_large_mosaic_off");
}
