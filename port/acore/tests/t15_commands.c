/* port/acore piece 15: unit tests (docs/acore.md, piece 15's "Tests:"). OWNED BY PIECE 15.
 * The expected register writes are the oracle's trace of the ROM's own driver (see p15_commands.h); GBATEK and the
 * SDAT docs do not cover the commands. Commands that call other pieces (players, channel allocation, variables) are
 * ACT_TODO until those pieces merge (the stubs do nothing). */
#include "act.h"

#define BASE 0x04000000u
#define CH(n) (BASE + 0x400u + 0x10u * (n))

/* ---- a register-write trace of one command list ---- */
typedef struct W {
  u32 addr;
  int size;
  u32 val;
} W;
static W tr[256];
static int ntr;

static void trace_begin(AcoreState *s) { s->trace = tmpfile(); }
static void trace_end(AcoreState *s) {
  ntr = 0;
  rewind(s->trace);
  unsigned long long t;
  unsigned a, v;
  int sz;
  while (ntr < 256 && fscanf(s->trace, "%llu %x %d %x", &t, &a, &sz, &v) == 4) tr[ntr].addr = a, tr[ntr].size = sz, tr[ntr].val = v, ntr++;
  fclose(s->trace);
  s->trace = NULL;
}
#define EXPECT_W(i, a, sz, v)                                                                                         \
  do {                                                                                                                \
    ACT_CHECK((i) < ntr);                                                                                             \
    ACT_EQ(tr[i].addr, (a));                                                                                          \
    ACT_EQ(tr[i].size, (sz));                                                                                         \
    ACT_EQ(tr[i].val, (v));                                                                                           \
  } while (0)

/* send one list and run the driver's command step (the periodic wake is piece 17's) */
static void go(AcoreState *s) {
  act_flush(s); /* queues the list and records a WAKE for the oracle streams (piece 17's wake is a stub here) */
  ac_cmd_proc(s);
}
static void one(AcoreState *s, u32 id, u32 a0, u32 a1, u32 a2, u32 a3) {
  act_cmd(s, id, a0, a1, a2, a3);
  go(s);
}

/* ---- SETUP_CHANNEL_PCM ---- */
ACT_TEST(p15_setup_pcm) {
  AcoreState *s = act_state();
  trace_begin(s);
  /* ch 4, timer 0xF123, PCM8 loop 0, loopStart 3, loopLen 100, volume 90, shift 1, pan 20 */
  one(s, AC_CMD_SETUP_CHANNEL_PCM, 0xF123u << 16 | 4, 0x02230000, 90u << 24 | 1u << 22 | 100, 0u << 26 | 0u << 24 | 20u << 16 | 3);
  trace_end(s);
  ACT_EQ(ntr, 5);
  EXPECT_W(0, CH(4) + 0x0, 4, 0x0014015Au); /* CNT: no start bit */
  EXPECT_W(1, CH(4) + 0x8, 2, 0x0EDDu);     /* TMR = -timer */
  EXPECT_W(2, CH(4) + 0xA, 2, 3);           /* PNT */
  EXPECT_W(3, CH(4) + 0xC, 4, 100);         /* LEN */
  EXPECT_W(4, CH(4) + 0x4, 4, 0x02230000);  /* SAD */
  ACT_EQ(ac_hw_read(s, CH(4), 4) >> 31, 0);
}

ACT_TEST(p15_setup_pcm_formats) {
  AcoreState *s = act_state();
  /* PCM16, one-shot (loop 2), shift 2 */
  one(s, AC_CMD_SETUP_CHANNEL_PCM, 0xF123u << 16 | 4, 0x02230000, 90u << 24 | 2u << 22 | 100, 2u << 26 | 1u << 24 | 20u << 16 | 3);
  ACT_EQ(ac_hw_read(s, CH(4), 4), 0x3014025Au);
  /* ADPCM, loop 2, shift 3, pan 127 */
  one(s, AC_CMD_SETUP_CHANNEL_PCM, 0xF123u << 16 | 4, 0x02230000, 5u << 24 | 3u << 22 | 100, 2u << 26 | 2u << 24 | 127u << 16 | 5);
  ACT_EQ(ac_hw_read(s, CH(4), 4), 0x507F0305u);
  /* timer 0 -> TMR 0; the 22-bit loop length and the 16-bit loop start are not cut further */
  one(s, AC_CMD_SETUP_CHANNEL_PCM, 6, 0x02230000, 127u << 24 | 0x3FFFFF, 2u << 26 | 1u << 24 | 0x1234);
  ACT_EQ(ac_hw_read(s, CH(6) + 8, 2), 0);
  ACT_EQ(ac_hw_read(s, CH(6) + 0xA, 2), 0x1234);
  ACT_EQ(ac_hw_read(s, CH(6) + 0xC, 4), 0x3FFFFF);
  /* the timer's low 16 bits count */
  one(s, AC_CMD_SETUP_CHANNEL_PCM, 0x1000u << 16 | 6, 0x02230000, 127u << 24 | 100, 2u << 26 | 1u << 24);
  ACT_EQ(ac_hw_read(s, CH(6) + 8, 2), 0xF000);
}

ACT_TEST(p15_setup_pcm_bad_channel) {
  AcoreState *s = act_state();
  trace_begin(s);
  one(s, AC_CMD_SETUP_CHANNEL_PCM, 20, 0x02230000, 127u << 24 | 100, 2u << 26 | 1u << 24); /* no channel 20: skipped */
  trace_end(s);
  ACT_EQ(ntr, 0);
}

/* ---- PSG and noise (the oracle's packing: volume | shift << 8, pan | timer << 8, duty) ---- */
ACT_TEST(p15_setup_psg_noise) {
  AcoreState *s = act_state();
  trace_begin(s);
  one(s, AC_CMD_SETUP_CHANNEL_PSG, 8, 100, 64 | 0xF000u << 8, 3);
  one(s, AC_CMD_SETUP_CHANNEL_NOISE, 14, 100 | 1u << 8, 64 | 0xFC00u << 8, 0);
  trace_end(s);
  ACT_EQ(ntr, 4);
  EXPECT_W(0, CH(8), 4, 0x63400064u); /* format 3, duty 3, pan 64, volume 100 */
  EXPECT_W(1, CH(8) + 8, 2, 0x1000);
  EXPECT_W(2, CH(14), 4, 0x60400164u); /* noise: no duty, shift 1 */
  EXPECT_W(3, CH(14) + 8, 2, 0x0400);
}

ACT_TEST(p15_setup_psg_masks) {
  AcoreState *s = act_state();
  trace_begin(s);
  one(s, AC_CMD_SETUP_CHANNEL_PSG, 8, 200 | 7u << 8, 200 | 0x12345u << 8, 5);
  trace_end(s);
  /* volume 7 bits, shift 2 bits, pan 7 bits (the oracle masks them); the timer is -(low 16 bits) */
  EXPECT_W(0, CH(8), 4, 0x65480348u);
  EXPECT_W(1, CH(8) + 8, 2, 0xDCBB);
}

/* ---- START_TIMER / STOP_TIMER ---- */
ACT_TEST(p15_start_stop_timer) {
  AcoreState *s = act_state();
  one(s, AC_CMD_SETUP_CHANNEL_PCM, 0xF000u << 16 | 4, 0x02230000, 127u << 24 | 100, 2u << 26 | 1u << 24 | 64u << 16);
  trace_begin(s);
  one(s, AC_CMD_START_TIMER, 0x10, 0, 0, 0);
  trace_end(s);
  ACT_EQ(ntr, 1);
  EXPECT_W(0, CH(4) + 3, 1, 0xB0); /* the CNT byte 3 with the start bit */
  ACT_EQ(ac_hw_read(s, CH(4), 4), 0xB040007Fu);

  trace_begin(s);
  one(s, AC_CMD_STOP_TIMER, 0x10, 0, 0, 0);
  trace_end(s);
  ACT_EQ(ntr, 1);
  EXPECT_W(0, CH(4), 4, 0x3040007Fu); /* the readback with the start bit cleared */

  trace_begin(s);
  one(s, AC_CMD_START_TIMER, 0x10, 0, 0, 0);
  one(s, AC_CMD_STOP_TIMER, 0x10, 0, 0, 1); /* flags bit 0: hold */
  trace_end(s);
  EXPECT_W(1, CH(4), 4, 0x3040807Fu);
}

ACT_TEST(p15_timer_masks) {
  AcoreState *s = act_state();
  trace_begin(s);
  one(s, AC_CMD_START_TIMER, 0x10030, 0, 0, 0); /* channels 4, 5 (bit 16 is no channel) */
  one(s, AC_CMD_STOP_TIMER, 0x0050, 0, 0, 0);   /* 4 and 6, in channel order */
  trace_end(s);
  ACT_EQ(ntr, 4);
  EXPECT_W(0, CH(4) + 3, 1, 0x80);
  EXPECT_W(1, CH(5) + 3, 1, 0x80);
  EXPECT_W(2, CH(4), 4, 0);
  EXPECT_W(3, CH(6), 4, 0);
}

ACT_TEST(p15_stop_timer_readback_mask) {
  AcoreState *s = act_state();
  /* volume 200 (the register keeps 7 bits), shift 3, pan 200 written as a byte: the STOP's CNT is the masked readback */
  act_reg(s, CH(4), 0x30000000u, 4);
  one(s, AC_CMD_CHANNEL_VOLUME, 0x10, 200, 3, 0);
  one(s, AC_CMD_CHANNEL_PAN, 0x10, 200, 0, 0);
  trace_begin(s);
  one(s, AC_CMD_STOP_TIMER, 0x10, 0, 0, 0);
  trace_end(s);
  EXPECT_W(0, CH(4), 4, 0x30480348u);
}

/* ---- CHANNEL_TIMER / VOLUME / PAN, MASTER_PAN ---- */
ACT_TEST(p15_channel_timer_volume_pan) {
  AcoreState *s = act_state();
  trace_begin(s);
  one(s, AC_CMD_CHANNEL_TIMER, 0x70, 0xF800, 0, 0);
  one(s, AC_CMD_CHANNEL_VOLUME, 0x20, 50, 1, 0);
  one(s, AC_CMD_CHANNEL_PAN, 0x10, 100, 0, 0);
  one(s, AC_CMD_CHANNEL_TIMER, 0xFFFF0010, 0x1234, 0, 0);
  one(s, AC_CMD_CHANNEL_VOLUME, 0x20, 0x1234, 5, 0);
  trace_end(s);
  ACT_EQ(ntr, 7);
  EXPECT_W(0, CH(4) + 8, 2, 0x0800);
  EXPECT_W(1, CH(5) + 8, 2, 0x0800);
  EXPECT_W(2, CH(6) + 8, 2, 0x0800);
  EXPECT_W(3, CH(5), 2, 0x0132); /* volume | shift << 8, raw */
  EXPECT_W(4, CH(4) + 2, 1, 100);
  EXPECT_W(5, CH(4) + 8, 2, 0xEDCC); /* only the low 16 bits of the mask select channels */
  EXPECT_W(6, CH(5), 2, 0x1734);
}

ACT_TEST(p15_master_pan) {
  AcoreState *s = act_state();
  one(s, AC_CMD_SETUP_CHANNEL_PCM, 0xF000u << 16 | 4, 0x02230000, 127u << 24 | 100, 2u << 26 | 1u << 24 | 20u << 16);
  one(s, AC_CMD_SETUP_CHANNEL_PSG, 8, 5, 201 | 0x1000u << 8, 1);
  one(s, AC_CMD_CHANNEL_PAN, 0x40, 203, 0, 0);
  trace_begin(s);
  one(s, AC_CMD_MASTER_PAN, 30, 0, 0, 0);
  trace_end(s);
  ACT_EQ(ntr, 16); /* every channel's pan byte */
  for (int i = 0; i < 16; i++) EXPECT_W(i, CH(i) + 2, 1, 30);

  /* under a master pan the setup and CHANNEL_PAN write the master pan */
  trace_begin(s);
  one(s, AC_CMD_SETUP_CHANNEL_PCM, 0xF000u << 16 | 6, 0x02230000, 127u << 24 | 100, 2u << 26 | 1u << 24 | 50u << 16);
  one(s, AC_CMD_CHANNEL_PAN, 0x10, 90, 0, 0);
  trace_end(s);
  EXPECT_W(0, CH(6), 4, 0x301E007Fu);
  EXPECT_W(5, CH(4) + 2, 1, 30);

  /* a negative value resets: each channel's own pan (a setup's 7 bits, CHANNEL_PAN's raw byte) */
  trace_begin(s);
  one(s, AC_CMD_MASTER_PAN, 0xFFFFFFFFu, 0, 0, 0);
  trace_end(s);
  ACT_EQ(ntr, 16);
  for (int i = 0; i < 16; i++) {
    u32 want = i == 4 ? 90 : i == 6 ? 0xCB : i == 8 ? 0x49 : 0;
    if (i == 6) want = 50; /* the setup under the master pan still recorded its own pan */
    EXPECT_W(i, CH(i) + 2, 1, want);
  }
  /* a pan > 127 is written raw */
  trace_begin(s);
  one(s, AC_CMD_MASTER_PAN, 200, 0, 0, 0);
  trace_end(s);
  EXPECT_W(0, CH(0) + 2, 1, 200);
}

/* ---- master volume, output selector, surround decay ---- */
ACT_TEST(p15_master_volume_output) {
  AcoreState *s = act_state();
  trace_begin(s);
  one(s, AC_CMD_MASTER_VOLUME, 50, 0, 0, 0);
  one(s, AC_CMD_MASTER_VOLUME, 200, 0, 0, 0);
  one(s, AC_CMD_OUTPUT_SELECTOR, 0, 0, 0, 0);
  one(s, AC_CMD_OUTPUT_SELECTOR, 1, 2, 1, 1);
  one(s, AC_CMD_OUTPUT_SELECTOR, 3, 3, 0, 1);
  one(s, AC_CMD_OUTPUT_SELECTOR, 2, 1, 1, 0);
  trace_end(s);
  ACT_EQ(ntr, 6);
  EXPECT_W(0, BASE + 0x500, 1, 50);
  EXPECT_W(1, BASE + 0x500, 1, 200);
  EXPECT_W(2, BASE + 0x501, 1, 0x80);
  EXPECT_W(3, BASE + 0x501, 1, 0xB9);
  EXPECT_W(4, BASE + 0x501, 1, 0xAF);
  EXPECT_W(5, BASE + 0x501, 1, 0x96);
}

ACT_TEST(p15_surround_decay) {
  AcoreState *s = act_state();
  trace_begin(s);
  one(s, AC_CMD_SURROUND_DECAY, 100, 0, 0, 0);
  trace_end(s);
  ACT_EQ(ntr, 14); /* channels 0, 2, 4-15: the oracle's behaviour */
  EXPECT_W(0, CH(0), 1, 0);
  EXPECT_W(1, CH(2), 1, 0);
  EXPECT_W(2, CH(4), 1, 0);
  EXPECT_W(13, CH(15), 1, 0);
}

/* ---- captures ---- */
ACT_TEST(p15_capture) {
  AcoreState *s = act_state();
  trace_begin(s);
  one(s, AC_CMD_SETUP_CAPTURE, 0x02300000, 64, 0, 0);                       /* capture 0, PCM16, one-shot */
  one(s, AC_CMD_SETUP_CAPTURE, 0x02300040, 0x12345, 0xF8000000u, 0);        /* capture 1: add, source, loop, PCM8 */
  one(s, AC_CMD_SETUP_CAPTURE, 0x02300040, 0x12345, 0x78000000u, 0);
  trace_end(s);
  ACT_EQ(ntr, 9);
  EXPECT_W(0, BASE + 0x508, 1, 0x04);
  EXPECT_W(1, BASE + 0x510, 4, 0x02300000);
  EXPECT_W(2, BASE + 0x514, 2, 64);
  EXPECT_W(3, BASE + 0x509, 1, 0x0B);
  EXPECT_W(4, BASE + 0x518, 4, 0x02300040);
  EXPECT_W(5, BASE + 0x51C, 2, 0x2345);
  EXPECT_W(6, BASE + 0x508, 1, 0x0B);
}

ACT_TEST(p15_capture_start_stop) {
  AcoreState *s = act_state();
  one(s, AC_CMD_SETUP_CAPTURE, 0x02300000, 64, 0, 0);
  trace_begin(s);
  one(s, AC_CMD_START_TIMER, 0, 1, 0, 0);
  one(s, AC_CMD_START_TIMER, 0, 2, 0, 0);
  one(s, AC_CMD_STOP_TIMER, 0, 3, 0, 0);
  one(s, AC_CMD_START_TIMER, 0, 3, 0, 0);
  one(s, AC_CMD_STOP_TIMER, 0, 2, 0, 0);
  trace_end(s);
  ACT_EQ(ntr, 6);
  EXPECT_W(0, BASE + 0x508, 1, 0x84); /* capture 0 keeps its setup bits */
  EXPECT_W(1, BASE + 0x509, 1, 0x80);
  EXPECT_W(2, BASE + 0x508, 1, 0);
  EXPECT_W(3, BASE + 0x509, 1, 0);
  EXPECT_W(4, BASE + 0x508, 2, 0x8080); /* both at once: one 16-bit write */
  EXPECT_W(5, BASE + 0x509, 1, 0);
  /* capture 1 reads back without bit 0 */
  one(s, AC_CMD_SETUP_CAPTURE, 0x02300040, 4, 0x78000000u | 1u << 31, 0);
  trace_begin(s);
  one(s, AC_CMD_START_TIMER, 0, 2, 0, 0);
  trace_end(s);
  EXPECT_W(0, BASE + 0x509, 1, 0x8A);
}

/* ---- alarms ---- */
ACT_TEST(p15_alarm) {
  AcoreState *s = act_state();
  s->now = 1000;
  one(s, AC_CMD_SETUP_ALARM, 2, 100, 200, 5);
  const AC_Alarm *a = &s->work.alarm[2];
  ACT_EQ(a->enable, 0);
  ACT_EQ(a->id, 5);
  ACT_EQ(a->tick_lo, 100);
  ACT_EQ(a->period_lo, 200);
  s->now = 2000;
  one(s, AC_CMD_START_TIMER, 0, 0, 4, 0);
  ACT_EQ(a->enable, 1);
  ACT_EQ(a->id, 5);
  AC_AlarmFire f[8];
  int n = ac_p15_alarm_fires(s, 2000 + 64 * 100 + 64 * 200 * 2 + 5, f, 8);
  ACT_EQ(n, 3);
  ACT_EQ(f[0].time, 2000 + 64 * 100);
  ACT_EQ(f[1].time, 2000 + 64 * 100 + 64 * 200);
  ACT_EQ(f[2].time, 2000 + 64 * 100 + 64 * 400);
  ACT_EQ(f[2].no, 2);
  ACT_EQ(f[2].id, 5);
  s->now = 2000 + 64 * 100 + 64 * 250;
  one(s, AC_CMD_STOP_TIMER, 0, 0, 4, 0);
  ACT_EQ(a->enable, 0);
  ACT_EQ(a->id, 6); /* the stop moves the id on (oracle) */
  n = ac_p15_alarm_fires(s, 1ull << 40, f, 8);
  ACT_EQ(n, 2); /* the third was after the stop */
  /* a one-shot (period 0) fires once */
  s->now = 100000;
  one(s, AC_CMD_SETUP_ALARM, 2, 100, 0, 9);
  one(s, AC_CMD_START_TIMER, 0, 0, 4, 0);
  n = ac_p15_alarm_fires(s, 1ull << 40, f, 8);
  ACT_EQ(n, 3);
  ACT_EQ(f[2].time, 100000 + 64 * 100);
  ACT_EQ(f[2].id, 9);
}

/* ---- shared work, finishCommandTag ---- */
ACT_TEST(p15_finish_tag) {
  AcoreState *s = act_state();
  one(s, AC_CMD_SHARED_WORK, AC_PLAN_SHARED, 0, 0, 0);
  ACT_EQ(s->drv.shared, AC_PLAN_SHARED);
  ACT_EQ(ac_rd32(s, AC_PLAN_SHARED + AC_SW_FINISH_TAG), 1); /* the list that set it counts */
  /* two commands in one list: one tag; two lists: two */
  act_cmd(s, AC_CMD_MASTER_VOLUME, 127, 0, 0, 0);
  act_cmd(s, AC_CMD_MASTER_VOLUME, 127, 0, 0, 0);
  go(s);
  ACT_EQ(ac_rd32(s, AC_PLAN_SHARED + AC_SW_FINISH_TAG), 2);
  act_cmd(s, AC_CMD_MASTER_VOLUME, 127, 0, 0, 0);
  act_send(s);
  act_cmd(s, AC_CMD_MASTER_VOLUME, 127, 0, 0, 0);
  act_send(s);
  ac_cmd_proc(s);
  ACT_EQ(ac_rd32(s, AC_PLAN_SHARED + AC_SW_FINISH_TAG), 4);
  ACT_EQ(s->drv.nqueue, 0); /* the queue is consumed */
  ac_cmd_proc(s);
  ACT_EQ(ac_rd32(s, AC_PLAN_SHARED + AC_SW_FINISH_TAG), 4);
}

ACT_TEST(p15_lists_in_order) {
  AcoreState *s = act_state();
  trace_begin(s);
  act_cmd(s, AC_CMD_MASTER_VOLUME, 1, 0, 0, 0);
  act_cmd(s, AC_CMD_MASTER_VOLUME, 2, 0, 0, 0);
  act_send(s);
  act_cmd(s, AC_CMD_MASTER_VOLUME, 3, 0, 0, 0);
  act_send(s);
  ac_cmd_proc(s);
  trace_end(s);
  ACT_EQ(ntr, 3);
  EXPECT_W(0, BASE + 0x500, 1, 1);
  EXPECT_W(1, BASE + 0x500, 1, 2);
  EXPECT_W(2, BASE + 0x500, 1, 3);
}

/* ---- READ_DRIVER_INFO ---- */
ACT_TEST(p15_read_driver_info) {
  AcoreState *s = act_state();
  const u32 buf = AC_PLAN_DRVINFO;
  s->drv.work_addr = 0x03809FBCu;
  s->work.channel[4].myNo = 4;
  s->work.channel[4].timer = 0x1234;
  s->work.player[3].volume = 99;
  s->work.track[7].cur = 0x02100123;
  s->work.alarm[2].id = 5;
  one(s, AC_CMD_SETUP_CHANNEL_PCM, 0xF000u << 16 | 4, 0x02230000, 127u << 24 | 100, 2u << 26 | 1u << 24 | 20u << 16);
  one(s, AC_CMD_START_TIMER, 0x10, 0, 0, 0);
  one(s, AC_CMD_READ_DRIVER_INFO, buf, 0, 0, 0);
  ACT_EQ(AC_DI_CHCTRL, 0x1180);
  ACT_EQ(ac_rd8(s, buf + 4 * sizeof(AC_ExChannel)), 4);
  ACT_EQ(ac_rd16(s, buf + 4 * sizeof(AC_ExChannel) + offsetof(AC_ExChannel, timer)), 0x1234);
  ACT_EQ(ac_rd8(s, buf + offsetof(AC_Work, player) + 3 * sizeof(AC_Player) + offsetof(AC_Player, volume)), 99);
  ACT_EQ(ac_rd32(s, buf + offsetof(AC_Work, track) + 7 * sizeof(AC_Track) + offsetof(AC_Track, cur)), 0x02100123);
  ACT_EQ(ac_rd8(s, buf + offsetof(AC_Work, alarm) + 2 * sizeof(AC_Alarm) + offsetof(AC_Alarm, id)), 5);
  ACT_EQ(ac_rd32(s, buf + AC_DI_CHCTRL + 4 * 4), 0xB014007Fu); /* channel 4's SOUNDxCNT, busy bit and all */
  ACT_EQ(ac_rd32(s, buf + AC_DI_CHCTRL + 4 * 5), 0);
  ACT_EQ(ac_rd32(s, buf + AC_DI_WORKADDR), 0x03809FBCu);
}

ACT_TEST(p15_read_driver_info_locked) {
  AcoreState *s = act_state();
  one(s, AC_CMD_LOCK_CHANNEL, 0x20, 0, 0, 0);
  one(s, AC_CMD_READ_DRIVER_INFO, AC_PLAN_DRVINFO, 0, 0, 0);
  if (ac_exch_locked(s) == 0) ACT_TODO("piece 11: LOCK_CHANNEL (the locked mask)");
  ACT_EQ(ac_rd32(s, AC_PLAN_DRVINFO + AC_DI_LOCKED), 0x20);
}

/* ---- INVALIDATE_WAVE: the channels playing from the range stop at once ---- */
ACT_TEST(p15_invalidate_wave) {
  AcoreState *s = act_state();
  s->work.channel[2].flags = AC_EXF_ACTIVE;
  s->work.channel[2].data = 0x02200100;
  s->work.channel[5].flags = AC_EXF_ACTIVE;
  s->work.channel[5].data = 0x02300000;
  act_reg(s, CH(2), 0xA840016Du, 4);
  act_reg(s, CH(5), 0xA840016Du, 4);
  trace_begin(s);
  one(s, AC_CMD_INVALIDATE_WAVE, 0x02200000, 0x02210000, 0, 0);
  trace_end(s);
  ACT_EQ(ntr, 1);
  EXPECT_W(0, CH(2), 4, 0x2840016Du);
  ACT_EQ(ac_hw_read(s, CH(5), 4), 0xA840016Du);
}

/* ---- the commands that other pieces carry out ---- */
ACT_TEST(p15_player_commands) { ACT_TODO("piece 9: START/STOP/PREPARE/START_PREPARED/PAUSE/SKIP, PLAYER_PARAM, TRACK_PARAM, MUTE, ALLOCATABLE (stubs)"); }
ACT_TEST(p15_variables) { ACT_TODO("piece 16: PLAYER_LOCAL_VAR / GLOBAL_VAR through ac_var_set (stub)"); }
ACT_TEST(p15_lock_commands) { ACT_TODO("piece 11: LOCK / UNLOCK / STOP_UNLOCKED_CHANNEL (stubs)"); }
ACT_TEST(p15_invalidate_players) { ACT_TODO("piece 9: INVALIDATE_SEQ / BANK stop a player (needs a started player)"); }

/* ---- the wave-out sequence of pm_voice.c / waveout.c: lock, setup PCM, timer, volume, pan, start, stop ---- */
ACT_TEST(p15_waveout_sequence) {
  AcoreState *s = act_state();
  u8 wave[512];
  for (int i = 0; i < 512; i++) wave[i] = (u8)i;
  act_mem(s, AC_PLAN_WAVE_BASE, wave, sizeof wave);
  const int ch = 4;
  act_cmd(s, AC_CMD_MASTER_VOLUME, 127, 0, 0, 0);
  act_cmd(s, AC_CMD_LOCK_CHANNEL, 1u << ch, 0, 0, 0);
  act_cmd(s, AC_CMD_SETUP_CHANNEL_PCM, 0xFA00u << 16 | ch, AC_PLAN_WAVE_BASE, 127u << 24 | 128, 2u << 26 | 1u << 24 | 64u << 16);
  act_cmd(s, AC_CMD_START_TIMER, 1u << ch, 0, 0, 0);
  trace_begin(s);
  go(s);
  ACT_EQ(s->drv.nqueue, 0);
  act_run(s, 6 * AC_CYCLES_PER_FRAME);
  act_cmd(s, AC_CMD_CHANNEL_TIMER, 1u << ch, 0xF800, 0, 0);
  act_cmd(s, AC_CMD_CHANNEL_VOLUME, 1u << ch, 80, 0, 0);
  act_cmd(s, AC_CMD_CHANNEL_PAN, 1u << ch, 32, 0, 0);
  go(s);
  act_cmd(s, AC_CMD_STOP_TIMER, 1u << ch, 0, 0, 0);
  go(s);
  act_run(s, 6 * AC_CYCLES_PER_FRAME);
  trace_end(s);
  act_emit(s, "p15_waveout");
  /* LOCK_CHANNEL (11) may write its own stop first: start at the setup's CNT, just before the first TMR write */
  int k = 0;
  while (k < ntr && tr[k].addr != CH(ch) + 8) k++;
  ACT_CHECK(k > 0 && k < ntr);
  k--;
  EXPECT_W(k + 0, CH(ch), 4, 0x3040007Fu);
  EXPECT_W(k + 1, CH(ch) + 8, 2, 0x0600);
  EXPECT_W(k + 2, CH(ch) + 0xA, 2, 0);
  EXPECT_W(k + 3, CH(ch) + 0xC, 4, 128);
  EXPECT_W(k + 4, CH(ch) + 0x4, 4, AC_PLAN_WAVE_BASE);
  EXPECT_W(k + 5, CH(ch) + 3, 1, 0xB0);
  EXPECT_W(k + 6, CH(ch) + 8, 2, 0x0800);
  EXPECT_W(k + 7, CH(ch), 2, 0x0050);
  EXPECT_W(k + 8, CH(ch) + 2, 1, 32);
  EXPECT_W(k + 9, CH(ch), 4, 0x30200050u); /* the stop: the readback with the start bit cleared */
  ACT_EQ(ntr, k + 10);
}
