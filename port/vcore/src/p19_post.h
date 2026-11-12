/* port/vcore piece 19: clear image, edge marking, fog, toon / highlight, antialiasing (docs/vcore.md, piece 19). OWNED BY PIECE 19: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P19_H
#define VC_P19_H

#include "vc_types.h"

typedef struct VC_P19 {
  int unused; /* add the piece's state here */
} VC_P19;

struct VCoreState;
u32 vc_p19_fog_density(const struct VCoreState *s, u32 d15);

#endif
