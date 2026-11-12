/* port/acore piece 9: unit tests (docs/acore.md, piece 9's "Tests:"). OWNED BY PIECE 9.
 *
 * The expected numbers come from the format docs and the oracle (the driver info and the shared work read back after
 * START_SEQ + PLAYER_PARAM scenes; see p09_player.c's header). The tests stand on piece 9 alone: the sequence in
 * memory is `wait 3000, end`, which the real piece 7 and the stub both leave running. */
#include "act.h"

#define SEQ 0x02100000u
static const u8 seq_wait[] = {0x80, 0x97, 0x38, 0xFF};

static AcoreState *mk(void) {
  AcoreState *s = act_state();
  act_mem(s, SEQ, seq_wait, sizeof seq_wait);
  return s;
}
static AC_Player *PLY(AcoreState *s, int p) { return &s->work.player[p]; }
static AC_Track *TRK(AcoreState *s, int t) { return &s->work.track[t]; }

/* a started player at tempo/ratio (the way the game sets them: after the start) */
static void start_at(AcoreState *s, int p, int tempo, int ratio) {
  ac_player_start(s, p, SEQ, 0, 0);
  ac_player_param(s, p, 0x18, (u32)tempo, 2);
  ac_player_param(s, p, 0x1A, (u32)ratio, 2);
}

/* ---- ticks per wake (oracle: tick counts and tempo counters after each of 8 periodic wakes) ---- */
static void ticks_check(int tempo, int ratio, const int *cum, const int *cnt) {
  AcoreState *s = mk();
  start_at(s, 0, tempo, ratio);
  ACT_EQ(PLY(s, 0)->tempo_counter, 240);
  for (int w = 0; w < 8; w++) {
    ac_seq_main(s, 1);
    ACT_EQ(s->p09.ticks[0], cum[w]);
    ACT_EQ(PLY(s, 0)->tempo_counter, cnt[w]);
  }
}
ACT_TEST(p09_ticks_tempo120) {
  /* 120 * 256 >> 8 = 120 a wake: a tick every second wake, the first at once */
  static const int cum[8] = {1, 1, 2, 2, 3, 3, 4, 4}, cnt[8] = {120, 240, 120, 240, 120, 240, 120, 240};
  ticks_check(120, 256, cum, cnt);
}
ACT_TEST(p09_ticks_tempo240) {
  static const int cum[8] = {1, 2, 3, 4, 5, 6, 7, 8}, cnt[8] = {240, 240, 240, 240, 240, 240, 240, 240};
  ticks_check(240, 256, cum, cnt);
}
ACT_TEST(p09_ticks_ratio128) {
  /* 120 * 128 >> 8 = 60 a wake: a tick every fourth wake (oracle: counter 60, 120, 180, 240, then 60) */
  static const int cum[8] = {1, 1, 1, 1, 2, 2, 2, 2}, cnt[8] = {60, 120, 180, 240, 60, 120, 180, 240};
  ticks_check(120, 128, cum, cnt);
}
ACT_TEST(p09_ticks_tempo255) {
  /* 255 a wake: the remainder grows (oracle: 255, 270, 285, 300 ...) and one tick a wake */
  static const int cum[8] = {1, 2, 3, 4, 5, 6, 7, 8}, cnt[8] = {255, 270, 285, 300, 315, 330, 345, 360};
  ticks_check(255, 256, cum, cnt);
}
ACT_TEST(p09_ticks_two_a_wake) {
  /* 120 * 1000 >> 8 = 468: the loop runs twice when the counter holds 480 (oracle: 468, 696, 684, 672 ...) */
  static const int cum[8] = {1, 2, 4, 6, 8, 10, 12, 14}, cnt[8] = {468, 696, 684, 672, 660, 648, 636, 624};
  ticks_check(120, 1000, cum, cnt);
}
ACT_TEST(p09_nonperiodic_wake_does_not_tick) {
  AcoreState *s = mk();
  start_at(s, 0, 240, 256);
  ac_seq_main(s, 1);
  ACT_EQ(s->p09.ticks[0], 1);
  ac_seq_main(s, 0);
  ac_seq_main(s, 0);
  ACT_EQ(s->p09.ticks[0], 1);
  ACT_EQ(PLY(s, 0)->tempo_counter, 240);
  ac_seq_main(s, 1);
  ACT_EQ(s->p09.ticks[0], 2);
}
ACT_TEST(p09_tick_counter_in_shared_work) {
  AcoreState *s = mk();
  s->drv.shared = AC_PLAN_SHARED;
  start_at(s, 2, 240, 256);
  ac_seq_main(s, 1);
  ac_seq_main(s, 1);
  ACT_EQ(ac_rd32(s, AC_PLAN_SHARED + AC_SW_PLAYER_TICKS(2)), 2);
  ac_player_start(s, 2, SEQ, 0, 0); /* a new start zeroes it */
  ACT_EQ(ac_rd32(s, AC_PLAN_SHARED + AC_SW_PLAYER_TICKS(2)), 0);
}

/* ---- start / stop (oracle: flags 03 -> 02 after a stop, 01 -> 00 for a prepared one) ---- */
ACT_TEST(p09_start_stop) {
  AcoreState *s = mk();
  /* params sent before the start are lost: the start resets the player */
  ac_player_param(s, 0, 4, 99, 1);
  ac_player_param(s, 0, 5, 50, 1);
  ac_player_start(s, 0, SEQ, 0, 0x02101000);
  AC_Player *pl = PLY(s, 0);
  ACT_EQ(pl->flags, AC_PLF_ACTIVE | AC_PLF_PREPARED);
  ACT_EQ(pl->myNo, 0);
  ACT_EQ(pl->prio, 64);
  ACT_EQ(pl->volume, 127);
  ACT_EQ(pl->extFader, 0);
  ACT_EQ(pl->tempo, 120);
  ACT_EQ(pl->tempo_ratio, 256);
  ACT_EQ(pl->tempo_counter, 240);
  ACT_EQ(pl->bank, 0x02101000);
  ACT_EQ(pl->tracks[0], 0);
  for (int i = 1; i < 16; i++) ACT_EQ(pl->tracks[i], 0xFF);
  AC_Track *t = TRK(s, 0);
  ACT_CHECK(t->flags & AC_TRF_ACTIVE);
  ACT_EQ(t->base, SEQ);
  ACT_EQ(t->cur, SEQ);
  ac_seq_main(s, 1);
  ac_player_stop(s, 0);
  ACT_EQ(pl->flags, AC_PLF_PREPARED);
  ACT_EQ(pl->tracks[0], 0xFF);
  ACT_CHECK(!(TRK(s, 0)->flags & AC_TRF_ACTIVE));
  ac_seq_main(s, 1);
  ACT_EQ(s->p09.ticks[0], 1); /* stopped: no more ticks */
}
ACT_TEST(p09_start_with_offset_second_player) {
  AcoreState *s = mk();
  ac_player_start(s, 0, SEQ, 0, 0);
  ac_player_start(s, 5, SEQ - 0x10, 0x10, 0);
  ACT_EQ(PLY(s, 5)->tracks[0], 1); /* the next free track of the pool */
  ACT_EQ(TRK(s, 1)->cur, SEQ);
  ACT_EQ(TRK(s, 1)->base, SEQ - 0x10);
  ac_player_stop(s, 0);
  ACT_EQ(PLY(s, 5)->flags, 3);
}
ACT_TEST(p09_restart_resets) {
  AcoreState *s = mk();
  start_at(s, 0, 240, 100);
  ac_seq_main(s, 1);
  ac_player_start(s, 0, SEQ, 0, 0);
  ACT_EQ(PLY(s, 0)->tempo, 120);
  ACT_EQ(PLY(s, 0)->tempo_ratio, 256);
  ACT_EQ(PLY(s, 0)->tracks[0], 0); /* the old track was freed first: the same index again */
  ACT_EQ(s->p09.ticks[0], 0);
}

/* ---- prepare + start (oracle: PREPARE -> flags 01, no ticks; START_PREPARED -> 03 and the first tick at once) ---- */
ACT_TEST(p09_prepare_then_start) {
  AcoreState *s = mk();
  ac_player_prepare(s, 0, SEQ, 0, 0);
  ACT_EQ(PLY(s, 0)->flags, AC_PLF_ACTIVE);
  ACT_CHECK(TRK(s, 0)->flags & AC_TRF_ACTIVE);
  ac_seq_main(s, 1);
  ac_seq_main(s, 1);
  ACT_EQ(s->p09.ticks[0], 0); /* prepared only: not running */
  ACT_EQ(PLY(s, 0)->tempo_counter, 240);
  ac_player_param(s, 0, 0x18, 240, 2); /* the game sets params between the two */
  ac_player_start_prepared(s, 0);
  ACT_EQ(PLY(s, 0)->flags, AC_PLF_ACTIVE | AC_PLF_PREPARED);
  ac_seq_main(s, 1);
  ACT_EQ(s->p09.ticks[0], 1);
  ACT_EQ(PLY(s, 0)->tempo, 240);
}
ACT_TEST(p09_prepare_then_stop) {
  AcoreState *s = mk();
  ac_player_prepare(s, 0, SEQ, 0, 0);
  ac_player_stop(s, 0);
  ACT_EQ(PLY(s, 0)->flags, 0); /* never started: no prepared bit left */
  ACT_CHECK(!(TRK(s, 0)->flags & AC_TRF_ACTIVE));
}

/* ---- pause (oracle: flag 04; no ticks and no tempo accumulation while paused) ---- */
ACT_TEST(p09_pause) {
  AcoreState *s = mk();
  start_at(s, 0, 240, 256);
  ac_seq_main(s, 1);
  ac_seq_main(s, 1);
  ACT_EQ(s->p09.ticks[0], 2);
  ac_player_pause(s, 0, 1);
  ACT_EQ(PLY(s, 0)->flags, AC_PLF_ACTIVE | AC_PLF_PREPARED | AC_PLF_PAUSE);
  ac_seq_main(s, 1);
  ac_seq_main(s, 1);
  ACT_EQ(s->p09.ticks[0], 2);
  ACT_EQ(PLY(s, 0)->tempo_counter, 240);
  ac_player_pause(s, 0, 0);
  ACT_EQ(PLY(s, 0)->flags, AC_PLF_ACTIVE | AC_PLF_PREPARED);
  ac_seq_main(s, 1);
  ACT_EQ(s->p09.ticks[0], 3); /* the oracle: one tick on the first wake after, none made up */
}

/* ---- skip ---- */
ACT_TEST(p09_skip) {
  AcoreState *s = mk();
  start_at(s, 0, 120, 256);
  ac_seq_main(s, 1);
  ACT_EQ(s->p09.ticks[0], 1);
  ac_player_skip(s, 0, 100);
  ACT_EQ(s->p09.ticks[0], 101); /* the tick counter advances by N (oracle) */
  ACT_EQ(PLY(s, 0)->flags, AC_PLF_ACTIVE | AC_PLF_PREPARED);
  ACT_CHECK(!(TRK(s, 0)->flags & AC_TRF_MUTE)); /* the tracks are muted only for the skip */
  ac_player_skip(s, 0, 0);
  ACT_EQ(s->p09.ticks[0], 101);
  ac_track_mute(s, 0, 1, AC_MUTE_NO_STOP);
  ac_player_skip(s, 0, 3);
  ACT_CHECK(TRK(s, 0)->flags & AC_TRF_MUTE); /* a mute set before stays */
  ac_player_skip(s, 7, 10);                  /* an idle player: nothing */
  ACT_EQ(s->p09.ticks[7], 0);
}

/* ---- the end of a player ---- */
ACT_TEST(p09_ends_when_tracks_end) {
  AcoreState *s = mk();
  start_at(s, 0, 240, 256);
  ac_player_open_track(s, 0, 1, SEQ);
  ac_seq_main(s, 1);
  ACT_EQ(PLY(s, 0)->flags, AC_PLF_ACTIVE | AC_PLF_PREPARED);
  TRK(s, 0)->flags &= (u8)~AC_TRF_ACTIVE; /* track 0 ends: the player goes on with track 1 */
  ac_seq_main(s, 1);
  ACT_EQ(PLY(s, 0)->flags, AC_PLF_ACTIVE | AC_PLF_PREPARED);
  TRK(s, 1)->flags &= (u8)~AC_TRF_ACTIVE; /* the last one: the player ends (oracle: flags 02, tracks ff) */
  ac_seq_main(s, 1);
  ACT_EQ(PLY(s, 0)->flags, AC_PLF_PREPARED);
  for (int i = 0; i < 16; i++) ACT_EQ(PLY(s, 0)->tracks[i], 0xFF);
}

/* ---- the pool of 32 tracks (oracle: 16 + 16, the third player gets none: flags 02; 15 + 15 leaves 2) ---- */
static void fill(AcoreState *s, int p, int n) {
  ac_player_start(s, p, SEQ, 0, 0);
  for (int i = 1; i < n; i++) ac_player_open_track(s, p, i, SEQ);
}
ACT_TEST(p09_track_pool_exhaustion) {
  AcoreState *s = mk();
  fill(s, 0, 16);
  fill(s, 1, 16);
  for (int i = 0; i < 16; i++) {
    ACT_EQ(PLY(s, 0)->tracks[i], i);
    ACT_EQ(PLY(s, 1)->tracks[i], 16 + i);
  }
  ac_player_start(s, 2, SEQ, 0, 0);
  ACT_EQ(PLY(s, 2)->flags, AC_PLF_PREPARED); /* no track: not active (the START's second half still sets bit 1) */
  ACT_EQ(PLY(s, 2)->tracks[0], 0xFF);
  ACT_EQ(PLY(s, 2)->tempo, 120); /* the reset happened before the failed alloc */
  ac_player_stop(s, 0);
  ac_player_start(s, 2, SEQ, 0, 0);
  ACT_EQ(PLY(s, 2)->flags, AC_PLF_ACTIVE | AC_PLF_PREPARED);
  for (int i = 0; i < 16; i++) ACT_EQ(PLY(s, 2)->tracks[i], i == 0 ? 0 : 0xFF);
}
ACT_TEST(p09_track_pool_partial) {
  AcoreState *s = mk();
  fill(s, 0, 15);
  fill(s, 1, 15);
  ACT_EQ(PLY(s, 1)->tracks[0], 15);
  fill(s, 2, 15);
  ACT_EQ(PLY(s, 2)->tracks[0], 30);
  ACT_EQ(PLY(s, 2)->tracks[1], 31);
  ACT_EQ(PLY(s, 2)->tracks[2], 0xFF);
  ACT_EQ(ac_player_open_track(s, 2, 2, SEQ), 0); /* the pool is empty: no track */
  ACT_EQ(ac_player_open_track(s, 0, 1, SEQ), 0); /* the slot is taken */
  ACT_EQ(ac_player_open_track(s, 0, 16, SEQ), 0);
}
ACT_TEST(p09_track_alloc_lowest_free) {
  AcoreState *s = mk();
  ac_player_start(s, 0, SEQ, 0, 0);
  ac_player_start(s, 1, SEQ, 0, 0);
  ac_player_start(s, 2, SEQ, 0, 0);
  ACT_EQ(PLY(s, 2)->tracks[0], 2);
  ac_player_stop(s, 1); /* track 1 free again (oracle: reuse) */
  ac_player_start(s, 3, SEQ, 0, 0);
  ACT_EQ(PLY(s, 3)->tracks[0], 1);
  ACT_EQ(ac_track_alloc(s), 3);
}
ACT_TEST(p09_open_track_base) {
  AcoreState *s = mk();
  ac_player_start(s, 0, SEQ, 0, 0);
  ACT_EQ(ac_player_open_track(s, 0, 3, SEQ + 2), 1);
  AC_Track *t = TRK(s, PLY(s, 0)->tracks[3]);
  ACT_CHECK(t->flags & AC_TRF_ACTIVE);
  ACT_EQ(t->cur, SEQ + 2);
  ACT_EQ(t->base, SEQ); /* the player's sequence base (oracle: all tracks share it) */
}

/* ---- mute (oracle: the flag 04 for 1, 2, 3; off clears it) ---- */
ACT_TEST(p09_mute_modes) {
  AcoreState *s = mk();
  ac_player_start(s, 0, SEQ, 0, 0);
  ac_player_open_track(s, 0, 1, SEQ);
  ac_track_mute(s, 0, 1, AC_MUTE_NO_STOP);
  ACT_CHECK(TRK(s, 0)->flags & AC_TRF_MUTE);
  ACT_CHECK(!(TRK(s, 1)->flags & AC_TRF_MUTE));
  ac_track_mute(s, 0, 1, AC_MUTE_OFF);
  ACT_CHECK(!(TRK(s, 0)->flags & AC_TRF_MUTE));
  ac_track_mute(s, 0, 2, AC_MUTE_RELEASE);
  ACT_CHECK(TRK(s, 1)->flags & AC_TRF_MUTE);
  ACT_CHECK(!(TRK(s, 0)->flags & AC_TRF_MUTE));
  ac_track_mute(s, 0, 3, AC_MUTE_STOP);
  ACT_CHECK(TRK(s, 0)->flags & AC_TRF_MUTE);
  ac_track_mute(s, 0, 0xFFFF, AC_MUTE_OFF);
  ACT_CHECK(!(TRK(s, 0)->flags & AC_TRF_MUTE) && !(TRK(s, 1)->flags & AC_TRF_MUTE));
  ACT_CHECK(TRK(s, 0)->flags & AC_TRF_ACTIVE); /* muting never ends a track */
  /* a mute sent before the start finds no track (the oracle: the note sounds) and is lost */
  ac_track_mute(s, 4, 1, AC_MUTE_STOP);
  ac_player_start(s, 4, SEQ, 0, 0);
  ACT_CHECK(!(TRK(s, PLY(s, 4)->tracks[0])->flags & AC_TRF_MUTE));
}

/* ---- the raw parameter writes (oracle: player prio 99, volume 50, extFader -256, ratio 100; track volume 77 on
 * tracks 0-1, prio 55 on 1-2, a word at 0x20 = the wait) ---- */
ACT_TEST(p09_player_param_raw) {
  AcoreState *s = mk();
  ac_player_start(s, 0, SEQ, 0, 0);
  ac_player_param(s, 0, 4, 99, 1);
  ac_player_param(s, 0, 5, 50, 1);
  ac_player_param(s, 0, 6, 0xFF00, 2);
  ac_player_param(s, 0, 0x1A, 100, 2);
  ACT_EQ(PLY(s, 0)->prio, 99);
  ACT_EQ(PLY(s, 0)->volume, 50);
  ACT_EQ(PLY(s, 0)->extFader, -256);
  ACT_EQ(PLY(s, 0)->tempo_ratio, 100);
  ac_player_param(s, 0, 0x20, 0x11223344, 4);
  ACT_EQ(PLY(s, 0)->bank, 0x11223344);
  ac_player_param(s, 0, 0x24, 1, 1); /* past the struct: dropped */
  ac_player_param(s, 0, 0x1E, 0xAAAABBBB, 3); /* a bad size: dropped */
  ACT_EQ(PLY(s, 0)->pad__, 0);
  ac_player_param(s, 16, 4, 1, 1);
}
ACT_TEST(p09_track_param_raw) {
  AcoreState *s = mk();
  ac_player_start(s, 0, SEQ, 0, 0);
  ac_player_open_track(s, 0, 1, SEQ);
  ac_player_open_track(s, 0, 2, SEQ);
  ac_track_param_raw(s, 0, 3, 4, 77, 1);
  ACT_EQ(TRK(s, 0)->volume, 77);
  ACT_EQ(TRK(s, 1)->volume, 77);
  ACT_CHECK(TRK(s, 2)->volume != 77);
  ac_track_param_raw(s, 0, 6, 0x12, 55, 2);
  ACT_EQ(TRK(s, 1)->prio, 55);
  ACT_EQ(TRK(s, 2)->prio, 55);
  ACT_EQ(TRK(s, 2)->transpose, 0);
  ac_track_param_raw(s, 0, 4, 0x20, 0x12345678, 4);
  ACT_EQ(TRK(s, 2)->wait, 0x12345678);
  ac_track_param_raw(s, 0, 0xFFF8, 4, 9, 1); /* slots without a track: nothing */
  ACT_CHECK(TRK(s, 3)->volume != 9);
  ac_track_param_raw(s, 0, 1, 0x3E, 9, 4); /* past the struct */
  ac_track_param_raw(s, 0, 1, 0x40, 9, 1);
  ACT_CHECK(TRK(s, 0)->volume == 77);
}
ACT_TEST(p09_allocatable_channels) {
  AcoreState *s = mk();
  ac_player_start(s, 0, SEQ, 0, 0);
  ac_track_allocatable(s, 0, 1, 0x00F0);
  ACT_EQ(TRK(s, 0)->channel_mask, 0x00F0);
  ACT_CHECK(TRK(s, 0)->flags & AC_TRF_CHANNEL_MASK);
}

/* ---- streams for the oracle (tools in the report: START_SEQ + params + mute / pause / stop scenes) ---- */
ACT_TEST(p09_emit_scene) {
  AcoreState *s = mk();
  act_mem(s, SEQ + 0x100, (const u8[]){0xE1, 0xF0, 0x00, 0x80, 0x97, 0x38, 0xFF}, 7);
  act_cmd(s, AC_CMD_MASTER_VOLUME, 127, 0, 0, 0);
  act_cmd(s, AC_CMD_START_SEQ, 0, SEQ + 0x100, 0, 0);
  act_flush(s);
  act_run(s, 6 * AC_DRIVER_PERIOD);
  act_cmd(s, AC_CMD_PLAYER_PARAM, 0, 0x1A, 128, 2);
  act_cmd(s, AC_CMD_PAUSE_SEQ, 0, 1, 0, 0);
  act_send(s);
  act_run(s, 4 * AC_DRIVER_PERIOD);
  act_cmd(s, AC_CMD_PAUSE_SEQ, 0, 0, 0, 0);
  act_cmd(s, AC_CMD_MUTE_TRACK, 0, 1, AC_MUTE_RELEASE, 0);
  act_send(s);
  act_run(s, 6 * AC_DRIVER_PERIOD);
  act_cmd(s, AC_CMD_STOP_SEQ, 0, 0, 0, 0);
  act_send(s);
  act_run(s, 4 * AC_DRIVER_PERIOD);
  act_emit(s, "p09_scene");
}
