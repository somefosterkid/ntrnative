/* port/acore piece 11: unit tests (docs/acore.md, piece 11's "Tests:"). OWNED BY PIECE 11.
 *
 * The expected values are the oracle's (driver mode, streams from tools/p11_streams.py, summarised in p11_alloc.h):
 * the allocation order, the steal rules, the lock sets and the register writes of a sync. The sync cases use the
 * writes of the game's own notes (title clip: a one-shot ADPCM channel `5000026B, FDF5, 1, 70FD, SAD`).
 */
#include "act.h"
#include <string.h>

typedef struct W {
  unsigned addr, size, value;
} W;

static FILE *tr_open(AcoreState *s) {
  FILE *f = tmpfile();
  acore_set_trace(s, f);
  return f;
}

/* the writes traced since tr_open (rewinds and parses the file) */
static int tr_read(AcoreState *s, FILE *f, W *w, int max) {
  int n = 0;
  unsigned long long t;
  fflush(f);
  rewind(f);
  while (n < max && fscanf(f, "%llu %x %u %x", &t, &w[n].addr, &w[n].size, &w[n].value) == 4) n++;
  acore_set_trace(s, NULL);
  fclose(f);
  return n;
}

#define CH(n, off) (0x04000400u + 0x10u * (n) + (off))

static AC_ExChannel *chan(AcoreState *s, int ch) { return &s->work.channel[ch]; }

/* a channel set the way piece 10 / 14 leave it for a start */
static void setup_pcm(AcoreState *s, int ch, int fmt, int loop, u16 vol, u8 pan, u16 timer, u32 data, u16 lstart, u32 llen) {
  AC_ExChannel *c = chan(s, ch);
  c->type = AC_EX_PCM;
  c->wave.format = (u8)fmt;
  c->wave.loopflag = (u8)loop;
  c->wave.loopstart = lstart;
  c->wave.looplen = llen;
  c->volume = vol;
  c->pan = pan;
  c->timer = timer;
  c->data = data;
}

static const int ORDER[16] = {4, 5, 6, 7, 2, 0, 3, 1, 8, 9, 10, 11, 14, 12, 15, 13};

ACT_TEST(p11_alloc_order) {
  AcoreState *s = act_state();
  for (int i = 0; i < 16; i++) ACT_EQ(ac_exch_alloc(s, 0xFFFF, 64, 0, 0), ORDER[i]);
  /* all busy, all equal (the quietest hardware volume wins, a tie goes to the first of the order: oracle, the title
     clip's steals; these channels all read volume 0) */
  ACT_EQ(ac_exch_alloc(s, 0xFFFF, 64, 0, 0), 4);
  ACT_EQ(ac_exch_alloc(s, 0xFFFF, 64, 0, 0), 4);
  act_emit(s, "p11_order");
}

ACT_TEST(p11_alloc_kinds) {
  AcoreState *s = act_state();
  for (int i = 8; i < 14; i++) ACT_EQ(ac_exch_alloc(s, 0x3F00, 64, 0, 1), i); /* PSG 8-13 in order */
  ACT_EQ(ac_exch_alloc(s, 0x3F00, 64, 0, 1), 8);                            /* the 7th and 8th steal the first */
  ACT_EQ(ac_exch_alloc(s, 0x3F00, 64, 0, 1), 8);
  ACT_EQ(ac_exch_alloc(s, 0xC000, 64, 0, 1), 14); /* noise 14, 15 */
  ACT_EQ(ac_exch_alloc(s, 0xC000, 64, 0, 1), 15);
  ACT_EQ(ac_exch_alloc(s, 0xC000, 64, 0, 1), 14); /* the third steals the first */
  ACT_EQ(ac_exch_alloc(s, 0, 64, 0, 1), -1);      /* an empty mask */
  AcoreState *s2 = act_state();
  ACT_EQ(ac_exch_alloc(s2, 0x00A0, 64, 0, 1), 5); /* a track's own mask: 5 before 7 */
  ACT_EQ(ac_exch_alloc(s2, 0x00A0, 64, 0, 1), 7);
}

ACT_TEST(p11_steal_priority) {
  /* x_prio_inc: priorities 40, 42, ... on the channels of the order, the 17th (72) takes the lowest (ch4) */
  AcoreState *s = act_state();
  for (int i = 0; i < 16; i++) ACT_EQ(ac_exch_alloc(s, 0xFFFF, 40 + 2 * i, 0, 0), ORDER[i]);
  ACT_EQ(ac_exch_alloc(s, 0xFFFF, 72, 0, 0), 4);
  ACT_EQ(chan(s, 4)->prio, 72);
  /* x_prio_dec: priorities 100, 98, ...: the lowest (70, ch13) is the victim, a 68 gets nothing, a 70 does */
  AcoreState *d = act_state();
  for (int i = 0; i < 16; i++) ACT_EQ(ac_exch_alloc(d, 0xFFFF, 100 - 2 * i, 0, 0), ORDER[i]);
  ACT_EQ(ac_exch_alloc(d, 0xFFFF, 68, 0, 0), -1);
  ACT_EQ(chan(d, 13)->prio, 70); /* untouched */
  ACT_EQ(ac_exch_alloc(d, 0xFFFF, 68, 1, 0), 13); /* strong: no priority check */
  ACT_EQ(ac_exch_alloc(d, 0xFFFF, 90, 0, 0), 13);
  /* x_prio_low: 16 notes of 80, a 10 gets nothing */
  AcoreState *l = act_state();
  for (int i = 0; i < 16; i++) ACT_EQ(ac_exch_alloc(l, 0xFFFF, 80, 0, 0), ORDER[i]);
  ACT_EQ(ac_exch_alloc(l, 0xFFFF, 10, 0, 0), -1);
  /* a failed allocation changes nothing */
  ACT_EQ(chan(l, 13)->prio, 80);
  act_emit(l, "p11_prio");
}

ACT_TEST(p11_steal_level_and_release) {
  AcoreState *s = act_state();
  for (int i = 0; i < 16; i++) ACT_EQ(ac_exch_alloc(s, 0xFFFF, 64, 0, 0), ORDER[i]);
  /* a released channel has priority 0: stolen before every other, even if its note had priority 100 (x_rel2),
     and by a priority 0 note (x_relp0) */
  chan(s, 2)->prio = 100;
  ac_exch_release(s, 2);
  ACT_EQ(chan(s, 2)->prio, 0);
  ACT_EQ(chan(s, 2)->env_status, AC_ENV_RELEASE);
  ACT_EQ(ac_exch_alloc(s, 0xFFFF, 0, 0, 0), 2);
  /* on equal priorities the quieter channel goes first (x_lvl: the decayed note on ch4 was the victim); this needs
     piece 12's level to follow env_decay, so it only runs once that piece reports one */
  AcoreState *t = act_state();
  for (int i = 0; i < 16; i++) ACT_EQ(ac_exch_alloc(t, 0xFFFF, 64, 0, 0), ORDER[i]);
  chan(t, 4)->env_decay = -20000;
  chan(t, 13)->env_decay = 0;
  if (ac_env_level(t, 4) != ac_env_level(t, 13)) {
    int louder_is_larger = ac_env_level(t, 4) < ac_env_level(t, 13);
    ACT_EQ(ac_exch_alloc(t, 0xFFFF, 64, 0, 0), louder_is_larger ? 4 : 13);
  }
  act_emit(t, "p11_release");
}

ACT_TEST(p11_lock_mask) {
  AcoreState *s = act_state();
  FILE *f = tr_open(s);
  chan(s, 5)->flags |= AC_EXF_ACTIVE;
  ac_exch_lock(s, 0x30, 0);
  W w[8];
  int n = tr_read(s, f, w, 8);
  ACT_EQ(n, 2); /* a stop write per channel of the mask, idle or not, in channel order */
  ACT_EQ(w[0].addr, CH(4, 0));
  ACT_EQ(w[0].size, 4);
  ACT_EQ(w[0].value, 0);
  ACT_EQ(w[1].addr, CH(5, 0));
  ACT_EQ(chan(s, 5)->flags & AC_EXF_ACTIVE, 0);
  ACT_EQ(ac_exch_locked(s), 0x30);
  /* the locked channels are never allocated: 6 7 2 0 */
  ACT_EQ(ac_exch_alloc(s, 0xFFFF, 64, 0, 0), 6);
  ACT_EQ(ac_exch_alloc(s, 0xFFFF, 64, 0, 0), 7);
  ACT_EQ(ac_exch_alloc(s, 0xFFFF, 64, 0, 0), 2);
  ACT_EQ(ac_exch_alloc(s, 0x30, 64, 0, 0), -1);
  /* flags: bit 0 picks the set; set 1 keeps channels from allocation but READ_DRIVER_INFO's mask is set 0 */
  ac_exch_lock(s, 0x300, 1);
  ACT_EQ(ac_exch_locked(s), 0x30);
  ACT_EQ(ac_exch_alloc(s, 0x300, 64, 0, 0), -1);
  ac_exch_lock(s, 0xF000, 2); /* flags 2 = set 0 */
  ACT_EQ(ac_exch_locked(s), 0xF030);
  ac_exch_unlock(s, 0x1000, 0);
  ACT_EQ(ac_exch_locked(s), 0xE030);
  ac_exch_unlock(s, 0x2000, 3); /* flags 3 = set 1: set 0 keeps it */
  ACT_EQ(ac_exch_locked(s), 0xE030);
  ac_exch_unlock(s, 0x300, 1);
  ACT_EQ(ac_exch_alloc(s, 0x300, 64, 0, 0), 8);
  ac_exch_unlock(s, 0x10, 0);
  ACT_EQ(ac_exch_alloc(s, 0x30, 64, 0, 0), 4);
  /* x_lockinfo: the oracle's READ_DRIVER_INFO mask after each step was 30 30 20 20 20 F020 E020 F020 F020 */
  AcoreState *o = act_state();
  ac_exch_lock(o, 0x30, 0);
  ac_exch_lock(o, 0x300, 1);
  ac_exch_unlock(o, 0x10, 0);
  ac_exch_unlock(o, 0x100, 0);
  ac_exch_unlock(o, 0x300, 1);
  ACT_EQ(ac_exch_locked(o), 0x20);
  ac_exch_lock(o, 0xF000, 2);
  ac_exch_unlock(o, 0x1000, 0);
  ACT_EQ(ac_exch_locked(o), 0xE020);
  ac_exch_lock(o, 0x1000, 0);
  ac_exch_unlock(o, 0x1000, 3);
  ACT_EQ(ac_exch_locked(o), 0xF020);
}

ACT_TEST(p11_stop_unlocked) {
  AcoreState *s = act_state();
  act_reg(s, CH(4, 0), 0xA840004F, 4); /* running */
  ac_exch_lock(s, 0x20, 0);
  FILE *f = tr_open(s);
  ac_exch_stop_unlocked(s, 0x30, 0); /* 4 unlocked: its CNT without the start bit; 5 is locked */
  W w[4];
  int n = tr_read(s, f, w, 4);
  ACT_EQ(n, 1);
  ACT_EQ(w[0].addr, CH(4, 0));
  ACT_EQ(w[0].size, 4);
  ACT_EQ(w[0].value, 0x2840004F);
  f = tr_open(s);
  ac_exch_stop_unlocked(s, 0x40, 0); /* idle channels get the write too (oracle: CNT = 0) */
  n = tr_read(s, f, w, 4);
  ACT_EQ(n, 1);
  ACT_EQ(w[0].value, 0);
}

ACT_TEST(p11_track_list) {
  AcoreState *s = act_state();
  int a = ac_exch_alloc(s, 0xFFFF, 64, 0, 3), b = ac_exch_alloc(s, 0xFFFF, 64, 0, 3), c = ac_exch_alloc(s, 0xFFFF, 64, 0, 5);
  ACT_EQ(a, 4);
  ACT_EQ(b, 5);
  ACT_EQ(c, 6);
  ACT_EQ(ac_track_first_channel(s, 3), 5); /* newest first */
  ACT_EQ(ac_track_next_channel(s, 5), 4);
  ACT_EQ(ac_track_next_channel(s, 4), -1);
  ACT_EQ(ac_track_first_channel(s, 5), 6);
  ACT_EQ(ac_track_first_channel(s, 0), -1);
  ACT_EQ(chan(s, 5)->callback_data, 3);
  ACT_CHECK(chan(s, 5)->callback != 0);
  /* stolen: the old track forgets the channel */
  for (int i = 3; i < 16; i++) ac_exch_alloc(s, 0xFFFF, 64, 0, 7);
  ACT_EQ(ac_exch_alloc(s, 0x2000, 64, 0, 9), 13);
  ACT_EQ(ac_track_first_channel(s, 9), 13);
  ACT_EQ(ac_track_first_channel(s, 7), 15);
  /* free takes it out of the middle of a list too */
  ac_exch_free(s, 5);
  ACT_EQ(ac_track_first_channel(s, 3), 4);
  ACT_EQ(chan(s, 5)->flags & AC_EXF_ACTIVE, 0);
  ACT_EQ(chan(s, 5)->callback, 0);
  /* ... and is allocated again first */
  ACT_EQ(ac_exch_alloc(s, 0xFFFF, 64, 0, 3), 5);
  ac_track_free_channels(s, 3);
  ACT_EQ(ac_track_first_channel(s, 3), -1);
  /* the channels let go of the track and play on (oracle, the intro clip: a sequence's end leaves its notes releasing) */
  ACT_CHECK(chan(s, 4)->flags & AC_EXF_ACTIVE);
  ACT_EQ(chan(s, 4)->callback, 0);
  ACT_CHECK(chan(s, 5)->flags & AC_EXF_ACTIVE);
  /* the release of a track's channels: all of them, the track's own release rate when given */
  ac_track_release_channels(s, 0, 5, -1);
  ACT_EQ(chan(s, 6)->env_status, AC_ENV_RELEASE);
  ACT_EQ(chan(s, 6)->prio, 0);
  ACT_CHECK(chan(s, 8)->env_status != AC_ENV_RELEASE);
}

ACT_TEST(p11_sync_pcm_start) {
  AcoreState *s = act_state();
  int ch = ac_exch_alloc(s, 0xFFFF, 64, 0, 0);
  ACT_EQ(ch, 4);
  /* the title clip's ADPCM one-shot, left half: 5000026B, TMR FDF5, PNT 1, LEN 70FD, SAD 021624BC */
  setup_pcm(s, ch, AC_FMT_ADPCM, 0, 0x026B, 0, 0x020B, 0x021624BC, 1, 0x70FD);
  ac_exch_start(s, ch, 100);
  ACT_CHECK(chan(s, ch)->flags & AC_EXF_START);
  ac_exch_set_sync(s, ch, AC_SYNC_START);
  FILE *f = tr_open(s);
  ac_exch_sync(s);
  W w[16];
  int n = tr_read(s, f, w, 16);
  ACT_EQ(n, 7);
  ACT_EQ(w[0].addr, CH(4, 0)); ACT_EQ(w[0].size, 4); ACT_EQ(w[0].value, 0);
  ACT_EQ(w[1].addr, CH(4, 0)); ACT_EQ(w[1].size, 4); ACT_EQ(w[1].value, 0x5000026B);
  ACT_EQ(w[2].addr, CH(4, 8)); ACT_EQ(w[2].size, 2); ACT_EQ(w[2].value, 0xFDF5);
  ACT_EQ(w[3].addr, CH(4, 10)); ACT_EQ(w[3].size, 2); ACT_EQ(w[3].value, 1);
  ACT_EQ(w[4].addr, CH(4, 12)); ACT_EQ(w[4].size, 4); ACT_EQ(w[4].value, 0x70FD);
  ACT_EQ(w[5].addr, CH(4, 4)); ACT_EQ(w[5].size, 4); ACT_EQ(w[5].value, 0x021624BC);
  ACT_EQ(w[6].addr, CH(4, 3)); ACT_EQ(w[6].size, 1); ACT_EQ(w[6].value, 0xD0);
  ACT_EQ(ac_exch_sync_flags(s, ch), 0);
  ACT_EQ(ac_hw_read(s, CH(4, 0), 4) >> 31, 1); /* the hardware channel started */
  /* the drv_note stream's channel: PCM16 loop, `28400304`, TMR FF76, PNT 0, LEN 20, SAD 0210204C, byte A8 */
  AcoreState *t = act_state();
  ACT_EQ(ac_exch_alloc(t, 0xFFFF, 64, 0, 0), 4);
  setup_pcm(t, 4, AC_FMT_PCM16, 1, 0x0304, 64, 0x8A, 0x0210204C, 0, 0x20);
  ac_exch_start(t, 4, 48);
  ac_exch_set_sync(t, 4, AC_SYNC_START);
  f = tr_open(t);
  ac_exch_sync(t);
  n = tr_read(t, f, w, 16);
  ACT_EQ(n, 7);
  ACT_EQ(w[1].value, 0x28400304);
  ACT_EQ(w[2].value, 0xFF76);
  ACT_EQ(w[6].value, 0xA8);
  act_emit(t, "p11_start");
}

ACT_TEST(p11_sync_psg_noise) {
  AcoreState *s = act_state();
  /* x_psg8: duty 4 at volume 6 divider 3: `64400306`, TMR E0BC, start byte E4; no PNT / LEN / SAD */
  AC_ExChannel *c = chan(s, 8);
  c->type = AC_EX_PSG, c->data = 4, c->volume = 0x0306, c->pan = 64, c->timer = 0x1F44;
  c->flags |= AC_EXF_ACTIVE;
  ac_exch_set_sync(s, 8, AC_SYNC_START);
  FILE *f = tr_open(s);
  ac_exch_sync(s);
  W w[8];
  int n = tr_read(s, f, w, 8);
  ACT_EQ(n, 4);
  ACT_EQ(w[1].addr, CH(8, 0)); ACT_EQ(w[1].value, 0x64400306);
  ACT_EQ(w[2].addr, CH(8, 8)); ACT_EQ(w[2].value, 0xE0BC);
  ACT_EQ(w[3].addr, CH(8, 3)); ACT_EQ(w[3].value, 0xE4);
  /* noise: `60400306`, TMR E0BA, start byte E0 */
  c = chan(s, 14);
  c->type = AC_EX_NOISE, c->data = 5 /* ignored */, c->volume = 0x0306, c->pan = 64, c->timer = 0x1F46;
  c->flags |= AC_EXF_ACTIVE;
  ac_exch_set_sync(s, 14, AC_SYNC_START);
  f = tr_open(s);
  ac_exch_sync(s);
  n = tr_read(s, f, w, 8);
  ACT_EQ(n, 4);
  ACT_EQ(w[1].value, 0x60400306);
  ACT_EQ(w[2].value, 0xE0BA);
  ACT_EQ(w[3].value, 0xE0);
}

ACT_TEST(p11_sync_order_and_flags) {
  AcoreState *s = act_state();
  W w[32];
  /* two channels started in one wake: both setups (channel order), then both start bytes (title clip, 4 and 5) */
  for (int ch = 5; ch >= 4; ch--) {
    setup_pcm(s, ch, AC_FMT_ADPCM, 0, 0x026B, ch == 4 ? 0 : 0x7F, 0x020B, 0x021624BC, 1, 0x70FD);
    s->work.channel[ch].flags |= AC_EXF_ACTIVE;
    ac_exch_set_sync(s, ch, AC_SYNC_START);
  }
  FILE *f = tr_open(s);
  ac_exch_sync(s);
  int n = tr_read(s, f, w, 32);
  ACT_EQ(n, 14);
  ACT_EQ(w[1].value, 0x5000026B);
  ACT_EQ(w[7].value, 0x507F026B);
  ACT_EQ(w[12].addr, CH(4, 3)); ACT_EQ(w[12].value, 0xD0);
  ACT_EQ(w[13].addr, CH(5, 3)); ACT_EQ(w[13].value, 0xD0);
  /* timer, volume, pan on a running channel: in that order, each its own write (title: `FDD6`, `0302`, pan byte) */
  AC_ExChannel *c = chan(s, 4);
  c->timer = 0x2AA, c->volume = 0x0302, c->pan = 5;
  ac_exch_set_sync(s, 4, AC_SYNC_PAN | AC_SYNC_VOLUME | AC_SYNC_TIMER);
  f = tr_open(s);
  ac_exch_sync(s);
  n = tr_read(s, f, w, 32);
  ACT_EQ(n, 3);
  ACT_EQ(w[0].addr, CH(4, 8)); ACT_EQ(w[0].size, 2); ACT_EQ(w[0].value, 0xFD56);
  ACT_EQ(w[1].addr, CH(4, 0)); ACT_EQ(w[1].size, 2); ACT_EQ(w[1].value, 0x0302);
  ACT_EQ(w[2].addr, CH(4, 2)); ACT_EQ(w[2].size, 1); ACT_EQ(w[2].value, 5);
  /* only the flagged ones, then nothing: a second sync writes nothing */
  ac_exch_set_sync(s, 4, AC_SYNC_VOLUME);
  f = tr_open(s);
  ac_exch_sync(s);
  ACT_EQ(tr_read(s, f, w, 32), 1);
  f = tr_open(s);
  ac_exch_sync(s);
  ACT_EQ(tr_read(s, f, w, 32), 0);
  /* a stop: CNT without the start bit (4 bytes), the other flags dropped */
  ac_exch_set_sync(s, 4, AC_SYNC_VOLUME);
  ac_exch_set_sync(s, 4, AC_SYNC_STOP);
  ACT_EQ(ac_exch_sync_flags(s, 4), AC_SYNC_STOP);
  f = tr_open(s);
  ac_exch_sync(s);
  n = tr_read(s, f, w, 32);
  ACT_EQ(n, 1);
  ACT_EQ(w[0].addr, CH(4, 0)); ACT_EQ(w[0].size, 4); ACT_EQ(w[0].value, 0x50050302 /* the volume and pan writes are in the register */);
  ACT_EQ(ac_hw_read(s, CH(4, 0), 4) >> 31, 0);
}

ACT_TEST(p11_free_stolen_same_wake) {
  /* x_prio_dec's end: a channel stolen by a note that is freed again before the sync: the old note's hardware is
     stopped, the new one never starts (the oracle shows `stopw ch13` and no START) */
  AcoreState *s = act_state();
  W w[8];
  for (int i = 0; i < 16; i++) ac_exch_alloc(s, 0xFFFF, 64, 0, 0);
  setup_pcm(s, 13, AC_FMT_PCM16, 1, 0x0304, 64, 0x8A, 0x0210204C, 0, 0x20);
  ac_exch_start(s, 13, 48);
  ac_exch_set_sync(s, 13, AC_SYNC_START);
  ac_exch_sync(s); /* ch13 running */
  ACT_EQ(ac_exch_alloc(s, 0x2000, 64, 0, 1), 13);
  ac_exch_set_sync(s, 13, AC_SYNC_START); /* the new note */
  ac_exch_free(s, 13);
  ACT_CHECK(ac_exch_sync_flags(s, 13) == AC_SYNC_STOP);
  /* a channel that never ran: nothing to stop */
  int fresh = ac_exch_alloc(s, 1u << 2, 200, 0, 1); /* channel 2 steals, was never started */
  ACT_EQ(fresh, 2);
  ac_exch_set_sync(s, 2, AC_SYNC_START);
  ac_exch_free(s, 2);
  ACT_EQ(ac_exch_sync_flags(s, 2), 0);
  FILE *f = tr_open(s);
  ac_exch_sync(s);
  ACT_EQ(tr_read(s, f, w, 8), 1);
  ACT_EQ(w[0].addr, CH(13, 0));
}

/* the oracle scenarios, for tools/acore_compare.py (ACT_EMIT=DIR): lock flags, stop unlocked, locks of a running
   channel; needs pieces 15 and 17 to be heard */
ACT_TEST(p11_emit_lock_scenarios) {
  AcoreState *s = act_state();
  act_cmd(s, AC_CMD_LOCK_CHANNEL, 0x30, 0, 0, 0);
  act_flush(s);
  act_run(s, 2 * AC_CYCLES_PER_FRAME);
  act_cmd(s, AC_CMD_LOCK_CHANNEL, 0x300, 1, 0, 0);
  act_flush(s);
  act_run(s, 2 * AC_CYCLES_PER_FRAME);
  act_cmd(s, AC_CMD_UNLOCK_CHANNEL, 0x10, 0, 0, 0);
  act_flush(s);
  act_run(s, 2 * AC_CYCLES_PER_FRAME);
  act_cmd(s, AC_CMD_STOP_UNLOCKED_CHANNEL, 0x30, 0, 0, 0);
  act_flush(s);
  act_run(s, 2 * AC_CYCLES_PER_FRAME);
  act_emit(s, "p11_locks");
}
