/* port/acore piece 10: Banks and note-on (docs/acore.md). OWNED BY PIECE 10.
 *
 * ac_note_on: the instrument of the track's program from the player's SBNK, its wave from the bank's wave archive
 * links, a channel from piece 11, the channel's fields filled in, its envelope set (track overrides first), start.
 * Formats: GBATEK 66 (SBNK), 68 (SWAV), kiwi. The sub-record's own type is the u16 before its 10 bytes in a drum
 * set / key split (GBATEK calls it "unknown, usually 1").
 */
#include "ac_shared.h"
#include <string.h>

/* one wave/note definition, resolved */
typedef struct BankInst {
  int type;      /* AC_INST_PCM, PSG, NOISE (the others do not make a note) */
  u32 swav;      /* wave number, or the PSG duty */
  u32 swar;      /* wave archive number (0-3) */
  int key;       /* the key the wave sounds at its own pitch */
  int a, d, sus, r;
  int pan;       /* 0-127, 64 centre */
} BankInst;

static int read_def(const AcoreState *s, u32 a, int type, BankInst *bi) {
  bi->type = type;
  bi->swav = ac_rd16(s, a);
  bi->swar = ac_rd16(s, a + 2);
  bi->key = ac_rd8(s, a + 4);
  bi->a = ac_rd8(s, a + 5);
  bi->d = ac_rd8(s, a + 6);
  bi->sus = ac_rd8(s, a + 7);
  bi->r = ac_rd8(s, a + 8);
  bi->pan = ac_rd8(s, a + 9);
  return 1;
}

/* the instrument for program prg at key; 0 when the program or key has none */
static int find_inst(const AcoreState *s, u32 bank, unsigned prg, int key, BankInst *bi) {
  if (!bank) return 0;
  if (prg >= ac_rd32(s, bank + AC_SBNK_INST_COUNT)) return 0;
  u32 rec = ac_rd32(s, bank + AC_SBNK_INST_RECORDS + 4 * prg);
  int type = (int)(rec & 0xFF);
  u32 a = bank + (rec >> 8);
  switch (type) {
  case AC_INST_PCM:
  case AC_INST_PSG:
  case AC_INST_NOISE:
    return read_def(s, a, type, bi);
  case AC_INST_DRUM_SET: {
    int lo = ac_rd8(s, a), hi = ac_rd8(s, a + 1);
    if (key < lo || key > hi) return 0;
    u32 e = a + 2 + 12u * (u32)(key - lo);
    int st = ac_rd16(s, e);
    if (st < AC_INST_PCM || st > AC_INST_NOISE) return 0;
    return read_def(s, e + 2, st, bi);
  }
  case AC_INST_KEY_SPLIT: {
    int i;
    for (i = 0; i < 8; i++)
      if (key <= ac_rd8(s, a + (u32)i)) break;
    if (i == 8) return 0;
    u32 e = a + 8 + 12u * (u32)i;
    int st = ac_rd16(s, e);
    if (st < AC_INST_PCM || st > AC_INST_NOISE) return 0;
    return read_def(s, e + 2, st, bi);
  }
  case AC_INST_DIRECTPCM: /* a channel is taken, then the note fails (oracle: CNT=0 write, no start) */
    return read_def(s, a, type, bi);
  default: /* 0 empty, 5 null: nothing is allocated (oracle: no register write) */
    return 0;
  }
}

/* the wave: the SWAV header parsed and its samples' DS address; 0 when the wave archive or wave is missing */
static int find_wave(const AcoreState *s, u32 bank, const BankInst *bi, AC_WaveParam *w, u32 *data) {
  if (bi->swar > 3) return 0;
  u32 swar = ac_rd32(s, bank + AC_SBNK_WAVEARC_LINK(bi->swar));
  if (!swar) return 0;
  if (bi->swav >= ac_rd32(s, swar + AC_SWAR_WAVE_COUNT)) return 0;
  u32 off = ac_rd32(s, swar + AC_SWAR_WAVE_OFFSETS + 4 * bi->swav);
  u32 h = off >= AC_MAIN_BASE ? off : swar + off;
  w->format = ac_rd8(s, h);
  w->loopflag = ac_rd8(s, h + 1);
  w->rate = ac_rd16(s, h + 2);
  w->timer = ac_rd16(s, h + 4);
  w->loopstart = ac_rd16(s, h + 6);
  w->looplen = ac_rd32(s, h + 8);
  if (w->format > 2) return 0;
  *data = h + 12;
  return 1;
}

int ac_note_on(AcoreState *s, int p, int t, int key, int velocity, s32 length) {
  if (p < 0 || p >= AC_NPLAYER || t < 0 || t >= AC_NTRACK || key < 0 || key > 127) return 0;
  AC_Player *pl = &s->work.player[p];
  AC_Track *tr = &s->work.track[t];
  BankInst bi;
  key += tr->transpose; /* the transpose applies at note-on (oracle) */
  if (key < 0) key = 0;
  if (key > 127) key = 127;
  if (!find_inst(s, pl->bank, tr->prgNo, key, &bi)) return 0;

  AC_WaveParam wave;
  u32 data = 0;
  u32 kind = bi.type == AC_INST_PSG ? 0x3F00 : bi.type == AC_INST_NOISE ? 0xC000 : 0xFFFF;
  int extype = bi.type == AC_INST_PSG ? AC_EX_PSG : bi.type == AC_INST_NOISE ? AC_EX_NOISE : AC_EX_PCM;
  memset(&wave, 0, sizeof wave);

  /* the channel is taken before the wave is looked up: a note whose wave is missing (or a direct PCM one) leaves
     the channel's stop write in the oracle's trace (CNT = 0, nothing else) */
  int prio = pl->prio + tr->prio;
  if (prio > 255) prio = 255;
  int ch = ac_exch_alloc(s, tr->channel_mask & kind, prio, 0, t);
  if (ch < 0 || ch >= AC_NCH) return 0;
  AC_ExChannel *c = &s->work.channel[ch];
  if (bi.type == AC_INST_PSG || bi.type == AC_INST_NOISE) {
    data = bi.swav; /* the duty */
    /* the base timer of a PSG / noise note (key = base key 60 runs the 8-step cycle at middle C: 8006): oracle, the
       battle clip's noise note key 78 -> TMR 0xF4F7 */
    wave.timer = 0x1F46;
  }
  else if (bi.type == AC_INST_PCM) {
    if (!find_wave(s, pl->bank, &bi, &wave, &data)) {
      ac_exch_free(s, ch);
      return 0;
    }
  } else if (bi.type == AC_INST_DIRECTPCM) {
    ac_exch_free(s, ch);
    return 0;
  }

  c->type = (u8)extype;
  c->wave = wave;
  c->data = data;
  c->key = (u8)key;
  c->original_key = (u8)bi.key;
  c->velocity = (u8)velocity;
  c->init_pan = (s8)(bi.pan - 64);
  c->prio = (u8)prio;

  /* the envelope: the track's value when it set one (0xFF = unset), else the instrument's */
  ac_env_init(s, ch);
  ac_env_set_attack(s, ch, tr->attack != 0xFF ? tr->attack : bi.a);
  ac_env_set_decay(s, ch, tr->decay != 0xFF ? tr->decay : bi.d);
  ac_env_set_sustain(s, ch, tr->sustain != 0xFF ? tr->sustain : bi.sus);
  ac_env_set_release(s, ch, tr->release != 0xFF ? tr->release : bi.r);

  ac_exch_start(s, ch, length);
  ac_porta_setup(s, ch, tr, key, length);
  return 1;
}
