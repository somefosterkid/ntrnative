// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/particle.c.
#include "types.h"
#include "gfl/arc.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nnsys/g3d.h"
#include "nnsys/gfd.h"
#include "spl/spl.h"

// Particle systems over SPL managers: each allocates from its own work memory through one of 16 allocators, and gives
// back the VRAM its resource took when freed

// Most emitters and particles a system has at once
#define PARTICLE_EMITTER_MAX 24
#define PARTICLE_MAX 200

// The kinds of an emitter's behaviors, as func_020518ac finds them
enum {
    PARTICLE_BEHAVIOR_GRAVITY,
    PARTICLE_BEHAVIOR_RANDOM,
    PARTICLE_BEHAVIOR_MAGNET,
    PARTICLE_BEHAVIOR_SPIN,
    PARTICLE_BEHAVIOR_COLLISION_PLANE,
    PARTICLE_BEHAVIOR_CONVERGENCE,
};

typedef struct {
    ParticleSystem *systems[PARTICLE_SYSTEM_MAX];
    // What func_020515cc's callback reads through func_02051774
    void *callbackParam;
    // The system whose resource is being uploaded, which the linked-list VRAM allocators record keys in
    ParticleSystem *uploading;
} ParticleGlobal;

static ParticleGlobal *data_02146a88;

static void *func_02050f1c(u32 size);
static void *func_02050f4c(u32 size);
static void *func_02050f7c(u32 size);
static void *func_02050fac(u32 size);
static void *func_02050fdc(u32 size);
static void *func_0205100c(u32 size);
static void *func_0205103c(u32 size);
static void *func_0205106c(u32 size);
static void *func_0205109c(u32 size);
static void *func_020510cc(u32 size);
static void *func_020510fc(u32 size);
static void *func_0205112c(u32 size);
static void *func_0205115c(u32 size);
static void *func_0205118c(u32 size);
static void *func_020511bc(u32 size);
static void *func_020511ec(u32 size);
static void func_020513f8(TCB *tcb, void *data);
static const void *func_020518ac(SPLEmitter *emitter, int type);
static u32 func_02051a3c(u32 size, BOOL is4x4comp);
static u32 func_02051a54(u32 size, BOOL is4x4comp);
static u32 func_02051aa0(u32 size, BOOL is4pltt);
static u32 func_02051ab8(u32 size, BOOL is4pltt);

static const VecFx32 data_020a159c = { FX32_ONE, FX32_ONE, FX32_ONE };
static const VecFx32 data_020a1590 = { 0, 0, 4 * FX32_ONE };
static const VecFx32 data_020a1584 = { 0, FX32_ONE, 0 };
static const VecFx32 data_020a1578 = { 0, 0, 0 };
static const SPLAllocFunc data_020a15a8[PARTICLE_SYSTEM_MAX] = {
    func_02050f1c, func_02050f4c, func_02050f7c, func_02050fac, func_02050fdc, func_0205100c,
    func_0205103c, func_0205106c, func_0205109c, func_020510cc, func_020510fc, func_0205112c,
    func_0205115c, func_0205118c, func_020511bc, func_020511ec,
};

void func_02050b80(HeapID heapId) {
    int i;

    data_02146a88 = GFL_HeapAllocate(heapId, sizeof(ParticleGlobal), FALSE, "particle.c", 190);
    for (i = 0; i < PARTICLE_SYSTEM_MAX; i++) {
        data_02146a88->systems[i] = NULL;
    }
    data_02146a88->callbackParam = NULL;
    data_02146a88->uploading = NULL;
}

void func_02050be8(void) {
    if (func_02051504() != 0) {
        func_02051534();
        func_02051574();
    }
}

ParticleSystem *func_02050c04(void *work, u32 size, BOOL camera, u32 heapId) {
    return func_02050c30(work, size, camera, 5, 6, 63, heapId);
}

ParticleSystem *func_02050c30(void *work, u32 size, BOOL camera, u32 fixPolyID, u32 minPolyID, u32 maxPolyID,
                              u32 heapId) {
    ParticleSystem *system;
    int id;

    for (id = 0; id < PARTICLE_SYSTEM_MAX; id++) {
        if (data_02146a88->systems[id] == NULL) {
            break;
        }
    }
    if (id >= PARTICLE_SYSTEM_MAX) {
        return NULL;
    }
    system = GFL_HeapAllocate(heapId, sizeof(ParticleSystem), FALSE, "particle.c", 285);
    MI_CpuFill8(system, 0, sizeof(ParticleSystem));
    system->cameraPos = data_020a1590;
    system->cameraUp = data_020a1584;
    system->cameraTarget = data_020a1578;
    system->extResource = FALSE;
    MI_CpuFill8(work, 0, size);
    system->workStart = work;
    system->work = work;
    system->workEnd = (u8 *)work + size;
    system->id = id;
    data_02146a88->systems[id] = system;
    if (camera == TRUE) {
        func_02051648(system, NULL, 2 * FX32_ONE, NULL, NULL, NULL, heapId);
    }
    system->manager =
        func_02052dc8(data_020a15a8[id], PARTICLE_EMITTER_MAX, PARTICLE_MAX, fixPolyID, minPolyID, maxPolyID);
    return system;
}

static inline void Particle_Remove(ParticleSystem *system) {
    int i;

    for (i = 0; i < PARTICLE_SYSTEM_MAX; i++) {
        if (data_02146a88->systems[i] == system) {
            data_02146a88->systems[i] = NULL;
            break;
        }
    }
}

void func_02050db8(ParticleSystem *system) {
    int i;

    func_02051620(system);
    if (system->vramRelease & PARTICLE_VRAM_TEX_FRM) {
        NNS_GfdSetFrmTexVramState(&system->texState);
    } else if (system->vramRelease & PARTICLE_VRAM_TEX_LNK) {
        for (i = 0; i < PARTICLE_VRAM_KEY_MAX; i++) {
            if (system->texKeys[i] != 0) {
                NNS_GfdFreeLnkTexVram(system->texKeys[i]);
                system->texKeys[i] = 0;
            }
        }
    }
    if (system->vramRelease & PARTICLE_VRAM_PLTT_FRM) {
        NNS_GfdSetFrmPlttVramState(&system->plttState);
    } else if (system->vramRelease & PARTICLE_VRAM_PLTT_LNK) {
        for (i = 0; i < PARTICLE_VRAM_KEY_MAX; i++) {
            if (system->plttKeys[i] != 0) {
                NNS_GfdFreeLnkPlttVram(system->plttKeys[i]);
                system->plttKeys[i] = 0;
            }
        }
    }
    system->vramRelease = 0;
    system->lastEmitter = NULL;
    if (system->resource != NULL && !system->extResource) {
        GFL_HeapFree(system->resource);
        system->resource = NULL;
    }
    Particle_Remove(system);
    if (system->camera != NULL) {
        GFL_G3DCameraFree(system->camera);
    }
    GFL_HeapFree(system);
}

void func_02050ed4(void) {
    int i;

    for (i = 0; i < PARTICLE_SYSTEM_MAX; i++) {
        if (data_02146a88->systems[i] != NULL) {
            func_02050db8(data_02146a88->systems[i]);
        }
    }
    GFL_HeapFree(data_02146a88);
    data_02146a88 = NULL;
}

// Takes size bytes of the system's work, rounded up to a word
static inline void *Particle_Alloc(ParticleSystem *system, u32 size) {
    void *ret = system->work;
    uintptr_t next = (uintptr_t)system->work + size;

    if (next % 4 != 0) {
        next += 4 - next % 4;
    }
    system->work = (void *)next;
    return ret;
}

static void *func_02050f1c(u32 size) {
    return Particle_Alloc(data_02146a88->systems[0], size);
}

static void *func_02050f4c(u32 size) {
    return Particle_Alloc(data_02146a88->systems[1], size);
}

static void *func_02050f7c(u32 size) {
    return Particle_Alloc(data_02146a88->systems[2], size);
}

static void *func_02050fac(u32 size) {
    return Particle_Alloc(data_02146a88->systems[3], size);
}

static void *func_02050fdc(u32 size) {
    return Particle_Alloc(data_02146a88->systems[4], size);
}

static void *func_0205100c(u32 size) {
    return Particle_Alloc(data_02146a88->systems[5], size);
}

static void *func_0205103c(u32 size) {
    return Particle_Alloc(data_02146a88->systems[6], size);
}

static void *func_0205106c(u32 size) {
    return Particle_Alloc(data_02146a88->systems[7], size);
}

static void *func_0205109c(u32 size) {
    return Particle_Alloc(data_02146a88->systems[8], size);
}

static void *func_020510cc(u32 size) {
    return Particle_Alloc(data_02146a88->systems[9], size);
}

static void *func_020510fc(u32 size) {
    return Particle_Alloc(data_02146a88->systems[10], size);
}

static void *func_0205112c(u32 size) {
    return Particle_Alloc(data_02146a88->systems[11], size);
}

static void *func_0205115c(u32 size) {
    return Particle_Alloc(data_02146a88->systems[12], size);
}

static void *func_0205118c(u32 size) {
    return Particle_Alloc(data_02146a88->systems[13], size);
}

static void *func_020511bc(u32 size) {
    return Particle_Alloc(data_02146a88->systems[14], size);
}

static void *func_020511ec(u32 size) {
    return Particle_Alloc(data_02146a88->systems[15], size);
}

void *func_0205121c(u32 arcId, u32 fileId, u32 heapId) {
    return GFL_ArcSysReadHeapNew(arcId, fileId, heapId);
}

void func_02051230(ParticleSystem *system, void *resource, BOOL immediate, TCBManager *tcbMgr) {
    int release;
    int i;

    release = func_02046bfc() == TRUE ? PARTICLE_VRAM_TEX_FRM : PARTICLE_VRAM_TEX_LNK;
    release |= func_02046c10() == TRUE ? PARTICLE_VRAM_PLTT_FRM : PARTICLE_VRAM_PLTT_LNK;
    system->vramRelease = release;
    if (release & PARTICLE_VRAM_TEX_FRM) {
        NNS_GfdGetFrmTexVramState(&system->texState);
        system->texAllocFunc = func_02051a3c;
    } else if (release & PARTICLE_VRAM_TEX_LNK) {
        for (i = 0; i < PARTICLE_VRAM_KEY_MAX; i++) {
            system->texKeys[i] = 0;
        }
        system->texAllocFunc = func_02051a54;
        system->texKeyCount = 0;
    }
    if (release & PARTICLE_VRAM_PLTT_FRM) {
        NNS_GfdGetFrmPlttVramState(&system->plttState);
        system->palAllocFunc = func_02051aa0;
    } else if (release & PARTICLE_VRAM_PLTT_LNK) {
        for (i = 0; i < PARTICLE_VRAM_KEY_MAX; i++) {
            system->plttKeys[i] = 0;
        }
        system->palAllocFunc = func_02051ab8;
        system->plttKeyCount = 0;
    }
    system->resource = resource;
    func_02052a50(system->manager, resource);
    if (immediate == TRUE) {
        func_02051394(system);
    } else if (tcbMgr != NULL) {
        GFL_TCBMgrAddTask(tcbMgr, func_020513f8, system, 0);
    }
}

void func_02051360(ParticleSystem *system, void *resource, BOOL immediate, TCBManager *tcbMgr) {
    func_02051230(system, resource, immediate, tcbMgr);
    system->extResource = TRUE;
}

void func_02051380(ParticleSystem *system, void *resource) {
    func_02051230(system, resource, FALSE, NULL);
}

void func_02051394(ParticleSystem *system) {
    data_02146a88->uploading = system;
    if (system->texAllocFunc == NULL) {
        func_020528bc(system->manager);
    } else {
        func_02052984(system->manager, system->texAllocFunc);
    }
    if (system->palAllocFunc == NULL) {
        func_020528a8(system->manager);
    } else {
        func_020528d0(system->manager, system->palAllocFunc);
    }
    data_02146a88->uploading = NULL;
}

static void func_020513f8(TCB *tcb, void *data) {
    func_02051394(data);
    GFL_TCBRemove(tcb);
}

void func_02051414(ParticleSystem *system) {
    G3DCameraProjection projection;
    FxLookAt lookAt;

    if (system->camera != NULL) {
        VecFx32 scale;
        MtxFx33 rot;

        // Draw with the system's camera, from the origin
        GFL_G3DSysMtxGetProjection(&projection);
        GFL_G3DSysMtxGetViewLookAt(&lookAt);
        scale = data_020a159c;
        {
            VecFx32 trans = {0, 0, 0};

            MTX_Identity33_(&rot);
            NNS_G3dGlbSetBaseTrans(&trans);
            NNS_G3dGlbSetBaseRot(&rot);
            NNS_G3dGlbSetBaseScale(&scale);
        }
        GFL_G3DCameraFlush(system->camera);
        GFL_G3DSysMtxViewFlush();
        NNS_G3dGeFlushBuffer();
    }
    func_020526a8(system->manager, &NNS_G3dGlb.cameraMtx);
    if (system->camera != NULL) {
        GFL_G3DSysMtxSetProjection(&projection);
        GFL_G3DSysMtxSetViewLookAt(&lookAt);
        GFL_G3DSysMtxViewFlush();
        NNS_G3dGeFlushBuffer();
    }
}

void func_020514f4(ParticleSystem *system) {
    func_02052770(system->manager);
}

int func_02051504(void) {
    int count = 0;
    int i;

    for (i = 0; i < PARTICLE_SYSTEM_MAX; i++) {
        if (data_02146a88->systems[i] != NULL) {
            count++;
        }
    }
    return count;
}

int func_02051534(void) {
    int i;
    int count = 0;

    for (i = 0; i < PARTICLE_SYSTEM_MAX; i++) {
        if (data_02146a88->systems[i] != NULL) {
            func_02051414(data_02146a88->systems[i]);
            count++;
        }
    }
    return count;
}

int func_02051574(void) {
    int i;
    int count = 0;

    for (i = 0; i < PARTICLE_SYSTEM_MAX; i++) {
        if (data_02146a88->systems[i] != NULL) {
            func_020514f4(data_02146a88->systems[i]);
            count++;
        }
    }
    return count;
}

SPLEmitter *func_020515b4(ParticleSystem *system, int resourceId, const VecFx32 *pos) {
    SPLEmitter *emitter = func_02052634(system->manager, resourceId, pos);

    system->lastEmitter = emitter;
    return emitter;
}

SPLEmitter *func_020515cc(ParticleSystem *system, int resourceId, SPLEmitterCallback callback, void *param) {
    SPLEmitter *emitter;

    if (system->manager->activeEmitters.count >= PARTICLE_EMITTER_MAX) {
        return (SPLEmitter *)-1;
    }
    data_02146a88->callbackParam = param;
    emitter = func_020525a0(system->manager, resourceId, callback);
    data_02146a88->callbackParam = NULL;
    system->lastEmitter = emitter;
    return emitter;
}

int func_02051614(ParticleSystem *system) {
    return system->manager->activeEmitters.count;
}

void func_02051620(ParticleSystem *system) {
    func_020524c8(system->manager);
}

void func_02051630(ParticleSystem *system, SPLEmitter *emitter) {
    func_0205250c(system->manager, emitter);
}

void *func_02051640(ParticleSystem *system) {
    return system->workStart;
}

void func_02051648(ParticleSystem *system, G3DCameraProjection *projection, u16 fov, const VecFx32 *position,
                   const VecFx32 *upVector, const VecFx32 *target, u32 heapId) {
    system->unk24.x = 0;
    system->unk24.y = 0;
    system->unk24.z = 0;
    system->cameraFov = fov;
    if (position == NULL) {
        position = &data_020a1590;
    }
    if (upVector == NULL) {
        upVector = &data_020a1584;
    }
    if (target == NULL) {
        target = &data_020a1578;
    }
    if (projection != NULL) {
        system->cameraProjection = projection->type;
        system->camera = GFL_G3DCameraCreate(projection->type, projection->param1, projection->param2,
                                             projection->param3, projection->param4, projection->near,
                                             projection->far, projection->ndcRangeOverride, position, upVector,
                                             target, heapId);
    } else {
        system->cameraProjection = G3DCAM_PROJECTION_PERSPECTIVE;
        // 45 degrees across a 4:3 view
        system->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, 0xb50, 0xb50, 0x1555, 0, FX32_ONE,
                                             900 * FX32_ONE, 0, position, upVector, target, heapId);
    }
}

void func_02051758(ParticleSystem *system) {
    GFL_G3DCameraFree(system->camera);
    system->camera = NULL;
}

void *func_02051774(void) {
    return data_02146a88->callbackParam;
}

G3DCamera *func_02051788(ParticleSystem *system) {
    return system->camera;
}

void func_02051790(ParticleSystem *system, VecFx32 *pos) {
    *pos = system->cameraPos;
}

void func_020517a4(ParticleSystem *system, const VecFx32 *pos) {
    system->cameraPos = *pos;
}

void func_020517b4(SPLEmitter *emitter, VecFx16 *axis) {
    *axis = emitter->axis;
}

fx32 func_020517d0(SPLEmitter *emitter) {
    return emitter->radius;
}

void func_020517d8(SPLEmitter *emitter, fx32 radius) {
    emitter->radius = radius;
}

fx32 func_020517e0(SPLEmitter *emitter) {
    return emitter->length;
}

void func_020517e8(SPLEmitter *emitter, fx32 length) {
    emitter->length = length;
}

fx16 func_020517f0(SPLEmitter *emitter) {
    return emitter->initVelPositionAmplifier;
}

void func_02051800(SPLEmitter *emitter, fx32 amplifier) {
    emitter->initVelPositionAmplifier = amplifier;
}

fx16 func_02051808(SPLEmitter *emitter) {
    return emitter->initVelAxisAmplifier;
}

void func_02051818(SPLEmitter *emitter, fx32 amplifier) {
    emitter->initVelAxisAmplifier = amplifier;
}

fx16 func_02051820(SPLEmitter *emitter) {
    return emitter->baseScale;
}

u16 func_02051830(SPLEmitter *emitter) {
    return emitter->particleLifeTime;
}

void func_02051838(SPLEmitter *emitter, const VecFx32 *pos) {
    emitter->position.x = pos->x + emitter->resource->header->emitterBasePos.x;
    emitter->position.y = pos->y + emitter->resource->header->emitterBasePos.y;
    emitter->position.z = pos->z + emitter->resource->header->emitterBasePos.z;
}

void func_02051880(SPLEmitter *emitter, const VecFx16 *axis) {
    emitter->axis = *axis;
}

void func_0205189c(SPLEmitter *emitter, fx32 scale) {
    emitter->baseScale = scale;
}

void func_020518a4(SPLEmitter *emitter, u16 lifeTime) {
    emitter->particleLifeTime = lifeTime;
}

// The parameters of the emitter's first behavior of a kind, or NULL
static const void *func_020518ac(SPLEmitter *emitter, int type) {
    int i;
    int count = emitter->resource->behaviorCount;

    if (count == 0) {
        return NULL;
    }
    for (i = 0; i < count; i++) {
        SPLBehavior *behavior = &emitter->resource->behaviors[i];

        if (behavior == NULL) {
            continue;
        }
        switch (type) {
        case PARTICLE_BEHAVIOR_GRAVITY:
            if (behavior->applyFunc == func_020582cc) {
                return behavior->object;
            }
            continue;
        case PARTICLE_BEHAVIOR_RANDOM:
            if (behavior->applyFunc == func_02058204) {
                return behavior->object;
            }
            continue;
        case PARTICLE_BEHAVIOR_MAGNET:
            if (behavior->applyFunc == func_02058180) {
                return behavior->object;
            }
            continue;
        case PARTICLE_BEHAVIOR_SPIN:
            if (behavior->applyFunc == func_020580a8) {
                return behavior->object;
            }
            continue;
        case PARTICLE_BEHAVIOR_COLLISION_PLANE:
            if (behavior->applyFunc == func_02057f8c) {
                return behavior->object;
            }
            continue;
        case PARTICLE_BEHAVIOR_CONVERGENCE:
            if (behavior->applyFunc == func_02057ef8) {
                return behavior->object;
            }
            continue;
        default:
            return NULL;
        }
    }
    return NULL;
}

void func_020519c4(SPLEmitter *emitter, const VecFx32 *target) {
    SPLMagnetBehavior *behavior = (SPLMagnetBehavior *)func_020518ac(emitter, PARTICLE_BEHAVIOR_MAGNET);

    if (behavior != NULL) {
        behavior->target = *target;
    }
}

void func_020519e4(SPLEmitter *emitter, const u16 *axis) {
    SPLSpinBehavior *behavior = (SPLSpinBehavior *)func_020518ac(emitter, PARTICLE_BEHAVIOR_SPIN);

    if (behavior != NULL) {
        behavior->axis = *axis;
    }
}

void func_02051a04(SPLEmitter *emitter, const VecFx32 *target) {
    SPLConvergenceBehavior *behavior = (SPLConvergenceBehavior *)func_020518ac(emitter, PARTICLE_BEHAVIOR_CONVERGENCE);

    if (behavior != NULL) {
        behavior->target = *target;
    }
}

void func_02051a24(SPLEmitter *emitter, SPLEmitterUpdateCallback callback) {
    emitter->updateCallback = callback;
}

void func_02051a2c(SPLEmitter *emitter, void *data) {
    emitter->userDataPtr = data;
}

void *func_02051a34(SPLEmitter *emitter) {
    return emitter->userDataPtr;
}

// The VRAM allocators of an uploading resource, which keep the keys the linked-list managers give
static u32 func_02051a3c(u32 size, BOOL is4x4comp) {
    return NNS_GfdGetTexKeyAddr(NNS_GfdAllocFrmTexVram(size, is4x4comp, 0));
}

static u32 func_02051a54(u32 size, BOOL is4x4comp) {
    NNSGfdTexKey key = NNS_GfdAllocTexVram(size, is4x4comp, 0);
    ParticleSystem *system = data_02146a88->uploading;

    system->texKeys[system->texKeyCount++] = key;
    return NNS_GfdGetTexKeyAddr(key);
}

static u32 func_02051aa0(u32 size, BOOL is4pltt) {
    return NNS_GfdGetPlttKeyAddr(NNS_GfdAllocFrmPlttVram(size, is4pltt, TRUE));
}

static u32 func_02051ab8(u32 size, BOOL is4pltt) {
    NNSGfdPlttKey key = NNS_GfdAllocPlttVram(size, is4pltt, 0);
    ParticleSystem *system = data_02146a88->uploading;

    system->plttKeys[system->plttKeyCount++] = key;
    return NNS_GfdGetPlttKeyAddr(key);
}

u16 func_02051b04(const void *resource) {
    return ((const SPLFileHeader *)resource)->resourceCount;
}

void *func_02051b0c(u32 layer) {
    switch (layer) {
    case 0:
        return G2_GetBG0CharPtr();
    case 1:
        return G2_GetBG1CharPtr();
    case 2:
        return G2_GetBG2CharPtr();
    case 3:
        return G2_GetBG3CharPtr();
    case 4:
        return G2S_GetBG0CharPtr();
    case 5:
        return G2S_GetBG1CharPtr();
    case 6:
        return G2S_GetBG2CharPtr();
    case 7:
        return G2S_GetBG3CharPtr();
    }
    return NULL;
}
