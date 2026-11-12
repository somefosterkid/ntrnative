/* port/acore piece 16: unit tests (docs/acore.md, piece 16's "Tests:"). OWNED BY PIECE 16. */
#include "act.h"

#define SW 0x0237F000u

static AcoreState *with_shared(void) {
  AcoreState *s = act_state();
  s->drv.shared = SW;
  return s;
}

ACT_TEST(p16_layout) {
  /* SNDSharedWork: finishCommandTag, playerStatus, channelStatus, captureStatus, 16 players x (16 s16 + tick u32),
   * 16 global s16: GBATEK-less, from lib/nitro/sdk/nitro/snd/common/work.h */
  ACT_EQ(AC_SW_FINISH_TAG, 0);
  ACT_EQ(AC_SW_PLAYER_STATUS, 4);
  ACT_EQ(AC_SW_CHANNEL_STATUS, 8);
  ACT_EQ(AC_SW_CAPTURE_STATUS, 0xA);
  ACT_EQ(AC_SW_PLAYER(0), 0x20);
  ACT_EQ(AC_SW_PLAYER(1), 0x44);
  ACT_EQ(AC_SW_PLAYER_VAR(2, 3), 0x20 + 2 * 0x24 + 6);
  ACT_EQ(AC_SW_PLAYER_TICKS(0), 0x40);
  ACT_EQ(AC_SW_PLAYER(15) + 0x24, AC_SW_GLOBAL_VAR(0));
  ACT_EQ(AC_SW_GLOBAL_VAR(15) + 2, AC_SW_SIZE);
}

ACT_TEST(p16_vars_in_shared_work) {
  AcoreState *s = with_shared();
  ac_var_set(s, 2, 3, 1234);
  ACT_EQ(ac_rd16(s, SW + 0x20 + 2 * 0x24 + 6), 1234);
  ACT_EQ(ac_var_get(s, 2, 3), 1234);
  ac_var_set(s, -1, 5, -77);
  ACT_EQ(ac_rd16(s, SW + 0x260 + 10), 0xFFB3);
  ACT_EQ(ac_var_get(s, -1, 5), -77);
  /* the ARM9 side writes the memory: the driver reads it back */
  act_mem(s, SW + AC_SW_PLAYER_VAR(15, 15), "\x34\x82", 2);
  ACT_EQ(ac_var_get(s, 15, 15), (s16)0x8234);
  /* neighbours untouched */
  ACT_EQ(ac_rd16(s, SW + 0x20 + 2 * 0x24 + 4), 0);
  ACT_EQ(ac_var_get(s, 15, 14), 0);
}

ACT_TEST(p16_vars_fallback_store) {
  AcoreState *s = act_state(); /* no SHARED_WORK yet */
  ACT_EQ(ac_var_get(s, 0, 0), -1);
  ACT_EQ(ac_var_get(s, -1, 15), -1);
  ac_var_set(s, 4, 9, 321);
  ac_var_set(s, -1, 2, -5);
  ACT_EQ(ac_var_get(s, 4, 9), 321);
  ACT_EQ(ac_var_get(s, 4, 8), -1);
  ACT_EQ(ac_var_get(s, 3, 9), -1);
  ACT_EQ(ac_var_get(s, -1, 2), -5);
  ACT_EQ(ac_rd16(s, SW + AC_SW_PLAYER_VAR(4, 9)), 0); /* nothing written to memory */
}

ACT_TEST(p16_vars_bad_index) {
  AcoreState *s = with_shared();
  ac_var_set(s, 0, 16, 5);
  ac_var_set(s, 16, 0, 5);
  ac_var_set(s, 0, -1, 5);
  ACT_EQ(ac_var_get(s, 0, 16), -1);
  ACT_EQ(ac_var_get(s, 16, 0), -1);
  ACT_EQ(ac_rd16(s, SW + AC_SW_PLAYER_VAR(1, 0)), 0); /* player 0's var 16 would be player 1's tick area */
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_TICKS(0)), 0);
}

ACT_TEST(p16_vars_reset_player) {
  AcoreState *s = with_shared();
  ac_var_set(s, 1, 0, 7);
  ac_var_set(s, 1, 15, 8);
  ac_var_set(s, 2, 0, 9);
  ac_var_set(s, -1, 0, 10);
  ac_var_reset_player(s, 1);
  ACT_EQ(ac_var_get(s, 1, 0), -1);
  ACT_EQ(ac_var_get(s, 1, 15), -1);
  ACT_EQ(ac_var_get(s, 2, 0), 9);
  ACT_EQ(ac_var_get(s, -1, 0), 10);
}

ACT_TEST(p16_no_shared_work_no_writes) {
  AcoreState *s = act_state();
  s->work.player[0].flags = AC_PLF_ACTIVE;
  ac_shared_update(s);
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_STATUS), 0);
}

ACT_TEST(p16_player_status) {
  AcoreState *s = with_shared();
  ac_shared_update(s);
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_STATUS), 0);
  s->work.player[0].flags = AC_PLF_ACTIVE; /* a start */
  s->work.player[5].flags = AC_PLF_ACTIVE | AC_PLF_PAUSE;
  s->work.player[7].flags = AC_PLF_PREPARED; /* prepared, not active */
  ac_shared_update(s);
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_STATUS), 0x21);
  s->work.player[0].flags = 0; /* an end */
  ac_shared_update(s);
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_STATUS), 0x20);
}

ACT_TEST(p16_channel_status) {
  AcoreState *s = with_shared();
  ac_hw_set_busy(s, 4, 1);
  ac_hw_set_busy(s, 15, 1);
  ac_shared_update(s);
  ACT_EQ(ac_rd16(s, SW + AC_SW_CHANNEL_STATUS), 0x8010);
  ac_hw_set_busy(s, 4, 0); /* a one-shot's end, or STOP_TIMER */
  ac_shared_update(s);
  ACT_EQ(ac_rd16(s, SW + AC_SW_CHANNEL_STATUS), 0x8000);
  ACT_EQ(ac_rd16(s, SW + AC_SW_CAPTURE_STATUS), 0);
}

ACT_TEST(p16_capture_status) {
  AcoreState *s = with_shared();
  act_reg(s, 0x04000000 + AC_REG_SNDCAP1CNT, 0x80, 1);
  ac_shared_update(s);
  ACT_EQ(ac_rd16(s, SW + AC_SW_CAPTURE_STATUS), 2);
  act_reg(s, 0x04000000 + AC_REG_SNDCAP0CNT, 0x80, 1);
  ac_shared_update(s);
  ACT_EQ(ac_rd16(s, SW + AC_SW_CAPTURE_STATUS), 3);
  act_reg(s, 0x04000000 + AC_REG_SNDCAP1CNT, 0, 1);
  ac_shared_update(s);
  ACT_EQ(ac_rd16(s, SW + AC_SW_CAPTURE_STATUS), 1);
}

ACT_TEST(p16_tick_counters) {
  AcoreState *s = with_shared();
  s->p09.ticks[0] = 5; /* the one tick counter is p09's (ai58); the shared work reads it */
  s->p09.ticks[3] = 1;
  ac_shared_update(s);
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_TICKS(0)), 5);
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_TICKS(3)), 1);
  ac_shared_update(s); /* a stopped player keeps its count (oracle) */
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_TICKS(0)), 5);
  s->p09.ticks[0] = 0; /* a restart counts from 0 (oracle; p09 resets it at START_SEQ) */
  ac_shared_update(s);
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_TICKS(0)), 0);
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_TICKS(3)), 1);
}

ACT_TEST(p16_wake_updates) {
  /* through the driver's wake: needs piece 17 */
  AcoreState *s = with_shared();
  s->work.player[1].flags = AC_PLF_ACTIVE;
  act_flush(s);
  if (!ac_rd32(s, SW + AC_SW_PLAYER_STATUS)) ACT_TODO("piece 17: ac_driver_wake calls ac_shared_update");
  ACT_EQ(ac_rd32(s, SW + AC_SW_PLAYER_STATUS), 2);
}

ACT_TEST(p16_command_vars) {
  AcoreState *s = with_shared();
  act_cmd(s, AC_CMD_PLAYER_LOCAL_VAR, 0, 3, 1234, 0);
  act_cmd(s, AC_CMD_PLAYER_GLOBAL_VAR, 5, 77, 0, 0);
  act_flush(s);
  if (ac_var_get(s, 0, 3) != 1234) ACT_TODO("piece 15/17: PLAYER_LOCAL_VAR / GLOBAL_VAR through the command path");
  ACT_EQ(ac_var_get(s, -1, 5), 77);
  ACT_EQ(ac_rd16(s, SW + AC_SW_PLAYER_VAR(0, 3)), 1234);
}

ACT_TEST(p16_sequence_var_ops) {
  /* 0xA1 (var = value) ... 0xB0 (set var) then read back: needs piece 7's ops */
  AcoreState *s = with_shared();
  ac_var_set(s, 0, 2, 42);
  ACT_EQ(ac_var_get(s, 0, 2), 42);
  ACT_TODO("piece 7: sequence ops 0xA1 / 0xB0 reading and writing ac_var_get / ac_var_set");
}
