#include "global.h"

#include "error_handling.h"
#include "particle_system.h"

struct ParticleSystem;

enum CameraProjection {
    CAMERA_PROJECTION_PERSPECTIVE = 0,
    CAMERA_PROJECTION_ORTHOGRAPHIC,
    CAMERA_PROJECTION_COUNT
};

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

#include "particle_system_globals.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "camera.h"
#include "library/spl_emitter.h"
#include "library/spl_manager.h"

static inline void *ParticleSystem_AllocMemory(ParticleSystem *particleSystem, u32 size);

ParticleSystem *sub_02014DB4(SPLTexVRAMAllocFunc texAllocFunc, SPLPalVRAMAllocFunc palAllocFunc, void *heap, int heapSize, BOOL hasCamera, enum HeapID heapID);

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

void *sub_02015264(enum NarcId narcID, void *memberIndex, enum HeapID heapID);

void sub_02015528(ParticleSystem *particleSystem, enum CameraProjection projection);

void *sub_020150AC(u32 size);

void *sub_020150AC(u32 size) {
    return ParticleSystem_AllocMemory(_021D10A0.systems[6], size);
}

void *sub_020150D8(u32 size);

void *sub_020150D8(u32 size) {
    return ParticleSystem_AllocMemory(_021D10A0.systems[7], size);
}
