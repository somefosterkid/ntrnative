/* port/acore's unit-test framework and stream builders (docs/acore.md, "Unit tests"). OWNED BY THE SKELETON.
 *
 *   ACT_TEST(p01_pcm16_one_per_sample) {        // the name starts with the piece's pNN_
 *     AcoreState *s = act_state_hw();           // a hardware-only state (no driver wakes), freed after the test
 *     act_mem(s, 0x02100000, ramp, sizeof ramp);
 *     act_reg(s, 0x04000400 + AC_CH_SAD, 0x02100000, 4);
 *     ...
 *     act_run(s, 10 * AC_CYCLES_PER_SAMPLE);
 *     ACT_EQ(act_sample(s, 4, 0), ...);
 *     act_emit(s, "p01_pcm16_ramp");            // with ACT_EMIT=DIR: DIR/p01_pcm16_ramp.acs for the oracle
 *   }
 *
 * Every act_mem / act_reg / act_flush / act_run both acts on the state and records the event, so a test is also a
 * stream: act_emit writes it (INFO, MEM, REGS, CMDS + WAKE, END) for the oracle and tools/acore_compare.py.
 */
#ifndef ACT_H
#define ACT_H
#include "ac_shared.h"
#include "acore.h"
#include <stdio.h>

typedef void (*act_fn)(void);
void act_register(const char *name, const char *file, act_fn fn);
void act_fail(const char *file, int line, const char *fmt, ...);
void act_todo(const char *msg);

#define ACT_TEST(name)                                                                                                \
  static void name(void);                                                                                             \
  __attribute__((constructor)) static void name##_act_register(void) { act_register(#name, __FILE__, name); }         \
  static void name(void)
#define ACT_CHECK(cond)                                                                                               \
  do {                                                                                                                \
    if (!(cond)) {                                                                                                    \
      act_fail(__FILE__, __LINE__, "%s", #cond);                                                                      \
      return;                                                                                                         \
    }                                                                                                                 \
  } while (0)
#define ACT_EQ(a, b)                                                                                                  \
  do {                                                                                                                \
    long long act_a_ = (long long)(a), act_b_ = (long long)(b);                                                       \
    if (act_a_ != act_b_) {                                                                                           \
      act_fail(__FILE__, __LINE__, "%s == %s: %lld (0x%llx) != %lld (0x%llx)", #a, #b, act_a_, act_a_, act_b_,       \
               act_b_);                                                                                               \
      return;                                                                                                         \
    }                                                                                                                 \
  } while (0)
#define ACT_TODO(msg)                                                                                                 \
  do {                                                                                                                \
    act_todo(msg);                                                                                                    \
    return;                                                                                                           \
  } while (0)

/* ---- states (freed when the test ends) ---- */
AcoreState *act_state(void);    /* the driver on, as SND_Init leaves it (ac_driver_init), the periodic wakes from 0 */
AcoreState *act_state_hw(void); /* hardware only: no driver wakes (mode=hw) */

/* ---- recorded actions (at s->now) ---- */
void act_mem(AcoreState *s, u32 addr, const void *data, u32 n);   /* write main memory */
void act_reg(AcoreState *s, u32 addr, u32 value, int size);       /* a register write (0x04000400... or 0x400...) */
void act_cmd(AcoreState *s, u32 id, u32 a0, u32 a1, u32 a2, u32 a3); /* add a command to the pending list */
void act_flush(AcoreState *s); /* the pending list sent + a wakeup: queued and the driver woken now */
void act_send(AcoreState *s);  /* the pending list sent without a wakeup (runs at the next periodic wake) */
void act_run(AcoreState *s, u64 cycles); /* run on by cycles (events, wakes, samples) */
/* output sample k (from the start), channel 0 = L, 1 = R; 0 when not rendered yet */
s16 act_sample(const AcoreState *s, u64 k, int lr);
/* the DS output value 0-0x3FF of a sample: (s16 + 0x8000) >> 6 */
static inline int act_dac(s16 v) { return ((int)v + 0x8000) >> 6; }

/* with ACT_EMIT=DIR set: writes the recorded stream as DIR/name.acs (a no-op otherwise). Name it pNN_... */
void act_emit(const AcoreState *s, const char *name);
/* a scratch path for this run */
const char *act_tmp(const char *name);

#endif
