/* Enhanced sound quality (acore_set_quality, docs/acore.md "Sound quality"): the default stays the DS's exact output;
 * Enhanced interpolates PCM channel samples (a 16-tap windowed sinc) and keeps the mixer's sum at
 * 16 bits instead of the 10-bit DAC. */
#include "act.h"
#include <math.h>
#include <stdlib.h>

#define DATA 0x02100000u
#define CH(n, r) (0x04000400u + 0x10u * (n) + (r))

static AcoreState *ramp_state(int hq, u32 tmr) { /* PCM16 ramp 1000 * i on channel 0, full volume, centre */
  AcoreState *s = act_state_hw();
  acore_set_quality(s, hq);
  s16 d[64];
  for (int i = 0; i < 64; i++) d[i] = (s16)(500 * i);
  act_mem(s, DATA, d, sizeof d);
  act_reg(s, AC_REG_SOUNDCNT, 0x807F, 2);
  act_reg(s, AC_REG_SOUNDBIAS, 0x200, 2);
  act_reg(s, CH(0, AC_CH_SAD), DATA, 4);
  act_reg(s, CH(0, AC_CH_TMR), tmr, 2);
  act_reg(s, CH(0, AC_CH_PNT), 0, 2);
  act_reg(s, CH(0, AC_CH_LEN), 32, 4);
  act_reg(s, CH(0, AC_CH_CNT), 0x80000000u | (u32)AC_FMT_PCM16 << 29 | (u32)AC_REPEAT_ONESHOT << 27 | 0x40u << 16 | 0x7Fu, 4);
  return s;
}

ACT_TEST(p19_default_holds_samples) { /* TMR 0xFC00: one sample per two output samples; exact mode holds each */
  AcoreState *s = ramp_state(0, 0xFC00);
  s32 prev = -1;
  int same = 0;
  for (int k = 0; k < 20; k++) {
    s32 v = ac_ch_output(s, 0);
    same += v == prev;
    prev = v;
    ac_ch_step(s, 0);
  }
  ACT_CHECK(same >= 8);
}

ACT_TEST(p19_enhanced_interpolates_ramp) { /* a straight ramp stays straight: every output step is the same, no holds */
  AcoreState *s = ramp_state(1, 0xFC00);
  s32 v[60];
  for (int k = 0; k < 60; k++) {
    v[k] = ac_ch_output(s, 0);
    ac_ch_step(s, 0);
  }
  for (int k = 44; k < 56; k++) ACT_CHECK(v[k + 1] - v[k] > 235 && v[k + 1] - v[k] < 265); /* 500 per sample, two output steps each (after the 16 samples' warm-up) */
}

ACT_TEST(p19_enhanced_mix_keeps_low_bits) {
  s32 chout[AC_NCH] = {0}, mix[2];
  s16 ds[2], hq[2];
  AcoreState *s = act_state_hw();
  act_reg(s, AC_REG_SOUNDCNT, 0x807F, 2);
  act_reg(s, AC_REG_SOUNDBIAS, 0x200, 2);
  act_reg(s, CH(0, AC_CH_CNT), 0x7Fu | 0x40u << 16, 4);
  chout[0] = 12345;
  ac_mix(s, chout, mix, ds);
  acore_set_quality(s, 1);
  ac_mix(s, chout, mix, hq);
  ACT_CHECK(ds[0] % 64 == 0);
  ACT_CHECK(hq[0] > ds[0] - 1 && hq[0] < ds[0] + 64); /* the same value, with the six bits the DAC drops */
  ACT_CHECK(hq[0] % 64 != 0);
  chout[1] = 0;
  for (int c = 0; c < 16; c++) chout[c] = 32767; /* full scale: clips at the rails, both modes */
  for (int c = 0; c < 16; c++) act_reg(s, CH(c, AC_CH_CNT), 0x7Fu | 0x7Fu << 16, 4);
  ac_mix(s, chout, mix, hq);
  ACT_CHECK(hq[0] == 32767 || hq[1] == 32767);
}

/* the point of it: a 4 kHz tone in an 11 kHz sample, heard at the mixer's 32.7 kHz. Everything that is not the tone
 * (the least-squares sine at 4 kHz removed) relative to the tone: the DS's held samples put their images there. */
static double tone_residual_db(int hq) {
  static s16 d[2400];
  const double pi = 3.14159265358979323846, rate = 16756991.0 / 1520.0, f0 = 4000.0, fo = 33513982.0 / 1024.0;
  for (int i = 0; i < 2400; i++) d[i] = (s16)(12000.0 * sin(2.0 * pi * f0 * i / rate));
  AcoreState *s = act_state_hw();
  acore_set_quality(s, hq);
  act_mem(s, DATA, d, sizeof d);
  act_reg(s, CH(0, AC_CH_SAD), DATA, 4);
  act_reg(s, CH(0, AC_CH_TMR), 0x10000 - 1520, 2);
  act_reg(s, CH(0, AC_CH_PNT), 0, 2);
  act_reg(s, CH(0, AC_CH_LEN), 1200, 4);
  act_reg(s, CH(0, AC_CH_CNT), 0x80000000u | (u32)AC_FMT_PCM16 << 29 | (u32)AC_REPEAT_ONESHOT << 27 | 0x40u << 16 | 0x7Fu, 4);
  enum { N = 3000 };
  static double y[N];
  for (int k = 0; k < N; k++) {
    y[k] = ac_ch_output(s, 0);
    ac_ch_step(s, 0);
  }
  double sc = 0, ss = 0, cc = 0, ys = 0, yc = 0, yy = 0;
  for (int k = 400; k < 2400; k++) { /* past the warm-up, before the end */
    double a = 2.0 * pi * f0 * k / fo, sn = sin(a), cs = cos(a);
    sc += sn * cs, ss += sn * sn, cc += cs * cs, ys += y[k] * sn, yc += y[k] * cs, yy += y[k] * y[k];
  }
  double det = ss * cc - sc * sc, p = (ys * cc - yc * sc) / det, q = (yc * ss - ys * sc) / det;
  double fit = p * ys + q * yc;
  return 10.0 * log10((yy - fit) / fit);
}

ACT_TEST(p19_enhanced_images_are_gone) {
  double ds = tone_residual_db(0), hq = tone_residual_db(1);
  fprintf(stderr, "4 kHz tone in an 11 kHz sample, everything but the tone: DS %.1f dB, Enhanced %.1f dB\n", ds, hq);
  ACT_CHECK(ds > -15.0);
  ACT_CHECK(hq < -50.0);
}

/* a one-shot of constant 10000 (64 samples, two output samples each): Enhanced plays it to its end and out through the
 * interpolation's tail. Before, the channel stopped at the true end while Enhanced ran eight periods late: the last eight
 * samples were never heard and the output stepped from 10000 to 0. */
ACT_TEST(p19_enhanced_oneshot_plays_out) {
  for (int hq = 0; hq < 2; hq++) {
    AcoreState *s = act_state_hw();
    acore_set_quality(s, hq);
    s16 d[64];
    for (int i = 0; i < 64; i++) d[i] = 10000;
    act_mem(s, DATA, d, sizeof d);
    act_reg(s, CH(0, AC_CH_SAD), DATA, 4);
    act_reg(s, CH(0, AC_CH_TMR), 0xFC00, 2);
    act_reg(s, CH(0, AC_CH_PNT), 0, 2);
    act_reg(s, CH(0, AC_CH_LEN), 32, 4);
    act_reg(s, CH(0, AC_CH_CNT), 0x80000000u | (u32)AC_FMT_PCM16 << 29 | (u32)AC_REPEAT_ONESHOT << 27 | 0x7Fu, 4);
    int high = 0;
    s32 prev = 0, step = 0;
    for (int k = 0; k < 220; k++) {
      s32 v = ac_ch_output(s, 0);
      high += v > 5000;
      if (k > 60 && abs(v - prev) > step) step = abs(v - prev); /* the biggest step after the start */
      prev = v;
      ac_ch_step(s, 0);
    }
    fprintf(stderr, "one-shot of 64 samples (128 output samples), %s: %d above half, biggest step at the end %d\n",
            hq ? "Enhanced" : "DS", high, (int)step);
    ACT_CHECK(high >= 124 && high <= 132); /* 64 samples x 2, both qualities */
    if (hq) {
      ACT_CHECK(step < 6000); /* a band-limited end (about 10000 / 2.2 a sample here), not the step to 0 */
      ACT_CHECK(prev == 0);   /* and silent after it */
    }
  }
}

/* a quality switch in the middle of a note (a looping 64-sample sine). Before, Enhanced's history was kept only in Enhanced,
 * so DS -> Enhanced interpolated over zeros (a dropout and two clicks) and Enhanced -> DS jumped eight periods. */
static int max_step(const AcoreState *s, u64 from, u64 to) {
  int m = 0;
  for (u64 k = from; k < to; k++) {
    int d = abs(act_sample(s, k + 1, 0) - act_sample(s, k, 0));
    m = d > m ? d : m;
  }
  return m;
}

static int switch_ok(int from) {
  AcoreState *s = act_state_hw();
  acore_set_quality(s, from);
  s16 d[64];
  for (int i = 0; i < 64; i++) d[i] = (s16)lrint(20000.0 * sin(2.0 * 3.14159265358979 * i / 64.0));
  act_mem(s, DATA, d, sizeof d);
  act_reg(s, AC_REG_SOUNDCNT, 0x807F, 2);
  act_reg(s, AC_REG_SOUNDBIAS, 0x200, 2);
  act_reg(s, CH(0, AC_CH_SAD), DATA, 4);
  act_reg(s, CH(0, AC_CH_TMR), 0xFC00, 2);
  act_reg(s, CH(0, AC_CH_PNT), 0, 2);
  act_reg(s, CH(0, AC_CH_LEN), 32, 4);
  act_reg(s, CH(0, AC_CH_CNT), 0x80000000u | (u32)AC_FMT_PCM16 << 29 | (u32)AC_REPEAT_LOOP << 27 | 0x40u << 16 | 0x7Fu, 4);
  act_run(s, 1000 * (u64)AC_CYCLES_PER_SAMPLE);
  acore_set_quality(s, !from);
  act_run(s, 1000 * (u64)AC_CYCLES_PER_SAMPLE);
  int before = max_step(s, 500, 990), after = max_step(s, 1400, 1990), at = max_step(s, 990, 1300);
  before = after > before ? after : before; /* the bigger of the two qualities' own steps (DS holds samples: bigger) */
  fprintf(stderr, "quality switch %s mid-note: biggest step before %d, at the switch %d\n", from ? "Enhanced -> DS" : "DS -> Enhanced",
          before, at);
  return at <= before * 3 / 2 + 64;
}

ACT_TEST(p19_quality_switch_mid_note) {
  ACT_CHECK(switch_ok(0));
  ACT_CHECK(switch_ok(1));
}
