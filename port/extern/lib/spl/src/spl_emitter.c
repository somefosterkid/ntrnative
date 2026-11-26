// The PC build compiles lib/spl/src/spl_emitter.c through this file (tools/port_sources.py): Smooth motion's tags (libntr's
// simulator/sim_g3tag.h) on lib/spl's particles. The two particle loops (func_0205312c's parents, func_02053044's children) draw
// each particle through their local function pointer draw: the hook tags it first (kind 3 particle: owner the emitter, inst the
// particle, state its age); func_02052fc8, one emitter's draw, ends with the empty tag; func_02053a60 gives a new emitter a new
// id (new particles: port/extern/lib/spl/src/spl_emit.c). Off (the default) each site is one flag test and the command stream is
// unchanged; BWPORT_VCAPTURE_TAGS=1 or SIM_G3TagEnable(1) (Smooth motion on) turns them on. The DS build never sees this file.
#pragma push_macro("func_02052fc8")
#pragma push_macro("func_02053a60")
#define func_02052fc8 func_02052fc8_game
#define func_02053a60 func_02053a60_game
#include "types.h"   // the game file's headers, before the hook below (their include guards keep them out of it)
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "spl/spl.h"
#include "../../../../../lib/spl/src/spl_internal.h"
#include <simulator/sim_g3tag.h>

static void PortSpl_Tag(const SPLManager *mgr, const SPLParticle *particle);

// draw( appears only as the two loops' calls of their local draw function (their mgr and particle)
#define draw(mgr, particle) ((SIM_G3TagOn ? PortSpl_Tag(mgr, particle) : (void)0), draw(mgr, particle))
#include "../../../../../lib/spl/src/spl_emitter.c"
#undef draw
#pragma pop_macro("func_02052fc8")
#pragma pop_macro("func_02053a60")

static void PortSpl_Tag(const SPLManager *mgr, const SPLParticle *particle) {
    SIM_G3Tag(SIM_G3TAG_OWNER(SIM_G3TAG_PARTICLE, mgr->drawEmitter), SIM_G3TagId(particle), particle->age);
}

void func_02052fc8(SPLManager *mgr) {
    func_02052fc8_game(mgr);
    if (SIM_G3TagOn) {
        SIM_G3Tag(0, 0, 0);
    }
}

void func_02053a60(SPLEmitter *emitter, SPLResource *resource, const VecFx32 *pos) {
    SIM_G3TagDrop(emitter);
    func_02053a60_game(emitter, resource, pos);
}
