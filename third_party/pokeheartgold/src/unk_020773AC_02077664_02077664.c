#include "global.h"

#include "egg_hatch_particle_system.h"
#include "heap.h"
#include "unk_020773AC.h"

s32 sub_020775AC(void *);

void sub_02077664(void *ptr) {
    sub_020775AC(((EggHatchParticleSystem *)ptr)->ps);
    Heap_Free(ptr);
}
