/* port/vcore piece 12: VRAM bank mapping (docs/vcore.md, piece 12). OWNED BY PIECE 12: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P12_H
#define VC_P12_H

#include "vc_types.h"

typedef struct VC_P12 {
  int unused; /* add the piece's state here */
} VC_P12;

#endif
