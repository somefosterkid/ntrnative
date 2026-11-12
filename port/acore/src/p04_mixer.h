/* port/acore piece 4: The mixer (docs/acore.md). OWNED BY PIECE 4.
 * p04_mixer.c/.h, tests/t04_mixer.c. `ac_mix(s, const s32 chout[16], s16 out[2])`: per channel the volume divider
 * (shift 0/1/2/4), the volume (N/128, register 127 = 128), the pan (left (128-N)/128, right N/128), the GBATEK 61
 * bit widths and the rounding at each step, the sum, SOUNDCNT bits 8-13 (output from mixer / ch1 / ch3 / ch1+ch3,
 * ch1 and ch3 to the mixer or not), the master volume (N/128/64), the master enable (bit 15), the bias (SOUNDBIAS)
 * and the clip to 0-3FFh; out = (v << 6) - 0x8000. GBATEK 58, 60, 61. Tests: one channel full left/right, centre
 * pan, each divider, master volume 0/64/127, the clip with 16 channels at full, the output selectors, bias 0 and
 * 3FFh.
 */
#ifndef AC_P04_H
#define AC_P04_H

/* the piece's own state, s->p04 (per channel / player / track arrays as it needs) */
typedef struct AC_P04 {
  int unused;
} AC_P04;

#endif
