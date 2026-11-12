/* port/vcore piece 20's unit tests: integration: the frame, the 3D layer, the screens (docs/vcore.md, piece 20). OWNED BY PIECE 20.
 * Run: build/vcore/vcore_tests p20 */
#include "vct.h"

/* which BGs vc_bg_line rendered under (engine, DISPCNT) */
static int bgs_on(VCoreState *s, int e, u32 dispcnt, int *three_d) {
  vct_eng32(s, e, VC_REG_DISPCNT, dispcnt);
  VC_Line *ln = &s->line;
  memset(ln, 0, sizeof *ln);
  for (int n = 0; n < 4; n++) vc_bg_line(s, e, n, 0, ln);
  if (three_d) *three_d = ln->bg3d;
  return ln->bg_on[0] | ln->bg_on[1] << 1 | ln->bg_on[2] << 2 | ln->bg_on[3] << 3;
}

/* GBATEK 25: modes 0-5 have all four BGs; mode 6 only BG2 (large); mode 7 none */
VCT_TEST(p20_mode_table_all_bgs_enabled) {
  VCoreState *s = vct_state();
  for (int m = 0; m < 6; m++) VCT_EQ(bgs_on(s, VC_ENG_A, 0x0F00 | m, NULL), 0xF);
  VCT_EQ(bgs_on(s, VC_ENG_A, 0x0F00 | 6, NULL), 0x4);
  VCT_EQ(bgs_on(s, VC_ENG_A, 0x0F00 | 7, NULL), 0);
}

VCT_TEST(p20_mode_enable_bits) {
  VCoreState *s = vct_state();
  VCT_EQ(bgs_on(s, VC_ENG_A, 0x0200 | 0, NULL), 0x2);
  VCT_EQ(bgs_on(s, VC_ENG_A, 0x0800 | 2, NULL), 0x8);
  VCT_EQ(bgs_on(s, VC_ENG_A, 0x0000 | 0, NULL), 0);
}

VCT_TEST(p20_engine_b_no_large) {
  VCoreState *s = vct_state();
  VCT_EQ(bgs_on(s, VC_ENG_B, 0x0F00 | 6, NULL), 0);
  VCT_EQ(bgs_on(s, VC_ENG_B, 0x0F00 | 5, NULL), 0xF);
}

VCT_TEST(p20_engine_b_no_3d) {
  VCoreState *s = vct_state();
  int d;
  VCT_EQ(bgs_on(s, VC_ENG_B, 0x0100 | 8, &d), 1);
  VCT_EQ(d, 0);
}

VCT_TEST(p20_bg0_3d) {
  VCoreState *s = vct_state();
  int d;
  VCT_EQ(bgs_on(s, VC_ENG_A, 0x0100 | 8, &d), 1);
  VCT_EQ(d, 1);
  VCT_EQ(bgs_on(s, VC_ENG_A, 0x0100 | 0, &d), 1);
  VCT_EQ(d, 0);
  VCT_EQ(bgs_on(s, VC_ENG_A, 0x0100 | 6, &d), 0);
  VCT_EQ(bgs_on(s, VC_ENG_A, 0x0100 | 6 | 8, &d), 1);
  VCT_EQ(d, 1);
}

static void fill3d(VCoreState *s, int y) {
  for (int x = 0; x < VC_W; x++) s->f3d.out[y][x] = VC_RGB6(x & 63, 2, 3) | (31u << 24);
}

VCT_TEST(p20_3d_layer_plain) {
  VCoreState *s = vct_state();
  fill3d(s, 5);
  s->f3d.out[5][7] = VC_RGB6(9, 9, 9) | (16u << 24);
  vc_3d_layer_line(s, 5, &s->line);
  VCT_EQ(s->line.l3d[0], VC_RGB6(0, 2, 3) | (31u << 24));
  VCT_EQ(s->line.l3d[7], VC_RGB6(9, 9, 9) | (16u << 24));
  VCT_EQ(s->line.l3d[255], VC_RGB6(63, 2, 3) | (31u << 24));
}

VCT_TEST(p20_3d_layer_alpha0_transparent) {
  VCoreState *s = vct_state();
  fill3d(s, 0);
  s->f3d.out[0][3] = VC_RGB6(9, 9, 9);
  vc_3d_layer_line(s, 0, &s->line);
  VCT_EQ(s->line.l3d[3], 0);
  VCT_CHECK(s->line.l3d[4] != 0);
}

/* HOFS 10: screen x shows 3D x+10; the last 10 pixels fall in the transparent half */
VCT_TEST(p20_3d_layer_scroll) {
  VCoreState *s = vct_state();
  fill3d(s, 1);
  vct_eng16(s, VC_ENG_A, VC_REG_BG0HOFS, 10);
  vc_3d_layer_line(s, 1, &s->line);
  VCT_EQ(s->line.l3d[0], VC_RGB6(10, 2, 3) | (31u << 24));
  VCT_EQ(s->line.l3d[245], VC_RGB6(255 & 63, 2, 3) | (31u << 24));
  VCT_EQ(s->line.l3d[246], 0);
  VCT_EQ(s->line.l3d[255], 0);
}

/* HOFS 266 = 256+10: 246 transparent pixels, then the image wraps in from its start */
VCT_TEST(p20_3d_layer_wrap) {
  VCoreState *s = vct_state();
  fill3d(s, 1);
  vct_eng16(s, VC_ENG_A, VC_REG_BG0HOFS, 266);
  vc_3d_layer_line(s, 1, &s->line);
  VCT_EQ(s->line.l3d[0], 0);
  VCT_EQ(s->line.l3d[245], 0);
  VCT_EQ(s->line.l3d[246], VC_RGB6(0, 2, 3) | (31u << 24));
  VCT_EQ(s->line.l3d[255], VC_RGB6(9, 2, 3) | (31u << 24));
}

VCT_TEST(p20_3d_layer_hofs_9bits) {
  VCoreState *s = vct_state();
  fill3d(s, 1);
  vct_eng16(s, VC_ENG_A, VC_REG_BG0HOFS, 0xFE00 | 10);
  vc_3d_layer_line(s, 1, &s->line);
  VCT_EQ(s->line.l3d[0], VC_RGB6(10, 2, 3) | (31u << 24));
}

/* POWCNT1 bit 15 set: engine A on the top screen; clear: on the bottom */
VCT_TEST(p20_screen_swap) {
  VCoreState *s = vct_state();
  vct_eng32(s, VC_ENG_A, VC_REG_DISPCNT, 0x10000);
  vct_eng32(s, VC_ENG_B, VC_REG_DISPCNT, 0x10000);
  vct_pal(s, 0, 0, 0, 0x001F);
  vct_pal(s, 1, 0, 0, 0x7C00);
  vct_io16(s, VC_REG_POWCNT1, 0x820F);
  vc_render_frame(s);
  VCT_EQ(s->screen[VC_SCREEN_TOP][10][10], VC_RGB6(62, 0, 0));
  VCT_EQ(s->screen[VC_SCREEN_BOTTOM][10][10], VC_RGB6(0, 0, 62));
  vct_io16(s, VC_REG_POWCNT1, 0x020F);
  vc_render_frame(s);
  VCT_EQ(s->screen[VC_SCREEN_TOP][10][10], VC_RGB6(0, 0, 62));
  VCT_EQ(s->screen[VC_SCREEN_BOTTOM][10][10], VC_RGB6(62, 0, 0));
}
