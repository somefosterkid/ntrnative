// Black's code at ov021 0x021a6cb8-0x021a6dc4. The name is the file name its asserts pass (tools/name_files.py).

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
#include "field/fieldmap_ctrl_hybrid.h"
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

// pokebw2's FieldmapCtrlNoGrid: the rail controller's work, Black's layout the same
typedef struct FieldmapCtrlNoGrid {
    FieldPlayer *player;
    u32 unk04;
} FieldmapCtrlNoGrid;

FieldmapCtrlNoGrid *func_ov021_021a6d58(Field *param_1, HeapID param_2);
void func_ov021_021a6d9c(FieldmapCtrlNoGrid *param_1);
void func_ov021_021a6da4(FieldmapCtrlNoGrid *param_1);
void func_ov021_021a6cb8(Field *param_1, VecFx32 *param_2, u16 param_3);
void func_ov021_021a6d30(Field *param_1);
void func_ov021_021a6d3c(Field *param_1, VecFx32 *pos);
VecFx32 *func_ov021_021a6d48(Field *param_1);

// The controllers of the rail maps and of the hybrid maps, which fieldmap.c's table of map kinds lists
const FieldmapCtrlVTable data_ov021_021d8ad8 = {
    1, func_ov021_021a6cb8, func_ov021_021a6d3c, func_ov021_021a6d30, func_ov021_021a6d48,
};

const FieldmapCtrlVTable data_ov021_021d8aec = {
    2, FieldmapCtrlHybrid_Create, FieldmapCtrlHybrid_Update, FieldmapCtrlHybrid_Free, FieldmapCtrlHybrid_GetPos,
};

void func_ov021_021a6cb8(Field *param_1, VecFx32 *param_2, u16 param_3) {
    FieldPlayer *player;
    FieldmapCtrlNoGrid *controller;
    FieldActor *actor;
    VecFx32 *position;
    GameSystem *gsys;
    GameData *gameData;
    PlayerState *playerState;
    RailPosition *pos;

    Field_GetCameraSystem(param_1);
    Field_GetNoGridMapper(param_1);
    player = Field_GetPlayer(param_1);
    controller = func_ov021_021a6d58(param_1, Field_GetHeapID(param_1));
    Field_SetController(param_1, controller);
    actor = FieldPlayer_GetActor(player);
    position = GetMModelWPosPtr(actor);
    Field_SetPlayerPosPtr(param_1, position);
    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    playerState = GameData_GetPlayerState(gameData);
    pos = PlayerState_GetRailPos(playerState);
    FieldPlayer_SetRailPos(player, pos);
    FieldPlayer_GetRailWorldPos(player, param_2);
    FieldPlayer_SetWPos(player, param_2);
    FieldPlayer_SetDirection(player, param_3);
}

void func_ov021_021a6d30(Field *param_1) {
    func_ov021_021a6d9c(Field_GetController(param_1));
}

void func_ov021_021a6d3c(Field *param_1, VecFx32 *pos) {
    func_ov021_021a6da4(Field_GetController(param_1));
}

VecFx32 *func_ov021_021a6d48(Field *param_1) {
    FieldPlayer *player;
    FieldActor *actor;

    player = Field_GetPlayer(param_1);
    actor = FieldPlayer_GetActor(player);
    return GetMModelWPosPtr(actor);
}

FieldmapCtrlNoGrid *func_ov021_021a6d58(Field *param_1, HeapID param_2) {
    NoGridMapper *mapper;
    FieldmapCtrlNoGrid *origPlayer;
    FieldPlayer *player;
    RailUnit *unit;

    mapper = Field_GetNoGridMapper(param_1);
    origPlayer = GFL_HeapAllocate(param_2, sizeof(FieldmapCtrlNoGrid), 1, "fieldmap_ctrl_nogrid_work.c", 0x3e);
    player = Field_GetPlayer(param_1);
    origPlayer->player = player;
    func_ov021_021a3664(player, param_2);
    unit = (RailUnit *)func_ov021_021a36a4(player);
    FieldNoGridMapper_SetCameraParent(mapper, unit);
    return origPlayer;
}

void func_ov021_021a6d9c(FieldmapCtrlNoGrid *param_1) {
    GFL_HeapFree(param_1);
}


void func_ov021_021a6da4(FieldmapCtrlNoGrid *param_1) {
    u32 pressedKeys;
    u32 heldKeys;

    pressedKeys = GCTX_HIDGetPressedKeys();
    heldKeys = GCTX_HIDGetHeldKeys();
    FieldPlayer_UpdateRailMovement(param_1->player, pressedKeys, heldKeys);
}

u32 func_ov021_021a6dc0(u32 *param_1) {
    return *param_1;
}
