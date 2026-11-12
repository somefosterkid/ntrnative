/* port/acore piece 16: Shared work and variables (docs/acore.md). OWNED BY PIECE 16. */
#include "ac_shared.h"

static void init_store(AcoreState *s) {
  AC_P16 *q = &s->p16;
  if (q->init) return;
  for (int p = 0; p < AC_NPLAYER; p++)
    for (int i = 0; i < AC_NVAR; i++) q->local[p][i] = AC_P16_DEFAULT_VAR;
  for (int i = 0; i < AC_NVAR; i++) q->global[i] = AC_P16_DEFAULT_VAR;
  q->init = 1;
}

/* the shared work's address of a variable, or 0 for the fallback store / a bad index */
static u32 var_addr(const AcoreState *s, int p, int n) {
  if (!s->drv.shared) return 0;
  return s->drv.shared + (p < 0 ? AC_SW_GLOBAL_VAR((u32)n) : AC_SW_PLAYER_VAR((u32)p, (u32)n));
}

static int var_ok(int p, int n) { return n >= 0 && n < AC_NVAR && p < AC_NPLAYER; }

s16 ac_var_get(AcoreState *s, int p, int n) {
  if (!var_ok(p, n)) return AC_P16_DEFAULT_VAR;
  u32 a = var_addr(s, p, n);
  if (a) return (s16)ac_rd16(s, a);
  init_store(s);
  return p < 0 ? s->p16.global[n] : s->p16.local[p][n];
}

void ac_var_set(AcoreState *s, int p, int n, s16 v) {
  if (!var_ok(p, n)) return;
  u32 a = var_addr(s, p, n);
  if (a) {
    ac_wr16(s, a, (u16)v);
    return;
  }
  init_store(s);
  if (p < 0) s->p16.global[n] = v;
  else s->p16.local[p][n] = v;
}

void ac_var_reset_player(AcoreState *s, int p) {
  if (p < 0 || p >= AC_NPLAYER) return;
  for (int i = 0; i < AC_NVAR; i++) ac_var_set(s, p, i, AC_P16_DEFAULT_VAR);
}

void ac_p16_tick(AcoreState *s, int p) {
  if (p >= 0 && p < AC_NPLAYER) s->p16.tick[p]++;
}

void ac_p16_tick_reset(AcoreState *s, int p) {
  if (p >= 0 && p < AC_NPLAYER) s->p16.tick[p] = 0;
}

/* UpdateSharedWork, the last step of a wake: the status words and tick counters (the variables are written as set).
 * The oracle: the channel bits follow the channels' busy bit in SOUNDxCNT, wave-out channels included (a locked
 * channel started by START_TIMER shows, and clears at STOP_TIMER); the player bit follows the active flag. */
void ac_shared_update(AcoreState *s) {
  u32 w = s->drv.shared;
  if (!w) return;
  u32 pst = 0, cst = 0, cap = 0;
  for (int p = 0; p < AC_NPLAYER; p++) {
    if (s->work.player[p].flags & AC_PLF_ACTIVE) pst |= 1u << p;
    ac_wr32(s, w + AC_SW_PLAYER_TICKS(p), s->p09.ticks[p]);
  }
  for (int c = 0; c < AC_NCH; c++)
    if (ac_ch_reg32(s, c, AC_CH_CNT) & AC_CNT_START) cst |= 1u << c;
  if (ac_hw_read(s, AC_REG_SNDCAP0CNT, 1) & 0x80) cap |= 1;
  if (ac_hw_read(s, AC_REG_SNDCAP1CNT, 1) & 0x80) cap |= 2;
  ac_wr32(s, w + AC_SW_PLAYER_STATUS, pst);
  ac_wr16(s, w + AC_SW_CHANNEL_STATUS, (u16)cst);
  ac_wr16(s, w + AC_SW_CAPTURE_STATUS, (u16)cap);
}
