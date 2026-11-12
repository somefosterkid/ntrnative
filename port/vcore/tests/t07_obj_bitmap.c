/* port/vcore piece 7's unit tests: bitmap OBJs, the OBJ window, OBJ mosaic (docs/vcore.md, piece 7). OWNED BY PIECE 7.
 * Run: build/vcore/vcore_tests p07. Expected values worked by hand from GBATEK 26, 07, 08. */
#include "vct.h"
#include <string.h>

static u8 vram[0x40000];

static void put16(u32 a, u16 v) { vram[a] = (u8)v; vram[a + 1] = (u8)(v >> 8); }

static VCoreState *setup(u32 dispcnt) {
  VCoreState *s = vct_state();
  memset(vram, 0, sizeof vram);
  vct_map_flat(s, VC_RGN_AOBJ, vram, sizeof vram);
  vct_eng32(s, 0, VC_REG_DISPCNT, dispcnt);
  return s;
}

static VC_ObjAttr obj(int w, int h, int tile, int alpha) {
  VC_ObjAttr o; memset(&o, 0, sizeof o);
  o.w = w; o.h = h; o.tile = tile; o.palette = alpha; o.mode = 3;
  return o;
}

/* 2D, 128 wide: tile 0x12 = x index 2, y index 1: 2*0x10 + 0x10*0x80 = 0x820. Texel (3,2): + 2*256 + 6. */
VCT_TEST(p07_bmp_2d_128) {
  VCoreState *s = setup(0);
  VC_ObjAttr o = obj(16, 16, 0x12, 9);
  put16(0x820 + 2 * 256 + 6, 0x8000 | 0x1234);
  VCT_EQ(vc_obj_bitmap_texel(s, 0, &o, 3, 2), VC_PX_OPAQUE | VC_PX_BMPOBJ | (9u << 24) | 0x1234);
}

/* 2D, 256 wide (bit 5): tile 0x25: MaskX 0x1F -> x idx 5, y idx 1: 5*0x10 + 0x20*0x80 = 0x1050. Texel (1,7): + 7*512 + 2. */
VCT_TEST(p07_bmp_2d_256) {
  VCoreState *s = setup(1u << 5);
  VC_ObjAttr o = obj(8, 8, 0x25, 15);
  put16(0x1050 + 7 * 512 + 2, 0xFFFF);
  VCT_EQ(vc_obj_bitmap_texel(s, 0, &o, 1, 7), VC_PX_OPAQUE | VC_PX_BMPOBJ | (15u << 24) | 0x7FFF);
}

/* 1D, 128-byte boundary: tile 3 -> 0x180; 32 wide: texel (5,2) at 0x180 + (2*32+5)*2 = 0x180 + 0x8A. */
VCT_TEST(p07_bmp_1d_128) {
  VCoreState *s = setup(1u << 6);
  VC_ObjAttr o = obj(32, 16, 3, 1);
  put16(0x180 + 0x8A, 0x8001);
  VCT_EQ(vc_obj_bitmap_texel(s, 0, &o, 5, 2), VC_PX_OPAQUE | VC_PX_BMPOBJ | (1u << 24) | 1);
}

/* 1D, 256-byte boundary (bit 22): tile 3 -> 0x300. */
VCT_TEST(p07_bmp_1d_256) {
  VCoreState *s = setup((1u << 6) | (1u << 22));
  VC_ObjAttr o = obj(8, 8, 3, 4);
  put16(0x300 + (1 * 8 + 1) * 2, 0x83E0);
  put16(0x180 + (1 * 8 + 1) * 2, 0x8001); /* where the 128-byte mapping would read */
  VCT_EQ(vc_obj_bitmap_texel(s, 0, &o, 1, 1), VC_PX_OPAQUE | VC_PX_BMPOBJ | (4u << 24) | 0x03E0);
}

VCT_TEST(p07_bmp_transparent_bit15) {
  VCoreState *s = setup(1u << 6);
  VC_ObjAttr o = obj(8, 8, 0, 15);
  put16(0, 0x7FFF); /* bit 15 clear */
  VCT_EQ(vc_obj_bitmap_texel(s, 0, &o, 0, 0), 0);
  put16(0, 0x8000); /* black but opaque */
  VCT_EQ(vc_obj_bitmap_texel(s, 0, &o, 0, 0), VC_PX_OPAQUE | VC_PX_BMPOBJ | (15u << 24));
}

VCT_TEST(p07_bmp_outside) {
  VCoreState *s = setup(1u << 6);
  VC_ObjAttr o = obj(8, 8, 0, 15);
  memset(vram, 0xFF, 0x1000);
  VCT_EQ(vc_obj_bitmap_texel(s, 0, &o, 8, 0), 0);
  VCT_EQ(vc_obj_bitmap_texel(s, 0, &o, 0, -1), 0);
}

VCT_TEST(p07_window_plot) {
  VCoreState *s = vct_state();
  VC_Line *ln = &s->line;
  VC_ObjAttr o; memset(&o, 0, sizeof o);
  vc_obj_window_plot(s, 0, &o, 10, ln);
  VCT_EQ(ln->objwin[10], 1);
  VCT_EQ(ln->objwin[9], 0);
  VCT_EQ(ln->obj[10], 0); /* no colour drawn */
}

/* MOSAIC bits 12-15 = 2 -> V size 3: lines 0-2 -> 0, 3-5 -> 3, 7 -> 6. */
VCT_TEST(p07_mosaic_y) {
  VCoreState *s = vct_state();
  vct_eng16(s, 0, VC_REG_MOSAIC, 0x2000);
  VCT_EQ(vc_mosaic_obj_y(s, 0, 0), 0);
  VCT_EQ(vc_mosaic_obj_y(s, 0, 2), 0);
  VCT_EQ(vc_mosaic_obj_y(s, 0, 3), 3);
  VCT_EQ(vc_mosaic_obj_y(s, 0, 5), 3);
  VCT_EQ(vc_mosaic_obj_y(s, 0, 7), 6);
  vct_eng16(s, 0, VC_REG_MOSAIC, 0x00FF); /* BG sizes only: OBJ V = 1 */
  VCT_EQ(vc_mosaic_obj_y(s, 0, 7), 7);
  vct_eng16(s, 0, VC_REG_MOSAIC, 0xF000); /* size 16 */
  VCT_EQ(vc_mosaic_obj_y(s, 0, 33), 32);
  VCT_EQ(vc_mosaic_obj_y(s, 0, 191), 176);
}

/* oracle: the H mosaic is per OBJ: screen pixel x shows the column of the block's first screen pixel (x - x%n),
 * clamped to the OBJ's left edge */
VCT_TEST(p07_mosaic_h) {
  VCoreState *s = vct_state();
  vct_eng16(s, 0, VC_REG_MOSAIC, 0x0500); /* size 6 */
  VCT_EQ(vc_mosaic_obj_hsize(s, 0), 6);
  VCT_EQ(vc_mosaic_obj_x(s, 0, 12, 12), 0);
  VCT_EQ(vc_mosaic_obj_x(s, 0, 17, 12), 0);  /* block 12..17 */
  VCT_EQ(vc_mosaic_obj_x(s, 0, 18, 12), 6);
  VCT_EQ(vc_mosaic_obj_x(s, 0, 14, 14), 0);  /* block start 12 is left of the OBJ: clamp */
  VCT_EQ(vc_mosaic_obj_x(s, 0, 20, 14), 4);  /* block 18 -> column 4 */
  VCT_EQ(vc_mosaic_obj_x(s, 0, 3, -10), 10); /* x 0 is the block start, OBJ starts off screen */
}

/* the vertical mosaic row is (y - y%n) clamped to the OBJ top by the caller (p05); the V flip comes after */
VCT_TEST(p07_mosaic_h_neighbours) {
  VCoreState *s = vct_state();
  vct_eng16(s, 0, VC_REG_MOSAIC, 0x0300);
  VCT_EQ(vc_mosaic_obj_x(s, 0, 7, 0), 4);
}

VCT_TEST(p07_mosaic_h_off) {
  VCoreState *s = vct_state();
  vct_eng16(s, 0, VC_REG_MOSAIC, 0x0000);
  VC_Line *ln = &s->line;
  ln->obj[0] = VC_PX_OPAQUE | VC_PX_MOSAIC | 1;
  ln->obj[1] = VC_PX_OPAQUE | VC_PX_MOSAIC | 2;
  vc_mosaic_obj_line(s, 0, ln);
  VCT_EQ(ln->obj[1], VC_PX_OPAQUE | VC_PX_MOSAIC | 2);
}
