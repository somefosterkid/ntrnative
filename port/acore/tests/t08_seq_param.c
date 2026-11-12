/* port/acore piece 8: unit tests (docs/acore.md, piece 8's "Tests:"). OWNED BY PIECE 8. */
#include "act.h"
#include <string.h>

void ac_p08_mix(AcoreState *s, int p, int t, int *vol, int *fader, int *pan, int *pitch); /* p08_seq_param.c */

/* piece 6 is a stub until it merges: the dB values below are only checked once ac_decibel works */
static int db_ready(void) { return ac_decibel(0) < ac_decibel(127); }

ACT_TEST(p08_init_defaults) {
  AcoreState *s = act_state();
  s->work.track[3].volume = 5;
  ac_track_init(s, 3);
  AC_Track *k = &s->work.track[3];
  ACT_EQ(k->volume, 127);
  ACT_EQ(k->volume2, 127);
  ACT_EQ(k->pan_range, 127);
  ACT_EQ(k->bend_range, 2);
  ACT_EQ(k->attack, 255);
  ACT_EQ(k->release, 255);
  ACT_EQ(k->prio, 64);
  ACT_EQ(k->mod.speed, 16);
  ACT_EQ(k->mod.range, 1);
  ACT_EQ(k->channel_mask, 0xFFFF);
  ACT_EQ(k->pitch_bend, 0);
  ACT_EQ(k->flags, AC_TRF_NOTE_WAIT | AC_TRF_CMP);
}

ACT_TEST(p08_each_command_field) {
  AcoreState *s = act_state();
  AC_Track *k = &s->work.track[1];
  AC_Player *pl = &s->work.player[2];
  ac_track_init(s, 1);
  ac_track_param(s, 2, 1, 0xC0, 0); ACT_EQ(k->pan, -64);
  ac_track_param(s, 2, 1, 0xC0, 127); ACT_EQ(k->pan, 63);
  ac_track_param(s, 2, 1, 0xC1, 99); ACT_EQ(k->volume, 99);
  ac_track_param(s, 2, 1, 0xC2, 77); ACT_EQ(pl->volume, 77);
  ac_track_param(s, 2, 1, 0xC3, -5); ACT_EQ(k->transpose, -5);
  ac_track_param(s, 2, 1, 0xC4, -127); ACT_EQ(k->pitch_bend, -127);
  ac_track_param(s, 2, 1, 0xC5, 12); ACT_EQ(k->bend_range, 12);
  ac_track_param(s, 2, 1, 0xC6, 40); ACT_EQ(k->prio, 40);
  ac_track_param(s, 2, 1, 0xC7, 1); ACT_CHECK(k->flags & AC_TRF_NOTE_WAIT);
  ac_track_param(s, 2, 1, 0xC7, 0); ACT_CHECK(!(k->flags & AC_TRF_NOTE_WAIT));
  ac_track_param(s, 2, 1, 0xC8, 1); ACT_CHECK(k->flags & AC_TRF_TIE);
  ac_track_param(s, 2, 1, 0xC8, 0); ACT_CHECK(!(k->flags & AC_TRF_TIE));
  ac_track_param(s, 2, 1, 0xC3, 2);
  ac_track_param(s, 2, 1, 0xC9, 60); ACT_EQ(k->porta_key, 62); ACT_CHECK(k->flags & AC_TRF_PORTA);
  ac_track_param(s, 2, 1, 0xCE, 0); ACT_CHECK(!(k->flags & AC_TRF_PORTA));
  ac_track_param(s, 2, 1, 0xCE, 1); ACT_CHECK(k->flags & AC_TRF_PORTA);
  ac_track_param(s, 2, 1, 0xCF, 9); ACT_EQ(k->porta_time, 9);
  ac_track_param(s, 2, 1, 0xCA, 30); ACT_EQ(k->mod.depth, 30);
  ac_track_param(s, 2, 1, 0xCB, 20); ACT_EQ(k->mod.speed, 20);
  ac_track_param(s, 2, 1, 0xCC, 2); ACT_EQ(k->mod.target, 2);
  ac_track_param(s, 2, 1, 0xCD, 3); ACT_EQ(k->mod.range, 3);
  ac_track_param(s, 2, 1, 0xE0, 300); ACT_EQ(k->mod.delay, 300);
  ac_track_param(s, 2, 1, 0xD0, 1); ACT_EQ(k->attack, 1);
  ac_track_param(s, 2, 1, 0xD1, 2); ACT_EQ(k->decay, 2);
  ac_track_param(s, 2, 1, 0xD2, 3); ACT_EQ(k->sustain, 3);
  ac_track_param(s, 2, 1, 0xD3, 4); ACT_EQ(k->release, 4);
  ac_track_param(s, 2, 1, 0xD5, 50); ACT_EQ(k->volume2, 50);
  ac_track_param(s, 2, 1, 0xE1, 150); ACT_EQ(pl->tempo, 150);
  ac_track_param(s, 2, 1, 0xE3, -300); ACT_EQ(k->sweep_pitch, -300);
  AC_Track before = *k;
  ac_track_param(s, 2, 1, 0xD6, 1);
  ACT_CHECK(memcmp(&before, k, sizeof before) == 0);
}

/* two channels chained through nextLink get the same values */
static void chain(AcoreState *s, int t, int c0, int c1) {
  s->drv.work_addr = 0x02300000;
  s->work.track[t].channel_list = AC_EXCH_ADDR(s, c0);
  s->work.channel[c0].nextLink = AC_EXCH_ADDR(s, c1);
  s->work.channel[c1].nextLink = 0;
}

ACT_TEST(p08_db_composition) {
  AcoreState *s = act_state();
  int ready = db_ready();
  ac_track_init(s, 0);
  s->work.player[0].volume = 127;
  int v, fd, pn, pt;
  ac_p08_mix(s, 0, 0, &v, &fd, &pn, &pt);
  ACT_EQ(v, 0);
  if (ready) { ACT_CHECK(ac_decibel(64) < 0); ACT_CHECK(ac_decibel(0) < ac_decibel(64)); } /* piece 6 real */
  s->work.track[0].volume = 64;
  ac_p08_mix(s, 0, 0, &v, &fd, &pn, &pt);
  ACT_EQ(v, ac_decibel_sq(64));
  s->work.track[0].volume2 = 64;
  ac_p08_mix(s, 0, 0, &v, &fd, &pn, &pt);
  ACT_EQ(v, 2 * ac_decibel_sq(64));
  s->work.track[0].volume = 0;
  s->work.track[0].volume2 = 127;
  ac_p08_mix(s, 0, 0, &v, &fd, &pn, &pt);
  ACT_EQ(v, ac_decibel(0));
  s->work.track[0].volume = 127;
  s->work.track[0].volume2 = 0;
  s->work.player[0].volume = 64;
  s->work.player[0].extFader = -10;
  s->work.track[0].extFader = -20;
  ac_p08_mix(s, 0, 0, &v, &fd, &pn, &pt);
  ACT_EQ(v, -32768); /* decibel(0) + decibel(64) clamps to s16 (the oracle, piece 8's rule) */
  ACT_EQ(fd, -30);
  chain(s, 0, 2, 5);
  ac_track_update_channels(s, 0, 0, 0);
  ACT_EQ(s->work.channel[2].user_decay, v);
  ACT_EQ(s->work.channel[5].user_decay, v);
  ACT_EQ(s->work.channel[5].user_decay2, -30);
}

ACT_TEST(p08_bend_range2) {
  AcoreState *s = act_state();
  ac_track_init(s, 0);
  s->work.player[0].volume = 127;
  int v, fd, pn, pt;
  ac_track_param(s, 0, 0, 0xC4, 127);
  ac_p08_mix(s, 0, 0, &v, &fd, &pn, &pt);
  ACT_EQ(pt, 127); /* 127/128 of 2 semitones = 127 sixty-fourths */
  ac_track_param(s, 0, 0, 0xC4, -127);
  ac_p08_mix(s, 0, 0, &v, &fd, &pn, &pt);
  ACT_EQ(pt, -127);
  ac_track_param(s, 0, 0, 0xC4, 64);
  ac_p08_mix(s, 0, 0, &v, &fd, &pn, &pt);
  ACT_EQ(pt, 64);
  s->work.track[0].ext_pitch = 30;
  ac_p08_mix(s, 0, 0, &v, &fd, &pn, &pt);
  ACT_EQ(pt, 94);
  chain(s, 0, 1, 3);
  ac_track_update_channels(s, 0, 0, 0);
  ACT_EQ(s->work.channel[1].user_pitch, 94);
  ACT_EQ(s->work.channel[3].user_pitch, 94);
}

ACT_TEST(p08_pan_and_mod_pushed) {
  AcoreState *s = act_state();
  ac_track_init(s, 4);
  chain(s, 4, 6, 7);
  ac_track_param(s, 0, 4, 0xC0, 64 + 30);
  s->work.track[4].ext_pan = 5;
  ac_track_update_channels(s, 0, 4, 0);
  ACT_EQ(s->work.channel[6].user_pan, 35);
  ACT_EQ(s->work.channel[7].pan_range, 127);
  s->work.track[4].pan_range = 64;
  ac_track_update_channels(s, 0, 4, 0);
  ACT_EQ(s->work.channel[6].user_pan, 15 + 5);
  ACT_EQ(s->work.channel[7].pan_range, 64);
  ac_track_param(s, 0, 4, 0xCA, 40);
  ac_track_param(s, 0, 4, 0xCC, 1);
  ac_track_param(s, 0, 4, 0xE0, 10);
  ac_track_update_channels(s, 0, 4, 0);
  ACT_EQ(s->work.channel[7].lfo.param.depth, 40);
  ACT_EQ(s->work.channel[7].lfo.param.target, 1);
  ACT_EQ(s->work.channel[7].lfo.param.delay, 10);
  ACT_EQ(s->work.channel[7].lfo.param.speed, 16);
}
