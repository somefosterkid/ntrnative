#include "global.h"

#include "library/spl.h"
#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "camera.h"
#include "particle_system.h"

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

#include "particle_system_globals.h"

SPLEmitter *sub_02015494(ParticleSystem *particleSystem, s32 resourceID, void (*callback)(SPLEmitter *), void *param);

SPLEmitter *sub_02015494(ParticleSystem *particleSystem, s32 resourceID, void (*callback)(SPLEmitter *), void *param) {
    SPLEmitter *temp_r0;

    _021D10A0.unk4 = param;
    temp_r0 = SPLManager_CreateEmitterWithCallback(particleSystem->manager, resourceID, callback);
    _021D10A0.unk4 = NULL;
    particleSystem->lastAddedEmitter = temp_r0;
    return temp_r0;
}
