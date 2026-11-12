/* port/acore piece 4: The mixer (docs/acore.md). OWNED BY PIECE 4.
 * GBATEK 58, 60, 61. Every stage keeps its fraction bits and floors (arithmetic shift) only where the table of
 * "Channel/Mixer Bit-Widths" strips them, so the per-channel chain is one exact product shifted once. */
#include "ac_shared.h"

/* One channel's left/right contribution in 16.8 fixed point (step 4 of the bit-width table). */
static void chan_lr(u32 cnt, s32 data, s32 lr[2]) {
  static const int div_shift[4] = {0, 1, 2, 4};
  s64 v = ((s64)data * 16) >> div_shift[AC_CNT_DIV(cnt)]; /* 16.4: exact */
  int vol = (int)AC_CNT_VOL(cnt), pan = (int)AC_CNT_PAN(cnt);
  if (vol == 127) vol = 128;
  if (pan == 127) pan = 128;
  v *= vol; /* 16.11 */
  lr[0] = (s32)((v * (128 - pan)) >> 10); /* 16.18 -> 16.8 */
  lr[1] = (s32)((v * pan) >> 10);
}

/* the output stage (GBATEK 61 steps 6-8) of one side: the source the selector picks, the master volume, the bias, the DAC */
static inline s16 out_side(const AcoreState *s, u32 cnt, u32 bias, int side, s32 sum, s32 lr1, s32 lr3) {
  int master = (int)(cnt & 0x7F);
  if (master == 127) master = 128;
  s32 src = sum;
  switch ((cnt >> (8 + 2 * side)) & 3) {
  case 1: src = lr1; break;
  case 2: src = lr3; break;
  case 3: src = lr1 + lr3; break;
  default: break;
  }
  if (s->hq) { /* Enhanced: the DAC's six truncated bits stay (the sum is 16-bit scale: 10-bit value << 6) */
    s32 w = (s32)(((s64)src * master) >> 15);
    if (!(cnt & 0x8000)) w = 0;
    w += (s32)bias << 6;
    return (s16)((w < 0 ? 0 : w > 0xFFFF ? 0xFFFF : w) - 0x8000);
  }
  s32 v = (s32)(((s64)src * master) >> 21); /* 14.21 -> 14.0 */
  if (!(cnt & 0x8000)) v = 0;
  v += (s32)bias;
  if (v < 0) v = 0;
  if (v > 0x3FF) v = 0x3FF;
  return (s16)((v << 6) - 0x8000);
}

/* mix[] gets the mixer's sums in 20.8 fixed point (GBATEK 61 step 5; channels 1/3 left out when SOUNDCNT bit 12 / 13
 * says so): what the capture units take. out[] = (DAC value << 6) - 0x8000. */
void ac_mix(AcoreState *s, const s32 chout[AC_NCH], s32 mix[2], s16 out[2]) {
  u32 cnt = ac_hw_read(s, AC_REG_SOUNDCNT, 2);
  u32 bias = ac_hw_read(s, AC_REG_SOUNDBIAS, 2) & 0x3FF;
  s32 lr[AC_NCH][2];
  s32 sum[2] = {0, 0};
  for (int ch = 0; ch < AC_NCH; ch++) {
    if (chout[ch] == 0) { /* a silent channel (most of them, most of the time) adds nothing: chan_lr would give 0, 0 */
      lr[ch][0] = lr[ch][1] = 0;
      continue;
    }
    chan_lr(ac_ch_reg32(s, ch, AC_CH_CNT), chout[ch], lr[ch]);
    if (ch == 1 && (cnt & 0x1000)) continue;
    if (ch == 3 && (cnt & 0x2000)) continue;
    sum[0] += lr[ch][0];
    sum[1] += lr[ch][1];
  }
  if (s->strm.playing && s->bus.dom != 1) ac_strm_sample(s, sum); /* the archive stream's channels (none in Black) */
  mix[0] = sum[0];
  mix[1] = sum[1];
  for (int side = 0; side < 2; side++) out[side] = out_side(s, cnt, bias, side, sum[side], lr[1][side], lr[3][side]);
}

/* ac_mix for n samples whose registers do not change (ac_state.c's block, no capture running): buf[ch][i] is channel ch's
 * output at sample i for the channels in `live` (the others are 0); pcm gets n interleaved L R samples. The same sums as n
 * ac_mix calls, with each channel's CNT decoded once. */
void ac_mix_block(AcoreState *s, const s32 (*buf)[AC_BLOCK], u32 live, int n, s16 *pcm) {
  static const int div_shift[4] = {0, 1, 2, 4};
  u32 cnt = ac_hw_read(s, AC_REG_SOUNDCNT, 2);
  u32 bias = ac_hw_read(s, AC_REG_SOUNDBIAS, 2) & 0x3FF;
  u32 insum = live & ~((cnt & 0x1000 ? 1u << 1 : 0) | (cnt & 0x2000 ? 1u << 3 : 0));
  int nch = 0, chs[AC_NCH], dsh[AC_NCH];
  s64 vol[AC_NCH], pl[AC_NCH], pr[AC_NCH];
  for (int ch = 0; ch < AC_NCH; ch++) {
    if (!(live >> ch & 1)) continue;
    u32 c = ac_ch_reg32(s, ch, AC_CH_CNT);
    int v = (int)AC_CNT_VOL(c), p = (int)AC_CNT_PAN(c);
    if (v == 127) v = 128;
    if (p == 127) p = 128;
    chs[nch] = ch, dsh[nch] = div_shift[AC_CNT_DIV(c)], vol[nch] = v, pl[nch] = 128 - p, pr[nch] = p;
    nch++;
  }
  int strm = s->bus.dom != 1; /* the archive stream is mixed with the music bus when it is split */
  for (int i = 0; i < n; i++) {
    s32 sum[2] = {0, 0}, lr1[2] = {0, 0}, lr3[2] = {0, 0};
    for (int k = 0; k < nch; k++) {
      int ch = chs[k];
      s32 d = buf[ch][i];
      if (d == 0) continue;
      s64 v = (((s64)d * 16) >> dsh[k]) * vol[k];
      s32 l = (s32)((v * pl[k]) >> 10), r = (s32)((v * pr[k]) >> 10);
      if (ch == 1) lr1[0] = l, lr1[1] = r;
      if (ch == 3) lr3[0] = l, lr3[1] = r;
      if (insum >> ch & 1) sum[0] += l, sum[1] += r;
    }
    if (strm && s->strm.playing) ac_strm_sample(s, sum);
    pcm[2 * i] = out_side(s, cnt, bias, 0, sum[0], lr1[0], lr3[0]);
    pcm[2 * i + 1] = out_side(s, cnt, bias, 1, sum[1], lr1[1], lr3[1]);
  }
}
