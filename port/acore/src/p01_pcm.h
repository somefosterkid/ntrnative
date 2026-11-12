/* port/acore piece 1: PCM channels (docs/acore.md). OWNED BY PIECE 1.
 * p01_pcm.c/.h, tests/t01_pcm.c. `ac_ch_start(s, ch)`, `ac_ch_stop(s, ch)`, `ac_ch_step(s, ch)`, `ac_ch_output(s,
 * ch)` (the channel's current sample, s16 scale, before volume; dispatches ADPCM to 2 and PSG/noise to 3).
 * SOUNDxCNT/SAD/TMR/PNT/LEN as latched at the start, the timer (TMR counting up at 16.76 MHz, overflow -> next
 * sample, reload), PCM8 (NN -> NN00) and PCM16 fetched from `s->mem` by SAD, the start delays (PCM 3 samples, GBATEK
 * 61), the repeat modes: manual, loop (PNT then LEN repeated), one-shot (busy cleared at the last sample, the last
 * sample held with Hold), the busy bit in SOUNDxCNT. GBATEK 57, 61. Tests: a PCM16 ramp at TMR 0xFE00 (one sample
 * per 512 ticks: exactly one per output sample), TMR 0xFF00 (two per output sample), PCM8 scaling, a loop's wrap
 * (PNT + LEN words), one-shot end and busy, Hold, a restart while running, the minimum length rule (PNT+LEN < 4
 * words).
 */
#ifndef AC_P01_H
#define AC_P01_H
#include "ac_types.h"

/* one channel's running state (latched at the start) */
typedef struct AC_P01Ch {
  u8 run;        /* started and not ended */
  u8 dead;       /* PNT+LEN < 4 words: busy, silent */
  u8 fmt, rep;
  s32 due;       /* steps (512 counts each) not yet run: ac_ch_output runs them, see p01_pcm.c */
  s32 delay;     /* output samples until the timer starts */
  u32 sad;       /* latched source address */
  u32 timer;     /* counts elapsed since the last overflow; overflows at 0x10000 - TMR (read live) */
  u32 pos;       /* next sample to fetch (in samples; ADPCM: counts the header word's 8) */
  u32 loopstart; /* PNT in samples */
  u32 total;     /* (PNT + LEN) in samples */
  s32 cur;       /* the current output sample (s16 scale) */
  float hist[32]; /* the last 16 fetched samples, oldest first from hist[hpos] (a ring stored twice: hist[i] = hist[i + 16]; cur
                     is the newest); kept in both qualities, read by Enhanced */
  u8 hpos;
  u8 tail;        /* Enhanced: periods the interpolation still plays after a one-shot's end (zeros go in) */
  u32 reload;    /* Enhanced: the TMR value the running period started from */
} AC_P01Ch;

typedef struct AC_P01 {
  AC_P01Ch ch[AC_NCH];
} AC_P01;

#endif
