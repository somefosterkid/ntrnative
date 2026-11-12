/* port/acore piece 12: Envelopes (docs/acore.md). OWNED BY PIECE 12.
 *
 * The amplitude envelope of a sequence note, one step per ac_env_update (the caller, piece 14, runs it on the
 * periodic wakes). The state lives in the work's AC_ExChannel (env_status, env_decay, attack, sustain, decay,
 * release): env_decay is the amplitude in 1/128 of dB*10, 0 = full, AC_ENV_MIN = -723 * 128 = silence (kiwi 4.2,
 * GBATEK 66: 'the SEQ Player treats 0 as 100% and -92544 as 0%').
 *
 *   attack   env = env * attack / 256 (C division: towards 0); env == 0 -> decay
 *   decay    env -= decay; env <= sustain level (ac_decibel(sustain) << 7) -> env = level, sustain
 *   sustain  nothing
 *   release  env -= release (floored at AC_ENV_MIN)
 *
 * The 0-127 values of the SBNK / sequence commands become: attack >= 109: a 19-entry table (index 127 - v), else
 * 255 - v; decay and release: 127 -> 0xFFFF, 126 -> 0x3C00, < 50 -> 2v + 1, else 0x1E00 / (126 - v); sustain stays
 * the 0-127 value (its level is the dB table's, piece 6). Those three are not in GBATEK or kiwi: fitted on the
 * oracle (every rate class 0..127 end, 49/50, 120, 126; attack 0..127), the register trace matching exactly.
 */
#include "ac_shared.h"

#define AC_ENV_MIN (-723 * 128)

static const u8 attack_table[19] = {0x00, 0x01, 0x05, 0x0E, 0x1A, 0x26, 0x33, 0x3F, 0x49, 0x54,
                                    0x5C, 0x64, 0x6D, 0x74, 0x7B, 0x7F, 0x84, 0x89, 0x8F};

static AC_ExChannel *ec(AcoreState *s, int ch) { return &s->work.channel[ch & (AC_NCH - 1)]; }

static int clamp127(int v) { return v < 0 ? 0 : v > 127 ? 127 : v; }

static u16 rate_of(int v) {
  v = clamp127(v);
  if (v == 127) return 0xFFFF;
  if (v == 126) return 0x3C00;
  if (v < 50) return (u16)(v * 2 + 1);
  return (u16)(0x1E00 / (126 - v));
}

void ac_env_init(AcoreState *s, int ch) {
  AC_ExChannel *c = ec(s, ch);
  c->env_status = AC_ENV_ATTACK;
  c->env_decay = AC_ENV_MIN;
}

void ac_env_set_attack(AcoreState *s, int ch, int v) {
  v = clamp127(v);
  ec(s, ch)->attack = v >= 109 ? attack_table[127 - v] : (u8)(255 - v);
}

void ac_env_set_decay(AcoreState *s, int ch, int v) { ec(s, ch)->decay = rate_of(v); }

void ac_env_set_sustain(AcoreState *s, int ch, int v) { ec(s, ch)->sustain = (u8)clamp127(v); }

void ac_env_set_release(AcoreState *s, int ch, int v) { ec(s, ch)->release = rate_of(v); }

void ac_env_update(AcoreState *s, int ch) {
  AC_ExChannel *c = ec(s, ch);
  switch (c->env_status) {
  case AC_ENV_ATTACK:
    c->env_decay = c->env_decay * (s32)c->attack / 256;
    if (c->env_decay == 0) c->env_status = AC_ENV_DECAY;
    break;
  case AC_ENV_DECAY: {
    s32 sus = (s32)ac_decibel_sq(c->sustain) * 128; /* the sustain level follows the square law too (oracle) */
    c->env_decay -= c->decay;
    if (c->env_decay <= sus) {
      c->env_decay = sus;
      c->env_status = AC_ENV_SUSTAIN;
    }
    break;
  }
  case AC_ENV_RELEASE:
    c->env_decay -= c->release;
    if (c->env_decay < AC_ENV_MIN) c->env_decay = AC_ENV_MIN;
    break;
  default: break;
  }
}

/* the envelope's amplitude in dB*10 (-723 = silent .. 0): env_decay >> 7, what the volume sum takes */
s32 ac_env_level(const AcoreState *s, int ch) {
  return s->work.channel[ch & (AC_NCH - 1)].env_decay >> 7;
}
