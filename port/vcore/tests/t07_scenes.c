/* port/vcore piece 7's synthetic scenes for the oracle (VCT_EMIT=DIR vcore_tests p07). OWNED BY PIECE 7. Each test
 * also checks that the scene is built as intended (the bitmap OBJ texel through vc_obj_bitmap_texel). */
#include "vct.h"
#include <string.h>

static void oam(VCoreState *s, int i, u16 a0, u16 a1, u16 a2) {
  u8 *o = s->oam + i * 8;
  o[0] = (u8)a0; o[1] = (u8)(a0 >> 8); o[2] = (u8)a1; o[3] = (u8)(a1 >> 8); o[4] = (u8)a2; o[5] = (u8)(a2 >> 8);
}
static void bput(VCoreState *s, u32 a, u16 v) { u8 *b = vc_bank(s, VC_BANK_A); b[a] = (u8)v; b[a + 1] = (u8)(v >> 8); }
/* engine A, bank A as its OBJ VRAM, display mode 1, OBJs on, all OAM entries hidden */
static VCoreState *scene(u32 dispcnt) {
  VCoreState *s = vct_state();
  vct_io8(s, VC_REG_VRAMCNT_A, 0x82);
  vct_eng32(s, 0, VC_REG_DISPCNT, dispcnt | (1u << 16) | (1u << 12));
  vct_pal(s, 0, 0, 0, 0x3DEF);
  for (int i = 0; i < 128; i++) oam(s, i, 0x0200, 0, 0);
  return s;
}
/* a w x h bitmap at addr (row stride in bytes); every 5th texel (x + y) has bit 15 clear */
static void fill(VCoreState *s, u32 addr, int w, int h, int stride) {
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
      u16 c = (u16)(((x * 31 / w) & 31) | (((y * 31 / h) & 31) << 5) | (((x ^ y) & 31) << 10));
      bput(s, addr + y * stride + x * 2, ((x + y) % 5 == 4) ? (c & 0x7FFF) : (c | 0x8000));
    }
}

VCT_TEST(p07_scene_bmp_2d) {
  VCoreState *s = scene(0);
  fill(s, 0, 32, 32, 256);
  oam(s, 0, 0x0C00 | 20, 0x8000 | 30, 0xF000 | 0);
  fill(s, 0x1000, 32, 32, 256); /* tile 0x20: y index 2 -> 0x20 * 0x80 = 0x1000 */
  oam(s, 1, 0x0C00 | 70, 0x8000 | 80, 0x8000 | 0x20);
  oam(s, 2, 0x0C00 | 120, 0x8000 | 150, 0x0000 | 0x20);
  vct_emit(s, "p07_scene_bmp_2d");
  s = scene(1u << 5);
  fill(s, 0, 32, 32, 512);
  oam(s, 0, 0x0C00 | 20, 0x8000 | 30, 0xA000);
  vct_emit(s, "p07_scene_bmp_2d_256");
}

VCT_TEST(p07_scene_bmp_1d) {
  VCoreState *s = scene(1u << 6);
  fill(s, 0, 32, 32, 64);
  oam(s, 0, 0x0C00 | 20, 0x8000 | 30, 0xF000);
  fill(s, 0x800, 16, 16, 32);
  oam(s, 1, 0x0C00 | 60, 0x4000 | 100, 0x8000 | 16); /* 16x16, tile 16 * 128 = 0x800 */
  vct_emit(s, "p07_scene_bmp_1d");
  s = scene((1u << 6) | (1u << 22));
  fill(s, 0x400, 16, 16, 32);
  oam(s, 0, 0x0C00 | 60, 0x4000 | 100, 0xC000 | 4); /* tile 4 * 256 = 0x400 */
  vct_emit(s, "p07_scene_bmp_1d_256");
}

/* an OBJ-window OBJ (tile OBJ, mode 2) removes the bitmap OBJ under it: WINOUT outside = OBJ, OBJ window = nothing */
VCT_TEST(p07_scene_objwin) {
  VCoreState *s = scene((1u << 6) | (1u << 15));
  fill(s, 0x2000, 32, 32, 64);
  oam(s, 0, 0x0C00 | 20, 0x8000 | 20, 0xF000 | 64); /* bitmap, tile 64 * 128 = 0x2000 */
  for (int i = 0; i < 0x800; i++) s->vram[0x4000 + i] = (i & 8) ? 0x11 : 0x00; /* 4bpp tiles from tile 512 (0x4000 / 32) */
  oam(s, 1, 0x0800 | 30, 0x8000 | 30, 512);
  vct_eng16(s, 0, VC_REG_WINOUT, 0x0010);
  vct_emit(s, "p07_scene_objwin");
}

/* mosaic: OBJ mosaic 4 x 5 on a mosaic bitmap OBJ beside a plain one and a mosaic one touching the plain one */
VCT_TEST(p07_scene_mosaic) {
  VCoreState *s = scene(1u << 6);
  fill(s, 0, 32, 32, 64);
  oam(s, 0, 0x1C00 | 20, 0x8000 | 30, 0xF000);
  fill(s, 0x800, 32, 32, 64);
  oam(s, 1, 0x0C00 | 20, 0x8000 | 62, 0xF000 | 16);
  fill(s, 0x1000, 32, 32, 64);
  oam(s, 2, 0x1C00 | 70, 0x8000 | 50, 0xF000 | 32);
  vct_eng16(s, 0, VC_REG_MOSAIC, 0x4300);
  vct_emit(s, "p07_scene_mosaic");
}
