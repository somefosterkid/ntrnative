/* port/acore piece 11: Channel allocation (docs/acore.md; the rules are written out in p11_alloc.h). */
#include "ac_shared.h"

/* the allocation order, measured on the oracle (17 PCM notes: 4 5 6 7 2 0 3 1 8 9 10 11 14 12 15 13) */
const unsigned char ac_exch_order[16] = {4, 5, 6, 7, 2, 0, 3, 1, 8, 9, 10, 11, 14, 12, 15, 13};

#define SYNC_OF(c) (((c)->flags >> AC_EXF_SYNC_SHIFT) & 0x1F)

static AC_ExChannel *chp(AcoreState *s, int ch) { return &s->work.channel[ch]; }

static u32 all_locked(const AcoreState *s) { return s->p11.lock[0] | s->p11.lock[1]; }

/* ---- the sync flags ---- */
void ac_exch_set_sync(AcoreState *s, int ch, int sync) {
  AC_ExChannel *c = chp(s, ch);
  u32 f = SYNC_OF(c);
  f = (sync & AC_SYNC_STOP) ? AC_SYNC_STOP : (f | (u32)sync) & 0x1F;
  c->flags = (u8)((c->flags & ((1u << AC_EXF_SYNC_SHIFT) - 1)) | f << AC_EXF_SYNC_SHIFT);
}

int ac_exch_sync_flags(const AcoreState *s, int ch) { return SYNC_OF(&s->work.channel[ch]); }

/* ---- the tracks' channel lists ---- */
int ac_track_first_channel(const AcoreState *s, int t) {
  if (t < 0 || t >= AC_NTRACK) return -1;
  int ch = AC_EXCH_NO(s, s->work.track[t].channel_list);
  return ch >= 0 && ch < AC_NCH ? ch : -1; /* an address that is no channel (0, or a test's stand-in) ends the list */
}

int ac_track_next_channel(const AcoreState *s, int ch) {
  if (ch < 0 || ch >= AC_NCH) return -1;
  int nx = AC_EXCH_NO(s, s->work.channel[ch].nextLink);
  return nx >= 0 && nx < AC_NCH ? nx : -1;
}

static void track_link(AcoreState *s, int t, int ch) {
  AC_ExChannel *c = chp(s, ch);
  c->nextLink = s->work.track[t].channel_list;
  s->work.track[t].channel_list = AC_EXCH_ADDR(s, ch);
}

static void track_unlink(AcoreState *s, int ch) {
  AC_ExChannel *c = chp(s, ch);
  if (c->callback) {
    int t = (int)c->callback_data;
    if (t >= 0 && t < AC_NTRACK) {
      u32 me = AC_EXCH_ADDR(s, ch);
      u32 *link = &s->work.track[t].channel_list;
      for (int guard = 0; *link && guard < AC_NCH; guard++) {
        if (*link == me) {
          *link = c->nextLink;
          break;
        }
        link = &s->work.channel[AC_EXCH_NO(s, *link)].nextLink;
      }
    }
  }
  c->callback = 0;
  c->callback_data = 0;
  c->nextLink = 0;
}

/* ---- allocation ---- */
static int amp_of(const AcoreState *s, int ch) {
  const AC_ExChannel *c = &s->work.channel[ch];
  /* the loudness of the hardware volume the last tick set (mantissa / divider: 1, 2, 4, 16), not the sum of its terms
     as the envelope stands now (oracle, the title clip: 55 of 57 steals match, the sum 54) */
  (void)s;
  static const unsigned char mul[4] = {16, 8, 4, 1};
  return (int)((c->volume & 0x7F) * mul[(c->volume >> 8) & 3]);
}

/* the player a track belongs to (-1: none) */
static int track_player(const AcoreState *s, int t) {
  for (int p = 0; p < AC_NPLAYER; p++)
    for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++)
      if (s->work.player[p].tracks[n] == t) return p;
  return -1;
}

int ac_exch_alloc(AcoreState *s, u32 chmask, int prio, int strong, int t) {
  int best = -1;
  u32 locked = all_locked(s);
  for (int i = 0; i < AC_NCH; i++) {
    int ch = ac_exch_order[i];
    if (!(chmask >> ch & 1) || (locked >> ch & 1)) continue;
    AC_ExChannel *c = chp(s, ch);
    if (!(c->flags & AC_EXF_ACTIVE)) {
      best = ch;
      break;
    }
    if (best < 0) {
      best = ch;
      continue;
    }
    /* lowest priority, then the quietest note (velocity + envelope + the track's decays; oracle, the title clip: 16
       released notes of other velocities and track volumes were taken in that order, the envelope alone is wrong);
       the later channel of the order wins a tie (oracle) */
    AC_ExChannel *b = chp(s, best);
    if (c->prio < b->prio || (c->prio == b->prio && amp_of(s, ch) < amp_of(s, best))) best = ch;
  }
  if (best < 0) return -1;
  AC_ExChannel *c = chp(s, best);
  if ((c->flags & AC_EXF_ACTIVE) && !strong && prio < c->prio) return -1;
  track_unlink(s, best); /* stolen: the old track forgets it */
  c->flags = (u8)((c->flags & ~(0x1F << AC_EXF_SYNC_SHIFT) & ~(AC_EXF_START | AC_EXF_AUTO_SWEEP)) | AC_EXF_ACTIVE);
  c->prio = (u8)prio;
  s->bus.music_ch &= (u16)~(1u << best); /* the music bus: the channel plays a music player's note (the sequence's own, not a direct channel) */
  if (t >= 0 && t < AC_NTRACK) {
    c->callback = 1;
    c->callback_data = (u32)t;
    track_link(s, t, best);
    int p = track_player(s, t);
    if (p >= 0 && (s->bus.music_players >> p & 1)) s->bus.music_ch |= (u16)(1u << best);
  }
  return best;
}

void ac_exch_start(AcoreState *s, int ch, s32 length) {
  AC_ExChannel *c = chp(s, ch);
  c->length = length;
  c->sweep_counter = 0;
  c->env_status = AC_ENV_ATTACK;
  ac_env_init(s, ch);
  ac_lfo_start(&c->lfo);
  c->flags |= AC_EXF_ACTIVE | AC_EXF_START;
}

/* the note is over (key off, a mute, the track ending): the release stage, and the lowest priority (oracle: a
 * priority 100 note in release was stolen by priority 64 notes, a priority 0 note too) */
void ac_exch_release(AcoreState *s, int ch) {
  AC_ExChannel *c = chp(s, ch);
  if (!(c->flags & AC_EXF_ACTIVE)) return;
  c->env_status = AC_ENV_RELEASE;
  c->prio = 0;
}

/* the channel is over: out of its track, inactive, and its hardware stopped at the next sync (when it ever
 * started, or the old note still runs) */
void ac_exch_free(AcoreState *s, int ch) {
  AC_ExChannel *c = chp(s, ch);
  int running = (int)(ac_hw_read(s, AC_REG_CH(ch) + AC_CH_CNT, 4) >> 31 & 1);
  int started = (s->p11.started >> ch & 1) || running;
  track_unlink(s, ch);
  c->flags &= (u8)~(AC_EXF_ACTIVE | AC_EXF_START);
  if (started)
    ac_exch_set_sync(s, ch, AC_SYNC_STOP);
  else
    c->flags &= (u8)~(0x1F << AC_EXF_SYNC_SHIFT);
}

void ac_track_release_channels(AcoreState *s, int p, int t, int release) {
  (void)p;
  for (int ch = ac_track_first_channel(s, t), n = 0; ch >= 0 && n < AC_NCH; ch = ac_track_next_channel(s, ch), n++) {
    if (release >= 0) ac_env_set_release(s, ch, release);
    ac_exch_release(s, ch);
  }
}

void ac_track_free_channels(AcoreState *s, int t) {
  int list[AC_NCH], n = 0;
  for (int ch = ac_track_first_channel(s, t); ch >= 0 && n < AC_NCH; ch = ac_track_next_channel(s, ch)) list[n++] = ch;
  /* the channels let go of the track but play their release out (oracle, the intro clip: a sequence's end keeps the
     released notes decaying; they end on their own and free themselves) */
  for (int i = 0; i < n; i++) track_unlink(s, list[i]);
  s->work.track[t].channel_list = 0;
}

/* ---- locks ---- */
static void hw_stop(AcoreState *s, int ch) {
  u32 cnt = ac_hw_read(s, AC_REG_CH(ch) + AC_CH_CNT, 4);
  ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT, cnt & ~AC_CNT_START, 4);
}

void ac_exch_lock(AcoreState *s, u32 chmask, u32 flags) {
  chmask &= 0xFFFF;
  for (int ch = 0; ch < AC_NCH; ch++) {
    if (!(chmask >> ch & 1)) continue;
    AC_ExChannel *c = chp(s, ch);
    if (c->flags & AC_EXF_ACTIVE) ac_exch_free(s, ch);
    c->flags &= (u8)~(0x1F << AC_EXF_SYNC_SHIFT); /* stopped right here */
    s->p11.started &= ~(1u << ch);
    hw_stop(s, ch);
  }
  s->bus.music_ch &= (u16)~chmask;
  s->p11.lock[flags & 1] |= chmask;
}

void ac_exch_unlock(AcoreState *s, u32 chmask, u32 flags) { s->p11.lock[flags & 1] &= ~chmask; }

u32 ac_exch_locked(const AcoreState *s) { return s->p11.lock[0]; }

void ac_exch_stop_unlocked(AcoreState *s, u32 chmask, u32 flags) {
  chmask &= 0xFFFF & ~s->p11.lock[flags & 1];
  for (int ch = 0; ch < AC_NCH; ch++)
    if (chmask >> ch & 1) hw_stop(s, ch);
}

/* ---- UpdateExChannel ---- */
static u32 start_cnt(const AC_ExChannel *c) {
  u32 cnt = (u32)(c->volume & 0x037F) | (u32)(c->pan & 0x7F) << 16;
  if (c->type == AC_EX_PCM)
    cnt |= (u32)(c->wave.format & 3) << 29 | (c->wave.loopflag ? AC_REPEAT_LOOP : AC_REPEAT_ONESHOT) << 27;
  else /* PSG: the duty is in `data`; noise has none; neither repeats */
    cnt |= (u32)AC_FMT_PSG << 29 | (c->type == AC_EX_PSG ? (c->data & 7u) << 24 : 0u);
  return cnt;
}

/* The driver's time inside UpdateExChannel, measured on the oracle's traces (every gap between two writes of the
 * title, intro, battle and menu clips; cycles). The first loop visits channels 0-15: an idle channel costs 14; a
 * channel to start stops it (write), writes CNT 117 later, TMR 7 after that (PCM: PNT +5, LEN +2, SAD +2) and is done
 * 43 after the last write; a volume write comes 38 into its channel and the channel is done 47 later, a pan write 40
 * and 43, a timer write 7 and 49 (timer then volume: 44 apart, done 47 after). The second loop writes the start
 * bytes: 28 per started channel, 25 for a skipped one the first loop wrote to, 14 for any other, and starts 5 before
 * the first loop's end. */
void ac_exch_sync(AcoreState *s) {
  u32 started = 0, touched = 0;
  for (int ch = 0; ch < AC_NCH; ch++) {
    AC_ExChannel *c = chp(s, ch);
    u32 f = SYNC_OF(c);
    u32 base = AC_REG_CH(ch);
    if (!ac_dom_ch(s, ch)) continue; /* the other bus's channel (bus split) */
    if (!f) {
      ac_drv_wait(s, 14);
      continue;
    }
    touched |= 1u << ch;
    c->flags &= (u8)~(0x1F << AC_EXF_SYNC_SHIFT);
    if (f & AC_SYNC_STOP) {
      s->p11.started &= ~(1u << ch);
      hw_stop(s, ch);
      ac_drv_wait(s, 46);
    } else if (f & AC_SYNC_START) {
      s->p11.started |= 1u << ch;
      hw_stop(s, ch);
      ac_drv_wait(s, 117);
      ac_hw_write(s, base + AC_CH_CNT, start_cnt(c), 4);
      ac_drv_wait(s, 7);
      ac_hw_write(s, base + AC_CH_TMR, (u32)(0x10000u - c->timer) & 0xFFFFu, 2);
      if (c->type == AC_EX_PCM) {
        ac_drv_wait(s, 5);
        ac_hw_write(s, base + AC_CH_PNT, c->wave.loopstart, 2);
        ac_drv_wait(s, 2);
        ac_hw_write(s, base + AC_CH_LEN, c->wave.looplen, 4);
        ac_drv_wait(s, 2);
        ac_hw_write(s, base + AC_CH_SAD, c->data, 4);
      }
      ac_drv_wait(s, 43);
      started |= 1u << ch;
    } else {
      int first = 1;
      if (f & AC_SYNC_TIMER) {
        ac_drv_wait(s, 7);
        ac_hw_write(s, base + AC_CH_TMR, (u32)(0x10000u - c->timer) & 0xFFFFu, 2);
        first = 0;
        ac_drv_wait(s, (f & (AC_SYNC_VOLUME | AC_SYNC_PAN)) ? 44 : 49);
      }
      if (f & AC_SYNC_VOLUME) {
        if (first) ac_drv_wait(s, 38);
        ac_hw_write(s, base + AC_CH_CNT, c->volume, 2);
        ac_drv_wait(s, (f & AC_SYNC_PAN) ? 40 : 47);
        first = 0;
      }
      if (f & AC_SYNC_PAN) {
        if (first) ac_drv_wait(s, 40);
        ac_hw_write(s, base + AC_CH_CNT + 2, c->pan, 1);
        ac_drv_wait(s, 43);
      }
    }
  }
  s->drv.cur -= 5;
  for (int ch = 0; ch < AC_NCH; ch++) {
    if (!ac_dom_ch(s, ch)) continue;
    if (started >> ch & 1) {
      ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT + 3, (start_cnt(chp(s, ch)) >> 24) | 0x80u, 1);
      ac_drv_wait(s, 28);
    } else {
      ac_drv_wait(s, touched >> ch & 1 ? 25 : 14);
    }
  }
}
