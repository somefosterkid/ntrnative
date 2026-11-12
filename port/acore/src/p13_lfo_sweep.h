/* port/acore piece 13: LFO, sweep and portamento (docs/acore.md). OWNED BY PIECE 13.
 * p13_lfo_sweep.c/.h, tests/t13_lfo_sweep.c. The rules are written at the top of the .c.
 *
 * How the callers use it (ordering fitted on the oracle):
 *  - p14 (per channel, each wake, in this order): value = ac_lfo_value(...) / ac_sweep_value(...), THEN
 *    ac_lfo_update(s, &ch->lfo, periodic) and ac_sweep_update(s, ch, periodic). The first computation after a note
 *    starts sees counter 0 (no modulation); the next wake's sees one step.
 *  - p9 / p7 (the sequencer's tick, for every channel of a track that ticks): ac_sweep_update(s, ch, 0). Sweeps
 *    without portamento time (non-auto) count sequencer ticks; auto sweeps count periodic steps (p14, periodic = 1).
 *  - p10 (note-on, after the channel is allocated and its length known): ac_porta_setup(s, ch, track, key, length).
 */
#ifndef AC_P13_H
#define AC_P13_H

/* the piece's own state, s->p13: none needed (the LFO and sweep state live in the AC_ExChannel) */
typedef struct AC_P13 {
  int unused;
} AC_P13;

struct AcoreState;
struct AC_Track;
/* note-on: the channel's sweep from the track (portamento from track->porta_key when AC_TRF_PORTA is set, plus
 * track->sweep_pitch; length = the note's ticks, or porta_time^2 * |pitch| >> 11 and AUTO_SWEEP when porta_time > 0);
 * sets track->porta_key = key. */
void ac_porta_setup(struct AcoreState *s, int ch, struct AC_Track *t, int key, s32 note_length);

#endif
