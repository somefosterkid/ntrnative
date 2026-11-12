/* port/vcore piece 2: affine BGs (docs/vcore.md, piece 2). OWNED BY PIECE 2: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P02_H
#define VC_P02_H

#include "vc_types.h"

typedef struct VC_P02 {
  int unused; /* add the piece's state here */
} VC_P02;

#endif
