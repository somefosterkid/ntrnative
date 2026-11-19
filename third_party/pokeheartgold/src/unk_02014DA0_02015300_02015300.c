#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "camera.h"
#include "filesystem_files_def.h"
#include "particle_system.h"

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

void SPLManager_LoadResources(SPLManager *mgr, const void *data);

BOOL SPLManager_UploadTexturesEx(SPLManager *mgr, SPLTexVRAMAllocFunc vramAlloc);

BOOL SPLManager_UploadPalettesEx(SPLManager *mgr, SPLPalVRAMAllocFunc vramAlloc);

BOOL SPLManager_UploadTextures(SPLManager *mgr);

BOOL SPLManager_UploadPalettes(SPLManager *mgr);

#include "particle_system_globals.h"

void sub_02015300(ParticleSystem *particleSystem);

void sub_02015300(ParticleSystem *particleSystem) {
    SPLManager_LoadResources(particleSystem->manager, particleSystem->resource);

    _021D10A0.current = particleSystem;

    if (particleSystem->texAllocFunc == NULL) {
        SPLManager_UploadTextures(particleSystem->manager);
    } else {
        SPLManager_UploadTexturesEx(particleSystem->manager, particleSystem->texAllocFunc);
    }

    if (particleSystem->palAllocFunc == NULL) {
        SPLManager_UploadPalettes(particleSystem->manager);
    } else {
        SPLManager_UploadPalettesEx(particleSystem->manager, particleSystem->palAllocFunc);
    }

    _021D10A0.current = NULL;
}
