/* port/acore: the music bus's unit tests (docs/acore.md "Game speed and sound"). The bus is the port's own
 * (no oracle: the DS has one clock): what these check is the bookkeeping that makes the split a no-op for everything that is
 * not a music player's note, and that joining again keeps the music's timing. */
#include "act.h"
#include <string.h>

ACT_TEST(p18_slot_flag_fifo) { /* the flag of each start is taken by the sequence that starts next on the slot, in order */
  AcoreState *s = act_state();
  acore_bus_slot(s, 3, 1);
  acore_bus_slot(s, 3, 0);
  ac_bus_player_started(s, 3);
  ACT_EQ(s->bus.music_players >> 3 & 1, 1);
  ac_bus_player_started(s, 3);
  ACT_EQ(s->bus.music_players >> 3 & 1, 0);
  ac_bus_player_started(s, 3); /* no flag left: the slot's last one (0) */
  ACT_EQ(s->bus.music_players >> 3 & 1, 0);
  acore_bus_slot(s, 2, 1);
  ac_bus_player_started(s, 2);
  ac_bus_player_started(s, 2); /* no flag left: the slot's last one (1) */
  ACT_EQ(s->bus.music_players >> 2 & 1, 1);
}

ACT_TEST(p18_domains) { /* dom 0 covers everything, 1 all but the music bus, 2 the music bus alone */
  AcoreState *s = act_state();
  s->bus.music_players = 1u << 2;
  s->bus.music_ch = 1u << 5;
  s->bus.dom = 0;
  ACT_CHECK(ac_dom_ch(s, 5) && ac_dom_ch(s, 4) && ac_dom_pl(s, 2) && ac_dom_pl(s, 1));
  s->bus.dom = 1;
  ACT_CHECK(!ac_dom_ch(s, 5) && ac_dom_ch(s, 4) && !ac_dom_pl(s, 2) && ac_dom_pl(s, 1));
  s->bus.dom = 2;
  ACT_CHECK(ac_dom_ch(s, 5) && !ac_dom_ch(s, 4) && ac_dom_pl(s, 2) && !ac_dom_pl(s, 1));
}

ACT_TEST(p18_lock_clears_music_tag) { /* a locked channel is the game's own (a direct channel setup), not a music note */
  AcoreState *s = act_state();
  s->bus.music_ch = 0x00FF;
  ac_exch_lock(s, 0x000F, 0);
  ACT_EQ(s->bus.music_ch, 0x00F0);
}

ACT_TEST(p18_not_split_makes_nothing) {
  AcoreState *s = act_state();
  int16_t buf[64];
  ACT_EQ(acore_bus_run(s, 32, buf), 0);
  ACT_EQ(acore_bus_split_on(s), 0);
}

ACT_TEST(p18_split_run_counts) { /* a split bus makes exactly the frames asked for, on a clock of its own */
  AcoreState *s = act_state();
  act_run(s, 5 * AC_CYCLES_PER_SAMPLE + 100);
  acore_bus_split(s, 1);
  ACT_EQ(acore_bus_split_on(s), 1);
  u64 n0 = s->nsamples, m0 = s->bus.mnsamples;
  ACT_EQ(m0, n0); /* the music clock starts where the main clock is */
  int16_t buf[2 * 1000];
  ACT_EQ(acore_bus_run(s, 1000, buf), 1000);
  ACT_EQ(s->bus.mnsamples, m0 + 1000);
  ACT_EQ(s->nsamples, n0); /* the main clock did not move */
  ACT_EQ(s->bus.dom, 1);   /* and the main clock's domain is back */
}

ACT_TEST(p18_join_keeps_the_wake_phase) {
  /* the main clock's next periodic wake after a join is as far from now as the music clock's next wake was from the music's
   * own time (else the first tick after the join came up to half a period early or late: a timing jump of the music) */
  AcoreState *s = act_state();
  s->drv.on = 1;
  act_run(s, 3 * AC_DRIVER_PERIOD + 12345);
  acore_bus_split(s, 1);
  int16_t buf[2 * 4096];
  ACT_EQ(acore_bus_run(s, 777, buf), 777); /* not a whole number of wake periods */
  u64 t = s->bus.mnsamples * AC_CYCLES_PER_SAMPLE + s->sample_phase;
  u64 rem = s->bus.mnext_tick - t;
  ACT_CHECK(s->bus.mnext_tick > t && rem <= AC_DRIVER_PERIOD);
  u64 now = s->now;
  acore_bus_split(s, 0);
  ACT_EQ(s->drv.next_tick + s->drv.lag_periodic, now + rem);
  ACT_EQ(acore_bus_split_on(s), 0);
}
