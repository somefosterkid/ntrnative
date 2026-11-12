/* port/acore piece 12: Envelopes (docs/acore.md). OWNED BY PIECE 12.
 * p12_envelope.c/.h, tests/t12_envelope.c. `ac_env_init`, `ac_env_set_attack/decay/sustain/release` (the 0-127
 * values -> the rates the driver uses: the attack table for 109-127, the decay/release rate conversion, the sustain
 * level in dB), `ac_env_update(s, ch)` (per periodic wake: attack multiplies towards 0, decay and release subtract,
 * the release's end at the floor -> the channel stops), `ac_env_level`. kiwi (SBNK ADSR), GBATEK 66. Tests: the
 * conversions at their ends, an attack of 127 (instant), a decay to a sustain level, a release to the end in N
 * wakes.
 */
#ifndef AC_P12_H
#define AC_P12_H

/* the piece's own state, s->p12 (per channel / player / track arrays as it needs) */
typedef struct AC_P12 {
  int unused;
} AC_P12;

#endif
