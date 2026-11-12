/* port/acore: the sound archive's stream (STRM), for the port's NNS_SndArcStrm* (port/src/strm_arc.c; Black 2's and
 * White 2's title music). Not a hardware piece: on the DS the ARM9 streams the file through a ring buffer that two
 * locked channels play. Here the whole file is decoded at once into host memory and played on the host side as those
 * channels would play it: nearest sample at the channel's timer (16756991 / rate, 512 counts a mixer sample), volume
 * and divider from the archive's volume and the fade (ac_decibel, ac_calc_volume), the left channel panned full left
 * and the right full right (a mono stream in the centre), added to the mixer's sum before the master volume.
 *
 * The STRM format (docs/acore.md, "The archive stream"): kiwi.ds's "Nitro Composer File (*.sdat) Specification"
 * (2007, with loveemu's strm2wave notes) for the HEAD and DATA blocks and the per-channel block interleave; GBATEK's
 * "DS Sound" for the IMA-ADPCM block header and the channel timer. */
#include "ac_shared.h"
#include "acore.h"

#include <stdlib.h>
#include <string.h>

#define STRM_TIMER_CLOCK 16756991u /* the channel timer's clock: 33513982 / 2 */

static u32 le32(const u8 *p) { return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24; }
static u16 le16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }

/* one channel's block of n samples into out (stride nch) */
static void decode_block(int type, const u8 *b, u32 len, u32 n, s16 *out, u32 nch) {
  if (type == 0) { /* PCM8 */
    for (u32 i = 0; i < n && i < len; i++) out[i * nch] = (s16)((s8)b[i] * 256);
  } else if (type == 1) { /* PCM16 */
    for (u32 i = 0; i < n && 2 * i + 1 < len; i++) out[i * nch] = (s16)le16(b + 2 * i);
  } else { /* IMA-ADPCM: a header word (the first sample, the index), then 4-bit codes low nibble first */
    if (len < 4) return;
    s32 pred = (s16)le16(b);
    u32 idx = b[2] & 0x7F;
    for (u32 i = 0; i < n && 4 + i / 2 < len; i++) {
      u8 c = b[4 + i / 2];
      out[i * nch] = (s16)ac_adpcm_step(&pred, &idx, (i & 1) ? (u32)(c >> 4) : (u32)(c & 15));
    }
  }
}

int16_t *acore_strm_decode(const void *file, size_t size, AcoreStrmInfo *info) {
  const u8 *f = file;
  if (size < 0x68 || memcmp(f, "STRM", 4) || memcmp(f + 0x10, "HEAD", 4)) return NULL;
  const u8 *h = f + 0x18;
  int type = h[0];
  u32 nch = h[2], rate = le16(h + 4), loop_start = le32(h + 8), nsamp = le32(h + 12), doff = le32(h + 16);
  u32 nblk = le32(h + 20), blen = le32(h + 24), bsamp = le32(h + 28), lblen = le32(h + 32), lbsamp = le32(h + 36);
  if (type > 2 || nch < 1 || nch > 2 || rate == 0 || nsamp == 0 || nblk == 0) return NULL;
  s16 *pcm = calloc((size_t)nsamp * nch, sizeof *pcm);
  if (!pcm) return NULL;
  for (u32 k = 0, at = 0; k < nblk && at < nsamp; k++) {
    int last = k == nblk - 1;
    u32 len = last ? lblen : blen, n = last ? lbsamp : bsamp;
    if (n > nsamp - at) n = nsamp - at;
    for (u32 c = 0; c < nch; c++) {
      size_t o = (size_t)doff + (size_t)k * nch * blen + (size_t)c * len;
      if (o >= size) break;
      decode_block(type, f + o, (u32)(size - o < len ? size - o : len), n, pcm + (size_t)at * nch + c, nch);
    }
    at += n;
  }
  info->nsamp = nsamp;
  info->nch = nch;
  info->rate = rate;
  info->loop = h[1] != 0;
  info->loop_start = loop_start < nsamp ? loop_start : 0;
  return pcm;
}

/* the channels' volume and divider: the archive's volume and the fade's, each through the driver's dB table */
static void strm_volume(AC_Strm *t) {
  int fv = t->fade_len ? (127 * t->fade + t->fade_len / 2) / t->fade_len : 127;
  t->cnt = ac_calc_volume(ac_decibel(t->vol) + ac_decibel(fv));
}

void acore_strm_set(AcoreState *s, const int16_t *pcm, const AcoreStrmInfo *info, int volume, uint32_t offset_ms) {
  AC_Strm *t = &s->strm;
  memset(t, 0, sizeof *t);
  if (!pcm || !info) return;
  t->pcm = pcm;
  t->nsamp = info->nsamp, t->nch = info->nch, t->rate = info->rate;
  t->loop = info->loop, t->loop_start = info->loop_start;
  t->timer = STRM_TIMER_CLOCK / info->rate;
  if (t->timer == 0) t->timer = 1;
  u64 at = (u64)offset_ms * info->rate / 1000;
  t->pos = at < info->nsamp ? (u32)at : 0;
  t->vol = volume;
  strm_volume(t);
}

void acore_strm_start(AcoreState *s) {
  if (s->strm.pcm) s->strm.playing = 1;
}

/* fade_frames game frames (560190 cycles each) to silence, then stopped; 0 stops at once */
void acore_strm_stop(AcoreState *s, int fade_frames) {
  AC_Strm *t = &s->strm;
  if (fade_frames <= 0 || !t->playing) {
    t->playing = 0;
    return;
  }
  t->fade = t->fade_len = fade_frames;
  t->fade_count = 0;
  strm_volume(t);
}

int acore_strm_playing(const AcoreState *s) { return s->strm.playing; }

uint32_t acore_strm_pos_ms(const AcoreState *s) {
  const AC_Strm *t = &s->strm;
  return t->playing ? (u32)((u64)t->pos * 1000 / t->rate) : 0;
}

void ac_strm_sample(AcoreState *s, s32 sum[2]) {
  static const int div_shift[4] = {0, 1, 2, 4};
  AC_Strm *t = &s->strm;
  int vol = (int)AC_CNT_VOL(t->cnt), dsh = div_shift[AC_CNT_DIV(t->cnt)];
  if (vol == 127) vol = 128;
  const s16 *f = t->pcm + (size_t)t->pos * t->nch;
  /* a channel as p04's chan_lr: 16.4 after the divider, times the volume (16.11), times the pan (16.18 -> 16.8) */
  s64 l = (((s64)f[0] * 16) >> dsh) * vol;
  if (t->nch == 2) {
    s64 r = (((s64)f[1] * 16) >> dsh) * vol;
    sum[0] += (s32)((l * 128) >> 10);
    sum[1] += (s32)((r * 128) >> 10);
  } else {
    sum[0] += (s32)((l * 64) >> 10);
    sum[1] += (s32)((l * 64) >> 10);
  }
  t->count += 512;
  while (t->count >= t->timer) {
    t->count -= t->timer;
    if (++t->pos >= t->nsamp) {
      t->pos = t->loop ? t->loop_start : 0;
      if (!t->loop) {
        t->playing = 0;
        return;
      }
    }
  }
  if (t->fade_len) {
    t->fade_count += AC_CYCLES_PER_SAMPLE;
    if (t->fade_count >= AC_CYCLES_PER_FRAME) {
      t->fade_count -= AC_CYCLES_PER_FRAME;
      if (--t->fade <= 0) t->playing = 0;
      else strm_volume(t);
    }
  }
}
