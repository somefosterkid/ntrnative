/* port/acore piece 14: unit tests (docs/acore.md, piece 14's "Tests:"). OWNED BY PIECE 14.
 * They stand on piece 14: the channel is built by hand, and the volume / timer expectations are written with 6's
 * functions on the arguments the SDK's formula gives (6 is tested by its own piece); the pan is exact. */
#include "act.h"

#define SYNC(c, f) (((c)->flags >> AC_EXF_SYNC_SHIFT) & (f))

/* channel 0, active, start pending, as piece 10/11 leave a fresh note; the cached results impossible so that any
 * computed value shows as a change */
static AC_ExChannel *mk(AcoreState *s, int key, int base, int vel, int pan) {
  AC_ExChannel *c = &s->work.channel[0];
  c->myNo = 0;
  c->flags = AC_EXF_ACTIVE | (AC_SYNC_START << AC_EXF_SYNC_SHIFT);
  c->env_status = AC_ENV_SUSTAIN;
  c->env_decay = 0;
  c->key = (u8)key;
  c->original_key = (u8)base;
  c->velocity = (u8)vel;
  c->init_pan = (s8)pan;
  c->pan_range = 127;
  c->wave.timer = 0xFE00;
  c->volume = 0xFFFF;
  c->timer = 0xFFFF;
  c->pan = 0xFF;
  c->lfo.param.depth = 0;
  return c;
}

ACT_TEST(p14_key_is_base_key) {
  AcoreState *s = act_state();
  AC_ExChannel *c = mk(s, 60, 60, 127, 0);
  ac_exch_main(s, 0);
  ACT_EQ(c->timer, ac_calc_timer(0xFE00, 0));
  ACT_EQ(c->volume, ac_calc_volume(ac_decibel(127)));
  ACT_EQ(c->pan, 64);
  ACT_EQ(SYNC(c, AC_SYNC_TIMER | AC_SYNC_VOLUME | AC_SYNC_PAN), AC_SYNC_TIMER | AC_SYNC_VOLUME | AC_SYNC_PAN);
  ACT_CHECK(c->flags & AC_EXF_ACTIVE);
  ACT_CHECK(SYNC(c, AC_SYNC_START)); /* 11's to clear */
}

ACT_TEST(p14_octave_up_and_down) {
  AcoreState *s = act_state();
  AC_ExChannel *c = mk(s, 72, 60, 127, 0);
  ac_exch_main(s, 0);
  ACT_EQ(c->timer, ac_calc_timer(0xFE00, 12 * 64));
  c->key = 48;
  ac_exch_main(s, 0);
  ACT_EQ(c->timer, ac_calc_timer(0xFE00, -12 * 64));
  c->key = 61; /* one semitone, plus the user pitch (1/64 semitones) and the sweep (13's value, 0 with none) */
  c->user_pitch = 10;
  ac_exch_main(s, 0);
  ACT_EQ(c->timer, ac_calc_timer(0xFE00, 64 + 10));
}

ACT_TEST(p14_velocity_and_decays) {
  AcoreState *s = act_state();
  AC_ExChannel *c = mk(s, 60, 60, 64, 0);
  ac_exch_main(s, 0);
  ACT_EQ(c->volume, ac_calc_volume(ac_decibel_sq(64))); /* velocity's square law (ai58, oracle) */
  /* the envelope (env_decay is dB*10 << 7) and both user decays add */
  c->env_decay = -100 * 128;
  c->user_decay = -20;
  c->user_decay2 = -3;
  c->velocity = 127;
  ac_exch_main(s, 0);
  ACT_EQ(c->volume, ac_calc_volume(ac_decibel(127) - 100 - 20 - 3));
  /* nothing changed: no sync flag set again */
  c->flags = (u8)(c->flags & ~(AC_SYNC_VOLUME << AC_EXF_SYNC_SHIFT));
  ac_exch_main(s, 0);
  ACT_EQ(SYNC(c, AC_SYNC_VOLUME), 0);
}

ACT_TEST(p14_pan_range) {
  AcoreState *s = act_state();
  static const int in[] = {-64, 0, 63}, out[] = {0, 64, 127};
  for (int i = 0; i < 3; i++) {
    AC_ExChannel *c = mk(s, 60, 60, 127, in[i]);
    ac_exch_main(s, 0);
    ACT_EQ(c->pan, out[i]);
  }
  /* user pan adds and the result clamps to 0-127 */
  AC_ExChannel *c = mk(s, 60, 60, 127, 63);
  c->user_pan = 30;
  ac_exch_main(s, 0);
  ACT_EQ(c->pan, 127);
  c->user_pan = -127;
  ac_exch_main(s, 0);
  ACT_EQ(c->pan, 0);
  /* the pan range scales the instrument pan (half range: -64 -> -32) before the user pan */
  c->user_pan = 0;
  c->pan_range = 64;
  c->init_pan = -64;
  ac_exch_main(s, 0);
  ACT_EQ(c->pan, 32);
  c->init_pan = 0;
  ac_exch_main(s, 0);
  ACT_EQ(c->pan, 64);
  /* the oracle's pan registers for instrument pan 0,1,2,17,33,63,64,65,100,126,127 (init pan = raw - 64) at pan
   * range 64 and 100 (drv stream, one note each): rounded (pan * range + 64) >> 7 */
  static const int raw[] = {0, 1, 2, 17, 33, 63, 64, 65, 100, 126, 127};
  static const int r64[] = {32, 33, 33, 41, 49, 64, 64, 65, 82, 95, 96};
  static const int r100[] = {14, 15, 16, 27, 40, 63, 64, 65, 92, 112, 113};
  for (int i = 0; i < 11; i++) {
    c->init_pan = (s8)(raw[i] - 64);
    c->pan_range = 64;
    ac_exch_main(s, 0);
    ACT_EQ(c->pan, r64[i]);
    c->pan_range = 100;
    ac_exch_main(s, 0);
    ACT_EQ(c->pan, r100[i]);
  }
}

ACT_TEST(p14_length_runs_out) {
  AcoreState *s = act_state();
  AC_ExChannel *c = mk(s, 60, 60, 127, 0);
  c->length = 3;
  ac_exch_length_tick(s, 0);
  ac_exch_length_tick(s, 0);
  ACT_EQ(c->length, 1);
  ac_exch_length_tick(s, 0);
  ACT_EQ(c->length, 0); /* released here through 11 */
  ac_exch_length_tick(s, 0);
  ACT_EQ(c->length, 0);
  c->length = -1; /* a tie never runs out */
  ac_exch_length_tick(s, 0);
  ACT_EQ(c->length, -1);
  /* the main step itself does not count the length (it counts sequence ticks, not wakes) */
  c->length = 5;
  ac_exch_main(s, 1);
  ACT_EQ(c->length, 5);
}

ACT_TEST(p14_channel_ends) {
  AcoreState *s = act_state();
  /* a release at the envelope floor ends the channel at the next wake: stop flagged, no longer active */
  AC_ExChannel *c = mk(s, 60, 60, 127, 0);
  c->env_status = AC_ENV_RELEASE;
  c->env_decay = -(723 << 7);
  ac_exch_main(s, 0);
  ACT_CHECK(!(c->flags & AC_EXF_ACTIVE));
  ACT_CHECK(SYNC(c, AC_SYNC_STOP));
  /* a running release above the floor does not */
  c = mk(s, 60, 60, 127, 0);
  c->env_status = AC_ENV_RELEASE;
  c->env_decay = -(500 << 7);
  ac_exch_main(s, 0);
  ACT_CHECK(c->flags & AC_EXF_ACTIVE);
  /* a hardware channel that stopped by itself (start done, busy bit clear) ends; a busy one does not */
  c = mk(s, 60, 60, 127, 0);
  c->flags = AC_EXF_ACTIVE;
  act_reg(s, 0x04000400 + AC_CH_CNT, 0xA840007F, 4);
  ac_exch_main(s, 0);
  ACT_CHECK(c->flags & AC_EXF_ACTIVE);
  ac_hw_set_busy(s, 0, 0);
  ac_exch_main(s, 0);
  ACT_CHECK(!(c->flags & AC_EXF_ACTIVE));
  ACT_CHECK(!SYNC(c, AC_SYNC_STOP)); /* ended by itself: freed, no stop write (ai58: the oracle's se_menu) */
}

ACT_TEST(p14_inactive_untouched) {
  AcoreState *s = act_state();
  AC_ExChannel *c = mk(s, 72, 60, 127, 0);
  c->flags = 0;
  ac_exch_main(s, 1);
  ACT_EQ(c->timer, 0xFFFF);
  ACT_EQ(c->flags, 0);
}
