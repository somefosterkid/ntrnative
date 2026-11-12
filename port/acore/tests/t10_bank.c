/* port/acore piece 10: unit tests (docs/acore.md, piece 10's "Tests:"). OWNED BY PIECE 10.
 * Banks and wave archives are built by hand here (little-endian bytes in main memory). The channel the note landed on
 * is found by its unique velocity, so the tests hold with piece 11's stub (channel 0) and with its real allocator. */
#include "act.h"
#include <string.h>

#define BANK 0x02100000u
#define SWAR 0x02101000u
#define SWAR2 0x02102000u

static void w16(u8 *b, u32 o, u32 v) { b[o] = (u8)v; b[o + 1] = (u8)(v >> 8); }
static void w32(u8 *b, u32 o, u32 v) { w16(b, o, v & 0xFFFF); w16(b, o + 2, v >> 16); }

/* a 10-byte definition: swav, swar, key, A, D, S, R, pan */
static void def(u8 *b, u32 o, u32 swav, u32 swar, int key, int a, int d, int sus, int r, int pan) {
  w16(b, o, swav); w16(b, o + 2, swar);
  b[o + 4] = (u8)key; b[o + 5] = (u8)a; b[o + 6] = (u8)d; b[o + 7] = (u8)sus; b[o + 8] = (u8)r; b[o + 9] = (u8)pan;
}

/* a SWAR of n waves: wave i = PCM16 (format 1, loop, rate 32000, timer 0xFB0A + i, loop start i, length 16 + i) */
static void make_swar(AcoreState *s, u32 addr, int n) {
  static u8 b[0x400];
  memset(b, 0, sizeof b);
  memcpy(b, "SWAR", 4);
  w32(b, AC_SWAR_WAVE_COUNT, (u32)n);
  for (int i = 0; i < n; i++) {
    u32 h = 0x100u + 0x40u * (u32)i;
    w32(b, AC_SWAR_WAVE_OFFSETS + 4u * (u32)i, h);
    b[h] = 1; b[h + 1] = 1; w16(b, h + 2, 32000); w16(b, h + 4, 0xFB0A + (u32)i);
    w16(b, h + 6, (u32)i); w32(b, h + 8, 16u + (u32)i);
  }
  act_mem(s, addr, b, sizeof b);
}

static u8 bankbuf[0x400];
/* the bank header with the links to the two SWARs; records are added by the caller at 0x100+ */
static void bank_begin(int ninst) {
  memset(bankbuf, 0, sizeof bankbuf);
  memcpy(bankbuf, "SBNK", 4);
  w32(bankbuf, AC_SBNK_WAVEARC_LINK(0), SWAR);
  w32(bankbuf, AC_SBNK_WAVEARC_LINK(1), SWAR2);
  w32(bankbuf, AC_SBNK_INST_COUNT, (u32)ninst);
}
static void bank_rec(int prg, int type, u32 off) { w32(bankbuf, AC_SBNK_INST_RECORDS + 4u * (u32)prg, (u32)type | off << 8); }
static void bank_end(AcoreState *s) { act_mem(s, BANK, bankbuf, sizeof bankbuf); }

static AcoreState *setup(int prg) {
  AcoreState *s = act_state();
  make_swar(s, SWAR, 3);
  s->work.player[0].bank = BANK;
  s->work.player[0].prio = 10;
  AC_Track *t = &s->work.track[0];
  t->prgNo = (u16)prg;
  t->channel_mask = 0xFFFF;
  t->attack = t->decay = t->sustain = t->release = 0xFF;
  t->prio = 5;
  return s;
}

static AC_ExChannel *find(AcoreState *s, int vel) {
  for (int i = 0; i < AC_NCH; i++)
    if (s->work.channel[i].velocity == vel) return &s->work.channel[i];
  return NULL;
}

ACT_TEST(p10_pcm_instrument) {
  bank_begin(2);
  bank_rec(1, AC_INST_PCM, 0x100);
  def(bankbuf, 0x100, 2, 0, 62, 20, 30, 40, 50, 100);
  AcoreState *s = setup(1);
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 66, 93, 48), 1);
  AC_ExChannel *c = find(s, 93);
  ACT_CHECK(c != NULL);
  ACT_EQ(c->type, AC_EX_PCM);
  ACT_EQ(c->key, 66);
  ACT_EQ(c->original_key, 62);
  ACT_EQ(c->init_pan, 100 - 64);
  ACT_EQ(c->wave.format, 1);
  ACT_EQ(c->wave.loopflag, 1);
  ACT_EQ(c->wave.rate, 32000);
  ACT_EQ(c->wave.timer, 0xFB0A + 2);
  ACT_EQ(c->wave.loopstart, 2);
  ACT_EQ(c->wave.looplen, 18);
  ACT_EQ(c->data, SWAR + 0x100 + 0x80 + 12);
}

ACT_TEST(p10_empty_and_out_of_range_program) {
  bank_begin(2);
  bank_rec(1, AC_INST_PCM, 0x100);
  def(bankbuf, 0x100, 0, 0, 60, 1, 2, 3, 4, 64);
  AcoreState *s = setup(0); /* record 0 is empty */
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 60, 93, 48), 0);
  s->work.track[0].prgNo = 5; /* past the count */
  ACT_EQ(ac_note_on(s, 0, 0, 60, 93, 48), 0);
  s->work.player[0].bank = 0; /* no bank */
  s->work.track[0].prgNo = 1;
  ACT_EQ(ac_note_on(s, 0, 0, 60, 93, 48), 0);
}

ACT_TEST(p10_psg_duty_from_record) {
  bank_begin(4);
  bank_rec(2, AC_INST_PSG, 0x100);
  def(bankbuf, 0x100, 5, 0, 60, 127, 127, 127, 127, 64); /* duty 5 */
  AcoreState *s = setup(2);
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 64, 91, 24), 1);
  AC_ExChannel *c = find(s, 91);
  ACT_CHECK(c != NULL);
  ACT_EQ(c->type, AC_EX_PSG);
  ACT_EQ(c->data, 5);
  ACT_EQ(c->original_key, 60);
  ACT_EQ(c->init_pan, 0);
  /* a different record, a different duty */
  def(bankbuf, 0x100, 2, 0, 60, 127, 127, 127, 127, 64);
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 64, 92, 24), 1);
  c = find(s, 92);
  ACT_CHECK(c != NULL);
  ACT_EQ(c->data, 2);
}

ACT_TEST(p10_noise_instrument) {
  bank_begin(4);
  bank_rec(3, AC_INST_NOISE, 0x100);
  def(bankbuf, 0x100, 0, 0, 60, 127, 40, 50, 60, 20);
  AcoreState *s = setup(3);
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 60, 90, 24), 1);
  AC_ExChannel *c = find(s, 90);
  ACT_CHECK(c != NULL);
  ACT_EQ(c->type, AC_EX_NOISE);
  ACT_EQ(c->init_pan, 20 - 64);
}

ACT_TEST(p10_direct_and_null_make_no_note) {
  bank_begin(6);
  bank_rec(4, AC_INST_DIRECTPCM, 0x100);
  bank_rec(5, AC_INST_NULL, 0x110);
  def(bankbuf, 0x100, 0, 0, 60, 1, 2, 3, 4, 64);
  def(bankbuf, 0x110, 0, 0, 60, 1, 2, 3, 4, 64);
  AcoreState *s = setup(5);
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 60, 89, 24), 0);
  s->work.track[0].prgNo = 4;
  ACT_EQ(ac_note_on(s, 0, 0, 60, 89, 24), 0);
  ACT_CHECK(find(s, 89) == NULL);
}

ACT_TEST(p10_drum_set_range) {
  bank_begin(3);
  bank_rec(2, AC_INST_DRUM_SET, 0x100);
  bankbuf[0x100] = 36; bankbuf[0x101] = 38; /* keys 36..38 */
  for (int i = 0; i < 3; i++) {
    w16(bankbuf, 0x102 + 12u * (u32)i, 1);
    def(bankbuf, 0x104 + 12u * (u32)i, (u32)i, 0, 60 + i, 10 + i, 20, 30, 40, 64 + i);
  }
  AcoreState *s = setup(2);
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 35, 88, 24), 0);
  ACT_EQ(ac_note_on(s, 0, 0, 39, 88, 24), 0);
  ACT_EQ(ac_note_on(s, 0, 0, 36, 81, 24), 1);
  AC_ExChannel *c = find(s, 81);
  ACT_CHECK(c != NULL);
  ACT_EQ(c->original_key, 60);
  ACT_EQ(c->wave.looplen, 16);
  ACT_EQ(ac_note_on(s, 0, 0, 38, 82, 24), 1);
  c = find(s, 82);
  ACT_CHECK(c != NULL);
  ACT_EQ(c->original_key, 62);
  ACT_EQ(c->wave.looplen, 18);
  ACT_EQ(c->init_pan, 2);
}

ACT_TEST(p10_key_split_regions) {
  bank_begin(3);
  bank_rec(2, AC_INST_KEY_SPLIT, 0x100);
  static const u8 ends[8] = {25, 35, 127, 0, 0, 0, 0, 0};
  memcpy(bankbuf + 0x100, ends, 8);
  for (int i = 0; i < 3; i++) {
    w16(bankbuf, 0x108 + 12u * (u32)i, 1);
    def(bankbuf, 0x10A + 12u * (u32)i, (u32)i, 0, 50 + i, 10, 20, 30, 40, 64);
  }
  AcoreState *s = setup(2);
  bank_end(s);
  static const struct { int key, region; } cases[] = {{0, 0}, {25, 0}, {26, 1}, {35, 1}, {36, 2}, {127, 2}};
  for (int i = 0; i < 6; i++) {
    int vel = 70 + i;
    ACT_EQ(ac_note_on(s, 0, 0, cases[i].key, vel, 24), 1);
    AC_ExChannel *c = find(s, vel);
    ACT_CHECK(c != NULL);
    ACT_EQ(c->original_key, 50 + cases[i].region);
    ACT_EQ(c->wave.looplen, 16 + cases[i].region);
  }
}

ACT_TEST(p10_key_split_past_last_region) {
  bank_begin(3);
  bank_rec(2, AC_INST_KEY_SPLIT, 0x100);
  static const u8 ends[8] = {25, 35, 0, 0, 0, 0, 0, 0}; /* nothing above 35 */
  memcpy(bankbuf + 0x100, ends, 8);
  for (int i = 0; i < 2; i++) {
    w16(bankbuf, 0x108 + 12u * (u32)i, 1);
    def(bankbuf, 0x10A + 12u * (u32)i, (u32)i, 0, 50, 10, 20, 30, 40, 64);
  }
  AcoreState *s = setup(2);
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 35, 69, 24), 1);
  ACT_EQ(ac_note_on(s, 0, 0, 36, 68, 24), 0);
  ACT_CHECK(find(s, 68) == NULL);
}

ACT_TEST(p10_missing_wave_archive) {
  bank_begin(2);
  bank_rec(1, AC_INST_PCM, 0x100);
  def(bankbuf, 0x100, 0, 3, 60, 1, 2, 3, 4, 64); /* archive link 3 is empty */
  AcoreState *s = setup(1);
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 60, 87, 24), 0);
  ACT_CHECK(find(s, 87) == NULL);
  def(bankbuf, 0x100, 0, 1, 60, 1, 2, 3, 4, 64); /* link 1 names a SWAR that was never loaded: still a wave */
  bank_end(s);
  make_swar(s, SWAR2, 0); /* zero waves: wave 0 is out of range */
  ACT_EQ(ac_note_on(s, 0, 0, 60, 87, 24), 0);
  def(bankbuf, 0x100, 7, 0, 60, 1, 2, 3, 4, 64); /* wave 7 of 3 */
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 60, 87, 24), 0);
}

ACT_TEST(p10_priority_and_fields) {
  bank_begin(2);
  bank_rec(1, AC_INST_PCM, 0x100);
  def(bankbuf, 0x100, 0, 0, 60, 20, 30, 40, 50, 64);
  AcoreState *s = setup(1);
  bank_end(s);
  ACT_EQ(ac_note_on(s, 0, 0, 60, 86, 24), 1);
  AC_ExChannel *c = find(s, 86);
  ACT_CHECK(c != NULL);
  ACT_EQ(c->prio, 15); /* player 10 + track 5 */
  ACT_EQ(c->init_pan, 0);
  ACT_EQ(c->data, SWAR + 0x100 + 12);
}
