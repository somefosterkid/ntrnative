// Black's code at ov021 0x0219c4ac-0x0219d448. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_actor.h"
#include "field/fldmmdl_blact.h"
#include "field/fldmmdl_drawdata.h"
#include "gfl/blact.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// An object code's cached material: its BlAct material id, and 1 (loaded by this system) | 2 (single use)
typedef struct FldBlActMatEntry {
    u16 objCode;
    u16 matId;
    u16 flags;
    u16 unk_6;
} FldBlActMatEntry;

typedef struct FldBlActMatCache {
    s32 count;
    FldBlActMatEntry *entries;
} FldBlActMatCache;

// A material waiting for a free slot of the async resource reader
typedef struct BlActLoadReq {
    u16 objCode;
    u16 arcIdx;
    u8 billboardSize;
    u8 atlasSize;
    u16 unk_6;
    u32 mode;
} BlActLoadReq;

// A resource being read
typedef struct BlActLoadSlot {
    u8 state;
    u8 unk_1[3];
    u32 mode;
    u16 objCode;
    u8 billboardSize;
    u8 atlasSize;
    u32 unk_c;
    void *res;
} BlActLoadSlot;

// An actor waiting for its material
typedef struct BlActPendActor {
    u16 *blActIdx;
    FieldActor *actor;
    u16 objCode;
    u8 billboardSize;
    u8 atlasSize;
    u8 ctrlType;
    u8 state;
    u16 unk_e;
    void *res;
    FldBlActLoadedFunc callback;
    void *callbackArg;
    VecFx32 pos;
} BlActPendActor;

typedef struct BlActLoader {
    u32 enabled;
    u16 reqCount;
    u16 slotCount;
    u16 actorCount;
    u16 unk_a;
    BlActLoadReq *reqs;
    BlActLoadSlot *slots;
    BlActPendActor *actors;
} BlActLoader;

// An actor whose object code changed: its BlAct is deleted once the new one is up
typedef struct FldBlActDelayed {
    FieldActor *actor;
    u16 blActIdx;
    u16 oldObjCode;
    u16 newObjCode;
    u16 unk_a;
} FldBlActDelayed;

typedef struct FieldBlActWork {
    u8 busy;
    u8 unk_1;
    u16 reqCount;
    BlActSys *sys;
    u16 firstTexMat;
    u16 unk_a;
    u32 unk_c;
    MMSys *mmsys;
    FldBlActMatCache matCache;
    BlActLoader *loader;
    FldBlActDelayed *delayed;
} FieldBlActWork;


void func_ov021_0219ceac(FieldBlActWork *work);
u32 func_ov021_0219ccf0(FieldBlActWork *work);
void func_ov021_0219cb64(FieldBlActWork *work, const FieldActorConfig *param_2, u16 param_3);
u32 FldActSys_AddBlActDefault(MMSys *param_1, VecFx32 *param_2, s32 param_3, s32 param_4, u16 param_5);
void FieldBlAct_UnbindActorObjCode(FieldBlActWork *work, FieldActor *param_2, u16 param_3, u16 param_4);

// The scale of the billboard actors' scene
static const VecFx32 data_ov021_021d8058 = { 0x7000, 0x7000, 0x7000 };

// The texture size bits of each sprite atlas size
static const u16 MMDL_TEXTURE_ATLAS_SIZE_BITS[64] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
};

void func_ov021_0219d2f4(FieldBlActWork *work);
void func_ov021_0219cc80(FieldBlActWork *work);
void func_ov021_0219caf4(FieldBlActWork *work);
void FldActSys_BindFieldBlAct(MMSys *system, void *fieldBlAct);

ArcTool *GetMMSysDataArcHandle(MMSys *system);
u32 FieldBlAct_FindCachedObjMat(FieldBlActWork *work, u32 objCode, u16 *matId, u16 *flags);
void FldBlAct_UpdateAsyncActorSetup(FieldBlActWork *work, BlActLoader *loader);
void FldBlAct_UpdateAsyncRead(FieldBlActWork *work);
void FldBlAct_UpdateAsyncDelete(FieldBlActWork *work);
void func_ov021_0219cb98(FieldBlActWork *work, u32 param_2);
void func_ov021_0219cb24(FieldBlActWork *work, void *param_2, u16 param_3, s32 param_4, u8 param_5, u32 param_6);
u16 func_ov021_0219d3f0(FieldBlActWork *work, u32 param_2, s32 param_3, s32 param_4, void *param_5, u32 param_6);
void FldAct_InitBlActNewMat(FieldBlActWork *work, FieldActor *actor, const FieldActorConfig *config, u16 *blActIdx, FldBlActLoadedFunc callback, void *callbackArg, VecFx32 *pos);
void func_ov021_0219d064(FieldBlActWork *work, BlActPendActor *entry);
void func_ov021_0219d1f8(BlActPendActor *entry);
u32 BlActSys_LoadSingleUseTexMaterial(BlActSys *param_1, void *param_2, s32 param_3, s32 param_4);
void func_ov021_0219cad8(FieldBlActWork *work, s32 param_2);
void func_ov021_0219cbe0(FieldBlActWork *work);
void func_ov021_0219d2cc(FieldBlActWork *work);
void func_ov021_0219d184(FieldBlActWork *work, FieldActor *actor);
u32 func_ov021_0219d3b4(FieldBlActWork *work, FieldActor *actor, u16 newObjCode);
u32 FldBlAct_ExecAsyncRead(FieldBlActWork *work, u16 param_2, u8 param_3, u8 param_4, u16 param_5, u32 param_6);
void func_ov021_0219c9d0(BlActSys *sys, u32 actor, void *data);
const BlActDefaultAnim *func_ov021_0219d3e4(s32 param_1);


u32 BlActSys_LoadSingleUseTexMaterial(BlActSys *param_1, void *param_2, s32 param_3, s32 param_4) {
    u32 result = 0xffffffff;
    const u16 *ptr;
    BlActScene *scene;
    u32 material;

    if (GFL_G3DResUploadTexData(param_2) == 1) {
        scene = BlActSys_GetScene(param_1);
        ptr = MMDL_BILLBOARD_SIZES_INT[param_3];
        material = BlActScene_AddMaterialExistTex(scene, GFL_G3DResGetTexVRAMHandle(param_2), GFL_G3DResGetPltVRAMHandle(param_2), 0, MMDL_TEXTURE_ATLAS_SIZE_BITS[param_4],
            MMDL_BILLBOARD_SIZES_INT[param_3][0], ptr[1]);
        result = func_02050238(param_1, material, NULL);
        GFL_G3DResFree(param_2);
    }
    return result;
}

void func_ov021_0219c52c(BlActSys *param_1, u32 param_2) {
    u16 material;
    BlActScene *scene;

    material = func_02050330(param_1, param_2);
    scene = BlActSys_GetScene(param_1);
    func_02050304(param_1, param_2);
    func_0204f214(scene, (u32)material);
}

void func_ov021_0219c554(MMSys *param_1, BlActSys *param_2, u16 param_3) {
    u8 basePolyId;
    FieldBlActWork *fieldBlAct;
    BlActScene *scene;
    VecFx32 scale;

    fieldBlAct = GFL_HeapAllocate(FldActSys_GetFieldHeapID(param_1), sizeof(FieldBlActWork), TRUE, "fldmmdl_blact.c", 0x168);
    fieldBlAct->mmsys = param_1;
    fieldBlAct->sys = param_2;
    fieldBlAct->reqCount = param_3;
    FldActSys_BindFieldBlAct(param_1, fieldBlAct);
    func_ov021_0219cad8(fieldBlAct, param_3);
    func_ov021_0219cbe0(fieldBlAct);
    func_ov021_0219d2cc(fieldBlAct);
    scene = BlActSys_GetScene(param_2);
    scale = data_ov021_021d8058;
    BlActScene_SetScale(scene, &scale);
    basePolyId = 0;
    BlActScene_SetBasePolyID(scene, &basePolyId);
}


void func_ov021_0219c5d0(MMSys *param_1) {
    FieldBlActWork *ptr;

    ptr = func_ov010_0216d520(param_1);
    func_ov021_0219d2f4(ptr);
    func_ov021_0219cc80(ptr);
    func_ov021_0219caf4(ptr);
    GFL_HeapFree(ptr);
    FldActSys_BindFieldBlAct(param_1, (void *)0x0);
}

void FldActSys_UpdateResourceLoader(MMSys *system) {
    FieldBlActWork *fieldBlAct = func_ov010_0216d520(system);

    if (fieldBlAct != NULL) {
        if (fieldBlAct->loader != NULL) {
            FldBlAct_UpdateAsyncActorSetup(fieldBlAct, fieldBlAct->loader);
            FldBlAct_UpdateAsyncRead(fieldBlAct);
        }
        FldBlAct_UpdateAsyncDelete(fieldBlAct);
        fieldBlAct->busy = 0;
    }
}


void FldActSys_FinishAsyncMatLoad(MMSys *system) {
    func_ov021_0219ceac(func_ov010_0216d520(system));
}

BOOL FldActSys_IsAsyncLoadPending(MMSys *system) {
    if (func_ov021_0219ccf0(func_ov010_0216d520(system)) == 1) {
        return 1;
    }
    return 0;
}

u8 func_ov021_0219c648(FieldActor *param_1) {
    MMSys *system;
    FieldBlActWork *fieldBlActWork;

    system = GetActorMModelSystem(param_1);
    fieldBlActWork = func_ov010_0216d520(system);
    return fieldBlActWork->busy;
}

void func_ov021_0219c658(MMSys *param_1, u16 param_2) {
    FieldBlActWork *work;
    FieldActorConfig FStack_30;

    work = func_ov010_0216d520(param_1);
    GetNPCMdlInfoForOBJCODE(param_1, param_2, &FStack_30);
    func_ov021_0219cb64(work, &FStack_30, 0);
}

void func_ov021_0219c680(MMSys *param_1) {
    u16 mmSysMdlInfoCacheEntryCount;
    FieldBlActWork *work;
    u32 index;
    FieldActorConfig FStack_30;
    work = func_ov010_0216d520(param_1);
    mmSysMdlInfoCacheEntryCount = GetMMSysMdlInfoCacheEntryCount(param_1);
    index = 0;
    while ((s32)index < (s32)(u32)mmSysMdlInfoCacheEntryCount) {
        GetMMSysMdlInfoCacheEntry(param_1, index, &FStack_30);
        func_ov021_0219cb64(work, &FStack_30, 0);
        index = index + 1;
    }
}

u32 FldActSys_AddBlActDefault(MMSys *param_1, VecFx32 *param_2, s32 param_3, s32 param_4, u16 param_5) {
    FieldBlActWork *fieldBlAct;
    BlActActorRequest request;
    u16 actor;
    BlActSys *sys;
    const BlActDefaultAnim *anim;
    const u16 *size;

    fieldBlAct = func_ov010_0216d520(param_1);
    request.texMat = param_5;
    size = data_ov021_021d75d0[param_3];
    request.scaleX = size[0];
    request.scaleY = size[1];
    request.pos = *param_2;
    request.alpha = 0x1f;
    request.visible = 1;
    request.callback = func_ov021_0219c9d0;
    request.lights = 1;
    request.callbackData = fieldBlAct;
    actor = BlActSys_ExecActorRequests(fieldBlAct->sys, fieldBlAct->firstTexMat, &request, 1, 0);
    anim = func_ov021_0219d3e4(param_4);
    if (anim->steps != NULL) {
        func_02050804(fieldBlAct->sys, actor, anim->steps, anim->count);
        sys = fieldBlAct->sys;
        func_0205082c(sys, actor, 0);
        func_020507f0(sys, actor, 1);
    }
    return actor;
}


u32 FldAct_InitBlActWithMat(FieldActor *param_1, u16 param_2) {
    MMSys *pMVar1;
    const FieldActorConfig *pFVar2;
    VecFx32 VStack_24;

    pMVar1 = GetActorMModelSystem(param_1);
    pFVar2 = GetActorMdlInfo(param_1);
    CopyActorPosAllAdd(param_1, &VStack_24);
    return FldActSys_AddBlActDefault(pMVar1, &VStack_24, (u32)pFVar2->billboardSize,
                                            (u32)pFVar2->spriteControllerType, param_2);
}

u32 func_ov021_0219c780(MMSys *param_1, VecFx32 *param_2, s32 param_3, s32 param_4, u16 param_5, u16 param_6) {
    u32 actor;
    FieldBlActWork *fieldBlActWork;

    actor = FldActSys_AddBlActDefault(param_1, param_2, param_3, param_4, param_5);
    if (param_6 != 0xffff) {
        fieldBlActWork = func_ov010_0216d520(param_1);
        func_02050894(fieldBlActWork->sys, actor, (u32)param_6);
    }
    return actor;
}

u32 func_ov021_0219c7b4(FieldActor *param_1, u16 param_2, u16 param_3) {
    MMSys *pMVar1;
    const FieldActorConfig *pFVar2;
    VecFx32 VStack_24;

    pMVar1 = GetActorMModelSystem(param_1);
    pFVar2 = GetActorMdlInfo(param_1);
    CopyActorPosAllAdd(param_1, &VStack_24);
    return func_ov021_0219c780(pMVar1, &VStack_24, (u32)pFVar2->billboardSize,
                                            (u32)pFVar2->spriteControllerType, param_2, param_3);
}

u32 FldAct_InitBlActExistMat(FieldActor *param_1, u32 param_2) {
    u16 matId;
    u16 flags;

    FieldBlAct_FindCachedObjMat(func_ov010_0216d520(GetActorMModelSystem(param_1)), param_2, &matId, &flags);
    return FldAct_InitBlActWithMat(param_1, matId);
}

BOOL func_ov021_0219c814(FieldActor *param_1, u16 *param_2) {
    FieldBlActWork *work;
    const FieldActorConfig *config;
    u16 matId;
    u16 flags;

    work = func_ov010_0216d520(GetActorMModelSystem(param_1));
    config = GetActorMdlInfo(param_1);
    FieldBlAct_FindCachedObjMat(work, config->uid, &matId, &flags);
    if (matId != 0xffff && (flags & 2) == 0) {
        *param_2 = FldAct_InitBlActExistMat(param_1, config->uid);
        return TRUE;
    }
    *param_2 = 0xffff;
    FldAct_InitBlActNewMat(work, param_1, config, param_2, NULL, NULL, NULL);
    return FALSE;
}


void FieldBlAct_UnbindActorObjCode(FieldBlActWork *param_1, FieldActor *param_2, u16 param_3, u16 param_4) {
    u16 second;
    u16 first;

    if (FieldBlAct_FindCachedObjMat(param_1, param_4, &second, &first) == 1) {
        if (first & 2) {
            BlActSys_FreeMaterials(param_1->sys, BlActSys_GetMatByActor(param_1->sys, param_3), 1);
        }
        if ((first & 1) && !FldAct_CheckObjCodeShared(param_2, param_4)) {
            func_ov021_0219cb98(param_1, param_4);
        }
    }
}

void func_ov021_0219c8d4(FieldActor *param_1, u32 param_2) {
    u16 objCode;
    FieldBlActWork *fieldBlAct;

    objCode = FldAct_GetObjCode(param_1);
    fieldBlAct = func_ov010_0216d520(GetActorMModelSystem(param_1));
    if (param_2 == 0xffff) {
        func_ov021_0219d184(fieldBlAct, param_1);
        return;
    }
    if (func_ov021_0219d3b4(fieldBlAct, param_1, param_2) != 1) {
        FieldBlAct_UnbindActorObjCode(fieldBlAct, param_1, param_2, objCode);
        BlActSys_DeleteActors(fieldBlAct->sys, (u16)param_2, 1);
    }
}

u32 func_ov021_0219c928(MMSys *param_1, const FieldActorConfig *param_2, u16 *param_3, VecFx32 *param_4, FldBlActLoadedFunc param_5, void *param_6) {
    FieldBlActWork *fieldBlAct;
    u16 id;

    fieldBlAct = func_ov010_0216d520(param_1);
    FieldBlAct_FindCachedObjMat(fieldBlAct, param_2->uid, &id, param_3);
    if ((param_3[0] & 2) == 0) {
        param_3[1] = FldActSys_AddBlActDefault(param_1, param_4, param_2->billboardSize, param_2->spriteControllerType, id);
        return 1;
    }
    param_3[1] = 0xffff;
    FldAct_InitBlActNewMat(fieldBlAct, NULL, param_2, &param_3[1], param_5, param_6, param_4);
    return 0;
}




void func_ov021_0219c98c(MMSys *param_1, u16 *param_2) {
    u16 first;
    FieldBlActWork *fieldBlActWork;

    fieldBlActWork = func_ov010_0216d520(param_1);
    if (param_2[1] != 0xffff) {
        if ((*param_2 & 2) != 0) {
            first = BlActSys_GetMatByActor(fieldBlActWork->sys, (u32)param_2[1]);
            BlActSys_FreeMaterials(fieldBlActWork->sys, first, 1);
        }
        BlActSys_DeleteActors(fieldBlActWork->sys, (u32)param_2[1], 1);
    }
    *param_2 = 0;
    param_2[1] = 0xffff;
}

void func_ov021_0219c9d0(BlActSys *sys, u32 actor, void *data) {
    FieldBlActWork *work = data;

    work->busy = work->busy | 1;
}

BlActSys *FieldBlAct_GetBlActSys(FieldBlActWork *work) {
    return work->sys;
}

void func_ov021_0219c9e0(FieldActor *param_1, u16 *param_2, u16 *param_3) {
    const FieldActorConfig *mdlInfo = GetActorMdlInfo(param_1);
    const u16 (*entry)[2] = &data_ov021_021d75d0[mdlInfo->billboardSize];

    *param_2 = (*entry)[0];
    *param_3 = (*entry)[1];
}


void FldBlAct_CreateMatCache(FldBlActMatCache *cache, s32 count, HeapID heapId) {
    FldBlActMatEntry *entry;

    entry = GFL_HeapAllocate(heapId, count << 3, 1, "fldmmdl_blact.c", 0x484);
    cache->count = count;
    cache->entries = entry;
    do {
        entry->objCode = 0xffff;
        entry->matId = 0xffff;
        entry++;
        count--;
    } while (count != 0);
}

void func_ov021_0219ca34(FldBlActMatCache *cache) {
    GFL_HeapFree(cache->entries);
}

void FldBlAct_RegistCachedMat(FldBlActMatCache *cache, u16 objCode, u16 matId, u16 flags) {
    s32 i;
    FldBlActMatEntry *entry;

    entry = cache->entries;
    i = 0;
    do {
        if (entry->objCode == 0xffff) {
            entry->objCode = objCode;
            entry->matId = matId;
            entry->flags = flags;
            return;
        }
        entry++;
        i++;
    } while (i < cache->count);
}


void func_ov021_0219ca6c(FldBlActMatCache *cache, u32 objCode) {
    s32 i;
    FldBlActMatEntry *entry;

    entry = cache->entries;
    i = 0;
    do {
        if (entry->objCode == objCode) {
            entry->objCode = 0xffff;
            entry->matId = 0xffff;
            entry->flags = 0;
            return;
        }
        i++;
        entry++;
    } while (i < cache->count);
}

u32 func_ov021_0219ca98(FldBlActMatCache *cache, u32 objCode, u16 *matId, u16 *flags) {
    s32 i;
    FldBlActMatEntry *entry;

    i = 0;
    entry = cache->entries;
    if (matId != NULL) {
        *matId = 0xffff;
    }
    do {
        if (entry->objCode == objCode) {
            if (matId != NULL) {
                *matId = entry->matId;
            }
            if (flags != NULL) {
                *flags = entry->flags;
            }
            return 1;
        }
        i++;
        entry++;
    } while (i < cache->count);
    return 0;
}

void func_ov021_0219cad8(FieldBlActWork *work, s32 count) {
    HeapID heapId;

    heapId = FldActSys_GetFieldHeapID(work->mmsys);
    FldBlAct_CreateMatCache(&work->matCache, count, heapId);
}

void func_ov021_0219caf4(FieldBlActWork *work) {
    FldBlActMatEntry *entry;
    s32 i;

    entry = work->matCache.entries;
    i = 0;
    do {
        if (entry->objCode != 0xffff) {
            func_ov021_0219cb98(work, entry->objCode);
        }
        entry++;
        i++;
    } while (i < work->matCache.count);
    func_ov021_0219ca34(&work->matCache);
}

void func_ov021_0219cb24(FieldBlActWork *work, void *param_2, u16 param_3, s32 param_4, u8 param_5, u32 param_6) {
    u32 flags;

    flags = 0xffffffff;
    if (param_6 & 2) {
        flags = 2;
    }
    FldBlAct_RegistCachedMat(&work->matCache, param_3, func_ov021_0219d3f0(work, param_3, param_4, param_5, param_2, flags), param_6);
}


void func_ov021_0219cb64(FieldBlActWork *work, const FieldActorConfig *param_2, u16 param_3) {
    const FieldActorResGroup *rscGroup = GetNPCMdlInfoG2DRscGroup(param_2);
    void *res = GFL_G3DSysReadArcToolResource(GetMMSysDataArcHandle(work->mmsys), rscGroup->res1);

    func_ov021_0219cb24(work, res, param_2->uid, param_2->billboardSize, param_2->spriteAtlasSize, param_3);
}

void func_ov021_0219cb98(FieldBlActWork *work, u32 objCode) {
    u16 flags;
    u16 id;

    func_ov021_0219ca98(&work->matCache, objCode, &id, &flags);
    if (flags & 2) {
        BlActSys_FreeMaterials(work->sys, id, 1);
    } else {
        func_ov021_0219c52c(work->sys, id);
    }
    func_ov021_0219ca6c(&work->matCache, objCode);
}




u32 FieldBlAct_FindCachedObjMat(FieldBlActWork *work, u32 objCode, u16 *matId, u16 *flags) {
    return func_ov021_0219ca98(&work->matCache, objCode, matId, flags);
}

void func_ov021_0219cbe0(FieldBlActWork *work) {
    HeapID heapId;
    BlActLoader *loader;
    BlActLoadReq *req;
    s32 i;

    heapId = FldActSys_GetFieldHeapID(work->mmsys);
    loader = GFL_HeapAllocate(heapId, sizeof(BlActLoader), TRUE, "fldmmdl_blact.c", 0x5d3);
    loader->reqCount = work->reqCount;
    loader->actorCount = GetActorLimit(work->mmsys);
    loader->slotCount = 1;
    req = GFL_HeapAllocate(heapId, loader->reqCount * sizeof(BlActLoadReq), TRUE, "fldmmdl_blact.c", 0x5da);
    loader->reqs = req;
    for (i = 0; i < loader->reqCount; i++, req++) {
        req->objCode = 0xffff;
    }
    loader->slots = GFL_HeapAllocate(heapId, loader->slotCount * sizeof(BlActLoadSlot), TRUE, "fldmmdl_blact.c", 0x5e7);
    loader->actors = GFL_HeapAllocate(heapId, loader->actorCount * sizeof(BlActPendActor), TRUE, "fldmmdl_blact.c", 0x5e9);
    loader->enabled = 1;
    work->loader = loader;
}

void func_ov021_0219cc80(FieldBlActWork *work) {
    BlActLoader *loader;
    BlActLoadSlot *slot;
    BlActPendActor *actor;
    u32 i;

    loader = work->loader;
    if (loader != NULL) {
        loader->enabled = 0;
        GFL_HeapFree(loader->reqs);
        slot = loader->slots;
        for (i = 0; i < loader->slotCount; i++, slot++) {
            if (slot->res != NULL) {
                GFL_G3DResFree(slot->res);
            }
        }
        GFL_HeapFree(loader->slots);
        actor = loader->actors;
        for (i = 0; i < loader->actorCount; i++, actor++) {
            if (actor->blActIdx != NULL && actor->res != NULL) {
                GFL_G3DResFree(actor->res);
            }
        }
        GFL_HeapFree(loader->actors);
        GFL_HeapFree(loader);
        work->loader = NULL;
    }
}

u32 func_ov021_0219ccf0(FieldBlActWork *work) {
    const BlActLoader *loader = work->loader;
    u32 i;
    BlActLoadReq *req = loader->reqs;
    BlActLoadSlot *slot;
    BlActPendActor *actor;

    for (i = 0; i < loader->reqCount; i++, req++) {
        if (req->objCode != 0xffff) {
            return 1;
        }
    }
    slot = loader->slots;
    for (i = 0; i < loader->slotCount; i++, slot++) {
        if (slot->res != NULL) {
            return 1;
        }
    }
    actor = loader->actors;
    for (i = 0; i < loader->actorCount; i++, actor++) {
        if (actor->blActIdx != NULL && actor->res != NULL) {
            return 1;
        }
    }
    return 0;
}

void FldBlAct_ReqAsyncLoad(FieldBlActWork *work, u16 objCode, u8 billboardSize, u8 atlasSize, u16 arcIdx, u32 mode) {
    u32 i;
    BlActLoadReq *req;

    for (i = 0, req = work->loader->reqs; i < work->loader->reqCount; i++, req++) {
        if (req->objCode == 0xffff) {
            req->objCode = objCode;
            req->billboardSize = billboardSize;
            req->atlasSize = atlasSize;
            req->arcIdx = arcIdx;
            req->mode = mode;
            return;
        }
    }
}

void FldBlAct_UpdateAsyncRead(FieldBlActWork *work) {
    BlActLoader *loader;
    u32 added;
    BlActLoadReq *req;
    u32 i;
    BlActLoadReq *base;

    loader = work->loader;
    req = loader->reqs;
    for (i = 0, added = 0; i < loader->reqCount; i++, req++) {
        if (req->objCode != 0xffff) {
            if (FldBlAct_ExecAsyncRead(work, req->objCode, req->billboardSize, req->atlasSize, req->arcIdx, req->mode) == 0) {
                break;
            }
            added++;
            req->objCode = 0xffff;
            if (added >= loader->slotCount) {
                break;
            }
        }
    }
    if (added != 0) {
        base = loader->reqs;
        for (i = 0; i < loader->reqCount - 1; i++) {
            if (base[i].objCode == 0xffff) {
                for (added = i + 1; added < loader->reqCount; added++) {
                    if (base[added].objCode != 0xffff) {
                        base[i] = base[added];
                        base[added].objCode = 0xffff;
                        break;
                    }
                }
            }
        }
    }
}

u32 FldBlAct_ExecAsyncRead(FieldBlActWork *work, u16 objCode, u8 billboardSize, u8 atlasSize, u16 arcIdx, u32 mode) {
    ArcTool *handle;
    void *res;
    u32 i;
    u32 count;
    BlActLoadSlot *slot;

    count = work->loader->slotCount;
    i = 0;
    slot = work->loader->slots;
    while (i < count) {
        if (slot->res == NULL) {
            slot->state = 0;
            slot->objCode = objCode;
            slot->billboardSize = billboardSize;
            slot->atlasSize = atlasSize;
            slot->mode = mode;
            handle = GetMMSysDataArcHandle(work->mmsys);
            res = GFL_G3DSysReadArcToolResource(handle, arcIdx);
            slot->res = res;
            slot->state = 1;
            return 1;
        }
        i++;
        slot++;
    }
    return 0;
}

void func_ov021_0219ceac(FieldBlActWork *work) {
    BlActLoader *loader;
    u32 i;
    BlActLoadSlot *slot;

    loader = work->loader;
    if (loader != NULL && loader->enabled == 1) {
        slot = loader->slots;
        for (i = 0; i < loader->slotCount; i++, slot++) {
            if (slot->res != NULL && slot->state == 1) {
                slot->state = 0;
                func_ov021_0219cb24(work, slot->res, slot->objCode, slot->billboardSize, slot->atlasSize, slot->mode);
                slot->res = NULL;
            }
        }
    }
}

u32 FldBlAct_CheckAsyncLoading(FieldBlActWork *work, u32 objCode, u16 *mode) {
    u32 i;
    BlActLoadReq *req;
    u32 j;
    BlActLoadSlot *slot;

    for (i = 0, req = work->loader->reqs; i < work->loader->reqCount; i++, req++) {
        if (req->objCode == objCode) {
            *mode = req->mode;
            return 1;
        }
    }
    for (j = 0, slot = work->loader->slots; j < work->loader->slotCount; j++, slot++) {
        if (slot->objCode == objCode && slot->res != NULL) {
            *mode = slot->mode;
            return 1;
        }
    }
    return 0;
}

u32 func_ov021_0219cf54(FieldBlActWork *work, u32 objCode) {
    u32 i;
    BlActLoadReq *req;
    u32 j;
    BlActLoadSlot *slot;

    for (i = 0, req = work->loader->reqs; i < work->loader->reqCount; i++, req++) {
        if (req->objCode == objCode) {
            req->objCode = 0xffff;
            return 1;
        }
    }
    for (j = 0, slot = work->loader->slots; j < work->loader->slotCount; j++, slot++) {
        if (slot->res != NULL && slot->objCode == objCode) {
            slot->state = 0;
            GFL_G3DResFree(slot->res);
            slot->res = NULL;
            return 1;
        }
    }
    return 0;
}

void FldAct_InitBlActNewMat(FieldBlActWork *work, FieldActor *actor, const FieldActorConfig *config, u16 *blActIdx, FldBlActLoadedFunc callback, void *callbackArg, VecFx32 *pos) {
    u16 flagsB;
    u16 flagsA;
    u32 i;
    u16 limit;
    const FieldActorResGroup *rscGroup;
    BlActPendActor *entry;

    i = 0;
    limit = GetActorLimit(work->mmsys);
    entry = work->loader->actors;
    for (; i < limit; i++, entry++) {
        if (entry->blActIdx == NULL) {
            entry->state = 0;
            entry->actor = actor;
            entry->blActIdx = blActIdx;
            entry->objCode = config->uid;
            entry->billboardSize = config->billboardSize;
            entry->atlasSize = config->spriteAtlasSize;
            entry->ctrlType = config->spriteControllerType;
            FieldBlAct_FindCachedObjMat(work, entry->objCode, &flagsB, &flagsA);
            if (flagsB == 0xffff && FldBlAct_CheckAsyncLoading(work, entry->objCode, &flagsA) == 0) {
                rscGroup = GetNPCMdlInfoG2DRscGroup(config);
                flagsA = 1;
                FldBlAct_ReqAsyncLoad(work, entry->objCode, entry->billboardSize, entry->atlasSize, rscGroup->res1, 1);
            }
            entry->callback = callback;
            entry->callbackArg = callbackArg;
            if (pos != NULL) {
                entry->pos = *pos;
            }
            entry->state = 1;
            return;
        }
    }
}

void func_ov021_0219d064(FieldBlActWork *work, BlActPendActor *entry) {
    u16 mat;
    u16 second;
    u16 flags;

    flags = 0xffff;
    entry->state = 0;
    if (entry->res != NULL) {
        mat = func_ov021_0219d3f0(work, entry->objCode, entry->billboardSize, entry->atlasSize, entry->res, 1);
        entry->res = NULL;
        FieldBlAct_FindCachedObjMat(work, entry->objCode, &flags, &second);
    } else {
        FieldBlAct_FindCachedObjMat(work, entry->objCode, &mat, &second);
    }
    if (entry->actor != NULL) {
        *entry->blActIdx = func_ov021_0219c7b4(entry->actor, mat, flags);
    } else {
        *entry->blActIdx = func_ov021_0219c780(work->mmsys, &entry->pos, entry->billboardSize, entry->ctrlType, mat, flags);
    }
    if (entry->callback != NULL) {
        entry->callback(*entry->blActIdx, entry->callbackArg);
    }
    if (entry->actor != NULL) {
        FldAct_InvokeUpdateCallback(entry->actor);
    }
}

void FldBlAct_UpdateAsyncActorSetup(FieldBlActWork *work, BlActLoader *loader) {
    u16 limit;
    u32 i;
    BlActPendActor *entry;
    u16 first;
    u16 second;

    i = 0;
    limit = GetActorLimit(work->mmsys);
    entry = work->loader->actors;
    for (; i < limit; i++, entry++) {
        if (entry->blActIdx != NULL && entry->state == 1 &&
            FieldBlAct_FindCachedObjMat(work, entry->objCode, &second, &first) == 1) {
            func_ov021_0219d064(work, entry);
            func_ov021_0219d1f8(entry);
        }
    }
}

u32 func_ov021_0219d144(FieldBlActWork *work, u32 objCode, FieldActor *actor) {
    u16 limit;
    BlActPendActor *entry;
    u32 i;

    i = 0;
    limit = GetActorLimit(work->mmsys);
    entry = work->loader->actors;
    while (i < limit) {
        if (entry->blActIdx != NULL && entry->actor != NULL && entry->actor != actor && entry->objCode == objCode) {
            return 1;
        }
        i++;
        entry++;
    }
    return 0;
}

void func_ov021_0219d184(FieldBlActWork *work, FieldActor *actor) {
    u16 limit;
    u32 i;
    BlActPendActor *entry;
    u16 flags;

    i = 0;
    limit = GetActorLimit(work->mmsys);
    entry = work->loader->actors;
    for (; i < limit; i++, entry++) {
        if (entry->blActIdx != NULL && entry->actor == actor) {
            entry->state = 0;
            if (FldBlAct_CheckAsyncLoading(work, entry->objCode, &flags) == 1 && (flags & 1) &&
                func_ov021_0219d144(work, entry->objCode, entry->actor) == 0) {
                func_ov021_0219cf54(work, entry->objCode);
            }
            if (entry->res != NULL) {
                GFL_G3DResFree(entry->res);
                entry->res = NULL;
            }
            func_ov021_0219d1f8(entry);
            return;
        }
    }
}

void func_ov021_0219d1f8(BlActPendActor *entry) {
    entry->state = 0;
    MI_CpuFill8(entry, 0, sizeof(BlActPendActor));
}

void func_ov021_0219d208(FieldActor *param_1, u16 objCode) {
    MMSys *system;
    FieldBlActWork *work;
    s32 i;
    FldBlActDelayed *entry;
    u16 second;
    u16 blActIdx;
    u16 first;

    system = GetActorMModelSystem(param_1);
    work = func_ov010_0216d520(system);
    entry = work->delayed;
    FieldBlAct_FindCachedObjMat(work, objCode, &second, &first);
    if (second != 0xffff && (first & 2) == 0) {
        FldAct_UpdateBlInfoForNewObjCode(param_1, objCode);
        return;
    }
    i = 0;
    blActIdx = FldAct_GetBlActIdx(param_1, 0);
    if (blActIdx == 0xffff) {
        FldAct_UpdateBlInfoForNewObjCode(param_1, objCode);
        return;
    }
    do {
        if (entry->actor == NULL) {
            break;
        }
        i++;
    } while (i < 4);
    if (i >= 4) {
        FldAct_UpdateBlInfoForNewObjCode(param_1, objCode);
        return;
    }
    entry->actor = param_1;
    entry->blActIdx = blActIdx;
    entry->oldObjCode = FldAct_GetObjCode(param_1);
    entry->newObjCode = objCode;
    FldAct_UpdateBlInfoForNewObjCode(param_1, objCode);
    if (FldAct_GetBlActIdx(param_1, 0) != 0xffff) {
        FieldBlAct_UnbindActorObjCode(work, param_1, entry->blActIdx, entry->oldObjCode);
        BlActSys_DeleteActors(work->sys, entry->blActIdx, 1);
        entry->actor = NULL;
    }
}

void func_ov021_0219d2cc(FieldBlActWork *work) {
    HeapID heapId;

    heapId = FldActSys_GetFieldHeapID(work->mmsys);
    work->delayed = GFL_HeapAllocate(heapId, 4 * sizeof(FldBlActDelayed), 1, "fldmmdl_blact.c", 0x979);
}

void func_ov021_0219d2f4(FieldBlActWork *work) {
    FldBlActDelayed *entry;
    s32 i;

    entry = work->delayed;
    if (entry != NULL) {
        i = 0;
        do {
            if (entry->actor != NULL) {
                BlActSys_DeleteActors(work->sys, entry->blActIdx, 1);
                entry->actor = NULL;
            }
            i++;
            entry++;
        } while (i < 4);
        GFL_HeapFree(work->delayed);
    }
}

void func_ov021_0219d324(FieldBlActWork *work, FldBlActDelayed *entry) {
    u16 oldObjCode;
    u16 curObjCode;
    BOOL BVar3;
    u32 idx;
    FieldActor *actor;

    actor = entry->actor;
    BVar3 = func_ov010_0216da30(actor);
    if (BVar3 == 0) {
        FieldBlAct_UnbindActorObjCode(work, actor, entry->blActIdx, entry->oldObjCode);
        BlActSys_DeleteActors(work->sys, entry->blActIdx, 1);
        entry->actor = NULL;
        return;
    }
    idx = FldAct_GetBlActIdx(actor, 0);
    if ((idx & 0xffff) != 0xffff) {
        oldObjCode = entry->oldObjCode;
        curObjCode = FldAct_GetObjCode(actor);
        if ((u32)oldObjCode != (u32)curObjCode) {
            FieldBlAct_UnbindActorObjCode(work, actor, entry->blActIdx, oldObjCode);
        }
        BlActSys_DeleteActors(work->sys, entry->blActIdx, 1);
        entry->actor = NULL;
    }
}

void FldBlAct_UpdateAsyncDelete(FieldBlActWork *work) {
    FldBlActDelayed *entry;
    s32 i;

    entry = work->delayed;
    if (entry != NULL) {
        i = 0;
        do {
            if (entry->actor != NULL) {
                func_ov021_0219d324(work, entry);
            }
            i++;
            entry++;
        } while (i < 4);
    }
}

u32 func_ov021_0219d3b4(FieldBlActWork *work, FieldActor *actor, u16 newObjCode) {
    FldBlActDelayed *entry;
    s32 i;

    entry = work->delayed;
    if (entry != NULL) {
        i = 0;
        do {
            if (entry->actor != NULL && entry->actor == actor && entry->blActIdx == newObjCode) {
                return 1;
            }
            i++;
            entry++;
        } while (i < 4);
    }
    return 0;
}

const BlActDefaultAnim *func_ov021_0219d3e4(s32 param_1) {
    return &data_ov021_021d7f68[param_1];
}

u16 func_ov021_0219d3f0(FieldBlActWork *work, u32 param_2, s32 param_3, s32 param_4, void *param_5, u32 param_6) {
    BlActMatRequest request;
    const u16 *ptr;

    if (param_6 == 0xffffffff) {
        return BlActSys_LoadSingleUseTexMaterial(work->sys, param_5, param_3, param_4);
    }
    request.texResource = param_5;
    request.format = 0;
    ptr = MMDL_BILLBOARD_SIZES_INT[param_3];
    request.size = (u8)MMDL_TEXTURE_ATLAS_SIZE_BITS[param_4];
    request.faceWidth = (u8)MMDL_BILLBOARD_SIZES_INT[param_3][0];
    request.faceHeight = (u8)ptr[1];
    request.mode = param_6;
    return BlActSys_ExecMatLoadRequests(work->sys, &request, 1);
}
