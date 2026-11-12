/* port/vcore piece 11: display capture (docs/vcore.md, piece 11). OWNED BY PIECE 11: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P11_H
#define VC_P11_H

#include "vc_types.h"

typedef struct VC_P11 {
  int unused; /* add the piece's state here */
} VC_P11;

#endif
