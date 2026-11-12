/* port/acore piece 9: Players (docs/acore.md). OWNED BY PIECE 9.
 *
 * The sequencer's player level: SeqMain (the tempo counter and the ticks), start / prepare / stop / pause / skip, the
 * pool of 32 tracks, the raw PLAYER_PARAM / TRACK_PARAM writes, track mute, the allocatable-channel mask.
 *
 * Everything here was fitted to the oracle (tools in C:\stress\au09 of: a scene, the driver info and the
 * shared work read back, the register trace) where GBATEK / kiwi say nothing; the findings:
 *
 *  - A START_SEQ / PREPARE_SEQ resets the player: flags 0, prio 64, volume 127, extFader 0, tempo 120, ratio 256,
 *    tempo counter 240, tracks[] 0xFF, the tick counter 0, and takes track 0 (the lowest free index of the pool of 32;
 *    "free" = the track's active flag clear). No free track: the player stays inactive. PLAYER_PARAM sent before the
 *    start is lost (the reset), the game sends it between PREPARE and START_PREPARED.
 *  - Flags: PREPARE sets ACTIVE only (bit 0); START_PREPARED sets PREPARED (bit 1) whether or not the player is active
 *    (a START that finds no free track ends with flags 0x02); SeqMain skips a player that is not both. STOP clears
 *    ACTIVE only (a started player ends with 0x02, a prepared one with 0x00).
 *  - SeqMain, per active + prepared + unpaused player, in player order: on a periodic wake
 *      while (counter >= 240) { counter -= 240; one tick of every active track in slot order; ticks++ }   then
 *      counter += tempo * tempo_ratio >> 8
 *    (the check comes before the add: a START's first wake ticks at once; a ratio of 1000 at tempo 120 gives 468 per
 *    wake: two ticks a wake); a non-periodic wake does not tick or add. A track opened (0x93) by a track in the same
 *    tick is ticked in that tick when its slot comes later. After the ticks (periodic or not) each of the player's
 *    tracks pushes its parameters into its channels (a TRACK_PARAM sent with a non-periodic wake reaches the
 *    channel's volume one wake sooner than with a periodic one). The player ends when no track is active.
 *  - STOP_SEQ and the end of a sequence release the tracks' channels at their own release rate (the oracle: a
 *    release-40 note keeps decaying); PAUSE (flag set) and MUTE_TRACK stop (3) kill them at once (volume to the floor
 *    and the channel off a wake later); MUTE_TRACK release (2) is the own-rate release; no-stop (1) only sets the
 *    track's mute flag; off (0) clears it. The release argument of ac_track_release_channels (piece 11): -1 = the
 *    note's own release rate, 127 = forced fast.
 *  - TRACK_PARAM's mask is over the player's track slots (bit n = tracks[n]); ALLOCATABLE_CHANNEL sets the track's
 *    channel_mask and its CHANNEL_MASK flag (0xC3 / 0x00F0 observed).
 *  - SKIP_SEQ: the oracle's driver goes quiet after it (ticks stop, no further register writes, the driver info is
 *    never written again): a FreeBIOS artifact? The game never sends it. Here: N ticks without note-ons (the tracks
 *    muted for the duration), the tick counter advanced by N.
 */
#include "ac_shared.h"
#include <string.h>

#define PL(s, p) (&(s)->work.player[p])
#define TR(s, t) (&(s)->work.track[t])
#define NO_REL (-1)
#define FAST_REL 127

static int pl_ok(int p) { return p >= 0 && p < AC_NPLAYER; }

/* the track of player p's slot n, or -1 */
static int slot_track(const AcoreState *s, int p, int n) {
  if (n < 0 || n >= AC_TRACKS_PER_PLAYER) return -1;
  int t = s->work.player[p].tracks[n];
  return t < AC_NTRACK ? t : -1;
}

static void set_ticks(AcoreState *s, int p, u32 v) {
  s->p09.ticks[p] = v;
  if (s->drv.shared) ac_wr32(s, s->drv.shared + AC_SW_PLAYER_TICKS(p), v);
}

int ac_track_alloc(AcoreState *s) {
  for (int t = 0; t < AC_NTRACK; t++)
    if (!(s->work.track[t].flags & AC_TRF_ACTIVE)) {
      /* a track that ended stays in its player's slot (its released notes still follow the track's volume) until the
         pool hands it out again: then the old player lets go of it (oracle, the battle clip: a sound effect's new
         sequence on a reused track slot was ticked by the old player, with the old bank) */
      for (int p = 0; p < AC_NPLAYER; p++)
        for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++)
          if (s->work.player[p].tracks[n] == t) s->work.player[p].tracks[n] = AC_NO_TRACK;
      return t;
    }
  return -1;
}

void ac_player_stop(AcoreState *s, int p) {
  if (!pl_ok(p)) return;
  AC_Player *pl = PL(s, p);
  if (!(pl->flags & AC_PLF_ACTIVE)) return;
  for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++) {
    int t = slot_track(s, p, n);
    if (t < 0) continue;
    ac_track_release_channels(s, p, t, NO_REL);
    ac_track_free_channels(s, t);
    TR(s, t)->flags &= (u8)~AC_TRF_ACTIVE;
    pl->tracks[n] = AC_NO_TRACK;
  }
  pl->flags &= (u8)~AC_PLF_ACTIVE;
}

void ac_player_prepare(AcoreState *s, int p, u32 base, u32 offset, u32 bank) {
  if (!pl_ok(p)) return;
  ac_player_stop(s, p);
  ac_bus_player_started(s, p);
  AC_Player *pl = PL(s, p);
  pl->flags = 0;
  pl->myNo = (u8)p;
  pl->prio = 64;
  pl->volume = 127;
  pl->extFader = 0;
  memset(pl->tracks, AC_NO_TRACK, sizeof pl->tracks);
  pl->tempo = 120;
  pl->tempo_ratio = 256;
  pl->tempo_counter = 240;
  pl->bank = bank;
  set_ticks(s, p, 0);
  for (int n = 0; n < 16; n++) s->p09.chmask[p][n] = 0xFFFF;
  ac_var_reset_player(s, p);
  int t = ac_track_alloc(s);
  if (t < 0) return;
  ac_track_init(s, t);
  AC_Track *tr = TR(s, t);
  tr->flags |= AC_TRF_ACTIVE;
  tr->base = base;
  tr->cur = base + offset;
  pl->tracks[0] = (u8)t;
  pl->flags |= AC_PLF_ACTIVE;
}

void ac_player_start_prepared(AcoreState *s, int p) {
  if (!pl_ok(p)) return;
  PL(s, p)->flags |= AC_PLF_PREPARED;
}

void ac_player_start(AcoreState *s, int p, u32 base, u32 offset, u32 bank) {
  ac_player_prepare(s, p, base, offset, bank);
  ac_player_start_prepared(s, p);
}

int ac_player_open_track(AcoreState *s, int p, int no, u32 addr) {
  if (!pl_ok(p) || no < 0 || no >= AC_TRACKS_PER_PLAYER) return 0;
  AC_Player *pl = PL(s, p);
  if (pl->tracks[no] != AC_NO_TRACK) return 0;
  int t = ac_track_alloc(s);
  if (t < 0) return 0;
  int first = slot_track(s, p, 0);
  u32 base = first >= 0 ? TR(s, first)->base : addr;
  ac_track_init(s, t);
  AC_Track *tr = TR(s, t);
  tr->flags |= AC_TRF_ACTIVE;
  tr->base = base;
  tr->cur = addr;
  if (s->p09.chmask[p][no] != 0xFFFF) { /* oracle, the intro clip: the mask sent at the start reaches the tracks opened later */
    tr->channel_mask = s->p09.chmask[p][no];
    tr->flags |= AC_TRF_CHANNEL_MASK;
  }
  pl->tracks[no] = (u8)t;
  return 1;
}

/* one tick of every active track of player p; 1 when the player went on, 0 when it ended */
static int player_tick(AcoreState *s, int p) {
  AC_Player *pl = PL(s, p);
  for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++) {
    int t = slot_track(s, p, n);
    if (t < 0 || !(TR(s, t)->flags & AC_TRF_ACTIVE)) continue;
    if (ac_track_tick(s, p, t)) TR(s, t)->flags &= (u8)~AC_TRF_ACTIVE;
  }
  set_ticks(s, p, s->p09.ticks[p] + 1);
  for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++) {
    int t = slot_track(s, p, n);
    if (t >= 0 && (TR(s, t)->flags & AC_TRF_ACTIVE)) return 1;
  }
  ac_player_stop(s, p);
  (void)pl;
  return 0;
}

void ac_seq_main(AcoreState *s, int periodic) {
  for (int p = 0; p < AC_NPLAYER; p++) {
    AC_Player *pl = PL(s, p);
    if ((pl->flags & (AC_PLF_ACTIVE | AC_PLF_PREPARED | AC_PLF_PAUSE)) != (AC_PLF_ACTIVE | AC_PLF_PREPARED)) continue;
    if (!ac_dom_pl(s, p)) continue; /* the other bus's player (bus split) */
    if (periodic) {
      u32 cnt = pl->tempo_counter;
      int alive = 1;
      while (cnt >= 240) {
        cnt -= 240;
        pl->tempo_counter = (u16)cnt; /* a track's tempo command may read it */
        if (!player_tick(s, p)) {
          alive = 0;
          break;
        }
        cnt = pl->tempo_counter;
      }
      if (!alive) continue;
      pl->tempo_counter = (u16)(cnt + (((u32)pl->tempo * pl->tempo_ratio) >> 8));
    }
    for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++) {
      int t = slot_track(s, p, n);
      if (t >= 0) ac_track_update_channels(s, p, t, 0);
    }
  }
}

void ac_player_pause(AcoreState *s, int p, int flag) {
  if (!pl_ok(p)) return;
  AC_Player *pl = PL(s, p);
  if (!flag) {
    pl->flags &= (u8)~AC_PLF_PAUSE;
    return;
  }
  pl->flags |= AC_PLF_PAUSE;
  for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++) {
    int t = slot_track(s, p, n);
    if (t >= 0) ac_track_release_channels(s, p, t, FAST_REL);
  }
}

void ac_player_skip(AcoreState *s, int p, u32 ticks) {
  if (!pl_ok(p)) return;
  AC_Player *pl = PL(s, p);
  if (!(pl->flags & AC_PLF_ACTIVE)) return;
  u8 saved[AC_TRACKS_PER_PLAYER];
  for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++) {
    int t = slot_track(s, p, n);
    saved[n] = t >= 0 ? (u8)(TR(s, t)->flags & AC_TRF_MUTE) : 0;
    if (t >= 0) TR(s, t)->flags |= AC_TRF_MUTE;
  }
  int alive = 1;
  for (u32 i = 0; i < ticks && alive; i++) alive = player_tick(s, p);
  if (!alive) return;
  for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++) {
    int t = slot_track(s, p, n);
    if (t >= 0 && !saved[n]) TR(s, t)->flags &= (u8)~AC_TRF_MUTE;
  }
}

void ac_player_param(AcoreState *s, int p, u32 ofs, u32 data, int size) {
  if (!pl_ok(p) || (size != 1 && size != 2 && size != 4) || ofs + (u32)size > sizeof(AC_Player)) return;
  u8 b[4] = {(u8)data, (u8)(data >> 8), (u8)(data >> 16), (u8)(data >> 24)};
  memcpy((u8 *)PL(s, p) + ofs, b, (size_t)size);
}

void ac_track_param_raw(AcoreState *s, int p, u32 mask, u32 ofs, u32 data, int size) {
  if (!pl_ok(p) || (size != 1 && size != 2 && size != 4) || ofs + (u32)size > sizeof(AC_Track)) return;
  u8 b[4] = {(u8)data, (u8)(data >> 8), (u8)(data >> 16), (u8)(data >> 24)};
  for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++) {
    if (!(mask >> n & 1)) continue;
    int t = slot_track(s, p, n);
    if (t >= 0) memcpy((u8 *)TR(s, t) + ofs, b, (size_t)size);
  }
}

void ac_track_mute(AcoreState *s, int p, u32 mask, int mute) {
  if (!pl_ok(p)) return;
  for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++) {
    if (!(mask >> n & 1)) continue;
    int t = slot_track(s, p, n);
    if (t < 0) continue;
    AC_Track *tr = TR(s, t);
    if (mute == AC_MUTE_OFF) {
      tr->flags &= (u8)~AC_TRF_MUTE;
      continue;
    }
    tr->flags |= AC_TRF_MUTE;
    if (mute == AC_MUTE_RELEASE) ac_track_release_channels(s, p, t, NO_REL);
    else if (mute == AC_MUTE_STOP) ac_track_release_channels(s, p, t, FAST_REL);
  }
}

void ac_track_allocatable(AcoreState *s, int p, u32 mask, u32 chmask) {
  if (!pl_ok(p)) return;
  for (int n = 0; n < AC_TRACKS_PER_PLAYER; n++) {
    if (!(mask >> n & 1)) continue;
    s->p09.chmask[p][n] = (u16)chmask;
    int t = slot_track(s, p, n);
    if (t < 0) continue;
    TR(s, t)->channel_mask = (u16)chmask;
    TR(s, t)->flags |= AC_TRF_CHANNEL_MASK;
  }
}
