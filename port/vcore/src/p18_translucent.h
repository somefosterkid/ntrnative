/* port/vcore piece 18: translucency, sorting, polygon IDs, shadows (docs/vcore.md, piece 18). OWNED BY PIECE 18: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P18_H
#define VC_P18_H

#include "vc_types.h"

typedef struct VC_P18 {
  int unused; /* no state: everything is in s->geo and s->f3d */
} VC_P18;

struct VCoreState;
/* 1 if polygon p is drawn in the translucent pass (GBATEK 43: alpha 1..30; GBATEK 47: A3I5 / A5I3 texture while
 * textures are enabled in DISP3DCNT) */
int vc_p18_is_translucent(const struct VCoreState *s, const VC_Poly *p);

/* the pixel write with the depth test's verdict given (vc_3d_plot = this with vc_3d_depth_test's answer) */
void vc_p18_plot(struct VCoreState *s, const VC_Poly *p, int x, int y, u32 depth, u32 rgb6a5, int edge, int depth_pass);

#endif
