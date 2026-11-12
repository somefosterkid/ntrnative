/* port/acore piece 3: PSG and noise (docs/acore.md). OWNED BY PIECE 3.
 * p03_psg_noise.c/.h, tests/t03_psg_noise.c. `ac_psg_start(s, ch)`, `ac_psg_next(s, ch)`: duty cycles on channels
 * 8-13 (8 steps, starting LOW, duty 7 = always LOW), the noise LFSR on 14-15 (X = 7FFFh at start, `X >>= 1; carry ?
 * (LOW, X ^= 6000h) : HIGH`), levels +-7FFFh, 1-sample start delay, format 3 on channels 0-7 silent. GBATEK 57, 61.
 * Tests: each duty's 8-step pattern, the first 32 noise outputs worked by hand, PSG ignoring LEN/repeat.
 *
 * Contract with piece 1: ac_psg_start resets the channel (step 0 / X = 7FFFh); each ac_psg_next call returns the
 * next step's level (the first call after a start is step 0). The 1-sample start delay is piece 1's (it calls
 * ac_psg_next once per output sample after the delay). The duty is read live from SOUNDxCNT at each step.
 */
#ifndef AC_P03_H
#define AC_P03_H

/* the piece's own state, s->p03 */
typedef struct AC_P03 {
  unsigned step[16]; /* duty channels: the next step 0-7 */
  unsigned lfsr[16]; /* noise channels: X */
} AC_P03;

#endif
