#include "global.h"

#include "error_handling.h"
#include "sys_task_api.h"
#include "unk_02014DA0.h"
#include "constants/heap.h"
#include "library/spl_emitter.h"
#include "library/spl_manager.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem_files_def.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "particle_system.h"

s32 sub_02015300(ParticleSystem *);

void sub_02015340(SysTask *arg0, void *arg1);

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);
typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);
void SPLManager_Draw(SPLManager *mgr, const MtxFx43 *viewMatrix);
void SPLManager_Update(Sprite *gfxElem);
typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;
typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;
void sub_020153D8(ParticleSystem *particleSystem);
void sub_02015414(ChooseStarterCursor *cursor);

void sub_02015340(SysTask *task, void *arg1) {
    sub_02015300(arg1);
    SysTask_Destroy(task);
}

#include "particle_system_globals.h"

void sub_02015354(u32 texKey) {
    s32 v0;
    ParticleSystem *ps;

    if (texKey == 0) {
        GF_AssertFail();
    }
    if (_021D10A0.current == NULL) {
        GF_AssertFail();
    }
    ps = _021D10A0.current;
    for (v0 = 0; v0 < 16; v0++) {
        if (ps->texKeys[v0] == 0) {
            _021D10A0.current->texKeys[v0] = texKey;
            return;
        }
    }
    GF_AssertFail();
}

void sub_02015394(u32 plttKey) {
    s32 v0;
    ParticleSystem *ps;

    if (plttKey == 0) {
        GF_AssertFail();
    }
    if (_021D10A0.current == NULL) {
        GF_AssertFail();
    }
    ps = _021D10A0.current;
    for (v0 = 0; v0 < 16; v0++) {
        if (ps->plttKeys[v0] == 0) {
            _021D10A0.current->plttKeys[v0] = plttKey;
            return;
        }
    }
    GF_AssertFail();
}

void sub_020153D8(ParticleSystem *particleSystem) {
    if (particleSystem->camera != 0L) {
        Camera_ApplyPerspectiveType(particleSystem->cameraProjection, particleSystem->camera);
        Camera_SetStaticPtr(particleSystem->camera);
        Camera_PushLookAtToNNSGlb();
    }

    NNS_G3dGlbFlush();

    const MtxFx43 *viewMatrix = NNS_G3dGlbGetCameraMtx();
    SPLManager_Draw(particleSystem->manager, viewMatrix);

    if (particleSystem->camera != 0L) {
        Camera_UnsetStaticPtr();
    }

    NNS_G3dGlbFlush();
}

void sub_02015414(ChooseStarterCursor *cursor) {
    SPLManager_Update(cursor->sprite);
}
