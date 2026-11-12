// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/billboard_act.c. Black has no
// BlActSys_SetActorLight, and BlActSys_ExecActorRequests declares its locals at the top, without an inline for the
// handle.
#include "types.h"
#include "gfl/areaman.h"
#include "gfl/blact.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"

// The billboard actor system: runs of materials and actors in a scene, and the animations of faces actors play

typedef BOOL (*BlActAnimCmdFunc)(BlActHandle *handle, const BlActAnimStep *step);

static void func_02050088(BlActTexMat *texMat);
static void func_020500a0(BlActHandle *handle);
static void BlActHandle_Reset(BlActHandle *handle);
static void func_02050550(BlActSys *sys, BlActHandle *handle);
static void func_020506b8(BlActSys *sys, BlActTexMat *texMat, u16 face);
static void func_02050738(BlActSys *sys, BlActTexMat *texMat);
static BOOL func_02050920(const BlActAnimStep *step);
static BOOL func_02050944(BlActHandle *handle, const BlActAnimStep *step);
static BlActAnimCmdFunc func_02050984(u32 cmd);
static BOOL func_020509e0(BlActHandle *handle, const BlActAnimStep *step);
static BOOL func_020509e8(BlActHandle *handle, const BlActAnimStep *step);
static BOOL func_02050a00(BlActHandle *handle, const BlActAnimStep *step);
static BOOL func_02050a58(BlActHandle *handle, const BlActAnimStep *step);

const BlActSceneSetup BLACT_DEFAULT_CONFIG = {
    0, 0, { FX32_ONE, FX32_ONE, FX32_ONE }, GX_RGB(31, 31, 31), GX_RGB(16, 16, 16), GX_RGB(16, 16, 16),
    GX_RGB(0, 0, 0), 63, BLACT_ORIGIN_CENTER,
};

BlActSys *BlActSys_Create(u16 texMatCount, u16 handleCount, BlActTransferFunc transfer, HeapID heapId) {
    BlActSys *sys = GFL_HeapAllocate(heapId, sizeof(BlActSys), TRUE, "billboard_act.c", 94);
    BlActSceneSetup setup;
    int i;

    sys->heapId = heapId;
    sys->transfer = transfer;
    sys->texMatCount = texMatCount;
    sys->handleCount = handleCount;
    setup = BLACT_DEFAULT_CONFIG;
    setup.materialCount = sys->texMatCount;
    setup.actorCount = sys->handleCount;
    sys->scene = BlActScene_Create(&setup, heapId);
    sys->texMats = GFL_HeapAllocate(heapId, sys->texMatCount * sizeof(BlActTexMat), TRUE, "billboard_act.c", 119);
    sys->texMatAlloc = GFL_AreaManCreate(sys->texMatCount, heapId);
    sys->handles = GFL_HeapAllocate(heapId, sys->handleCount * sizeof(BlActHandle), TRUE, "billboard_act.c", 123);
    sys->handleAlloc = GFL_AreaManCreate(sys->handleCount, heapId);
    for (i = 0; i < sys->texMatCount; i++) {
        func_02050088(&sys->texMats[i]);
    }
    for (i = 0; i < sys->handleCount; i++) {
        BlActHandle_Reset(&sys->handles[i]);
    }
    return sys;
}

void BlActSys_Free(BlActSys *sys) {
    int i;

    for (i = 0; i < sys->handleCount; i++) {
        if (sys->handles[i].actor != BLACT_NONE) {
            BlActScene_ClearActorMaterial(sys->scene, sys->handles[i].actor);
        }
    }
    GFL_AreaManFree(sys->handleAlloc);
    GFL_HeapFree(sys->handles);
    GFL_AreaManFree(sys->texMatAlloc);
    GFL_HeapFree(sys->texMats);
    func_0204ed24(sys->scene);
    GFL_HeapFree(sys);
}

BlActScene *BlActSys_GetScene(BlActSys *sys) {
    return sys->scene;
}

static void func_02050088(BlActTexMat *texMat) {
    texMat->material = BLACT_NONE;
    texMat->texResource = NULL;
}

static void func_020500a0(BlActHandle *handle) {
    handle->step = 0;
    handle->wait = 0;
    handle->loops = 0;
    handle->cmd = 0;
}

static void BlActHandle_Reset(BlActHandle *handle) {
    handle->actor = BLACT_NONE;
    handle->anims = NULL;
    handle->animCount = 0;
    handle->anim = 0;
    func_020500a0(handle);
    handle->animate = FALSE;
    handle->callback = NULL;
    handle->callbackData = NULL;
}

u16 BlActSys_ExecMatLoadRequests(BlActSys *sys, const BlActMatRequest *requests, u32 count) {
    u32 first = GFL_AreaManAllocDefault(sys->texMatAlloc, count);
    u32 i;

    // BUG: unlike BlActSys_ExecActorRequests, this does not check that the materials fit, and writes before the
    // table when they do not
#ifdef BUGFIX
    if (first == AREAMAN_FAIL) {
        return BLACT_NONE;
    }
#endif
    for (i = 0; i < count; i++) {
        const BlActMatRequest *request = &requests[i];
        u32 material;

        switch (request->mode) {
        case BLACT_LOAD_KEEP:
            material = BlActScene_AddMaterialNewTex(sys->scene, request->texResource, request->format, request->size,
                                                    request->faceWidth, request->faceHeight);
            sys->texMats[first + i].material = material;
            sys->texMats[first + i].texResource = NULL;
            break;
        case BLACT_LOAD_TRIM:
            material = BlActScene_AddMaterialNewTex(sys->scene, request->texResource, request->format, request->size,
                                                    request->faceWidth, request->faceHeight);
            BlActScene_TrimMatTex(sys->scene, material);
            sys->texMats[first + i].material = material;
            sys->texMats[first + i].texResource = NULL;
            break;
        case BLACT_LOAD_TRANSFER:
            sys->texMats[first + i].material = BLACT_NONE;
            sys->texMats[first + i].texResource = request->texResource;
            break;
        }
    }
    return first;
}

u16 func_02050238(BlActSys *sys, u32 material, void *texResource) {
    u32 idx = GFL_AreaManAllocDefault(sys->texMatAlloc, 1);

    sys->texMats[idx].material = material;
    sys->texMats[idx].texResource = texResource;
    return idx;
}

void BlActSys_FreeMaterials(BlActSys *sys, u16 first, u32 count) {
    u32 i;

    for (i = 0; i < count; i++) {
        if (sys->texMats[first + i].material != BLACT_NONE) {
            BlActScene_FreeMaterial(sys->scene, sys->texMats[first + i].material);
        }
        if (sys->texMats[first + i].texResource != NULL) {
            GFL_G3DResFree(sys->texMats[first + i].texResource);
        }
        func_02050088(&sys->texMats[first + i]);
    }
    GFL_AreaManDeAlloc(sys->texMatAlloc, first, count);
}

void func_02050304(BlActSys *sys, u32 texMat) {
    func_02050088(&sys->texMats[texMat]);
    GFL_AreaManDeAlloc(sys->texMatAlloc, texMat, 1);
}

u16 func_02050330(BlActSys *sys, u32 texMat) {
    return sys->texMats[texMat].material;
}

u16 BlActSys_ExecActorRequests(BlActSys *sys, u16 firstTexMat, const BlActActorRequest *requests, u32 count, u32 type) {
    u32 first = GFL_AreaManAllocDefault(sys->handleAlloc, count);
    const BlActActorRequest *request;
    BlActHandle *handle;
    u32 i;
    u32 actor;

    if (first == AREAMAN_FAIL) {
        return BLACT_NONE;
    }
    for (i = 0; i < count; i++) {
        handle = &sys->handles[first + i];
        request = &requests[i];
        BlActHandle_Reset(handle);
        handle->texMat = firstTexMat + request->texMat;
        actor = BlActScene_AddNewActor(sys->scene, sys->texMats[handle->texMat].material, request->scaleX,
                                       request->scaleY, &request->pos, request->alpha, request->lights, type);
        handle->actor = actor;
        handle->callback = request->callback;
        handle->callbackData = request->callbackData;
        BlActScene_SetActorHidden(sys->scene, actor, &request->visible);
        handle->cmd = BLACT_ANIM_CMD_END;
    }
    return first;
}

void BlActSys_DeleteActors(BlActSys *sys, u32 first, u32 count) {
    u32 i;

    for (i = 0; i < count; i++) {
        BlActScene_ClearActorMaterial(sys->scene, sys->handles[first + i].actor);
        BlActHandle_Reset(&sys->handles[first + i]);
    }
    GFL_AreaManDeAlloc(sys->handleAlloc, first, count);
}

void BlActSys_UpdateActors(BlActSys *sys) {
    int i;

    for (i = 0; i < sys->handleCount; i++) {
        BlActHandle *handle = &sys->handles[i];

        if (handle->actor != BLACT_NONE) {
            if (handle->callback != NULL) {
                handle->callback(sys, i, handle->callbackData);
            }
            func_02050550(sys, handle);
        }
    }
}

static void func_02050550(BlActSys *sys, BlActHandle *handle) {
    const BlActAnimStep *step;
    u16 face;
    u16 wait;
    BOOL flipS;
    BOOL flipT;

    if (handle->anims == NULL || handle->animate == FALSE) {
        return;
    }
    if (handle->wait != 0) {
        handle->wait--;
        return;
    }
    step = &handle->anims[handle->anim][handle->step];
    while (func_02050920(step)) {
        if (func_02050944(handle, step)) {
            return;
        }
        step = &handle->anims[handle->anim][handle->step];
    }
    handle->step++;
    face = step->face;
    wait = step->arg;
    if (sys->texMats[handle->texMat].texResource != NULL) {
        func_020506b8(sys, &sys->texMats[handle->texMat], face);
    } else {
        func_0204f66c(sys->scene, handle->actor, &face);
    }
    if (step->flipS) {
        flipS = TRUE;
    } else {
        flipS = FALSE;
    }
    if (step->flipT) {
        flipT = TRUE;
    } else {
        flipT = FALSE;
    }
    func_0204f764(sys->scene, handle->actor, &flipS);
    func_0204f794(sys->scene, handle->actor, &flipT);
    if (wait != 0) {
        wait--;
    }
    handle->wait = wait;
}

// Sends a face's characters to the material's place in VRAM
static void func_020506b8(BlActSys *sys, BlActTexMat *texMat, u16 face) {
    u8 *image = GFL_G3DResGetTexImageData(texMat->texResource);
    u32 texAddr;
    u32 offset;
    u32 size;

    func_0204f2b4(sys->scene, texMat->material, &texAddr);
    func_0204f344(sys->scene, texMat->material, face, &offset, &size, FALSE);
    if (sys->transfer != NULL) {
        sys->transfer(FALSE, texAddr, image + offset, size);
    }
}

static void func_02050738(BlActSys *sys, BlActTexMat *texMat) {
    void *pltt = GFL_G3DResGetTexPaletteData(texMat->texResource);
    u32 plttAddr;

    func_0204f2cc(sys->scene, texMat->material, &plttAddr);
    if (sys->transfer != NULL) {
        sys->transfer(TRUE, plttAddr, pltt, 0x20);
    }
}

void BlActSys_Draw(BlActSys *sys, G3DCamera *camera, G3DLight *lights) {
    BlActScene_Draw(sys->scene, camera, lights);
}

void func_020507a0(BlActSys *sys, u32 actor, BOOL visible) {
    BlActScene_SetActorHidden(sys->scene, sys->handles[actor].actor, &visible);
}

BOOL func_020507d0(BlActSys *sys, u32 actor) {
    return func_0204f704(sys->scene, sys->handles[actor].actor);
}

void func_020507f0(BlActSys *sys, u32 actor, BOOL animate) {
    sys->handles[actor].animate = animate;
}

void func_02050804(BlActSys *sys, u32 actor, const BlActAnimStep *const *anims, u16 animCount) {
    BlActHandle *handle = &sys->handles[actor];

    handle->anims = anims;
    handle->animCount = animCount;
    handle->anim = 0;
    func_020500a0(handle);
}

void func_0205082c(BlActSys *sys, u32 actor, u16 anim) {
    BlActHandle *handle = &sys->handles[actor];

    handle->anim = anim;
    func_020500a0(handle);
}

u16 func_02050848(BlActSys *sys, u32 actor) {
    return sys->handles[actor].anim;
}

void func_0205085c(BlActSys *sys, u32 actor, u16 step) {
    BlActHandle *handle = &sys->handles[actor];

    func_020500a0(handle);
    handle->step = step;
}

u16 func_02050880(BlActSys *sys, u32 actor) {
    return sys->handles[actor].step;
}

void func_02050894(BlActSys *sys, u32 actor, u32 texMat) {
    BlActTexMat *dst = &sys->texMats[sys->handles[actor].texMat];

    dst->texResource = sys->texMats[texMat].texResource;
    func_02050738(sys, dst);
}

u32 func_020508c4(BlActSys *sys, u32 actor) {
    return sys->handles[actor].actor;
}

u16 BlActSys_GetMatByActor(BlActSys *sys, u32 actor) {
    return sys->handles[actor].texMat;
}

BOOL func_020508ec(BlActSys *sys, u32 actor, u16 *cmd) {
    u16 last = sys->handles[actor].cmd;

    *cmd = last;
    if (last == BLACT_ANIM_CMD_GOTO || last == BLACT_ANIM_CMD_LOOP || last == BLACT_ANIM_CMD_CHANGE
        || last == BLACT_ANIM_CMD_END) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_02050920(const BlActAnimStep *step) {
    if (func_02050984(step->face) != NULL) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_02050944(BlActHandle *handle, const BlActAnimStep *step) {
    BlActAnimCmdFunc func = func_02050984(step->face);

    handle->cmd = step->face;
    return func(handle, step);
}

static BlActAnimCmdFunc func_02050984(u32 cmd) {
    switch (cmd) {
    case BLACT_ANIM_CMD_END:
        return func_020509e0;
    case BLACT_ANIM_CMD_CHANGE:
        return func_020509e8;
    case BLACT_ANIM_CMD_LOOP:
        return func_02050a00;
    case BLACT_ANIM_CMD_GOTO:
        return func_02050a58;
    }
    return NULL;
}

static BOOL func_020509e0(BlActHandle *handle, const BlActAnimStep *step) {
    return TRUE;
}

static BOOL func_020509e8(BlActHandle *handle, const BlActAnimStep *step) {
    handle->anim = step->arg;
    func_020500a0(handle);
    return FALSE;
}

static BOOL func_02050a00(BlActHandle *handle, const BlActAnimStep *step) {
    u8 target = step->arg & 0xff;
    u8 times = (step->arg & 0xff00) >> 8;

    if (handle->loops < times) {
        handle->step = target;
        handle->wait = 0;
        handle->loops++;
    } else {
        handle->loops = 0;
        handle->wait = 0;
        handle->step++;
    }
    return FALSE;
}

static BOOL func_02050a58(BlActHandle *handle, const BlActAnimStep *step) {
    handle->step = step->arg;
    handle->wait = 0;
    return FALSE;
}
