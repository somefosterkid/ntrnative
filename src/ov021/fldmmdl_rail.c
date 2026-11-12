// Black's code at ov021 0x0219e2c0-0x0219ef14 (pokebw2's fldmmdl_rail.c): the actors on the rails, their position, collision
// and move codes.

#include "field/ov021_0219d524.h"
#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_actor_internal.h"
#include "field/field_g3dobj.h"
#include "field/field_map.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_rail.h"
#include "field/field_effect.h"
#include "field/zone.h"
#include "gfl/random.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "struct_decls.h"
#include "gfl/sound.h"

void func_ov021_0219e354(FieldActor *param_1);
void SetActorPositionRailCoordinates(ActorPositionRail *rail, const RailPosition *pos);
u8 func_ov021_0219e424(FieldActor *param_1, u32 param_2, u32 *param_3);
void *func_ov021_0219e968(FieldActor *param_1, u32 param_2);
void func_ov021_0219ea94(ActorPositionRail *rail, FieldActor *actor);
void func_ov021_0219eab8(ActorPositionRail *rail, FieldActor *actor);
void func_ov021_0219eb18(FieldActor *param_1, u16 param_2);
void func_ov021_0219eba0(FieldActor *param_1, s8 param_2);
const s32 *func_ov021_0219ea40(s32 key);

// A set of directions an actor chooses among, by the number the actor's data gives
typedef struct {
    s32 id;
    const s32 *list;
} DirListEntry;

// How long an actor waits, ended by -1
const s32 data_ov021_021d84ac[5] = { 0x10, 0x20, 0x30, 0x40, -1 };

// The directions an actor chooses among, ended by 9, in data_ov021_021d84e8's order
const s32 data_ov021_021d84c0[5] = { 0, 1, 2, 3, 9 };
const s32 data_ov021_021d8424[3] = { 0, 2, 9 };
const s32 data_ov021_021d8430[3] = { 0, 3, 9 };
const s32 data_ov021_021d8448[3] = { 1, 2, 9 };
const s32 data_ov021_021d8454[3] = { 1, 3, 9 };
const s32 data_ov021_021d848c[4] = { 0, 1, 2, 9 };
const s32 data_ov021_021d849c[4] = { 0, 1, 3, 9 };
const s32 data_ov021_021d846c[4] = { 0, 2, 3, 9 };
const s32 data_ov021_021d847c[4] = { 1, 2, 3, 9 };
const s32 data_ov021_021d843c[3] = { 0, 1, 9 };
const s32 data_ov021_021d8460[3] = { 2, 3, 9 };

// The sets of directions, by the number in the actor's data, ended by 11
const DirListEntry data_ov021_021d84e8[12] = {
    { 0, data_ov021_021d84c0 },
    { 1, data_ov021_021d8424 },
    { 2, data_ov021_021d8430 },
    { 3, data_ov021_021d8448 },
    { 4, data_ov021_021d8454 },
    { 5, data_ov021_021d848c },
    { 6, data_ov021_021d849c },
    { 7, data_ov021_021d846c },
    { 8, data_ov021_021d847c },
    { 9, data_ov021_021d843c },
    { 10, data_ov021_021d8460 },
    { 11, NULL },
};

// The direction an actor ends up facing in, by the side of the rail's turn and the direction it was in
const u8 data_ov021_021d84d4[20] = { 0, 0, 0, 0, 0, 1, 2, 3, 2, 3, 1, 0, 1, 0, 3, 2, 3, 2, 0, 1 };

// The angle of each direction
const u16 data_ov021_021d841c[4] = { 0x0000, 0x8000, 0x4000, 0xc000 };

void func_ov021_0219eaec(ActorPositionRail *rail, FieldActor *actor);
void func_ov021_0219eb4c(FieldActor *param_1);
void func_ov021_0219ed8c(FieldActor *param_1);
void func_ov021_0219edc4(FieldActor *param_1);
void func_ov021_0219ef04(FieldActor *param_1);
void func_ov021_0219eb90(FieldActor *param_1);
void func_ov021_0219ebf4(FieldActor *param_1);
void func_ov021_0219ed7c(FieldActor *param_1);
void func_ov021_0219e334(FieldActor *param_1, u16 *param_2);
void func_ov021_0219e370(FieldActor *param_1, u16 param_2, s32 param_3);
void func_ov021_0219e6c8(FieldActor *param_1, u16 param_2);
void func_ov021_0219ea78(ActorPositionRail *param_1, FieldActor *param_2);
u32 func_ov021_0219e938(u32 param_1);
u32 func_ov021_0219e940(s32 param_1);
void func_ov021_0219ea5c(ActorPositionRail *rail, FieldActor *actor);
u32 func_ov021_021b80d8(RailUnit *);
u32 func_ov021_021b7de0(RailUnit *unit, const RailPosition *pos);
BOOL func_ov021_0219e498(FieldActor *param_1, RailPosition *param_2);
void func_ov021_021b7ed4(RailUnit *unit, VecFx32 *dest);
u32 func_ov021_02197edc(FieldActor *param_1);

void SetActorInitedPositionRail(FieldActor *actor, const RailPosition *position) {
    ActorPositionRail *pAVar1;
    VecFx32 VStack_20;

    pAVar1 = GetNPCRailPosPtrAddr(actor);
    RailUnit_SetRailPos(pAVar1->unit, position);
    SetActorPositionRailCoordinates(pAVar1, position);
    RailUnit_GetCalcPos(pAVar1->unit, &VStack_20);
    SetActorWPosValue(actor, &VStack_20);
}

void GetActorRailPos(FieldActor *actor, RailPosition *position) {
    MMSys *system;
    NoGridMapper *mapper;
    ActorPositionRail *pAVar3;

    system = GetActorMModelSystem(actor);
    mapper = FldActSys_GetNoGridMapper(system);
    FieldNoGridMapper_GetRailSystem(mapper);
    pAVar3 = GetNPCRailPosPtrAddr(actor);
    if (func_ov021_021b8040(pAVar3->unit) != 0) {
        func_ov021_021b7e60(pAVar3->unit, func_ov021_021b8038(pAVar3->unit), position, NULL);
        return;
    }
    func_ov021_021b7dc0(pAVar3->unit, position);
}

void func_ov021_0219e334(FieldActor *param_1, u16 *param_2) {
    ActorPositionRail *pAVar1;

    pAVar1 = GetNPCRailPosPtrAddr(param_1);
    func_ov021_021b7dc0(pAVar1->unit, (RailPosition *)param_2);
}

RailUnit *FldAct_GetRailUnit(FieldActor *actor) {
    ActorPositionRail *pAVar1;

    pAVar1 = GetNPCRailPosPtrAddr(actor);
    return pAVar1->unit;
}

void func_ov021_0219e354(FieldActor *param_1) {
    ActorPositionRail *railPos;

    railPos = GetNPCRailPosPtrAddr(param_1);
    func_ov021_021b810c(railPos->unit);
    func_ov021_0219ea78(railPos, param_1);
}

void func_ov021_0219e370(FieldActor *param_1, u16 param_2, s32 param_3) {
    ActorPositionRail *railPos;
    u32 dir;
    u32 val;

    railPos = GetNPCRailPosPtrAddr(param_1);
    dir = func_ov021_0219e938(param_2);
    val = func_ov021_0219e940(param_3);
    func_ov021_021b80e0(railPos->unit, val, dir);
}

u8 func_ov021_0219e398(FieldActor *param_1, RailPosition *param_2, RailPosition *param_3, u32 *param_4) {
    ActorPositionRail *railPos;
    NoGridMapper *mapper;
    u8 flags;
    u32 tile;

    flags = 0;
    railPos = GetNPCRailPosPtrAddr(param_1);
    mapper = FldActSys_GetNoGridMapper(GetActorMModelSystem(param_1));
    if (func_ov021_021b7de0(railPos->unit, param_2) == 0) {
        flags |= 1;
    }
    if (func_ov021_021b7de0(railPos->unit, param_3) == 0) {
        flags |= 1;
    }
    if (func_ov010_0216dcb4(param_1) == 0) {
        tile = FieldNoGridMapper_GetTileAtPos(mapper, param_3);
        if (GetTileFlags(tile) & 1) {
            flags |= 2;
        }
        if (param_4 != NULL) {
            *param_4 = tile;
        }
    }
    if (func_ov021_0219e498(param_1, param_3) != 0) {
        flags |= 4;
    }
    return flags;
}

u8 func_ov021_0219e418(FieldActor *param_1, u16 param_2) {
    return func_ov021_0219e424(param_1, param_2, (u32 *)0x0);
}

u8 func_ov021_0219e424(FieldActor *param_1, u32 param_2, u32 *param_3) {
    FieldRailSystem *rail;
    RailPosition posA;
    u16 actorFaceDir;
    ActorPositionRail *railPos;
    RailPosition tmp;
    u32 result;

    rail = FieldNoGridMapper_GetRailSystem(FldActSys_GetNoGridMapper(GetActorMModelSystem(param_1)));
    railPos = GetNPCRailPosPtrAddr(param_1);
    func_ov021_021b7dc0(railPos->unit, &posA);
    actorFaceDir = GetActorFaceDir(param_1);
    if (actorFaceDir == param_2) {
        result = func_ov021_021b7e60(railPos->unit, func_ov021_0219e938(param_2), &tmp, NULL);
    } else {
        result = func_ov021_021b7cd8(rail, &posA, func_ov021_0219e938(param_2), &tmp);
    }
    if (result != 0) {
        return func_ov021_0219e398(param_1, &posA, &tmp, param_3);
    }
    return 1;
}

BOOL func_ov021_0219e498(FieldActor *param_1, RailPosition *param_2) {
    MMSys *system;
    FieldRailSystem *rail;
    RailUnit *unit;
    FieldActor *actor;
    u32 iter;
    VecFx32 posA;
    VecFx32 posB;
    fx32 ratio;

    system = GetActorMModelSystem(param_1);
    rail = FieldNoGridMapper_GetRailSystem(FldActSys_GetNoGridMapper(system));
    iter = 0;
    func_ov021_021b7d38(rail, param_2, &posA);
    ratio = FX_Div(func_ov021_021b7c38(rail), 0x4000);
    if (NextActor(system, &actor, &iter) == 1) {
        do {
            if (actor == param_1) {
                continue;
            }
            if (CheckActorFlag(actor, 0x80) != 0) {
                continue;
            }
            {
                BOOL off = CheckActorFlag(actor, 0x2000) == 0;
                if (off) {
                    continue;
                }
            }
            unit = FldAct_GetRailUnit(actor);
            if (func_ov021_021b8040(unit) != 0) {
                func_ov021_021b7e60(unit, func_ov021_021b8038(unit), NULL, &posB);
                if (func_ov021_021b83b0(&posA, &posB, ratio) != 0) {
                    return 1;
                }
            }
            func_ov021_021b7ed4(unit, &posB);
            if (func_ov021_021b83b0(&posA, &posB, ratio) != 0) {
                return 1;
            }
        } while (NextActor(system, &actor, &iter) == 1);
    }
    return 0;
}

FieldActor *FindActorByRailPos(MMSys *system, const RailPosition *position, BOOL checkInit) {
    u32 iter = 0;
    FieldRailSystem *rail;
    FieldActor *actor;
    VecFx32 posA;
    VecFx32 posB;
    fx32 ratio;

    rail = FieldNoGridMapper_GetRailSystem(FldActSys_GetNoGridMapper(system));
    func_ov021_021b7d38(rail, position, &posA);
    ratio = FX_Div(func_ov021_021b7c38(rail), 0x4000);
    while (NextActor(system, &actor, &iter) == 1) {
        RailPosition initPos;
        RailPosition railPos;

        if (IsActorFlagClear(actor, 0x2000)) {
            continue;
        }
        if (checkInit) {
            func_ov021_0219e334(actor, &initPos.componentId);
            func_ov021_021b7d38(rail, &initPos, &posB);
            if (func_ov021_021b83b0(&posA, &posB, ratio)) {
                return actor;
            }
        }
        GetActorRailPos(actor, &railPos);
        func_ov021_021b7d38(rail, &railPos, &posB);
        if (func_ov021_021b83b0(&posA, &posB, ratio)) {
            return actor;
        }
    }
    return NULL;
}

u8 func_ov021_0219e634(FieldActor *actor, RailPosition *position) {
    MMSys *system;
    ActorPositionRail *pAVar2;

    system = GetActorMModelSystem(actor);
    FldActSys_GetNoGridMapper(system);
    pAVar2 = GetNPCRailPosPtrAddr(actor);
    return func_ov021_021b7e60(pAVar2->unit, ConvDirToRailDir(GetActorFaceDir(actor)), position, NULL);
}

u32 GetActorRailPosForDir(FieldActor *param_1, u16 param_2, RailPosition *param_3) {
    u16 actorFaceDir;
    MMSys *system;
    NoGridMapper *mapper;
    FieldRailSystem *rail;
    ActorPositionRail *pAVar3;
    u32 railDir;

    system = GetActorMModelSystem(param_1);
    mapper = FldActSys_GetNoGridMapper(system);
    rail = FieldNoGridMapper_GetRailSystem(mapper);
    actorFaceDir = GetActorFaceDir(param_1);
    pAVar3 = GetNPCRailPosPtrAddr(param_1);
    if (actorFaceDir != param_2) {
        GetActorRailPos(param_1, param_3);
        railDir = ConvDirToRailDir(param_2);
        railDir = func_ov021_021b7cd8(rail, param_3, railDir, param_3);
        return railDir;
    }
    railDir = ConvDirToRailDir(param_2);
    return func_ov021_021b7e60(pAVar3->unit, railDir, param_3, NULL);
}

void func_ov021_0219e6c8(FieldActor *param_1, u16 param_2) {
    MMSys *system;
    NoGridMapper *mapper;
    FieldRailSystem *railSystem;
    RailPosition railPos;
    VecFx32 pos;

    system = GetActorMModelSystem(param_1);
    mapper = FldActSys_GetNoGridMapper(system);
    railSystem = FieldNoGridMapper_GetRailSystem(mapper);
    GetActorRailPos(param_1, &railPos);
    func_ov021_021b7c90(railSystem, &railPos, ConvDirToRailDir(param_2), &pos);
    SetActorInitialGPosX(param_1, GetGPosX(param_1));
    SetActorInitialGPosY(param_1, FldAct_GetGPosY(param_1));
    SetActorInitialGPosZ(param_1, GetGPosZ(param_1));
    SetActorGPosX(param_1, (s16)((pos.x >> 4) / 4096));
    SetActorGPosY(param_1, (s16)((pos.y >> 4) / 4096));
    SetActorGPosZ(param_1, (s16)((pos.z >> 4) / 4096));
}

void func_ov021_0219e764(FieldActor *param_1, VecFx16 *param_2) {
    u32 faceDir;

    FldAct_GetRailUnit(param_1);
    faceDir = GetActorFaceDir(param_1);
    func_ov021_0219e784(param_1, (u16)faceDir, param_2);
}

void func_ov021_0219e784(FieldActor *actor, u32 dir, VecFx16 *dest) {
    RailUnit *railUnit;
    u32 idx;
    u16 angle;
    s32 cosVal;
    s32 sinVal;
    MtxFx33 mtx;
    VecFx32 vec;
    s32 z;
    s32 y;
    s32 x;

    railUnit = FldAct_GetRailUnit(actor);
    func_ov021_021b8050(railUnit, &dest->x);
    idx = func_ov021_021b80d8(railUnit);
    angle = data_ov021_021d841c[*(data_ov021_021d84d4 + idx * 4 + dir)];
    cosVal = FX_SinCosTable_[(angle >> 4) * 2 + 1];
    dest->y = FX_Mul(cosVal, dest->y);
    VEC_Set(&vec, dest->x, dest->y, dest->z);
    x = dest->x;
    y = dest->y;
    z = dest->z;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    sinVal = FX_SinCosTable_[(angle >> 4) * 2];
    MTX_RotY33_(&mtx, sinVal, cosVal);
    MTX_MultVec33(&vec, &mtx, &vec);
    VEC_Normalize(&vec, &vec);
    VEC_Fx16Set(dest, vec.x, vec.y, vec.z);
}

void func_ov021_0219e828(FieldActor *param_1) {
    func_ov021_0219e968(param_1, sizeof(ActorPositionRail));
}

void func_ov021_0219e834(FieldActor *param_1) {
    ActorPositionRail *railPos;

    railPos = GetNPCRailPosPtrAddr(param_1);
    func_ov021_0219ea78(railPos, param_1);
}

void func_ov021_0219e844(FieldActor *param_1) {
    ActorPositionRail *pAVar1;

    pAVar1 = GetNPCRailPosPtrAddr(param_1);
    func_ov021_0219ea94(pAVar1, param_1);
}

void func_ov021_0219e854(FieldActor *param_1) {
    ActorPositionRail *pAVar1;

    pAVar1 = GetNPCRailPosPtrAddr(param_1);
    func_ov021_0219eab8(pAVar1, param_1);
}

void func_ov021_0219e864(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 0);
}

void func_ov021_0219e870(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 1);
}

void func_ov021_0219e87c(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 2);
}

void func_ov021_0219e888(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 3);
}

void func_ov021_0219e894(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 4);
}

void func_ov021_0219e8a0(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 5);
}

void func_ov021_0219e8ac(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 6);
}

void func_ov021_0219e8b8(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 7);
}

void func_ov021_0219e8c4(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 8);
}

void func_ov021_0219e8d0(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 9);
}

void func_ov021_0219e8dc(FieldActor *param_1) {
    func_ov021_0219eb18(param_1, 10);
}

void func_ov021_0219e8e8(FieldActor *param_1) {
    func_ov021_0219eb4c(param_1);
}

void func_ov021_0219e8f0(FieldActor *param_1) {
    func_ov021_0219eb90(param_1);
}

void func_ov021_0219e8f8(FieldActor *param_1) {
    func_ov021_0219eba0(param_1, 0);
}

void func_ov021_0219e904(FieldActor *param_1) {
    func_ov021_0219eba0(param_1, 9);
}

void func_ov021_0219e910(FieldActor *param_1) {
    func_ov021_0219ebf4(param_1);
}

void func_ov021_0219e918(FieldActor *param_1) {
    func_ov021_0219ed7c(param_1);
}

void func_ov021_0219e920(FieldActor *param_1) {
    func_ov021_0219ed8c(param_1);
}

void func_ov021_0219e928(FieldActor *param_1) {
    func_ov021_0219edc4(param_1);
}

void func_ov021_0219e930(FieldActor *param_1) {
    func_ov021_0219ef04(param_1);
}

u32 func_ov021_0219e938(u32 param_1) {
    return ConvDirToRailDir(param_1);
}

u32 func_ov021_0219e940(s32 speed) {
    u32 result = 1;

    switch (speed) {
    case 0x10:
        result = 0;
        break;
    case 4:
        result = 2;
        break;
    case 2:
        result = 3;
        break;
    case 8:
        result = 1;
        break;
    }
    return result;
}

void *func_ov021_0219e968(FieldActor *param_1, u32 param_2) {
    RailPosition posData;
    RailPosition *pos = &posData;
    BOOL set = func_ov010_02164380(param_1, pos);
    ActorPositionRail *rail = ClearActorPositionBlock(param_1, param_2);

    func_ov021_0219ea5c(rail, param_1);
    if (set) {
        SetActorInitedPositionRail(param_1, pos);
    } else {
        MI_CpuFill8(pos, 0, sizeof(RailPosition));
        SetActorInitedPositionRail(param_1, pos);
    }
    return rail;
}

BOOL func_ov021_0219e9a8(FieldActor *actor, s32 front, s32 side, const RailPosition *pos) {
    s32 width = GetActorWalkAreaW(actor);
    s32 height = GetActorWalkAreaH(actor);

    if (width != -1) {
        s32 min = side - width;
        s32 max = side + width;

        if (min > pos->posSide || max < pos->posSide) {
            return TRUE;
        }
    }
    if (height != -1) {
        s32 min = front - height;
        s32 max = front + height;

        if (min > pos->posFront || max < pos->posFront) {
            return TRUE;
        }
    }
    return FALSE;
}

s32 func_ov021_0219e9f8(const s32 *param_1, s32 param_2) {
    s32 val;
    s32 i;

    i = 0;
    val = *param_1;
    while (param_2 != val) {
        i = i + 1;
        val = param_1[i];
    }
    return i;
}

s32 func_ov021_0219ea10(const s32 *param_1, s32 param_2) {
    u32 max;

    max = func_ov021_0219e9f8(param_1, param_2);
    max = GFL_RandomLC(max);
    return param_1[max];
}

s32 func_ov021_0219ea24(s32 param_1, s32 param_2) {
    const s32 *ptr;
    u32 max;

    ptr = func_ov021_0219ea40(param_1);
    max = func_ov021_0219e9f8(ptr, param_2);
    max = GFL_RandomLC(max);
    return ptr[max];
}

const s32 *func_ov021_0219ea40(s32 key) {
    const DirListEntry *entry;
    s32 id;

    entry = data_ov021_021d84e8;
    id = 0;
    do {
        if (id == key) {
            return entry->list;
        }
        entry++;
        id = entry->id;
    } while (id != 0xb);
    return NULL;
}

void func_ov021_0219ea5c(ActorPositionRail *rail, FieldActor *actor) {
    MMSys *system;
    NoGridMapper *mapper;
    RailUnit *unit;

    system = GetActorMModelSystem(actor);
    mapper = FldActSys_GetNoGridMapper(system);
    unit = FieldNoGridMapper_AcquireUnit(mapper);
    rail->unit = unit;
    rail->position.componentIsLine = 2;
}

void func_ov021_0219ea78(ActorPositionRail *param_1, FieldActor *param_2) {
    RailPosition RStack_18;

    func_ov021_021b7dc0(param_1->unit, &RStack_18);
    SetActorPositionRailCoordinates(param_1, &RStack_18);
}

void func_ov021_0219ea94(ActorPositionRail *rail, FieldActor *actor) {
    MMSys *system;
    NoGridMapper *mapper;

    system = GetActorMModelSystem(actor);
    mapper = FldActSys_GetNoGridMapper(system);
    FieldNoGridMapper_ReleaseUnit(mapper, rail->unit);
    ClearActorFlag(actor, 0x2000);
}

void func_ov021_0219eab8(ActorPositionRail *rail, FieldActor *actor) {
    MMSys *system;
    NoGridMapper *mapper;
    RailUnit *unit;

    system = GetActorMModelSystem(actor);
    mapper = FldActSys_GetNoGridMapper(system);
    unit = FieldNoGridMapper_AcquireUnit(mapper);
    rail->unit = unit;
    func_ov021_0219eaec(rail, actor);
}

void SetActorPositionRailCoordinates(ActorPositionRail *rail, const RailPosition *pos) {
    rail->position = *pos;
}

void func_ov021_0219eaec(ActorPositionRail *rail, FieldActor *actor) {
    VecFx32 VStack_18;

    if (rail->position.componentIsLine != 2) {
        RailUnit_SetRailPos(rail->unit, &rail->position);
        RailUnit_GetCalcPos(rail->unit, &VStack_18);
        SetActorWPosValue(actor, &VStack_18);
    }
}

// pokebw2's ActorRailWaitWork, the look-around move's work: the rail place, then the frames to the next turn and the
// set of directions it turns among (data_ov021_021d84e8's number)
typedef struct {
    ActorPositionRail rail;
    s16 wait;
    u16 dirs;
} ActorRailWaitWork;

WORK_AREA_CHECK(ActorRailWaitWork, FIELD_ACTOR_MOVE_WORK_SIZE);

// pokebw2's ActorRailWalkWork, the random walk's work (func_ov021_0219eba0, func_ov021_0219ebf4): the set of
// directions it walks among, the frames it waits, and the place on the rail it set out from
typedef struct {
    ActorPositionRail rail;
    u8 state : 4;
    u8 dir : 4;
    s8 wait;
    s8 side;
    u8 front;
} ActorRailWalkWork;

WORK_AREA_CHECK(ActorRailWalkWork, FIELD_ACTOR_MOVE_WORK_SIZE);

// pokebw2's ActorRailPatrolWork, the walk back and forth (func_ov021_0219ed8c, func_ov021_0219edc4): set when it
// walks back, and the place on the rail it set out from
typedef struct {
    ActorPositionRail rail;
    u8 state;
    u8 reversed;
    s8 side;
    u8 front;
} ActorRailPatrolWork;

WORK_AREA_CHECK(ActorRailPatrolWork, FIELD_ACTOR_MOVE_WORK_SIZE);

void func_ov021_0219eb18(FieldActor *param_1, u16 param_2) {
    ActorRailWaitWork *railPos;

    railPos = func_ov021_0219e968(param_1, sizeof(ActorRailWaitWork));
    railPos->wait = func_ov021_0219ea10(data_ov021_021d84ac, -1);
    railPos->dirs = param_2;
    func_ov010_0216d674(param_1, 0);
    func_ov010_0216da50(param_1);
}

void func_ov021_0219eb4c(FieldActor *param_1) {
    ActorRailWaitWork *railPos;
    s32 dir;

    railPos = (ActorRailWaitWork *)GetNPCRailPosPtrAddr(param_1);
    railPos->wait--;
    if (railPos->wait <= 0) {
        railPos->wait = func_ov021_0219ea10(data_ov021_021d84ac, -1);
        dir = func_ov021_0219ea24(railPos->dirs, 9);
        CheckSetActorFaceDir(param_1, dir);
    }
    func_ov021_0219ea78(&railPos->rail, param_1);
}

void func_ov021_0219eb90(FieldActor *param_1) {
    ActorPositionRail *pAVar1;

    pAVar1 = GetNPCRailPosPtrAddr(param_1);
    func_ov021_0219ea94(pAVar1, param_1);
}

void func_ov021_0219eba0(FieldActor *param_1, s8 param_2) {
    RailPosition sp0;
    ActorRailWalkWork *work;

    work = func_ov021_0219e968(param_1, sizeof(ActorRailWalkWork));
    GetActorRailPos(param_1, &sp0);
    work->state = 0;
    work->wait = 0;
    work->dir = param_2;
    work->front = sp0.posFront;
    work->side = sp0.posSide;
    func_ov010_0216d674(param_1, 0U);
    func_ov010_0216da50(param_1);
}

void func_ov021_0219ebf4(FieldActor *param_1) {
    ActorRailWalkWork *move;
    RailPosition pos;
    RailPosition buf;
    u32 dirVal;
    u32 faceDir;
    u32 acmd;

    move = (ActorRailWalkWork *)GetNPCRailPosPtrAddr(param_1);
    switch (move->state) {
    case 0:
        func_ov010_0216da50(param_1);
        func_ov010_0216da7c(param_1);
        faceDir = GetActorFaceDir(param_1);
        acmd = GetAcmdForDir(faceDir, 0);
        func_ov010_0216d438(param_1, acmd);
        move->state++;
        break;
    case 1:
        if (func_ov021_02197edc(param_1) == 0) {
            break;
        }
        move->wait = func_ov021_0219ea10(data_ov021_021d84ac, -1);
        move->state++;
        /* falls through */
    case 2:
        move->wait--;
        if (move->wait != 0) {
            break;
        }
        move->state++;
        /* falls through */
    case 3:
        dirVal = func_ov021_0219ea24(move->dir, 9);
        ChangeActorDirection(param_1, dirVal);
        if (func_ov021_0219e418(param_1, dirVal) != 0) {
            move->state = 0;
            break;
        }
        GetActorRailPos(param_1, &pos);
        GetActorRailPosForDir(param_1, dirVal, &buf);
        if (pos.componentId != buf.componentId) {
            move->state = 0;
            break;
        }
        if (func_ov021_0219e9a8(param_1, move->front, move->side, &buf) != 0) {
            move->state = 0;
            break;
        }
        acmd = GetAcmdForDir(dirVal, 0xc);
        func_ov010_0216d438(param_1, acmd);
        func_ov010_0216da44(param_1);
        move->state++;
        /* falls through */
    case 4:
        if (func_ov021_02197edc(param_1) == 0) {
            break;
        }
        func_ov010_0216da50(param_1);
        move->state = 0;
        break;
    }
    func_ov021_0219ea78(&move->rail, param_1);
}

void func_ov021_0219ed7c(FieldActor *param_1) {
    ActorPositionRail *pAVar1;

    pAVar1 = GetNPCRailPosPtrAddr(param_1);
    func_ov021_0219ea94(pAVar1, param_1);
}

void func_ov021_0219ed8c(FieldActor *param_1) {
    ActorRailPatrolWork *walk;
    RailPosition position;

    walk = func_ov021_0219e968(param_1, sizeof(ActorRailPatrolWork));
    GetActorRailPos(param_1, &position);
    walk->state = 0;
    walk->reversed = 0;
    walk->front = position.posFront;
    walk->side = position.posSide;
    func_ov010_0216d674(param_1, 0);
    func_ov010_0216da50(param_1);
}

void func_ov021_0219edc4(FieldActor *param_1) {
    ActorRailPatrolWork *move;
    RailPosition pos;
    RailPosition buf;
    u32 motionDir;
    u32 blocked;
    u32 moved;
    u32 acmd;
    u32 kind;
    u32 result;
    u32 invDir;

    move = (ActorRailPatrolWork *)GetNPCRailPosPtrAddr(param_1);
    switch (move->state) {
    case 0:
        motionDir = GetDefaultActorDir(param_1);
        if (move->reversed == 1) {
            motionDir = GetInverseDirection((u16)motionDir);
        }
        ChangeActorDirection(param_1, motionDir);
        move->state = 1;
        /* falls through */
    case 1:
        GetActorRailPos(param_1, &pos);
        if (move->reversed != 0 && move->front == pos.posFront && move->side == pos.posSide) {
            invDir = GetInverseDirection(GetActorMotionDir(param_1));
            ChangeActorDirection(param_1, invDir);
            move->reversed = 0;
        }
        moved = FALSE;
        motionDir = GetActorMotionDir(param_1);
        blocked = func_ov021_0219e418(param_1, motionDir);
        GetActorRailPosForDir(param_1, motionDir, &buf);
        result = func_ov021_0219e9a8(param_1, move->front, move->side, &buf);
        if (pos.componentId != buf.componentId) {
            moved = TRUE;
        }
        if ((blocked & 1) || result != 0 || moved) {
            move->reversed = 1;
            motionDir = GetInverseDirection((u16)motionDir);
            blocked = func_ov021_0219e418(param_1, motionDir);
            GetActorRailPosForDir(param_1, motionDir, &buf);
        }
        kind = 0xc;
        if (blocked != 0) {
            kind = 0x20;
        }
        acmd = GetAcmdForDir(motionDir, (u16)kind);
        func_ov010_0216d438(param_1, acmd);
        func_ov010_0216da44(param_1);
        move->state = 2;
        break;
    case 2:
        if (func_ov021_02197edc(param_1) == 1) {
            func_ov010_0216da50(param_1);
            move->state = 0;
        }
        break;
    }
    func_ov021_0219ea78(&move->rail, param_1);
}

void func_ov021_0219ef04(FieldActor *param_1) {
    ActorPositionRail *pAVar1;

    pAVar1 = GetNPCRailPosPtrAddr(param_1);
    func_ov021_0219ea94(pAVar1, param_1);
}
