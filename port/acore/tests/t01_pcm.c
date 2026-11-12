/* port/acore piece 1: unit tests (docs/acore.md, piece 1's "Tests:"). OWNED BY PIECE 1.
 *
 * The expected values are worked from GBATEK 57/61; where the oracle differs (the start delay is counted in timer
 * overflows from the start step, Hold does nothing at a one-shot's end, a TMR change is taken at the next overflow)
 * the test says so. The samples are the channel's own (before the mixer): a hand-written loop reads ac_ch_output and
 * steps exactly as ac_hw_sample does, while a second state records the same writes as a stream (ACT_EMIT=DIR
 * writes DIR/p01_*.acs; the mixer of piece 4 turns them into the oracle's WAV: pan 127 puts the channel alone on
 * the right).
 */
#include "act.h"
#include <string.h>

#define NS 120
#define DATA 0x02100000u
#define CH(n, r) (0x04000400u + 0x10u * (n) + (r))
#define PAN_R 0x007F0000u /* pan 127: all to the right */

typedef struct {
  AcoreState *m; /* driven by hand */
  AcoreState *e; /* the same writes as a stream */
  int k;
  s32 out[NS];
  int busy[NS]; /* the busy bit after step k */
} Rig;

static void rig_init(Rig *r) {
  memset(r, 0, sizeof *r);
  r->m = act_state_hw();
  r->e = act_state_hw();
  act_reg(r->e, AC_REG_SOUNDCNT, 0x807F, 2);
  act_reg(r->e, AC_REG_SOUNDBIAS, 0x200, 2);
}
static void rig_mem(Rig *r, const void *d, u32 n) {
  act_mem(r->m, DATA, d, n);
  act_mem(r->e, DATA, d, n);
}
static void rig_reg(Rig *r, u32 a, u32 v, int sz) {
  act_reg(r->m, a, v, sz);
  act_reg(r->e, a, v, sz);
}
static void rig_run(Rig *r, int n) {
  for (int i = 0; i < n && r->k < NS; i++, r->k++) {
    r->out[r->k] = ac_ch_output(r->m, 0);
    ac_ch_step(r->m, 0);
    r->busy[r->k] = (ac_ch_reg32(r->m, 0, AC_CH_CNT) >> 31) & 1;
    act_run(r->e, AC_CYCLES_PER_SAMPLE);
  }
}
/* the five writes of a channel start */
static void rig_start(Rig *r, int ch, u32 tmr, u32 pnt, u32 len, u32 cnt) {
  rig_reg(r, CH(ch, AC_CH_SAD), DATA, 4);
  rig_reg(r, CH(ch, AC_CH_TMR), tmr, 2);
  rig_reg(r, CH(ch, AC_CH_PNT), pnt, 2);
  rig_reg(r, CH(ch, AC_CH_LEN), len, 4);
  rig_reg(r, CH(ch, AC_CH_CNT), cnt, 4);
}
#define CNT(fmt, rep, extra) (0x80000000u | (u32)(fmt) << 29 | (u32)(rep) << 27 | PAN_R | 0x7Fu | (extra))

static void ramp16(Rig *r, int n) { /* sample i = 0x100 * (i + 1) */
  s16 d[64];
  for (int i = 0; i < n; i++) d[i] = (s16)(0x100 * (i + 1));
  rig_mem(r, d, (u32)(2 * n));
}

ACT_TEST(p01_pcm16_one_per_sample) { /* TMR 0xFE00: 512 counts per sample = one per output sample */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  rig_run(&r, 10);
  rig_start(&r, 0, 0xFE00, 0, 16, CNT(AC_FMT_PCM16, AC_REPEAT_ONESHOT, 0));
  rig_run(&r, 40);
  for (int k = 0; k < 12; k++) ACT_EQ(r.out[k], 0); /* started at 10: sample 0 is heard at 12 */
  for (int i = 0; i < 32; i++) ACT_EQ(r.out[12 + i], 0x100 * (i + 1));
  ACT_EQ(r.out[44], 0);
  ACT_EQ(r.out[45], 0);
  act_emit(r.e, "p01_pcm16_fe00");
}

ACT_TEST(p01_pcm16_two_per_sample) { /* TMR 0xFF00: 256 counts per sample = two per output sample */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  rig_run(&r, 10);
  rig_start(&r, 0, 0xFF00, 0, 16, CNT(AC_FMT_PCM16, AC_REPEAT_ONESHOT, 0));
  rig_run(&r, 30);
  ACT_EQ(r.out[10], 0);
  for (int k = 11; k < 27; k++) ACT_EQ(r.out[k], 0x100 * (2 * (k - 11) + 2)); /* every other one: 2, 4, 6... */
  ACT_EQ(r.out[27], 0);
  act_emit(r.e, "p01_pcm16_ff00");
}

ACT_TEST(p01_start_delay_counts_overflows) { /* GBATEK 61: PCM starts after 3 samples: the 3rd overflow is sample 0 */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  rig_run(&r, 10);
  rig_start(&r, 0, 0xFC00, 0, 16, CNT(AC_FMT_PCM16, AC_REPEAT_LOOP, 0)); /* 1024 counts = 2 steps per sample */
  rig_run(&r, 20);
  for (int k = 0; k < 15; k++) ACT_EQ(r.out[k], 0);
  ACT_EQ(r.out[15], 0x100); /* 3 * 1024 counts = 6 steps, less the first step the oracle runs at the start */
  ACT_EQ(r.out[16], 0x100);
  ACT_EQ(r.out[17], 0x200);
  ACT_EQ(r.out[18], 0x200);
  ACT_EQ(r.out[19], 0x300);
  act_emit(r.e, "p01_start_delay");
}

ACT_TEST(p01_odd_timer_keeps_the_remainder) { /* TMR 0xFD80: 640 counts per sample = 1.25 steps */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  rig_start(&r, 0, 0xFD80, 0, 16, CNT(AC_FMT_PCM16, AC_REPEAT_LOOP, 0));
  rig_run(&r, 24);
  /* overflows at 640 * n; step k has run 512 * (k + 1) counts when it is read: sample 0 (3rd overflow) at 1920 */
  static const int idx[] = {-1, -1, -1, 0, 1, 1, 2, 3, 4, 5, 5, 6, 7};
  for (int k = 0; k < 13; k++) ACT_EQ(r.out[k], idx[k] < 0 ? 0 : 0x100 * (idx[k] + 1));
  act_emit(r.e, "p01_odd_timer");
}

ACT_TEST(p01_pcm8_scaling) { /* PCM8 NN -> NN00 */
  Rig r;
  rig_init(&r);
  static const u8 d[16] = {0x7F, 0x80, 0x01, 0xFF, 0x00, 0x40, 0xC0, 0x10, 1, 2, 3, 4, 5, 6, 7, 8};
  rig_mem(&r, d, sizeof d);
  rig_start(&r, 0, 0xFE00, 0, 4, CNT(AC_FMT_PCM8, AC_REPEAT_ONESHOT, 0));
  rig_run(&r, 24);
  static const s32 want[8] = {0x7F00, -0x8000, 0x100, -0x100, 0, 0x4000, -0x4000, 0x1000};
  ACT_EQ(r.out[1], 0);
  for (int i = 0; i < 16; i++) ACT_EQ(r.out[2 + i], i < 8 ? want[i] : 0x100 * (i - 7));
  ACT_EQ(r.out[18], 0);
  act_emit(r.e, "p01_pcm8");
}

ACT_TEST(p01_loop_wrap) { /* loop: PNT words once, then LEN words repeated: the first PNT words are not repeated */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  rig_start(&r, 0, 0xFE00, 2, 3, CNT(AC_FMT_PCM16, AC_REPEAT_LOOP, 0)); /* samples 0..9, wrap to 4 */
  rig_run(&r, 40);
  ACT_EQ(r.out[1], 0);
  for (int i = 0; i < 10; i++) ACT_EQ(r.out[2 + i], 0x100 * (i + 1));
  for (int j = 0; j < 20; j++) ACT_EQ(r.out[12 + j], 0x100 * (4 + j % 6 + 1));
  ACT_EQ(r.busy[39], 1);
  act_emit(r.e, "p01_loop");
}

ACT_TEST(p01_oneshot_end_and_busy) { /* the busy bit clears as the last sample starts; the output drops after it */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  rig_start(&r, 0, 0xFE00, 1, 3, CNT(AC_FMT_PCM16, AC_REPEAT_ONESHOT, 0)); /* PNT + LEN = 4 words = 8 samples */
  rig_run(&r, 20);
  ACT_EQ(r.out[9], 0x800); /* sample 7, the last, is heard at step 9 */
  ACT_EQ(r.out[10], 0);
  ACT_EQ(r.busy[8], 1);
  ACT_EQ(r.busy[9], 0); /* read after step 9: cleared when sample 7 was taken */
  ACT_EQ(r.busy[19], 0);
  ACT_EQ(ac_ch_reg32(r.m, 0, AC_CH_CNT) & 0x7FFFFFFFu, CNT(AC_FMT_PCM16, AC_REPEAT_ONESHOT, 0) & 0x7FFFFFFFu);
  act_emit(r.e, "p01_oneshot");
}

ACT_TEST(p01_hold_is_not_honoured) { /* GBATEK 61 keeps the last sample with Hold; the oracle drops it (suspect.txt) */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  rig_start(&r, 0, 0xFE00, 0, 4, CNT(AC_FMT_PCM16, AC_REPEAT_ONESHOT, 0x8000));
  rig_run(&r, 20);
  ACT_EQ(r.out[9], 0x800);
  ACT_EQ(r.out[10], 0);
  ACT_EQ(r.out[19], 0);
  act_emit(r.e, "p01_hold");
}

ACT_TEST(p01_restart_while_running) { /* stop, start: the sound begins again with the full start delay */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  u32 c = CNT(AC_FMT_PCM16, AC_REPEAT_LOOP, 0);
  rig_start(&r, 0, 0xFE00, 0, 16, c);
  rig_run(&r, 10);
  ACT_EQ(r.out[9], 0x800);
  rig_reg(&r, CH(0, AC_CH_CNT), c & 0x7FFFFFFFu, 4); /* stop before step 10 */
  rig_run(&r, 2);
  ACT_EQ(r.out[10], 0);
  rig_reg(&r, CH(0, AC_CH_CNT), c, 4); /* start before step 12 */
  rig_run(&r, 6);
  ACT_EQ(r.out[12], 0);
  ACT_EQ(r.out[13], 0);
  ACT_EQ(r.out[14], 0x100); /* from sample 0 again, 2 steps after the start */
  ACT_EQ(r.out[15], 0x200);
  act_emit(r.e, "p01_restart");
}

ACT_TEST(p01_restart_same_step) { /* stop and start with no step between */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  u32 c = CNT(AC_FMT_PCM16, AC_REPEAT_LOOP, 0);
  rig_start(&r, 0, 0xFE00, 0, 16, c);
  rig_run(&r, 10);
  rig_reg(&r, CH(0, AC_CH_CNT), c & 0x7FFFFFFFu, 4);
  rig_reg(&r, CH(0, AC_CH_CNT), c, 4);
  rig_run(&r, 5);
  ACT_EQ(r.out[9], 0x800);
  ACT_EQ(r.out[10], 0);
  ACT_EQ(r.out[11], 0);
  ACT_EQ(r.out[12], 0x100);
  act_emit(r.e, "p01_restart_same");
}

ACT_TEST(p01_minimum_length) { /* PNT + LEN < 4 words: busy stays set, no sound; 4 words play */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  rig_start(&r, 0, 0xFE00, 1, 2, CNT(AC_FMT_PCM16, AC_REPEAT_ONESHOT, 0)); /* 3 words */
  rig_run(&r, 40);
  for (int k = 0; k < 40; k++) ACT_EQ(r.out[k], 0);
  ACT_EQ(r.busy[39], 1);
  Rig q;
  rig_init(&q);
  ramp16(&q, 32);
  rig_start(&q, 0, 0xFE00, 0, 4, CNT(AC_FMT_PCM16, AC_REPEAT_ONESHOT, 0));
  rig_run(&q, 20);
  ACT_EQ(q.out[2], 0x100);
  ACT_EQ(q.out[9], 0x800);
  Rig p;
  rig_init(&p);
  ramp16(&p, 32);
  rig_start(&p, 0, 0xFE00, 2, 1, CNT(AC_FMT_PCM16, AC_REPEAT_LOOP, 0)); /* 3 words in loop mode: the same */
  rig_run(&p, 20);
  for (int k = 0; k < 20; k++) ACT_EQ(p.out[k], 0);
  act_emit(r.e, "p01_minlen");
}

ACT_TEST(p01_tmr_change_is_taken_at_the_next_overflow) { /* a pitch change of a running channel (oracle) */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  rig_start(&r, 0, 0xFE00, 0, 16, CNT(AC_FMT_PCM16, AC_REPEAT_LOOP, 0));
  rig_run(&r, 6);
  ACT_EQ(r.out[2], 0x100);
  ACT_EQ(r.out[5], 0x400);
  rig_reg(&r, CH(0, AC_CH_TMR), 0xFC00, 2); /* 1024 counts per sample from the next overflow on */
  rig_run(&r, 8);
  ACT_EQ(r.out[6], 0x500); /* the overflow in flight (512 counts) still used the old reload */
  ACT_EQ(r.out[7], 0x500);
  ACT_EQ(r.out[8], 0x600);
  ACT_EQ(r.out[9], 0x600);
  ACT_EQ(r.out[10], 0x700);
  act_emit(r.e, "p01_tmr_change");
}

ACT_TEST(p01_sad_is_latched_at_the_start) { /* later SAD/PNT/LEN writes do not move a running channel (no game does it) */
  Rig r;
  rig_init(&r);
  ramp16(&r, 32);
  rig_start(&r, 0, 0xFE00, 0, 16, CNT(AC_FMT_PCM16, AC_REPEAT_LOOP, 0));
  rig_run(&r, 6);
  rig_reg(&r, CH(0, AC_CH_SAD), DATA + 16, 4);
  rig_run(&r, 4);
  ACT_EQ(r.out[7], 0x600);
  ACT_EQ(r.out[9], 0x800);
}

ACT_TEST(p01_channels_are_independent) {
  AcoreState *s = act_state_hw();
  s16 a[16], b[16];
  for (int i = 0; i < 16; i++) a[i] = (s16)(0x100 * (i + 1)), b[i] = (s16)(-0x100 * (i + 1));
  act_mem(s, DATA, a, sizeof a);
  act_mem(s, DATA + 0x100, b, sizeof b);
  act_reg(s, CH(3, AC_CH_SAD), DATA, 4), act_reg(s, CH(3, AC_CH_TMR), 0xFE00, 2), act_reg(s, CH(3, AC_CH_LEN), 8, 4);
  act_reg(s, CH(9, AC_CH_SAD), DATA + 0x100, 4), act_reg(s, CH(9, AC_CH_TMR), 0xFF00, 2);
  act_reg(s, CH(9, AC_CH_LEN), 8, 4);
  act_reg(s, CH(3, AC_CH_CNT), CNT(AC_FMT_PCM16, AC_REPEAT_LOOP, 0), 4);
  act_reg(s, CH(9, AC_CH_CNT), CNT(AC_FMT_PCM16, AC_REPEAT_LOOP, 0), 4);
  s32 o3[6], o9[6];
  for (int k = 0; k < 6; k++) {
    o3[k] = ac_ch_output(s, 3), o9[k] = ac_ch_output(s, 9);
    ac_ch_step(s, 3), ac_ch_step(s, 9);
  }
  ACT_EQ(o3[2], 0x100);
  ACT_EQ(o3[5], 0x400);
  ACT_EQ(o9[0], 0);
  ACT_EQ(o9[1], -0x200);
  ACT_EQ(o9[2], -0x400);
  ACT_EQ(o9[5], -0xA00);
  ACT_EQ(ac_ch_output(s, 5), 0);
}
