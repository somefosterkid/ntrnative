/* port/acore piece 14: Channel update (docs/acore.md). OWNED BY PIECE 14.
 *
 * ac_exch_main(s, periodic) is the driver's ExChannelMain: per active ExChannel, in this order:
 *   1. the end checks, on the state the previous wake left (observed with the oracle, drv_note: the release reaches
 *      the floor and its volume is written at wake W+1, the hardware stop at W+2): a channel in RELEASE whose
 *      envelope is at the floor, or whose hardware channel stopped by itself (a one-shot's end) with no start
 *      pending, ends: ac_exch_free (11: the end callback to the track), the STOP sync flag.
 *   2. on a periodic wake: the envelope (12), the LFO and the sweep (13) advance one step.
 *   3. volume = decibel(velocity) + envelope + user decays (+ the LFO when it targets volume) -> calc_volume (6).
 *   4. timer = calc_timer(wave timer, (key - original key) * 64 + user pitch + sweep (+ LFO pitch)).
 *   5. pan = init pan scaled by the pan range + user pan (+ LFO pan), +64, clamped to 0-127.
 *   A changed volume / timer / pan sets its sync flag in the channel's flags (AC_SYNC_* << AC_EXF_SYNC_SHIFT); the
 *   registers are written by 11's ac_exch_sync at the next wake.
 * The note's length is NOT counted here: the oracle shows it counts sequence ticks (24 ticks at tempo 120 = 48
 * wakes), so the track's tick does it, through ac_exch_length_tick below (declared here, not in ac_shared.h).
 */
#include "ac_shared.h"

#define AC_ENV_FLOOR (-(723 << 7)) /* env_decay at the quietest: -723 dB*10, scaled by 128 */

static void set_sync(AC_ExChannel *c, u32 sync) { c->flags = (u8)(c->flags | (sync << AC_EXF_SYNC_SHIFT)); }
static int sync_pending(const AC_ExChannel *c, u32 sync) { return (c->flags >> AC_EXF_SYNC_SHIFT) & sync; }

int ac_exch_pan_value(const AC_ExChannel *c, int lfo_pan) {
  int pan = c->init_pan;
  if (c->pan_range != 127) pan = (pan * (int)c->pan_range + 64) >> 7;
  pan += c->user_pan + lfo_pan;
  pan += 64;
  if (pan < 0) pan = 0;
  if (pan > 127) pan = 127;
  return pan;
}

void ac_exch_length_tick(AcoreState *s, int ch) {
  AC_ExChannel *c = &s->work.channel[ch];
  if (c->length > 0) {
    c->length--;
    if (c->length == 0) ac_exch_release(s, ch);
  }
}

void ac_exch_main(AcoreState *s, int periodic) {
  for (int i = 0; i < AC_NCH; i++) {
    AC_ExChannel *c = &s->work.channel[i];
    if (!(c->flags & AC_EXF_ACTIVE)) continue;
    if (!ac_dom_ch(s, i)) continue; /* the other bus's channel (bus split) */

    int hw_busy = (ac_ch_reg32(s, i, AC_CH_CNT) & AC_CNT_START) != 0;
    int starting = (c->flags & AC_EXF_START) != 0;
    int ended = !starting && !sync_pending(c, AC_SYNC_START) && !hw_busy;
    if (ended) { /* the hardware stopped itself: no stop write (oracle: se_menu's one-shot ADPCM note) */
      ac_exch_free(s, i);
      c->flags = (u8)(c->flags & ~(AC_EXF_ACTIVE | (0x1F << AC_EXF_SYNC_SHIFT)));
      s->p11.started &= ~(1u << i);
      continue;
    }

    if (periodic) ac_env_update(s, i);
    ac_lfo_update(s, &c->lfo, periodic);
    ac_sweep_update(s, i, periodic);

    int lfo = ac_lfo_value(s, &c->lfo);
    int lfo_vol = 0, lfo_pitch = 0, lfo_pan = 0;
    if (c->lfo.param.target == 0) lfo_pitch = lfo;
    else if (c->lfo.param.target == 1) lfo_vol = lfo;
    else lfo_pan = lfo;

    int vol = ac_decibel_sq(c->velocity) + (c->env_decay >> 7) + c->user_decay + c->user_decay2 + lfo_vol;
    if (vol < -32768) vol = -32768;
    if (vol > 32767) vol = 32767;
    /* a note in release whose total volume reached the floor ends in this very step (oracle: no volume write) */
    if (c->env_status == AC_ENV_RELEASE && vol <= -723) {
      ac_exch_free(s, i);
      c->flags = (u8)(c->flags & ~AC_EXF_ACTIVE);
      set_sync(c, AC_SYNC_STOP);
      continue;
    }
    u16 volume = ac_calc_volume(vol);

    int pitch = ((int)c->key - (int)c->original_key) * 64 + c->user_pitch + ac_sweep_value(s, i) + lfo_pitch;
    u16 timer = ac_calc_timer(c->wave.timer, pitch);

    u8 pan = (u8)ac_exch_pan_value(c, lfo_pan);

    if (volume != c->volume) { c->volume = volume; set_sync(c, AC_SYNC_VOLUME); }
    if (timer != c->timer) { c->timer = timer; set_sync(c, AC_SYNC_TIMER); }
    if (pan != c->pan) { c->pan = pan; set_sync(c, AC_SYNC_PAN); }
    if (starting) { /* ac_exch_start's flag becomes the sync flag 11 acts on */
      c->flags &= (u8)~AC_EXF_START;
      set_sync(c, AC_SYNC_START);
    }
  }
}
