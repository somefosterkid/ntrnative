/* port/vcore piece 1: text BGs (docs/vcore.md, piece 1). OWNED BY PIECE 1: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P01_H
#define VC_P01_H

#include "vc_types.h"

typedef struct VC_P01 {
  int unused; /* add the piece's state here */
} VC_P01;

#endif
