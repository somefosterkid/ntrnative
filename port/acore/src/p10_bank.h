/* port/acore piece 10: Banks and note-on (docs/acore.md). OWNED BY PIECE 10.
 * p10_bank.c/.h, tests/t10_bank.c. `ac_note_on(s, p, t, key, velocity, length)`: the instrument from the player's
 * bank (SBNK: the record for the program, types 1 PCM, 2 PSG, 3 noise, 4 direct PCM, 5 null, 16 drum set, 17 key
 * split), the wave from the bank's wave archive links (SWAR: the wave's SWAV header: format, loop, rate, timer, loop
 * start, length), the channel set up through 11 (`ac_exch_alloc`) and its start (`ac_exch_start`: key, base key,
 * velocity, length, ADSR from the instrument unless the track's are set, the instrument's pan). GBATEK 66, 67, 68,
 * kiwi. Tests: each instrument type on a hand-made bank, a drum set's range, a key split's regions, a missing wave
 * archive (no note), the PSG duty from the record.
 */
#ifndef AC_P10_H
#define AC_P10_H

/* the piece's own state, s->p10 (per channel / player / track arrays as it needs) */
typedef struct AC_P10 {
  int unused;
} AC_P10;

#endif
