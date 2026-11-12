/* port/vcore piece 9: the compositor and the colour effects (docs/vcore.md, piece 9). OWNED BY PIECE 9: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P09_H
#define VC_P09_H

#include "vc_types.h"

typedef struct VC_P09 {
  int unused; /* the compositor keeps no state between lines */
} VC_P09;

/* The colour arithmetic, on RGB6 values (r6 | g6 << 8 | b6 << 16; bits above 23 are dropped).
 * GBATEK 09: I = MIN(31, I1st*EVA + I2nd*EVB) with the coefficients in 1/16, I = I1st + (31-I1st)*EVY,
 * I = I1st - I1st*EVY; the engine works in 6 bits per channel (the 5-bit colour widened by one bit). */
u32 vc_blend_alpha(u32 c1, u32 c2, int eva, int evb); /* eva, evb 0-16 (the caller clamps) */
u32 vc_blend_bright(u32 c, int evy);                  /* brightness increase, evy 0-16 */
u32 vc_blend_dark(u32 c, int evy);                    /* brightness decrease, evy 0-16 */
u32 vc_blend_3d(u32 c1, u32 c2, int alpha5);          /* the 3D layer c1 over c2 with its own alpha 1-31 */

#endif
