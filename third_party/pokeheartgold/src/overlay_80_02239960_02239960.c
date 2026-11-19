#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_texture.h"

#include "camera.h"
#include "heap.h"
#include "unk_02014DA0.h"
#include "particle_system.h"

struct ParticleSystem;
typedef struct FrontierParticleSystem {
    ParticleSystem *particleSys[8];
    u16 heapID;
} FrontierParticleSystem;
void ov80_02239A98(ParticleSystem *ps);

FrontierParticleSystem *ov80_02239960(enum HeapID heapID);
void ov80_02239980(FrontierParticleSystem *fps);

FrontierParticleSystem *ov80_02239960(enum HeapID heapID) {
    FrontierParticleSystem *fps = Heap_Alloc(heapID, sizeof(FrontierParticleSystem));
    MI_CpuClear8(fps, sizeof(FrontierParticleSystem));

    fps->heapID = heapID;
    sub_02014DA0();

    return fps;
}

void ov80_02239980(FrontierParticleSystem *fps) {
    for (int i = 0; i < 8; i++) {
        if (fps->particleSys[i] != 0L) {
            ov80_02239A98(fps->particleSys[i]);
        }
    }

    Heap_Free(fps);
}
