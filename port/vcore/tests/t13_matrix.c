/* port/vcore piece 13's unit tests: the matrix engine and the command processor (docs/vcore.md, piece 13). OWNED BY PIECE 13.
 * Run: build/vcore/vcore_tests p13. Expected values are worked by hand from GBATEK 36-39, 42. */
#include "vct.h"

#define ONE 0x1000

static VCoreState *fresh(void) {
  VCoreState *s = vct_state();
  vc_mtx_reset(s);
  return s;
}
static void cmd(VCoreState *s, u32 op, u32 n, const u32 *p) { vc_gx_command(s, op, p, n); }
static void mode(VCoreState *s, u32 m) { cmd(s, VC_OP_MTX_MODE, 1, &m); }
static int is_ident(const VC_Mtx *m) {
  for (int i = 0; i < 16; i++)
    if (m->m[i] != ((i % 5 == 0) ? ONE : 0)) return 0;
  return 1;
}
static void trans(VCoreState *s, s32 x, s32 y, s32 z) {
  u32 p[3] = {(u32)x, (u32)y, (u32)z};
  cmd(s, VC_OP_MTX_TRANS, 3, p);
}

VCT_TEST(p13_reset_identity) {
  VCoreState *s = fresh();
  VCT_CHECK(is_ident(vc_mtx_clip(s)) && is_ident(vc_mtx_pos(s)) && is_ident(vc_mtx_vec(s)));
  VCT_CHECK(is_ident(vc_mtx_tex(s)) && is_ident(vc_mtx_proj(s)));
  VCT_EQ(s->p13.sp, 0);
  VCT_EQ(s->p13.stack_error, 0);
}

VCT_TEST(p13_mode_and_load_4x4) {
  VCoreState *s = fresh();
  u32 p[16];
  for (int i = 0; i < 16; i++) p[i] = (u32)(i * 0x100 - 0x300);
  mode(s, 1);
  cmd(s, VC_OP_MTX_LOAD_4x4, 16, p);
  for (int i = 0; i < 16; i++) VCT_EQ(vc_mtx_pos(s)->m[i], i * 0x100 - 0x300);
  VCT_CHECK(is_ident(vc_mtx_proj(s)) && is_ident(vc_mtx_vec(s))); /* mode 1 leaves vec alone */
  /* clip = pos x identity projection */
  for (int i = 0; i < 16; i++) VCT_EQ(vc_mtx_clip(s)->m[i], i * 0x100 - 0x300);
  mode(s, 0);
  cmd(s, VC_OP_MTX_LOAD_4x4, 16, p);
  for (int i = 0; i < 16; i++) VCT_EQ(vc_mtx_proj(s)->m[i], i * 0x100 - 0x300);
  mode(s, 3);
  cmd(s, VC_OP_MTX_LOAD_4x4, 16, p);
  VCT_EQ(vc_mtx_tex(s)->m[5], 5 * 0x100 - 0x300);
  mode(s, 0x1D); /* only bits 0-1 count: mode 1 */
  VCT_EQ(s->p13.mode, 1);
}

VCT_TEST(p13_load_4x3) {
  VCoreState *s = fresh();
  u32 p[12];
  for (int i = 0; i < 12; i++) p[i] = (u32)(i + 1);
  mode(s, 1);
  cmd(s, VC_OP_MTX_LOAD_4x3, 12, p);
  /* rows: {1,2,3,0} {4,5,6,0} {7,8,9,0} {10,11,12,1.0} */
  const s32 e[16] = {1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9, 0, 10, 11, 12, ONE};
  for (int i = 0; i < 16; i++) VCT_EQ(vc_mtx_pos(s)->m[i], e[i]);
}

VCT_TEST(p13_mult_order_and_3x3) {
  VCoreState *s = fresh();
  mode(s, 1);
  /* C = M * C: translate by (1,0,0) first, then scale by 2 in the matrix sense C = S * T */
  trans(s, ONE, 0, 0);
  u32 sc[3] = {2 * ONE, 3 * ONE, 4 * ONE};
  cmd(s, VC_OP_MTX_SCALE, 3, sc);
  /* S * T: row 3 of T is (1,0,0,1); S's rows are diagonal, so row 3 stays (1,0,0,1); row 0 = 2,0,0,0 */
  const VC_Mtx *m = vc_mtx_pos(s);
  VCT_EQ(m->m[0], 2 * ONE);
  VCT_EQ(m->m[5], 3 * ONE);
  VCT_EQ(m->m[10], 4 * ONE);
  VCT_EQ(m->m[12], ONE); /* v * S * T: scale first, then translate: (0,0,0,1) -> x = 1 */
  VCT_EQ(m->m[15], ONE);
  /* 3x3 mult by a 90 degree turn: rows {0,1,0} {-1,0,0} {0,0,1}, applied to the identity */
  VCoreState *s2 = fresh();
  mode(s2, 1);
  u32 r[9] = {0, ONE, 0, (u32)-ONE, 0, 0, 0, 0, ONE};
  cmd(s2, VC_OP_MTX_MULT_3x3, 9, r);
  VCT_EQ(vc_mtx_pos(s2)->m[1], ONE);
  VCT_EQ(vc_mtx_pos(s2)->m[4], -ONE);
  VCT_EQ(vc_mtx_pos(s2)->m[10], ONE);
  VCT_EQ(vc_mtx_pos(s2)->m[15], ONE);
  VCT_EQ(vc_mtx_pos(s2)->m[12], 0);
  /* M * C with C = translation (3,0,0): the 3x3 rotates the translation row: (3,0,0,1)*R -> (0,3,0,1)? no:
   * row 3 of M is (0,0,0,1), so (M*C) row 3 = row 3 of C = (3,0,0,1); rows 0-2 become R * C's rows */
  trans(s2, 3 * ONE, 0, 0); /* C = T * R: row 3 = (3,0,0,1) * R's... */
  /* T has rows identity + (3,0,0,1); (T*R) row 3 = (3,0,0,1)*R = 3*R.row0 + R.row3 = (0,3,0,1) */
  VCT_EQ(vc_mtx_pos(s2)->m[12], 0);
  VCT_EQ(vc_mtx_pos(s2)->m[13], 3 * ONE);
  VCT_EQ(vc_mtx_pos(s2)->m[15], ONE);
}

VCT_TEST(p13_mult_4x4_4x3_rounding) {
  VCoreState *s = fresh();
  mode(s, 1);
  u32 a[16] = {0};
  a[0] = 0x1800; a[5] = 0x800; a[10] = ONE; a[15] = ONE; /* 1.5, 0.5 */
  cmd(s, VC_OP_MTX_LOAD_4x4, 16, a);
  u32 b[16] = {0};
  b[0] = 0x1800; b[5] = 0x1001; b[10] = ONE; b[15] = ONE; /* 1.5, 1.000244 */
  cmd(s, VC_OP_MTX_MULT_4x4, 16, b);
  /* M*C: 0x1800*0x1800 >> 12 = 0x2400 (2.25); 0x1001*0x800 >> 12 = 0x800 (0x800800 >> 12 = 0x800, floor) */
  VCT_EQ(vc_mtx_pos(s)->m[0], 0x2400);
  VCT_EQ(vc_mtx_pos(s)->m[5], 0x800);
  /* negative values floor (arithmetic shift): -1 * 1 >> 12 = -1 */
  u32 c[16] = {0};
  c[0] = (u32)-1; c[5] = ONE; c[10] = ONE; c[15] = ONE;
  cmd(s, VC_OP_MTX_LOAD_4x4, 16, c);
  u32 d[12] = {1, 0, 0, 0, ONE, 0, 0, 0, ONE, 0, 0, 0};
  cmd(s, VC_OP_MTX_MULT_4x3, 12, d); /* M = diag(1/4096, 1, 1), C[0][0] = -1/4096: product = -1 >> 12 = -1 */
  VCT_EQ(vc_mtx_pos(s)->m[0], -1);
  VCT_EQ(vc_mtx_pos(s)->m[15], ONE);
}

VCT_TEST(p13_64bit_products) {
  s32 in[4] = {0x7FFFFFFF, 0, 0, 0};
  VC_Mtx m;
  for (int i = 0; i < 16; i++) m.m[i] = 0;
  m.m[0] = 0x7FFFFFFF;
  s32 out[4];
  vc_mtx_apply(&m, in, out);
  /* (2^31-1)^2 >> 12 = (2^62 - 2^32 + 1) >> 12 = 2^50 - 2^20, truncated to 32 bits = 0xFFF00000 */
  VCT_EQ((u32)out[0], 0xFFF00000u);
  /* v * M: out[j] = sum_k in[k] * m[k*4 + j] */
  for (int i = 0; i < 16; i++) m.m[i] = 0;
  m.m[1] = 2 * ONE;       /* in[0] -> out[1] */
  m.m[4 * 3 + 0] = ONE;   /* in[3] -> out[0] */
  s32 in2[4] = {ONE, 0, 0, ONE};
  vc_mtx_apply(&m, in2, out);
  VCT_EQ(out[0], ONE);
  VCT_EQ(out[1], 2 * ONE);
  VCT_EQ(out[2], 0);
  /* in place */
  vc_mtx_apply(&m, in2, in2);
  VCT_EQ(in2[1], 2 * ONE);
}

VCT_TEST(p13_mode2_both_scale_only_pos) {
  VCoreState *s = fresh();
  mode(s, 2);
  trans(s, ONE, 2 * ONE, 3 * ONE);
  VCT_EQ(vc_mtx_pos(s)->m[12], ONE);
  VCT_EQ(vc_mtx_vec(s)->m[12], ONE); /* translate acts on both */
  u32 sc[3] = {2 * ONE, 2 * ONE, 2 * ONE};
  cmd(s, VC_OP_MTX_SCALE, 3, sc);
  VCT_EQ(vc_mtx_pos(s)->m[0], 2 * ONE);
  VCT_EQ(vc_mtx_vec(s)->m[0], ONE); /* the vector matrix is not scaled */
  /* identity and load go to both */
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  VCT_CHECK(is_ident(vc_mtx_pos(s)) && is_ident(vc_mtx_vec(s)));
  u32 p[12] = {2 * ONE, 0, 0, 0, 2 * ONE, 0, 0, 0, 2 * ONE, 5, 6, 7};
  cmd(s, VC_OP_MTX_LOAD_4x3, 12, p);
  VCT_EQ(vc_mtx_pos(s)->m[12], 5);
  VCT_EQ(vc_mtx_vec(s)->m[13], 6);
  /* mode 1 leaves the vector matrix, mode 1 scale too */
  mode(s, 1);
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  VCT_CHECK(is_ident(vc_mtx_pos(s)));
  VCT_EQ(vc_mtx_vec(s)->m[12], 5);
}

VCT_TEST(p13_clip_is_pos_times_proj) {
  VCoreState *s = fresh();
  mode(s, 0);
  u32 sc[3] = {2 * ONE, ONE, ONE};
  cmd(s, VC_OP_MTX_SCALE, 3, sc); /* proj = diag(2,1,1,1) */
  VCT_EQ(vc_mtx_clip(s)->m[0], 2 * ONE);
  mode(s, 1);
  trans(s, ONE, 0, 0); /* pos translation row (1,0,0,1) */
  /* clip = pos * proj: row 3 = (1,0,0,1) * diag(2,1,1,1) = (2,0,0,1) */
  VCT_EQ(vc_mtx_clip(s)->m[12], 2 * ONE);
  VCT_EQ(vc_mtx_clip(s)->m[15], ONE);
  /* a texture matrix change doesn't touch the clip matrix */
  mode(s, 3);
  trans(s, 5 * ONE, 0, 0);
  VCT_EQ(vc_mtx_clip(s)->m[12], 2 * ONE);
  VCT_EQ(vc_mtx_tex(s)->m[12], 5 * ONE);
}

VCT_TEST(p13_push_pop_pos_and_vec) {
  VCoreState *s = fresh();
  mode(s, 1);
  trans(s, ONE, 0, 0);
  cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  VCT_EQ(s->p13.sp, 1);
  trans(s, ONE, 0, 0);
  VCT_EQ(vc_mtx_pos(s)->m[12], 2 * ONE);
  u32 one = 1;
  cmd(s, VC_OP_MTX_POP, 1, &one);
  VCT_EQ(s->p13.sp, 0);
  VCT_EQ(vc_mtx_pos(s)->m[12], ONE);
  VCT_EQ(vc_mtx_clip(s)->m[12], ONE);
  /* mode 1 stack ops use both: put something in vec via mode 2 */
  mode(s, 2);
  trans(s, 0, 7, 0);
  mode(s, 1);
  cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  mode(s, 2);
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  mode(s, 1);
  cmd(s, VC_OP_MTX_POP, 1, &one);
  VCT_EQ(vc_mtx_vec(s)->m[13], 7);
  VCT_EQ(vc_mtx_pos(s)->m[13], 7);
  VCT_EQ(s->p13.stack_error, 0);
}

VCT_TEST(p13_pop_offsets) {
  VCoreState *s = fresh();
  mode(s, 1);
  for (int i = 0; i < 4; i++) {
    trans(s, ONE, 0, 0); /* x = 1,2,3,4 after each; push saves before the next */
    cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  }
  VCT_EQ(s->p13.sp, 4);
  /* stack: [0]=1 [1]=2 [2]=3 [3]=4. pop 3: sp = 1, C = [1] = 2 */
  u32 three = 3;
  cmd(s, VC_OP_MTX_POP, 1, &three);
  VCT_EQ(s->p13.sp, 1);
  VCT_EQ(vc_mtx_pos(s)->m[12], 2 * ONE);
  /* pop -2 (6-bit 0x3E): sp = 1 + 2 = 3, C = [3] = 4 */
  u32 m2 = 0x3E;
  cmd(s, VC_OP_MTX_POP, 1, &m2);
  VCT_EQ(s->p13.sp, 3);
  VCT_EQ(vc_mtx_pos(s)->m[12], 4 * ONE);
  /* the parameter's upper bits are unused: 0xFFFFFFC1 = offset +1 */
  u32 up = 0xFFFFFFC1u;
  cmd(s, VC_OP_MTX_POP, 1, &up);
  VCT_EQ(s->p13.sp, 2);
  VCT_EQ(vc_mtx_pos(s)->m[12], 3 * ONE);
  VCT_EQ(s->p13.stack_error, 0);
}

VCT_TEST(p13_store_restore) {
  VCoreState *s = fresh();
  mode(s, 1);
  trans(s, 5 * ONE, 0, 0);
  u32 n = 7;
  cmd(s, VC_OP_MTX_STORE, 1, &n);
  VCT_EQ(s->p13.sp, 0); /* the pointer is untouched */
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  VCT_CHECK(is_ident(vc_mtx_pos(s)));
  cmd(s, VC_OP_MTX_RESTORE, 1, &n);
  VCT_EQ(vc_mtx_pos(s)->m[12], 5 * ONE);
  VCT_EQ(vc_mtx_clip(s)->m[12], 5 * ONE);
  VCT_EQ(s->p13.sp, 0);
  VCT_EQ(s->p13.stack_error, 0);
  /* bits above 4 of the address are unused: 0x27 = 7 */
  n = 0x27;
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  cmd(s, VC_OP_MTX_RESTORE, 1, &n);
  VCT_EQ(vc_mtx_pos(s)->m[12], 5 * ONE);
  /* 30 is the last valid entry */
  n = 30;
  cmd(s, VC_OP_MTX_STORE, 1, &n);
  VCT_EQ(s->p13.stack_error, 0);
  /* 31 sets the error flag, and still works */
  n = 31;
  cmd(s, VC_OP_MTX_STORE, 1, &n);
  VCT_EQ(s->p13.stack_error, 1);
  trans(s, ONE, 0, 0);
  cmd(s, VC_OP_MTX_RESTORE, 1, &n);
  VCT_EQ(vc_mtx_pos(s)->m[12], 5 * ONE);
}

VCT_TEST(p13_stack_overflow_flag) {
  VCoreState *s = fresh();
  mode(s, 1);
  for (int i = 0; i < 31; i++) cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  VCT_EQ(s->p13.sp, 31);
  VCT_EQ(s->p13.stack_error, 0); /* 31 pushes fill entries 0..30 */
  cmd(s, VC_OP_MTX_PUSH, 0, NULL); /* the 32nd writes entry 31: error */
  VCT_EQ(s->p13.stack_error, 1);
  VCT_EQ(s->p13.sp, 32);
  /* the pointer is 6 bits: it wraps at 64 */
  for (int i = 0; i < 32; i++) cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  VCT_EQ(s->p13.sp, 0);
}

VCT_TEST(p13_stack_underflow_flag) {
  VCoreState *s = fresh();
  mode(s, 1);
  u32 one = 1;
  cmd(s, VC_OP_MTX_POP, 1, &one); /* sp 0 - 1 = 63: invalid */
  VCT_EQ(s->p13.sp, 63);
  VCT_EQ(s->p13.stack_error, 1);
  /* the upper half mirrors the lower: sp 63 reads entry 31 */
  s->p13.stack_error = 0;
  s->p13.sp = 0x25; /* 37: mirror of 5, but beyond 30: error */
  cmd(s, VC_OP_MTX_POP, 1, &one);
  VCT_EQ(s->p13.sp, 0x24);
  VCT_EQ(s->p13.stack_error, 1);
}

VCT_TEST(p13_projection_stack_one_entry) {
  VCoreState *s = fresh();
  mode(s, 0);
  u32 sc[3] = {2 * ONE, 2 * ONE, 2 * ONE};
  cmd(s, VC_OP_MTX_SCALE, 3, sc);
  cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  VCT_EQ(s->p13.proj_sp, 1);
  VCT_EQ(s->p13.stack_error, 0);
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  VCT_EQ(vc_mtx_clip(s)->m[0], ONE);
  u32 junk = 0x1F; /* the parameter is ignored in projection mode */
  cmd(s, VC_OP_MTX_POP, 1, &junk);
  VCT_EQ(s->p13.proj_sp, 0);
  VCT_EQ(vc_mtx_proj(s)->m[0], 2 * ONE);
  VCT_EQ(vc_mtx_clip(s)->m[0], 2 * ONE); /* clip follows the pop */
  VCT_EQ(s->p13.stack_error, 0);
  /* a second push without a pop: the flag */
  cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  VCT_EQ(s->p13.stack_error, 1);
  /* store/restore use entry 0 whatever the parameter */
  s->p13.stack_error = 0;
  u32 five = 5;
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  cmd(s, VC_OP_MTX_STORE, 1, &five);
  cmd(s, VC_OP_MTX_SCALE, 3, sc);
  cmd(s, VC_OP_MTX_RESTORE, 1, &five);
  VCT_EQ(vc_mtx_proj(s)->m[0], ONE);
  VCT_EQ(vc_mtx_clip(s)->m[0], ONE);
  /* the position stack is separate */
  VCT_EQ(s->p13.sp, 0);
}

VCT_TEST(p13_texture_stack) {
  VCoreState *s = fresh();
  mode(s, 3);
  trans(s, 3 * ONE, 0, 0);
  cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  VCT_CHECK(is_ident(vc_mtx_tex(s)));
  u32 one = 1;
  cmd(s, VC_OP_MTX_POP, 1, &one);
  VCT_EQ(vc_mtx_tex(s)->m[12], 3 * ONE);
  VCT_CHECK(is_ident(vc_mtx_pos(s)) && is_ident(vc_mtx_proj(s)));
  VCT_EQ(s->p13.sp, 0);
}

static int is_zero(const VC_Mtx *m) {
  for (int i = 0; i < 16; i++)
    if (m->m[i]) return 0;
  return 1;
}

/* The next four tests record the ORACLE's rules (GBATEK is silent): fitted over every PUSH/POP/STORE/RESTORE/LOAD
 * sequence up to length 4 in projection mode and over pushes up to 66 deep, pops of any offset and RESTORE of every
 * index in modes 1 and 2 (tools: a probe comparing the oracle's frame with a frame of the final matrix loaded). */
VCT_TEST(p13_empty_stack_slots_are_zero) {
  VCoreState *s = fresh();
  mode(s, 2);
  trans(s, ONE, 0, 0);
  u32 n = 20; /* never stored */
  cmd(s, VC_OP_MTX_RESTORE, 1, &n);
  VCT_CHECK(is_zero(vc_mtx_pos(s)) && is_zero(vc_mtx_vec(s)) && is_zero(vc_mtx_clip(s)));
  s = fresh();
  mode(s, 1);
  u32 one = 1;
  cmd(s, VC_OP_MTX_POP, 1, &one); /* pop of an empty stack: the zero matrix, and the error flag */
  VCT_CHECK(is_zero(vc_mtx_pos(s)));
  VCT_EQ(s->p13.stack_error, 1);
}

VCT_TEST(p13_projection_stack_saturates) {
  VCoreState *s = fresh();
  mode(s, 0);
  u32 one = 1;
  cmd(s, VC_OP_MTX_POP, 1, &one); /* empty: zero matrix */
  VCT_CHECK(is_zero(vc_mtx_proj(s)) && is_zero(vc_mtx_clip(s)));
  VCT_EQ(s->p13.proj_sp, 0);
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  cmd(s, VC_OP_MTX_PUSH, 0, NULL); /* stack = I */
  trans(s, 2 * ONE, 0, 0);
  s->p13.stack_error = 0;
  cmd(s, VC_OP_MTX_PUSH, 0, NULL); /* full: flag, but the entry IS overwritten */
  VCT_EQ(s->p13.stack_error, 1);
  VCT_EQ(s->p13.proj_sp, 1);
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  cmd(s, VC_OP_MTX_POP, 1, &one);
  VCT_EQ(vc_mtx_proj(s)->m[12], 2 * ONE);
  /* a second POP: flag, the pointer stays 0 and the entry loads again */
  s->p13.stack_error = 0;
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  cmd(s, VC_OP_MTX_POP, 1, &one);
  VCT_EQ(s->p13.stack_error, 1);
  VCT_EQ(s->p13.proj_sp, 0);
  VCT_EQ(vc_mtx_proj(s)->m[12], 2 * ONE);
  VCT_EQ(vc_mtx_clip(s)->m[12], 2 * ONE);
}

VCT_TEST(p13_texture_stack_saturates) {
  VCoreState *s = fresh();
  mode(s, 3);
  u32 one = 1;
  cmd(s, VC_OP_MTX_POP, 1, &one);
  VCT_CHECK(is_zero(vc_mtx_tex(s)));
  cmd(s, VC_OP_MTX_IDENTITY, 0, NULL);
  trans(s, 1, 0, 0);
  cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  VCT_EQ(s->p13.stack_error, 1);
  VCT_EQ(s->p13.tex_sp, 1);
  cmd(s, VC_OP_MTX_POP, 1, &one);
  cmd(s, VC_OP_MTX_POP, 1, &one);
  VCT_EQ(s->p13.tex_sp, 0);
  VCT_EQ(vc_mtx_tex(s)->m[12], 1);
}

VCT_TEST(p13_deep_push_pop_wraps_at_64) {
  VCoreState *s = fresh();
  mode(s, 2);
  for (int i = 0; i < 66; i++) {
    trans(s, 8, 0, 0);
    cmd(s, VC_OP_MTX_PUSH, 0, NULL);
  }
  VCT_EQ(s->p13.sp, 2);
  u32 one = 1;
  /* push k (0-based) writes entry (k mod 64) & 31 with x = 8 (k+1); entry 1 last by push 65 */
  cmd(s, VC_OP_MTX_POP, 1, &one);
  VCT_EQ(s->p13.sp, 1);
  VCT_EQ(vc_mtx_pos(s)->m[12], 8 * 66);
}

VCT_TEST(p13_dispatch_and_short_params) {
  VCoreState *s = fresh();
  /* vc_gx_command routes the matrix opcodes; missing parameters count as 0 */
  vc_gx_command(s, VC_OP_MTX_MODE, (u32[]){1}, 1);
  VCT_EQ(s->p13.mode, 1);
  vc_gx_command(s, VC_OP_MTX_TRANS, (u32[]){ONE}, 1);
  VCT_EQ(vc_mtx_pos(s)->m[12], ONE);
  VCT_EQ(vc_mtx_pos(s)->m[13], 0);
  vc_gx_command(s, 0x7F, NULL, 0); /* an invalid command is ignored */
  VCT_EQ(vc_mtx_pos(s)->m[12], ONE);
}

VCT_TEST(p13_scene_translate_scale) {
  /* a scene for the oracle: a perspective-free ortho triangle through TRANS/SCALE/PUSH/POP */
  VCoreState *s = vct_state();
  vct_io16(s, 0x60, 0x3101); /* DISP3DCNT: texture off, shading on, backdrop colour via CLEAR_COLOR; harmless */
  vct_io32(s, 0x00, 0x10100 | 0x0008); /* DISPCNT: mode 1, BG0 on as 3D */
  u32 v;
  v = 1; vct_gx(s, VC_OP_MTX_MODE, 1, &v);
  vct_gx(s, VC_OP_MTX_IDENTITY, 0, NULL);
  u32 t[3] = {0x400, 0, 0};
  vct_gx(s, VC_OP_MTX_TRANS, 3, t);
  vct_gx(s, VC_OP_MTX_PUSH, 0, NULL);
  u32 sc[3] = {0x800, 0x800, 0x1000};
  vct_gx(s, VC_OP_MTX_SCALE, 3, sc);
  v = 0x7FFF; vct_gx(s, VC_OP_COLOR, 1, &v);
  v = 0; vct_gx(s, VC_OP_POLYGON_ATTR, 1, &(u32){0x001F0000});
  vct_gx(s, VC_OP_BEGIN_VTXS, 1, &v);
  vct_gx(s, VC_OP_VTX_16, 2, (u32[]){0x00000000, 0});
  vct_gx(s, VC_OP_VTX_16, 2, (u32[]){0x00001000, 0});
  vct_gx(s, VC_OP_VTX_16, 2, (u32[]){0x10000000, 0});
  vct_gx(s, VC_OP_END_VTXS, 0, NULL);
  vct_gx(s, VC_OP_MTX_POP, 1, &(u32){1});
  vct_gx(s, VC_OP_SWAP_BUFFERS, 1, &v);
  vct_emit(s, "p13_translate_scale");
}
