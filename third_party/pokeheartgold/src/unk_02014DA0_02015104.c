#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "camera.h"
#include "error_handling.h"
#include "filesystem_files_def.h"
#include "particle_system.h"

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

static inline void *ParticleSystem_AllocMemory(ParticleSystem *particleSystem, u32 size);

static inline void *ParticleSystem_AllocMemory(ParticleSystem *particleSystem, u32 size) {
    void *allocated = particleSystem->heap;

#if defined(SDK_PORT) || defined(NONMATCHING)
    u8 *newHeapPos = (u8 *)particleSystem->heap + size;
    u32 offset = (u32)((uintptr_t)newHeapPos % 4);
    if (offset > 0) {
        newHeapPos += 4 - offset;
    }

    particleSystem->heap = newHeapPos;
#else
    u32 newHeapPos = (u32)particleSystem->heap + size;
    u32 offset = newHeapPos % 4;
    if (offset > 0) {
        newHeapPos += 4 - offset;
    }

    particleSystem->heap = (void *)newHeapPos;
#endif
    if (particleSystem->heap >= particleSystem->heapEnd) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    return allocated;
}

void *sub_02015104(u32 size);

#include "particle_system_globals.h"

void *sub_02015104(u32 size) {
    return ParticleSystem_AllocMemory(_021D10A0.systems[8], size);
}

void *sub_02015130(u32 size);

void *sub_02015130(u32 size) {
    return ParticleSystem_AllocMemory(_021D10A0.systems[9], size);
}

void *sub_0201515C(u32 size);

void *sub_0201515C(u32 size) {
    return ParticleSystem_AllocMemory(_021D10A0.systems[10], size);
}

void *sub_02015188(u32 size);

void *sub_02015188(u32 size) {
    return ParticleSystem_AllocMemory(_021D10A0.systems[11], size);
}

void *sub_020151B4(u32 size);

void *sub_020151B4(u32 size) {
    return ParticleSystem_AllocMemory(_021D10A0.systems[12], size);
}
