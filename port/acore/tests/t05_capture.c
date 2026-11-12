/* port/acore piece 5: unit tests (docs/acore.md, piece 5 Tests:). OWNED BY PIECE 5.
 *
 * The tests feed ac_cap_sample by hand (the mixer of piece 4 and the channels of piece 1 are not needed): step k is
 * at cycle k * 1024, the mixer value of a step is given in 20.8 (value << 8). The tables marked ORACLE are what the
 * oracle (aoracle, mode=hw) put in memory for the same scenario: a PCM16 ramp 100, 200, ... on channel 0 (left, vol
 * 127, TMR FE00, started at cycle 1000: its first sample 100 is in the mixer at step 3, +100 per step), capture 0
 * started at cycle 3000 on the timer of channel 1, DAD 0x02200000. The scenarios are emitted as streams (ACT_EMIT)
 * for acore_render vs aoracle once pieces 1 and 4 are in.
 */
#include "act.h"
#include <string.h>

#define DST 0x02200000u

static const u32 CNT0 = 0x04000508u, CNT1 = 0x04000509u, DAD0 = 0x04000510u, LEN0 = 0x04000514u;
static const u32 DAD1 = 0x04000518u, LEN1 = 0x0400051Cu, SOUNDCNT = 0x04000500u;
static const u32 TMR1 = 0x04000418u, TMR3 = 0x04000438u;

/* step k at k * 1024, with the mixer (value << 8) of the two sides */
static void step(AcoreState *s, u64 k, s32 l, s32 r) {
  if (k < 3) return; /* the captures of the scenarios start at cycle 3000 */
  s32 mix[2] = {l * 256, r * 256}, ch[AC_NCH] = {0};
  s->now = k * 1024;
  ac_cap_sample(s, mix, ch);
}
static void step_raw(AcoreState *s, u64 k, s32 l20_8, s32 r20_8, const s32 *chout) {
  s32 mix[2] = {l20_8, r20_8}, ch[AC_NCH] = {0};
  if (chout) memcpy(ch, chout, sizeof ch);
  s->now = k * 1024;
  ac_cap_sample(s, mix, ch);
}
/* the ramp of the scenario: 100 * (k - 2) from step 3 */
static s32 ramp(u64 k) { return k >= 3 ? (s32)(100 * (k - 2)) : 0; }
static void at(AcoreState *s, u64 t) { s->now = t; }

static AcoreState *setup(u32 tmr, u32 len_words, u32 cnt) {
  AcoreState *s = act_state_hw();
  u8 fill[0x100];
  memset(fill, 0xAA, sizeof fill);
  at(s, 0);
  act_mem(s, DST, fill, sizeof fill);
  act_reg(s, SOUNDCNT, 0x8000 | 127, 2);
  act_reg(s, TMR1, tmr, 2);
  act_reg(s, DAD0, DST, 4);
  act_reg(s, LEN0, len_words, 2);
  at(s, 3000);
  act_reg(s, CNT0, cnt, 1);
  return s;
}
static s16 m16(AcoreState *s, int i, u32 ofs) { return (s16)ac_rd16(s, DST + ofs + 2 * (u32)i); }
static int m8(AcoreState *s, int i) { return (s8)ac_rd8(s, DST + (u32)i); }
#define UNTOUCHED ((s16)0xAAAA)

ACT_TEST(p05_pcm16_const_mixer) {
  AcoreState *s = setup(0xFE00, 2, 0x84); /* one-shot, PCM16, mixer, 2 words = 4 samples */
  for (u64 k = 0; k < 12; k++) step(s, k, 1234, -77);
  for (int i = 0; i < 4; i++) ACT_EQ(m16(s, i, 0), 1234);
  ACT_EQ(m16(s, 4, 0), UNTOUCHED); /* one-shot: nothing after 2 words */
  ACT_EQ(ac_hw_read(s, 0x508, 1) & 0x80, 0); /* the busy bit is clear */
  act_emit(s, "p05_pcm16_const");
}

ACT_TEST(p05_right_unit) {
  AcoreState *s = act_state_hw();
  at(s, 0);
  act_reg(s, SOUNDCNT, 0x8000 | 127, 2);
  act_reg(s, TMR3, 0xFE00, 2);
  act_reg(s, DAD1, DST, 4);
  act_reg(s, LEN1, 1, 2);
  act_reg(s, CNT1, 0x84, 1);
  for (u64 k = 0; k < 4; k++) step_raw(s, k, 500 * 256, -300 * 256, NULL); /* timer of channel 3, the right mixer */
  ACT_EQ(m16(s, 0, 0), -300);
  ACT_EQ(m16(s, 1, 0), -300);
  ACT_EQ(m16(s, 2, 0), 0);
  ACT_EQ(ac_hw_read(s, 0x509, 1) & 0x80, 0);
  /* the timer of channel 1 does not drive unit 1 */
  AcoreState *t = act_state_hw();
  act_reg(t, SOUNDCNT, 0x8000 | 127, 2);
  act_reg(t, TMR1, 0xFE00, 2); /* TMR3 stays 0: a period of 65536 ticks */
  act_reg(t, DAD1, DST, 4);
  act_reg(t, LEN1, 1, 2);
  act_reg(t, CNT1, 0x84, 1);
  for (u64 k = 0; k < 20; k++) step_raw(t, k, 500 * 256, -300 * 256, NULL);
  ACT_EQ(m16(t, 0, 0), 0);
}

ACT_TEST(p05_mixer_clip_and_floor) {
  /* the mixer (20.8) is clipped to -0x8000..0x7FFF; PCM16 is the integer part, floored (ORACLE: 4 channels of 32767
   * give 32767, of -32768 give -32768; a sweep of all 65536 values: x >> 8 of the 16.8 value) */
  AcoreState *s = setup(0xFE00, 4, 0x84);
  s32 v[8] = {40000 * 256, -50000 * 256, 0x7FFFFF, -0x800000, 0x1234F8, -(16 * 256 + 128), -1, 255};
  for (u64 k = 3; k < 3 + 8; k++) step_raw(s, k, v[k - 3], 0, NULL);
  s16 want[8] = {32767, -32768, 32767, -32768, 0x1234, -17, -1, 0};
  for (int i = 0; i < 8; i++) ACT_EQ(m16(s, i, 0), want[i]);
}

ACT_TEST(p05_pcm8_rounding) {
  /* PCM8 = the integer part >> 8, floor: ORACLE (all 65536 values: pure floor; GBATEK 61 rounds some negatives toward
   * zero, the oracle does not: p05_capture.c, AC_CAP_PCM8_ROUND). Four samples to a word, low byte first. */
  AcoreState *s = setup(0xFE00, 4, 0x8C); /* one-shot, PCM8 */
  static const s32 v[16] = {0, 100, 255, 256, 300, 511, -1, -128, -129, -256, -257, 0x7FFF, -0x8000, 40000, -40000, 1};
  static const int w[16] = {0, 0, 0, 1, 1, 1, -1, -1, -1, -1, -2, 127, -128, 127, -128, 0};
  for (u64 k = 3; k < 3 + 16; k++) step(s, k, v[k - 3], 0);
  for (int i = 0; i < 16; i++) ACT_EQ(m8(s, i), w[i]);
  ACT_EQ(ac_rd8(s, DST + 16), 0xAA);
  /* a fraction does not matter: 0xFF.FF gives 255 gives 0; -0x00.01 gives -1 */
  AcoreState *t = setup(0xFE00, 1, 0x8C);
  step_raw(t, 3, 0xFFFF, 0, NULL);
  step_raw(t, 4, -1, 0, NULL);
  step_raw(t, 5, 0x7FFFFF, 0, NULL);
  step_raw(t, 6, 0x80, 0, NULL);
  ACT_EQ(m8(t, 0), 0);
  ACT_EQ(m8(t, 1), -1);
  ACT_EQ(m8(t, 2), 127);
  ACT_EQ(m8(t, 3), 0);
  act_emit(s, "p05_pcm8");
}

ACT_TEST(p05_timer_rates) {
  /* ORACLE: the ramp index (value / 100) of the captured samples, mixer source, capture 0 started at 3000 */
  static const struct { u32 tmr; int idx[12]; } T[] = {
      {0xFE00, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
      {0xFF00, {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6}},
      {0xFD00, {2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18}},
      {0xFC00, {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24}},
      {0xFE80, {1, 2, 3, 3, 4, 5, 6, 6, 7, 8, 9, 9}},
  };
  for (unsigned i = 0; i < sizeof T / sizeof *T; i++) {
    AcoreState *s = setup(T[i].tmr, 6, 0x84);
    for (u64 k = 0; k < 40; k++) step(s, k, ramp(k), 0);
    for (int j = 0; j < 12; j++) ACT_EQ(m16(s, j, 0), 100 * T[i].idx[j]);
    char name[32];
    snprintf(name, sizeof name, "p05_timer_%04x", T[i].tmr);
    act_emit(s, name);
  }
}

ACT_TEST(p05_start_phase) {
  /* ORACLE: a start written at or before the cycle of a step takes part in it; one cycle later, in the next step.
   * The first captured ramp index for starts at 3000, 3072, 3073, 5000, 5120, 5121, 6000 */
  static const u64 t0[] = {3000, 3072, 3073, 5000, 5120, 5121, 6000};
  static const int first[] = {1, 1, 2, 3, 3, 4, 4};
  for (int i = 0; i < 7; i++) {
    AcoreState *s = setup(0xFE00, 6, 0x00); /* set up but not started */
    int started = 0;
    for (u64 k = 0; k < 16; k++) {
      if (!started && t0[i] <= k * 1024) {
        at(s, t0[i]);
        act_reg(s, CNT0, 0x84, 1);
        started = 1;
      }
      step(s, k, ramp(k), 0);
    }
    ACT_EQ(m16(s, 0, 0), 100 * first[i]);
  }
}

ACT_TEST(p05_loop_wrap) {
  /* loop, 2 words = 4 samples: the buffer is rewritten from DAD; the samples are ramp indices 1..17, the last
   * complete wrap is 13..16 */
  AcoreState *s = setup(0xFE00, 2, 0x80);
  for (u64 k = 0; k < 20; k++) step(s, k, ramp(k), 0);
  for (int i = 0; i < 4; i++) ACT_EQ(m16(s, i, 0), 100 * (13 + i));
  ACT_EQ(m16(s, 4, 0), UNTOUCHED);
  ACT_EQ(ac_hw_read(s, 0x508, 1) & 0x80, 0x80); /* still running */
  /* ORACLE: a loop of 3 words (6 samples), stopped at 30000 (after step 29): [1900..2400] = ramp indices 19-24, the
   * last complete wrap; the 3 samples after it are lost */
  AcoreState *t = setup(0xFE00, 3, 0x80);
  for (u64 k = 0; k < 30; k++) step(t, k, ramp(k), 0);
  at(t, 30000);
  act_reg(t, CNT0, 0, 1);
  for (int i = 0; i < 6; i++) ACT_EQ(m16(t, i, 0), 100 * (19 + i));
  ACT_EQ(m16(t, 6, 0), UNTOUCHED);
  act_emit(t, "p05_loop3_stop");
}

ACT_TEST(p05_blocks) {
  /* ORACLE: words are written in blocks of 4; a stop loses the block in progress */
  AcoreState *s = setup(0xFE00, 8, 0x84);
  for (u64 k = 0; k < 6; k++) step(s, k, ramp(k), 0);
  at(s, 6000);
  act_reg(s, CNT0, 0, 1); /* stop with 3 samples in, 1 word complete */
  for (int i = 0; i < 12; i++) ACT_EQ(m16(s, i, 0), UNTOUCHED);
  /* DAD written during a capture is used by the next block: 8 samples at DST, the rest at DST + 0x40 (ORACLE) */
  AcoreState *t = setup(0xFE00, 8, 0x84);
  for (u64 k = 0; k < 11; k++) step(t, k, ramp(k), 0);
  at(t, 10300); /* after step 10 (cycle 10240): the first block was written at DST */
  act_reg(t, DAD0, DST + 0x40, 4);
  for (u64 k = 11; k < 25; k++) step(t, k, ramp(k), 0);
  for (int i = 0; i < 8; i++) ACT_EQ(m16(t, i, 0), 100 * (1 + i));
  for (int i = 0; i < 8; i++) ACT_EQ(m16(t, 8 + i, 0x40), 100 * (9 + i));
  ACT_EQ(m16(t, 0, 0x40), UNTOUCHED);
  /* a DAD write before step 10 puts the first block there too (ORACLE: 10200 gives all at DST + 0x40) */
  AcoreState *u = setup(0xFE00, 8, 0x84);
  for (u64 k = 0; k < 10; k++) step(u, k, ramp(k), 0);
  at(u, 10200);
  act_reg(u, DAD0, DST + 0x40, 4);
  for (u64 k = 10; k < 25; k++) step(u, k, ramp(k), 0);
  for (int i = 0; i < 16; i++) ACT_EQ(m16(u, i, 0x40), 100 * (1 + i));
  ACT_EQ(m16(u, 0, 0), UNTOUCHED);
  /* LEN cut to 1 word during the capture (ORACLE): only (100, 200) is written, then it ends */
  AcoreState *w = setup(0xFE00, 8, 0x84);
  for (u64 k = 0; k < 6; k++) step(w, k, ramp(k), 0);
  at(w, 6000);
  act_reg(w, LEN0, 1, 2);
  for (u64 k = 6; k < 20; k++) step(w, k, ramp(k), 0);
  ACT_EQ(m16(w, 0, 0), 100);
  ACT_EQ(m16(w, 1, 0), 200);
  ACT_EQ(m16(w, 2, 0), UNTOUCHED);
  ACT_EQ(ac_hw_read(w, 0x508, 1) & 0x80, 0);
}

ACT_TEST(p05_len_dad_rules) {
  /* LEN 0 is 1 word; DAD bits 0-1 are ignored (ORACLE) */
  AcoreState *s = setup(0xFE00, 0, 0x84);
  for (u64 k = 0; k < 12; k++) step(s, k, ramp(k), 0);
  ACT_EQ(m16(s, 0, 0), 100);
  ACT_EQ(m16(s, 1, 0), 200);
  ACT_EQ(m16(s, 2, 0), UNTOUCHED);
  AcoreState *t = setup(0xFE00, 2, 0x80);
  at(t, 3010);
  act_reg(t, DAD0, DST + 2, 4);
  for (u64 k = 0; k < 12; k++) step(t, k, ramp(k), 0);
  ACT_EQ(m16(t, 0, 0), 500); /* the last complete wrap: ramp indices 5..8 */
  ACT_EQ(m16(t, 4, 0), UNTOUCHED);
}

ACT_TEST(p05_master_enable_and_timer_reload) {
  /* ORACLE: nothing is captured while SOUNDCNT bit 15 is clear; enabled at 10000, the first step is 10 (the channel
   * is frozen too: 0, 0, 100, ... from step 10) */
  AcoreState *s = setup(0xFE00, 6, 0x84);
  at(s, 3001);
  act_reg(s, SOUNDCNT, 127, 2);
  for (u64 k = 3; k < 10; k++) step(s, k, 0, 0);
  ACT_EQ(m16(s, 0, 0), UNTOUCHED);
  at(s, 10000);
  act_reg(s, SOUNDCNT, 0x8000 | 127, 2);
  for (u64 k = 10; k < 24; k++) step(s, k, ramp(k - 9), 0);
  s16 want[6] = {0, 0, 100, 200, 300, 400};
  for (int i = 0; i < 6; i++) ACT_EQ(m16(s, i, 0), want[i]);
  /* a TMR write is used at the next overflow: FE00 to FC00 at 6000 gives 100 200 300 400 600 800 1000 (ORACLE) */
  AcoreState *t = setup(0xFE00, 8, 0x84);
  for (u64 k = 0; k < 6; k++) step(t, k, ramp(k), 0);
  at(t, 6000);
  act_reg(t, TMR1, 0xFC00, 2);
  for (u64 k = 6; k < 40; k++) step(t, k, ramp(k), 0);
  s16 w2[10] = {100, 200, 300, 400, 600, 800, 1000, 1200, 1400, 1600};
  for (int i = 0; i < 10; i++) ACT_EQ(m16(t, i, 0), w2[i]);
  act_emit(t, "p05_tmr_change");
}

ACT_TEST(p05_restart_and_busy) {
  /* ORACLE: a one-shot clears the busy bit and a new write of the start bit starts it again at DAD; a write of the
   * start bit while running does nothing */
  AcoreState *s = setup(0xFE00, 1, 0x84);
  for (u64 k = 0; k < 20; k++) step(s, k, ramp(k), 0);
  ACT_EQ(m16(s, 0, 0), 100);
  ACT_EQ(m16(s, 1, 0), 200);
  ACT_EQ(ac_hw_read(s, 0x508, 1), 0x04);
  at(s, 20000);
  act_reg(s, CNT0, 0x84, 1);
  for (u64 k = 20; k < 24; k++) step(s, k, ramp(k), 0);
  ACT_EQ(m16(s, 0, 0), 1800);
  ACT_EQ(m16(s, 1, 0), 1900);
  AcoreState *t = setup(0xFE00, 4, 0x84);
  for (u64 k = 0; k < 6; k++) step(t, k, ramp(k), 0);
  at(t, 6000);
  act_reg(t, CNT0, 0x84, 1); /* no edge */
  for (u64 k = 6; k < 16; k++) step(t, k, ramp(k), 0);
  for (int i = 0; i < 8; i++) ACT_EQ(m16(t, i, 0), 100 * (1 + i));
  act_emit(s, "p05_restart");
}

ACT_TEST(p05_add_mode) {
  /* GBATEK 59, worked by hand (the oracle ignores bits 0 and 1): source = channel 0, with and without addition */
  s32 ch[AC_NCH] = {0};
  AcoreState *s = act_state_hw();
  at(s, 0);
  act_reg(s, SOUNDCNT, 0x8000 | 127, 2);
  act_reg(s, TMR1, 0xFE00, 2);
  act_reg(s, DAD0, DST, 4);
  act_reg(s, LEN0, 2, 2);
  act_reg(s, 0x04000400, 127, 4); /* channel 0: volume 127 (128/128), shift 0 */
  act_reg(s, 0x04000410, 64, 4);  /* channel 1: volume 64 */
  act_reg(s, CNT0, 0x87, 1);      /* start, source channel 0, one-shot, add mode */
  s32 a[4] = {1000, 0x4000, -0x8000, 300}, b[4] = {-250, 0x4000, -0x8000, 0x7FFF};
  for (int k = 0; k < 4; k++) {
    ch[0] = a[k], ch[1] = b[k];
    step_raw(s, (u64)k, 0, 0, ch);
  }
  /* a + b/2: 1000 - 125; 0x4000 + 0x2000; -0x8000 - 0x4000 = -0xC000, & FFFF = 0x4000; 300 + 0x3FFF.8 = 0x412B */
  s16 want[4] = {875, 0x6000, 0x4000, 0x412B};
  for (int i = 0; i < 4; i++) ACT_EQ(m16(s, i, 0), want[i]);
  /* addition off: plain ch(a) after its volume; both negative gives -0x8000 */
  AcoreState *t = act_state_hw();
  at(t, 0);
  act_reg(t, SOUNDCNT, 0x8000 | 127, 2);
  act_reg(t, TMR1, 0xFE00, 2);
  act_reg(t, DAD0, DST, 4);
  act_reg(t, LEN0, 2, 2);
  act_reg(t, 0x04000400, 64 | (1 << 8), 4); /* volume 64 (half), divider 2: a quarter */
  act_reg(t, 0x04000410, 127, 4);
  act_reg(t, CNT0, 0x86, 1); /* source channel 0, no addition */
  s32 a2[4] = {4000, -4000, -4000, 5}, b2[4] = {-9, -9, 9, -9};
  for (int k = 0; k < 4; k++) {
    ch[0] = a2[k], ch[1] = b2[k];
    step_raw(t, (u64)k, 0, 0, ch);
  }
  /* 4000/4 = 1000 (b < 0 but a >= 0); both < 0 gives -0x8000; -1000 with b >= 0; 5/4 floors to 1 */
  s16 w2[4] = {1000, -0x8000, -1000, 1};
  for (int i = 0; i < 4; i++) ACT_EQ(m16(t, i, 0), w2[i]);
  /* channel 0 off (0): the sum is the plain ch(b); PCM8 takes the high byte of the 16-bit result */
  AcoreState *u = act_state_hw();
  at(u, 0);
  act_reg(u, SOUNDCNT, 0x8000 | 127, 2);
  act_reg(u, TMR1, 0xFE00, 2);
  act_reg(u, DAD0, DST, 4);
  act_reg(u, LEN0, 1, 2);
  act_reg(u, 0x04000410, 127, 4);
  act_reg(u, CNT0, 0x8F, 1); /* PCM8, one-shot, ch0, add */
  for (int k = 0; k < 4; k++) {
    ch[0] = 0, ch[1] = 0x1234 + 0x100 * k;
    step_raw(u, (u64)k, 0, 0, ch);
  }
  ACT_EQ(m8(u, 0), 0x12);
  ACT_EQ(m8(u, 1), 0x13);
  ACT_EQ(m8(u, 2), 0x14);
  ACT_EQ(m8(u, 3), 0x15);
}
