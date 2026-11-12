/* port/acore piece 13: unit tests (docs/acore.md, piece 13's "Tests:"). OWNED BY PIECE 13.
 * Expected values are worked by hand from the rules at the top of p13_lfo_sweep.c (fitted on the oracle:
 * delay shifts the first modulated value by exactly `delay` periodic steps, speed 16 = 32 steps per period, pan
 * swing 64 +- 63 at depth 127 range 1, the portamento sweeps of sequences with C9 / CE / CF / E3).
 * Tests that read the sine need piece 6's ac_sine (a stub returns 0): they are TODO until it merges.
 */
#include "act.h"
#include <string.h>

static AC_Lfo mk(int target, int speed, int depth, int range, int delay) {
  AC_Lfo l;
  ac_lfo_init(&l.param);
  l.param.target = (u8)target;
  l.param.speed = (u8)speed;
  l.param.depth = (u8)depth;
  l.param.range = (u8)range;
  l.param.delay = (u16)delay;
  ac_lfo_start(&l);
  return l;
}

ACT_TEST(p13_init_start) {
  AC_LfoParam p;
  p.target = p.speed = p.depth = p.range = 99;
  p.delay = 99;
  ac_lfo_init(&p);
  ACT_EQ(p.target, 0);
  ACT_EQ(p.speed, 16);
  ACT_EQ(p.depth, 0);
  ACT_EQ(p.range, 1);
  ACT_EQ(p.delay, 0);
  AC_Lfo l = {p, 7, 9};
  ac_lfo_start(&l);
  ACT_EQ(l.counter, 0);
  ACT_EQ(l.delay_counter, 0);
}

ACT_TEST(p13_delay) {
  AcoreState *s = act_state();
  AC_Lfo l = mk(2, 16, 127, 1, 3);
  for (int i = 0; i < 3; i++) {
    ac_lfo_update(s, &l, 1);
    ACT_EQ(l.counter, 0); /* the delay counts, the counter stays */
    ACT_EQ(l.delay_counter, i + 1);
  }
  ac_lfo_update(s, &l, 1);
  ACT_EQ(l.counter, 16 << 6); /* the first step: sine index 4 */
  ac_lfo_update(s, &l, 0);    /* a command's wake does not advance */
  ACT_EQ(l.counter, 16 << 6);
}

ACT_TEST(p13_period_speed16) {
  AcoreState *s = act_state();
  AC_Lfo l = mk(2, 16, 127, 1, 0);
  for (int i = 0; i < 32; i++) {
    ACT_EQ(l.counter >> 8, 4 * i); /* the sine index of step i */
    ac_lfo_update(s, &l, 1);
  }
  ACT_EQ(l.counter, 0); /* one period = 32 steps */
  l = mk(0, 255, 1, 1, 0); /* the fastest speed wraps inside 128 << 8 */
  for (int i = 0; i < 100; i++) {
    ac_lfo_update(s, &l, 1);
    ACT_CHECK(l.counter < 0x8000);
  }
  ACT_EQ(l.counter, (100 * 255 * 64) & 0x7FFF);
}

ACT_TEST(p13_depth0_runs) {
  AcoreState *s = act_state();
  AC_Lfo l = mk(2, 16, 0, 1, 0);
  for (int i = 0; i < 5; i++) {
    ACT_EQ(ac_lfo_value(s, &l), 0);
    ac_lfo_update(s, &l, 1);
  }
  ACT_EQ(l.counter, 5 * 1024); /* the counter runs while depth is 0 (fitted: a later depth finds the phase) */
}

ACT_TEST(p13_targets) {
  AcoreState *s = act_state();
  /* the formulas, against whatever ac_sine returns (piece 6; a stub returns 0 and the loop is vacuous) */
  for (int target = 0; target < 3; target++) {
    for (int i = 0; i < 128; i += 5) {
      AC_Lfo l = mk(target, 16, 100, 3, 0);
      l.counter = (u16)(i << 8 | 0x7F); /* the low byte does not matter */
      s32 v = (s32)ac_sine(i) * 100 * 3;
      ACT_EQ(ac_lfo_value(s, &l), target == 0 ? (v * 64) >> 14 : v >> 8);
    }
  }
  if (ac_sine(32) != 127) return; /* hand-worked peaks, with the real sine table */
  AC_Lfo l = mk(2, 16, 127, 1, 0);
  l.counter = 32 << 8; /* the sine peak (+127) */
  ACT_EQ(ac_lfo_value(s, &l), 63); /* pan: 127 * 127 * 1 >> 8 */
  l.counter = 96 << 8; /* the trough (-127) */
  ACT_EQ(ac_lfo_value(s, &l), -64); /* arithmetic shift: -16129 >> 8 (the oracle reached pan 0) */
  l.param.target = 1;
  ACT_EQ(ac_lfo_value(s, &l), -64); /* volume: the same, in dB * 10 */
  l.param.target = 0;
  l.param.range = 8;
  l.counter = 32 << 8;
  ACT_EQ(ac_lfo_value(s, &l), 504); /* pitch: 127 * 127 * 8 * 64 >> 14, 1/64 semitones */
  l.param.depth = 0;
  ACT_EQ(ac_lfo_value(s, &l), 0);
}

ACT_TEST(p13_sweep_half) {
  AcoreState *s = act_state();
  AC_ExChannel *c = &s->work.channel[3];
  c->sweep_pitch = 768;
  c->sweep_length = 100;
  c->sweep_counter = 0;
  ACT_EQ(ac_sweep_value(s, 3), 768);
  for (int i = 0; i < 50; i++) ac_sweep_update(s, 3, 0); /* not auto: sequencer ticks */
  ACT_EQ(c->sweep_counter, 50);
  ACT_EQ(ac_sweep_value(s, 3), 384); /* half its length: half the pitch */
  ac_sweep_update(s, 3, 1);          /* a periodic step does not move a non-auto sweep */
  ACT_EQ(c->sweep_counter, 50);
  c->sweep_pitch = -768;
  c->sweep_length = 150;
  c->sweep_counter = 1;
  ACT_EQ(ac_sweep_value(s, 3), -762); /* -768 * 149 / 150 truncated toward zero (fitted: -762) */
  c->sweep_counter = 150;
  ACT_EQ(ac_sweep_value(s, 3), 0);
  ac_sweep_update(s, 3, 0);
  ACT_EQ(c->sweep_counter, 150); /* stops at the length */
  c->sweep_length = 0;
  ACT_EQ(ac_sweep_value(s, 3), 0);
}

ACT_TEST(p13_sweep_auto) {
  AcoreState *s = act_state();
  AC_ExChannel *c = &s->work.channel[0];
  c->flags |= AC_EXF_AUTO_SWEEP;
  c->sweep_pitch = 640;
  c->sweep_length = 20;
  c->sweep_counter = 0;
  ac_sweep_update(s, 0, 0);
  ACT_EQ(c->sweep_counter, 0);
  for (int i = 0; i < 10; i++) ac_sweep_update(s, 0, 1);
  ACT_EQ(ac_sweep_value(s, 0), 320);
}

ACT_TEST(p13_portamento_time0) {
  AcoreState *s = act_state();
  AC_Track t;
  memset(&t, 0, sizeof t);
  t.flags = AC_TRF_PORTA;
  t.porta_key = 72;
  t.porta_time = 0;
  t.sweep_pitch = 0;
  ac_porta_setup(s, 5, &t, 60, 48);
  AC_ExChannel *c = &s->work.channel[5];
  ACT_EQ(c->sweep_pitch, 768); /* (72 - 60) * 64: starts a octave up and slides to the note */
  ACT_EQ(c->sweep_length, 48); /* time 0: the note's length, counted in sequencer ticks */
  ACT_EQ(c->sweep_counter, 0);
  ACT_EQ(c->flags & AC_EXF_AUTO_SWEEP, 0);
  ACT_EQ(t.porta_key, 60);     /* the next note slides from this key */
  /* off: only the track's sweep pitch */
  t.flags = 0;
  t.sweep_pitch = -100;
  ac_porta_setup(s, 5, &t, 65, 10);
  ACT_EQ(c->sweep_pitch, -100);
  ACT_EQ(t.porta_key, 65);     /* tracked with the flag off too (fitted) */
}

ACT_TEST(p13_portamento_time) {
  AcoreState *s = act_state();
  AC_Track t;
  memset(&t, 0, sizeof t);
  t.flags = AC_TRF_PORTA;
  t.porta_key = 72;
  t.porta_time = 20;
  ac_porta_setup(s, 1, &t, 60, 1000);
  AC_ExChannel *c = &s->work.channel[1];
  ACT_EQ(c->sweep_pitch, 768);
  ACT_EQ(c->sweep_length, 150); /* 20 * 20 * 768 >> 11 (fitted: the slide took 150 periodic steps) */
  ACT_CHECK(c->flags & AC_EXF_AUTO_SWEEP);
  /* portamento and the track's sweep pitch add: 768 + 100, 400 * 868 >> 11 = 169 */
  t.porta_key = 72;
  t.sweep_pitch = 100;
  ac_porta_setup(s, 1, &t, 60, 1000);
  ACT_EQ(c->sweep_pitch, 868);
  ACT_EQ(c->sweep_length, 169);
  /* downward */
  t.porta_key = 48;
  t.sweep_pitch = 0;
  ac_porta_setup(s, 1, &t, 60, 1000);
  ACT_EQ(c->sweep_pitch, -768);
  ACT_EQ(c->sweep_length, 150);
}
