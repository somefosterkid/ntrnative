// Black 2's and White 2's particles (port/extern/lib/spl/src/spl_emitter.c's twin): pokebw2's lib/spl/src/spl_emitter.c compiled
// through this file (tools/port_sources.py --b2w2), with Smooth motion's tags (libntr's simulator/sim_g3tag.h). The two particle
// loops (SPLEmitter_DrawParticles, SPLEmitter_DrawChildren) draw each particle through their local function pointer draw: the
// hook tags it first (kind 3 particle: owner the emitter, inst the particle, state its age); SPLEmitter_Draw, one emitter's draw,
// ends with the empty tag; SPLEmitter_Init gives a new emitter a new id (new particles: port/extern_b2w2/lib/spl/src/spl_emit.c).
// Off (the default) each site is one flag test and the command stream is unchanged; BWPORT_VCAPTURE_TAGS=1 or SIM_G3TagEnable(1)
// (Smooth motion on) turns them on. The DS build never sees this file.
#pragma push_macro("SPLEmitter_Draw")
#pragma push_macro("SPLEmitter_Init")
#define SPLEmitter_Draw SPLEmitter_Draw_game
#define SPLEmitter_Init SPLEmitter_Init_game
#include "types.h"   // the game file's headers, before the hook below (their include guards keep them out of it)
#include "nitro/fx.h"
#include "nitro/gx.h"
#include "spl/spl.h"
#include "../../../../../third_party/pokebw2/lib/spl/src/spl_internal.h"
#include <simulator/sim_g3tag.h>

static void PortSpl_Tag(const SPLManager *mgr, const SPLParticle *particle);

// draw( appears only as the two loops' calls of their local draw function (their mgr and particle)
#define draw(mgr, particle) ((SIM_G3TagOn ? PortSpl_Tag(mgr, particle) : (void)0), draw(mgr, particle))
#include "../../../../../third_party/pokebw2/lib/spl/src/spl_emitter.c"
#undef draw
#pragma pop_macro("SPLEmitter_Draw")
#pragma pop_macro("SPLEmitter_Init")

static void PortSpl_Tag(const SPLManager *mgr, const SPLParticle *particle) {
    SIM_G3Tag(SIM_G3TAG_OWNER(SIM_G3TAG_PARTICLE, mgr->drawEmitter), SIM_G3TagId(particle), particle->age);
}

void SPLEmitter_Draw(SPLManager *mgr) {
    SPLEmitter_Draw_game(mgr);
    if (SIM_G3TagOn) {
        SIM_G3Tag(0, 0, 0);
    }
}

void SPLEmitter_Init(SPLEmitter *emitter, SPLResource *resource, const VecFx32 *pos) {
    SIM_G3TagDrop(emitter);
    SPLEmitter_Init_game(emitter, resource, pos);
}
