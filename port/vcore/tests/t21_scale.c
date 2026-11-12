/* port/vcore piece 21's unit tests: the 3D at N x (docs/vcore.md, "vcore at N x"). OWNED BY PIECE 21.
 * Run: build/vcore/vcore_tests p21. A frame goes through the real pipeline (the geometry commands, p16's walk on the N x
 * grid, the picture): engine A shows BG0 = the 3D in display mode 1 on the top screen, backdrop black. Expected pixels are
 * worked by hand from GBATEK 44 (screen_x = (x + w) * width / 2w, y from the top) and 51 (an opaque polygon leaves out its
 * right and bottom edges). */
#include "vct.h"

#define ONE 0x1000

static VCoreState *frame3d(int scale) {
  VCoreState *s = vct_state();
  vct_eng32(s, VC_ENG_A, VC_REG_DISPCNT, 0x00010108u); /* display mode 1, BG0 on, BG0 is the 3D */
  vct_eng32(s, VC_ENG_B, VC_REG_DISPCNT, 0x00010000u); /* engine B: display mode 1, nothing on: the backdrop */
  vct_io16(s, VC_REG_POWCNT1, 0x820F);                 /* both engines and the 3D on, engine A on the top screen */
  vct_io16(s, VC_REG_CLEAR_DEPTH, 0x7FFF);            /* the rear plane at the far end: polygons pass the depth test */
  vcore_set_scale(s, scale);
  return s;
}
static void gx(VCoreState *s, u32 op, u32 a, u32 b, u32 c) {
  u32 p[3] = {a, b, c};
  vct_gx(s, op, op == VC_OP_END_VTXS ? 0 : 1, p);
}
static void v16(VCoreState *s, int x, int y, int z) {
  u32 p[2] = {(u32)(x & 0xFFFF) | (u32)(y & 0xFFFF) << 16, (u32)(z & 0xFFFF)};
  vct_gx(s, VC_OP_VTX_16, 2, p);
}
static void begin_poly(VCoreState *s, u32 attr, int type) {
  gx(s, VC_OP_POLYGON_ATTR, attr, 0, 0);
  gx(s, VC_OP_COLOR, 0x7FFF, 0, 0);
  gx(s, VC_OP_BEGIN_VTXS, (u32)type, 0, 0);
}
static void end_frame(VCoreState *s) {
  gx(s, VC_OP_END_VTXS, 0, 0, 0);
  gx(s, VC_OP_SWAP_BUFFERS, 0, 0, 0);
}
static u32 px(const uint8_t *pic, int w, int x, int y) {
  const uint8_t *p = pic + ((size_t)y * w + x) * 4;
  return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16;
}

/* a white quad (-0.5..0.5) lands on DS pixels x 64..191, y 48..143 (right and bottom dropped); at 3x on x 192..575, y 144..431 */
VCT_TEST(p21_quad_grid) {
  VCoreState *s = frame3d(3);
  begin_poly(s, 0x001F00C0, 1);
  v16(s, -ONE / 2, ONE / 2, 0);
  v16(s, -ONE / 2, -ONE / 2, 0);
  v16(s, ONE / 2, -ONE / 2, 0);
  v16(s, ONE / 2, ONE / 2, 0);
  end_frame(s);
  vcore_render(s);
  int w = 0, h = 0;
  const uint8_t *pic = vcore_picture(s, &w, &h);
  VCT_CHECK(pic != NULL);
  VCT_EQ(w, 768);
  VCT_EQ(h, 1152);
  const u32 white = 0xFFFFFF, black = 0;
  VCT_EQ(px(pic, w, 192, 144), white);
  VCT_EQ(px(pic, w, 575, 431), white);
  VCT_EQ(px(pic, w, 191, 144), black);
  VCT_EQ(px(pic, w, 192, 143), black);
  VCT_EQ(px(pic, w, 576, 300), black); /* the dropped right edge: one N x pixel */
  VCT_EQ(px(pic, w, 300, 432), black); /* the dropped bottom edge */
  VCT_EQ(px(pic, w, 576, 431), black);
  /* the bottom screen (engine B, blank) is black */
  VCT_EQ(px(pic, w, 300, 576 + 300), black);
}

/* a triangle that collapses to one DS pixel (POLYGON_ATTR bit 13 and DISP_1DOT_DEPTH show it) is an N x N block at 1x scaled: DS pixel
 * (136, 90) (x = y = 0x100: (0x100 + 0x1000) * 256 / 0x2000, (0x1000 - 0x100) * 192 / 0x2000) at 3x is x 408..410, y 270..272 */
VCT_TEST(p21_dot_block) {
  VCoreState *s = frame3d(3);
  vct_io16(s, VC_REG_DISP_1DOT_DEPTH, 0x7FFF); /* the polygon is shown: bit 13 and w <= this (GBATEK 35) */
  begin_poly(s, 0x001F20C0, 0);
  v16(s, 0x100, 0x100, 0);
  v16(s, 0x100, 0x100, 0);
  v16(s, 0x100, 0x100, 0);
  end_frame(s);
  vcore_render(s);
  int w = 0, h = 0;
  const uint8_t *pic = vcore_picture(s, &w, &h);
  VCT_CHECK(pic != NULL);
  const u32 white = 0xFFFFFF;
  for (int j = 0; j < 3; j++)
    for (int i = 0; i < 3; i++) VCT_EQ(px(pic, w, 408 + i, 270 + j), white);
  VCT_EQ(px(pic, w, 407, 270), 0);
  VCT_EQ(px(pic, w, 411, 271), 0);
  VCT_EQ(px(pic, w, 409, 269), 0);
  VCT_EQ(px(pic, w, 409, 273), 0);
  /* Dot=fine: one N x pixel (the vertex's own position on the N x grid) */
  vcore_set_options(s, 1, 0, 0);
  vcore_render(s);
  pic = vcore_picture(s, &w, &h);
  int white_px = 0;
  for (int y = 262; y < 282; y++)
    for (int x = 400; x < 420; x++)
      if (px(pic, w, x, y) == white) white_px++;
  VCT_EQ(white_px, 1);
}

/* the N path forced at scale 1 draws the 1x core's picture */
VCT_TEST(p21_scale1_equals_1x) {
  VCoreState *a = frame3d(1), *b = frame3d(1);
  VCoreState *both[2] = {a, b};
  for (int k = 0; k < 2; k++) {
    VCoreState *s = both[k];
    begin_poly(s, 0x001F00C0, 0);
    v16(s, -ONE / 3, ONE / 2, 0);
    v16(s, -ONE / 2, -ONE / 3, 0);
    v16(s, ONE / 2, -ONE / 7, 0);
    end_frame(s);
  }
  vcore_render(a);
  b->p21.force_n = 1;
  vcore_render(b);
  int w = 0, h = 0;
  const uint8_t *pic = vcore_picture(b, &w, &h);
  VCT_CHECK(pic != NULL);
  VCT_EQ(w, 256);
  VCT_EQ(h, 384);
  uint8_t ref[256 * 384 * 4];
  vcore_frame_rgba(a, ref);
  VCT_CHECK(memcmp(ref, pic, sizeof ref) == 0);
}
