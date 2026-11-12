/* port/vcore piece 13: the matrix engine and the command processor (docs/vcore.md, piece 13). OWNED BY PIECE 13.
 * Written from GBATEK: 36 (command list), 37 (load/multiply), 38 (matrix types), 39 (stacks), 42 (maths). */
#include <string.h>
#include "vc_shared.h"

static void mtx_identity(VC_Mtx *m) {
  for (int i = 0; i < 16; i++) m->m[i] = (i % 5 == 0) ? 0x1000 : 0;
}

/* r = a * b (4x4, row-major, GBATEK 42: 64-bit sums, >> 12) */
static void mtx_mul(const VC_Mtx *a, const VC_Mtx *b, VC_Mtx *r) {
  VC_Mtx t;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      s64 sum = 0;
      for (int k = 0; k < 4; k++) sum += (s64)a->m[i * 4 + k] * b->m[k * 4 + j];
      t.m[i * 4 + j] = (s32)(sum >> 12);
    }
  *r = t;
}

static void update_clip(VCoreState *s) {
  mtx_mul(&s->p13.pos, &s->p13.proj, &s->p13.clip); /* ClipMatrix = Position * Projection (GBATEK 37) */
}

void vc_mtx_reset(VCoreState *s) {
  VC_P13 *p = &s->p13;
  memset(p, 0, sizeof *p);
  mtx_identity(&p->proj);
  mtx_identity(&p->pos);
  mtx_identity(&p->vec);
  mtx_identity(&p->tex);
  /* the stacks start as all-zero matrices (the memset above), not identity: oracle, e.g. RESTORE of a slot never
   * STOREd, or POP on an empty stack, gives the zero matrix (GBATEK is silent). */
  update_clip(s);
}

void vc_gx_command(VCoreState *s, u32 op, const u32 *param, u32 nparam) {
  /* the dispatcher (skeleton): each opcode to its piece */
  if (op >= VC_OP_MTX_MODE && op <= VC_OP_MTX_TRANS) vc_cmd_mtx(s, op, param, nparam);
  else if (op == VC_OP_COLOR || op == VC_OP_NORMAL || op == VC_OP_TEXCOORD ||
           (op >= VC_OP_DIF_AMB && op <= VC_OP_SHININESS) || op == VC_OP_VEC_TEST)
    vc_cmd_light(s, op, param, nparam);
  else if (op >= VC_OP_VTX_16 && op <= VC_OP_VTX_DIFF) vc_cmd_vtx(s, op, param, nparam);
  else if ((op >= VC_OP_POLYGON_ATTR && op <= VC_OP_PLTT_BASE) || op == VC_OP_BEGIN_VTXS ||
           op == VC_OP_END_VTXS || op == VC_OP_VIEWPORT || op == VC_OP_BOX_TEST || op == VC_OP_POS_TEST)
    vc_cmd_poly(s, op, param, nparam);
  else if (op == VC_OP_SWAP_BUFFERS) vc_cmd_swap(s, param, nparam);
}

/* load (or multiply C = M * C) the matrices the current mode selects: 0 proj, 1 pos, 2 pos+vec, 3 tex */
static void apply_current(VCoreState *s, int also_vec, const VC_Mtx *m, int load) {
  VC_P13 *p = &s->p13;
  VC_Mtx *t[2] = {NULL, NULL};
  switch (p->mode) {
    case 0: t[0] = &p->proj; break;
    case 1: t[0] = &p->pos; break;
    case 2: t[0] = &p->pos; if (also_vec) t[1] = &p->vec; break;
    default: t[0] = &p->tex; break;
  }
  for (int i = 0; i < 2; i++) {
    if (!t[i]) continue;
    if (load) *t[i] = *m;
    else mtx_mul(m, t[i], t[i]); /* C = M * C */
  }
  if (p->mode <= 2) update_clip(s);
}

static u32 prm(const u32 *param, u32 n, u32 i) { return i < n ? param[i] : 0; }

/* build a full 4x4 from the parameter forms of GBATEK 38 */
static void build(VC_Mtx *m, u32 op, const u32 *param, u32 n) {
  mtx_identity(m);
  switch (op) {
    case VC_OP_MTX_LOAD_4x4: case VC_OP_MTX_MULT_4x4:
      for (u32 i = 0; i < 16; i++) m->m[i] = (s32)prm(param, n, i);
      break;
    case VC_OP_MTX_LOAD_4x3: case VC_OP_MTX_MULT_4x3: /* 12 values fill rows 0-3, columns 0-2 */
      for (u32 r = 0; r < 4; r++)
        for (u32 c = 0; c < 3; c++) m->m[r * 4 + c] = (s32)prm(param, n, r * 3 + c);
      break;
    case VC_OP_MTX_MULT_3x3:
      for (u32 r = 0; r < 3; r++)
        for (u32 c = 0; c < 3; c++) m->m[r * 4 + c] = (s32)prm(param, n, r * 3 + c);
      break;
    case VC_OP_MTX_SCALE:
      for (u32 i = 0; i < 3; i++) m->m[i * 5] = (s32)prm(param, n, i);
      break;
    case VC_OP_MTX_TRANS:
      for (u32 i = 0; i < 3; i++) m->m[12 + i] = (s32)prm(param, n, i);
      break;
  }
}

/* stack ops (GBATEK 39): mode 1 and 2 both act on the coordinate and directional stacks */
static void stack_op(VCoreState *s, u32 op, u32 prm0) {
  VC_P13 *p = &s->p13;
  if (p->mode == 0 || p->mode == 3) {
    VC_Mtx *cur = p->mode == 0 ? &p->proj : &p->tex;
    VC_Mtx *stk = p->mode == 0 ? &p->proj_stack : &p->tex_stack;
    int *sp = p->mode == 0 ? &p->proj_sp : &p->tex_sp;
    switch (op) {
      /* fitted to the oracle over every PUSH/POP/STORE/RESTORE/LOAD sequence up to length 4 (GBATEK says only
       * "1 entry"): the pointer saturates at 0/1 instead of wrapping. PUSH always writes the entry (even when it
       * is already full: error flag) and sets the pointer to 1; POP always loads the entry (even when empty:
       * error flag, so an empty POP loads the zero matrix) and sets the pointer to 0. */
      case VC_OP_MTX_PUSH:
        if (*sp) p->stack_error = 1;
        *stk = *cur;
        *sp = 1;
        break;
      case VC_OP_MTX_POP: /* offset ignored */
        if (!*sp) p->stack_error = 1;
        *cur = *stk;
        *sp = 0;
        break;
      case VC_OP_MTX_STORE: *stk = *cur; break; /* parameter ignored */
      case VC_OP_MTX_RESTORE: *cur = *stk; break;
    }
    if (p->mode == 0) update_clip(s);
    return;
  }
  int n;
  switch (op) {
    case VC_OP_MTX_PUSH:
      if (p->sp >= 31) p->stack_error = 1; /* entries 31..63 raise the flag */
      p->pos_stack[p->sp & 31] = p->pos;
      p->vec_stack[p->sp & 31] = p->vec;
      p->sp = (p->sp + 1) & 63;
      break;
    case VC_OP_MTX_POP:
      n = (int)(prm0 & 0x3F);
      if (n & 0x20) n -= 64; /* signed 6 bit */
      p->sp = (p->sp - n) & 63;
      if (p->sp >= 31) p->stack_error = 1;
      p->pos = p->pos_stack[p->sp & 31];
      p->vec = p->vec_stack[p->sp & 31];
      update_clip(s);
      break;
    case VC_OP_MTX_STORE:
      n = (int)(prm0 & 0x1F);
      if (n >= 31) p->stack_error = 1;
      p->pos_stack[n] = p->pos;
      p->vec_stack[n] = p->vec;
      break;
    case VC_OP_MTX_RESTORE:
      n = (int)(prm0 & 0x1F);
      if (n >= 31) p->stack_error = 1;
      p->pos = p->pos_stack[n];
      p->vec = p->vec_stack[n];
      update_clip(s);
      break;
  }
}

void vc_cmd_mtx(VCoreState *s, u32 op, const u32 *param, u32 nparam) {
  VC_P13 *p = &s->p13;
  VC_Mtx m;
  switch (op) {
    case VC_OP_MTX_MODE: p->mode = (int)(prm(param, nparam, 0) & 3); break;
    case VC_OP_MTX_PUSH: case VC_OP_MTX_POP: case VC_OP_MTX_STORE: case VC_OP_MTX_RESTORE:
      stack_op(s, op, prm(param, nparam, 0));
      break;
    case VC_OP_MTX_IDENTITY:
      mtx_identity(&m);
      apply_current(s, 1, &m, 1);
      break;
    case VC_OP_MTX_LOAD_4x4: case VC_OP_MTX_LOAD_4x3:
      build(&m, op, param, nparam);
      apply_current(s, 1, &m, 1);
      break;
    case VC_OP_MTX_MULT_4x4: case VC_OP_MTX_MULT_4x3: case VC_OP_MTX_MULT_3x3: case VC_OP_MTX_TRANS:
      build(&m, op, param, nparam);
      apply_current(s, 1, &m, 0);
      break;
    case VC_OP_MTX_SCALE: /* never the vector matrix, even in mode 2 */
      build(&m, op, param, nparam);
      apply_current(s, 0, &m, 0);
      break;
  }
}

const VC_Mtx *vc_mtx_clip(VCoreState *s) { return &s->p13.clip; }
const VC_Mtx *vc_mtx_pos(VCoreState *s) { return &s->p13.pos; }
const VC_Mtx *vc_mtx_vec(VCoreState *s) { return &s->p13.vec; }
const VC_Mtx *vc_mtx_tex(VCoreState *s) { return &s->p13.tex; }
const VC_Mtx *vc_mtx_proj(VCoreState *s) { return &s->p13.proj; }

void vc_mtx_apply(const VC_Mtx *m, const s32 in[4], s32 out[4]) {
  /* v' = v * M, GBATEK 42: 64-bit sums, >> 12 (checked by tests) */
  s32 r[4];
  for (int j = 0; j < 4; j++) {
    s64 sum = 0;
    for (int k = 0; k < 4; k++) sum += (s64)in[k] * m->m[k * 4 + j];
    r[j] = (s32)(sum >> 12);
  }
  for (int j = 0; j < 4; j++) out[j] = r[j]; /* in-place safe */
}
