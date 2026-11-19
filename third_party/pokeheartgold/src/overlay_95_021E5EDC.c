#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"

#include "sys_task.h"
#include "particle_system.h"
#include "overlay_95_egg_hatch_app.h"

struct ParticleSystem;
s32 sub_020154B0(ParticleSystem *particleSystem);
void Heap_Free(void *ptr);

BOOL ov95_021E5EDC(EggHatchParticleSystem *param0);
void ov95_021E5EF0(EggHatchParticleSystem *ptr);

BOOL ov95_021E5EDC(EggHatchParticleSystem *param0) {
    EggHatchParticleSystem *v0 = param0;

    if (sub_020154B0(v0->ps) == 0) {
        return 0;
    }

    return 1;
}

void ov95_021E5EF0(EggHatchParticleSystem *ptr) {
    Heap_Free(ptr);
}
