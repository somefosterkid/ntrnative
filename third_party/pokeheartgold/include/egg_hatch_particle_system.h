#ifndef POKEHEARTGOLD_EGG_HATCH_PARTICLE_SYSTEM_H
#define POKEHEARTGOLD_EGG_HATCH_PARTICLE_SYSTEM_H

#include "constants/heap.h"

struct ParticleSystem;
struct SPLEmitter;

typedef struct EggHatchParticleSystemArgs {
    enum HeapID heapID;
    int narcIdx;
} EggHatchParticleSystemArgs;

// The particle system the egg hatch scene and the evolution scene share (sub_02077604 makes it, sub_02077634 starts an
// emitter, sub_02077664 deletes it; overlay 95 has its own copies of the three: ov95_021E5E90 / 021E5EC0 / 021E5EF0).
// sizeof == 0x10 on the DS; the evolution task (EvolutionData::unk_30) and EggHatchGraphics::eps hold one.
typedef struct EggHatchParticleSystem {
    EggHatchParticleSystemArgs args;
    struct SPLEmitter *emitter;
    struct ParticleSystem *ps;
} EggHatchParticleSystem;

#endif // POKEHEARTGOLD_EGG_HATCH_PARTICLE_SYSTEM_H
