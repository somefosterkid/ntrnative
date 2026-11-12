// Black's code at ov021 0x0219d448-0x0219e0a8 (pokebw2's fldmmdl_g3dobj.c, which its asserts name: tools/name_files.py).

#include "types.h"
#include "field/field_actor.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "field/field_g3dobj.h"
#include "field/fldmmdl_g3dobj.h"
#include "field/ov021_0219d524.h"
#include "field/field.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_rail.h"
#include "field/field_effect.h"
#include "field/zone.h"
#include "gfl/random.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_vec.h"
#include "gfl/sound.h"

// A resource group the field's objects share: its index in the group table, and the object code whose model it holds
// (pokebw2's ActorG3DEntry)
typedef struct ActorG3DEntry {
    u16 resGroup;
    u16 objCode;
} ActorG3DEntry;

// The actor system's 3D model table (pokebw2's ActorG3DSystem): one entry for each actor the system holds
typedef struct ActorG3DSystem {
    MMSys *system;
    FieldG3DObjSystem *objSystem;
    u16 count;
    ActorG3DEntry *entries;
} ActorG3DSystem;

// What an animation command with a wait keeps in the actor's work (pokebw2's ActorWaitWork)
typedef struct ActorWaitWork {
    s16 unk0;
    s16 wait;
} ActorWaitWork;

void func_ov021_0219d4ac(ActorG3DSystem *param_1, HeapID param_2);
void FldActSys_BindActorG3DSystem(MMSys *system, void *actorG3DSystem);
void func_ov021_0219d4ec(ActorG3DSystem *param_1);
ArcTool *GetMMSysDataArcHandle(MMSys *system);
u16 FieldActorG3DSystem_RegistObj(ActorG3DSystem *param_1, u16 param_2, u16 param_3);
u16 func_ov021_0219d558(ActorG3DSystem *param_1, u32 param_2);
void func_ov021_0219d604(ActorG3DSystem *param_1, u32 param_2);
u32 func_ov021_0219d85c(FieldActor *param_1);
void func_ov021_0219d8dc(FieldActor *param_1);
void func_ov021_0219dc34(FieldActor *param_1, s16 param_2);
void func_ov021_0219e354(FieldActor *param_1);
void func_ov021_0219dc88(FieldActor *param_1, u32 param_2, s32 param_3, u32 param_4);
void func_ov021_0219de64(FieldActor *param_1, s16 param_2, u16 param_3, u32 param_4);
void func_ov021_0219e00c(FieldActor *param_1, u32 param_2, s32 param_3, u8 param_4, u16 param_5, s16 param_6, u16 param_7, u32 param_8);
void func_ov021_0219e084(FieldActor *param_1, u16 param_2, s32 param_3, u8 param_4, u16 param_5, s16 param_6, u16 param_7);
void func_ov021_0219dbc8(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4);
void func_ov021_0219db44(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4);
void func_ov021_0219db48(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4);
FieldEffects *FldAct_GetFieldEffects(FieldActor *param_1);
void func_ov021_0219da80(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4);
void func_ov021_0219db04(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4);
void func_ov021_0219db5c(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4);
void func_ov021_0219daa4(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4);
void func_ov021_0219dad0(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4);
void func_ov021_0219db0c(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4);
void func_ov021_0219d7ac(FieldActor *param_1);
void func_ov021_0219d7d0(FieldActor *param_1);
u32 func_ov021_0219d780(FieldActor *param_1);
void func_ov021_0219d7f0(FieldActor *param_1);
void func_ov021_0219d824(FieldActor *param_1);
void func_ov021_0219d990(FieldActor *param_1);
void func_ov021_0219d930(FieldActor *param_1);
void func_ov021_0219da20(FieldActor *param_1);
void func_ov021_0219d9d8(FieldActor *param_1);
void func_ov021_0219e334(FieldActor *param_1, u16 *param_2);
void func_ov021_0219e370(FieldActor *param_1, u16 param_2, s32 param_3);
void func_ov021_0219e6c8(FieldActor *param_1, u16 param_2);

void func_ov021_0219d448(MMSys *param_1, FieldG3DObjSystem *param_2) {
    HeapID heapId;
    ActorG3DSystem *actorG3DSystem;

    heapId = FldActSys_GetFieldHeapID(param_1);
    actorG3DSystem = GFL_HeapAllocate(heapId, sizeof(ActorG3DSystem), 1, "fldmmdl_g3dobj.c", 72);
    actorG3DSystem->system = param_1;
    actorG3DSystem->objSystem = param_2;
    actorG3DSystem->count = GetActorLimit(param_1);
    func_ov021_0219d4ac(actorG3DSystem, heapId);
    FldActSys_BindActorG3DSystem(param_1, actorG3DSystem);
}

void func_ov021_0219d488(MMSys *param_1) {
    ActorG3DSystem *ptr;

    ptr = FldActSys_GetFieldBlAct(param_1);
    func_ov021_0219d4ec(ptr);
    GFL_HeapFree(ptr);
}

FieldG3DObjSystem *FldAct_GetG3DObjSystem(FieldActor *param_1) {
    MMSys *system;
    ActorG3DSystem *sys;

    system = GetActorMModelSystem(param_1);
    sys = FldActSys_GetFieldBlAct(system);
    return sys->objSystem;
}

void func_ov021_0219d4ac(ActorG3DSystem *param_1, HeapID param_2) {
    s32 i;

    param_1->entries = GFL_HeapAllocate(param_2, param_1->count * sizeof(ActorG3DEntry), 1, "fldmmdl_g3dobj.c", 127);
    for (i = 0; i < param_1->count; i++) {
        param_1->entries[i].objCode = 0xffff;
    }
}

void func_ov021_0219d4ec(ActorG3DSystem *param_1) {
    s32 i;
    u16 val;
    ActorG3DEntry *entry;

    val = param_1->count;
    i = 0;
    entry = param_1->entries;
    if (((s32) val > 0) && ((s32) val > 0)) {
        do {
            if (entry->objCode != 0xFFFF) {
                FieldG3DObjSystem_FreeResGroup(param_1->objSystem, entry->resGroup);
            }
            i += 1;
            entry++;
        } while (i < (s32) param_1->count);
    }
    GFL_HeapFree(param_1->entries);
}

u16 FieldActorG3DSystem_RegistObj(ActorG3DSystem *param_1, u16 param_2, u16 param_3) {
    s32 i;
    ActorG3DEntry *entry;
    s32 count;

    i = 0;
    entry = param_1->entries;
    count = param_1->count;
    if (count > 0) {
        do {
            if (entry->objCode == 0xffff) {
                entry->objCode = param_2;
                entry->resGroup = param_3;
                return i;
            }
            i++;
            entry++;
        } while (i < count);
    }
    return 0;
}

u16 func_ov021_0219d558(ActorG3DSystem *param_1, u32 param_2) {
    s32 i;
    ActorG3DEntry *entry;
    s32 count;

    i = 0;
    entry = param_1->entries;
    count = param_1->count;
    if (count > 0) {
        do {
            if (entry->objCode == param_2) {
                return entry->resGroup;
            }
            i++;
            entry++;
        } while (i < count);
    }
    return 0xffff;
}


u16 FldActSys_SetupG3DObj(MMSys *param_1, ActorG3DSystem *param_2, u16 param_3, const u16 *param_4) {
    u16 result;
    ArcTool *arc;
    s32 i;
    FieldG3DObjResRequest FStack_38;

    arc = GetMMSysDataArcHandle(param_1);
    FieldG3DObjResRequest_Clear(&FStack_38);
    if (*param_4 != 0xffff) {
        FieldG3DObjResRequest_SetModel(&FStack_38, arc, *param_4);
    }
    if (param_4[1] != 0xffff) {
        FieldG3DObjResRequest_SetModel(&FStack_38, arc, param_4[1]);
    }
    FieldG3DObjResRequest_SetAnmArc(&FStack_38, arc);
    i = 0;
    do {
        if (param_4[i + 2] != 0xffff) {
            FieldG3DObjResRequest_AddAnm(&FStack_38, param_4[i + 2]);
        }
        i = i + 1;
    } while (i < 3);
    result = FieldG3DObjSystem_AddResGroup(param_2->objSystem, &FStack_38, 0);
    FieldActorG3DSystem_RegistObj(param_2, param_3, result);
    return result;
}

void func_ov021_0219d604(ActorG3DSystem *param_1, u32 param_2) {
    s32 i;
    ActorG3DEntry *entry;
    i = 0;
    entry = param_1->entries;
    while (i < (s32)(u32)param_1->count) {
        if (entry->objCode == param_2) {
            FieldG3DObjSystem_FreeResGroup(param_1->objSystem, entry->resGroup);
            entry->objCode = 0xffff;
            return;
        }
        i = i + 1;
        entry = entry + 1;
    }
}

u16 func_ov021_0219d634(FieldActor *param_1) {
    u16 resGroupIdx;
    const FieldActorConfig *config;
    MMSys *system;
    ActorG3DSystem *fieldBlAct;
    const FieldActorResGroup *pFVar3;

    config = GetActorMdlInfo(param_1);
    system = GetActorMModelSystem(param_1);
    fieldBlAct = FldActSys_GetFieldBlAct(system);
    resGroupIdx = func_ov021_0219d558(fieldBlAct, (u32)config->uid);
    if (resGroupIdx == 0xffff) {
        pFVar3 = GetNPCMdlInfoG3DRscGroup(config);
        resGroupIdx = FldActSys_SetupG3DObj(system, fieldBlAct, config->uid, &pFVar3->res1);
    }
    resGroupIdx = FieldG3DObjSystem_AddObj(fieldBlAct->objSystem, resGroupIdx, 0, (VecFx32 *)0x0);
    return resGroupIdx;
}

void func_ov021_0219d680(FieldActor *param_1, u16 param_2) {
    u16 objCode;
    MMSys *system;
    ActorG3DSystem *fieldBlAct;
    BOOL BVar2;

    system = GetActorMModelSystem(param_1);
    fieldBlAct = FldActSys_GetFieldBlAct(system);
    objCode = FldAct_GetObjCode(param_1);
    FieldG3DObjSystem_FreeObj(fieldBlAct->objSystem, param_2);
    BVar2 = FldAct_CheckObjCodeShared(param_1, objCode);
    if (BVar2 == 0) {
        func_ov021_0219d604(fieldBlAct, (u32)objCode);
    }
}

void func_ov021_0219d6b8(FieldActor *actor) {
    func_ov010_0216d6f4(actor);
    SetActorFlag(actor, 2);
}

void func_ov021_0219d6cc(FieldActor *actor) {
    u32 result;
    BOOL BVar2;
    result = func_ov010_0216da20(actor, 2);
    if (result == 0) {
        func_ov021_0219d7ac(actor);
        func_ov021_0219d7d0(actor);
        BVar2 = CheckActorMovementFlag(actor, 0x200);
        if (BVar2 != 0) {
            func_ov021_02197eb4(actor);
        } else {
            BVar2 = func_ov010_0216db10(actor);
            if ((BVar2 == 0) && (result = func_ov021_0219d780(actor), result == 1)) {
                func_ov010_0216d708(actor);
            }
        }
        func_ov021_0219d7f0(actor);
        func_ov021_0219d824(actor);
    }
}

u32 func_ov021_0219d724(FieldActor *param_1, RailPosition *param_2) {
    MMSys *system;
    NoGridMapper *mapper;

    system = GetActorMModelSystem(param_1);
    mapper = FldActSys_GetNoGridMapper(system);
    return FieldNoGridMapper_GetTileAtPos(mapper, param_2);
}

u8 func_ov021_0219d738(FieldActor *param_1, s32 param_2) {
    u16 dir;
    s32 val;
    VecFx16 aVStack_20 [1];
    static const u8 data_ov021_021d80e4[16] = { 1, 3, 3, 3, 3, 3, 3, 0, 0, 2, 2, 2, 2, 2, 2, 1 };

    dir = GetActorFaceDir(param_1);
    func_ov021_0219e784(param_1, (u32)dir, aVStack_20);
    aVStack_20[0].y = 0;
    VEC_Fx16Normalize(aVStack_20, aVStack_20);
    dir = FX_Atan2Idx((s32)aVStack_20[0].x, (s32)aVStack_20[0].z);
    val = (u32)dir - param_2;
    if (val < 0) {
        val = val + 0x10000;
    }
    return data_ov021_021d80e4[val >> 0xc];
}

u32 func_ov021_0219d780(FieldActor *param_1) {
    BOOL BVar1;

    BVar1 = IsActorFlag16(param_1);
    if (BVar1 == 1) {
        return 1;
    }
    BVar1 = CheckActorMovementFlag(param_1, 0x800);
    if (BVar1 == 0) {
        return 1;
    }
    GetActorMoveCode(param_1);
    return 1;
}

void func_ov021_0219d7ac(FieldActor *param_1) {
    BOOL BVar1;
    s32 result;

    BVar1 = CheckActorMovementFlag(param_1, 0x800);
    if ((BVar1 != 0) && (result = func_ov021_0219d85c(param_1), result == 1)) {
        func_ov010_0216da70(param_1);
    }
}

void func_ov021_0219d7d0(FieldActor *param_1) {
    BOOL BVar1;

    BVar1 = CheckActorMovementFlag(param_1, 0x20);
    if (BVar1 != 0) {
        func_ov021_0219d8dc(param_1);
    }
    ClearActorMovementFlag(param_1, 0xa0);
}

void func_ov021_0219d7f0(FieldActor *param_1) {
    BOOL BVar1;
    BVar1 = CheckActorMovementFlag(param_1, 0x80);
    if (BVar1 != 0) {
        func_ov021_0219d990(param_1);
    } else {
        BVar1 = CheckActorMovementFlag(param_1, 0x20);
        if (BVar1 != 0) {
            func_ov021_0219d930(param_1);
        }
    }
    ClearActorMovementFlag(param_1, 0xa0);
}

void func_ov021_0219d824(FieldActor *param_1) {
    BOOL BVar1;
    BVar1 = CheckActorMovementFlag(param_1, 0x100);
    if (BVar1 != 0) {
        func_ov021_0219da20(param_1);
    } else {
        BVar1 = CheckActorMovementFlag(param_1, 0x40);
        if (BVar1 != 0) {
            func_ov021_0219d9d8(param_1);
        }
    }
    ClearActorMovementFlag(param_1, 0x140);
}

u32 func_ov021_0219d85c(FieldActor *param_1) {
    MMSys *system;
    NoGridMapper *mapper;
    RailPosition origPos;
    u32 origY;
    u32 tile;

    origY = 0xffffffff;
    tile = 0xffffffff;
    if (func_ov010_0216dcb4(param_1) == 0) {
        system = GetActorMModelSystem(param_1);
        mapper = FldActSys_GetNoGridMapper(system);
        RailPosition railPos;
        GetActorRailPos(param_1, &railPos);
        func_ov021_0219e334(param_1, &origPos.componentId);
        origY = FieldNoGridMapper_GetTileAtPos(mapper, &origPos);
        tile = FieldNoGridMapper_GetTileAtPos(mapper, &railPos);
    }
    SetCachedOrigYTileUnderActor(param_1, origY);
    SetCachedTileUnderActor(param_1, tile);
    if (tile == 0xffffffff) {
        SetActorMovementFlag(param_1, 0x800);
        return 0;
    }
    ClearActorMovementFlag(param_1, 0x800);
    return 1;
}


void func_ov021_0219d8dc(FieldActor *param_1) {
    BOOL BVar2;
    u32 cachedTileUnderActor;
    u32 cachedOrigYTileUnderActor;
    const FieldActorConfig *pFVar5;

    if ((func_ov021_0219d85c(param_1) != 0) && (BVar2 = func_ov010_0216db24(param_1), BVar2 == 1)) {
        cachedTileUnderActor = GetCachedTileUnderActor(param_1);
        cachedOrigYTileUnderActor = GetCachedOrigYTileUnderActor(param_1);
        pFVar5 = GetActorMdlInfo(param_1);
        func_ov021_0219da80(param_1, cachedTileUnderActor, cachedOrigYTileUnderActor, pFVar5);
        func_ov021_0219db04(param_1, cachedTileUnderActor, cachedOrigYTileUnderActor, pFVar5);
        func_ov021_0219db5c(param_1, cachedTileUnderActor, cachedOrigYTileUnderActor, pFVar5);
    }
}

void func_ov021_0219d930(FieldActor *param_1) {
    BOOL BVar2;
    u32 cachedTileUnderActor;
    u32 cachedOrigYTileUnderActor;
    const FieldActorConfig *pFVar5;

    if ((func_ov021_0219d85c(param_1) != 0) && (BVar2 = func_ov010_0216db24(param_1), BVar2 == 1)) {
        cachedTileUnderActor = GetCachedTileUnderActor(param_1);
        cachedOrigYTileUnderActor = GetCachedOrigYTileUnderActor(param_1);
        pFVar5 = GetActorMdlInfo(param_1);
        func_ov021_0219daa4(param_1, cachedTileUnderActor, cachedOrigYTileUnderActor, pFVar5);
        func_ov021_0219dad0(param_1, cachedTileUnderActor, cachedOrigYTileUnderActor, pFVar5);
        func_ov021_0219db0c(param_1, cachedTileUnderActor, cachedOrigYTileUnderActor, pFVar5);
        func_ov021_0219db5c(param_1, cachedTileUnderActor, cachedOrigYTileUnderActor, pFVar5);
    }
}

void func_ov021_0219d990(FieldActor *param_1) {
    BOOL BVar2;
    u32 cachedTileUnderActor;
    u32 cachedOrigYTileUnderActor;
    const FieldActorConfig *pFVar5;

    if ((func_ov021_0219d85c(param_1) != 0) && (BVar2 = func_ov010_0216db24(param_1), BVar2 == 1)) {
        cachedTileUnderActor = GetCachedTileUnderActor(param_1);
        cachedOrigYTileUnderActor = GetCachedOrigYTileUnderActor(param_1);
        pFVar5 = GetActorMdlInfo(param_1);
        func_ov021_0219db0c(param_1, cachedTileUnderActor, cachedOrigYTileUnderActor, pFVar5);
        func_ov021_0219db5c(param_1, cachedTileUnderActor, cachedOrigYTileUnderActor, pFVar5);
    }
}

void func_ov021_0219d9d8(FieldActor *param_1) {
    u32 tile;
    u32 origY;
    const FieldActorConfig *mdlInfo;

    if (func_ov021_0219d85c(param_1) != 0 && func_ov010_0216db24(param_1) == 1) {
        tile = GetCachedTileUnderActor(param_1);
        origY = GetCachedOrigYTileUnderActor(param_1);
        mdlInfo = GetActorMdlInfo(param_1);
        func_ov021_0219dbc8(param_1, tile, origY, mdlInfo);
        func_ov021_0219db44(param_1, tile, origY, mdlInfo);
    }
}

void func_ov021_0219da20(FieldActor *param_1) {
    u32 tile;
    u32 origY;
    const FieldActorConfig *mdlInfo;

    if (func_ov021_0219d85c(param_1) != 0 && func_ov010_0216db24(param_1) == 1) {
        tile = GetCachedTileUnderActor(param_1);
        origY = GetCachedOrigYTileUnderActor(param_1);
        mdlInfo = GetActorMdlInfo(param_1);
        func_ov021_0219dbc8(param_1, tile, origY, mdlInfo);
        func_ov021_0219db44(param_1, tile, origY, mdlInfo);
        func_ov021_0219daa4(param_1, tile, origY, mdlInfo);
        func_ov021_0219db48(param_1, tile, origY, mdlInfo);
    }
}

void func_ov021_0219da80(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4) {
    FieldEffects *effects;

    if (GetTileFlags(param_2) & 0x20) {
        effects = FldAct_GetFieldEffects(param_1);
        func_ov021_021ac760(effects, param_1, 0, 0);
    }
}

void func_ov021_0219daa4(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4) {
    FieldEffects *effects;

    if (param_2 != 0xffffffff && (GetTileFlags(param_2) & 0x20)) {
        effects = FldAct_GetFieldEffects(param_1);
        func_ov021_021ac760(effects, param_1, 1, 0);
    }
}

void func_ov021_0219dad0(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4) {
    const FieldActorConfig *config;
    FieldEffects *effects;
    BOOL isType2;

    config = param_4;
    if (param_3 != 0xffffffff && (GetTileFlags(param_3) & 8)) {
        effects = FldAct_GetFieldEffects(param_1);
        isType2 = FALSE;
        if (config->footprintType == 2) {
            isType2 = TRUE;
        }
        func_ov021_021bbdd4(param_1, effects, isType2);
    }
}






void func_ov021_0219db04(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4) {
    func_ov021_0219db0c(param_1, param_2, param_3, param_4);
}

void func_ov021_0219db0c(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4) {
    MMSys *system;
    FieldEffects *effects;

    system = GetActorMModelSystem(param_1);
    if (func_ov010_0216d98c(system) != 0) {
        if (CheckActorMovementFlag(param_1, 0x4000) == 0) {
            effects = FldAct_GetFieldEffects(param_1);
            func_ov021_021ac2c0(param_1, effects);
            SetActorMovementFlag(param_1, 0x4000);
        }
    }
}


void func_ov021_0219db44(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4) {
}

void func_ov021_0219db48(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4) {
    FieldEffects *effects;

    effects = FldAct_GetFieldEffects(param_1);
    func_ov021_021ac578(param_1, effects);
}

void func_ov021_0219db5c(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4) {
    const FieldActorConfig *config;
    MMSys *system;
    FieldEffects *effects;
    u32 tile;
    u32 newTile;

    config = param_4;
    if (config->enableReflections != 0 && FldAct_CheckReflectingFlag(param_1) == 0 && param_2 != 0xffffffff) {
        tile = 0xffffffff;
        if (GetTileFlags(param_2) & 0x40) {
            tile = param_2;
        } else {
            newTile = func_ov010_02163f40(param_1, 1);
            if (GetTileFlags(newTile) & 0x40) {
                tile = newTile;
            }
        }
        if (tile != 0xffffffff) {
            effects = FldAct_GetFieldEffects(param_1);
            system = GetActorMModelSystem(param_1);
            FldAct_SetReflectingFlag(param_1, 1);
            func_ov021_021bbfb8(system, param_1, effects, 1);
        }
    }
}



void func_ov021_0219dbc8(FieldActor *param_1, u32 param_2, u32 param_3, const FieldActorConfig *param_4) {
    BOOL BVar1;
    u32 tileType;

    if ((param_4->enableReflections != 0) && (BVar1 = FldAct_CheckReflectingFlag(param_1), BVar1 != 0))
    {
        tileType = func_ov010_02163f40(param_1, 1);
        if (tileType == 0xffffffff) {
            FldAct_SetReflectingFlag(param_1, 0);
            return;
        }
        tileType = GetTileFlags(tileType);
        if ((tileType & 0x40) == 0) {
            FldAct_SetReflectingFlag(param_1, 0);
        }
    }
}

FieldEffects *FldAct_GetFieldEffects(FieldActor *param_1) {
    MMSys *mmSys;
    Field *field;

    mmSys = GetActorMModelSystem(param_1);
    field = GetMMSysField(mmSys);
    return (FieldEffects *)Field_GetFieldEffects(field);
}

u32 func_ov021_0219dc18(FieldActor *param_1) {
    SetActorMovementFlag(param_1, 0x400);
    return 0;
}

u32 func_ov021_0219dc28(FieldActor *param_1) {
    FieldActor_NextAcmdState(param_1);
    return 1;
}

void func_ov021_0219dc34(FieldActor *param_1, s16 param_2) {
    CheckSetActorFaceDir(param_1, param_2);
    func_ov010_0216d674(param_1, 0);
    SetActorInitialGPosToNowGPos(param_1);
    FieldActor_NextAcmdState(param_1);
}


u32 func_ov021_0219dc58(FieldActor *param_1) {
    func_ov021_0219dc34(param_1, 0);
    return 1;
}

u32 func_ov021_0219dc64(FieldActor *param_1) {
    func_ov021_0219dc34(param_1, 1);
    return 1;
}

u32 func_ov021_0219dc70(FieldActor *param_1) {
    func_ov021_0219dc34(param_1, 2);
    return 1;
}

u32 func_ov021_0219dc7c(FieldActor *param_1) {
    func_ov021_0219dc34(param_1, 3);
    return 1;
}

void func_ov021_0219dc88(FieldActor *param_1, u32 param_2, s32 param_3, u32 param_4) {
    func_ov021_0219e370(param_1, (u16)param_2, param_3);
    func_ov021_0219e6c8(param_1, (u16)param_2);
    ChangeActorDirection(param_1, (u16)param_2);
    func_ov010_0216d674(param_1, param_4);
    SetActorMovementFlag(param_1, 0x20);
    FieldActor_NextAcmdState(param_1);
}


u32 func_ov021_0219dcc4(FieldActor *param_1) {
    RailUnit *unit;
    VecFx32 VStack_20;

    unit = FldAct_GetRailUnit(param_1);
    func_ov021_0219e354(param_1);
    RailUnit_GetCalcPos(unit, &VStack_20);
    SetActorWPosValue(param_1, &VStack_20);
    if (func_ov021_021b8040(unit) != 0) {
        return 0;
    }
    SetActorMovementFlag(param_1, 0x440);
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_0219dd18(FieldActor *param_1) {
    func_ov010_0216d674(param_1, 0);
    return 0;
}

u32 func_ov021_0219dd24(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 0, 0x10, 2);
    return 1;
}

u32 func_ov021_0219dd34(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 1, 0x10, 2);
    return 1;
}

u32 func_ov021_0219dd44(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 2, 0x10, 2);
    return 1;
}

u32 func_ov021_0219dd54(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 3, 0x10, 2);
    return 1;
}

u32 func_ov021_0219dd64(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 0, 8, 3);
    return 1;
}

u32 func_ov021_0219dd74(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 1, 8, 3);
    return 1;
}

u32 func_ov021_0219dd84(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 2, 8, 3);
    return 1;
}

u32 func_ov021_0219dd94(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 3, 8, 3);
    return 1;
}

u32 func_ov021_0219dda4(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 0, 4, 4);
    return 1;
}

u32 func_ov021_0219ddb4(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 1, 4, 4);
    return 1;
}

u32 func_ov021_0219ddc4(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 2, 4, 4);
    return 1;
}

u32 func_ov021_0219ddd4(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 3, 4, 4);
    return 1;
}

u32 func_ov021_0219dde4(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 0, 2, 5);
    return 1;
}

u32 func_ov021_0219ddf4(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 1, 2, 5);
    return 1;
}

u32 func_ov021_0219de04(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 2, 2, 5);
    return 1;
}

u32 func_ov021_0219de14(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 3, 2, 5);
    return 1;
}

u32 func_ov021_0219de24(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 0, 4, 10);
    return 1;
}

u32 func_ov021_0219de34(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 1, 4, 10);
    return 1;
}

u32 func_ov021_0219de44(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 2, 4, 10);
    return 1;
}

u32 func_ov021_0219de54(FieldActor *param_1) {
    func_ov021_0219dc88(param_1, 3, 4, 10);
    return 1;
}

void func_ov021_0219de64(FieldActor *param_1, s16 param_2, u16 param_3, u32 param_4) {
    ActorWaitWork *work;

    work = func_ov010_0216d6bc(param_1, sizeof(ActorWaitWork));
    work->unk0 = param_4;
    work->wait = param_3;
    CheckSetActorFaceDir(param_1, param_2);
    func_ov010_0216d674(param_1, param_4);
    SetActorInitialGPosToNowGPos(param_1);
    FieldActor_NextAcmdState(param_1);
}


u32 func_ov021_0219de98(FieldActor *param_1) {
    ActorWaitWork *work;

    work = func_ov010_0216d6d4(param_1);
    work->wait = work->wait + -1;
    if (0 < work->wait) {
        return 0;
    }
    SetActorMovementFlag(param_1, 0x400);
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_0219decc(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 0, 0x20, 1);
    return 1;
}

u32 func_ov021_0219dedc(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 1, 0x20, 1);
    return 1;
}

u32 func_ov021_0219deec(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 2, 0x20, 1);
    return 1;
}

u32 func_ov021_0219defc(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 3, 0x20, 1);
    return 1;
}

u32 func_ov021_0219df0c(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 0, 0x10, 2);
    return 1;
}

u32 func_ov021_0219df1c(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 1, 0x10, 2);
    return 1;
}

u32 func_ov021_0219df2c(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 2, 0x10, 2);
    return 1;
}

u32 func_ov021_0219df3c(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 3, 0x10, 2);
    return 1;
}

u32 func_ov021_0219df4c(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 0, 8, 3);
    return 1;
}

u32 func_ov021_0219df5c(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 1, 8, 3);
    return 1;
}

u32 func_ov021_0219df6c(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 2, 8, 3);
    return 1;
}

u32 func_ov021_0219df7c(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 3, 8, 3);
    return 1;
}

u32 func_ov021_0219df8c(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 0, 2, 5);
    return 1;
}

u32 func_ov021_0219df9c(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 1, 2, 5);
    return 1;
}

u32 func_ov021_0219dfac(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 2, 2, 5);
    return 1;
}

u32 func_ov021_0219dfbc(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 3, 2, 5);
    return 1;
}

u32 func_ov021_0219dfcc(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 0, 4, 4);
    return 1;
}

u32 func_ov021_0219dfdc(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 1, 4, 4);
    return 1;
}

u32 func_ov021_0219dfec(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 2, 4, 4);
    return 1;
}

u32 func_ov021_0219dffc(FieldActor *param_1) {
    func_ov021_0219de64(param_1, 3, 4, 4);
    return 1;
}

void func_ov021_0219e00c(FieldActor *param_1, u32 param_2, s32 param_3, u8 param_4, u16 param_5, s16 param_6, u16 param_7, u32 param_8) {
    NPCRailMove *move;

    move = func_ov010_0216d6bc(param_1, 0x10);
    move->unk_c = param_2;
    move->unk_0 = param_3;
    move->unk_d = param_4;
    move->unk_e = param_5;
    move->unk_f = param_6;
    move->unk_8 = param_7;
    if (param_3 == 0) {
        SetActorInitialGPosToNowGPos(param_1);
    } else {
        func_ov021_0219e370(param_1, param_2, 8);
        func_ov021_0219e6c8(param_1, param_2);
    }
    SetActorMovementFlag(param_1, 0xa0);
    ChangeActorDirection(param_1, param_2);
    func_ov010_0216d674(param_1, param_5);
    FieldActor_NextAcmdState(param_1);
    if (param_8 != 0) {
        GFL_SndSEPlay(param_8);
    }
}


void func_ov021_0219e084(FieldActor *param_1, u16 param_2, s32 param_3, u8 param_4, u16 param_5, s16 param_6, u16 param_7) {
    func_ov021_0219e00c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, 0x55e);
}
