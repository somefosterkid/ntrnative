/* port/vcore piece 3's unit tests: extended BGs (affine 16-bit map, 256-colour and direct-colour bitmaps)
 * (docs/vcore.md, piece 3). OWNED BY PIECE 3. Expected values are worked by hand from GBATEK (DS Video BG Modes /
 * Control, LCD I/O BG Rotation/Scaling, BG Screen Data Format, DS Video Extended Palettes).
 * Run: build/vcore/vcore_tests p03 */
#include "vct.h"

#include <string.h>

static u8 vram[0x80000]; /* engine BG VRAM (512 KB) */
static u8 xpal[0x8000];  /* BG extended palettes, slots 0-3 */

#define PX(c) (VC_PX_OPAQUE | (c))
#define PAL(i) ((i) * 3 + 1) /* the test palette: colour i = 3i+1 */

/* a state with BG n of engine e in the extended mode: cnt, identity matrix, VRAM and ext palettes mapped */
static VCoreState *setup(int e, int n, u16 cnt, u32 dispcnt) {
  VCoreState *s = vct_state();
  memset(vram, 0, sizeof vram);
  memset(xpal, 0, sizeof xpal);
  vct_map_flat(s, e ? VC_RGN_BBG : VC_RGN_ABG, vram, e ? 0x20000 : 0x80000);
  vct_map_flat(s, e ? VC_RGN_BBGEXT : VC_RGN_ABGEXT, xpal, 0x8000);
  vct_eng32(s, e, VC_REG_DISPCNT, dispcnt);
  vct_eng16(s, e, VC_REG_BG0CNT + 2 * n, cnt);
  u32 pa = VC_REG_BG2PA + 0x10 * (n - 2);
  vct_eng16(s, e, pa, 0x100);
  vct_eng16(s, e, pa + 6, 0x100);
  for (int i = 1; i < 256; i++) vct_pal(s, e, 0, i, (u16)PAL(i)); /* distinct colours, bit 15 clear */
  return s;
}
static void ref(VCoreState *s, int e, int n, s32 x, s32 y) {
  u32 r = VC_REG_BG2X + 0x10 * (n - 2);
  vct_eng32(s, e, r, (u32)x & 0x0FFFFFFF);
  vct_eng32(s, e, r + 4, (u32)y & 0x0FFFFFFF);
}
static void put16(u32 a, u16 v) {
  vram[a] = (u8)v;
  vram[a + 1] = (u8)(v >> 8);
}
static VC_Line line;
static u32 *run(VCoreState *s, int e, int n, int y) {
  memset(&line, 0xEE, sizeof line); /* the renderer must write every pixel */
  vc_bg_ext_line(s, e, n, y, &line);
  return line.bg[n];
}

/* ---- the 16-bit map kind ---- */
VCT_TEST(p03_map16_basic) {
  /* BGxCNT 0x0104: bit7=0 map kind, char base 1 (bits 2-5) = 0x4000, screen base 1 = 0x800, size 0 = 128x128 */
  VCoreState *s = setup(0, 2, 0x0104, 0);
  put16(0x800, 1);                                                      /* map (0,0) = tile 1 */
  for (int fx = 0; fx < 8; fx++) vram[0x4000 + 64 + fx] = (u8)(1 + fx); /* tile 1 row 0 */
  u32 *p = run(s, 0, 2, 0);
  for (int x = 0; x < 8; x++) VCT_EQ(p[x], PX(PAL(1 + x)));
  VCT_EQ(p[8], 0);   /* map (1,0) = tile 0, all colour 0 = transparent */
  VCT_EQ(p[200], 0); /* beyond the 128 pixels: transparent (wrap off) */
  p = run(s, 0, 2, 1);
  VCT_EQ(p[0], 0); /* row 1 of tile 1 is empty */
}

VCT_TEST(p03_map16_flips) {
  VCoreState *s = setup(0, 2, 0x0104, 0);
  put16(0x800, 1 | 0x400); /* H flip */
  put16(0x802, 1 | 0x800); /* V flip */
  put16(0x804, 1 | 0xC00); /* both */
  for (int i = 0; i < 64; i++) vram[0x4000 + 64 + i] = (u8)(i + 1); /* tile 1: pixel (fx,fy) = fy*8+fx+1 */
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(0 * 8 + 7 + 1))); /* H flip: x0 shows fx 7, fy 0 */
  VCT_EQ(p[7], PX(PAL(0 * 8 + 0 + 1)));
  VCT_EQ(p[8], PX(PAL(7 * 8 + 0 + 1)));  /* V flip, line 0 shows fy 7 */
  VCT_EQ(p[15], PX(PAL(7 * 8 + 7 + 1)));
  VCT_EQ(p[16], PX(PAL(7 * 8 + 7 + 1))); /* both: fx 7, fy 7 */
  VCT_EQ(p[23], PX(PAL(7 * 8 + 0 + 1)));
  p = run(s, 0, 2, 7);
  VCT_EQ(p[0], PX(PAL(7 * 8 + 7 + 1))); /* H flip, line 7: fy 7 */
  VCT_EQ(p[8], PX(PAL(0 * 8 + 0 + 1))); /* V flip, line 7: fy 0 */
}

VCT_TEST(p03_map16_tile_number_10_bits) {
  VCoreState *s = setup(0, 3, 0x0104, 0);
  put16(0x800, 0x3FF);
  vram[0x4000 + 0x3FF * 64] = 9;
  u32 *p = run(s, 0, 3, 0);
  VCT_EQ(p[0], PX(PAL(9)));
  /* the palette number bits (12-15) do not select a tile and are ignored without extended palettes */
  put16(0x800, 0x3FF | 0xF000);
  p = run(s, 0, 3, 0);
  VCT_EQ(p[0], PX(PAL(9)));
}

VCT_TEST(p03_map16_extpal_slots) {
  /* DISPCNT bit 30: colours from the extended palette, slot n of BGn, palette number of the entry */
  VCoreState *s = setup(0, 2, 0x0104, 1u << 30);
  put16(0x800, 1 | 3 << 12);
  vram[0x4000 + 64] = 5;
  xpal[2 * 0x2000 + 3 * 512 + 5 * 2] = 0x34;
  xpal[2 * 0x2000 + 3 * 512 + 5 * 2 + 1] = 0x12;
  xpal[3 * 0x2000 + 3 * 512 + 5 * 2] = 0x78; /* slot 3 is BG3's, not read */
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(0x1234));
  /* BG3 reads slot 3 */
  s = setup(0, 3, 0x0104, 1u << 30);
  put16(0x800, 1 | 3 << 12);
  vram[0x4000 + 64] = 5;
  xpal[2 * 0x2000 + 3 * 512 + 5 * 2] = 0x34;
  xpal[3 * 0x2000 + 3 * 512 + 5 * 2] = 0x78;
  xpal[3 * 0x2000 + 3 * 512 + 5 * 2 + 1] = 0x23;
  p = run(s, 0, 3, 0);
  VCT_EQ(p[0], PX(0x2378));
}

VCT_TEST(p03_map16_extpal_colour0_transparent) {
  VCoreState *s = setup(0, 2, 0x0104, 1u << 30);
  put16(0x800, 1);
  xpal[2 * 0x2000 + 0] = 0xFF; /* the extended palette's colour 0 is never drawn: tile pixel 0 is transparent */
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], 0);
}

VCT_TEST(p03_map16_palette_number_ignored_without_ext) {
  VCoreState *s = setup(0, 2, 0x0104, 0);
  put16(0x800, 1 | 7 << 12);
  vram[0x4000 + 64] = 2;
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(2))); /* the standard 256-colour palette */
}

VCT_TEST(p03_map16_bases_engine_a_dispcnt) {
  /* engine A: char base = BGxCNT*16K + DISPCNT[24-26]*64K, screen base = BGxCNT*2K + DISPCNT[27-29]*64K */
  VCoreState *s = setup(0, 2, 0x0104, (1u << 24) | (2u << 27));
  put16(0x20000 + 0x800, 1);
  vram[0x10000 + 0x4000 + 64] = 4;
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(4)));
}

VCT_TEST(p03_map16_bases_engine_b_no_dispcnt) {
  /* engine B has no DISPCNT base bits: those bits are ignored */
  VCoreState *s = setup(1, 2, 0x0104, (1u << 24) | (2u << 27));
  put16(0x800, 1);
  vram[0x4000 + 64] = 4;
  u32 *p = run(s, 1, 2, 0);
  VCT_EQ(p[0], PX(PAL(4)));
}

VCT_TEST(p03_map16_sizes) {
  /* size 3: 1024x1024 = 128x128 tiles, entry (tx,ty) at screen base + (ty*128 + tx)*2; read tile (100, 90) */
  VCoreState *s = setup(0, 2, 0x0104 | 3 << 14, 0);
  put16(0x800 + (90 * 128 + 100) * 2, 1);
  vram[0x4000 + 64 + 3 * 8 + 5] = 6; /* tile 1 pixel (5, 3) */
  ref(s, 0, 2, 805 << 8, 723 << 8);  /* texel (805, 723) = tile (100, 90) pixel (5, 3) */
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(6)));
  /* size 2: 512x512 = 64 tiles per row */
  s = setup(0, 2, 0x0104 | 2 << 14, 0);
  put16(0x800 + (10 * 64 + 20) * 2, 1);
  vram[0x4000 + 64 + 1 * 8 + 2] = 7;
  ref(s, 0, 2, 162 << 8, 81 << 8); /* tile (20, 10) pixel (2, 1) */
  p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(7)));
  /* size 1: 256x256 = 32 tiles per row; size 0 128x128 = 16 */
  s = setup(0, 2, 0x0104 | 1 << 14, 0);
  put16(0x800 + (5 * 32 + 31) * 2, 1);
  vram[0x4000 + 64] = 8;
  ref(s, 0, 2, 248 << 8, 40 << 8);
  p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(8)));
  s = setup(0, 2, 0x0104, 0);
  put16(0x800 + (5 * 16 + 15) * 2, 1);
  vram[0x4000 + 64] = 8;
  ref(s, 0, 2, 120 << 8, 40 << 8);
  p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(8)));
  VCT_EQ(p[8], 0); /* texel x 128 is outside */
}

VCT_TEST(p03_map16_wrap) {
  /* BGxCNT bit 13 = 1: the texel position wraps at the map size */
  VCoreState *s = setup(0, 2, 0x0104 | 1 << 13, 0);
  put16(0x800 + (15 * 16 + 15) * 2, 1); /* tile (15,15) = the last of a 128x128 map */
  vram[0x4000 + 64 + 63] = 3;           /* its pixel (7,7) */
  ref(s, 0, 2, -1 * 256, -1 * 256);     /* texel (-1,-1) wraps to (127,127) */
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(3)));
  s = setup(0, 2, 0x0104, 0);           /* without the bit: transparent */
  put16(0x800 + (15 * 16 + 15) * 2, 1);
  vram[0x4000 + 64 + 63] = 3;
  ref(s, 0, 2, -1 * 256, -1 * 256);
  p = run(s, 0, 2, 0);
  VCT_EQ(p[0], 0);
}

/* ---- the 256-colour bitmap ---- */
VCT_TEST(p03_bmp256_basic_and_base) {
  /* bit 7 = 1, bit 2 = 0: 256-colour bitmap; screen base 2 * 16 KB = 0x8000; size 1 = 256x256 */
  VCoreState *s = setup(0, 2, 0x0080 | 2 << 8 | 1 << 14, 0);
  vram[0x8000 + 5 * 256 + 3] = 10;
  vram[0x8000 + 5 * 256 + 4] = 0;
  u32 *p = run(s, 0, 2, 5);
  VCT_EQ(p[3], PX(PAL(10)));
  VCT_EQ(p[4], 0); /* index 0 transparent */
  VCT_EQ(p[2], 0);
  /* DISPCNT's screen base bits do not move a bitmap */
  s = setup(0, 2, 0x0080 | 2 << 8 | 1 << 14, 3u << 27);
  vram[0x8000] = 11;
  p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(11)));
}

VCT_TEST(p03_bmp256_sizes) {
  /* 128x128, 256x256, 512x256, 512x512 by bits 14-15: the pitch is the width; the last texel of each */
  static const int w[4] = {128, 256, 512, 512}, h[4] = {128, 256, 256, 512};
  for (int z = 0; z < 4; z++) {
    VCoreState *s = setup(0, 3, 0x0080 | z << 14, 0);
    int tx = w[z] - 1, ty = h[z] - 1;
    vram[(u32)ty * w[z] + tx] = (u8)(20 + z);
    ref(s, 0, 3, tx << 8, ty << 8);
    u32 *p = run(s, 0, 3, 0);
    VCT_EQ(p[0], PX(PAL(20 + z)));
    VCT_EQ(p[1], 0); /* x = w: outside, wrap off */
  }
}

VCT_TEST(p03_bmp256_wrap_and_outside) {
  /* wrap bit 13 = 0: outside transparent; = 1: texel position taken modulo the bitmap size */
  VCoreState *s = setup(0, 2, 0x0080 | 1 << 14, 0);
  vram[0] = 12;
  ref(s, 0, 2, -1 * 256, 0); /* x0 = -1: transparent, x1 = 0 */
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], 0);
  VCT_EQ(p[1], PX(PAL(12)));
  s = setup(0, 2, 0x0080 | 1 << 14 | 1 << 13, 0);
  vram[0] = 12;
  vram[255 * 256 + 255] = 13;
  ref(s, 0, 2, -1 * 256, -1 * 256);
  p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(13))); /* (-1,-1) wraps to (255,255) */
  p = run(s, 0, 2, 1);
  VCT_EQ(p[1], PX(PAL(12))); /* y = 1 -> texel y 0 */
}

/* ---- the direct-colour bitmap ---- */
VCT_TEST(p03_direct_basic) {
  /* bit 7 = 1, bit 2 = 1: BGR555 halfwords, bit 15 = opaque; size 1 256x256, base 1 = 0x4000 */
  VCoreState *s = setup(0, 2, 0x0084 | 1 << 8 | 1 << 14, 0);
  put16(0x4000 + (2 * 256 + 7) * 2, 0x8000 | 0x1234);
  put16(0x4000 + (2 * 256 + 8) * 2, 0x1234); /* bit 15 clear: transparent */
  put16(0x4000 + (2 * 256 + 9) * 2, 0x8000); /* opaque black */
  u32 *p = run(s, 0, 2, 2);
  VCT_EQ(p[7], PX(0x1234));
  VCT_EQ(p[8], 0);
  VCT_EQ(p[9], PX(0));
  VCT_EQ(p[10], 0);
}

VCT_TEST(p03_direct_sizes_pitch) {
  static const int w[4] = {128, 256, 512, 512}, h[4] = {128, 256, 256, 512};
  for (int z = 0; z < 4; z++) {
    VCoreState *s = setup(0, 2, 0x0084 | z << 14, 0);
    int tx = w[z] - 1, ty = h[z] - 1;
    put16(((u32)ty * w[z] + tx) * 2, 0x8000 | (u16)(0x100 + z));
    ref(s, 0, 2, tx << 8, ty << 8);
    u32 *p = run(s, 0, 2, 0);
    VCT_EQ(p[0], PX(0x100 + z));
    VCT_EQ(p[1], 0);
  }
}

VCT_TEST(p03_direct_wrap) {
  VCoreState *s = setup(0, 3, 0x0084 | 1 << 13, 0);
  put16(0, 0x8000 | 0x7C00);
  ref(s, 0, 3, 128 << 8, 128 << 8); /* texel (128,128) wraps to (0,0) in a 128x128 bitmap */
  u32 *p = run(s, 0, 3, 0);
  VCT_EQ(p[0], PX(0x7C00));
  VCT_EQ(p[1], 0); /* texel (129,128) wraps to (1,0): empty */
}

VCT_TEST(p03_direct_engine_b) {
  VCoreState *s = setup(1, 2, 0x0084 | 1 << 14, 0);
  put16(0, 0x8000 | 0x03E0);
  u32 *p = run(s, 1, 2, 0);
  VCT_EQ(p[0], PX(0x03E0));
}

/* ---- the affine transform ---- */
VCT_TEST(p03_scale_rotate_reference) {
  VCoreState *s = setup(0, 2, 0x0080 | 1 << 14, 0);
  for (int i = 0; i < 256; i++) vram[3 * 256 + i] = (u8)(i ? i : 255); /* row 3: index = x */
  for (int i = 0; i < 256; i++) vram[i * 256 + 5] = (u8)(i ? i : 255); /* column 5: index = y */
  /* PA = 0x80: every second screen pixel steps one texel */
  vct_eng16(s, 0, VC_REG_BG2PA, 0x80);
  vct_eng16(s, 0, VC_REG_BG2PB, 0);
  vct_eng16(s, 0, VC_REG_BG2PD, 0x100);
  ref(s, 0, 2, 0, 3 << 8);
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(255))); /* texel 0 (index 255 by construction) */
  VCT_EQ(p[1], PX(PAL(255))); /* 0.5 -> texel 0 */
  VCT_EQ(p[2], PX(PAL(1)));
  VCT_EQ(p[7], PX(PAL(3)));
  /* PA = 0x200: two texels per pixel */
  vct_eng16(s, 0, VC_REG_BG2PA, 0x200);
  p = run(s, 0, 2, 0);
  VCT_EQ(p[3], PX(PAL(6)));
  /* rotation by 90 degrees: PA = 0, PB = -1, PC = 1, PD = 0: texel (X - y, Y + x) */
  vct_eng16(s, 0, VC_REG_BG2PA, 0);
  vct_eng16(s, 0, VC_REG_BG2PB, (u16)-0x100);
  vct_eng16(s, 0, VC_REG_BG2PC, 0x100);
  vct_eng16(s, 0, VC_REG_BG2PD, 0);
  ref(s, 0, 2, 5 << 8, 0);
  p = run(s, 0, 2, 0); /* y = 0: column 5, texel y = x */
  VCT_EQ(p[9], PX(PAL(9)));
  p = run(s, 0, 2, 2); /* y = 2: X = 5 - 2 = 3, texel (3, x): only row 3 is filled there, with index = 3 */
  VCT_EQ(p[3], PX(PAL(3)));
  VCT_EQ(p[4], 0);
}

VCT_TEST(p03_reference_is_signed_28_bit) {
  VCoreState *s = setup(0, 2, 0x0080 | 1 << 14 | 1 << 13, 0);
  vram[255 * 256 + 255] = 30;
  /* BGxX = 0x0FFFFF00 is -1.0 as a 28-bit signed number: texel -1 wraps to 255 */
  vct_eng32(s, 0, VC_REG_BG2X, 0x0FFFFF00);
  vct_eng32(s, 0, VC_REG_BG2Y, 0xFFFFFF00); /* the top 4 bits are not used */
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(30)));
}

VCT_TEST(p03_line_start_uses_pb_pd) {
  /* line y starts at X + y*PB, Y + y*PD */
  VCoreState *s = setup(0, 2, 0x0084 | 1 << 14, 0);
  put16((10 * 256 + 4) * 2, 0x8000 | 0x1111);
  vct_eng16(s, 0, VC_REG_BG2PB, 0x80); /* half a texel per line in x */
  vct_eng16(s, 0, VC_REG_BG2PD, 0x100);
  ref(s, 0, 2, 0, 0);
  u32 *p = run(s, 0, 2, 10); /* X = 5 texels, Y = 10: pixel 0 is texel (5,10): empty */
  VCT_EQ(p[0], 0);
  ref(s, 0, 2, -1 * 256, 0);
  p = run(s, 0, 2, 10); /* X = -1 + 5 = 4: texel (4,10) at pixel 0 */
  VCT_EQ(p[0], PX(0x1111));
}

/* ---- the three kinds are told apart by BGxCNT bits 7 and 2 ---- */
VCT_TEST(p03_kind_selection) {
  /* bit 2 is char base bit 0 in the map kind (char base 1 = 0x4000 here), and the direct-colour flag in a bitmap */
  VCoreState *s = setup(0, 2, 0x0104, 0);
  put16(0x800, 1);
  vram[0x4000 + 64] = 2;
  u32 *p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(2)));
  s = setup(0, 2, 0x0080, 0); /* bit 2 clear, bit 7 set: 256-colour bitmap (byte pixels) */
  vram[0] = 2;
  vram[1] = 0;
  put16(2, 0x8000 | 0x2222);
  p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(PAL(2)));
  s = setup(0, 2, 0x0084, 0); /* bits 2 and 7: direct colour */
  put16(0, 0x8000 | 0x2222);
  p = run(s, 0, 2, 0);
  VCT_EQ(p[0], PX(0x2222));
}

/* ---- synthetic scenes for the oracle (VCT_EMIT=DIR): mode 5, only BG2 on, graphics display ---- */
static void emit_scene(const char *name, u16 cnt, u32 extra_dispcnt, int kind, s32 rx, s32 ry, u16 pa, u16 pb, u16 pc,
                       u16 pd) {
  VCoreState *s = vct_state();
  s->io9[VC_REG_VRAMCNT_A] = 0x81;     /* bank A -> engine A BG 0x06000000 */
  s->io9[VC_REG_VRAMCNT_A + 1] = 0x89; /* bank B -> engine A BG 0x06020000 */
  s->io9[VC_REG_VRAMCNT_A + 4] = 0x84; /* bank E -> engine A BG extended palettes */
  vct_io16(s, VC_REG_POWCNT1, 0x820F);
  for (int i = 1; i < 256; i++) vct_pal(s, 0, 0, i, (u16)((i * 37) & 0x7FFF));
  vct_pal(s, 0, 0, 0, 0x2108);
  u32 sbase = ((cnt >> 8) & 31) * (kind == 0 ? 0x800u : 0x4000u);
  u32 cbase = ((cnt >> 2) & 15) * 0x4000u;
  u8 *vr = vc_bank(s, VC_BANK_A); /* banks A and B are contiguous in s->vram: 256 KB */
  u8 *e5 = vc_bank(s, VC_BANK_E);
  if (kind == 0) {
    for (u32 t = 0; t < 128 * 128 && sbase + t * 2 + 1 < 0x20000; t++) {
      u16 ent = (u16)((t * 7 + (t >> 5)) & 0x3FF) | (u16)(((t * 5) & 3) << 10) | (u16)(((t >> 3) & 15) << 12);
      vr[sbase + t * 2] = (u8)ent;
      vr[sbase + t * 2 + 1] = (u8)(ent >> 8);
    }
    for (u32 i = cbase; i < 0x20000; i++) vr[i] = (u8)((i * 13 + (i >> 6)) ^ (i >> 3));
    for (u32 i = 0; i < 0x8000; i++) e5[i] = (u8)(i * 5 + (i >> 4));
  } else {
    for (u32 i = sbase; i < 0x20000; i++) vr[i] = (u8)((i * 29) ^ (i >> 7));
    if (kind == 2)
      for (u32 i = sbase + 1; i < 0x20000; i += 2) vr[i] |= (u8)((i >> 3) & 1) << 7; /* some pixels transparent */
  }
  vct_eng16(s, 0, VC_REG_BG2CNT, cnt);
  vct_eng16(s, 0, VC_REG_BG2PA, pa);
  vct_eng16(s, 0, VC_REG_BG2PB, pb);
  vct_eng16(s, 0, VC_REG_BG2PC, pc);
  vct_eng16(s, 0, VC_REG_BG2PD, pd);
  vct_eng32(s, 0, VC_REG_BG2X, (u32)rx & 0x0FFFFFFF);
  vct_eng32(s, 0, VC_REG_BG2Y, (u32)ry & 0x0FFFFFFF);
  vct_eng32(s, 0, VC_REG_DISPCNT, 0x10000 | 5 | (1u << 10) | extra_dispcnt);
  vct_emit(s, name);
}

VCT_TEST(p03_emit_scenes) {
  emit_scene("p03_map16_identity", 0x0104, 0, 0, 0, 0, 0x100, 0, 0, 0x100);
  emit_scene("p03_map16_size3_wrap", 0x0104 | 3 << 14 | 1 << 13, 0, 0, -100 * 256, -37 * 256, 0x100, 0, 0, 0x100);
  emit_scene("p03_map16_size1_nowrap_scale", 0x0104 | 1 << 14, 0, 0, -20 * 256, -20 * 256, 0x80, 0, 0, 0x180);
  emit_scene("p03_map16_extpal", 0x0104 | 2 << 14, 1u << 30, 0, 0, 0, 0x100, 0, 0, 0x100);
  emit_scene("p03_map16_rotate", 0x0104 | 3 << 14 | 1 << 13, 0, 0, 100 * 256, 0, 0x0B5, 0x0B5, (u16)-0x0B5, 0x0B5);
  emit_scene("p03_map16_dispcnt_base", 0x0104, 1u << 24, 0, 0, 0, 0x100, 0, 0, 0x100);
  emit_scene("p03_bmp256_128", 0x0080, 0, 1, 0, 0, 0x100, 0, 0, 0x100);
  emit_scene("p03_bmp256_256_base1", 0x0080 | 1 << 8 | 1 << 14, 0, 1, 0, 0, 0x100, 0, 0, 0x100);
  emit_scene("p03_bmp256_512x256_scroll", 0x0080 | 2 << 14, 0, 1, 200 * 256, 20 * 256, 0x100, 0, 0, 0x100);
  emit_scene("p03_bmp256_wrap_scale", 0x0080 | 1 << 14 | 1 << 13, 0, 1, -50 * 256, -50 * 256, 0x60, 0x20, 0xFFE0,
             0x60);
  emit_scene("p03_bmp256_nowrap_edge", 0x0080 | 1 << 14, 0, 1, -50 * 256, -50 * 256, 0x100, 0, 0, 0x100);
  emit_scene("p03_direct_256", 0x0084 | 1 << 14, 0, 2, 0, 0, 0x100, 0, 0, 0x100);
  emit_scene("p03_direct_128_wrap", 0x0084 | 1 << 13, 0, 2, 0, 0, 0x100, 0, 0, 0x100);
  emit_scene("p03_direct_512x256_rot", 0x0084 | 2 << 14, 0, 2, 100 * 256, 0, 0x0B5, 0x0B5, (u16)-0x0B5, 0x0B5);
  emit_scene("p03_direct_256_base2", 0x0084 | 2 << 8 | 1 << 14, 0, 2, 0, 0, 0x100, 0, 0, 0x100);
}
