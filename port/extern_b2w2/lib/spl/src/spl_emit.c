// Black 2's and White 2's particle emission (port/extern/lib/spl/src/spl_emit.c's twin): pokebw2's lib/spl/src/spl_emit.c
// compiled through this file (tools/port_sources.py --b2w2), with Smooth motion's tags (libntr's simulator/sim_g3tag.h; the draw:
// port/extern_b2w2/lib/spl/src/spl_emitter.c). Each new particle, parent or child, is pushed onto its emitter's list here
// (SPLList_PushFront): the hook gives it a new tag id first. Off (the default) each site is one flag test and the command stream
// is unchanged; BWPORT_VCAPTURE_TAGS=1 or SIM_G3TagEnable(1) (Smooth motion on) turns them on. The DS build never sees this file.
#include "types.h"   // the game file's headers, before the hook below (their include guards keep them out of it)
#include "nitro/fx.h"
#include "nitro/gx.h"
#include "spl/spl.h"
#include "../../../../../third_party/pokebw2/lib/spl/src/spl_internal.h"
#include <simulator/sim_g3tag.h>

// SPLList_PushFront is called here only for the new particles
#define SPLList_PushFront(list, node) (SIM_G3TagDrop(node), SPLList_PushFront(list, node))
#include "../../../../../third_party/pokebw2/lib/spl/src/spl_emit.c"
#undef SPLList_PushFront
