/* port/acore piece 7: unit tests (docs/acore.md, piece 7's "Tests:"). OWNED BY PIECE 7. */
#include "act.h"
#include <string.h>

#define SEQ 0x02100000u

/* a state with track 0 of player 0 active on the bytes b[0..n) */
static AcoreState *mk(const u8 *b, int n) {
  AcoreState *s = act_state();
  act_mem(s, SEQ, b, (u32)n);
  AC_Track *t = &s->work.track[0];
  memset(t, 0, sizeof *t);
  t->flags = AC_TRF_ACTIVE | AC_TRF_NOTE_WAIT;
  t->base = SEQ;
  t->cur = SEQ;
  return s;
}
#define TR(s) (&(s)->work.track[0])
static int ticks(AcoreState *s, int n) {
  int e = 0;
  for (int i = 0; i < n; i++) e = ac_track_tick(s, 0, 0);
  return e;
}
/* piece 16's variables work? */
static int vars_ok(AcoreState *s) {
  ac_var_set(s, 0, 15, 1234);
  return ac_var_get(s, 0, 15) == 1234;
}

ACT_TEST(p07_wait_exact_ticks) {
  const u8 q[] = {0x80, 3, 0x80, 5, 0xFF};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 3);
  ACT_EQ(TR(s)->cur, SEQ + 2);
  ticks(s, 2);
  ACT_EQ(TR(s)->wait, 1);
  ACT_EQ(TR(s)->cur, SEQ + 2); /* still counting */
  ticks(s, 1);                 /* wait reaches 0: the next command runs in the same tick */
  ACT_EQ(TR(s)->wait, 5);
  ACT_EQ(TR(s)->cur, SEQ + 4);
}

ACT_TEST(p07_vlq_lengths) {
  const struct { u8 b[6]; int n; s32 v; } c[] = {
    {{0x80, 0x7F}, 2, 0x7F},
    {{0x80, 0x81, 0x00}, 3, 0x80},
    {{0x80, 0x81, 0x80, 0x00}, 4, 0x4000},
    {{0x80, 0xFF, 0xFF, 0xFF, 0x7F}, 5, 0x0FFFFFFF},
    {{0x80, 0x82, 0x2C}, 3, 0x12C},
  };
  for (unsigned i = 0; i < sizeof c / sizeof c[0]; i++) {
    AcoreState *s = mk(c[i].b, c[i].n);
    ticks(s, 1);
    ACT_EQ(TR(s)->wait, c[i].v);
    ACT_EQ(TR(s)->cur, SEQ + (u32)c[i].n);
  }
}

ACT_TEST(p07_note_wait) {
  const u8 q[] = {60, 100, 24, 0xFF};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 24);
  ACT_EQ(TR(s)->cur, SEQ + 3);
  /* porta_key: set by p10's ac_porta_setup when the note gets a channel (no bank in this test) */
  const u8 q2[] = {64, 90, 0x81, 0x40, 0xFF}; /* two-byte length */
  s = mk(q2, sizeof q2);
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 0xC0);
  ACT_EQ(TR(s)->cur, SEQ + 4);
  const u8 q3[] = {60, 100, 24, 0x80, 7, 0xFF}; /* note-wait off: the next command runs at once */
  s = mk(q3, sizeof q3);
  TR(s)->flags &= (u8)~AC_TRF_NOTE_WAIT;
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 7);
  ACT_EQ(TR(s)->cur, SEQ + 5);
}

ACT_TEST(p07_note_zero_length_finish_wait) {
  const u8 q[] = {60, 100, 0, 0x80, 9, 0xFF};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_CHECK(TR(s)->flags & AC_TRF_NOTE_FINISH_WAIT);
  ACT_EQ(TR(s)->cur, SEQ + 3);
  TR(s)->channel_list = 0x1234; /* a channel still sounding: the track holds */
  ticks(s, 3);
  ACT_EQ(TR(s)->cur, SEQ + 3);
  TR(s)->channel_list = 0;
  ticks(s, 1);
  ACT_CHECK(!(TR(s)->flags & AC_TRF_NOTE_FINISH_WAIT));
  ACT_EQ(TR(s)->wait, 9);
}

ACT_TEST(p07_program) {
  const u8 q[] = {0x81, 0x85, 0x01, 0xFF}; /* VLQ 0x281 */
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->prgNo, 0x281);
  ACT_EQ(TR(s)->cur, SEQ + 4);
  ACT_EQ(ac_track_tick(s, 0, 0), 1);
}

ACT_TEST(p07_end_of_track) {
  const u8 q[] = {0xFF, 0x80, 4};
  AcoreState *s = mk(q, sizeof q);
  ACT_EQ(ticks(s, 1), 1);
  ACT_CHECK(!(TR(s)->flags & AC_TRF_ACTIVE));
  ACT_EQ(TR(s)->wait, 0);
  ACT_EQ(ticks(s, 1), 1); /* stays ended */
}

ACT_TEST(p07_tracks_used) {
  const u8 q[] = {0xFE, 0x03, 0x00, 0x80, 2, 0xFF};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->cur, SEQ + 5);
  ACT_EQ(TR(s)->wait, 2);
}

ACT_TEST(p07_jump) {
  const u8 q[] = {0x94, 0x06, 0x00, 0x00, 0x80, 1, 0x80, 5, 0xFF};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 5);
  ACT_EQ(TR(s)->cur, SEQ + 8);
  const u8 q2[] = {0x80, 2, 0x94, 0x00, 0x00, 0x00}; /* a backward jump loops */
  s = mk(q2, sizeof q2);
  ticks(s, 1);
  ticks(s, 2); /* wait ends: jump, wait again */
  ACT_EQ(TR(s)->cur, SEQ + 2);
  ACT_EQ(TR(s)->wait, 2);
}

ACT_TEST(p07_call_return) {
  /* 0: call 8; 4: wait 3; 6: end; 8: wait 2; 10: return */
  const u8 q[] = {0x95, 8, 0, 0, 0x80, 3, 0xFF, 0, 0x80, 2, 0xFD};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->call_stack_depth, 1);
  ACT_EQ(TR(s)->call_stack[0], SEQ + 4);
  ACT_EQ(TR(s)->wait, 2);
  ACT_EQ(TR(s)->cur, SEQ + 10);
  ticks(s, 2); /* return, then wait 3 */
  ACT_EQ(TR(s)->call_stack_depth, 0);
  ACT_EQ(TR(s)->wait, 3);
  ACT_EQ(TR(s)->cur, SEQ + 6);
}

ACT_TEST(p07_return_without_call) {
  const u8 q[] = {0xFD, 0x80, 4};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 4);
}

ACT_TEST(p07_nested_calls_depth3) {
  /* 0: call A(0x10); 4: wait 1. A: call B(0x20); wait 2; ret. B: call C(0x30); ret. C: wait 9; ret */
  u8 q[0x40] = {0};
  q[0] = 0x95; q[1] = 0x10;
  q[4] = 0x80; q[5] = 1;
  q[6] = 0xFF;
  q[0x10] = 0x95; q[0x11] = 0x20; q[0x14] = 0x80; q[0x15] = 2; q[0x16] = 0xFD;
  q[0x20] = 0x95; q[0x21] = 0x30; q[0x24] = 0xFD;
  q[0x30] = 0x80; q[0x31] = 9; q[0x32] = 0xFD;
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->call_stack_depth, 3);
  ACT_EQ(TR(s)->call_stack[0], SEQ + 4);
  ACT_EQ(TR(s)->call_stack[1], SEQ + 0x14);
  ACT_EQ(TR(s)->call_stack[2], SEQ + 0x24);
  ACT_EQ(TR(s)->wait, 9);
  ticks(s, 9); /* C returns to B, B returns to A, A waits 2 */
  ACT_EQ(TR(s)->call_stack_depth, 1);
  ACT_EQ(TR(s)->wait, 2);
  ticks(s, 2);
  ACT_EQ(TR(s)->call_stack_depth, 0);
  ACT_EQ(TR(s)->wait, 1);
  ACT_EQ(TR(s)->cur, SEQ + 6);
}

ACT_TEST(p07_call_overflow_ignored) {
  /* a call with the stack full is not taken: it falls through */
  const u8 q[] = {0x95, 0x20, 0, 0, 0x80, 6, 0xFF};
  AcoreState *s = mk(q, sizeof q);
  TR(s)->call_stack_depth = 3;
  ticks(s, 1);
  ACT_EQ(TR(s)->call_stack_depth, 3);
  ACT_EQ(TR(s)->wait, 6);
  ACT_EQ(TR(s)->cur, SEQ + 6);
}

ACT_TEST(p07_loop_counted) {
  const u8 q[] = {0xD4, 3, 0x80, 2, 0xFC, 0x80, 7, 0xFF};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->call_stack_depth, 1);
  ACT_EQ(TR(s)->loop_count[0], 3);
  ACT_EQ(TR(s)->wait, 2);
  ticks(s, 2); /* end marker: count 3 -> 2, back to the wait */
  ACT_EQ(TR(s)->loop_count[0], 2);
  ACT_EQ(TR(s)->cur, SEQ + 4);
  ACT_EQ(TR(s)->wait, 2);
  ticks(s, 2);
  ACT_EQ(TR(s)->loop_count[0], 1);
  ticks(s, 2); /* third pass ends: count 0, leave the loop */
  ACT_EQ(TR(s)->call_stack_depth, 0);
  ACT_EQ(TR(s)->wait, 7);
  ACT_EQ(TR(s)->cur, SEQ + 7);
}

ACT_TEST(p07_loop_infinite) {
  const u8 q[] = {0xD4, 0, 0x80, 1, 0xFC, 0xFF};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 50);
  ACT_EQ(TR(s)->call_stack_depth, 1);
  ACT_EQ(TR(s)->loop_count[0], 0);
  ACT_CHECK(TR(s)->flags & AC_TRF_ACTIVE);
}

ACT_TEST(p07_loop_nested_with_call) {
  /* loop 2 { call F } ; wait 8; F at 12: wait 1; ret */
  const u8 q[] = {0xD4, 2, 0x95, 12, 0, 0, 0xFC, 0x80, 8, 0xFF, 0, 0, 0x80, 1, 0xFD};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->call_stack_depth, 2);
  ticks(s, 1); /* ret, loop end -> back, call again, wait */
  ACT_EQ(TR(s)->call_stack_depth, 2);
  ACT_EQ(TR(s)->loop_count[0], 1);
  ticks(s, 1);
  ACT_EQ(TR(s)->call_stack_depth, 0);
  ACT_EQ(TR(s)->wait, 8);
}

ACT_TEST(p07_loop_overflow) {
  const u8 q[] = {0xD4, 5, 0x80, 4, 0xFF};
  AcoreState *s = mk(q, sizeof q);
  TR(s)->call_stack_depth = 3;
  ticks(s, 1);
  ACT_EQ(TR(s)->call_stack_depth, 3);
  ACT_EQ(TR(s)->wait, 4);
}

ACT_TEST(p07_variables) {
  AcoreState *s = act_state();
  if (!vars_ok(s)) ACT_TODO("variables read back: needs piece 16's ac_var_get/set");
  const u8 q[] = {0xB0, 3, 100, 0, 0xB1, 3, 20, 0, 0xB2, 3, 5, 0, 0xB3, 3, 2, 0, 0xB4, 3, 3, 0,
                  0xB5, 3, 2, 0, 0xB5, 3, 0xFF, 0xFF, 0x80, 1, 0xFF};
  s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(ac_var_get(s, 0, 3), ((((100 + 20 - 5) * 2) / 3) << 2) >> 1);
  const u8 q2[] = {0xB0, 18, 0x34, 0x12, 0xB1, 18, 0xFF, 0xFF, 0x80, 1}; /* global variable 16+2 */
  s = mk(q2, sizeof q2);
  ticks(s, 1);
  ACT_EQ(ac_var_get(s, -1, 2), 0x1233);
  const u8 q3[] = {0xB0, 1, 9, 0, 0xB4, 1, 0, 0, 0x80, 1}; /* division by zero leaves the variable */
  s = mk(q3, sizeof q3);
  ticks(s, 1);
  ACT_EQ(ac_var_get(s, 0, 1), 9);
  const u8 q4[] = {0xB0, 1, 0xFF, 0x7F, 0xB1, 1, 1, 0, 0x80, 1}; /* 16 bit wrap */
  s = mk(q4, sizeof q4);
  ticks(s, 1);
  ACT_EQ(ac_var_get(s, 0, 1), -32768);
}

ACT_TEST(p07_compare_and_if) {
  AcoreState *s = act_state();
  if (!vars_ok(s)) ACT_TODO("variables: needs piece 16's ac_var_get/set");
  /* v0 = 5; a compare, then "if wait 7": the wait is set only when the flag is */
  static const struct { u8 op; s16 val; int flag; } c[] = {
    {0xB8, 5, 1}, {0xB8, 4, 0}, {0xB9, 5, 1}, {0xB9, 6, 0}, {0xBA, 4, 1}, {0xBA, 5, 0},
    {0xBB, 5, 1}, {0xBB, 4, 0}, {0xBC, 6, 1}, {0xBC, 5, 0}, {0xBD, 4, 1}, {0xBD, 5, 0},
  };
  for (unsigned i = 0; i < sizeof c / sizeof c[0]; i++) {
    const u8 q[] = {0xB0, 0, 5, 0, c[i].op, 0, (u8)c[i].val, (u8)(c[i].val >> 8), 0xA2, 0x80, 7, 0xFF};
    s = mk(q, sizeof q);
    ticks(s, 1);
    ACT_EQ(!!(TR(s)->flags & AC_TRF_CMP), c[i].flag);
    ACT_EQ(TR(s)->wait, c[i].flag ? 7 : 0);
  }
}

ACT_TEST(p07_if_prefix_skips_arguments) {
  /* flag clear: "if" skips a note (3 bytes), a jump (4), a wait (2), a param (2), an end, then wait 3 runs */
  const u8 q[] = {0xA2, 60, 100, 24, 0xA2, 0x94, 0, 0, 0, 0xA2, 0x80, 9, 0xA2, 0xC1, 50, 0xA2, 0xFF, 0x80, 3};
  AcoreState *s = mk(q, sizeof q);
  ACT_EQ(ticks(s, 1), 0);
  ACT_EQ(TR(s)->wait, 3);
  ACT_CHECK(TR(s)->flags & AC_TRF_ACTIVE);
  /* skipped with a variable prefix and a random prefix (their extra bytes are consumed too) */
  const u8 q2[] = {0xA2, 0xA1, 0x80, 4, 0xA2, 0xA0, 0x80, 1, 0, 9, 0, 0x80, 2};
  s = mk(q2, sizeof q2);
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 2);
  ACT_EQ(TR(s)->cur, SEQ + 13);
  const u8 q3[] = {0xA2, 0x80, 5}; /* flag set: it runs */
  s = mk(q3, sizeof q3);
  TR(s)->flags |= AC_TRF_CMP;
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 5);
}

ACT_TEST(p07_variable_prefix) {
  AcoreState *s = act_state();
  if (!vars_ok(s)) ACT_TODO("variables: needs piece 16's ac_var_get/set");
  const u8 q[] = {0xB0, 4, 0x2C, 0x01, 0xA1, 0x80, 4, 0xFF}; /* wait (v4) */
  s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 300);
  ACT_EQ(TR(s)->cur, SEQ + 7);
  const u8 q2[] = {0xB0, 4, 12, 0, 0xA1, 60, 100, 4, 0xFF}; /* a note's length from a variable */
  s = mk(q2, sizeof q2);
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 12);
  ACT_EQ(TR(s)->cur, SEQ + 8);
  const u8 q3[] = {0xB0, 4, 12, 0, 0xB0, 5, 3, 0, 0xA1, 0xB1, 5, 4, 0x80, 1}; /* v5 += v4 */
  s = mk(q3, sizeof q3);
  ticks(s, 1);
  ACT_EQ(ac_var_get(s, 0, 5), 15);
}

ACT_TEST(p07_random_prefix) {
  AcoreState *s = act_state();
  if (!vars_ok(s)) ACT_TODO("variables: needs piece 16's ac_var_get/set");
  const u8 q[] = {0xA0, 0x80, 10, 0, 20, 0, 0xFF}; /* wait random in [10, 20] */
  for (int i = 0; i < 50; i++) {
    s = mk(q, sizeof q);
    for (int k = 0; k < i; k++) ac_random(s);
    ticks(s, 1);
    ACT_CHECK(TR(s)->wait >= 10 && TR(s)->wait <= 20);
    ACT_EQ(TR(s)->cur, SEQ + 6);
  }
  const u8 q2[] = {0xA0, 0x80, 7, 0, 7, 0, 0xFF}; /* degenerate range */
  s = mk(q2, sizeof q2);
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 7);
  const u8 q3[] = {0xB6, 2, 9, 0, 0xB6, 3, 0xF7, 0xFF, 0x80, 1}; /* random op, positive and negative bound */
  s = mk(q3, sizeof q3);
  ticks(s, 1);
  ACT_CHECK(ac_var_get(s, 0, 2) >= 0 && ac_var_get(s, 0, 2) <= 9);
  ACT_CHECK(ac_var_get(s, 0, 3) <= 0 && ac_var_get(s, 0, 3) >= -9);
}

ACT_TEST(p07_param_commands_to_8) {
  const u8 q[] = {0xC1, 77, 0x80, 1};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->cur, SEQ + 4); /* the argument is consumed whatever 8 does */
  if (TR(s)->volume != 77) ACT_TODO("parameter commands: needs piece 8's ac_track_param");
}

ACT_TEST(p07_param_args_consumed) {
  /* every 8-bit command C0-D6 except D4 takes one byte; E0-E3 two */
  u8 q[64], n = 0;
  for (int c = 0xC0; c <= 0xD6; c++) {
    if (c == 0xD4) continue;
    q[n++] = (u8)c; q[n++] = 1;
  }
  for (int c = 0xE0; c <= 0xE3; c++) { q[n++] = (u8)c; q[n++] = 1; q[n++] = 0; }
  q[n++] = 0x80; q[n++] = 3;
  AcoreState *s = mk(q, n);
  ticks(s, 1);
  ACT_EQ(TR(s)->wait, 3);
  ACT_EQ(TR(s)->cur, SEQ + n);
}

ACT_TEST(p07_open_track_consumes) {
  const u8 q[] = {0xFE, 0x03, 0x00, 0x93, 1, 0x10, 0, 0, 0x80, 4};
  AcoreState *s = mk(q, sizeof q);
  ticks(s, 1);
  ACT_EQ(TR(s)->cur, SEQ + 10);
  ACT_EQ(TR(s)->wait, 4);
}
