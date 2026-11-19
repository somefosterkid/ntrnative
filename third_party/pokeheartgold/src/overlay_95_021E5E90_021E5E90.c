#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "library/spl_emitter.h"
#include "heap.h"
#include "particle_system.h"
#include "egg_hatch_particle_system.h"
#include "overlay_95_egg_hatch_app.h"

struct ParticleSystem;

struct SPLEmitter;

enum CameraProjection {
    CAMERA_PROJECTION_PERSPECTIVE = 0,
    CAMERA_PROJECTION_ORTHOGRAPHIC,
    CAMERA_PROJECTION_COUNT
};

void sub_02015528(ParticleSystem *particleSystem, enum CameraProjection projection);

ParticleSystem *ov95_021E5E18(enum HeapID heapID, int narcID, int narcMemberIdx);

ParticleSystem *NewParticleSystem(enum HeapID heapID);

EggHatchParticleSystem *ov95_021E5E90(EggHatchParticleSystemArgs *args);

EggHatchParticleSystem *ov95_021E5E90(EggHatchParticleSystemArgs *args) {
    EggHatchParticleSystem *eps = Heap_Alloc(args->heapID, sizeof(EggHatchParticleSystem));

    GF_ASSERT(eps != NULL);

    eps->args = *args;
    eps->ps = ov95_021E5E18(eps->args.heapID, (enum NarcId)116, eps->args.narcIdx);

    sub_02015528(eps->ps, CAMERA_PROJECTION_ORTHOGRAPHIC);

    return eps;
}
