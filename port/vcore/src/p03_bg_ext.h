/* port/vcore piece 3: extended BGs (affine 16-bit map, 256-colour and direct-colour bitmaps) (docs/vcore.md, piece 3). OWNED BY PIECE 3: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P03_H
#define VC_P03_H

#include "vc_types.h"

typedef struct VC_P03 {
  int unused; /* add the piece's state here */
} VC_P03;

#endif
