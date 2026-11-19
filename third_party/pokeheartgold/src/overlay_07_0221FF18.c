#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "sys_task.h"
#include "particle_system.h"

struct ParticleSystem;

typedef void (*SPLEmitterCallback)(SPLEmitter *emtr);

SPLEmitter *sub_02015494(ParticleSystem *particleSystem, int resourceID, SPLEmitterCallback callback, void *param);

extern const SPLEmitterCallback ov07_022353E4[23];

SPLEmitter *ov07_0221FF18(ParticleSystem *ps, int resourceID, int callbackID, void *battleAnimSystemPtr);

void ov06_0221BAD8(ParticleSystem *param_1);
void ov07_0221FF2C(ParticleSystem *param_1);

SPLEmitter *ov07_0221FF18(ParticleSystem *ps, int resourceID, int callbackID, void *battleAnimSystemPtr) {
    return sub_02015494(ps, resourceID, ov07_022353E4[callbackID], battleAnimSystemPtr);
}

void ov07_0221FF2C(ParticleSystem *param_1) {
    ov06_0221BAD8(param_1);
}
