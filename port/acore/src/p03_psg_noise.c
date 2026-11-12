/* port/acore piece 3: PSG and noise (docs/acore.md). OWNED BY PIECE 3. GBATEK "DS Sound Notes", PSG sections. */
#include "ac_shared.h"

#define PSG_HIGH 0x7FFF
#define PSG_LOW (-0x7FFF)

void ac_psg_start(AcoreState *s, int ch) {
  if (ch < 0 || ch >= AC_NCH) return;
  s->p03.step[ch] = 0; /* a duty cycle always starts at the begin of the LOW period */
  s->p03.lfsr[ch] = 0x7FFF;
}

s32 ac_psg_next(AcoreState *s, int ch) {
  if (ch < 8 || ch >= AC_NCH) return 0; /* format 3 on channels 0-7 is silent */
  if (ch >= 14) {
    /* noise: X = X >> 1; carry ? (LOW, X ^= 6000h) : HIGH */
    unsigned x = s->p03.lfsr[ch];
    unsigned carry = x & 1u;
    x >>= 1;
    if (carry) x ^= 0x6000u;
    s->p03.lfsr[ch] = x;
    return carry ? PSG_LOW : PSG_HIGH;
  }
  /* duty d (0-7) is HIGH for the last d+1 of the 8 steps; duty 7 is always LOW (GBATEK table) */
  unsigned duty = AC_CNT_DUTY(ac_ch_reg32(s, ch, AC_CH_CNT));
  unsigned step = s->p03.step[ch];
  s->p03.step[ch] = (step + 1) & 7u;
  return (duty != 7 && step >= 7 - duty) ? PSG_HIGH : PSG_LOW;
}
