/* port/acore piece 9: Players (docs/acore.md). OWNED BY PIECE 9.
 * p09_player.c/.h, tests/t09_player.c. `ac_seq_main(s, periodic)` (each active player: the tempo counter `+= tempo *
 * tempo_ratio / 256`, a tick per 240, the ticks' track updates, the tick counter), `ac_player_start / prepare /
 * start_prepared / stop / pause / skip`, track allocation from the pool of 32 (`ac_track_alloc`), the player's prio
 * and volume, mute (`ac_track_mute`: off, no-stop, release, stop), the raw PLAYER_PARAM / TRACK_PARAM writes into
 * the DS-layout structs, a player's end when its tracks end. GBATEK 64, kiwi (tempo 0xE1, 0xFE). Tests: ticks per
 * wake at tempo 120/240 and ratio 256/128, start/stop, prepare + start, pause, skip N ticks, track pool exhaustion,
 * mute modes.
 */
#ifndef AC_P09_H
#define AC_P09_H
#include "ac_types.h"

/* the piece's own state, s->p09 */
typedef struct AC_P09 {
  u16 chmask[AC_NPLAYER][16]; /* ALLOCATABLE_CHANNEL per player and track slot: a track opened later (0x93) takes its slot's mask */
  u32 ticks[AC_NPLAYER]; /* each player's tick counter (also written to the shared work's tickCounter when it is set) */
} AC_P09;

struct AcoreState;
/* the first free track of the pool of 32 (active flag clear), or -1; the caller marks it active */
int ac_track_alloc(struct AcoreState *s);

#endif
