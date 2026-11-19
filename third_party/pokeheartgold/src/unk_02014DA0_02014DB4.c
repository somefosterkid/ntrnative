#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "camera.h"
#include "error_handling.h"
#include "heap.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
// The work area holds the SPL manager, emitters, particles and resources: structs with pointers, so on a 64-bit host it needs twice
// its DS size. Callers pass the DS size (they allocate and free their buffer as on the DS); sub_02014DB4 takes the doubled block
// itself and sub_02014EBC frees it.
#define PARTICLE_HEAP_HOST_SIZE(size) ((size) * 2)
#endif

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

/* local copy of struct ParticleSystem (src/overlay_70_02238F24.c) */
typedef struct ParticleSystem {
    SPLManager *manager;
    void *resource;
    SPLEmitter *lastAddedEmitter;
    void *heapStart;
    void *heap;
    void *heapEnd;
    void *texAllocFunc;
    void *palAllocFunc;
    Camera *camera;
    VecFx32 sLotoId;
    u16 cameraFov;
    u8 pad_32[2];
    VecFx32 cameraPos;
    VecFx32 cameraUp;
    VecFx32 cameraTarget;
    u32 texKeys[16];
    u32 plttKeys[16];
    u8 vramAutoRelease;
    u8 unused2;
    u8 id;
    u8 cameraProjection;
} ParticleSystem;

extern VecFx32 _020F6078;

extern VecFx32 _020F6084;

extern VecFx32 _020F6090;

extern SPLAlloc _020F609C[];

#include "particle_system_globals.h"

SPLManager *SPLManager_New(SPLAlloc alloc, u16 maxEmitters, u16 maxParticles, u16 fixPolyID, u16 minPolyID, u16 maxPolyID);

void sub_020154E4(ParticleSystem *particleSystem, VecFx32 *up);

ParticleSystem *sub_02014DB4(SPLTexVRAMAllocFunc texAllocFunc, SPLPalVRAMAllocFunc palAllocFunc, void *heap, int heapSize, BOOL hasCamera, enum HeapID heapID);

ParticleSystem *sub_02014DB4(SPLTexVRAMAllocFunc texAllocFunc, SPLPalVRAMAllocFunc palAllocFunc, void *heap, int heapSize, BOOL hasCamera, enum HeapID heapID) {
    ParticleSystem *particleSystem;
    int id;

    for (id = 0; id < 16; id++) {
        if (_021D10A8[id] == NULL) {
            break;
        }
    }
    if (id >= 16) {
        return NULL;
    }

    particleSystem = Heap_Alloc(heapID, sizeof(ParticleSystem));
    if (particleSystem == NULL) {
        GF_AssertFail();
    }
    memset(particleSystem, 0, sizeof(ParticleSystem));

    particleSystem->texAllocFunc = texAllocFunc;
    particleSystem->palAllocFunc = palAllocFunc;
    particleSystem->cameraPos = _020F6090;
    particleSystem->cameraUp = _020F6078;
    particleSystem->cameraTarget = _020F6084;

#if defined(SDK_PORT) || defined(NONMATCHING)
    {
        // The caller's DS-sized buffer shrinks to one pointer, the address of the doubled work block taken from the same heap. It
        // stays heapStart: sub_020154D0 returns it and the caller frees it after sub_02014EBC, which frees the block first.
        void **block = heap;
        void *workArea;

        Heap_Realloc(heap, sizeof(void *));
        workArea = Heap_Alloc(heapID, PARTICLE_HEAP_HOST_SIZE(heapSize));
        if (workArea == NULL) {
            GF_AssertFail();
        }
        *block = workArea;
        memset(workArea, 0, PARTICLE_HEAP_HOST_SIZE(heapSize));
        particleSystem->heapStart = heap;
        particleSystem->heap = workArea;
        particleSystem->heapEnd = (void *)((u8 *)workArea + PARTICLE_HEAP_HOST_SIZE(heapSize));
    }
#else
    memset(heap, 0, heapSize);
    particleSystem->heapStart = heap;
    particleSystem->heap = heap;
    particleSystem->heapEnd = (void *)((u8 *)heap + heapSize);
#endif
    particleSystem->id = id;
    _021D10A8[id] = particleSystem;

    if (hasCamera == TRUE) {
        particleSystem->camera = Camera_New(heapID);
        particleSystem->sLotoId.x = 0;
        particleSystem->sLotoId.y = 0;
        particleSystem->sLotoId.z = 0;
        particleSystem->cameraFov = 0x2000;
        Camera_Init_FromTargetAndPos(&_020F6084, &_020F6090, particleSystem->cameraFov, 0, FALSE, particleSystem->camera);
        particleSystem->cameraProjection = 0;
        Camera_SetStaticPtr(particleSystem->camera);
    }

    particleSystem->manager = SPLManager_New(_020F609C[id], 20, 200, 5, 6, 0x3f);
    sub_020154E4(particleSystem, &_020F6078);
    return particleSystem;
}
