/* port/acore piece 8: Track parameters (docs/acore.md). OWNED BY PIECE 8.
 *
 * ac_track_param is the sequence commands that set a track/player parameter (the value the sequence read, already
 * resolved: variable, random or immediate); ac_track_update_channels runs every tick and pushes the track's mixed
 * values into each of its channels' user fields.
 *
 * Conventions (GBATEK 64, kiwi): pan 0xC0 stores value - 64; transpose and pitch bend are signed bytes; the
 * volume of a channel is a sum of decibel*10 values (ac_decibel, piece 6), so volume 127 adds 0 and 0 adds the
 * table's minimum. The track fader and the player fader (dB*10, set from the ARM9) add on top.
 */
#include "ac_shared.h"
#include <string.h>

static AC_Track *track_of(AcoreState *s, int t) { return (t >= 0 && t < AC_NTRACK) ? &s->work.track[t] : NULL; }

void ac_track_init(AcoreState *s, int t) {
  AC_Track *k = track_of(s, t);
  if (!k) return;
  memset(k, 0, sizeof *k);
  k->pan_range = 127;
  k->volume = 127;
  k->volume2 = 127;
  k->bend_range = 2;
  k->attack = k->decay = k->sustain = k->release = 255; /* 255 = the instrument's own value */
  k->prio = 64;
  k->porta_key = 60;
  k->mod.speed = 16;
  k->mod.range = 1;
  k->channel_mask = 0xFFFF;
  k->flags = AC_TRF_NOTE_WAIT | AC_TRF_CMP; /* the oracle's fresh track: 0x43 with ACTIVE (set by piece 9) */
}

void ac_track_param(AcoreState *s, int p, int t, int cmd, s32 value) {
  AC_Track *k = track_of(s, t);
  if (!k) return;
  AC_Player *pl = (p >= 0 && p < AC_NPLAYER) ? &s->work.player[p] : NULL;
#define FLAG(bit, on) (k->flags = (u8)((on) ? (k->flags | (bit)) : (k->flags & ~(bit))))
  switch (cmd) {
  case 0xC0: k->pan = (s8)(value - 64); break;
  case 0xC1: k->volume = (u8)value; break;
  case 0xC2: if (pl) pl->volume = (u8)value; break;
  case 0xC3: k->transpose = (s8)value; break;
  case 0xC4: k->pitch_bend = (s8)value; break;
  case 0xC5: k->bend_range = (u8)value; break;
  case 0xC6: k->prio = (u8)value; break;
  case 0xC7: FLAG(AC_TRF_NOTE_WAIT, value != 0); break; /* mono (1) / poly (0): one note at a time = note wait */
  case 0xC8: FLAG(AC_TRF_TIE, value != 0); break;
  case 0xC9: k->porta_key = (u8)(value + k->transpose); FLAG(AC_TRF_PORTA, 1); break;
  case 0xCE: FLAG(AC_TRF_PORTA, value != 0); break;
  case 0xCF: k->porta_time = (u8)value; break;
  case 0xCA: k->mod.depth = (u8)value; break;
  case 0xCB: k->mod.speed = (u8)value; break;
  case 0xCC: k->mod.target = (u8)value; break;
  case 0xCD: k->mod.range = (u8)value; break;
  case 0xE0: k->mod.delay = (u16)value; break;
  case 0xD0: k->attack = (u8)value; break;
  case 0xD1: k->decay = (u8)value; break;
  case 0xD2: k->sustain = (u8)value; break;
  case 0xD3: k->release = (u8)value; break;
  case 0xD5: k->volume2 = (u8)value; break;
  case 0xE1: if (pl) pl->tempo = (u16)value; break;
  case 0xE3: k->sweep_pitch = (s16)value; break;
  case 0xD6: break; /* print: ignored */
  default: break;
  }
#undef FLAG
}

/* the player, track and expression volumes follow the square law too, as the velocity does (oracle, the intro clip:
 * track volume 95 counted twice, -25 -> -50 dB*10); 0 stays the table's minimum */
static int vol_db(int x) { return x == 0 ? ac_decibel(0) : ac_decibel_sq(x); }

static int clamp_s8(int v) { return v < -128 ? -128 : v > 127 ? 127 : v; }

/* The mixed values of track t of player p: volume (dB*10, user_decay), fader sum (user_decay2), pan, pitch (1/64 semitones). Exposed for the tests. */
void ac_p08_mix(AcoreState *s, int p, int t, int *vol, int *fader, int *pan, int *pitch) {
  const AC_Track *k = &s->work.track[t];
  const AC_Player *pl = &s->work.player[p & (AC_NPLAYER - 1)];
  int v = vol_db(pl->volume & 0x7F) + vol_db(k->volume & 0x7F) + vol_db(k->volume2 & 0x7F);
  if (v < -32768) v = -32768;
  if (v > 32767) v = 32767;
  int f = pl->extFader + k->extFader; /* user_decay2: both faders (oracle: track -10 + player -50 = -60) */
  if (f < -32768) f = -32768;
  if (f > 32767) f = 32767;
  *fader = f;
  int pn = k->pan;
  if (k->pan_range != 127) pn = (pn * k->pan_range) >> 7;
  pn += k->ext_pan;
  /* bend (signed 7 bit) x range (semitones) in 1/64 semitones: bend/128 of a range */
  int pt = (k->pitch_bend * k->bend_range) << 6 >> 7;
  pt += k->ext_pitch;
  *vol = v;
  *pan = clamp_s8(pn);
  *pitch = pt < -32768 ? -32768 : pt > 32767 ? 32767 : pt;
}

void ac_track_update_channels(AcoreState *s, int p, int t, int release) {
  AC_Track *k = track_of(s, t);
  if (!k) return;
  int vol, fader, pan, pitch;
  ac_p08_mix(s, p, t, &vol, &fader, &pan, &pitch);
  (void)release; /* the release request is piece 11's (ac_track_release_channels) */
  u32 a = k->channel_list;
  for (int guard = 0; a && guard < AC_NCH; guard++) {
    int n = AC_EXCH_NO(s, a);
    if (n < 0 || n >= AC_NCH) break;
    AC_ExChannel *c = &s->work.channel[n];
    /* a note in release no longer follows the track (oracle, the title clip: a track volume change left the released
       notes' volume alone) */
    if (c->env_status == AC_ENV_RELEASE) {
      c->user_decay2 = (s16)fader; /* the faders still reach it (oracle, the title clip's fade-out) */
      a = c->nextLink;
      continue;
    }
    c->user_decay = (s16)vol;
    c->user_decay2 = (s16)fader;
    c->user_pitch = (s16)pitch;
    c->user_pan = (s8)pan;
    c->pan_range = k->pan_range;
    c->lfo.param.target = k->mod.target;
    c->lfo.param.speed = k->mod.speed;
    c->lfo.param.depth = k->mod.depth;
    c->lfo.param.range = k->mod.range;
    c->lfo.param.delay = k->mod.delay;
    a = c->nextLink;
  }
}
