/* port/acore piece 4: unit tests (docs/acore.md, piece 4's "Tests:"). OWNED BY PIECE 4.
 * They call ac_mix directly with hand-made channel outputs and register values (no channel is started), the
 * expected values worked by hand from GBATEK 58/60/61. The p04_* scenes also emit streams for the oracle: constant
 * PCM16 loops on the real channels (ACT_EMIT=DIR). */
#include "act.h"
#include "ac_shared.h"

#define CNT(vol, div, pan) ((u32)(vol) | (u32)(div) << 8 | (u32)(pan) << 16) /* no start bit: the channel stays off */
#define SC 0x04000500u
#define SB 0x04000504u

static AcoreState *setup(int master, int selbits, int bias) {
  AcoreState *s = act_state_hw();
  act_reg(s, SC, 0x8000u | (u32)selbits | (u32)master, 2);
  act_reg(s, SB, (u32)bias, 2);
  return s;
}
static void chcnt(AcoreState *s, int ch, u32 cnt) { act_reg(s, 0x04000400u + (u32)ch * 16, cnt, 4); }
static int dac(AcoreState *s, const s32 *chout, int side, s32 *mix) {
  s32 m[2];
  s16 out[2];
  ac_mix(s, chout, mix ? mix : m, out);
  return act_dac(out[side]);
}

ACT_TEST(p04_full_left_right) {
  AcoreState *s = setup(127, 0, 0x200);
  s32 c[AC_NCH] = {0};
  s32 mix[2];
  c[0] = 0x7FFF;
  chcnt(s, 0, CNT(127, 0, 0)); /* pan 0 = all left */
  ACT_EQ(dac(s, c, 0, mix), 0x3FF);
  ACT_EQ(dac(s, c, 1, NULL), 0x200);
  ACT_EQ(mix[0], 0x7FFF00); /* 20.8 */
  ACT_EQ(mix[1], 0);
  chcnt(s, 0, CNT(127, 0, 127)); /* pan 127 = N 128 = all right */
  ACT_EQ(dac(s, c, 0, NULL), 0x200);
  ACT_EQ(dac(s, c, 1, mix), 0x3FF);
  ACT_EQ(mix[1], 0x7FFF00);
  c[0] = -0x8000; /* the low end: -0x200 -> 0 */
  ACT_EQ(dac(s, c, 1, NULL), 0);
  s16 out[2];
  ac_mix(s, c, mix, out);
  ACT_EQ(out[1], -0x8000);
  ACT_EQ(out[0], 0);
}

ACT_TEST(p04_centre_pan) {
  AcoreState *s = setup(127, 0, 0x200);
  s32 c[AC_NCH] = {0}, mix[2];
  c[0] = 0x4000;
  chcnt(s, 0, CNT(127, 0, 64)); /* 0x4000 * 128/128 * 64/128 = 0x2000 each side */
  ACT_EQ(dac(s, c, 0, mix), 0x200 + 0x80); /* 0x200000 * 128 >> 21 */
  ACT_EQ(mix[0], 0x200000);
  ACT_EQ(mix[1], 0x200000);
}

ACT_TEST(p04_dividers) {
  /* data 0x4000, vol 127 (128), pan 0: 16.8 = 0x4000 >> shift, << 8; master 128: >> 14 -> v */
  static const int shift[4] = {0, 1, 2, 4};
  for (int d = 0; d < 4; d++) {
    AcoreState *s = setup(127, 0, 0x200);
    s32 c[AC_NCH] = {0}, mix[2];
    c[0] = 0x4000;
    chcnt(s, 0, CNT(127, d, 0));
    int v = dac(s, c, 0, mix);
    ACT_EQ(mix[0], (0x4000 >> shift[d]) << 8);
    ACT_EQ(v, 0x200 + ((0x4000 >> shift[d]) >> 6));
  }
  /* the fraction is kept to the end of the channel chain, floored there: 3 / 16 * 1 * (1/2) -> 16.8 floor */
  AcoreState *s = setup(127, 0, 0x200);
  s32 c[AC_NCH] = {0}, mix[2];
  c[0] = 3;
  chcnt(s, 0, CNT(127, 3, 64)); /* 3*256/16/2 = 24 */
  dac(s, c, 0, mix);
  ACT_EQ(mix[0], 24);
  c[0] = -3;
  dac(s, c, 0, mix);
  ACT_EQ(mix[0], -24);
  c[0] = 0x4000;
  chcnt(s, 0, CNT(1, 3, 1)); /* vol 1/128, pan 1: left 127/128; 16.4 = 0x4000, * 1 * 127 >> 10 */
  dac(s, c, 0, mix);
  ACT_EQ(mix[0], (0x4000 * 127) >> 10);
}

ACT_TEST(p04_volume_scale) {
  AcoreState *s = setup(127, 0, 0x200);
  s32 c[AC_NCH] = {0}, mix[2];
  c[0] = 0x4000;
  chcnt(s, 0, CNT(64, 0, 0));
  dac(s, c, 0, mix);
  ACT_EQ(mix[0], 0x2000 << 8);
  chcnt(s, 0, CNT(126, 0, 0)); /* 126/128 */
  dac(s, c, 0, mix);
  ACT_EQ(mix[0], (0x4000 * 126 / 128) << 8);
  chcnt(s, 0, CNT(0, 0, 0));
  dac(s, c, 0, mix);
  ACT_EQ(mix[0], 0);
}

ACT_TEST(p04_master_volume) {
  s32 c[AC_NCH] = {0};
  c[0] = 0x7000; /* 16.8 0x700000 with vol 127, pan 0 */
  AcoreState *s = setup(0, 0, 0x200);
  chcnt(s, 0, CNT(127, 0, 0));
  ACT_EQ(dac(s, c, 0, NULL), 0x200); /* master 0: silent */
  s = setup(64, 0, 0x200);
  chcnt(s, 0, CNT(127, 0, 0));
  ACT_EQ(dac(s, c, 0, NULL), 0x200 + (0x700000 * 64 >> 21)); /* +0xE0 */
  s = setup(127, 0, 0x200);
  chcnt(s, 0, CNT(127, 0, 0));
  ACT_EQ(dac(s, c, 0, NULL), 0x200 + (0x700000 * 128 >> 21)); /* 127 = 128: +0x1C0 */
  s = setup(126, 0, 0x200);
  chcnt(s, 0, CNT(127, 0, 0));
  ACT_EQ(dac(s, c, 0, NULL), 0x200 + (0x700000 * 126 >> 21));
  /* negative values floor */
  c[0] = -1;
  s = setup(127, 0, 0x200);
  chcnt(s, 0, CNT(127, 0, 0));
  ACT_EQ(dac(s, c, 0, NULL), 0x1FF); /* -256*128 >> 21 = -1 */
}

ACT_TEST(p04_master_enable) {
  AcoreState *s = setup(127, 0, 0x155);
  s32 c[AC_NCH] = {0};
  c[0] = 0x7FFF;
  chcnt(s, 0, CNT(127, 0, 0));
  ACT_EQ(dac(s, c, 0, NULL), 0x354);
  act_reg(s, SC, 0x007F, 2); /* bit 15 off: the bias alone */
  ACT_EQ(dac(s, c, 0, NULL), 0x155);
  ACT_EQ(dac(s, c, 1, NULL), 0x155);
}

ACT_TEST(p04_clip_16_channels) {
  AcoreState *s = setup(127, 0, 0x200);
  s32 c[AC_NCH], mix[2];
  for (int i = 0; i < AC_NCH; i++) c[i] = 0x7FFF, chcnt(s, i, CNT(127, 0, 64));
  ACT_EQ(dac(s, c, 0, mix), 0x3FF);
  ACT_EQ(mix[0], 16 * (0x7FFF00 / 2)); /* 20.8 sum, not clipped */
  for (int i = 0; i < AC_NCH; i++) c[i] = -0x8000;
  ACT_EQ(dac(s, c, 1, mix), 0);
  ACT_EQ(mix[1], -16 * 0x400000);
  for (int i = 0; i < AC_NCH; i++) chcnt(s, i, CNT(127, 0, 0)), c[i] = 0x7FFF;
  ACT_EQ(dac(s, c, 1, NULL), 0x200); /* nothing on the right */
  /* 8 channels at full: 8 * 0x1FF >> .. = 0xFF8-ish: clipped at 3FF with bias 200 */
  for (int i = 0; i < AC_NCH; i++) c[i] = i < 1 ? 0x7FFF : 0;
  ACT_EQ(dac(s, c, 0, NULL), 0x3FF);
}

ACT_TEST(p04_output_selectors) {
  AcoreState *s;
  s32 c[AC_NCH] = {0}, mix[2];
  /* ch0 left 0x2000 (v 0x80 : 0x200000 >>14 -> 0x80... ), ch1 left 0x1000, ch3 left 0x0800, all pan 0, vol 127 */
  c[0] = 0x2000, c[1] = 0x1000, c[3] = 0x0800;
#define SETUP(sel)                                                                                                    \
  s = setup(127, sel, 0x200);                                                                                         \
  for (int i = 0; i < 4; i++) chcnt(s, i, CNT(127, 0, 0))
  /* v = sum8 >> 14 where sum8 = data << 8 -> data >> 6 */
  SETUP(0);
  ACT_EQ(dac(s, c, 0, mix), 0x200 + ((0x2000 + 0x1000 + 0x800) >> 6));
  ACT_EQ(mix[0], (0x2000 + 0x1000 + 0x800) << 8);
  SETUP(1 << 8);
  ACT_EQ(dac(s, c, 0, NULL), 0x200 + (0x1000 >> 6)); /* left from ch1 */
  SETUP(2 << 8);
  ACT_EQ(dac(s, c, 0, NULL), 0x200 + (0x800 >> 6));
  SETUP(3 << 8);
  ACT_EQ(dac(s, c, 0, NULL), 0x200 + ((0x1000 + 0x800) >> 6));
  /* bits 12/13: ch1 / ch3 not to the mixer */
  SETUP(1 << 12);
  ACT_EQ(dac(s, c, 0, mix), 0x200 + ((0x2000 + 0x800) >> 6));
  ACT_EQ(mix[0], (0x2000 + 0x800) << 8);
  SETUP(2 << 12);
  ACT_EQ(dac(s, c, 0, mix), 0x200 + ((0x2000 + 0x1000) >> 6));
  SETUP(3 << 12);
  ACT_EQ(dac(s, c, 0, NULL), 0x200 + (0x2000 >> 6));
  /* ch1 only to the left output, kept out of the mixer; the right output (selector 0) is the right mixer */
  SETUP((1 << 8) | (1 << 12) | (3 << 10));
  ACT_EQ(dac(s, c, 0, NULL), 0x200 + (0x1000 >> 6));
  ACT_EQ(dac(s, c, 1, NULL), 0x200); /* right: ch1+ch3 right = 0 (pan 0) */
  /* the selected channel goes through its own pan: pan 127 sends ch1 right */
  SETUP(1 << 10);
  chcnt(s, 1, CNT(127, 0, 127));
  ACT_EQ(dac(s, c, 1, NULL), 0x200 + (0x1000 >> 6));
  ACT_EQ(dac(s, c, 0, NULL), 0x200 + ((0x2000 + 0x800) >> 6));
}

ACT_TEST(p04_bias) {
  s32 c[AC_NCH] = {0};
  c[0] = 0x7FFF;
  AcoreState *s = setup(127, 0, 0);
  chcnt(s, 0, CNT(127, 0, 0));
  s16 out[2];
  s32 mix[2];
  ac_mix(s, c, mix, out);
  ACT_EQ(act_dac(out[0]), 0x1FF); /* bias 0: the signal's +1FF, the negative half is clipped to 0 */
  ACT_EQ(act_dac(out[1]), 0);
  ACT_EQ(out[1], -0x8000);
  c[0] = -0x8000;
  ac_mix(s, c, mix, out);
  ACT_EQ(act_dac(out[0]), 0);
  s = setup(127, 0, 0x3FF);
  chcnt(s, 0, CNT(127, 0, 0));
  c[0] = 0x7FFF;
  ac_mix(s, c, mix, out);
  ACT_EQ(act_dac(out[0]), 0x3FF);
  ACT_EQ(out[0], 0x7FC0);
  ACT_EQ(act_dac(out[1]), 0x3FF); /* silence on the right = bias 3FF */
  c[0] = -0x8000;
  ac_mix(s, c, mix, out);
  ACT_EQ(act_dac(out[0]), 0x1FF);
  s = setup(127, 0, 0x7FF); /* only bits 0-9 count */
  ac_mix(s, c, mix, out);
  ACT_EQ(act_dac(out[1]), 0x3FF);
}

/* ---- scenes for the oracle: constant PCM16 loops on real channels (they need piece 1 in acore) ---- */
static void const_ch(AcoreState *s, int ch, u32 addr, s16 v, u32 cnt_flags) {
  s16 buf[16];
  for (int i = 0; i < 16; i++) buf[i] = v;
  act_mem(s, addr, buf, sizeof buf);
  u32 base = 0x04000400u + (u32)ch * 16;
  act_reg(s, base + AC_CH_SAD, addr, 4);
  act_reg(s, base + AC_CH_TMR, 0xFE00, 2);
  act_reg(s, base + AC_CH_PNT, 0, 2);
  act_reg(s, base + AC_CH_LEN, 8, 4);
  act_reg(s, base + AC_CH_CNT, 0x80000000u | (1u << 29) | (1u << 27) | cnt_flags, 4); /* PCM16 loop */
}

static void scene(const char *name, int master, int selbits, int bias, int n, const s16 *vals, const u32 *flags) {
  AcoreState *s = act_state_hw();
  act_reg(s, SC, 0x8000u | (u32)selbits | (u32)master, 2);
  act_reg(s, SB, (u32)bias, 2);
  for (int i = 0; i < n; i++) const_ch(s, i, 0x02100000u + 0x100u * (u32)i, vals[i], flags[i]);
  act_run(s, 40 * AC_CYCLES_PER_SAMPLE);
  act_emit(s, name);
}

ACT_TEST(p04_emit_scenes) {
  s16 v[16];
  u32 f[16];
  /* pan, volume, dividers on four channels, bias 0x200, master 127 and 64 */
  for (int i = 0; i < 4; i++) v[i] = (s16)(0x6000 - 0x1800 * i), f[i] = (u32)(60 + 5 * i) | (u32)i << 8 | (u32)(i * 40) << 16;
  scene("p04_mix4", 127, 0, 0x200, 4, v, f);
  scene("p04_mix4_m64", 64, 0, 0x200, 4, v, f);
  scene("p04_mix4_m1", 1, 0, 0x155, 4, v, f);
  scene("p04_mix4_sel3", 127, (3 << 8) | (2 << 10) | (1 << 12), 0x200, 4, v, f);
  scene("p04_mix4_sel2", 127, (2 << 8) | (1 << 10) | (2 << 12), 0x200, 4, v, f);
  scene("p04_mix4_b0", 127, 0, 0, 4, v, f);
  scene("p04_mix4_b3ff", 127, 0, 0x3FF, 4, v, f);
  for (int i = 0; i < 16; i++) v[i] = (i & 1) ? (s16)0x7FFF : (s16)-0x8000, f[i] = 127u | (u32)(i * 8) << 16;
  scene("p04_mix16", 127, 0, 0x200, 16, v, f);
  for (int i = 0; i < 16; i++) v[i] = 0x7FFF, f[i] = 127u | 64u << 16;
  scene("p04_clip16", 127, 0, 0x200, 16, v, f);
  for (int i = 0; i < 16; i++) v[i] = (s16)(-0x8000), f[i] = 127u | 64u << 16;
  scene("p04_clip16_neg", 127, 0, 0x200, 16, v, f);
  for (int i = 0; i < 16; i++) v[i] = (s16)(i * 997 - 7000), f[i] = (u32)(i * 8 + 3) | (u32)(i & 3) << 8 | (u32)(127 - i * 8) << 16;
  scene("p04_mixodd", 100, 0, 0x200, 16, v, f);
  {
    AcoreState *s = act_state_hw();
    act_reg(s, SC, 0x007F, 2); /* master disabled: the bias alone */
    act_reg(s, SB, 0x1A0, 2);
    for (int i = 0; i < 4; i++) const_ch(s, i, 0x02100000u + 0x100u * (u32)i, v[i], f[i]);
    act_run(s, 40 * AC_CYCLES_PER_SAMPLE);
    act_emit(s, "p04_disabled");
  }
  scene("p04_mixodd_sel", 100, (3 << 8) | (1 << 10) | (1 << 12) | (1 << 13), 0x200, 16, v, f);
}
