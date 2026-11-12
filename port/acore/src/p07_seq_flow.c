/* port/acore piece 7: The sequence reader and flow (docs/acore.md). OWNED BY PIECE 7.
 *
 * One tick of a track. Command set after GBATEK 64 and kiwi (nds-sdat); the prefixes and variable ops follow the
 * usual SSEQ layout (loveemu): a prefix 0xA0 / 0xA1 replaces the LAST parameter of the next command by a random
 * value (two s16 follow: min, max) or by a variable (one byte follows: the number, 0-15 the player's, 16-31 the
 * global ones); 0xA2 makes the next command run only when the track's condition flag is set (otherwise it is read
 * and dropped). Prefixes can be chained in any order.
 *
 * Notes pass the key untransposed to ac_note_on (the transpose is piece 8's, applied to the channels' pitch).
 * Parameter commands go to ac_track_param(s, p, t, cmd, value) with the value as read: 0xC0-0xD6 the byte (0-255),
 * 0xE0 / 0xE1 the u16, 0xE3 the s16.
 */
#include "ac_shared.h"

typedef struct Rd {
  AcoreState *s;
  AC_Track *tr;
  int p;
  int mode; /* 0 plain, 1 random, 2 variable */
} Rd;

static u8 rd8(Rd *r) { return ac_rd8(r->s, r->tr->cur++); }
static s16 rd16(Rd *r) {
  u32 lo = rd8(r);
  u32 hi = rd8(r);
  return (s16)(lo | hi << 8);
}
static u32 rd24(Rd *r) {
  u32 a = rd8(r), b = rd8(r), c = rd8(r);
  return a | b << 8 | c << 16;
}
static s32 rd_vlq(Rd *r) {
  s32 v = 0;
  for (int i = 0; i < 4; i++) {
    u8 b = rd8(r);
    v = (v << 7) | (b & 0x7F);
    if (!(b & 0x80)) break;
  }
  return v;
}

static s16 var_get(Rd *r, int n) {
  n &= 31;
  return n < 16 ? ac_var_get(r->s, r->p, n) : ac_var_get(r->s, -1, n - 16);
}
static void var_set(Rd *r, int n, s16 v) {
  n &= 31;
  if (n < 16) ac_var_set(r->s, r->p, n, v);
  else ac_var_set(r->s, -1, n - 16, v);
}

/* a random value in [min, max] from the driver's generator ((rand * range) >> 16) */
static s32 random_in(AcoreState *s, int min, int max) {
  if (max < min) { int t = min; min = max; max = t; }
  u32 rnd = ac_random(s);
  return (s32)((rnd * (u32)(max - min + 1)) >> 16) + min;
}

/* the last parameter of a command: plain (kind 0 = u8, 1 = s16, 2 = VLQ), or replaced by the prefix */
static s32 rd_arg(Rd *r, int kind) {
  if (r->mode == 1) {
    s16 mn = rd16(r);
    s16 mx = rd16(r);
    return random_in(r->s, mn, mx);
  }
  if (r->mode == 2) {
    int n = rd8(r);
    return var_get(r, n);
  }
  if (kind == 0) return rd8(r);
  if (kind == 1) return rd16(r);
  return rd_vlq(r);
}

/* runs one command (reads it fully; acts only when exec). Returns 1 when the track ended. */
static int run_cmd(Rd *r, int exec) {
  AcoreState *s = r->s;
  AC_Track *tr = r->tr;
  int p = r->p;
  r->mode = 0;
  u8 cmd = rd8(r);
  for (;;) { /* prefixes */
    if (cmd == 0xA0) r->mode = 1;
    else if (cmd == 0xA1) r->mode = 2;
    else if (cmd == 0xA2) { if (!(tr->flags & AC_TRF_CMP)) exec = 0; }
    else break;
    cmd = rd8(r);
  }

  if (cmd < 0x80) { /* note: key, velocity, length (VLQ) */
    int vel = rd8(r);
    s32 len = rd_arg(r, 2);
    if (!exec) return 0;
    if (vel > 127) vel = 127;
    ac_note_on(s, p, (int)(tr - s->work.track), cmd, vel, len);
    if (!(tr->flags & AC_TRF_PORTA)) (void)0;
    if (tr->flags & AC_TRF_NOTE_WAIT) {
      tr->wait = len;
      if (len == 0) tr->flags |= AC_TRF_NOTE_FINISH_WAIT;
    }
    return 0;
  }

  switch (cmd) {
  case 0x80: { s32 w = rd_arg(r, 2); if (exec) tr->wait = w; return 0; }
  case 0x81: { s32 v = rd_arg(r, 2); if (exec) tr->prgNo = (u16)v; return 0; }
  case 0x93: {
    int no = rd8(r);
    u32 off = rd24(r);
    if (exec) ac_player_open_track(s, p, no, tr->base + off);
    return 0;
  }
  case 0x94: { u32 off = rd24(r); if (exec) tr->cur = tr->base + off; return 0; }
  case 0x95: {
    u32 off = rd24(r);
    if (exec && tr->call_stack_depth < AC_CALL_DEPTH) {
      tr->call_stack[tr->call_stack_depth] = tr->cur;
      tr->loop_count[tr->call_stack_depth] = 0;
      tr->call_stack_depth++;
      tr->cur = tr->base + off;
    }
    return 0;
  }
  case 0xFD:
    if (exec && tr->call_stack_depth > 0) {
      tr->call_stack_depth--;
      tr->cur = tr->call_stack[tr->call_stack_depth];
    }
    return 0;
  case 0xD4: {
    s32 n = rd_arg(r, 0);
    if (exec && tr->call_stack_depth < AC_CALL_DEPTH) {
      tr->call_stack[tr->call_stack_depth] = tr->cur;
      tr->loop_count[tr->call_stack_depth] = (u8)n;
      tr->call_stack_depth++;
    }
    return 0;
  }
  case 0xFC:
    if (exec && tr->call_stack_depth > 0) {
      int d = tr->call_stack_depth - 1;
      int cnt = tr->loop_count[d];
      if (cnt) {
        cnt--;
        tr->loop_count[d] = (u8)cnt;
        if (cnt == 0) { tr->call_stack_depth--; return 0; } /* done: continue past the end marker */
      }
      tr->cur = tr->call_stack[d];
    }
    return 0;
  case 0xFE: (void)rd16(r); return 0; /* tracks used: the player's business */
  case 0xFF:
    if (!exec) return 0;
    tr->flags &= (u8)~AC_TRF_ACTIVE;
    return 1;
  default: break;
  }

  if (cmd >= 0xB0 && cmd <= 0xBD) { /* variable ops: number, value */
    int n = rd8(r);
    s32 v = rd_arg(r, 1);
    if (!exec || cmd == 0xB7) return 0;
    if (cmd == 0xB6) { /* random: 0..v, or -v..0 for a negative v */
      v = v < 0 ? -random_in(s, 0, -v) : random_in(s, 0, v);
      cmd = 0xB0;
    }
    s32 a = var_get(r, n);
    switch (cmd) {
    case 0xB0: var_set(r, n, (s16)v); break;
    case 0xB1: var_set(r, n, (s16)(a + v)); break;
    case 0xB2: var_set(r, n, (s16)(a - v)); break;
    case 0xB3: var_set(r, n, (s16)(a * v)); break;
    case 0xB4: if (v != 0) var_set(r, n, (s16)(a / v)); break;
    case 0xB5: var_set(r, n, (s16)(v < 0 ? (a >> (-v > 31 ? 31 : -v)) : (v > 31 ? 0 : (s32)((u32)a << v)))); break;
    default: {
      int c = 0;
      switch (cmd) {
      case 0xB8: c = a == v; break;
      case 0xB9: c = a >= v; break;
      case 0xBA: c = a > v; break;
      case 0xBB: c = a <= v; break;
      case 0xBC: c = a < v; break;
      case 0xBD: c = a != v; break;
      }
      if (c) tr->flags |= AC_TRF_CMP;
      else tr->flags &= (u8)~AC_TRF_CMP;
    }
    }
    return 0;
  }

  /* parameter commands: 8-bit 0xC0-0xD6 (not 0xD4), 16-bit 0xE0-0xE3 */
  if ((cmd >= 0xC0 && cmd <= 0xD6) || (cmd >= 0xE0 && cmd <= 0xE3)) {
    int wide = cmd >= 0xE0;
    s32 v = rd_arg(r, wide ? 1 : 0);
    if (!exec) return 0;
    if (wide && cmd != 0xE3) v &= 0xFFFF;
    else if (!wide) v &= 0xFF;
    ac_track_param(s, p, (int)(tr - s->work.track), cmd, v);
  }
  return 0; /* unknown commands: no arguments */
}

int ac_track_tick(AcoreState *s, int p, int t) {
  AC_Track *tr = &s->work.track[t];
  Rd r = {s, tr, p, 0};
  if (!(tr->flags & AC_TRF_ACTIVE)) return 1;
  /* the track's channels, once per sequencer tick: the note length counts down (p14), a non-auto sweep advances (p13) */
  for (int ch = ac_track_first_channel(s, t), n = 0; ch >= 0 && n < AC_NCH; ch = ac_track_next_channel(s, ch), n++) {
    ac_exch_length_tick(s, ch);
    ac_sweep_update(s, ch, 0);
  }
  if (tr->flags & AC_TRF_NOTE_FINISH_WAIT) {
    if (tr->channel_list) return 0;
    tr->flags &= (u8)~AC_TRF_NOTE_FINISH_WAIT;
  }
  if (tr->wait > 0) {
    tr->wait--;
    if (tr->wait > 0) return 0;
  }
  for (int guard = 0; tr->wait == 0 && !(tr->flags & AC_TRF_NOTE_FINISH_WAIT) && guard < 100000; guard++) {
    if (run_cmd(&r, 1)) return 1;
  }
  return 0;
}
