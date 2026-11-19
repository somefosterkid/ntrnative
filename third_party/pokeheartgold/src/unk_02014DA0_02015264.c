#include "global.h"

#include "sys_task.h"
#include "constants/heap.h"
#include "library/spl_emitter.h"
#include "library/spl_manager.h"
#include "camera.h"
#include "filesystem_files_def.h"
#include "sys_task_api.h"

void * AllocAndReadWholeNarcMemberByIdPair(NarcId narc_id, s32 file_id, enum HeapID heapID);

void * sub_02015264(NarcId narc_id, s32 file_id, enum HeapID heapID);

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
void sub_02015300(ParticleSystem *particleSystem);
void sub_02015340(SysTask *task, void *param);
void sub_0201526C(ParticleSystem *particleSystem, void *resource, enum VRAMAutoRelease autoRelease, BOOL uploadImmediately);

void * sub_02015264(NarcId narc_id, s32 file_id, enum HeapID heapID) {
    return AllocAndReadWholeNarcMemberByIdPair(narc_id, file_id, heapID);
}

void sub_0201526C(ParticleSystem *particleSystem, void *resource, enum VRAMAutoRelease autoRelease, BOOL uploadImmediately) {
    ((particleSystem->manager != 0L) ? (void)0 : GF_AssertFail());
    ((particleSystem->resource == 0L) ? (void)0 : GF_AssertFail());

    particleSystem->vramAutoRelease = autoRelease;

    if (autoRelease & VRAM_AUTO_RELEASE_TEXTURE_FRM) {
        NNS_GfdGetFrmTexVramState(&particleSystem->textureVRAMState);
    } else if (autoRelease & VRAM_AUTO_RELEASE_TEXTURE_LNK) {
        for (int i = 0; i < 16; i++) {
            particleSystem->textureKeys[i] = (u32)0x0;
        }
    }

    if (autoRelease & VRAM_AUTO_RELEASE_PALETTE_FRM) {
        NNS_GfdGetFrmPlttVramState(&particleSystem->paletteVRAMState);
    } else if (autoRelease & VRAM_AUTO_RELEASE_PALETTE_LNK) {
        for (int i = 0; i < 16; i++) {
            particleSystem->paletteKeys[i] = (u32)0x0;
        }
    }

    particleSystem->resource = resource;

    if (uploadImmediately == 1) {
        sub_02015300(particleSystem);
    } else {
        SysTask_CreateOnVWaitQueue(sub_02015340, particleSystem, 5);
    }
}
