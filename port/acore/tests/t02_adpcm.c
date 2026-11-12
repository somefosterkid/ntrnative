/* port/acore piece 2: unit tests (docs/acore.md, piece 2's "Tests:"). OWNED BY PIECE 2.
 * Expected values are worked by hand from GBATEK's "in practice" formula (Diff = T/8 + [b0] T/4 + [b1] T/2 + [b2] T,
 * each division truncating). */
#include "act.h"

#define SRC 0x02100000u

/* a state with an ADPCM block at SRC: header word, then the bytes */
static AcoreState *setup(u32 pred, u32 idx, const u8 *bytes, u32 n) {
  AcoreState *s = act_state_hw();
  u8 buf[512];
  u32 hdr = (pred & 0xFFFF) | (idx << 16);
  buf[0] = (u8)hdr;
  buf[1] = (u8)(hdr >> 8);
  buf[2] = (u8)(hdr >> 16);
  buf[3] = (u8)(hdr >> 24);
  for (u32 i = 0; i < n; i++) buf[4 + i] = bytes[i];
  act_mem(s, SRC, buf, 4 + n);
  /* SAD only (the start bit stays clear: piece 1 is not under test here) */
  act_reg(s, 0x04000400 + AC_CH_SAD, SRC, 4);
  ac_adpcm_start(s, 0);
  return s;
}

ACT_TEST(p02_block_of_16_nibbles) {
  static const u8 blk[8] = {0x37, 0x8F, 0x50, 0x69, 0xA1, 0xC4, 0xE2, 0x07};
  /* header: pred 0, index 0. Per nibble: step T, diff, predictor (index after):
   *  7: T7  diff 0+1+3+7=11 -> 11 (8)    3: T16 2+4+8=14 -> 25 (7)     F: T14 1+3+7+14=25 -> 0 (15)
   *  8: T31 3 -> -3 (14)                 0: T28 3 -> 0 (13)           5: T25 3+6+25=34 -> 34 (17)
   *  9: T37 4+9=13 -> 21 (16)            6: T34 4+17+34=55 -> 76 (22) 1: T60 7+15=22 -> 98 (21)
   *  A: T55 6+27=33 -> 65 (20)           4: T50 6+50=56 -> 121 (22)   C: T60 7+60=67 -> 54 (24)
   *  2: T73 9+36=45 -> 99 (23)           E: T66 8+33+66=107 -> -8 (29)
   *  7: T118 14+29+59+118=220 -> 212 (37)                              0: T253 31 -> 243 (36)
   * (index steps: nibble&7 0-3 -> -1, 4 -> +2, 5 -> +4, 6 -> +6, 7 -> +8) */
  static const int want[16] = {11, 25, 0, -3, 0, 34, 21, 76, 98, 65, 121, 54, 99, -8, 212, 243};
  static const int widx[16] = {8, 7, 15, 14, 13, 17, 16, 22, 21, 20, 22, 24, 23, 29, 37, 36};
  AcoreState *s = setup(0, 0, blk, 8);
  for (u32 i = 0; i < 16; i++) {
    ACT_EQ(ac_adpcm_next(s, 0, i), want[i]);
    ACT_EQ(s->p02.idx[0], widx[i]);
  }
  /* the header's own values */
  s = setup(0x1234, 5, blk, 8);
  ACT_EQ(s->p02.pred[0], 0x1234);
  ACT_EQ(s->p02.idx[0], 5);
  s = setup(0x8001, 3, blk, 8); /* negative start value is sign-extended */
  ACT_EQ(s->p02.pred[0], -0x7FFF);
}

ACT_TEST(p02_clip_asymmetry) {
  /* high: +7FFFh clips upward */
  static const u8 hi[2] = {0x77, 0x07};
  AcoreState *s = setup(0x7FF0, 88, hi, 2); /* T = 7FFFh */
  ACT_EQ(ac_adpcm_next(s, 0, 0), 0x7FFF);
  ACT_EQ(ac_adpcm_next(s, 0, 1), 0x7FFF);
  /* a start value of -8000h is not clipped by an addition of 0: stays -8000h (T=7 -> T/8 = 0, nibble 0) */
  static const u8 z[4] = {0x10, 0x80, 0x08, 0x00};
  s = setup(0x8000, 0, z, 4);
  ACT_EQ(ac_adpcm_next(s, 0, 0), -0x8000); /* nibble 0: diff 0, "Max(+7FFF)" leaves -8000h */
  ACT_EQ(ac_adpcm_next(s, 0, 1), -0x7FFF); /* nibble 1: diff 1 */
  /* low: Min(-7FFFh) clips -8000h to -7FFFh, even for a subtraction of 0 */
  s = setup(0x8000, 0, z, 4);
  ACT_EQ(ac_adpcm_next(s, 0, 3), -0x7FFF); /* nibble 3 is 8: subtract 0 from -8000h */
}

ACT_TEST(p02_index_clamps) {
  /* nibble 0 (-1) at index 0 stays 0; nibble 7 (+8) at 88 stays 88; a header index above 88 clamps after one step */
  static const u8 b[4] = {0x70, 0x00, 0x00, 0x00};
  AcoreState *s = setup(0, 0, b, 4);
  ac_adpcm_next(s, 0, 1); /* high nibble of byte 0 = 7 */
  /* nibble 0 first on a fresh state */
  s = setup(0, 0, b, 4);
  ac_adpcm_next(s, 0, 0);
  ACT_EQ(s->p02.idx[0], 0);
  ac_adpcm_next(s, 0, 1); /* 7: +8 */
  ACT_EQ(s->p02.idx[0], 8);
  static const u8 c[2] = {0x77, 0x77};
  s = setup(0, 86, c, 2);
  ac_adpcm_next(s, 0, 0);
  ACT_EQ(s->p02.idx[0], 88);
  ac_adpcm_next(s, 0, 1);
  ACT_EQ(s->p02.idx[0], 88);
  static const u8 d[2] = {0x00, 0x00};
  s = setup(0, 88, d, 2); /* 88 steps down by one per zero nibble, 0 floors */
  ac_adpcm_next(s, 0, 0);
  ACT_EQ(s->p02.idx[0], 87);
  s = setup(0, 127, d, 2); /* header index past the table: step 0 (no change), then clamped into 0..88 */
  ACT_EQ(ac_adpcm_next(s, 0, 0), 0);
  ACT_EQ(s->p02.idx[0], 88);
}

ACT_TEST(p02_loop_restores_saved_state) {
  static const u8 blk[8] = {0x37, 0x8F, 0x50, 0x69, 0xA1, 0xC4, 0xE2, 0x07};
  AcoreState *s = setup(0, 0, blk, 8);
  s32 first[4], again[4];
  ac_adpcm_next(s, 0, 0);
  ac_adpcm_next(s, 0, 1); /* pred 25, idx 7 */
  ac_adpcm_loop_save(s, 0);
  for (u32 i = 0; i < 4; i++) first[i] = ac_adpcm_next(s, 0, 2 + i);
  ACT_EQ(first[0], 0);
  ACT_EQ(first[1], -3);
  ACT_EQ(first[3], 34);
  ac_adpcm_next(s, 0, 6);
  ac_adpcm_loop_restore(s, 0);
  ACT_EQ(s->p02.pred[0], 25);
  ACT_EQ(s->p02.idx[0], 7);
  for (u32 i = 0; i < 4; i++) again[i] = ac_adpcm_next(s, 0, 2 + i);
  for (u32 i = 0; i < 4; i++) ACT_EQ(again[i], first[i]);
  /* a second channel is independent */
  AcoreState *t = setup(100, 10, blk, 8);
  ACT_EQ(t->p02.pred[1], 0);
  ACT_EQ(t->p02.pred[0], 100);
}

ACT_TEST(p02_start_delay) {
  /* 3 samples as for PCM + 8 for the header word's 8 nibbles (GBATEK 61) */
  ACT_EQ(AC_ADPCM_START_DELAY, 3 + 8);
  /* the decoder's side: the header consumes no nibble index, nibble 0 is the first byte after it */
  static const u8 b[1] = {0x07};
  AcoreState *s = setup(0, 0, b, 1);
  ACT_EQ(ac_adpcm_next(s, 0, 0), 11); /* low nibble 7 of byte SRC+4 */
}

/* a stream for the oracle (hardware mode): channel 0 ADPCM, start bit set, a long block of varied nibbles */
ACT_TEST(p02_emit_adpcm_stream) {
  AcoreState *s = act_state_hw();
  u8 buf[4 + 2000];
  u32 x = 12345;
  buf[0] = 0x00; buf[1] = 0x10; buf[2] = 10; buf[3] = 0; /* pred 0x1000, index 10 */
  for (u32 i = 0; i < 2000; i++) {
    x = x * 1103515245u + 12345u;
    buf[4 + i] = (u8)(x >> 16);
  }
  act_mem(s, SRC, buf, sizeof buf);
  act_reg(s, 0x04000504, 0x200, 2);           /* SOUNDBIAS: the default 0x200 (the oracle starts at 0) */
  act_reg(s, 0x04000500, 0x807F, 2);          /* SOUNDCNT: master 127, enable */
  act_reg(s, 0x04000400 + AC_CH_SAD, SRC, 4);
  act_reg(s, 0x04000400 + 8, 0xFE00, 2);      /* TMR */
  act_reg(s, 0x04000400 + AC_CH_PNT, 0, 2);
  act_reg(s, 0x04000400 + AC_CH_LEN, 500, 4);
  act_reg(s, 0x04000400, 0x8000007Fu | (2u << 29) | (2u << 27) | (64u << 16), 4); /* ADPCM, one-shot (mode 2), vol, pan */
  act_run(s, 2100 * AC_CYCLES_PER_SAMPLE);
  act_emit(s, "p02_adpcm_noise");
}

/* the same data looped: PNT 4 words (loop start = data nibble 24), LEN 20 words (the data ends at nibble 184) */
ACT_TEST(p02_emit_adpcm_loop) {
  AcoreState *s = act_state_hw();
  u8 buf[4 + 2000];
  u32 x = 12345;
  buf[0] = 0x00; buf[1] = 0x10; buf[2] = 10; buf[3] = 0;
  for (u32 i = 0; i < 2000; i++) {
    x = x * 1103515245u + 12345u;
    buf[4 + i] = (u8)(x >> 16);
  }
  act_mem(s, SRC, buf, sizeof buf);
  act_reg(s, 0x04000504, 0x200, 2);
  act_reg(s, 0x04000500, 0x807F, 2);
  act_reg(s, 0x04000400 + AC_CH_SAD, SRC, 4);
  act_reg(s, 0x04000400 + 8, 0xFE00, 2);
  act_reg(s, 0x04000400 + AC_CH_PNT, 4, 2);
  act_reg(s, 0x04000400 + AC_CH_LEN, 20, 4);
  act_reg(s, 0x04000400, 0x8000007Fu | (2u << 29) | (1u << 27) | (64u << 16), 4); /* ADPCM, loop (mode 1) */
  act_run(s, 2100 * AC_CYCLES_PER_SAMPLE);
  act_emit(s, "p02_adpcm_loop");
}
