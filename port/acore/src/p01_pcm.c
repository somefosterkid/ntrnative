/* port/acore piece 1: PCM channels (docs/acore.md). OWNED BY PIECE 1.
 *
 * A channel is started by SOUNDxCNT's start bit (0 -> 1): SAD, PNT and LEN are latched then (the oracle reads them
 * live, which no game relies on). The timer counts up from TMR, 512 counts per output sample; every overflow reloads
 * the live TMR (the excess counts are kept) and takes the next sample. The start delay (GBATEK 61: PCM 3 samples,
 * PSG 1, ADPCM 11) is in timer overflows: the first 2 (ADPCM 10, PSG 0) fetch nothing, the next is sample 0.
 *
 * Order: the oracle runs a channel's 512 counts BEFORE the mixer reads it in the same step (a register write just
 * before step k lands before that step's counts), while ac_hw_sample reads and then steps. So ac_ch_step only books
 * a step as due, and ac_ch_output runs the due steps before it returns the sample; ac_ch_start books one at once.
 * That is the oracle's order exactly (streams a-q of tests/t01_pcm.c, TMR changes while running included).
 *
 * The sample position counts in samples: a word holds 4 PCM8, 2 PCM16 or 8 ADPCM samples (ADPCM's first word is the
 * header: positions below 8 are never fetched). One-shot plays PNT+LEN words once; loop plays PNT+LEN words once
 * and then repeats from PNT. PSG/noise are infinite (piece 3), ADPCM decodes in piece 2. A PCM/ADPCM length of
 * PNT+LEN < 4 words keeps the busy bit and stays silent (GBATEK 57).
 */
#include "ac_shared.h"
#include <math.h>
#include <string.h>

/* Enhanced: the interpolation kernel. A DS channel holds each sample until the next, which puts the sample's images
 * (the tone mirrored around the channel's rate) into the output. The enhanced output is the band-limited curve through
 * the channel's samples: a 16-tap Kaiser-windowed sinc (beta 8, cutoff 0.92 of the channel's Nyquist), tabulated at
 * 128 fractions of a sample with linear interpolation between rows. Images of a tone near the channel's Nyquist come
 * out some 80 dB down (a 4-point cubic: 10-30 dB). */
#define HQ_TAPS 16
#define HQ_PH 128
static float hq_tab[HQ_PH + 1][HQ_TAPS];
static int hq_ready;

static double bessel0(double x) {
  double sum = 1.0, term = 1.0;
  for (int k = 1; k < 40; k++) {
    term *= (x / (2.0 * k)) * (x / (2.0 * k));
    sum += term;
  }
  return sum;
}

__attribute__((noinline)) static void hq_init(void) { /* once: kept out of the per-sample path */
  const double pi = 3.14159265358979323846, cutoff = 0.92, beta = 8.0, half = HQ_TAPS / 2;
  for (int p = 0; p <= HQ_PH; p++) {
    double t = (double)p / HQ_PH, w[HQ_TAPS], sum = 0.0;
    for (int j = 0; j < HQ_TAPS; j++) {
      double x = (j - (half - 1)) - t; /* tap j sits at j, the point at half - 1 + t */
      double a = x * cutoff * pi;
      double r = x / half;
      w[j] = (fabs(a) < 1e-9 ? 1.0 : sin(a) / a) * (fabs(r) >= 1.0 ? 0.0 : bessel0(beta * sqrt(1.0 - r * r)) / bessel0(beta));
      sum += w[j];
    }
    for (int j = 0; j < HQ_TAPS; j++) hq_tab[p][j] = (float)(w[j] / sum);
  }
  hq_ready = 1;
}

#define SPW(fmt) ((fmt) == AC_FMT_PCM8 ? 4u : (fmt) == AC_FMT_PCM16 ? 2u : 8u)

static void fetch(AcoreState *s, int ch);

/* the history is a ring stored twice (hist[i] = hist[i + 16]), so the last 16, oldest first, are hist + hpos: a push is two
   stores, not a 15-float move */
static inline void hist_push(AC_P01Ch *c, float v) {
  c->hist[c->hpos] = c->hist[c->hpos + 16] = v;
  c->hpos = (u8)((c->hpos + 1) & 15);
}

static void advance(AcoreState *s, int ch, u32 ticks) {
  AC_P01Ch *c = &s->p01.ch[ch];
  if (c->dead) return;
  c->timer += ticks;
  while (c->run && c->timer >= 0x10000u) {
    /* the reload is read live: the driver changes TMR of a running channel (pitch), and the new value is taken at
       the next overflow, the period in flight is not retargeted (oracle, streams n2/n3) */
    c->reload = ac_ch_reg16(s, ch, AC_CH_TMR);
    c->timer = c->timer - 0x10000u + c->reload;
    if (c->delay > 0) c->delay--;
    else {
      fetch(s, ch);
      /* the history Enhanced's interpolation reads (kept in DS mode too, so a switch to Enhanced has it) */
      hist_push(c, (float)c->cur);
    }
  }
  while (!c->run && c->tail && c->timer >= 0x10000u) { /* Enhanced, after a one-shot's end: the last samples play out */
    c->timer = c->timer - 0x10000u + c->reload;
    hist_push(c, 0.0f);
    c->tail--;
  }
}

void ac_ch_start(AcoreState *s, int ch) {
  AC_P01Ch *c = &s->p01.ch[ch];
  u32 cnt = ac_ch_reg32(s, ch, AC_CH_CNT);
  memset(c, 0, sizeof *c);
  c->run = 1;
  c->fmt = AC_CNT_FORMAT(cnt);
  c->rep = AC_CNT_REPEAT(cnt);
  c->sad = ac_ch_reg32(s, ch, AC_CH_SAD) & 0x07FFFFFCu;
    u32 pnt = ac_ch_reg16(s, ch, AC_CH_PNT);
  u32 len = ac_ch_reg32(s, ch, AC_CH_LEN) & 0x3FFFFFu;
  u32 spw = SPW(c->fmt);
  c->loopstart = pnt * spw;
  c->total = (pnt + len) * spw;
  c->timer = c->reload = ac_ch_reg16(s, ch, AC_CH_TMR);
  if (c->fmt == AC_FMT_PSG) {
    c->delay = 0;
    ac_psg_start(s, ch);
  } else {
    c->delay = c->fmt == AC_FMT_ADPCM ? 10 : 2;
    c->pos = c->fmt == AC_FMT_ADPCM ? 8 : 0;
    if (pnt + len < 4) c->dead = 1;
    if (c->fmt == AC_FMT_ADPCM) ac_adpcm_start(s, ch);
  }
  c->due = 1;
}

void ac_ch_stop(AcoreState *s, int ch) {
  AC_P01Ch *c = &s->p01.ch[ch];
  c->run = 0;
  c->cur = 0;
  c->tail = 0;
}

/* Enhanced: the sinc-interpolated curve through the last 16 samples, between the 8th and 9th, at the timer's position in the
   period: eight periods late (the DS holds a sample until the next one) */
static s32 hq_output(AC_P01Ch *c) {
  if (!hq_ready) hq_init();
  float q = (float)(c->timer > c->reload ? c->timer - c->reload : 0) * (float)HQ_PH / (float)(0x10000u - c->reload);
  int p = (int)q;
  if (p >= HQ_PH) p = HQ_PH - 1;
  float f = q - (float)p, d0 = 0.0f, d1 = 0.0f;
  const float *h = c->hist + c->hpos;
  for (int j = 0; j < HQ_TAPS; j++) d0 += hq_tab[p][j] * h[j], d1 += hq_tab[p + 1][j] * h[j];
  float v = d0 + f * (d1 - d0);
  return (s32)(v > 32767.0f ? 32767.0f : v < -32768.0f ? -32768.0f : v);
}

static inline s32 ch_output(AcoreState *s, int ch) {
  AC_P01Ch *c = &s->p01.ch[ch];
  while (c->due > 0) { /* the steps owed since the last read */
    c->due--;
    if (c->run || c->tail) advance(s, ch, AC_TICKS_PER_SAMPLE);
  }
  if ((!s->hq && !s->hq_fade) || !(c->run || c->tail) || c->fmt == AC_FMT_PSG) return c->cur;
  s32 h = hq_output(c);
  if (!s->hq_fade) return h;
  /* just after a quality switch: from the other quality's output to this one's (the switch stepped by up to 8 periods) */
  s32 now = s->hq ? h : c->cur, was = s->hq ? c->cur : h;
  return now + (s32)((float)(was - now) * (float)s->hq_fade / 65.0f);
}

s32 ac_ch_output(AcoreState *s, int ch) { return ch_output(s, ch); }

static void fetch(AcoreState *s, int ch) {
  AC_P01Ch *c = &s->p01.ch[ch];
  if (c->fmt == AC_FMT_PSG) {
    c->cur = ac_psg_next(s, ch);
    return;
  }
  if (c->pos >= c->total) { /* past the end */
    if (c->rep == AC_REPEAT_ONESHOT) {
      c->run = 0;
      c->cur = 0; /* the oracle ignores Hold here (suspect.txt) */
      c->tail = s->hq ? 16 : 0; /* Enhanced: the interpolation is 8 periods late and 16 wide: the end plays out */
      return;
    }
    c->pos = c->loopstart;
    if (c->fmt == AC_FMT_ADPCM) ac_adpcm_loop_restore(s, ch);
  }
  if (c->fmt == AC_FMT_ADPCM && c->pos == c->loopstart) ac_adpcm_loop_save(s, ch);
  switch (c->fmt) {
  case AC_FMT_PCM8: c->cur = (s32)(s8)ac_rd8(s, c->sad + c->pos) * 256; break;
  case AC_FMT_PCM16: c->cur = (s16)ac_rd16(s, c->sad + c->pos * 2); break;
  default: c->cur = ac_adpcm_next(s, ch, c->pos - 8); break;
  }
  if (c->pos == c->total - 1 && c->rep == AC_REPEAT_ONESHOT) ac_hw_set_busy(s, ch, 0);
  c->pos++;
}

void ac_ch_step(AcoreState *s, int ch) {
  if (s->p01.ch[ch].run || s->p01.ch[ch].tail) s->p01.ch[ch].due++;
}

/* k output samples of one channel, each read and then stepped as ac_hw_sample does (ac_state.c's block: channel by channel) */
void ac_ch_run(AcoreState *s, int ch, s32 *out, int k) {
  AC_P01Ch *c = &s->p01.ch[ch];
  for (int i = 0; i < k; i++) {
    out[i] = ch_output(s, ch);
    if (c->run || c->tail) c->due++;
  }
}
