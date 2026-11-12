/* port/vcore piece 13: the matrix engine and the command processor (docs/vcore.md, piece 13). OWNED BY PIECE 13: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P13_H
#define VC_P13_H

#include "vc_types.h"

typedef struct VC_P13 {
  VC_Mtx proj, pos, vec, tex, clip; /* the current matrices; clip = pos x proj, kept up to date */
  int mode;                         /* MTX_MODE 0-3 */
  VC_Mtx proj_stack;                /* projection stack: 1 entry (GBATEK 39) */
  VC_Mtx tex_stack;                 /* texture stack: treated like the projection stack (1 entry) */
  VC_Mtx pos_stack[32], vec_stack[32]; /* coordinate and directional stacks (31 valid + entry 31) */
  int proj_sp, tex_sp;              /* 1-bit pointers */
  int sp;                           /* the 6-bit coordinate stack pointer (shared with the directional stack) */
  int stack_error;                  /* GXSTAT bit 15: a stack over/underflow was seen */
} VC_P13;

#endif
