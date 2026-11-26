// The PC build compiles lib/spl/src/spl_emit.c through this file (tools/port_sources.py): Smooth motion's tags (libntr's
// simulator/sim_g3tag.h; the particles' draw: port/extern/lib/spl/src/spl_emitter.c). Each new particle, parent or child, is
// pushed onto its emitter's list here (func_020583cc): the hook gives it a new tag id first. Off (the default) each site is one
// flag test and the command stream is unchanged; BWPORT_VCAPTURE_TAGS=1 or SIM_G3TagEnable(1) (Smooth motion on) turns them on.
// The DS build never sees this file.
#include "types.h"   // the game file's headers, before the hook below (their include guards keep them out of it)
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/gx.h"
#include "spl/spl.h"
#include "../../../../../lib/spl/src/spl_internal.h"
#include <simulator/sim_g3tag.h>

// func_020583cc (the list's push) is called here only for the new particles. White's build renames it (-include w_rename.h defines
// func_020583cc as w_func_020583cc), so the real call goes through a name that is expanded at the helper, before the hook macro replaces
// the rename's
#define PORT_SPL_PUSH_REAL func_020583cc
static inline void PortSpl_PushNew(SPLList *list, SPLNode *node) {
    SIM_G3TagDrop(node);
    PORT_SPL_PUSH_REAL(list, node);
}
#define func_020583cc(list, node) PortSpl_PushNew(list, node)
#include "../../../../../lib/spl/src/spl_emit.c"
#undef func_020583cc
