/* The sound archive's stream (ac_strm.c, docs/acore.md "The archive stream"): STRM decoding (PCM8, PCM16, IMA-ADPCM, blocks),
 * its samples in the mix at full volume, the fade, the end and the position. */
#include "act.h"
#include <stdlib.h>
#include <string.h>

static void put32(u8 *p, u32 v) { p[0] = (u8)v, p[1] = (u8)(v >> 8), p[2] = (u8)(v >> 16), p[3] = (u8)(v >> 24); }
static void put16(u8 *p, u32 v) { p[0] = (u8)v, p[1] = (u8)(v >> 8); }

/* a STRM file: type, channels, rate, nsamp, blocks of bsamp samples (blen bytes), the data given (already interleaved) */
static u8 *make_strm(int type, int nch, int rate, int loop, u32 loop_start, u32 nsamp, u32 nblk, u32 blen, u32 bsamp,
                     u32 lblen, u32 lbsamp, const u8 *data, u32 dlen, size_t *size) {
  u8 *f = calloc(1, 0x68 + dlen);
  memcpy(f, "STRM", 4);
  put32(f + 8, 0x68 + dlen);
  memcpy(f + 0x10, "HEAD", 4);
  u8 *h = f + 0x18;
  h[0] = (u8)type, h[1] = (u8)loop, h[2] = (u8)nch;
  put16(h + 4, (u32)rate);
  put32(h + 8, loop_start), put32(h + 12, nsamp), put32(h + 16, 0x68), put32(h + 20, nblk);
  put32(h + 24, blen), put32(h + 28, bsamp), put32(h + 32, lblen), put32(h + 36, lbsamp);
  memcpy(f + 0x68, data, dlen);
  *size = 0x68 + dlen;
  return f;
}

ACT_TEST(p20_decode_pcm16_blocks) { /* stereo, two blocks of 4 and a last of 2: L R interleaved per block */
  u8 d[(4 + 4 + 2) * 2 * 2];
  u8 *p = d;
  for (int b = 0; b < 3; b++) {
    int n = b < 2 ? 4 : 2;
    for (int c = 0; c < 2; c++)
      for (int i = 0; i < n; i++, p += 2) put16(p, (u32)(s16)((c ? -1 : 1) * (100 * (b * 4 + i) + 1)));
  }
  size_t size;
  u8 *f = make_strm(1, 2, 32728, 0, 0, 10, 3, 8, 4, 4, 2, d, sizeof d, &size);
  AcoreStrmInfo in;
  int16_t *pcm = acore_strm_decode(f, size, &in);
  ACT_CHECK(pcm != NULL);
  ACT_EQ(in.nsamp, 10);
  ACT_EQ(in.nch, 2);
  for (int i = 0; i < 10; i++) {
    ACT_EQ(pcm[2 * i], 100 * i + 1);
    ACT_EQ(pcm[2 * i + 1], -(100 * i + 1));
  }
  free(pcm);
  free(f);
}

ACT_TEST(p20_decode_pcm8_adpcm) {
  u8 d8[4] = {0x7F, 0x80, 0x01, 0};
  size_t size;
  u8 *f = make_strm(0, 1, 16000, 0, 0, 3, 1, 4, 3, 4, 3, d8, 4, &size);
  AcoreStrmInfo in;
  int16_t *pcm = acore_strm_decode(f, size, &in);
  ACT_EQ(pcm[0], 0x7F00);
  ACT_EQ(pcm[1], -0x8000);
  ACT_EQ(pcm[2], 0x100);
  free(pcm), free(f);
  /* ADPCM: header 1000, index 0; code 7 = +(7>>3 + 7>>2 + 7>>1 + 7) = 0+1+3+7 = 11; index 0 + 8 = 8; code 8|0 (= -0) at step 16 -> -2 */
  u8 da[6] = {0xE8, 0x03, 0x00, 0x00, 0x87, 0x00};
  f = make_strm(2, 1, 16000, 0, 0, 2, 1, 6, 2, 6, 2, da, 6, &size);
  pcm = acore_strm_decode(f, size, &in);
  ACT_EQ(pcm[0], 1011);
  ACT_EQ(pcm[1], 1009);
  free(pcm), free(f);
}

ACT_TEST(p20_play_fade_end) { /* full volume, full left / right: each sample is the DAC's input as is (DS mode, bias 0x200) */
  AcoreState *s = act_state_hw();
  act_reg(s, AC_REG_SOUNDCNT, 0x807F, 2);
  act_reg(s, AC_REG_SOUNDBIAS, 0x200, 2);
  enum { N = 4000 };
  int16_t *pcm = malloc(N * 2 * sizeof *pcm);
  for (int i = 0; i < N; i++) pcm[2 * i] = 6400, pcm[2 * i + 1] = -3200;
  AcoreStrmInfo in = {N, 2, 32728, 0, 0};
  acore_strm_set(s, pcm, &in, 127, 0);
  ACT_EQ(acore_strm_pos_ms(s), 0);
  acore_strm_start(s);
  act_run(s, 100 * AC_CYCLES_PER_SAMPLE);
  ACT_EQ(act_sample(s, 50, 0), 6400);
  ACT_EQ(act_sample(s, 50, 1), -3200);
  ACT_EQ(acore_strm_pos_ms(s), 100 * 1000 / 32728);
  acore_strm_stop(s, 2); /* two frames: half volume, then stopped */
  act_run(s, 2 * AC_CYCLES_PER_FRAME + 2 * AC_CYCLES_PER_SAMPLE);
  ACT_CHECK(!acore_strm_playing(s));
  ACT_EQ(acore_strm_pos_ms(s), 0);
  s16 mid = act_sample(s, 100 + 600, 0);
  ACT_CHECK(mid > 0 && mid < 6400);
  ACT_EQ(act_sample(s, 100 + 2 * 548 + 1, 0), 0);
  /* to the end without a loop: stops by itself */
  acore_strm_set(s, pcm, &in, 127, 0);
  acore_strm_start(s);
  act_run(s, (u64)(N + 10) * AC_CYCLES_PER_SAMPLE);
  ACT_CHECK(!acore_strm_playing(s));
  acore_strm_set(s, NULL, NULL, 0, 0);
  free(pcm);
}
