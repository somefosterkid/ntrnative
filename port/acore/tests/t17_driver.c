/* port/acore piece 17: unit tests (docs/acore.md, piece 17's "Tests:"). OWNED BY PIECE 17. */
#include "act.h"
#include <string.h>

/* SND_Init's state: channels off, master volume 0x7F, selectors 0 (checked against the oracle's READ_DRIVER_INFO) */
ACT_TEST(p17_init_state) {
  AcoreState *s = act_state();
  ACT_EQ(ac_hw_read(s, 0x04000500, 2), 0x807F);
  for (int ch = 0; ch < AC_NCH; ch++) {
    ACT_EQ(ac_ch_reg32(s, ch, AC_CH_CNT) & AC_CNT_START, 0);
    ACT_EQ(s->work.channel[ch].myNo, ch);
  }
  for (int p = 0; p < AC_NPLAYER; p++) ACT_EQ(s->work.player[p].myNo, p);
  ACT_EQ(s->drv.nqueue, 0);
  ACT_EQ(s->drv.wakes, 0);
}

/* the periodic alarm: tick0 + k * 174592, the first one at tick0 itself */
ACT_TEST(p17_tick0_default) {
  AcoreState *s = act_state();
  act_run(s, 1);
  ACT_EQ(s->drv.wakes, 1); /* the wake at cycle 0 */
  act_run(s, AC_DRIVER_PERIOD - 1);
  ACT_EQ(s->drv.wakes, 1);
  act_run(s, 1); /* now = 174592: the second */
  ACT_EQ(s->drv.wakes, 2);
  ACT_EQ(s->drv.next_tick, 2 * AC_DRIVER_PERIOD);
  act_run(s, 10 * AC_DRIVER_PERIOD);
  ACT_EQ(s->drv.wakes, 12);
}

ACT_TEST(p17_tick0_info) {
  /* a stream with INFO tick0=1000 (the loader sets next_tick) */
  static const char info[] = "mode=driver\ntick0=1000\n";
  u8 f[8 + 8 + sizeof info + 4 + 16];
  size_t n = 0;
  memcpy(f, "ACST", 4); f[4] = 1; memset(f + 5, 0, 3); n = 8;
  memcpy(f + n, "INFO", 4); u32 sz = sizeof info - 1; memcpy(f + n + 4, &sz, 4); memcpy(f + n + 8, info, sz);
  n += 8 + ((sz + 3) & ~3u);
  memcpy(f + n, "END ", 4); sz = 8; memcpy(f + n + 4, &sz, 4); u64 t = 1000 + 2 * AC_DRIVER_PERIOD;
  memcpy(f + n + 8, &t, 8); n += 16;
  AcoreState *s = acore_create();
  char err[128];
  ACT_EQ(acore_load_acs_mem(s, f, n, err, sizeof err), 0);
  ACT_EQ(s->drv.tick0, 1000);
  acore_run(s, 1000 + AC_LAG_PERIODIC); /* the wake lands AC_LAG_PERIODIC after the alarm (ai58) */
  ACT_EQ(s->drv.wakes, 0);
  acore_run(s, 1001 + AC_LAG_PERIODIC);
  ACT_EQ(s->drv.wakes, 1);
  acore_render_all(s); /* up to END, exclusive: the wakes at 1000 and 175592 */
  ACT_EQ(s->drv.wakes, 2);
  acore_destroy(s);
}

/* a hardware-mode stream has no periodic wakes */
ACT_TEST(p17_hw_no_wakes) {
  AcoreState *s = act_state_hw();
  act_run(s, 3 * AC_DRIVER_PERIOD);
  ACT_EQ(s->drv.wakes, 0);
}

/* the end of a stream: samples k * 1024 below END, the clock stops at END */
ACT_TEST(p17_end_of_stream) {
  AcoreState *s = act_state();
  act_run(s, 10 * AC_CYCLES_PER_SAMPLE);
  ACT_EQ(s->nsamples, 10);
  ACT_EQ(s->now, 10 * AC_CYCLES_PER_SAMPLE);
  act_run(s, 1); /* sample 10 is at cycle 10240, so it is taken once the clock passes it */
  ACT_EQ(s->nsamples, 11);
  ACT_EQ(act_sample(s, 10, 0), act_sample(s, 9, 0)); /* silence: whatever the mixer's bias gives (piece 4) */
  ACT_EQ(act_sample(s, 11, 0), 0); /* not rendered yet */
}

/* the wake order: a command list and the periodic tick at the same cycle: the list is queued first, the tick runs it */
ACT_TEST(p17_wake_order_cmd_then_tick) {
  ACT_TODO("p15 (ac_cmd_proc drains the queue) and p16 (finishCommandTag): a MASTER_VOLUME list and a tick in one wake");
}
ACT_TEST(p17_wake_order_sync_first) {
  ACT_TODO("p11 + p14: a channel flagged by wake n is started in wake n+1's ac_exch_sync, before that wake's commands");
}
ACT_TEST(p17_end_to_end_note) {
  ACT_TODO("p07-p15: START_SEQ of a one-note sequence, the channel's registers; stream p17_note for the oracle");
}
