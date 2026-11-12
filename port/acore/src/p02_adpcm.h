/* port/acore piece 2: IMA-ADPCM (docs/acore.md). OWNED BY PIECE 2.
 * p02_adpcm.c/.h, tests/t02_adpcm.c. `ac_adpcm_start(s, ch)` (the header word: PCM16 value and index),
 * `ac_adpcm_next(s, ch, nib)` (one 4-bit sample: GBATEK's "in practice" diff with its rounding, the clipping quirks,
 * the index table), the loop: at loop start the state is saved, at the loop end restored. The 11-sample start delay
 * (3 + 8 for the header) is piece 1's; AC_ADPCM_START_DELAY names it. GBATEK 61, 68.
 */
#ifndef AC_P02_H
#define AC_P02_H

#define AC_ADPCM_START_DELAY 11 /* output samples between the start bit and the first sound: 3 + 8 header samples */

/* the piece's own state, s->p02 (per channel) */
typedef struct AC_P02 {
  u32 sad[16];   /* the header's address (latched by ac_adpcm_start) */
  s32 pred[16];  /* current PCM16 value */
  u32 idx[16];   /* current table index (0..127 from the header, then 0..88) */
  s32 spred[16]; /* the pair saved at the loop start */
  u32 sidx[16];
} AC_P02;

#endif
