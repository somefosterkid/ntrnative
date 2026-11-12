// Black's code at ov021 0x021a5880-0x021a6cb8: the player on the rails and in free space. The name is the file name its
// heap calls pass (pokebw2's field_player_nogrid.c).

#include "types.h"
#include "field/event_data.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_controller.h"
#include "field/field_map.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_player.h"
#include "field/field_player_core.h"
#include "field/field_player_nogrid.h"
#include "field/field_rail.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "field/ov021_0219d524.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"

// What the player is doing, which the next command depends on (the grid file's GRID_STATE_BRAKE)
#define GRID_STATE_BRAKE 3

// pokebw2's names (its field_player_nogrid.c): the sound the player makes spinning
typedef struct StepSound {
    u8 active;
    u8 kind;
    s16 step;
} StepSound;

// The input hold of a slide: bit 0 active, bit 1 held, bit 2 pending, bits 3-7 mode (pokebw2's bitfields)
typedef struct SlideState {
    u8 flags;
    s16 time;
    u32 dir;
} SlideState;

struct FieldPlayerNoGrid {
    u32 state;
    u32 flags;
    StepSound sound;
    SlideState slide;
    FieldPlayerCore *core;
    Field *field;
    FieldActor *actor;
    RailUnit *railUnit;
};

// What the player does on each kind of slide (pokebw2's SlideFunc)
typedef u32 (*SlideFunc)(FieldPlayerNoGrid *nogrid, u32 dir, u32 a2);

// pokebw2's FieldPlayerGrid_Free

Field *FieldPlayerCore_GetField(FieldPlayerCore *core);

void func_ov021_021a6bb4(SlideState *slide, s32 param_2, FieldActor *param_3);
u32 func_ov021_021a682c(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3);
u32 func_ov021_021a5a68(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u32 param_4, s32 param_5);
void func_ov021_021a5b4c(FieldPlayerNoGrid *nogrid, u32 param_2, s32 param_3);
void func_ov021_021a5ad4(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u32 param_4, u32 param_5, s32 param_6);
void func_ov021_021a5b7c(FieldPlayerNoGrid *nogrid);
void func_ov021_021a6c30(SlideState *slide);
void func_ov021_021a6b88(SlideState *slide);

// pokebw2's sZoneNPC
static ZoneNPC data_ov021_021ddee0 = {
    .uid = 0xFF,
    .modelId = 1,
    .moveCode = 0x45,
    .areaW = 0xFFFF,
    .areaH = 0xFFFF,
    .isRail = TRUE,
};

FieldPlayerNoGrid *func_ov021_021a5880(FieldPlayerCore *core, HeapID heapId)
{
    FieldPlayerNoGrid *nogrid;

    nogrid = GFL_HeapAllocate(heapId, sizeof(FieldPlayerNoGrid), 1, "field_player_nogrid.c", 0x1a0);
    nogrid->core = core;
    nogrid->field = FieldPlayerCore_GetField(core);
    nogrid->actor = FieldPlayerCore_GetActor(nogrid->core);
    if (GetActorMoveCode(nogrid->actor) != 0x45) {
        data_ov021_021ddee0.modelId = FldAct_GetObjCode(nogrid->actor);
        FldAct_Transplant(nogrid->actor, &data_ov021_021ddee0);
    }
    nogrid->railUnit = FldAct_GetRailUnit(nogrid->actor);
    {
        RailPosition pos = { 0 };
        SetActorInitedPositionRail(nogrid->actor, &pos);
    }
    func_ov021_021a6b88(&nogrid->slide);
    switch (FieldPlayerCore_GetExState(nogrid->core)) {
    case 2:
        FieldPlayerCore_SetSpecialSeq(nogrid->core, 4);
        func_ov021_021a3ce8(nogrid->core);
        break;
    case 3:
        FieldPlayerCore_SetSpecialSeq(nogrid->core, 0x100);
        func_ov021_021a3ce8(nogrid->core);
        break;
    }
    return nogrid;
}

void func_ov021_021a5920(FieldPlayerNoGrid *nogrid)
{
    GFL_HeapFree(nogrid);
}

void func_ov021_021a5928(FieldPlayerNoGrid *param, RailPosition *railPos)
{
    if (GetActorMoveCode(param->actor) != 0x45) {
        data_ov021_021ddee0.modelId = FldAct_GetObjCode(param->actor);
        FldAct_Transplant(param->actor, &data_ov021_021ddee0);
    }

    param->railUnit = FldAct_GetRailUnit(param->actor);
    SetActorInitedPositionRail(param->actor, railPos);
}

void func_ov021_021a5964(FieldPlayerNoGrid *nogrid)
{
    nogrid->railUnit = NULL;
}

void func_ov021_021a596c(FieldPlayerNoGrid *nogrid, u32 pressedKeys, u32 heldKeys) {
    u16 dir;
    u32 result;
    u32 command;

    dir = FieldPlayerCore_GetMoveDirByKey(nogrid->core, heldKeys);
    func_ov021_021a3cc0(nogrid->core);
    func_ov021_021a6bb4(&nogrid->slide, dir, nogrid->actor);
    result = func_ov021_021a682c(nogrid, dir, 0);
    command = func_ov021_021a5a68(nogrid, result, pressedKeys, heldKeys, 0);
    func_ov021_021a5b4c(nogrid, heldKeys, command);
    func_ov021_021a5ad4(nogrid, command, result, pressedKeys, heldKeys, 0);
    func_ov021_021a5b7c(nogrid);
    func_ov021_021a6c30(&nogrid->slide);
}

BOOL func_ov021_021a59dc(FieldPlayerNoGrid *nogrid)
{
    if (nogrid->state == GRID_STATE_BRAKE)
        return TRUE;
    return FALSE;
}


u32 func_ov021_021a5c3c(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u16 param_4, s32 param_5);
u32 func_ov021_021a6014(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u16 param_4, s32 param_5);
u32 func_ov021_021a64b4(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u16 param_4, s32 param_5);
u32 func_ov021_021a652c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
u32 func_ov021_021a658c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
u32 func_ov021_021a661c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
u32 func_ov021_021a666c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a6738(FieldPlayerNoGrid *nogrid);
void func_ov021_021a672c(FieldPlayerNoGrid *nogrid);
u8 func_ov021_0219e424(FieldActor *nogrid, u32 param_2, u32 *param_3);
u32 func_ov021_021a5ca4(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
u32 func_ov021_021a5d04(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
u32 func_ov021_021a6c6c(FieldPlayerNoGrid *nogrid, u32 param_2);
u32 func_ov021_021a607c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
u32 func_ov021_021a60dc(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
u32 func_ov021_021a66f8(FieldPlayerNoGrid *nogrid, u32 param_2);
u32 func_ov021_021a6908(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3);
u32 func_ov021_021a6974(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3);
u32 func_ov021_021a69d8(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3);
u32 func_ov021_021a6a3c(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3);
u32 func_ov021_021a6a64(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3);
u32 func_ov021_021a6a8c(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3);
u32 func_ov021_021a6aa0(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3);
const SlideFunc data_ov021_021d8ab8[8] = { NULL, func_ov021_021a6908, func_ov021_021a6974, func_ov021_021a69d8, func_ov021_021a6a3c, func_ov021_021a6a64, func_ov021_021a6a8c, func_ov021_021a6aa0 };
u32 GetActorRailPosForDir(FieldActor *nogrid, u32 param_2, RailPosition *param_3);

const u8 data_ov021_021d8ab0[2][4] = { { 0x00, 0x02, 0x01, 0x03 }, { 0x00, 0x03, 0x01, 0x02 } };

void func_ov021_021a6bac(SlideState *slide);
void func_ov021_021a6b9c(SlideState *slide);
void func_ov021_021a63ec(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a63f0(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a6428(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a6454(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a6478(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
u32 func_ov021_021a6200(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a5ea0(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a5ea4(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a5edc(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a5f14(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a5f38(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a5f74(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a5f98(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a635c(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
u32 func_ov021_021a5e28(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a6b08(StepSound *sound, u32 param_2);
void func_ov021_021a6b30(StepSound *sound);
void func_ov021_021a6b50(StepSound *sound, FieldActor *param_2);
s32 func_ov021_021a6c4c(SlideState *slide, s32 param_2);
u32 func_ov021_021a6c64(SlideState *slide);
u32 func_ov021_021a5dd8(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a5bc0(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a6278(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a627c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a62b4(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a62d8(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a62fc(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a6338(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
u32 func_ov021_021a61b0(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6);
void func_ov021_021a6870(FieldPlayerNoGrid *nogrid);


BOOL func_ov021_021a59ec(FieldPlayerNoGrid *nogrid) {
    BOOL result = TRUE;

    if (!func_ov021_021a66f8(nogrid, 1)) {
        result = FALSE;
    }
    return result;
}

void func_ov021_021a5a00(FieldPlayerNoGrid *nogrid, const RailPosition *param_2) {
    SetActorInitedPositionRail(nogrid->actor, param_2);
}

void func_ov021_021a5a0c(FieldPlayerNoGrid *nogrid, RailPosition *pos) {
    GetActorRailPos(nogrid->actor, pos);
}

void func_ov021_021a5a18(FieldPlayerNoGrid *nogrid, u32 dir, RailPosition *pos) {
    GetActorRailPosForDir(nogrid->actor, dir, pos);
}

void func_ov021_021a5a24(FieldPlayerNoGrid *nogrid, VecFx32 *pos) {
    RailUnit_GetCalcPos(nogrid->railUnit, pos);
}



RailUnit *func_ov021_021a5a30(FieldPlayerNoGrid *nogrid) {
    return nogrid->railUnit;
}

void func_ov021_021a5a34(FieldPlayerNoGrid *nogrid) {
    u16 dir;
    FieldActor *actor;

    if (nogrid->state == 3) {
        actor = nogrid->actor;
        dir = GetActorFaceDir(actor);
        func_ov010_0216d4b4(actor);
        CheckSetActorFaceDir(actor, dir);
        func_ov010_0216d674(actor, 0);
        FieldPlayerCore_SetMoveStatus(nogrid->core, 0);
    }
}

u32 func_ov021_021a5a68(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u32 param_4, s32 param_5) {
    u32 uStack_1c;

    switch(FieldPlayerCore_GetExState(nogrid->core)) {
    case 0:
        uStack_1c = func_ov021_021a5c3c(nogrid, param_3, param_4, param_2 & 0xffff, param_5);
        break;
    case 1:
        uStack_1c = func_ov021_021a6014(nogrid, param_3, param_4, param_2 & 0xffff, param_5);
        break;
    case 2:
    case 3:
        uStack_1c = func_ov021_021a64b4(nogrid, param_3, param_4, param_2 & 0xffff, param_5);
    }
    return uStack_1c;
}

void func_ov021_021a5ad4(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u32 param_4, u32 param_5, s32 param_6) {
    switch (FieldPlayerCore_GetExState(nogrid->core)) {
    case 0:
        func_ov021_021a5bc0(nogrid, param_2, param_4, param_5, param_3, param_6);
        return;
    case 1:
        func_ov021_021a5f98(nogrid, param_2, param_4, param_5, param_3, param_6);
        return;
    case 2:
    case 3:
        func_ov021_021a635c(nogrid, param_2, param_4, param_5, param_3, param_6);
        return;
    }
}


void func_ov021_021a5b4c(FieldPlayerNoGrid *nogrid, u32 param_2, s32 param_3) {
    FieldPlayerCore_CalcSeparateKeyDirs(nogrid->core, param_2);
    func_ov021_021a6738(nogrid);
    if (param_3 == 2) {
        func_ov021_021a672c(nogrid);
    }
}

u8 func_ov021_021a5b6c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 *param_4) {
    return func_ov021_0219e424(param_2, param_3, param_4);
}

void func_ov021_021a5b7c(FieldPlayerNoGrid *nogrid) {
    if (func_ov021_021a66f8(nogrid, 0x8) != 0) {
        func_ov021_021a6b08(&nogrid->sound, 0);
    } else if (func_ov021_021a66f8(nogrid, 0x10) != 0) {
        func_ov021_021a6b08(&nogrid->sound, 1);
    } else {
        func_ov021_021a6b30(&nogrid->sound);
    }
    func_ov021_021a6b50(&nogrid->sound, nogrid->actor);
}

void func_ov021_021a5bc0(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    FieldActor *actor = nogrid->actor;

    switch (param_2) {
    case 0:
        func_ov021_021a5ea0(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 1:
        func_ov021_021a5ea4(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 2:
        func_ov021_021a5edc(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 3:
        func_ov021_021a5f14(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 4:
        func_ov021_021a5f38(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 5:
        func_ov021_021a5f74(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    }
}


u32 func_ov021_021a5c3c(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u16 param_4, s32 param_5) {
    u32 ret = 0;
    FieldActor *actor = nogrid->actor;

    switch (nogrid->state) {
    case 0:
        ret = func_ov021_021a5ca4(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    case 1:
        ret = func_ov021_021a5d04(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    case 2:
        ret = func_ov021_021a5dd8(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    case 3:
        ret = func_ov021_021a5e28(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    }
    return ret;
}


u32 func_ov021_021a5ca4(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    if (IsAllActorAcmdFinished(param_2) == 1) {
        if (param_5 != 9) {
            if (param_5 != GetActorFaceDir(param_2) && param_6 == 0) {
                return 3;
            }
            return func_ov021_021a5d04(nogrid, param_2, param_3, param_4, param_5, param_6);
        }
        return 1;
    }
    return 0;
}

u32 func_ov021_021a5d04(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    RailPosition pos;
    u32 tileType;
    u32 tileClass;
    u32 dir;
    u32 ret;

    if (IsAllActorAcmdFinished(param_2) == 0) {
        return 0;
    }
    if (param_5 == 9) {
        return func_ov021_021a5ca4(nogrid, param_2, param_3, param_4, param_5, param_6);
    }
    ret = func_ov021_021a5b6c(nogrid, param_2, param_5, NULL);
    if (param_6 == 1 && (ret & 2) != 0) {
        ret = 0;
    }
    if (ret == 0) {
        return 2;
    }
    if ((ret & 2) != 0) {
        GetActorRailPosForDir(param_2, param_5, &pos);
        tileType = func_ov021_0219d724(param_2, &pos);
        dir = 9;
        tileClass = GetTileClass(tileType);
        GetTileFlags(tileType);
        if (MapTile_IsLedgeU(tileClass)) {
            dir = 0;
        } else if (MapTile_IsLedgeD(tileClass)) {
            dir = 1;
        } else if (MapTile_IsLedgeL(tileClass)) {
            dir = 2;
        } else if (MapTile_IsLedgeR(tileClass)) {
            dir = 3;
        }
        if (dir != 9 && dir == param_5) {
            return 5;
        }
    }
    return 4;
}




u32 func_ov021_021a5dd8(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    BOOL BVar1;
    u32 result;

    BVar1 = IsAllActorAcmdFinished(param_2);
    if (BVar1 == 0) {
        return 0;
    }
    if (param_5 == 9) {
        result = func_ov021_021a5ca4(nogrid, param_2, param_3, param_4, param_5, param_6);
        return result;
    }
    result = func_ov021_021a5d04(nogrid, param_2, param_3, param_4, param_5, param_6);
    return result;
}

u32 func_ov021_021a5e28(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 dir;

    dir = GetActorFaceDir(param_2);
    if (param_5 != 9 && param_5 != dir) {
        func_ov010_0216d4b4(param_2);
        return func_ov021_021a5d04(nogrid, param_2, param_3, param_4, param_5, param_6);
    }
    if (IsAllActorAcmdFinished(param_2) == 0) {
        return 0;
    }
    if (param_5 == 9) {
        return func_ov021_021a5ca4(nogrid, param_2, param_3, param_4, param_5, param_6);
    }
    return func_ov021_021a5d04(nogrid, param_2, param_3, param_4, param_5, param_6);
}


void func_ov021_021a5ea0(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
}

void func_ov021_021a5ea4(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    if (param_5 == 9) {
        param_5 = GetActorFaceDir(param_2);
    }
    acmd = GetAcmdForDir(param_5, 0);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 0;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 0);
}

void func_ov021_021a5edc(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;
    u16 acmd_00;
    if (param_6 == 1) {
        acmd_00 = 0x14;
    } else if ((param_4 & 2) != 0) {
        acmd_00 = 0x58;
    } else {
        acmd_00 = 0xc;
    }
    acmd = GetAcmdForDir(param_5, acmd_00);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 1;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 1);
}

void func_ov021_021a5f14(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    acmd = GetAcmdForDir(param_5, 0x28);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 2;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 2);
}

void func_ov021_021a5f38(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    acmd = GetAcmdForDir(param_5, 0x1c);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 3;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 0);
    if (func_ov021_021a6c6c(nogrid, (u32)param_5) == 0) {
        GFL_SndSEPlay(0x55a);
    }
}

void func_ov021_021a5f74(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    acmd = GetAcmdForDir(param_5, 0x38);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 1;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 1);
}

void func_ov021_021a5f98(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    FieldActor *actor = nogrid->actor;

    switch (param_2) {
    case 0:
        func_ov021_021a6278(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 1:
        func_ov021_021a627c(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 2:
        func_ov021_021a62b4(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 3:
        func_ov021_021a62d8(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 4:
        func_ov021_021a62fc(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 5:
        func_ov021_021a6338(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    }
}

u32 func_ov021_021a6014(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u16 param_4, s32 param_5) {
    u32 ret = 0;
    FieldActor *actor = nogrid->actor;

    switch (nogrid->state) {
    case 0:
        ret = func_ov021_021a607c(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    case 1:
        ret = func_ov021_021a60dc(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    case 2:
        ret = func_ov021_021a61b0(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    case 3:
        ret = func_ov021_021a6200(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    }
    return ret;
}



u32 func_ov021_021a607c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    if (IsAllActorAcmdFinished(param_2) == 1) {
        if (param_5 != 9) {
            if (param_5 != GetActorFaceDir(param_2) && param_6 == 0) {
                return 3;
            }
            return func_ov021_021a60dc(nogrid, param_2, param_3, param_4, param_5, param_6);
        }
        return 1;
    }
    return 0;
}

u32 func_ov021_021a60dc(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    RailPosition pos;
    u32 tileType;
    u32 tileClass;
    u32 dir;
    u32 ret;

    if (IsAllActorAcmdFinished(param_2) == 0) {
        return 0;
    }
    if (param_5 == 9) {
        return func_ov021_021a607c(nogrid, param_2, param_3, param_4, param_5, param_6);
    }
    ret = func_ov021_021a5b6c(nogrid, param_2, param_5, NULL);
    if (param_6 == 1 && (ret & 2) != 0) {
        ret = 0;
    }
    if (ret == 0) {
        return 2;
    }
    if ((ret & 2) != 0) {
        GetActorRailPosForDir(param_2, param_5, &pos);
        tileType = func_ov021_0219d724(param_2, &pos);
        dir = 9;
        tileClass = GetTileClass(tileType);
        GetTileFlags(tileType);
        if (MapTile_IsLedgeU(tileClass)) {
            dir = 0;
        } else if (MapTile_IsLedgeD(tileClass)) {
            dir = 1;
        } else if (MapTile_IsLedgeL(tileClass)) {
            dir = 2;
        } else if (MapTile_IsLedgeR(tileClass)) {
            dir = 3;
        }
        if (dir != 9 && dir == param_5) {
            return 5;
        }
    }
    return 4;
}



u32 func_ov021_021a61b0(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    BOOL BVar1;
    u32 result;

    BVar1 = IsAllActorAcmdFinished(param_2);
    if (BVar1 == 0) {
        return 0;
    }
    if (param_5 == 9) {
        result = func_ov021_021a607c(nogrid, param_2, param_3, param_4, param_5, param_6);
        return result;
    }
    result = func_ov021_021a60dc(nogrid, param_2, param_3, param_4, param_5, param_6);
    return result;
}

u32 func_ov021_021a6200(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 dir;

    dir = GetActorFaceDir(param_2);
    if (param_5 != 9 && param_5 != dir) {
        func_ov010_0216d4b4(param_2);
        return func_ov021_021a60dc(nogrid, param_2, param_3, param_4, param_5, param_6);
    }
    if (IsAllActorAcmdFinished(param_2) == 0) {
        return 0;
    }
    if (param_5 == 9) {
        return func_ov021_021a607c(nogrid, param_2, param_3, param_4, param_5, param_6);
    }
    return func_ov021_021a60dc(nogrid, param_2, param_3, param_4, param_5, param_6);
}


void func_ov021_021a6278(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
}

void func_ov021_021a627c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    if (param_5 == 9) {
        param_5 = GetActorFaceDir(param_2);
    }
    acmd = GetAcmdForDir(param_5, 0);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 0;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 0);
}

void func_ov021_021a62b4(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    acmd = GetAcmdForDir(param_5, 0x14);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 1;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 1);
}

void func_ov021_021a62d8(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    acmd = GetAcmdForDir(param_5, 0x28);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 2;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 2);
}

void func_ov021_021a62fc(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    acmd = GetAcmdForDir(param_5, 0x1c);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 3;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 0);
    if (func_ov021_021a6c6c(nogrid, (u32)param_5) == 0) {
        GFL_SndSEPlay(0x55a);
    }
}

void func_ov021_021a6338(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    acmd = GetAcmdForDir(param_5, 0x38);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 1;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 1);
}

void func_ov021_021a635c(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    FieldActor *actor = FieldPlayerCore_GetActor(nogrid->core);

    switch (param_2) {
    case 0:
        func_ov021_021a63ec(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 1:
        func_ov021_021a63f0(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 2:
        func_ov021_021a6428(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 3:
        func_ov021_021a6454(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    case 4:
        func_ov021_021a6478(nogrid, actor, param_3, param_4, param_5, param_6);
        return;
    }
}


void func_ov021_021a63ec(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
}

void func_ov021_021a63f0(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    if (param_5 == 9) {
        param_5 = GetActorFaceDir(param_2);
    }
    acmd = GetAcmdForDir(param_5, 0);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 0;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 0);
}

void func_ov021_021a6428(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;
    u16 acmd_00;

    acmd_00 = 0x14;
    if (param_6 != 1) {
        acmd_00 = 0x10;
    }
    acmd = GetAcmdForDir(param_5, acmd_00);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 1;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 1);
}

void func_ov021_021a6454(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    acmd = GetAcmdForDir(param_5, 0x28);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 2;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 2);
}

void func_ov021_021a6478(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 acmd;

    acmd = GetAcmdForDir(param_5, 0x1c);
    FldAct_SetAcmd(param_2, acmd);
    nogrid->state = 3;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 0);
    if (func_ov021_021a6c6c(nogrid, (u32)param_5) == 0) {
        GFL_SndSEPlay(0x55a);
    }
}

u32 func_ov021_021a64b4(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3, u16 param_4, s32 param_5) {
    u32 ret;
    FieldActor *actor = FieldPlayerCore_GetActor(nogrid->core);

    ret = 0;
    switch (nogrid->state) {
    case 0:
        ret = func_ov021_021a652c(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    case 1:
        ret = func_ov021_021a658c(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    case 2:
        ret = func_ov021_021a661c(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    case 3:
        ret = func_ov021_021a666c(nogrid, actor, param_2, param_3, param_4, param_5);
        break;
    }
    return ret;
}

u32 func_ov021_021a652c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    if (IsAllActorAcmdFinished(param_2) == 1) {
        if (param_5 != 9) {
            if (param_5 != GetActorFaceDir(param_2) && param_6 == 0) {
                return 3;
            }
            return func_ov021_021a658c(nogrid, param_2, param_3, param_4, param_5, param_6);
        }
        return 1;
    }
    return 0;
}

u32 func_ov021_021a658c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u32 ret;
    u32 tileType;
    u32 flags;

    if (IsAllActorAcmdFinished(param_2) == 0) {
        return 0;
    }
    if (param_5 == 9) {
        return func_ov021_021a652c(nogrid, param_2, param_3, param_4, param_5, param_6);
    }
    ret = func_ov021_021a5b6c(nogrid, param_2, param_5, &tileType);
    if (param_6 == 1 && ret != 0 && (ret & 0x10) == 0) {
        ret = 0;
    }
    if (tileType != 0xFFFFFFFF) {
        GetTileClass(tileType);
        flags = GetTileFlags(tileType);
        if (ret == 0) {
            if ((flags & 2) != 0) {
                return 2;
            }
            if (param_6 == 1) {
                return 2;
            }
        }
    }
    return 4;
}

u32 func_ov021_021a661c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    if (IsAllActorAcmdFinished(param_2) == 0) {
        return 0;
    }
    if (param_5 == 9) {
        return func_ov021_021a652c(nogrid, param_2, param_3, param_4, param_5, param_6);
    }
    return func_ov021_021a658c(nogrid, param_2, param_3, param_4, param_5, param_6);
}

u32 func_ov021_021a666c(FieldPlayerNoGrid *nogrid, FieldActor *param_2, u32 param_3, u32 param_4, u16 param_5, s32 param_6) {
    u16 dir;

    dir = GetActorFaceDir(param_2);
    if (param_5 != 9 && param_5 != dir) {
        func_ov010_0216d4b4(param_2);
        return func_ov021_021a658c(nogrid, param_2, param_3, param_4, param_5, param_6);
    }
    if (IsAllActorAcmdFinished(param_2) == 0) {
        return 0;
    }
    if (param_5 == 9) {
        return func_ov021_021a652c(nogrid, param_2, param_3, param_4, param_5, param_6);
    }
    return func_ov021_021a658c(nogrid, param_2, param_3, param_4, param_5, param_6);
}






void func_ov021_021a66e4(FieldPlayerNoGrid *nogrid, u32 param_2) {
    nogrid->flags = param_2 | nogrid->flags;
}

void func_ov021_021a66ec(FieldPlayerNoGrid *nogrid, u32 param_2) {
    nogrid->flags = ~param_2 & nogrid->flags;
}

u32 func_ov021_021a66f8(FieldPlayerNoGrid *nogrid, u32 param_2) {
    if ((nogrid->flags & param_2) != 0) {
        return 1;
    }
    return 0;
}

void func_ov021_021a6708(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66e4(nogrid, 1);
}

void func_ov021_021a6714(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66ec(nogrid, 1);
}

void func_ov021_021a6720(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66e4(nogrid, 2);
}

void func_ov021_021a672c(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66ec(nogrid, 2);
}

void func_ov021_021a6738(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66ec(nogrid, 4);
}

void func_ov021_021a6744(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66e4(nogrid, 8);
}

void func_ov021_021a6750(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66ec(nogrid, 8);
}

void func_ov021_021a675c(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66e4(nogrid, 0x10);
}

void func_ov021_021a6768(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66ec(nogrid, 0x10);
}

void func_ov021_021a6774(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66e4(nogrid, 0x20);
}

void func_ov021_021a6780(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66ec(nogrid, 0x20);
}

void func_ov021_021a678c(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66e4(nogrid, 0x40);
}

void func_ov021_021a6798(FieldPlayerNoGrid *nogrid) {
    func_ov021_021a66ec(nogrid, 0x40);
}

u32 func_ov021_021a67a4(FieldPlayerNoGrid *nogrid) {
    u32 tileType;
    BOOL BVar3;
    u32 tileClass;

    tileType = GetCachedTileUnderActor(nogrid->actor);
    if (func_ov021_021a66f8(nogrid, 2) == 0) {
        tileClass = (u32)GetTileClass(tileType);
        BVar3 = IsTileSlidingIce(tileClass);
        if (BVar3 == 1) {
            return 1;
        }
        BVar3 = func_ov021_021bb1ac(tileClass);
        if (BVar3 == 1) {
            return 2;
        }
        BVar3 = func_ov021_021bb1b8(tileClass);
        if (BVar3 == 1) {
            return 3;
        }
        BVar3 = func_ov021_021bb1c4(tileClass);
        if (BVar3 == 1) {
            return 4;
        }
        BVar3 = func_ov021_021bb1d0(tileClass);
        if (BVar3 == 1) {
            return 5;
        }
        BVar3 = func_ov021_021bb1dc(tileClass);
        if (BVar3 == 1) {
            return 6;
        }
        BVar3 = func_ov021_021bb1e8(tileClass);
        if (BVar3 == 1) {
            return 7;
        }
    }
    return 0;
}

u32 func_ov021_021a682c(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3) {
    u32 type;

    if (IsAllActorAcmdFinished(nogrid->actor) == 0) {
        return param_2;
    }
    if (param_3 == 0) {
        type = func_ov021_021a67a4(nogrid);
        if (type != 0) {
            return data_ov021_021d8ab8[type](nogrid, param_2, param_3);
        }
    }
    func_ov021_021a6870(nogrid);
    return param_2;
}


void func_ov021_021a6870(FieldPlayerNoGrid *nogrid) {
    u16 direction;
    u32 result;
    FieldActor *actor;

    actor = nogrid->actor;
    result = func_ov021_021a66f8(nogrid, 0x59);
    if (result != 0) {
        ClearActorFlag(actor, 0x18);
    }
    result = func_ov021_021a66f8(nogrid, 1);
    if (result != 0) {
        func_ov021_021a6714(nogrid);
        func_ov021_021a6720(nogrid);
        FieldPlayerCore_SetMoveStatus(nogrid->core, 0);
    }
    result = func_ov021_021a66f8(nogrid, 0x18);
    if (result != 0) {
        func_ov021_021a6750(nogrid);
        func_ov021_021a6768(nogrid);
        direction = GetActorMotionDir(actor);
        direction = GetInverseDirection((u32)direction);
        ChangeActorDirection(actor, direction);
    }
    result = func_ov021_021a66f8(nogrid, 0x20);
    if (result != 0) {
        func_ov021_021a6780(nogrid);
        ChangeActorDirection(actor, 3);
    }
    result = func_ov021_021a66f8(nogrid, 0x40);
    if (result != 0) {
        func_ov021_021a6798(nogrid);
        func_ov021_021a6bac(&nogrid->slide);
    }
}

u32 func_ov021_021a6908(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3) {
    u16 dir;
    FieldActor *actor;

    actor = nogrid->actor;
    dir = GetActorMotionDir(actor);
    if (func_ov021_021a5b6c(nogrid, actor, dir, NULL) != 0) {
        func_ov021_021a6870(nogrid);
        return param_2;
    }
    func_ov021_021a6708(nogrid);
    func_ov021_021a6750(nogrid);
    func_ov021_021a6768(nogrid);
    CheckSetActorFaceDir(actor, dir);
    FldAct_SetAcmd(actor, GetAcmdForDir(dir, 0x10));
    SetActorFlag(actor, 0x18);
    nogrid->state = 1;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 1);
    return dir;
}

u32 func_ov021_021a6974(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3) {
    u16 dir;
    FieldActor *actor;

    actor = nogrid->actor;
    dir = GetActorMotionDir(actor);
    if (func_ov021_021a5b6c(nogrid, actor, dir, NULL) != 0) {
        func_ov021_021a6870(nogrid);
        return param_2;
    }
    func_ov021_021a6708(nogrid);
    func_ov021_021a6744(nogrid);
    func_ov021_021a6768(nogrid);
    FldAct_SetAcmd(actor, GetAcmdForDir(dir, 0x10));
    SetActorFlag(actor, 0x18);
    nogrid->state = 1;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 1);
    return dir;
}

u32 func_ov021_021a69d8(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3) {
    u16 dir;
    FieldActor *actor;

    actor = nogrid->actor;
    dir = GetActorMotionDir(actor);
    if (func_ov021_021a5b6c(nogrid, actor, dir, NULL) != 0) {
        func_ov021_021a6870(nogrid);
        return param_2;
    }
    func_ov021_021a6708(nogrid);
    func_ov021_021a675c(nogrid);
    func_ov021_021a6750(nogrid);
    FldAct_SetAcmd(actor, GetAcmdForDir(dir, 0x10));
    SetActorFlag(actor, 0x18);
    nogrid->state = 1;
    FieldPlayerCore_SetMoveStatus(nogrid->core, 1);
    return dir;
}

u32 func_ov021_021a6a3c(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3) {
    if (GetActorMotionDir(nogrid->actor) == 2) {
        GFL_SndSEPlay(0x55e);
    }
    return func_ov021_021a6908(nogrid, param_2, param_3);
}

u32 func_ov021_021a6a64(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3) {
    if (GetActorMotionDir(nogrid->actor) == 3) {
        GFL_SndSEPlay(0x55e);
    }
    return func_ov021_021a6908(nogrid, param_2, param_3);
}







u32 func_ov021_021a6a8c(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3) {
    u32 result;

    result = func_ov021_021a6908(nogrid, param_2, param_3);
    func_ov021_021a6774(nogrid);
    return result;
}

u32 func_ov021_021a6aa0(FieldPlayerNoGrid *nogrid, u32 param_2, u32 param_3) {
    u16 dir;

    if (func_ov021_021a66f8(nogrid, 0x40) == 0) {
        func_ov021_021a678c(nogrid);
        func_ov021_021a6b9c(&nogrid->slide);
    }
    dir = func_ov021_021a6c4c(&nogrid->slide, param_2);
    if (func_ov021_021a6c64(&nogrid->slide)) {
        SetActorFlag(nogrid->actor, 0x18);
        FldAct_SetAcmd(nogrid->actor, GetAcmdForDir(dir, 8));
        FieldPlayerCore_SetMoveStatus(nogrid->core, 1);
        dir = 9;
    } else {
        ClearActorFlag(nogrid->actor, 0x18);
    }
    return dir;
}


void func_ov021_021a6b08(StepSound *sound, u32 param_2) {
    if (sound->active != 1 || param_2 != sound->kind) {
        GFL_SndSEPlay(0x6d1);
        sound->active = 1;
        sound->kind = param_2;
        sound->step = 0;
    }
}


void func_ov021_021a6b30(StepSound *sound) {
    if (sound->active != 0) {
        sound->active = 0;
        sound->step = 0;
        GFL_SndPlayerStop(GFL_SndSeqGetPlayerIndex(0x6d1));
    }
}

void func_ov021_021a6b50(StepSound *sound, FieldActor *param_2) {
    if (sound->active != 0) {
        sound->step = (sound->step + 1) % 4;
        SetActorFaceDir(param_2, data_ov021_021d8ab0[sound->kind][sound->step]);
    }
}



void func_ov021_021a6b88(SlideState *slide) {
    MI_CpuFill8(slide, 0, sizeof(SlideState));
    slide->dir = 9;
}

void func_ov021_021a6b9c(SlideState *slide) {
    slide->flags = (slide->flags & ~1) | 1;
}


void func_ov021_021a6bac(SlideState *slide) {
    func_ov021_021a6b88(slide);
}

void func_ov021_021a6bb4(SlideState *slide, s32 param_2, FieldActor *param_3) {
    u8 flags;
    u8 flags2;

    flags = slide->flags;
    if (((u32) (flags << 0x1F) >> 0x1F) != 0) {
        if (((u32) (flags << 0x18) >> 0x1B) != 0) {
            if (param_2 != 9) {
                slide->time = 0;
                slide->flags = (u8) (flags & ~0xF8);
                if (((u32) (slide->flags << 0x1E) >> 0x1F) != 0) {
                    slide->dir = param_2;
                }
            }
        } else if (param_2 == 9) {
            if (slide->time < 0x1E) {
                slide->time = (s16) (slide->time + 1);
            } else {
                slide->flags = (u8) ((flags & ~0xF8) | 8);
            }
        }
        if (IsAllActorAcmdFinished(param_3) == 1) {
            flags2 = slide->flags;
            if (((u32) (flags2 << 0x18) >> 0x1B) == 0) {
                slide->flags = (u8) (flags2 & ~2);
                slide->flags = (u8) (slide->flags & ~4);
                return;
            }
            slide->flags = (u8) (2 | flags2);
            slide->flags = (u8) (4 | slide->flags);
        }
    }
}


void func_ov021_021a6c30(SlideState *slide) {
    if (((u32)slide->flags << 29) >> 31) {
        slide->flags = slide->flags & ~4;
    }
    if (!(((u32)slide->flags << 30) >> 31)) {
        slide->dir = 9;
    }
}


s32 func_ov021_021a6c4c(SlideState *slide, s32 param_2) {
    if (((u32)slide->flags << 29) >> 31) {
        return 1;
    }
    if (slide->dir != 9) {
        return slide->dir;
    }
    return param_2;
}



u32 func_ov021_021a6c64(SlideState *slide) {
    return ((u32)slide->flags << 30) >> 31;
}


u32 func_ov021_021a6c6c(FieldPlayerNoGrid *nogrid, u32 param_2) {
    FieldActor *pFVar1;
    GameSystem *gsys;
    GameData *gameData;
    EventData *data;
    RailPosition RStack_18;

    pFVar1 = FieldPlayerCore_GetActor(nogrid->core);
    gsys = Field_GetGameSystem(nogrid->field);
    gameData = GSYS_GetGameData(gsys);
    data = GameData_GetEventData(gameData);
    GetActorRailPosForDir(pFVar1, param_2, &RStack_18);
    if (GetWarpIDByPlayerPosRail(data, &RStack_18) != 0xffff) {
        return 1;
    }
    return 0;
}
