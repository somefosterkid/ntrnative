/* port/acore piece 7: The sequence reader and flow (docs/acore.md). OWNED BY PIECE 7.
 * p07_seq_flow.c/.h, tests/t07_seq_flow.c. `ac_track_tick(s, p, t)`: one tick of a track: the wait countdown, then
 * commands until a wait: notes 0x00-0x7F (key, velocity, VLQ length: to 10 via `ac_note_on`), 0x80 wait, 0x81
 * program, 0x93 open track, 0x94 jump, 0x95 call, 0xFD return, 0xD4 loop start, 0xFC loop end, 0xA0 random / 0xA1
 * variable / 0xA2 if prefixes, the variable ops 0xB0-0xBD (set, add, sub, mul, div, shift, random, compare -> the
 * track's condition flag), 0xFE (tracks used), 0xFF end; the reads through `ac_rd8` from the track's DS addresses;
 * parameter commands passed to 8 (`ac_track_param`). GBATEK 64, kiwi. Tests: each command on a hand-made sequence in
 * memory (the track's cur, wait, call stack, loop counts, variables after N ticks), VLQ lengths, the if-prefix
 * skipping, nested calls to depth 3.
 */
#ifndef AC_P07_H
#define AC_P07_H

/* the piece's own state, s->p07 (per channel / player / track arrays as it needs) */
typedef struct AC_P07 {
  int unused;
} AC_P07;

#endif
