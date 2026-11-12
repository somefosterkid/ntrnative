/* port/vcore piece 8's unit tests: windows (docs/vcore.md, piece 8). Expected values worked by hand from GBATEK 07. */
#include "vct.h"

#define A 0
static VCoreState *mk(u32 dispcnt) {
  VCoreState *s = vct_state();
  vct_eng32(s, A, VC_REG_DISPCNT, dispcnt);
  return s;
}
static void run(VCoreState *s, int y) {
  for (int x = 0; x < VC_W; x++) s->line.winmask[x] = 0x3F;
  vc_window_line(s, A, y, &s->line);
}

VCT_TEST(p08_off_keeps_preset) {
  VCoreState *s = mk(0x1F00);
  vct_eng16(s, A, VC_REG_WININ, 0x0101);
  vct_eng16(s, A, VC_REG_WIN0H, 0x1020); vct_eng16(s, A, VC_REG_WIN0V, 0x1020);
  vc_window_line(s, A, 0x18, &s->line);
  for (int x = 0; x < VC_W; x++) VCT_EQ(s->line.winmask[x], 0x3F);
}
VCT_TEST(p08_win0_alone) {
  VCoreState *s = mk(0x2000);
  vct_eng16(s, A, VC_REG_WIN0H, 0x1020); /* x 16..31 */
  vct_eng16(s, A, VC_REG_WIN0V, 0x0A14); /* y 10..19 */
  vct_eng16(s, A, VC_REG_WININ, 0x0015);
  vct_eng16(s, A, VC_REG_WINOUT, 0x0002);
  run(s, 10);
  VCT_EQ(s->line.winmask[15], 2); VCT_EQ(s->line.winmask[16], 0x15);
  VCT_EQ(s->line.winmask[31], 0x15); VCT_EQ(s->line.winmask[32], 2);
  run(s, 19); VCT_EQ(s->line.winmask[20], 0x15);
  run(s, 20); VCT_EQ(s->line.winmask[20], 2);
  run(s, 9); VCT_EQ(s->line.winmask[20], 2);
  vct_emit(s, "p08_win0");
}
VCT_TEST(p08_win1_alone) {
  VCoreState *s = mk(0x4000);
  vct_eng16(s, A, VC_REG_WIN0H, 0x00FF); vct_eng16(s, A, VC_REG_WIN0V, 0x00C0);
  vct_eng16(s, A, VC_REG_WIN1H, 0x4080); vct_eng16(s, A, VC_REG_WIN1V, 0x2060);
  vct_eng16(s, A, VC_REG_WININ, 0x3C15);
  vct_eng16(s, A, VC_REG_WINOUT, 0x0001);
  run(s, 0x30);
  VCT_EQ(s->line.winmask[0x3F], 1); VCT_EQ(s->line.winmask[0x40], 0x3C);
  VCT_EQ(s->line.winmask[0x7F], 0x3C); VCT_EQ(s->line.winmask[0x80], 1);
  run(s, 0x60); VCT_EQ(s->line.winmask[0x50], 1);
  vct_emit(s, "p08_win1");
}
VCT_TEST(p08_priority_win0_over_win1_over_obj) {
  VCoreState *s = mk(0xE000);
  vct_eng16(s, A, VC_REG_WIN0H, 0x1030); vct_eng16(s, A, VC_REG_WIN0V, 0x0020);
  vct_eng16(s, A, VC_REG_WIN1H, 0x2050); vct_eng16(s, A, VC_REG_WIN1V, 0x0020);
  vct_eng16(s, A, VC_REG_WININ, 0x0A01);
  vct_eng16(s, A, VC_REG_WINOUT, 0x1F04);
  for (int x = 0; x < VC_W; x++) s->line.objwin[x] = (x >= 0x08 && x < 0x60);
  vc_window_line(s, A, 5, &s->line);
  VCT_EQ(s->line.winmask[0x07], 4);      /* outside */
  VCT_EQ(s->line.winmask[0x08], 0x1F);   /* obj window only */
  VCT_EQ(s->line.winmask[0x10], 0x01);   /* win0 beats win1 and obj */
  VCT_EQ(s->line.winmask[0x2F], 0x01);
  VCT_EQ(s->line.winmask[0x30], 0x0A);   /* win1 beyond win0 */
  VCT_EQ(s->line.winmask[0x4F], 0x0A);
  VCT_EQ(s->line.winmask[0x50], 0x1F);   /* obj window beyond win1 */
  VCT_EQ(s->line.winmask[0x60], 4);
  vct_emit(s, "p08_priority");
}
VCT_TEST(p08_objwin_needs_bit15) {
  VCoreState *s = mk(0x4000);
  vct_eng16(s, A, VC_REG_WINOUT, 0x3F02);
  s->line.objwin[5] = 1;
  vc_window_line(s, A, 0, &s->line);
  VCT_EQ(s->line.winmask[5], 2);
  s = mk(0x8000);
  vct_eng16(s, A, VC_REG_WINOUT, 0x3F02);
  s->line.objwin[5] = 1;
  vc_window_line(s, A, 0, &s->line);
  VCT_EQ(s->line.winmask[5], 0x3F); VCT_EQ(s->line.winmask[6], 2);
}
VCT_TEST(p08_empty_window) {
  VCoreState *s = mk(0x2000);
  vct_eng16(s, A, VC_REG_WIN0H, 0x2020); vct_eng16(s, A, VC_REG_WIN0V, 0x0010);
  vct_eng16(s, A, VC_REG_WININ, 0x001F); vct_eng16(s, A, VC_REG_WINOUT, 0x0001);
  run(s, 0);
  for (int x = 0; x < VC_W; x++) VCT_EQ(s->line.winmask[x], 1);
  vct_eng16(s, A, VC_REG_WIN0H, 0x0010); vct_eng16(s, A, VC_REG_WIN0V, 0x2020); /* y1 == y2 */
  run(s, 0x20); VCT_EQ(s->line.winmask[3], 1); VCT_EQ(s->line.winmask[0x0F], 1);
  vct_emit(s, "p08_empty");
}
VCT_TEST(p08_wrap_x) {
  VCoreState *s = mk(0x2000);
  vct_eng16(s, A, VC_REG_WIN0H, 0xC040); /* x1=192 > x2=64 */
  vct_eng16(s, A, VC_REG_WIN0V, 0x00C0);
  vct_eng16(s, A, VC_REG_WININ, 0x0010); vct_eng16(s, A, VC_REG_WINOUT, 0x0001);
  run(s, 3);
  VCT_EQ(s->line.winmask[0], 0x10); VCT_EQ(s->line.winmask[63], 0x10);
  VCT_EQ(s->line.winmask[64], 1); VCT_EQ(s->line.winmask[191], 1);
  VCT_EQ(s->line.winmask[192], 0x10); VCT_EQ(s->line.winmask[255], 0x10);
  vct_emit(s, "p08_wrapx");
}
VCT_TEST(p08_wrap_y) {
  VCoreState *s = mk(0x2000);
  vct_eng16(s, A, VC_REG_WIN0H, 0x00FF);
  vct_eng16(s, A, VC_REG_WIN0V, 0xA010); /* y1=160 > y2=16 */
  vct_eng16(s, A, VC_REG_WININ, 0x0010); vct_eng16(s, A, VC_REG_WINOUT, 0x0001);
  run(s, 5); VCT_EQ(s->line.winmask[10], 0x10);
  run(s, 16); VCT_EQ(s->line.winmask[10], 1);
  run(s, 159); VCT_EQ(s->line.winmask[10], 1);
  run(s, 160); VCT_EQ(s->line.winmask[10], 0x10);
  run(s, 191); VCT_EQ(s->line.winmask[10], 0x10);
  vct_emit(s, "p08_wrapy");
}
VCT_TEST(p08_effect_bit) {
  VCoreState *s = mk(0xA000);
  vct_eng16(s, A, VC_REG_WININ, 0x0000); vct_eng16(s, A, VC_REG_WINOUT, 0x2010);
  s->line.objwin[1] = 1;
  vc_window_line(s, A, 0, &s->line);
  VCT_EQ(s->line.winmask[0], 0x10);
  VCT_EQ(s->line.winmask[1], 0x20);
  vct_eng16(s, A, VC_REG_WINOUT, 0x3F20); s->line.objwin[1] = 1;
  vc_window_line(s, A, 0, &s->line);
  VCT_EQ(s->line.winmask[0], 0x20); VCT_EQ(s->line.winmask[1], 0x3F);
}
VCT_TEST(p08_engine_b) {
  VCoreState *s = vct_state();
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x2000);
  vct_eng16(s, 1, VC_REG_WIN0H, 0x0810); vct_eng16(s, 1, VC_REG_WIN0V, 0x00FF);
  vct_eng16(s, 1, VC_REG_WININ, 0x0004); vct_eng16(s, 1, VC_REG_WINOUT, 0x0008);
  vc_window_line(s, 1, 2, &s->line);
  VCT_EQ(s->line.winmask[7], 8); VCT_EQ(s->line.winmask[8], 4); VCT_EQ(s->line.winmask[16], 8);
}
/* the oracle's Y flag entering line 0 (fitted; GBATEK has no rule): events ordered 7..262, 0..6 */
static VCoreState *ys;
static int yon(u16 v, int y) {
  VCoreState *s = ys ? ys : (ys = mk(0x2000));
  vct_eng16(s, A, VC_REG_WIN0H, 0x00FF); vct_eng16(s, A, VC_REG_WIN0V, v);
  vct_eng16(s, A, VC_REG_WININ, 0x0010); vct_eng16(s, A, VC_REG_WINOUT, 0x0001);
  run(s, y);
  return s->line.winmask[10] == 0x10;
}
VCT_TEST(p08_y_flag_line0) {
  ys = 0;
  VCT_EQ(yon(0xBE02, 0), 0); VCT_EQ(yon(0xBE02, 1), 0); VCT_EQ(yon(0xBE02, 190), 1); /* 190 > 2: no wrap onto 0 */
  VCT_EQ(yon(0x0507, 0), 1); VCT_EQ(yon(0x0507, 6), 1); VCT_EQ(yon(0x0507, 7), 0);   /* 5..7 starts on at 0 */
  VCT_EQ(yon(0x3205, 0), 0); VCT_EQ(yon(0x3209, 0), 1); VCT_EQ(yon(0x3209, 9), 0);   /* 50 > b: wraps if b >= 7 */
  VCT_EQ(yon(0x0705, 0), 0); VCT_EQ(yon(0x0205, 0), 0); VCT_EQ(yon(0x0502, 0), 1);
  VCT_EQ(yon(0x0502, 2), 0); VCT_EQ(yon(0x0502, 5), 1);
  VCT_EQ(yon(0x0A0A, 0), 0); VCT_EQ(yon(0x0A0A, 10), 0); VCT_EQ(yon(0x0A0A, 50), 0);
}
