#include "global.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "camera.h"
#include "filesystem_files_def.h"
#include "heap.h"

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

enum VRAMAutoRelease {
    VRAM_AUTO_RELEASE_NONE = 0,

    VRAM_AUTO_RELEASE_TEXTURE_FRM = (1 << 0),
    VRAM_AUTO_RELEASE_TEXTURE_LNK = (1 << 1),
    VRAM_AUTO_RELEASE_PALETTE_FRM = (1 << 2),
    VRAM_AUTO_RELEASE_PALETTE_LNK = (1 << 3),
};

typedef struct ParticleSystem {
    SPLManager *manager;
    void *resource;
    SPLEmitter *lastAddedEmitter;
    void *heapStart;
    void *heap;
    void *heapEnd;
    SPLTexVRAMAllocFunc texAllocFunc;
    SPLPalVRAMAllocFunc palAllocFunc;
    Camera *camera;
    VecFx32 sLotoId;
    u16 cameraFov;
    VecFx32 cameraPos;
    VecFx32 cameraUp;
    VecFx32 cameraTarget;
    union {
        NNSGfdFrmTexVramState textureVRAMState;
        NNSGfdTexKey textureKeys[16];
    };
    union {
        NNSGfdFrmPlttVramState paletteVRAMState;
        NNSGfdPlttKey paletteKeys[16];
    };
    u8 vramAutoRelease;
    u8 unused2;
    u8 id;
    u8 cameraProjection;
} ParticleSystem;

void sub_020154B8(ParticleSystem *particleSystem);

#include "particle_system_globals.h"

void sub_02014EBC(ParticleSystem *particleSystem);

void sub_02014EBC(ParticleSystem *particleSystem) {
    sub_020154B8(particleSystem);

    if (particleSystem->vramAutoRelease & VRAM_AUTO_RELEASE_TEXTURE_FRM) {
        NNS_GfdSetFrmTexVramState(&particleSystem->textureVRAMState);
    } else if (particleSystem->vramAutoRelease & VRAM_AUTO_RELEASE_TEXTURE_LNK) {
        for (int i = 0; i < 16; i++) {
            if (particleSystem->textureKeys[i] != (u32)0x0) {
                NNS_GfdFreeLnkTexVram(particleSystem->textureKeys[i]);
                particleSystem->textureKeys[i] = (u32)0x0;
            }
        }
    }

    if (particleSystem->vramAutoRelease & (1 << 2)) {
        NNS_GfdSetFrmPlttVramState(&particleSystem->paletteVRAMState);
    } else if (particleSystem->vramAutoRelease & (1 << 3)) {
        for (int i = 0; i < 16; i++) {
            if (particleSystem->paletteKeys[i] != (u32)0x0) {
                NNS_GfdFreeLnkPlttVram(particleSystem->paletteKeys[i]);
                particleSystem->paletteKeys[i] = (u32)0x0;
            }
        }
    }

    particleSystem->vramAutoRelease = VRAM_AUTO_RELEASE_NONE;
    particleSystem->lastAddedEmitter = 0L;

    if (particleSystem->resource != 0L) {
        Heap_Free(particleSystem->resource);
        particleSystem->resource = 0L;
    }

    for (int i = 0; i < 16; i++) {
        if (_021D10A8[i] == particleSystem) {
            _021D10A8[i] = 0L;
            break;
        }
    }

    if (particleSystem->camera != 0L) {
        Camera_Delete(particleSystem->camera);
    }

#if defined(SDK_PORT) || defined(NONMATCHING)
    {
        // the doubled work block sub_02014DB4 took on the host: its address is in the caller's buffer (heapStart), which the caller
        // frees after this
        void **block = particleSystem->heapStart;

        Heap_Free(*block);
    }
#endif
    Heap_Free(particleSystem);
}
