/* port/acore piece 14: Channel update (docs/acore.md). OWNED BY PIECE 14.
 * p14_exchannel.c/.h, tests/t14_exchannel.c. `ac_exch_main(s, periodic)`: per active ExChannel the envelope (12) and
 * LFO / sweep (13) steps, the volume (velocity, envelope, user decay -> dB -> 6's calc_volume), the pitch (key - base
 * key in 1/64 semitones, user pitch, sweep, LFO -> calc_timer from the wave's timer), the pan (init pan, user pan,
 * LFO, pan range), the end of a channel (release at the floor, hardware stop) and the sync flags for 11.
 */
#ifndef AC_P14_H
#define AC_P14_H

/* the piece's own state, s->p14 (none needed: everything is in the DS-layout channel) */
typedef struct AC_P14 {
  int unused;
} AC_P14;

struct AcoreState;
struct AC_ExChannel;
/* the pan register value 0-127 of a channel (init pan, pan range, user pan, + the LFO's pan term) */
int ac_exch_pan_value(const struct AC_ExChannel *c, int lfo_pan);
/* one sequence tick of a note's length (the track's tick calls this for each channel of the track): counts down
 * and releases the channel when it reaches 0; length <= 0 (a tie) is left alone. Not in ac_shared.h (frozen):
 * owners of 7/9 include p14_exchannel.h. */
void ac_exch_length_tick(struct AcoreState *s, int ch);

#endif
