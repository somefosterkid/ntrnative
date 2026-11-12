/* port/vcore piece 4: the large bitmap BG and BG mosaic (docs/vcore.md, piece 4). OWNED BY PIECE 4: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P04_H
#define VC_P04_H

#include "vc_types.h"

typedef struct VC_P04 {
  int unused; /* add the piece's state here */
} VC_P04;

#endif
