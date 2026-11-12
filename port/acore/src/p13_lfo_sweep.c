/* port/acore piece 13: LFO, sweep and portamento (docs/acore.md). OWNED BY PIECE 13.
 *
 * Everything here was fitted against the oracle (the ROM's own driver, tools in <scratch> and the notes in
 * p13_lfo_sweep.h) with sequences that set the modulation / portamento commands and read the SOUNDxCNT / TMR writes:
 *
 *  LFO    per periodic step: while delay_counter < delay it counts up (value stays 0); then counter += speed * 64
 *         (mod 128 << 8, the sine index is counter >> 8, a full period of 128). It runs even while depth is 0.
 *         value = sine(idx) * depth * range: pitch (target 0) = value * 64 >> 14 (1/64 semitones), volume and pan
 *         (targets 1 and 2) = value >> 8 (dB*10 and pan units); arithmetic shifts (pan -64 seen at the sine's -127).
 *  sweep  value = sweep_pitch * (length - counter) / length (C division); the counter runs 0 .. length.
 *         porta_time == 0: length = the note's length in ticks, the counter advances per sequencer tick.
 *         porta_time  > 0: length = porta_time^2 * |sweep_pitch| >> 11 (periodic steps), the counter advances per
 *         periodic step (the channel's AUTO_SWEEP flag); tempo does not matter.
 *  porta  sweep_pitch = (porta_key - key) * 64 (when the track's portamento flag is on) + the track's sweep_pitch;
 *         the track's porta_key is set to the note's key at every note-on (flag on or off).
 */
#include "ac_shared.h"

void ac_lfo_init(AC_LfoParam *p) {
  p->target = 0;
  p->speed = 16;
  p->depth = 0;
  p->range = 1;
  p->delay = 0;
}

void ac_lfo_start(AC_Lfo *l) {
  l->delay_counter = 0;
  l->counter = 0;
}

void ac_lfo_update(AcoreState *s, AC_Lfo *l, int periodic) {
  (void)s;
  if (!periodic) return; /* a wake caused by a command does not advance the LFO (no permanent shift seen) */
  if (l->delay_counter < l->param.delay) {
    l->delay_counter++;
    return;
  }
  l->counter = (u16)((l->counter + (l->param.speed << 6)) & 0x7FFF);
}

s32 ac_lfo_value(AcoreState *s, const AC_Lfo *l) {
  (void)s;
  if (l->param.depth == 0) return 0;
  s32 v = (s32)ac_sine(l->counter >> 8) * l->param.depth * l->param.range;
  if (l->param.target == 0) return (v * 64) >> 14;
  return v >> 8;
}

s32 ac_sweep_value(AcoreState *s, int ch) {
  const AC_ExChannel *c = &s->work.channel[ch];
  if (c->sweep_pitch == 0 || c->sweep_length <= 0) return 0;
  s32 left = c->sweep_length - c->sweep_counter;
  if (left < 0) left = 0;
  return (s32)((s64)c->sweep_pitch * left / c->sweep_length);
}

void ac_sweep_update(AcoreState *s, int ch, int periodic) {
  AC_ExChannel *c = &s->work.channel[ch];
  int autosw = (c->flags & AC_EXF_AUTO_SWEEP) != 0;
  if (autosw != (periodic != 0)) return; /* auto: per periodic step; otherwise per sequencer tick (periodic = 0) */
  if (c->sweep_counter < c->sweep_length) c->sweep_counter++;
}

void ac_porta_setup(AcoreState *s, int ch, AC_Track *t, int key, s32 note_length) {
  AC_ExChannel *c = &s->work.channel[ch];
  s32 sweep = (s16)t->sweep_pitch;
  if (t->flags & AC_TRF_PORTA) sweep += ((int)t->porta_key - key) * 64;
  c->sweep_pitch = (s16)sweep;
  c->sweep_counter = 0;
  c->flags &= (u8)~AC_EXF_AUTO_SWEEP;
  if (t->porta_time == 0) {
    c->sweep_length = note_length;
  } else {
    s32 a = sweep < 0 ? -sweep : sweep;
    c->sweep_length = (s32)(((u32)t->porta_time * t->porta_time * (u32)a) >> 11);
    c->flags |= AC_EXF_AUTO_SWEEP;
  }
  t->porta_key = (u8)key;
}
