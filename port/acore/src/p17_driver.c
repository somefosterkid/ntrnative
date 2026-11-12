/* port/acore piece 17: The driver loop and integration (docs/acore.md). OWNED BY PIECE 17. */
#include "ac_shared.h"

/* SND_Init's state, checked against the oracle: the DS driver's work image read back by READ_DRIVER_INFO before any
 * sequence has run holds channel[i].myNo = i, player[i].myNo = i and zeros elsewhere (a player's track slots are 0, not
 * 0xFF); the boot's only sound-register writes are SOUNDCNT = 0x807F (byte writes 0x80 to +1, 0x7F to +0: master
 * enable, master volume 0x7F, output selectors 0, no pan/hold bits). That boot is not part of a stream, so the
 * register is set here without a trace line. Channels are off (their blocks zero). */
void ac_driver_init(AcoreState *s) {
  for (int i = 0; i < AC_NCH; i++) s->work.channel[i].myNo = (u8)i;
  for (int i = 0; i < AC_NPLAYER; i++) s->work.player[i].myNo = (u8)i;
  s->io[AC_REG_SOUNDCNT] = 0x7F;
  s->io[AC_REG_SOUNDCNT + 1] = 0x80;
  s->io[AC_REG_SOUNDBIAS] = 0x00; /* the reset value 0x200 (the boot never writes it) */
  s->io[AC_REG_SOUNDBIAS + 1] = 0x02;
}

/* One wake of the ARM7's sound thread, in the SDK's order: start/stop what the last step flagged, run every queued
 * command list, tick the sequencer, update the hardware channels, publish the shared work, advance the random
 * generator. The periodic alarm and a wakeup word run the same steps; only `periodic` differs (the sequencer and
 * the channel update advance time only on the alarm). */
void ac_driver_wake(AcoreState *s, int periodic) {
  s->drv.in_wake = 1;
  s->drv.wake_t = s->now;
  s->drv.cur = s->drv.pass_word ? AC_T_PRO_WORD : AC_T_PRO;
  ac_exch_sync(s);
  s->drv.autogap = 1;
  s->drv.last_addr = 0;
  ac_cmd_proc(s);
  s->drv.autogap = 0;
  ac_seq_main(s, periodic);
  ac_exch_main(s, periodic);
  ac_shared_update(s);
  ac_random(s);
  ac_drv_wait(s, 300); /* SeqMain's idle part, ExChannelMain, UpdateSharedWork */
  s->drv.free_at = s->drv.wake_t + s->drv.cur;
  s->drv.in_wake = 0;
}

/* One wake of the music clock (bus split): the music players' share of a periodic wake: the channels the last step flagged
 * start or stop, the music players' sequences tick, their channels' envelopes, LFOs and sweeps advance. The commands, the
 * shared work and the random generator stay with the main clock. The register writes land at once (the music clock has
 * no wake latency to model). */
void ac_music_wake(AcoreState *s) {
  u64 cur = s->drv.cur;
  ac_exch_sync(s);
  ac_seq_main(s, 1);
  ac_exch_main(s, 1);
  s->drv.cur = cur;
}

/* The time the driver needs to reach a command's first register write, from the oracle's traces (cmd_proc starts
 * about 450 after the sync loop; the gaps between a list's writes: lock -> setup 214, setup -> start 296, start ->
 * channel volume ~700, volume -> timer ~233; MASTER_VOLUME at the first wake 895). A command without writes costs
 * 150 (a guess: the START_SEQ of drv_note takes ~5900 in all). */
void ac_drv_cmd_time(AcoreState *s, u32 id) {
  static const u16 cost[AC_CMD_COUNT] = {[AC_CMD_START_SEQ] = 5800, [AC_CMD_LOCK_CHANNEL] = 446,
    [AC_CMD_SETUP_CHANNEL_PCM] = 214, [AC_CMD_START_TIMER] = 296, [AC_CMD_CHANNEL_VOLUME] = 700,
    [AC_CMD_CHANNEL_TIMER] = 233, [AC_CMD_MASTER_VOLUME] = 895};
  ac_drv_wait(s, id < AC_CMD_COUNT && cost[id] ? cost[id] : 150);
}
