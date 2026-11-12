/* port/vcore piece 5: normal OBJs and the OAM loop (docs/vcore.md, piece 5). OWNED BY PIECE 5: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P05_H
#define VC_P05_H

#include "vc_types.h"

typedef struct VC_P05 {
  int unused; /* add the piece's state here */
} VC_P05;

#endif
