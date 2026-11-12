/* port/acore piece 5: Capture (docs/acore.md). OWNED BY PIECE 5.
 *
 * Two capture units. Unit n = 0 / 1 records the left / right mixer, or channel 2n (SNDCAPxCNT bit 1), at the rate
 * of the timer of channel 2n+1 (SOUND1TMR / SOUND3TMR), into main memory at DAD for LEN words.
 *
 * Timing (oracle, black box; GBATEK says nothing of it): the unit has its own down-counter, loaded with
 * 0x10000 - TMR (in 16.76 MHz ticks) at the start (SNDCAPxCNT bit 7 0 -> 1) and reloaded from TMR at each overflow;
 * every output sample step (1024 cycles) takes 512 ticks off it and each overflow in the step records one sample
 * of the mixer value of that step (a faster timer: several per step, equal; a slower one: some steps none). The
 * first step at or after the write takes part (a write at the cycle of a step counts). Nothing runs while SOUNDCNT
 * bit 15 (master enable) is clear (the counter and the channels freeze); the master volume does not matter (the
 * capture is taken before it). Writing the start bit again while it is set does nothing; the end of a one-shot
 * clears it.
 *
 * Memory (oracle): samples are packed little-endian into words; words are written to DAD + 4 * pos in blocks, when
 * 4 words are complete or when the last word of the buffer is (so a stop loses the block in progress; DAD and LEN
 * are read when a block is written, so a write to them during a capture acts from the next block). DAD bits 0-1
 * are ignored, LEN 0 = 1. A block write stops at the end of the buffer: a loop goes on at DAD, a one-shot clears
 * the busy bit.
 *
 * Values: the mixer (20.8, from piece 4) clipped to -0x8000..0x7FFF (16.8), PCM16 = the integer part (floor),
 * PCM8 = the integer part >> 8 (floor). GBATEK 61 adds a rounding for PCM8 (the MSB of the discarded fraction set:
 * negative values rounded up); the oracle does NOT do it (pure floor, all 65536 values checked), so by the rule
 * "the oracle wins" it is off (AC_CAP_PCM8_ROUND 1 turns it on). Channel source (bit 1) and add mode (bit 0) are
 * documented (GBATEK 59) but the oracle ignores both bits (it always records the mixer, add or not, and its mixer
 * output does not change with add): implemented from GBATEK only, no oracle stream checks them. ch(a) is taken
 * after its volume and divider (16.11, floored: the plain volume scaling of the oracle matches it on the mixer
 * path).
 */
#include "ac_shared.h"
#include <string.h>

#ifndef AC_CAP_PCM8_ROUND
#define AC_CAP_PCM8_ROUND 0
#endif

#define CAP_TICKS_PER_SAMPLE 512 /* the 16.76 MHz channel clock in one output sample (1024 cycles) */

static u32 cap_tmr(AcoreState *s, int n) { return ac_hw_read(s, AC_REG_CH(2 * n + 1) + 8, 2) & 0xFFFF; }
static u32 cap_cnt(const AcoreState *s, int n) { return ac_hw_read(s, AC_REG_SNDCAP0CNT + (u32)n, 1); }
static u32 cap_dad(const AcoreState *s, int n) {
  return ac_hw_read(s, n ? AC_REG_SNDCAP1DAD : AC_REG_SNDCAP0DAD, 4) & 0x07FFFFFCu;
}
static u32 cap_len(const AcoreState *s, int n) {
  u32 l = ac_hw_read(s, n ? AC_REG_SNDCAP1LEN : AC_REG_SNDCAP0LEN, 2) & 0xFFFF;
  return l ? l : 1;
}

void ac_cap_start(AcoreState *s, int n) {
  AC_P05_Unit *u = &s->p05.u[n & 1];
  memset(u, 0, sizeof *u);
  u->running = 1;
  u->ticks = (int)(0x10000u - cap_tmr(s, n & 1));
}

void ac_cap_stop(AcoreState *s, int n) {
  s->p05.u[n & 1].running = 0; /* the block in progress is lost */
}

/* GBATEK 59/60 would have the add mode change the mixer too; the oracle does not, and the add needs the volumes
 * and pans of the channels (piece 4), so there is nothing to do here. */
void ac_cap_premix(AcoreState *s, s32 chout[AC_NCH]) {
  (void)s;
  (void)chout;
}

/* the output of a channel after its volume and divider, 16.11 */
static s64 cap_chan(AcoreState *s, int ch, s32 out) {
  u32 c = ac_ch_reg32(s, ch, 0);
  s64 vol = c & 0x7F;
  int div = (int)((c >> 8) & 3);
  if (vol == 127) vol = 128;
  if (div == 3) div = 4;
  return (s64)out * vol * (1 << (4 - div));
}

/* the value the unit records now (16 bits, or 8 for PCM8) */
static s32 cap_value(AcoreState *s, int n, const s32 mix[2], const s32 chout[AC_NCH]) {
  u32 cnt = cap_cnt(s, n);
  if (!(cnt & 2)) { /* the mixer: 20.8 clipped to 16.8 */
    s32 x = mix[n];
    if (x > 0x7FFFFF) x = 0x7FFFFF;
    if (x < -0x800000) x = -0x800000;
    if (cnt & 8) { /* PCM8: the 16.8 / 0x10000 */
      s32 r = x >> 16;
#if AC_CAP_PCM8_ROUND
      if (x < 0 && (x & 0x8000)) r++;
#endif
      return r;
    }
    return x >> 8;
  }
  /* channel source (GBATEK 59): ch(a) is not clipped; the bugs */
  s64 fa = cap_chan(s, 2 * n, chout[2 * n]), fb = cap_chan(s, 2 * n + 1, chout[2 * n + 1]);
  s32 v;
  if (cnt & 1) v = (s16)(u16)((fa + fb) >> 11); /* addition: the overflow bug, ANDed with FFFFh */
  else if ((fa >> 11) < 0 && (fb >> 11) < 0) v = -0x8000; /* both negative bug */
  else v = (s32)(fa >> 11);
  if (cnt & 8) v >>= 8;
  return v;
}

/* write the pending words at DAD + 4 * pos; the end of the buffer loops or finishes */
static void cap_flush(AcoreState *s, int n) {
  AC_P05_Unit *u = &s->p05.u[n];
  u32 dad = cap_dad(s, n), len = cap_len(s, n);
  for (int i = 0; i < u->npend && u->running; i++) {
    ac_wr32(s, dad + 4 * u->pos, u->pend[i]);
    if (++u->pos >= len) {
      u->pos = 0;
      if (cap_cnt(s, n) & 4) { /* one-shot: done */
        u->running = 0;
        s->io[AC_REG_SNDCAP0CNT + n] &= (u8)~0x80;
      }
    }
  }
  u->npend = 0;
}

static void cap_push(AcoreState *s, int n, s32 v) {
  AC_P05_Unit *u = &s->p05.u[n];
  if (cap_cnt(s, n) & 8) {
    u->word |= (u32)(v & 0xFF) << (8 * u->nbytes);
    u->nbytes++;
  } else {
    u->word |= (u32)(v & 0xFFFF) << (8 * u->nbytes);
    u->nbytes += 2;
  }
  if (u->nbytes < 4) return;
  u->pend[u->npend++] = u->word;
  u->word = 0;
  u->nbytes = 0;
  /* a block is 4 words, or what is left of the buffer */
  if (u->npend == 4 || u->pos + (u32)u->npend >= cap_len(s, n)) cap_flush(s, n);
}

void ac_cap_sample(AcoreState *s, const s32 mix[2], const s32 chout[AC_NCH]) {
  if (!(ac_hw_read(s, AC_REG_SOUNDCNT, 2) & 0x8000)) return;
  for (int n = 0; n < 2; n++) {
    AC_P05_Unit *u = &s->p05.u[n];
    if (!u->running) continue;
    int t = CAP_TICKS_PER_SAMPLE;
    s32 v = 0;
    int have = 0;
    while (u->running && t >= u->ticks) {
      t -= u->ticks;
      u->ticks = (int)(0x10000u - cap_tmr(s, n));
      if (!have) v = cap_value(s, n, mix, chout), have = 1;
      cap_push(s, n, v);
    }
    u->ticks -= t;
  }
}
