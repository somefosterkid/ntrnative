/* port/acore piece 8: Track parameters (docs/acore.md). OWNED BY PIECE 8.
 * p08_seq_param.c/.h, tests/t08_seq_param.c. `ac_track_param(s, p, t, cmd, value, ...)`: pan 0xC0, volume 0xC1,
 * player volume 0xC2, transpose 0xC3, pitch bend 0xC4, bend range 0xC5, priority 0xC6, mono/poly 0xC7, tie 0xC8,
 * portamento key 0xC9 / on-off 0xCE / time 0xCF, modulation depth 0xCA, speed 0xCB, type 0xCC, range 0xCD, delay
 * 0xE0, attack/decay/sustain/release 0xD0-0xD3, expression 0xD5, tempo 0xE1, sweep pitch 0xE3, the print 0xD6
 * (ignored); and `ac_track_update_channels(s, p, t, release)`: the track's volume (volume, expression, player
 * volume, faders, in dB through 6), pan (pan, pan range, the player/ext pan), pitch (bend x range, transpose, ext
 * pitch) and modulation pushed into each of its channels' user fields every tick. GBATEK 64, kiwi. Tests: each
 * command's field, the dB composition of volume 127/64/0 with expression, a bend of +-127 at range 2.
 */
#ifndef AC_P08_H
#define AC_P08_H

/* the piece's own state, s->p08 (per channel / player / track arrays as it needs) */
typedef struct AC_P08 {
  int unused;
} AC_P08;

#endif
