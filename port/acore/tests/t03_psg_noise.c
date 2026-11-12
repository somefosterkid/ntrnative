/* port/acore piece 3: unit tests (docs/acore.md, piece 3's "Tests:"). OWNED BY PIECE 3. */
#include "act.h"
#include <string.h>

static u32 cnt_word(int duty, int repeat, int fmt) {
  return (u32)duty << 24 | (u32)repeat << 27 | (u32)fmt << 29 | 0x7Fu; /* start bit left clear */
}

/* GBATEK's patterns, '_' LOW and '-' HIGH, one per duty 0-7 */
static const char *const pat[8] = {"_______-", "______--", "_____---", "____----",
                                   "___-----", "__------", "_-------", "________"};

ACT_TEST(p03_duty_patterns) {
  AcoreState *s = act_state_hw();
  for (int duty = 0; duty < 8; duty++) {
    int ch = 8 + duty % 6;
    ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT, cnt_word(duty, 0, 3), 4);
    ac_psg_start(s, ch);
    for (int i = 0; i < 24; i++) ACT_EQ(ac_psg_next(s, ch), pat[duty][i & 7] == '-' ? 0x7FFF : -0x7FFF);
  }
}

ACT_TEST(p03_restart_low) {
  AcoreState *s = act_state_hw();
  ac_hw_write(s, AC_REG_CH(9) + AC_CH_CNT, cnt_word(6, 0, 3), 4);
  ac_psg_start(s, 9);
  for (int i = 0; i < 5; i++) ac_psg_next(s, 9);
  ac_psg_start(s, 9);
  ACT_EQ(ac_psg_next(s, 9), -0x7FFF);
  ACT_EQ(ac_psg_next(s, 9), 0x7FFF);
}

/* X = 7FFFh; each step `X >>= 1; carry ? (LOW, X ^= 6000h) : HIGH`. By hand: 7FFF -> 3FFF^6000 = 5FFF LOW,
 * 5FFF -> 2FFF^6000 = 4FFF LOW, 4FFF -> 27FF^6000 = 47FF LOW, 47FF -> 23FF^6000 = 43FF LOW, ... the XOR keeps
 * bit 14 set and the low bits ones, so the output is LOW for 14 steps (the ones drain out), then HIGH 14 times
 * while the 0 bits reach bit 0, then one LOW and HIGH x3. */
ACT_TEST(p03_noise_first32) {
  AcoreState *s = act_state_hw();
  ac_hw_write(s, AC_REG_CH(14) + AC_CH_CNT, cnt_word(0, 0, 3), 4);
  ac_psg_start(s, 14);
  static const char *exp = "00000000000000111111111111110111"; /* 1 = HIGH */
  ACT_EQ(s->p03.lfsr[14], 0x7FFF);
  for (int i = 0; i < 32; i++) {
    ACT_EQ(ac_psg_next(s, 14), exp[i] == '1' ? 0x7FFF : -0x7FFF);
    if (i == 0) ACT_EQ(s->p03.lfsr[14], 0x5FFF);
    if (i == 1) ACT_EQ(s->p03.lfsr[14], 0x4FFF);
    if (i == 2) ACT_EQ(s->p03.lfsr[14], 0x47FF);
  }
  ac_psg_start(s, 14); /* a restart goes back to X = 7FFF */
  for (int i = 0; i < 32; i++) ACT_EQ(ac_psg_next(s, 14), exp[i] == '1' ? 0x7FFF : -0x7FFF);
}

ACT_TEST(p03_noise_channels_independent) {
  AcoreState *s = act_state_hw();
  ac_psg_start(s, 14);
  for (int i = 0; i < 20; i++) ac_psg_next(s, 14);
  ac_psg_start(s, 15);
  ACT_EQ(ac_psg_next(s, 15), -0x7FFF);
  ACT_EQ(s->p03.lfsr[15], 0x5FFF);
}

ACT_TEST(p03_ignores_len_repeat) {
  AcoreState *s = act_state_hw();
  s32 a[40], b[40];
  for (int k = 0; k < 2; k++) {
    ac_hw_write(s, AC_REG_CH(10) + AC_CH_CNT, cnt_word(2, k ? 2 : 0, 3), 4);
    ac_hw_write(s, AC_REG_CH(10) + AC_CH_LEN, k ? 1 : 0, 4);
    ac_psg_start(s, 10);
    for (int i = 0; i < 40; i++) (k ? b : a)[i] = ac_psg_next(s, 10);
  }
  ACT_CHECK(memcmp(a, b, sizeof a) == 0);
  ACT_EQ(a[39], 0x7FFF); /* step 7 of duty 2: still going after 40 samples */
  ACT_EQ(a[32], -0x7FFF);
}

ACT_TEST(p03_pcm_channels_silent) {
  AcoreState *s = act_state_hw();
  for (int ch = 0; ch < 8; ch++) {
    ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT, cnt_word(3, 0, 3), 4);
    ac_psg_start(s, ch);
    for (int i = 0; i < 8; i++) ACT_EQ(ac_psg_next(s, ch), 0);
  }
}
