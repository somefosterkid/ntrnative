/* port/acore piece 6: Tables and arithmetic (docs/acore.md). OWNED BY PIECE 6.
 * p06_tables.c/.h, tests/t06_tables.c. `ac_decibel(s, x)` (the 0-127 -> dB*10 table the driver uses for volumes,
 * velocity and the sustain level), `ac_volume_table(i)` (BIOS GetVolumeTable, 724 entries), `ac_pitch_table(i)`
 * (BIOS GetPitchTable, 768 entries), `ac_sine(i)` (the LFO's sine, a quarter wave of 33 values),
 * `ac_calc_timer(base_timer, pitch)` (pitch in 1/64 semitones: octave shift and the pitch table),
 * `ac_calc_volume(db, &vol, &shift)` (dB*10 -> the register volume and divider), `ac_random(s)` (the driver's LCG,
 * advanced every wake). Sources: GBATEK 72 + the BIOS SWIs' ranges, kiwi; the tables are not in GBATEK: write them
 * from their formulas and check every entry against the oracle (a driver stream that plays each pitch / volume and
 * the trace's TMR / volume values). Tests: known entries (the table ends, the octave boundaries), calc_timer at +-1
 * octave, calc_volume at 0, -6 dB, the minimum.
 */
#ifndef AC_P06_H
#define AC_P06_H

/* the piece's own state, s->p06 (per channel / player / track arrays as it needs) */
typedef struct AC_P06 {
  u32 seed; /* ac_random state */
} AC_P06;

#endif
