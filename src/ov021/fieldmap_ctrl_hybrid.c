// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/fieldmap_ctrl_hybrid.c, ported by tools/port_file.py.
#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_camera.h"
#include "field/field_controller.h"
#include "field/field_map.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_player.h"
#include "field/field_rail.h"
#include "field/fieldmap_ctrl_hybrid.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/game_data.h"
#include "system/game_system.h"

// What FieldPlayer_GetMoveDirByKey returns when no direction is held
#define MOVE_DIR_NONE 9

// The controller types
#define HYBRID_GRID 0
#define HYBRID_RAIL 1

void func_ov021_021a6dc4(FieldmapCtrlHybrid *controller, Field *field);
void func_ov021_021a6f5c(Field *field, FieldmapCtrlHybrid *controller);
void func_ov021_021a7010(Field *field, FieldmapCtrlHybrid *controller);
void func_ov021_021a70d0(Field *field, FieldmapCtrlHybrid *controller, u16 dir, BOOL keyDir, const RailPosition *railPos,
                         u32 pressedKeys, u32 heldKeys);
void func_ov021_021a712c(Field *field, FieldmapCtrlHybrid *controller, u16 dir, BOOL keyDir, const VecFx32 *pos,
                         u32 pressedKeys, u32 heldKeys);
BOOL func_ov021_021a7198(Field *field, FieldmapCtrlHybrid *controller, VecFx32 *pos, u16 dir);
BOOL func_ov021_021a71e4(Field *field, FieldmapCtrlHybrid *controller, RailPosition *railPos, u16 dir);
void func_ov021_021a7258(Field *field, FieldmapCtrlHybrid *controller, u32 type, const void *pos, u16 dir, u16 a5,
                         u16 a6);

// The update of each controller type
// pokebw2's sUpdateFuncs
static void (*data_ov021_021ddf38[2])(Field *field, FieldmapCtrlHybrid *controller) = {
    func_ov021_021a6f5c,
    func_ov021_021a7010,
};

// The player's actor on the grid, whose object code is filled in
// pokebw2's sGridPlayer
static ZoneNPC data_ov021_021ddf40 = { 0xff, 0x1, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0xffff, FALSE };

// Moves the player to the other kind of movement where it stands, if it can
void func_ov021_021a6dc4(FieldmapCtrlHybrid *controller, Field *field) {
    u16 dir = GetActorFaceDir(FieldPlayer_GetActor(controller->player));
    RailPosition railPos;
    VecFx32 pos;

    if (controller->activeType == HYBRID_GRID) {
        if (func_ov021_021a71e4(field, controller, &railPos, dir)) {
            func_ov021_021a70d0(field, controller, dir, FALSE, &railPos, 0, 0);
        }
    } else if (func_ov021_021a7198(field, controller, &pos, dir)) {
        func_ov021_021a712c(field, controller, dir, FALSE, &pos, 0, 0);
    }
    controller->unk08 = 0;
}

void FieldmapCtrlHybrid_Create(Field *field, VecFx32 *pos, u16 dir) {
    PlayerState *playerState = GameData_GetPlayerState(GSYS_GetGameData(Field_GetGameSystem(field)));
    HeapID heapId = Field_GetHeapID(field);
    FieldmapCtrlHybrid *controller =
        GFL_HeapAllocate(heapId, sizeof(FieldmapCtrlHybrid), TRUE, "fieldmap_ctrl_hybrid.c", 189);
    u32 isRail;
    FieldActor *actor;
    u16 a6;
    u16 a5;
    RailPosition *railPos;

    Field_SetController(field, controller);
    controller->player = Field_GetPlayer(field);
    isRail = func_02012ac0(playerState);
    controller->activeType = 2;
    actor = FieldPlayer_GetActor(controller->player);
    func_ov021_0219f058(actor, &a5, &a6);
    if (isRail == FALSE) {
        FieldPlayer_InitGrid(controller->player, heapId);
        func_ov021_021a3664(controller->player, heapId);
        FieldPlayer_SetDirection(controller->player, dir);
        func_ov021_021a7258(field, controller, isRail, pos, dir, a5, a6);
    } else {
        railPos = PlayerState_GetRailPos(playerState);
        func_ov021_021a3664(controller->player, heapId);
        FieldPlayer_InitGrid(controller->player, heapId);
        FieldPlayer_SetDirection(controller->player, dir);
        func_ov021_021a7258(field, controller, isRail, railPos, dir, a5, a6);
    }
    Field_SetPlayerPosPtr(field, GetMModelWPosPtr(actor));
    FieldPlayer_GetWPos(controller->player, pos);
}

void FieldmapCtrlHybrid_Free(Field *field) {
    GFL_HeapFree(Field_GetController(field));
}

void FieldmapCtrlHybrid_Update(Field *field, VecFx32 *pos) {
    FieldmapCtrlHybrid *controller = Field_GetController(field);

    data_ov021_021ddf38[controller->activeType](field, controller);
    FieldPlayer_GetWPos(controller->player, pos);
}

VecFx32 *FieldmapCtrlHybrid_GetPos(Field *field) {
    return GetMModelWPosPtr(FieldPlayer_GetActor(Field_GetPlayer(field)));
}

// On the grid: walks onto a rail when the player walks against a wall from a rail's tile
void func_ov021_021a6f5c(Field *field, FieldmapCtrlHybrid *controller) {
    u32 pressedKeys = GCTX_HIDGetPressedKeys();
    u32 heldKeys = GCTX_HIDGetHeldKeys();
    u32 flags = 0;
    FieldActor *actor = FieldPlayer_GetActor(controller->player);
    u16 dir = FieldPlayer_GetMoveDirByKey(controller->player, heldKeys);
    u32 tileUnder;
    u32 tileAhead;
    RailPosition railPos;

    if ((IsAllActorAcmdFinished(actor) == TRUE || func_ov021_021a2fb0(controller->player) == TRUE)
        && dir != MOVE_DIR_NONE) {
        tileUnder = FieldPlayer_GetTileTypeUnder(controller->player);
        tileAhead = FieldPlayer_GetTileTypeInDir(controller->player, dir);
        if (func_ov021_021bb1a0(GetTileClass(tileUnder)) && MapTile_BlocksCollision(tileAhead)
            && func_ov021_021a71e4(field, controller, &railPos, dir)) {
            func_ov021_021a70d0(field, controller, dir, TRUE, &railPos, pressedKeys, heldKeys);
            return;
        }
    }
    if (FieldPlayer_CheckRunningShoesFlag(controller->player) == TRUE) {
        flags |= 1;
    }
    FieldPlayer_UpdateGridMovement(controller->player, pressedKeys, heldKeys, flags);
}

// On a rail: steps off onto the grid the same way
void func_ov021_021a7010(Field *field, FieldmapCtrlHybrid *controller) {
    u32 pressedKeys = GCTX_HIDGetPressedKeys();
    u32 heldKeys = GCTX_HIDGetHeldKeys();
    FieldActor *actor = FieldPlayer_GetActor(controller->player);
    u16 dir;
    BOOL keyDir;
    u32 tileUnder;
    u32 tileAhead;
    VecFx32 pos;

    if (controller->unk08 == 0) {
        dir = FieldPlayer_GetMoveDirByKey(controller->player, heldKeys);
        keyDir = TRUE;
    } else {
        dir = GetActorMotionDir(actor);
        keyDir = FALSE;
    }
    if ((IsAllActorAcmdFinished(actor) == TRUE || func_ov021_021a2fb0(controller->player) == TRUE)
        && dir != MOVE_DIR_NONE) {
        tileUnder = FieldPlayer_GetTileTypeUnder(controller->player);
        tileAhead = FieldPlayer_GetTileTypeInDir(controller->player, dir);
        if (func_ov021_021bb1a0(GetTileClass(tileUnder)) && MapTile_BlocksCollision(tileAhead)
            && func_ov021_021a7198(field, controller, &pos, dir)) {
            func_ov021_021a712c(field, controller, dir, keyDir, &pos, pressedKeys, heldKeys);
            controller->unk08 = 0;
            return;
        }
    }
    controller->unk08 = func_ov021_021a3698(controller->player);
    FieldPlayer_UpdateRailMovement(controller->player, pressedKeys, heldKeys);
}

void func_ov021_021a70d0(Field *field, FieldmapCtrlHybrid *controller, u16 dir, BOOL keyDir, const RailPosition *railPos,
                         u32 pressedKeys, u32 heldKeys) {
    FieldActor *actor = FieldPlayer_GetActor(controller->player);
    u16 a6;
    u16 a5;

    func_ov021_0219f058(actor, &a5, &a6);
    FieldPlayer_UpdateGridMovement(controller->player, 0, 0, 0);
    if (!keyDir) {
        dir = GetActorFaceDir(actor);
    }
    func_ov021_021a7258(field, controller, HYBRID_RAIL, railPos, dir, a5, a6);
    FieldPlayer_UpdateRailMovement(controller->player, pressedKeys, heldKeys);
}

void func_ov021_021a712c(Field *field, FieldmapCtrlHybrid *controller, u16 dir, BOOL keyDir, const VecFx32 *pos,
                         u32 pressedKeys, u32 heldKeys) {
    u32 flags = 0;
    FieldActor *actor = FieldPlayer_GetActor(controller->player);
    u16 a6;
    u16 a5;

    func_ov021_0219f058(actor, &a5, &a6);
    FieldPlayer_UpdateRailMovement(controller->player, 0, 0);
    if (!keyDir) {
        dir = GetActorFaceDir(actor);
    }
    func_ov021_021a7258(field, controller, HYBRID_GRID, pos, dir, a5, a6);
    if (FieldPlayer_CheckRunningShoesFlag(controller->player) == TRUE) {
        flags |= 1;
    }
    FieldPlayer_UpdateGridMovement(controller->player, pressedKeys, heldKeys, flags);
}

// Whether the grid tile in the direction is free, with the player's position
BOOL func_ov021_021a7198(Field *field, FieldmapCtrlHybrid *controller, VecFx32 *pos, u16 dir) {
    FieldG3DMapper *mapper = Field_GetG3DMapper(field);
    VecFx32 ahead;

    FieldPlayer_GetWPos(controller->player, pos);
    ahead = *pos;
    ExpandVecInGridDir(dir, &ahead, FX32_CONST(16));
    if (!MapTile_BlocksCollision(GetTileTypeAtPos(mapper, &ahead))) {
        return TRUE;
    }
    return FALSE;
}

// Whether there is a rail where the player stands, and where on it
BOOL func_ov021_021a71e4(Field *field, FieldmapCtrlHybrid *controller, RailPosition *railPos, u16 dir) {
    FieldRailSystem *rail = FieldNoGridMapper_GetRailSystem(Field_GetNoGridMapper(field));
    VecFx32 hit;
    VecFx32 pos;
    VecFx32 top;
    RailPosition found;
    RailPosition next;
    BOOL result;

    FieldPlayer_GetWPos(controller->player, &pos);
    top.x = pos.x;
    top.y = pos.y + FX32_CONST(4);
    top.z = pos.z;
    pos.y -= FX32_CONST(4);
    result = CalculateRailCurves(rail, &pos, &top, &found, &hit);
    if (result) {
        result = func_ov021_021b7cd8(rail, &found, ConvDirToRailDir(dir), &next);
    }
    *railPos = found;
    return result;
}

// Switches the player to the controller type at the position, a VecFx32 for the grid and a RailPosition for a rail
void func_ov021_021a7258(Field *field, FieldmapCtrlHybrid *controller, u32 type, const void *pos, u16 dir, u16 a5,
                         u16 a6) {
    NoGridMapper *mapper = Field_GetNoGridMapper(field);
    FieldCamera *camera = Field_GetCameraSystem(field);
    FieldActor *actor;
    const VecFx32 *vec;
    VecFx32 position;

    if (controller->activeType == type) {
        return;
    }
    actor = FieldPlayer_GetActor(controller->player);
    if (func_ov010_0216dc4c(actor)) {
        func_ov010_0216d4b4(actor);
    }
    if (type == HYBRID_GRID) {
        vec = pos;
        position = *vec;
        data_ov021_021ddf40.modelId = FldAct_GetObjCode(actor);
        func_ov021_021a368c(controller->player);
        FldAct_Transplant(actor, &data_ov021_021ddf40);
        SetActorGPos(actor, FX_Whole(position.x) / 16, FX_Whole(position.y) / 16, FX_Whole(position.z) / 16, dir);
        CopyActorWPos(actor, &position);
        FieldPlayer_SetWPos(controller->player, &position);
        FieldNoGridMapper_ClearCameraParent(mapper);
        FieldCamera_ChangeTransformType(camera, 0);
        FieldCamera_ResetBind(camera);
        FieldCamera_LoadDefaults(camera);
    } else {
        func_ov021_021a3680(controller->player, pos);
        FieldNoGridMapper_SetCameraParent(mapper, (RailUnit *)func_ov021_021a36a4(controller->player));
        FieldCamera_EnableDelay(camera);
    }
    CheckSetActorFaceDir(actor, dir);
    SetActorMotionDir(actor, dir);
    func_ov021_0219f0b0(actor, a5, a6);
    controller->activeType = type;
}
