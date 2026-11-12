/* port/acore piece 6: unit tests (docs/acore.md, piece 6's "Tests:"). OWNED BY PIECE 6.
 * Expected values: the formulas in p06_tables.c's header (worked by hand), the game's table ends, and TMR values the
 * oracle wrote for notes (probe streams: a looping wave of time T, instrument key 60, a note of key K, pitch bend
 * b/64 semitone: the oracle's SOUNDxTMR = 0x10000 - calc_timer(T, (K - 60) * 64 + b)). */
#include "act.h"

ACT_TEST(p06_decibel_table) {
  ACT_EQ(ac_decibel(0), -32768);
  ACT_EQ(ac_decibel(1), -421);
  ACT_EQ(ac_decibel(2), -361);
  ACT_EQ(ac_decibel(64), -60);
  ACT_EQ(ac_decibel(100), -21);
  ACT_EQ(ac_decibel(126), -1);
  ACT_EQ(ac_decibel(127), 0);
  /* -6 dB (a half of the amplitude): 200 * log10(0.5) = -60.2 */
  ACT_EQ(ac_decibel(64), -60);
  /* monotonic, and the formula round(200 * log10(x / 127)) at a few x */
  for (int x = 2; x < 128; x++) ACT_CHECK(ac_decibel(x) >= ac_decibel(x - 1));
  /* out of range clamps */
  ACT_EQ(ac_decibel(-5), -32768);
  ACT_EQ(ac_decibel(500), 0);
}

ACT_TEST(p06_pitch_table) {
  ACT_EQ(ac_pitch_table(0), 0);
  ACT_EQ(ac_pitch_table(1), 59);
  ACT_EQ(ac_pitch_table(2), 118);
  ACT_EQ(ac_pitch_table(767), 65418);
  /* a half octave (384): 65536 * (sqrt(2) - 1) = 27146.2 */
  ACT_EQ(ac_pitch_table(384), 27146);
  /* a semitone is 64 steps: 2^(1/12) - 1 = 0.059463 */
  ACT_EQ(ac_pitch_table(64), 3897);
  for (int i = 1; i < 768; i++) ACT_CHECK(ac_pitch_table(i) > ac_pitch_table(i - 1));
}

ACT_TEST(p06_volume_table) {
  ACT_EQ(ac_volume_table(0), 0);     /* -723 dB */
  ACT_EQ(ac_volume_table(1), 1);
  ACT_EQ(ac_volume_table(723), 127); /* 0 dB */
  ACT_EQ(ac_volume_table(722), 126); /* -1: the BIOS table; the formula would round to 127 */
  ACT_EQ(ac_volume_table(723 - 59), 0x41);
  ACT_EQ(ac_volume_table(723 - 60), 0x40);   /* -6 dB: 128 * 10^-0.3 = 64 */
  ACT_EQ(ac_volume_table(723 - 61), 0x7e);   /* the next segment (shift 1) restarts high */
  ACT_EQ(ac_volume_table(723 - 120), 0x40);
  ACT_EQ(ac_volume_table(723 - 121), 0x7e);
  ACT_EQ(ac_volume_table(723 - 240), 0x20);
  ACT_EQ(ac_volume_table(723 - 241), 0x7f);
  ACT_EQ(ac_volume_table(-3), 0);    /* clamps */
  ACT_EQ(ac_volume_table(900), 127);
}

ACT_TEST(p06_sine) {
  ACT_EQ(ac_sine(0), 0);
  ACT_EQ(ac_sine(1), 6);
  ACT_EQ(ac_sine(16), 90);   /* 127 * sin(pi / 4) */
  ACT_EQ(ac_sine(31), 127);
  ACT_EQ(ac_sine(32), 127);
  ACT_EQ(ac_sine(33), 127);
  ACT_EQ(ac_sine(48), 90);
  ACT_EQ(ac_sine(63), 6);
  ACT_EQ(ac_sine(64), 0);
  ACT_EQ(ac_sine(65), -6);
  ACT_EQ(ac_sine(96), -127);
  ACT_EQ(ac_sine(127), -6);
  ACT_EQ(ac_sine(128), 0);
  for (int i = 0; i < 64; i++) ACT_EQ(ac_sine(i), -ac_sine(i + 64));
  for (int i = 0; i < 64; i++) ACT_EQ(ac_sine(i), ac_sine(64 - i));
}

ACT_TEST(p06_calc_timer_octaves) {
  ACT_EQ(ac_calc_timer(1000, 0), 1000);
  ACT_EQ(ac_calc_timer(1000, 768), 500);    /* +1 octave: half the period */
  ACT_EQ(ac_calc_timer(1000, -768), 2000);  /* -1 octave */
  ACT_EQ(ac_calc_timer(1000, 1536), 250);
  ACT_EQ(ac_calc_timer(1000, -1536), 4000);
  /* a semitone up: 1000 * 65536 / (65536 + 3897) = 943.9 -> 943 (the table form: truncated) */
  ACT_EQ(ac_calc_timer(1000, 64), (1000 * (ac_pitch_table(704) + 0x10000u)) >> 17);
  ACT_EQ(ac_calc_timer(512, 64), 483);   /* the oracle's README: 512 * (pitchtable[704] + 0x10000) >> 17 */
  /* clamps */
  ACT_EQ(ac_calc_timer(1000, 768 * 8), 0x10);
  ACT_EQ(ac_calc_timer(1000, -768 * 8), 0xFFFF);
  ACT_EQ(ac_calc_timer(0xFFFF, -768), 0xFFFF);
  ACT_EQ(ac_calc_timer(1000, 768 * 40), 0x10);
}

/* values read from the oracle's trace (SOUNDxTMR = 0x10000 - the period) */
ACT_TEST(p06_calc_timer_oracle) {
  ACT_EQ(0x10000 - ac_calc_timer(1047, -3840 - 127), 27964);
  ACT_EQ(0x10000 - ac_calc_timer(1047, -3840 - 126), 27997);
  ACT_EQ(0x10000 - ac_calc_timer(1047, -3840 - 125), 28031);
  ACT_EQ(0x10000 - ac_calc_timer(8378, -3840 - 127), 1);    /* clamped to the longest period */
  ACT_EQ(0x10000 - ac_calc_timer(511, -3840 - 127), 47199);
  ACT_EQ(0x10000 - ac_calc_timer(511, -3840 - 126), 47215);
}

ACT_TEST(p06_calc_volume) {
  ACT_EQ(ac_calc_volume(0), 127);
  ACT_EQ(ac_calc_volume(-1), 0x7e);
  ACT_EQ(ac_calc_volume(-60), 0x40);          /* -6 dB: shift 0, volume 64 */
  ACT_EQ(ac_calc_volume(-61), 1 << 8 | 0x7e); /* shift 1 (/2) from -61 */
  ACT_EQ(ac_calc_volume(-120), 1 << 8 | 0x40);
  ACT_EQ(ac_calc_volume(-121), 2 << 8 | 0x7e);
  ACT_EQ(ac_calc_volume(-240), 2 << 8 | 0x20);
  ACT_EQ(ac_calc_volume(-241), 3 << 8 | 0x7f);
  /* the minimum */
  ACT_EQ(ac_calc_volume(-723), 3 << 8 | 0);
  ACT_EQ(ac_calc_volume(-5000), 3 << 8 | 0);
  ACT_EQ(ac_calc_volume(100), 127);
  /* oracle: a note at expression 127 and track volume 126 wrote 0x7e */
  ACT_EQ(ac_calc_volume(ac_decibel(126) + ac_decibel(127)), 0x7e);
}

ACT_TEST(p06_random) {
  AcoreState *s = act_state();
  s->p06.seed = 0;
  /* x1 = 1013904223 = 0x3C6EF35F: the high half is 0x3C6E */
  ACT_EQ(ac_random(s), 0x3C6E);
  /* x2 = x1 * 1664525 + 1013904223 mod 2^32 */
  u32 x = 1013904223u;
  x = x * 1664525u + 1013904223u;
  ACT_EQ(ac_random(s), x >> 16);
}
