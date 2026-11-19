#include "global.h"

#include "library/spl.h"
#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "particle_system.h"
#include "unk_02014DA0.h"

SPLEmitter *sub_02015484(ParticleSystem *particleSystem, int resourceID, const VecFx32 *position) {
    SPLEmitter *emitter = SPLManager_CreateEmitter(particleSystem->manager, resourceID, position);
    particleSystem->lastAddedEmitter = emitter;
    return emitter;
}
