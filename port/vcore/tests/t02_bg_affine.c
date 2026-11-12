/* port/vcore piece 2's unit tests: affine BGs (docs/vcore.md, piece 2). OWNED BY PIECE 2.
 * Run: build/vcore/vcore_tests p02
 * Expected values are worked by hand from GBATEK 06 (X + x*PA + y*PB, Y + x*PC + y*PD, 24.8), 04 (sizes,
 * overflow), 12 (8-bit entries), 25 (bases). The map entry at tile (tx, ty) is f(tx, ty) (below); every pixel
 * of tile t has colour index t, except tile 255 (only at tile 0,0), whose pixel (i, j) has index 100 + j*8 + i,
 * and tile 0 (at tile 1,0), which is all index 0. Palette colour i = BGR555 i. */
#include "vct.h"

static int f(int tx, int ty) {
  if (tx == 0 && ty == 0) return 255;
  if (tx == 1 && ty == 0) return 0;
  return 1 + ((tx * 5 + ty * 11) % 200);
}

/* bank A (128 KB) is engine A BG VRAM; bank C is engine B BG VRAM. Map at screen offset scr, tiles at chr. */
static VCoreState *mk(int e, int sizebits, u32 scr, u32 chr) {
  VCoreState *s = vct_state();
  u8 *v = vc_bank(s, e ? VC_BANK_C : VC_BANK_A);
  vct_map_flat(s, e ? VC_RGN_BBG : VC_RGN_ABG, v, 0x20000);
  int dim = 1 << sizebits, nt = dim / 8;
  for (int ty = 0; ty < nt; ty++)
    for (int tx = 0; tx < nt; tx++) v[scr + ty * nt + tx] = (u8)f(tx, ty);
  for (int t = 0; t < 256; t++)
    for (int j = 0; j < 8; j++)
      for (int i = 0; i < 8; i++) v[chr + t * 64 + j * 8 + i] = (u8)(t == 255 ? 100 + j * 8 + i : t);
  for (int i = 1; i < 256; i++) vct_pal(s, e, 0, i, (u16)i);
  return s;
}
static void regs(VCoreState *s, int e, int n, u16 cnt, u16 pa, u16 pb, u16 pc, u16 pd, u32 x, u32 y) {
  u32 b = (u32)(n - 2) * 0x10;
  vct_eng16(s, e, VC_REG_BG0CNT + 2 * n, cnt);
  vct_eng16(s, e, VC_REG_BG2PA + b, pa);
  vct_eng16(s, e, VC_REG_BG2PB + b, pb);
  vct_eng16(s, e, VC_REG_BG2PC + b, pc);
  vct_eng16(s, e, VC_REG_BG2PD + b, pd);
  vct_eng32(s, e, VC_REG_BG2X + b, x);
  vct_eng32(s, e, VC_REG_BG2Y + b, y);
}
/* char base block 1 (16 KB), size index sizeidx (bits 14-15), overflow bit 13 */
#define CNT(sizeidx, wrap) ((u16)(1u << 2 | (wrap) << 13 | (sizeidx) << 14))
/* the pixel the map and tile data give at BG position (px, py) */
static u32 want(int px, int py) {
  int t = f(px >> 3, py >> 3);
  int idx = t == 255 ? 100 + (py & 7) * 8 + (px & 7) : t;
  return idx ? (VC_PX_OPAQUE | (u32)idx) : 0;
}
/* a plain frame for the oracle: mode 2, BG2 on, bank A as engine A BG VRAM */
static void emit(VCoreState *s, const char *name) {
  vct_eng32(s, VC_ENG_A, VC_REG_DISPCNT, 0x00010402u | (vc_eng32(s, 0, 0) & 0x3F000000u));
  s->io9[VC_REG_VRAMCNT_A] = 0x81;
  vct_emit(s, name);
}
static void line(VCoreState *s, int e, int n, int y) { vc_bg_affine_line(s, e, n, y, &s->line); }

VCT_TEST(p02_pos_formula) {
  /* X=0x1234, PA=0x123, PB=-0x100, PC=0x10, PD=0x200, Y=-0x100 (28 bits 0xFFFFF00); x=1, y=2:
     tx = 0x1234 + 0x123 - 2*0x100 = 0x1157; ty = -0x100 + 0x10 + 2*0x200 = 0x310 */
  VCoreState *s = vct_state();
  regs(s, 0, 2, 0, 0x123, 0xFF00, 0x10, 0x200, 0x1234, 0x0FFFFF00);
  s32 tx, ty;
  vc_bg_affine_pos(s, 0, 2, 1, 2, &tx, &ty);
  VCT_EQ(tx, 0x1157);
  VCT_EQ(ty, 0x310);
  vc_bg_affine_pos(s, 0, 2, 0, 0, &tx, &ty);
  VCT_EQ(tx, 0x1234);
  VCT_EQ(ty, -0x100);
}

VCT_TEST(p02_pos_bg3_registers) {
  VCoreState *s = vct_state();
  regs(s, 0, 2, 0, 0x100, 0, 0, 0x100, 0x500, 0);
  regs(s, 0, 3, 0, 0x200, 0x10, 0x20, 0x300, 0x0700, 0x0900);
  s32 tx, ty;
  /* tx = 0x700 + 2*0x200 + 3*0x10 = 0xB30; ty = 0x900 + 2*0x20 + 3*0x300 = 0x1240 */
  vc_bg_affine_pos(s, 0, 3, 2, 3, &tx, &ty);
  VCT_EQ(tx, 0xB30);
  VCT_EQ(ty, 0x1240);
}

VCT_TEST(p02_pos_28bit_sign) {
  /* 28-bit signed: 0x08000000 is -2^27 (bit 27 = sign); the upper nibble is ignored, so 0xF8000000 is the same */
  VCoreState *s = vct_state();
  regs(s, 0, 2, 0, 0, 0, 0, 0, 0xF8000000u, 0x07FFFFFFu);
  s32 tx, ty;
  vc_bg_affine_pos(s, 0, 2, 0, 0, &tx, &ty);
  VCT_EQ(tx, -0x08000000);
  VCT_EQ(ty, 0x07FFFFFF);
}

VCT_TEST(p02_identity) {
  VCoreState *s = mk(0, 7, 0, 0x4000);
  regs(s, 0, 2, CNT(0, 0), 0x100, 0, 0, 0x100, 0, 0);
  line(s, 0, 2, 0);
  /* y=0: tile (0,0) = 255, pixel (3,0) = 100 + 0 + 3 = 103; tile (1,0) = tile 0 -> transparent */
  VCT_EQ(s->line.bg[2][3], VC_PX_OPAQUE | 103);
  VCT_EQ(s->line.bg[2][8], 0);
  VCT_EQ(f(2, 0), 11); /* 1 + 10 */
  VCT_EQ(s->line.bg[2][16], VC_PX_OPAQUE | 11);
  line(s, 0, 2, 19); /* y=19: tile row 2, pixel row 3 */
  VCT_EQ(f(1, 2), 28); /* 1 + (5 + 22) */
  VCT_EQ(s->line.bg[2][9], VC_PX_OPAQUE | 28);
  /* a 128x128 BG: x 128-255 is outside, transparent (overflow bit clear) */
  VCT_EQ(s->line.bg[2][127], want(127, 19));
  VCT_EQ(s->line.bg[2][128], 0);
  VCT_EQ(s->line.bg[2][255], 0);
  line(s, 0, 2, 130); /* y=130 is outside too */
  VCT_EQ(s->line.bg[2][0], 0);
  VCT_EQ(s->line.bg[2][50], 0);
  emit(s, "p02_identity");
}

VCT_TEST(p02_identity_256) {
  VCoreState *s = mk(0, 8, 0, 0x4000);
  regs(s, 0, 2, CNT(1, 0), 0x100, 0, 0, 0x100, 0, 0);
  line(s, 0, 2, 130);
  VCT_EQ(f(5, 16), 2); /* 1 + (25 + 176) % 200 = 2 */
  VCT_EQ(s->line.bg[2][40], VC_PX_OPAQUE | 2);
  for (int x = 0; x < 256; x += 7) VCT_EQ(s->line.bg[2][x], want(x, 130));
  line(s, 0, 2, 255);
  for (int x = 0; x < 256; x += 5) VCT_EQ(s->line.bg[2][x], want(x, 255));
  emit(s, "p02_identity_256");
}

VCT_TEST(p02_scale_2x) {
  /* PA = PD = 0x200: pixel (x, y) shows BG (2x, 2y): (9, 17) -> (18, 34) = tile (2, 4) = 1 + (10 + 44) = 55 */
  VCoreState *s = mk(0, 8, 0, 0x4000);
  regs(s, 0, 2, CNT(1, 0), 0x200, 0, 0, 0x200, 0, 0);
  line(s, 0, 2, 17);
  VCT_EQ(s->line.bg[2][9], VC_PX_OPAQUE | 55);
  for (int x = 0; x < 128; x++) VCT_EQ(s->line.bg[2][x], want(2 * x, 34));
  VCT_EQ(s->line.bg[2][128], 0); /* BG x = 256 */
  emit(s, "p02_scale_2x");
}

VCT_TEST(p02_scale_half) {
  /* PA = PD = 0x80: (9, 17) -> tx = 9*128 = 0x480 -> 4, ty = 17*128 = 0x880 -> 8: tile (0, 1) = 1 + 11 = 12 */
  VCoreState *s = mk(0, 8, 0, 0x4000);
  regs(s, 0, 2, CNT(1, 0), 0x80, 0, 0, 0x80, 0, 0);
  line(s, 0, 2, 17);
  VCT_EQ(s->line.bg[2][9], VC_PX_OPAQUE | 12);
  for (int x = 0; x < 256; x++) VCT_EQ(s->line.bg[2][x], want(x / 2, 8));
  emit(s, "p02_scale_half");
}

VCT_TEST(p02_rotate_90) {
  /* PA = 0, PB = 0x100, PC = -0x100, PD = 0, Y = 100: pixel (x, y) shows BG (y, 100 - x); (9, 17) -> (17, 91):
     tile (2, 11) = 1 + (10 + 121) = 132 */
  VCoreState *s = mk(0, 8, 0, 0x4000);
  regs(s, 0, 2, CNT(1, 0), 0, 0x100, 0xFF00, 0, 0, 100 << 8);
  line(s, 0, 2, 17);
  VCT_EQ(s->line.bg[2][9], VC_PX_OPAQUE | 132);
  for (int x = 0; x < 101; x++) VCT_EQ(s->line.bg[2][x], want(17, 100 - x));
  VCT_EQ(s->line.bg[2][101], 0); /* BG y = -1: outside */
  emit(s, "p02_rotate_90");
}

VCT_TEST(p02_reference_point) {
  /* X = -8 (the word 0xFFFFF800, upper nibble ignored): pixel 8 shows BG x = 0 */
  VCoreState *s = mk(0, 8, 0, 0x4000);
  regs(s, 0, 2, CNT(1, 0), 0x100, 0, 0, 0x100, 0xFFFFF800u, 0);
  line(s, 0, 2, 0);
  VCT_EQ(s->line.bg[2][0], 0);
  VCT_EQ(s->line.bg[2][7], 0);
  VCT_EQ(s->line.bg[2][8], VC_PX_OPAQUE | 100); /* tile 255 pixel (0, 0) = 100 */
  VCT_EQ(s->line.bg[2][9], VC_PX_OPAQUE | 101);
  /* a reference y of 3.5: row 3 (the fraction 0x80 is dropped) */
  regs(s, 0, 2, CNT(1, 0), 0x100, 0, 0, 0x100, 0, 0x380);
  line(s, 0, 2, 0);
  VCT_EQ(s->line.bg[2][2], VC_PX_OPAQUE | (100 + 3 * 8 + 2));
  /* line y adds y*PB: PB = 0x100, X = 0 -> line 4 starts at BG x = 4 */
  regs(s, 0, 2, CNT(1, 0), 0x100, 0x100, 0, 0x100, 0, 0);
  line(s, 0, 2, 4);
  VCT_EQ(s->line.bg[2][0], want(4, 4));
  VCT_EQ(s->line.bg[2][3], want(7, 4));
  VCT_EQ(s->line.bg[2][4], 0); /* BG (8, 4): tile (1, 0) = tile 0 */
  emit(s, "p02_reference_point");
}

VCT_TEST(p02_sizes) {
  /* the four sizes (the map row is size/8 tiles; char base block 4 = 0x10000 so the 16 KB map of 1024 fits) */
  static const int dim[4] = {128, 256, 512, 1024};
  for (int sz = 0; sz < 4; sz++) {
    VCoreState *s = mk(0, 7 + sz, 0, 0x10000);
    int y = dim[sz] - 3, x0 = dim[sz] > 256 ? dim[sz] - 256 : 0;
    regs(s, 0, 2, (u16)(4u << 2 | sz << 14), 0x100, 0, 0, 0x100, (u32)x0 << 8, (u32)y << 8);
    line(s, 0, 2, 0);
    for (int x = 0; x < 256; x++) {
      int px = x0 + x;
      VCT_EQ(s->line.bg[2][x], px < dim[sz] ? want(px, y) : 0);
    }
    if (sz == 3) {
      /* BG (1000, 0): tile (125, 0) = 1 + 625 % 200 = 26 */
      regs(s, 0, 2, (u16)(4u << 2 | sz << 14), 0x100, 0, 0, 0x100, 1000u << 8, 0);
      line(s, 0, 2, 0);
      VCT_EQ(f(125, 0), 26);
      VCT_EQ(s->line.bg[2][0], VC_PX_OPAQUE | 26);
      emit(s, "p02_size1024");
    }
    if (sz == 2) emit(s, "p02_size512");
  }
}

VCT_TEST(p02_wrap_vs_transparent) {
  /* size 128, X = 120, Y = 120: pixel (x, y) at BG (120 + x, 120 + y) */
  for (int w = 0; w < 2; w++) {
    VCoreState *s = mk(0, 7, 0, 0x4000);
    regs(s, 0, 2, CNT(0, w), 0x100, 0, 0, 0x100, 120 << 8, 120 << 8);
    line(s, 0, 2, 5); /* BG y = 125 */
    for (int x = 0; x < 256; x++) {
      int px = 120 + x;
      VCT_EQ(s->line.bg[2][x], (!w && px >= 128) ? 0 : want(px & 127, 125));
    }
    line(s, 0, 2, 10); /* BG y = 130: outside, or wraps to 2 */
    for (int x = 0; x < 256; x++) VCT_EQ(s->line.bg[2][x], w ? want((120 + x) & 127, 2) : 0);
    if (w) {
      VCT_EQ(want(0, 2), VC_PX_OPAQUE | 116); /* tile 255 pixel (0, 2) = 100 + 16 */
      VCT_EQ(s->line.bg[2][8], VC_PX_OPAQUE | 116);
    }
    emit(s, w ? "p02_wrap" : "p02_nowrap");
  }
  /* negative positions wrap too: -8 -> 120, tile (15, 0) = 1 + 75 = 76 */
  VCoreState *s = mk(0, 7, 0, 0x4000);
  regs(s, 0, 2, CNT(0, 1), 0x100, 0, 0, 0x100, 0xFFFFF800u, 0);
  line(s, 0, 2, 0);
  VCT_EQ(s->line.bg[2][0], VC_PX_OPAQUE | 76);
  VCT_EQ(s->line.bg[2][8], VC_PX_OPAQUE | 100);
}

VCT_TEST(p02_bases_engine_a) {
  /* char base block 2 (32 KB), screen base 3 (6 KB); DISPCNT bits 24-26 = 1 adds 64 KB to the tiles */
  VCoreState *s = vct_state();
  u8 *v = vc_bank(s, VC_BANK_A);
  vct_map_flat(s, VC_RGN_ABG, v, 0x20000);
  vct_eng32(s, 0, VC_REG_DISPCNT, 1u << 24);
  vct_eng16(s, 0, VC_REG_BG2CNT, (u16)(2 << 2 | 3 << 8)); /* size 128 */
  vct_eng16(s, 0, VC_REG_BG2PA, 0x100);
  vct_eng16(s, 0, VC_REG_BG2PD, 0x100);
  v[3 * 0x800] = 7;                               /* map entry (0,0) = tile 7 */
  v[0x10000 + 0x8000 + 7 * 64 + 2 * 8 + 5] = 9;   /* tile 7 pixel (5, 2) = index 9 */
  vct_pal(s, 0, 0, 9, 0x1234);
  line(s, 0, 2, 2);
  VCT_EQ(s->line.bg[2][5], VC_PX_OPAQUE | 0x1234);
  VCT_EQ(s->line.bg[2][4], 0);
  /* DISPCNT bits 27-29 = 1 adds 64 KB to the map, bits 24-26 = 0: tiles at 32 KB */
  vct_eng32(s, 0, VC_REG_DISPCNT, 1u << 27);
  v[0x10000 + 3 * 0x800] = 8;
  v[0x8000 + 8 * 64 + 2 * 8 + 5] = 11;
  vct_pal(s, 0, 0, 11, 0x0555);
  line(s, 0, 2, 2);
  VCT_EQ(s->line.bg[2][5], VC_PX_OPAQUE | 0x0555);
  emit(s, "p02_bases_a");
}

VCT_TEST(p02_bases_engine_b) {
  /* engine B ignores DISPCNT bits 24-29 */
  VCoreState *s = vct_state();
  u8 *v = vc_bank(s, VC_BANK_C);
  vct_map_flat(s, VC_RGN_BBG, v, 0x20000);
  vct_eng32(s, 1, VC_REG_DISPCNT, (7u << 24) | (7u << 27));
  vct_eng16(s, 1, VC_REG_BG2CNT, (u16)(1 << 2 | 1 << 8));
  vct_eng16(s, 1, VC_REG_BG2PA, 0x100);
  vct_eng16(s, 1, VC_REG_BG2PD, 0x100);
  v[0x800] = 3;                   /* map at 2 KB */
  v[0x4000 + 3 * 64 + 8 + 1] = 5; /* tile 3 at 16 KB, pixel (1, 1) */
  vct_pal(s, 1, 0, 5, 0x7C00);
  line(s, 1, 2, 1);
  VCT_EQ(s->line.bg[2][1], VC_PX_OPAQUE | 0x7C00);
  VCT_EQ(s->line.bg[2][0], 0);
}

VCT_TEST(p02_colour0_transparent_and_palette) {
  VCoreState *s = mk(0, 7, 0, 0x4000);
  regs(s, 0, 2, CNT(0, 0), 0x100, 0, 0, 0x100, 0, 0);
  vct_pal(s, 0, 0, 0, 0x7FFF);  /* palette colour 0 is not drawn */
  vct_pal(s, 0, 0, 11, 0x03E0); /* tile (2,0) = 11 */
  vct_pal(s, 0, 0, 255, 0x1F);
  line(s, 0, 2, 0);
  VCT_EQ(s->line.bg[2][8], 0);                      /* tile 0 */
  VCT_EQ(s->line.bg[2][16], VC_PX_OPAQUE | 0x03E0); /* tile 11 */
  vct_pal(s, 0, 1, 11, 0x1111); /* the OBJ palette is not used */
  line(s, 0, 2, 0);
  VCT_EQ(s->line.bg[2][16], VC_PX_OPAQUE | 0x03E0);
}

VCT_TEST(p02_bg3_and_engine_b) {
  VCoreState *s = mk(1, 7, 0, 0x4000);
  regs(s, 1, 3, CNT(0, 0), 0x100, 0, 0, 0x100, 0, 0);
  line(s, 1, 3, 0);
  VCT_EQ(s->line.bg[3][3], VC_PX_OPAQUE | 103);
  VCT_EQ(s->line.bg[3][16], VC_PX_OPAQUE | 11);
}
