/* port/acore piece 15: Commands (docs/acore.md). OWNED BY PIECE 15. See p15_commands.h for the register sequences,
 * which come from the oracle's trace (the ROM's driver in melonDS); GBATEK/kiwi say nothing about the commands. */
#include "ac_shared.h"

#define CH_MASK 0xFFFFu
/* what the channel registers read back: the volume 7 bits, the divider 2, hold, the pan 7, duty, repeat, format,
 * start (the oracle's STOP_TIMER writes the masked readback) */
#define CNT_READ_MASK 0xFF7F837Fu

static u32 cnt_read(const AcoreState *s, int ch) { return ac_hw_read(s, AC_REG_CH(ch) + AC_CH_CNT, 4) & CNT_READ_MASK; }

static int arg_player(u32 p) { return p < AC_NPLAYER ? (int)p : -1; }

/* ---- alarms ---- */
static void alarm_start(AcoreState *s, int no) {
  AC_Alarm *a = &s->work.alarm[no];
  AC_P15 *p = &s->p15;
  a->enable = 1;
  if (p->nalarm_run == AC_P15_MAX_ALARM_RUNS) { /* drop the oldest */
    for (int i = 1; i < AC_P15_MAX_ALARM_RUNS; i++) p->alarm_run[i - 1] = p->alarm_run[i];
    p->nalarm_run--;
  }
  AC_AlarmRun *r = &p->alarm_run[p->nalarm_run++];
  r->no = (unsigned char)no, r->id = a->id;
  r->start = s->now;
  r->first = s->now + ((u64)a->tick_lo | (u64)a->tick_hi << 32) * AC_OS_TICK_CYCLES;
  r->period = ((u64)a->period_lo | (u64)a->period_hi << 32) * AC_OS_TICK_CYCLES;
  r->stop = ~0ull;
}

static void alarm_stop(AcoreState *s, int no) {
  AC_Alarm *a = &s->work.alarm[no];
  AC_P15 *p = &s->p15;
  for (int i = 0; i < p->nalarm_run; i++)
    if (p->alarm_run[i].no == no && p->alarm_run[i].stop == ~0ull) p->alarm_run[i].stop = s->now;
  a->enable = 0;
  a->id++; /* the oracle: a stop moves the id on */
}

int ac_p15_alarm_fires(const AcoreState *s, unsigned long long until, AC_AlarmFire *out, int max) {
  const AC_P15 *p = &s->p15;
  int n = 0;
  unsigned long long k[AC_P15_MAX_ALARM_RUNS] = {0};
  for (;;) { /* merge the runs by time: take the earliest next firing */
    int best = -1;
    unsigned long long bt = 0;
    for (int i = 0; i < p->nalarm_run; i++) {
      const AC_AlarmRun *r = &p->alarm_run[i];
      if (r->period == 0 && k[i] > 0) continue;
      unsigned long long t = r->first + k[i] * r->period;
      if (t > r->stop || t > until) continue;
      if (best < 0 || t < bt) best = i, bt = t;
    }
    if (best < 0 || n >= max) break;
    out[n].time = bt, out[n].no = p->alarm_run[best].no, out[n].id = p->alarm_run[best].id;
    n++, k[best]++;
  }
  return n;
}

/* ---- captures: the readback of SNDCAPxCNT (the oracle drops capture 1's bit 0) ---- */
static u32 cap_read(const AcoreState *s, int n) { return ac_hw_read(s, AC_REG_SNDCAP0CNT + (u32)n, 1) & (n ? 0x8Eu : 0x8Fu); }

static void setup_common(AcoreState *s, int ch, u32 cnt, u32 pan) {
  s->p15.chan_pan[ch] = (u8)(pan & 0x7F);
  u32 reg_pan = s->p15.master_on ? (u32)s->p15.master_pan : (pan & 0x7F);
  ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT, cnt | reg_pan << 16, 4);
}

static void write_timer(AcoreState *s, int ch, u32 timer) {
  ac_hw_write(s, AC_REG_CH(ch) + AC_CH_TMR, (0x10000u - (timer & 0xFFFF)) & 0xFFFF, 2);
}

static void do_invalidate_wave(AcoreState *s, u32 start, u32 end) {
  for (int ch = 0; ch < AC_NCH; ch++) {
    const AC_ExChannel *c = &s->work.channel[ch];
    if (!(c->flags & AC_EXF_ACTIVE) || c->data < start || c->data >= end) continue;
    /* stopped at once, in the command's own step (the oracle: the CNT write with the start bit cleared) */
    ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT, cnt_read(s, ch) & ~AC_CNT_START, 4);
    ac_exch_free(s, ch);
    s->work.channel[ch].flags &= (u8)~(0x1F << AC_EXF_SYNC_SHIFT); /* already stopped: no second stop at the next sync (oracle, title) */
    s->p11.started &= ~(1u << ch);
  }
}

/* INVALIDATE_SEQ / BANK: the players reading the range stop (the oracle: the notes release, no new ones) */
static void do_invalidate_player(AcoreState *s, u32 start, u32 end, int bank) {
  for (int p = 0; p < AC_NPLAYER; p++) {
    const AC_Player *pl = &s->work.player[p];
    if (!(pl->flags & (AC_PLF_ACTIVE | AC_PLF_PREPARED))) continue;
    int hit = 0;
    if (bank) {
      hit = pl->bank >= start && pl->bank < end;
    } else {
      for (int i = 0; i < AC_TRACKS_PER_PLAYER && !hit; i++) {
        if (pl->tracks[i] >= AC_NTRACK) continue;
        const AC_Track *t = &s->work.track[pl->tracks[i]];
        hit = (t->flags & AC_TRF_ACTIVE) && t->base >= start && t->base < end;
      }
    }
    if (hit) ac_player_stop(s, p);
  }
}

static void read_driver_info(AcoreState *s, u32 addr) {
  const u8 *w = (const u8 *)&s->work;
  for (u32 i = 0; i < sizeof(AC_Work); i++) ac_wr8(s, addr + AC_DI_WORK + i, w[i]);
  for (int ch = 0; ch < AC_NCH; ch++) ac_wr32(s, addr + (u32)AC_DI_CHCTRL + 4u * (u32)ch, cnt_read(s, ch));
  ac_wr32(s, addr + (u32)AC_DI_WORKADDR, s->drv.work_addr);
  ac_wr32(s, addr + (u32)AC_DI_LOCKED, ac_exch_locked(s));
  for (u32 i = 0; i < 24; i++) ac_wr8(s, addr + (u32)AC_DI_LOCKED + 4 + i, 0);
}

void ac_cmd_exec(AcoreState *s, const u32 cmd[5]) {
  AC_P15 *p = &s->p15;
  const u32 a0 = cmd[1], a1 = cmd[2], a2 = cmd[3], a3 = cmd[4];
  int pl;
  switch (cmd[0]) {
  case AC_CMD_START_SEQ:
    if ((pl = arg_player(a0)) >= 0) ac_player_start(s, pl, a1, a2, a3);
    break;
  case AC_CMD_STOP_SEQ:
    if ((pl = arg_player(a0)) >= 0) ac_player_stop(s, pl);
    break;
  case AC_CMD_PREPARE_SEQ:
    if ((pl = arg_player(a0)) >= 0) ac_player_prepare(s, pl, a1, a2, a3);
    break;
  case AC_CMD_START_PREPARED_SEQ:
    if ((pl = arg_player(a0)) >= 0) ac_player_start_prepared(s, pl);
    break;
  case AC_CMD_PAUSE_SEQ:
    if ((pl = arg_player(a0)) >= 0) ac_player_pause(s, pl, a1 != 0);
    break;
  case AC_CMD_SKIP_SEQ:
    if ((pl = arg_player(a0)) >= 0) ac_player_skip(s, pl, a1);
    break;
  case AC_CMD_PLAYER_PARAM:
    if ((pl = arg_player(a0)) >= 0) ac_player_param(s, pl, a1, a2, (int)a3);
    break;
  case AC_CMD_TRACK_PARAM:
    if ((pl = arg_player(a0 & 0xFFFFFFu)) >= 0) ac_track_param_raw(s, pl, a1, a2, a3, (int)(a0 >> 24));
    break;
  case AC_CMD_MUTE_TRACK:
    if ((pl = arg_player(a0)) >= 0) ac_track_mute(s, pl, a1, (int)a2);
    break;
  case AC_CMD_ALLOCATABLE_CHANNEL:
    if ((pl = arg_player(a0)) >= 0) ac_track_allocatable(s, pl, a1, a2);
    break;
  case AC_CMD_PLAYER_LOCAL_VAR:
    if ((pl = arg_player(a0)) >= 0 && a1 < AC_NVAR) ac_var_set(s, pl, (int)a1, (s16)a2);
    break;
  case AC_CMD_PLAYER_GLOBAL_VAR:
    if (a0 < AC_NVAR) ac_var_set(s, -1, (int)a0, (s16)a1);
    break;

  case AC_CMD_START_TIMER:
    for (int ch = 0; ch < AC_NCH; ch++)
      if (a0 >> ch & 1) ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT + 3, (cnt_read(s, ch) >> 24 | 0x80u), 1);
    if ((a1 & 3) == 3)
      ac_hw_write(s, AC_REG_SNDCAP0CNT, (cap_read(s, 0) | 0x80u) | (cap_read(s, 1) | 0x80u) << 8, 2);
    else
      for (int n = 0; n < 2; n++)
        if (a1 >> n & 1) ac_hw_write(s, AC_REG_SNDCAP0CNT + (u32)n, cap_read(s, n) | 0x80u, 1);
    for (int n = 0; n < AC_NALARM; n++)
      if (a2 >> n & 1) alarm_start(s, n);
    break;
  case AC_CMD_STOP_TIMER:
    for (int ch = 0; ch < AC_NCH; ch++)
      if (a0 >> ch & 1)
        ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT, (cnt_read(s, ch) & ~AC_CNT_START) | ((a3 & 1) ? AC_CNT_HOLD : 0), 4);
    for (int n = 0; n < 2; n++)
      if (a1 >> n & 1) ac_hw_write(s, AC_REG_SNDCAP0CNT + (u32)n, 0, 1);
    for (int n = 0; n < AC_NALARM; n++)
      if (a2 >> n & 1) alarm_stop(s, n);
    break;

  case AC_CMD_SETUP_CHANNEL_PCM: { /* a0 ch | timer << 16, a1 data, a2 loopLen | shift << 22 | vol << 24, a3 ... */
    u32 ch = a0 & 0xFFFF;
    if (ch >= AC_NCH) break;
    s->bus.music_ch &= (u16)~(1u << ch);
    u32 cnt = (a2 >> 24 & 0x7F) | (a2 >> 22 & 3) << 8 | (a3 >> 26 & 3) << 27 | (a3 >> 24 & 3) << 29;
    setup_common(s, (int)ch, cnt, a3 >> 16 & 0xFF);
    write_timer(s, (int)ch, a0 >> 16);
    ac_hw_write(s, AC_REG_CH(ch) + AC_CH_PNT, a3 & 0xFFFF, 2);
    ac_hw_write(s, AC_REG_CH(ch) + AC_CH_LEN, a2 & 0x3FFFFF, 4);
    ac_hw_write(s, AC_REG_CH(ch) + AC_CH_SAD, a1, 4);
    break;
  }
  case AC_CMD_SETUP_CHANNEL_PSG:     /* a0 ch, a1 volume | shift << 8, a2 pan | timer << 8, a3 duty (the oracle's packing) */
  case AC_CMD_SETUP_CHANNEL_NOISE: { /* a0 ch, a1 volume | shift << 8, a2 pan | timer << 8 */
    if (a0 >= AC_NCH) break;
    s->bus.music_ch &= (u16)~(1u << a0);
    u32 cnt = (a1 & 0x7F) | (a1 >> 8 & 3) << 8 | 3u << 29;
    if (cmd[0] == AC_CMD_SETUP_CHANNEL_PSG) cnt |= (a3 & 7) << 24;
    setup_common(s, (int)a0, cnt, a2 & 0xFF);
    write_timer(s, (int)a0, a2 >> 8);
    break;
  }
  case AC_CMD_SETUP_CAPTURE: { /* a0 buffer, a1 length (words), a2 add << 27 | source << 28 | loop << 29 | pcm8 << 30 | cap << 31 */
    u32 n = a2 >> 31;
    u32 ctl = (a2 >> 27 & 1) | (a2 >> 28 & 1) << 1 | (~a2 >> 29 & 1) << 2 | (a2 >> 30 & 1) << 3;
    ac_hw_write(s, AC_REG_SNDCAP0CNT + n, ctl, 1);
    ac_hw_write(s, AC_REG_SNDCAP0DAD + 8 * n, a0, 4);
    ac_hw_write(s, AC_REG_SNDCAP0LEN + 8 * n, a1 & 0xFFFF, 2);
    break;
  }
  case AC_CMD_SETUP_ALARM:
    if (a0 < AC_NALARM) {
      AC_Alarm *al = &s->work.alarm[a0];
      al->id = (u8)a3;
      al->tick_lo = a1, al->tick_hi = 0;
      al->period_lo = a2, al->period_hi = 0;
    }
    break;

  case AC_CMD_CHANNEL_TIMER:
    for (int ch = 0; ch < AC_NCH; ch++)
      if (a0 >> ch & 1) write_timer(s, ch, a1);
    break;
  case AC_CMD_CHANNEL_VOLUME:
    for (int ch = 0; ch < AC_NCH; ch++)
      if (a0 >> ch & 1) ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT, (a1 | a2 << 8) & 0xFFFF, 2);
    break;
  case AC_CMD_CHANNEL_PAN:
    for (int ch = 0; ch < AC_NCH; ch++)
      if (a0 >> ch & 1) {
        p->chan_pan[ch] = (u8)a1;
        ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT + 2, p->master_on ? (u32)p->master_pan : (a1 & 0xFF), 1);
      }
    break;
  case AC_CMD_SURROUND_DECAY:
    /* the oracle writes 0 to the volume byte of every channel but 1 and 3, whatever the decay (not used by the
     * game; suspect.txt) */
    for (int ch = 0; ch < AC_NCH; ch++)
      if (ch != 1 && ch != 3) ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT, 0, 1);
    break;
  case AC_CMD_MASTER_VOLUME:
    ac_hw_write(s, AC_REG_SOUNDCNT, a0 & 0xFF, 1);
    break;
  case AC_CMD_MASTER_PAN:
    if ((s32)a0 < 0) {
      p->master_on = 0;
      for (int ch = 0; ch < AC_NCH; ch++) ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT + 2, p->chan_pan[ch], 1);
    } else {
      p->master_on = 1, p->master_pan = (int)(a0 & 0xFF);
      for (int ch = 0; ch < AC_NCH; ch++) ac_hw_write(s, AC_REG_CH(ch) + AC_CH_CNT + 2, (u32)p->master_pan, 1);
    }
    break;
  case AC_CMD_OUTPUT_SELECTOR: /* SOUNDCNT byte 1: left | right << 2 | ch1 << 4 | ch3 << 5 | enable */
    ac_hw_write(s, AC_REG_SOUNDCNT + 1, (a0 & 3) | (a1 & 3) << 2 | (a2 & 1) << 4 | (a3 & 1) << 5 | 0x80u, 1);
    break;

  case AC_CMD_LOCK_CHANNEL:
    ac_exch_lock(s, a0 & CH_MASK, a1);
    break;
  case AC_CMD_UNLOCK_CHANNEL:
    ac_exch_unlock(s, a0 & CH_MASK, a1);
    break;
  case AC_CMD_STOP_UNLOCKED_CHANNEL:
    ac_exch_stop_unlocked(s, a0 & CH_MASK, a1);
    break;
  case AC_CMD_SHARED_WORK:
    s->drv.shared = a0;
    break;
  case AC_CMD_INVALIDATE_SEQ:
    do_invalidate_player(s, a0, a1, 0);
    break;
  case AC_CMD_INVALIDATE_BANK:
    do_invalidate_player(s, a0, a1, 1);
    break;
  case AC_CMD_INVALIDATE_WAVE:
    do_invalidate_wave(s, a0, a1);
    break;
  case AC_CMD_READ_DRIVER_INFO:
    read_driver_info(s, a0);
    break;
  default:
    break;
  }
}

void ac_cmd_proc(AcoreState *s) {
  for (u32 i = 0; i < s->drv.nqueue; i++) {
    const AC_List *l = &s->drv.queue[i];
    for (u32 c = 0; c < l->n; c++) ac_drv_cmd_time(s, l->cmd[c][0]), ac_cmd_exec(s, l->cmd[c]);
    s->p15.finish_tag++;
    if (s->drv.shared) ac_wr32(s, s->drv.shared + AC_SW_FINISH_TAG, s->p15.finish_tag);
  }
  s->drv.nqueue = 0;
}
