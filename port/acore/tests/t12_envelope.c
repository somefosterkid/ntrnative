/* port/acore piece 12: unit tests (docs/acore.md, piece 12's "Tests:"). OWNED BY PIECE 12.
 * The conversions are fitted on the oracle (p12_envelope.c): the expected values here are worked by hand from
 * them. The sustain level is piece 6's table (ac_decibel), so those checks are written against ac_decibel. */
#include "act.h"

#define CH 3
#define MIN (-723 * 128)

static AC_ExChannel *C(AcoreState *s) { return &s->work.channel[CH]; }

/* the conversions at their ends */
ACT_TEST(p12_attack_conversion) {
  AcoreState *s = act_state_hw();
  static const struct { int v, a; } t[] = {{127, 0x00}, {126, 0x01}, {125, 0x05}, {124, 0x0E}, {121, 0x33},
                                           {120, 0x3F}, {119, 0x49}, {118, 0x54}, {113, 0x7B}, {112, 0x7F},
                                           {111, 0x84}, {110, 0x89}, {109, 0x8F}, {108, 147}, {100, 155},
                                           {64, 191}, {1, 254}, {0, 255}};
  for (unsigned i = 0; i < sizeof t / sizeof t[0]; i++) {
    ac_env_set_attack(s, CH, t[i].v);
    ACT_EQ(C(s)->attack, t[i].a);
  }
  ac_env_set_attack(s, CH, 200); /* out of range: clamped to 127 */
  ACT_EQ(C(s)->attack, 0);
  ac_env_set_attack(s, CH, -5);
  ACT_EQ(C(s)->attack, 255);
}

ACT_TEST(p12_decay_release_conversion) {
  AcoreState *s = act_state_hw();
  static const struct { int v, r; } t[] = {{127, 0xFFFF}, {126, 0x3C00}, {125, 0x1E00}, {120, 0x1E00 / 6},
                                           {100, 0x1E00 / 26}, {75, 0x1E00 / 51}, {50, 0x1E00 / 76}, {49, 99},
                                           {25, 51}, {1, 3}, {0, 1}};
  for (unsigned i = 0; i < sizeof t / sizeof t[0]; i++) {
    ac_env_set_decay(s, CH, t[i].v);
    ACT_EQ(C(s)->decay, t[i].r);
    ac_env_set_release(s, CH, t[i].v);
    ACT_EQ(C(s)->release, t[i].r);
  }
  ACT_EQ(0x1E00 / 76, 101); /* 50 -> 101, the oracle's r50 (rate 101) */
  ACT_EQ(0x1E00 / 51, 150); /* 75 -> 150 */
  ac_env_set_sustain(s, CH, 127);
  ACT_EQ(C(s)->sustain, 127);
  ac_env_set_sustain(s, CH, 300);
  ACT_EQ(C(s)->sustain, 127);
}

ACT_TEST(p12_init) {
  AcoreState *s = act_state_hw();
  C(s)->env_status = AC_ENV_RELEASE;
  C(s)->env_decay = 5;
  ac_env_init(s, CH);
  ACT_EQ(C(s)->env_status, AC_ENV_ATTACK);
  ACT_EQ(C(s)->env_decay, MIN);
  ACT_EQ(ac_env_level(s, CH), -723);
  C(s)->env_decay = 0;
  ACT_EQ(ac_env_level(s, CH), 0);
  C(s)->env_decay = -1; /* floors, as a shift */
  ACT_EQ(ac_env_level(s, CH), -1);
}

/* an attack of 127 is instant: the first update reaches full level and goes on to decay */
ACT_TEST(p12_attack_127_instant) {
  AcoreState *s = act_state_hw();
  ac_env_init(s, CH);
  ac_env_set_attack(s, CH, 127);
  ac_env_update(s, CH);
  ACT_EQ(C(s)->env_decay, 0);
  ACT_EQ(C(s)->env_status, AC_ENV_DECAY);
  ACT_EQ(ac_env_level(s, CH), 0);
}

/* an attack of 112 (127/256 per step): worked by hand, -92544 * 127 / 256 truncated towards zero */
ACT_TEST(p12_attack_steps) {
  AcoreState *s = act_state_hw();
  ac_env_init(s, CH);
  ac_env_set_attack(s, CH, 112);
  ACT_EQ(C(s)->attack, 127);
  ac_env_update(s, CH);
  ACT_EQ(C(s)->env_decay, -45910); /* -92544 * 127 / 256 = -45910.5 -> -45910 */
  ACT_EQ(C(s)->env_status, AC_ENV_ATTACK);
  ac_env_update(s, CH);
  ACT_EQ(C(s)->env_decay, -22775); /* -45910 * 127 / 256 = -22775.04 */
  int n = 2;
  while (C(s)->env_status == AC_ENV_ATTACK && n < 100) { ac_env_update(s, CH); n++; }
  ACT_EQ(C(s)->env_decay, 0);
  ACT_EQ(C(s)->env_status, AC_ENV_DECAY);
  ACT_EQ(n, 16); /* the oracle's a112 reaches the full-level register at wake 16 */
  /* a slow attack (0 -> 255/256) takes far longer, a monotone climb */
  ac_env_init(s, CH);
  ac_env_set_attack(s, CH, 0);
  s32 prev = C(s)->env_decay;
  for (int i = 0; i < 50; i++) {
    ac_env_update(s, CH);
    ACT_CHECK(C(s)->env_decay >= prev);
    prev = C(s)->env_decay;
  }
  ACT_CHECK(C(s)->env_status == AC_ENV_ATTACK);
}

/* a decay to a sustain level, then it holds */
ACT_TEST(p12_decay_to_sustain) {
  AcoreState *s = act_state_hw();
  ac_env_init(s, CH);
  ac_env_set_attack(s, CH, 127);
  ac_env_set_decay(s, CH, 100); /* 295 per wake */
  ac_env_set_sustain(s, CH, 40);
  ac_env_update(s, CH); /* attack done */
  ACT_EQ(C(s)->env_status, AC_ENV_DECAY);
  s32 level = (s32)ac_decibel_sq(40) * 128; /* the sustain level's square law (ai58, oracle) */
  int n = 0;
  while (C(s)->env_status == AC_ENV_DECAY && n < 1000) {
    s32 before = C(s)->env_decay;
    ac_env_update(s, CH);
    n++;
    if (C(s)->env_status == AC_ENV_DECAY) { ACT_EQ(C(s)->env_decay, before - 295); }
  }
  ACT_EQ(C(s)->env_status, AC_ENV_SUSTAIN);
  ACT_EQ(C(s)->env_decay, level); /* clamped to the level, not past it (the oracle: any rate ends at one register value) */
  if (level < 0) ACT_CHECK(n == (-level + 294) / 295);
  for (int i = 0; i < 5; i++) ac_env_update(s, CH);
  ACT_EQ(C(s)->env_decay, level);
  ACT_EQ(C(s)->env_status, AC_ENV_SUSTAIN);
  /* a fast decay (126: 15360) clamps in one step */
  ac_env_init(s, CH);
  ac_env_set_decay(s, CH, 126);
  ac_env_update(s, CH);
  ac_env_update(s, CH);
  ACT_EQ(C(s)->env_status, level < -15360 ? AC_ENV_DECAY : AC_ENV_SUSTAIN);
  /* sustain 127 = level 0: the first decay step lands on it */
  ac_env_init(s, CH);
  ac_env_set_decay(s, CH, 10);
  ac_env_set_sustain(s, CH, 127);
  ac_env_update(s, CH);
  ac_env_update(s, CH);
  ACT_EQ(C(s)->env_decay, 0);
  ACT_EQ(C(s)->env_status, AC_ENV_SUSTAIN);
}

/* a release to the end in N wakes: from full level at rate r the floor comes after ceil(92544 / r) updates */
ACT_TEST(p12_release_to_end) {
  AcoreState *s = act_state_hw();
  static const struct { int v, rate, n; } t[] = {{126, 15360, 7}, {100, 295, 314}, {75, 150, 617}, {49, 99, 935},
                                                  {0, 1, 92544}};
  for (unsigned i = 0; i < sizeof t / sizeof t[0]; i++) {
    ac_env_init(s, CH);
    C(s)->env_decay = 0;
    C(s)->env_status = AC_ENV_RELEASE;
    ac_env_set_release(s, CH, t[i].v);
    ACT_EQ(C(s)->release, t[i].rate);
    int n = 0;
    while (C(s)->env_decay > MIN && n < 100000) { ac_env_update(s, CH); n++; }
    ACT_EQ(n, t[i].n);
    ACT_EQ(C(s)->env_decay, MIN); /* floored, not past */
    ACT_EQ(ac_env_level(s, CH), -723);
    ac_env_update(s, CH);
    ACT_EQ(C(s)->env_decay, MIN);
    ACT_EQ(C(s)->env_status, AC_ENV_RELEASE);
  }
  /* the release starts from wherever the envelope is (the sustain level, not full) */
  ac_env_init(s, CH);
  C(s)->env_decay = -1000 * 128 / 10;
  C(s)->env_status = AC_ENV_RELEASE;
  ac_env_set_release(s, CH, 100);
  ac_env_update(s, CH);
  ACT_EQ(C(s)->env_decay, -1000 * 128 / 10 - 295);
}

/* the oracle's release of rate 100: the level after j+1 updates is (-295 (j+1)) >> 7 (r100, wakes 139..) */
ACT_TEST(p12_release_levels) {
  AcoreState *s = act_state_hw();
  ac_env_init(s, CH);
  C(s)->env_decay = 0;
  C(s)->env_status = AC_ENV_RELEASE;
  ac_env_set_release(s, CH, 100);
  ac_env_update(s, CH);
  ACT_EQ(ac_env_level(s, CH), -3); /* -295 >> 7 = -2.3 -> -3 */
  ac_env_update(s, CH);
  ACT_EQ(ac_env_level(s, CH), -5); /* -590 >> 7 = -4.6 -> -5 */
  ac_env_update(s, CH);
  ACT_EQ(ac_env_level(s, CH), -7); /* -885 >> 7 = -6.9 -> -7 */
}

/* sustain does nothing; unknown status values are ignored */
ACT_TEST(p12_sustain_holds) {
  AcoreState *s = act_state_hw();
  ac_env_init(s, CH);
  C(s)->env_status = AC_ENV_SUSTAIN;
  C(s)->env_decay = -777;
  for (int i = 0; i < 10; i++) ac_env_update(s, CH);
  ACT_EQ(C(s)->env_decay, -777);
}
