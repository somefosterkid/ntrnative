/* port/acore: the .acs loader, the register file and the event loop (docs/acore.md, "The time base").
 * OWNED BY THE SKELETON; the loop belongs to piece 17 from now on. */
#include "acore.h"
#include "ac_shared.h"
#include <stdlib.h>
#include <string.h>

const char *const ac_cmd_names[AC_CMD_COUNT] = {
    "START_SEQ", "STOP_SEQ", "PREPARE_SEQ", "START_PREPARED_SEQ", "PAUSE_SEQ", "SKIP_SEQ", "PLAYER_PARAM",
    "TRACK_PARAM", "MUTE_TRACK", "ALLOCATABLE_CHANNEL", "PLAYER_LOCAL_VAR", "PLAYER_GLOBAL_VAR", "START_TIMER",
    "STOP_TIMER", "SETUP_CHANNEL_PCM", "SETUP_CHANNEL_PSG", "SETUP_CHANNEL_NOISE", "SETUP_CAPTURE", "SETUP_ALARM",
    "CHANNEL_TIMER", "CHANNEL_VOLUME", "CHANNEL_PAN", "SURROUND_DECAY", "MASTER_VOLUME", "MASTER_PAN",
    "OUTPUT_SELECTOR", "LOCK_CHANNEL", "UNLOCK_CHANNEL", "STOP_UNLOCKED_CHANNEL", "SHARED_WORK", "INVALIDATE_SEQ",
    "INVALIDATE_BANK", "INVALIDATE_WAVE", "READ_DRIVER_INFO"};

_Static_assert(sizeof(AC_ExChannel) == 0x54, "AC_ExChannel is the DS's 0x54 bytes");
_Static_assert(offsetof(AC_ExChannel, lfo) == 0x28 && offsetof(AC_ExChannel, wave) == 0x38, "AC_ExChannel layout");
_Static_assert(sizeof(AC_Player) == 0x24 && offsetof(AC_Player, tempo_ratio) == 0x1A, "AC_Player layout");
_Static_assert(sizeof(AC_Track) == 0x40 && offsetof(AC_Track, mod) == 0x18 && offsetof(AC_Track, base) == 0x24,
               "AC_Track layout");
_Static_assert(offsetof(AC_Work, player) == 0x540 && offsetof(AC_Work, track) == 0x780, "AC_Work layout");

static u32 rd32le(const u8 *p) { return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24; }
static u64 rd64le(const u8 *p) { return (u64)rd32le(p) | (u64)rd32le(p + 4) << 32; }

static void reset(AcoreState *s) {
  u8 *mem = s->mem;
  s16 *pcm = s->pcm;
  size_t cap = s->pcm_cap;
  FILE *trace = s->trace;
  free(s->file);
  free(s->ev);
  memset(s, 0, sizeof *s);
  s->mem = mem;
  memset(s->mem, 0, AC_MAIN_SIZE);
  s->pcm = pcm;
  s->pcm_cap = cap;
  s->trace = trace;
  s->drv.on = 1;
  s->drv.work_addr = 0x03800000u;
  ac_driver_init(s);
}

AcoreState *acore_create(void) {
  AcoreState *s = calloc(1, sizeof *s);
  if (!s) return NULL;
  s->mem = calloc(1, AC_MAIN_SIZE);
  if (!s->mem) {
    free(s);
    return NULL;
  }
  reset(s);
  return s;
}

void acore_destroy(AcoreState *s) {
  if (!s) return;
  free(s->mem);
  free(s->pcm);
  free(s->file);
  free(s->ev);
  free(s);
}

const char *acore_info(const AcoreState *s, const char *key) {
  static char val[256];
  size_t kl = strlen(key);
  const char *p = s->info;
  while (*p) {
    const char *e = strchr(p, '\n');
    size_t l = e ? (size_t)(e - p) : strlen(p);
    if (l > kl && !strncmp(p, key, kl) && p[kl] == '=') {
      size_t vl = l - kl - 1 < sizeof val - 1 ? l - kl - 1 : sizeof val - 1;
      memcpy(val, p + kl + 1, vl);
      val[vl] = 0;
      return val;
    }
    p += l + (e ? 1 : 0);
  }
  return NULL;
}

int acore_load_acs_mem(AcoreState *s, const void *data, size_t size, char *err, size_t errlen) {
  reset(s);
  if (size < 8 || memcmp(data, "ACST", 4) || rd32le((const u8 *)data + 4) != 1) {
    snprintf(err, errlen, "not an ACST v1 stream");
    return 1;
  }
  s->file = malloc(size);
  if (!s->file) return snprintf(err, errlen, "out of memory"), 1;
  memcpy(s->file, data, size);
  s->file_size = size;
  u32 cap = 256;
  s->ev = malloc(cap * sizeof *s->ev);
  size_t pos = 8;
  while (pos + 8 <= size) {
    const u8 *c = s->file + pos;
    u32 sz = rd32le(c + 4);
    const u8 *d = c + 8;
    if (pos + 8 + sz > size) return snprintf(err, errlen, "chunk %.4s at 0x%zx runs past the end", c, pos), 1;
    AC_Event e;
    memset(&e, 0, sizeof e);
    int is_ev = 1;
    if (!memcmp(c, "INFO", 4)) {
      size_t l = sz < sizeof s->info - 1 ? sz : sizeof s->info - 1;
      memcpy(s->info, d, l);
      s->info[l] = 0;
      is_ev = 0;
    } else if (!memcmp(c, "MEM ", 4) && sz >= 12) {
      e.type = AC_EV_MEM, e.time = rd64le(d), e.addr = rd32le(d + 8), e.n = sz - 12, e.data = d + 12;
    } else if (!memcmp(c, "CMDS", 4) && sz >= 16) {
      e.type = AC_EV_CMDS, e.time = rd64le(d), e.frame = rd32le(d + 8), e.n = rd32le(d + 12), e.data = d + 16;
      if (16 + 20ull * e.n > sz) return snprintf(err, errlen, "CMDS at 0x%zx: %u commands don't fit", pos, e.n), 1;
    } else if (!memcmp(c, "WAKE", 4) && sz >= 8) {
      e.type = AC_EV_WAKE, e.time = rd64le(d);
    } else if (!memcmp(c, "REGS", 4) && sz >= 12) {
      e.type = AC_EV_REGS, e.time = rd64le(d), e.n = rd32le(d + 8), e.data = d + 12;
      if (12 + 12ull * e.n > sz) return snprintf(err, errlen, "REGS at 0x%zx: %u writes don't fit", pos, e.n), 1;
    } else if (!memcmp(c, "END ", 4) && sz >= 8) {
      e.type = AC_EV_END, e.time = rd64le(d);
      s->end_time = e.time;
    } else {
      is_ev = 0; /* unknown chunks are skipped */
    }
    if (is_ev) {
      if (s->nev && e.time < s->ev[s->nev - 1].time)
        return snprintf(err, errlen, "chunk %.4s at 0x%zx goes back in time", c, pos), 1;
      if (s->nev == cap) s->ev = realloc(s->ev, (cap *= 2) * sizeof *s->ev);
      s->ev[s->nev++] = e;
      if (e.time > s->end_time) s->end_time = e.time;
    }
    pos += 8 + ((sz + 3) & ~3u);
  }
  const char *v;
  if ((v = acore_info(s, "mode")) && !strcmp(v, "hw")) {
    s->drv.on = 0;
    s->io[AC_REG_SOUNDBIAS + 1] = 0; /* melonDS resets SOUNDBIAS to 0; the driver's boot leaves 0x200 */
  }
  if ((v = acore_info(s, "tick0"))) s->drv.tick0 = strtoull(v, NULL, 0);
  if ((v = acore_info(s, "sphase"))) s->sample_phase = strtoull(v, NULL, 0);
  if ((v = acore_info(s, "work_addr"))) s->drv.work_addr = (u32)strtoul(v, NULL, 0);
  s->drv.timed = 1;
  s->drv.next_tick = s->drv.tick0;
  s->drv.lag_periodic = 0;
  s->drv.lag_word = AC_LAG_WORD;
  if ((v = acore_info(s, "lag_p"))) s->drv.lag_periodic = strtoull(v, NULL, 0);
  if ((v = acore_info(s, "lag_w"))) s->drv.lag_word = strtoull(v, NULL, 0);
  return 0;
}

int acore_load_acs(AcoreState *s, const char *path, char *err, size_t errlen) {
  FILE *f = fopen(path, "rb");
  if (!f) return snprintf(err, errlen, "can't open %s", path), 1;
  fseek(f, 0, SEEK_END);
  long n = ftell(f);
  fseek(f, 0, SEEK_SET);
  u8 *buf = malloc(n > 0 ? (size_t)n : 1);
  size_t got = buf ? fread(buf, 1, (size_t)n, f) : 0;
  fclose(f);
  int r = got == (size_t)n ? acore_load_acs_mem(s, buf, got, err, errlen) : (snprintf(err, errlen, "read error"), 1);
  free(buf);
  return r;
}

/* ---- the register file ---- */
static u32 reg_ofs(u32 ofs) { return ofs >= AC_IO_BASE ? ofs - AC_IO_BASE : ofs; }

u32 ac_hw_read(const AcoreState *s, u32 ofs, int size) {
  ofs = reg_ofs(ofs);
  u32 v = 0;
  if (!s->drv.in_wake && ofs >= AC_IO_FIRST && ofs + (u32)size <= AC_IO_END) { /* the mixer's reads, ~50 a sample: no loop */
    const u8 *p = &s->io[ofs];
    if (size == 4) return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24;
    if (size == 2) return (u32)p[0] | (u32)p[1] << 8;
    if (size == 1) return p[0];
  }
  for (int i = 0; i < size; i++)
    if (ofs + i >= AC_IO_FIRST && ofs + i < AC_IO_END) v |= (u32)s->io[ofs + i] << (8 * i);
  if (s->drv.in_wake) /* the driver sees its own queued writes */
    for (u32 k = 0; k < s->drv.npq; k++) {
      const struct AC_PendWrite *w = &s->drv.pq[k];
      for (int i = 0; i < size; i++) {
        int j = (int)(ofs + (u32)i) - (int)w->addr;
        if (j >= 0 && j < w->size) v = (v & ~(0xFFu << (8 * i))) | (((w->value >> (8 * j)) & 0xFFu) << (8 * i));
      }
    }
  return v;
}

void ac_hw_set_busy(AcoreState *s, int ch, int busy) {
  u8 *b = &s->io[AC_REG_CH(ch) + 3];
  *b = busy ? (u8)(*b | 0x80) : (u8)(*b & 0x7F);
}

static void hw_apply(AcoreState *s, u32 ofs, u32 value, int size);

/* A write from inside a timed wake is queued for wake_t + cur (the pq stays ascending: cur only grows, and a later
 * wake's writes are inserted after the earlier ones due at the same time). */
void ac_hw_write(AcoreState *s, u32 ofs, u32 value, int size) {
  ofs = reg_ofs(ofs);
  if (s->drv.timed && s->drv.in_wake && s->drv.npq < sizeof s->drv.pq / sizeof s->drv.pq[0]) {
    if (s->drv.autogap) { /* CNT -> TMR 7, TMR -> PNT 5, PNT -> LEN 2, LEN -> SAD 2 (the channel setup of SETUP_CHANNEL_PCM) */
      u32 la = s->drv.last_addr;
      if (ofs >= AC_IO_FIRST && ofs < AC_REG_SOUNDCNT && (ofs & ~0xFu) == (la & ~0xFu)) {
        u32 lo = la & 0xF, o = ofs & 0xF;
        if (lo == 0 && o == 8) s->drv.cur += 7;
        else if (lo == 8 && o == 0xA) s->drv.cur += 5;
        else if (lo == 0xA && o == 0xC) s->drv.cur += 2;
        else if (lo == 0xC && o == 4) s->drv.cur += 2;
      }
      s->drv.last_addr = ofs;
    }
    u64 due = s->drv.wake_t + s->drv.cur;
    u32 k = s->drv.npq++;
    while (k > 0 && s->drv.pq[k - 1].due > due) s->drv.pq[k] = s->drv.pq[k - 1], k--;
    s->drv.pq[k] = (struct AC_PendWrite){due, ofs, value, size};
    return;
  }
  hw_apply(s, ofs, value, size);
}

static void hw_apply(AcoreState *s, u32 ofs, u32 value, int size) {
  if (s->trace) fprintf(s->trace, "%llu %08X %d %08X\n", (unsigned long long)s->now, AC_IO_BASE + ofs, size, value);
  for (int i = 0; i < size; i++) {
    u32 o = ofs + i;
    if (o < AC_IO_FIRST || o >= AC_IO_END) continue;
    u8 old = s->io[o], nv = (u8)(value >> (8 * i));
    s->io[o] = nv;
    if (o < AC_REG_SOUNDCNT && (o & 0xF) == 3 && ((old ^ nv) & 0x80)) { /* SOUNDxCNT bit 31 */
      int ch = (int)((o - AC_IO_FIRST) >> 4);
      if (nv & 0x80) ac_ch_start(s, ch);
      else ac_ch_stop(s, ch);
    } else if ((o == AC_REG_SNDCAP0CNT || o == AC_REG_SNDCAP1CNT) && ((old ^ nv) & 0x80)) {
      if (nv & 0x80) ac_cap_start(s, (int)(o - AC_REG_SNDCAP0CNT));
      else ac_cap_stop(s, (int)(o - AC_REG_SNDCAP0CNT));
    }
  }
}

/* ---- the event loop ---- */
static void push_sample(AcoreState *s, s16 l, s16 r) {
  size_t n = (size_t)(s->nsamples - s->pcm_off);
  if (2 * n + 2 > s->pcm_cap) {
    s->pcm_cap = s->pcm_cap ? s->pcm_cap * 2 : 1 << 16;
    s->pcm = realloc(s->pcm, s->pcm_cap * sizeof *s->pcm);
  }
  s->pcm[2 * n] = l;
  s->pcm[2 * n + 1] = r;
  s->nsamples++;
}

/* one mixer sample: the channels of the running domain (s->bus.dom) are read, mixed and stepped; solo mutes the others' share
 * of the mix (they are still read and stepped, so the run is the one without solo). Dom 0, solo 0: the whole core. */
static void hw_sample(AcoreState *s, s16 out[2]) {
  s32 chout[AC_NCH], mix[2] = {0, 0};
  out[0] = out[1] = 0;
  int dom = s->bus.dom, solo = s->bus.solo;
  for (int ch = 0; ch < AC_NCH; ch++) {
    if (dom == 0 || ac_dom_ch(s, ch)) chout[ch] = ac_ch_output(s, ch);
    else chout[ch] = 0;
  }
  if (solo)
    for (int ch = 0; ch < AC_NCH; ch++)
      if ((s->bus.music_ch >> ch & 1) != (solo == 1)) chout[ch] = 0;
  if (s->hq_fade) s->hq_fade--;
  if (dom != 2) ac_cap_premix(s, chout);
  ac_mix(s, chout, mix, out);
  if (dom != 2) ac_cap_sample(s, mix, chout);
  for (int ch = 0; ch < AC_NCH; ch++)
    if (dom == 0 || ac_dom_ch(s, ch)) ac_ch_step(s, ch);
}

void ac_hw_sample(AcoreState *s) {
  s16 out[2];
  hw_sample(s, out);
  push_sample(s, out[0], out[1]);
}

/* k samples with no event between them (acore_run). A channel's output depends only on its own state, its own registers and
 * memory, which nothing changes between events, except a capture unit (it writes memory a channel may play): so with no unit
 * running (and no solo or quality fade) each channel's k outputs are made in a row, then mixed sample by sample. The same
 * steps in the same order per channel as k ac_hw_sample calls, so the same samples; fewer calls and a channel's state stays hot. */
static void hw_block(AcoreState *s, u64 k) {
  if (k < 2 || s->p05.u[0].running || s->p05.u[1].running || s->hq_fade || s->bus.solo) {
    while (k--) ac_hw_sample(s);
    return;
  }
  int dom = s->bus.dom;
  s32 buf[AC_NCH][AC_BLOCK];
  while (k) {
    int n = k < AC_BLOCK ? (int)k : AC_BLOCK;
    u32 live = 0;
    for (int ch = 0; ch < AC_NCH; ch++) {
      if (dom != 0 && !ac_dom_ch(s, ch)) continue;
      ac_ch_run(s, ch, buf[ch], n);
      live |= 1u << ch;
    }
    size_t at = (size_t)(s->nsamples - s->pcm_off);
    while (2 * (at + (size_t)n) > s->pcm_cap) {
      s->pcm_cap = s->pcm_cap ? s->pcm_cap * 2 : 1 << 16;
      s->pcm = realloc(s->pcm, s->pcm_cap * sizeof *s->pcm);
    }
    ac_mix_block(s, (const s32(*)[AC_BLOCK])buf, live, n, s->pcm + 2 * at);
    s->nsamples += (u64)n;
    k -= (u64)n;
  }
}

/* ---- the music bus ---- */
void ac_bus_slot(AcoreState *s, int slot, int music) {
  AC_Bus *b = &s->bus;
  if (slot < 0 || slot >= AC_NPLAYER) return;
  music = music ? 1 : 0;
  b->last[slot] = (u8)music;
  if (b->nfifo[slot] == 8) memmove(b->fifo[slot], b->fifo[slot] + 1, 7), b->nfifo[slot]--;
  b->fifo[slot][b->nfifo[slot]++] = (u8)music;
}

void ac_bus_player_started(AcoreState *s, int p) {
  AC_Bus *b = &s->bus;
  int m = b->last[p];
  if (b->nfifo[p]) {
    m = b->fifo[p][0];
    memmove(b->fifo[p], b->fifo[p] + 1, (size_t)--b->nfifo[p]);
  }
  if (m) b->music_players |= (u16)(1u << p);
  else b->music_players &= (u16)~(1u << p);
}

void acore_bus_slot(AcoreState *s, int slot, int music) { ac_bus_slot(s, slot, music); }

void acore_bus_solo(AcoreState *s, int mode) { s->bus.solo = mode; }

/* Splits the music players off (on) or joins them again (off). Called between two acore_run calls. The music clock starts
 * where the main clock is (its next sample and next alarm are the main clock's next ones); joining continues the main
 * clock with every player and channel, as the music clock left them. */
void acore_bus_split(AcoreState *s, int on) {
  AC_Bus *b = &s->bus;
  on = on ? 1 : 0;
  if (on == b->split) return;
  b->split = on;
  b->dom = on;
  if (on) {
    b->mnsamples = s->nsamples;
    b->mnext_tick = s->drv.next_tick + s->drv.lag_periodic;
  } else {
    /* the main clock's next periodic wake keeps the distance the music clock's next wake had from the music's own time, so
     * the music's ticks stay evenly spaced across the join (else the first tick after it came up to half a period early or
     * late: a 2-60 sample jump of the music's timing) */
    u64 t = b->mnsamples * AC_CYCLES_PER_SAMPLE + s->sample_phase;
    u64 rem = b->mnext_tick > t ? b->mnext_tick - t : 0;
    u64 at = s->now + rem;
    s->drv.next_tick = at > s->drv.lag_periodic ? at - s->drv.lag_periodic : 0;
  }
}

int acore_bus_split_on(const AcoreState *s) { return s->bus.split; }

/* `frames` samples of the music bus (interleaved s16 L R) on the music clock; 0 samples when the bus is not split */
size_t acore_bus_run(AcoreState *s, unsigned frames, int16_t *out) {
  AC_Bus *b = &s->bus;
  if (!b->split) return 0;
  u64 save_now = s->now;
  int save_dom = b->dom;
  b->dom = 2;
  for (unsigned i = 0; i < frames; i++) {
    u64 t = b->mnsamples * AC_CYCLES_PER_SAMPLE + s->sample_phase;
    while (b->mnext_tick <= t) {
      s->now = b->mnext_tick;
      b->mnext_tick += AC_DRIVER_PERIOD;
      ac_music_wake(s);
    }
    s->now = t;
    s16 o[2];
    hw_sample(s, o);
    out[2 * i] = o[0];
    out[2 * i + 1] = o[1];
    b->mnsamples++;
  }
  b->dom = save_dom;
  s->now = save_now;
  return frames;
}

static void run_event(AcoreState *s, const AC_Event *e) {
  switch (e->type) {
  case AC_EV_MEM:
    for (u32 i = 0; i < e->n; i++) ac_wr8(s, e->addr + i, e->data[i]);
    break;
  case AC_EV_CMDS:
    if (s->drv.nqueue < AC_MAX_QUEUE) {
      AC_List *l = &s->drv.queue[s->drv.nqueue++];
      l->n = e->n < AC_MAX_LIST ? e->n : AC_MAX_LIST;
      for (u32 i = 0; i < l->n; i++)
        for (int j = 0; j < 5; j++) l->cmd[i][j] = rd32le(e->data + 20 * i + 4 * j);
    }
    break;
  case AC_EV_WAKE:
    if (s->drv.on && s->drv.npend < 16) s->drv.pendk[s->drv.npend] = 0, s->drv.pend[s->drv.npend++] = s->now;
    break;
  case AC_EV_REGS:
    for (u32 i = 0; i < e->n; i++) {
      const u8 *w = e->data + 12 * i;
      hw_apply(s, reg_ofs(rd32le(w)), rd32le(w + 4), (int)rd32le(w + 8));
    }
    break;
  default:
    break;
  }
}

void acore_run(AcoreState *s, u64 until) {
  for (;;) {
    u64 t_sample = s->nsamples * AC_CYCLES_PER_SAMPLE + s->sample_phase;
    u64 t_ev = s->next_ev < s->nev ? s->ev[s->next_ev].time : UINT64_MAX;
    /* a pass starts when its trigger has arrived and the thread is free */
    u64 busy = s->drv.free_at;
    u64 a_tick = s->drv.on ? s->drv.next_tick + s->drv.lag_periodic : UINT64_MAX;
    u64 a_word = s->drv.npend ? s->drv.pend[0] : UINT64_MAX;
    u64 t_tick = a_tick == UINT64_MAX ? a_tick : (a_tick > busy ? a_tick : busy);
    u64 t_word = a_word == UINT64_MAX ? a_word : (a_word > busy ? a_word : busy);
    if (t_tick == t_word && a_word < a_tick) t_tick = UINT64_MAX; /* the word came first */
    u64 t_pq = s->drv.npq ? s->drv.pq[0].due : UINT64_MAX;
    u64 t = t_sample < t_ev ? t_sample : t_ev;
    if (t_pq < t) t = t_pq;
    if (t_tick < t) t = t_tick;
    if (t_word < t) t = t_word;
    if (t >= until) break;
    s->now = t;
    /* at one instant: stream events, then the periodic wake, then a WAKE word's wake, then the sample */
    if (t_pq == t) {
      struct AC_PendWrite w = s->drv.pq[0];
      memmove(s->drv.pq, s->drv.pq + 1, --s->drv.npq * sizeof w);
      hw_apply(s, w.addr, w.value, w.size);
    } else if (t_ev == t) {
      run_event(s, &s->ev[s->next_ev++]);
    } else if (t_tick == t) {
      s->drv.next_tick += AC_DRIVER_PERIOD;
      s->drv.wakes++;
      s->drv.pass_word = 0;
      ac_driver_wake(s, 1);
    } else if (t_word == t) {
      int k = s->drv.pendk[0];
      memmove(s->drv.pend, s->drv.pend + 1, --s->drv.npend * sizeof s->drv.pend[0]);
      memmove(s->drv.pendk, s->drv.pendk + 1, s->drv.npend);
      s->drv.wakes++;
      s->drv.pass_word = 1;
      ac_driver_wake(s, 0);
      if (!k && s->drv.timed && s->drv.npend < 16) { /* the pass that follows a word (measured: title, drv_note) */
        memmove(s->drv.pend + 1, s->drv.pend, s->drv.npend * sizeof s->drv.pend[0]);
        memmove(s->drv.pendk + 1, s->drv.pendk, s->drv.npend);
        s->drv.pend[0] = 0, s->drv.pendk[0] = 1, s->drv.npend++;
      }
    } else {
      /* the samples before the next event (or `until`) in one block */
      u64 next = until;
      if (t_ev < next) next = t_ev;
      if (t_pq < next) next = t_pq;
      if (t_tick < next) next = t_tick;
      if (t_word < next) next = t_word;
      u64 k = (next - t_sample + AC_CYCLES_PER_SAMPLE - 1) / AC_CYCLES_PER_SAMPLE; /* the samples at t_sample + i * 1024 < next */
      hw_block(s, k);
      s->now = t_sample + (k - 1) * AC_CYCLES_PER_SAMPLE;
    }
  }
  s->now = until;
}

void acore_render_all(AcoreState *s) { acore_run(s, s->end_time); }
uint64_t acore_end(const AcoreState *s) { return s->end_time; }
const int16_t *acore_pcm(const AcoreState *s, size_t *n) {
  *n = (size_t)(s->nsamples - s->pcm_off);
  return s->pcm;
}
/* the live port (port/bridge/acore_bridge.c) takes the samples out as it renders them: forget the ones returned so
 * far (the time base, nsamples, goes on) */
void acore_pcm_discard(AcoreState *s) { s->pcm_off = s->nsamples; }
void acore_set_trace(AcoreState *s, FILE *f) { s->trace = f; }

/* what acore_load_acs_mem sets for a driver-mode stream with no INFO timing keys: the writes of a wake land after its
 * own latency (the pq), the periodic alarm starts at cycle 0, a WAKE word's pass has its measured lag */
void acore_set_quality(AcoreState *s, int enhanced) {
  if ((enhanced ? 1 : 0) != s->hq && s->now) s->hq_fade = 64; /* a switch while sound runs: cross-fade the channels' outputs */
  s->hq = enhanced ? 1 : 0;
}

void acore_set_live(AcoreState *s) {
  s->drv.timed = 1;
  s->drv.next_tick = s->drv.tick0;
  s->drv.lag_periodic = 0;
  s->drv.lag_word = AC_LAG_WORD;
}
uint8_t *acore_mem(AcoreState *s) { return s->mem; }
