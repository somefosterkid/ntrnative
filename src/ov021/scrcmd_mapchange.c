// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_mapchange.c, ported by tools/port_file.py.
// The script commands that change the map: warps, warp pads, quicksand, the Union Room, rails and flying. The ROM has
// no name for the file; scrcmd_mapchange.c is descriptive. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/event_data.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_script.h"
#include "field/scrcmd_mapchange.h"
#include "field/zone.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"

// The world position of a tile
#define GRID_TO_FX32(n) ((n) * FX32_CONST(16))

GameEvent *func_ov020_02184d90(GameSystem *gsys, Field *field, u16 zoneId, RailPosition *pos, u16 dir, BOOL unk);
GameEvent *func_ov020_02184de8(GameSystem *gsys, Field *field, u16 zoneId, RailPosition *pos, u16 dir, BOOL unk);

BOOL s00C2_MapChangeWarp(VM *vm, FieldScriptEnv *env) {
    VecFx32 pos;
    GameSystem *gsys = ScriptWork_GetGameSystem(FieldScriptEnv_GetScriptWork(env));
    Field *field = GSYS_GetField(gsys);
    u16 zoneId = VM_Read16(vm);
    u16 x = VM_Read16(vm);
    u16 z = VM_Read16(vm);
    u16 dir = VM_Read16(vm);
    GameEvent *event;

    pos.x = GRID_TO_FX32(x);
    pos.y = 0;
    pos.z = GRID_TO_FX32(z);
    event = GSYS_GetNowEvent(gsys);
    GameEvent_ChainNext(event, EventMapChangeWarp_CreateGrid(gsys, field, zoneId, &pos, dir, FALSE));
    return TRUE;
}

BOOL s00BE_MapChangeFake(VM *vm, FieldScriptEnv *env) {
    VecFx32 pos;
    GameSystem *gsys = ScriptWork_GetGameSystem(FieldScriptEnv_GetScriptWork(env));
    Field *field = GSYS_GetField(gsys);
    u16 zoneId = VM_Read16(vm);
    u16 x = VM_Read16(vm);
    u16 z = VM_Read16(vm);
    u16 dir = VM_Read16(vm);
    GameEvent *event;

    pos.x = GRID_TO_FX32(x);
    pos.y = 0;
    pos.z = GRID_TO_FX32(z);
    event = GSYS_GetNowEvent(gsys);
    GameEvent_ChainNext(event, EventMapChangeFakeWarp_Create(gsys, field, zoneId, &pos, dir));
    return TRUE;
}

// Sink into the quicksand under the player
BOOL s00C1_MapChangeQuicksand(VM *vm, FieldScriptEnv *env) {
    VecFx32 effectPos;
    VecFx32 pos;
    VecFx32 playerPos;
    ZoneTrigger *trigger;
    GameData *gameData;
    EventData *eventData;
    EventWork *eventWork;
    GameEvent *event;
    FieldActor *actor;
    GameSystem *gsys = ScriptWork_GetGameSystem(FieldScriptEnv_GetScriptWork(env));
    Field *field = GSYS_GetField(gsys);
    u16 zoneId = VM_Read16(vm);
    u16 x = VM_Read16(vm);
    u16 z = VM_Read16(vm);

    gameData = GSYS_GetGameData(gsys);
    eventData = GameData_GetEventData(gameData);
    eventWork = GameData_GetEventWork(gameData);
    FieldPlayer_GetWPos(Field_GetPlayer(field), &playerPos);
    trigger = FindQuicksandTrigger(eventData, eventWork, &playerPos, 9);
    if (trigger != NULL) {
        GetTriggerCenterPos(trigger, &effectPos);
    }
    pos.x = GRID_TO_FX32(x);
    pos.y = 0;
    pos.z = GRID_TO_FX32(z);
    event = GSYS_GetNowEvent(gsys);
    GameEvent_ChainNext(event, EventMapChangeQuicksand_Create(gsys, field, &effectPos, zoneId, &pos));
    actor = FieldPlayer_GetActor(Field_GetPlayer(field));
    func_ov021_021bb578(Field_GetFieldEffects(field), actor, 0, 1);
    return TRUE;
}

BOOL s00BF_MapChangeWarpPad(VM *vm, FieldScriptEnv *env) {
    VecFx32 pos;
    GameSystem *gsys = ScriptWork_GetGameSystem(FieldScriptEnv_GetScriptWork(env));
    Field *field = GSYS_GetField(gsys);
    u16 zoneId = VM_Read16(vm);
    u16 x = VM_Read16(vm);
    u16 z = VM_Read16(vm);
    u16 dir = VM_Read16(vm);
    GameEvent *event;

    pos.x = GRID_TO_FX32(x);
    pos.y = 0;
    pos.z = GRID_TO_FX32(z);
    event = GSYS_GetNowEvent(gsys);
    GameEvent_ChainNext(event, EventMapChangeWarpPad_Create(gsys, field, zoneId, &pos, dir));
    return TRUE;
}

BOOL s00C3_MapChangeUnionRoom(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = ScriptWork_GetGameSystem(FieldScriptEnv_GetScriptWork(env));
    Field *field = GSYS_GetField(gsys);
    GameEvent *event = GSYS_GetNowEvent(gsys);

    GameEvent_ChainNext(event, EventUnionRoomWarp_Create(gsys, field));
    return TRUE;
}

BOOL s00C4_MapChangeCore(VM *vm, FieldScriptEnv *env) {
    VecFx32 pos;
    GameSystem *gsys = ScriptWork_GetGameSystem(FieldScriptEnv_GetScriptWork(env));
    Field *field = GSYS_GetField(gsys);
    u16 zoneId = VM_Read16(vm);
    u16 x = VM_Read16(vm);
    u16 y = VM_Read16(vm);
    u16 z = VM_Read16(vm);
    u16 dir = VM_Read16(vm);
    GameEvent *event;

    FieldPlayer_GetActor(Field_GetPlayer(field));
    pos.x = GRID_TO_FX32(x);
    pos.y = GRID_TO_FX32(y);
    pos.z = GRID_TO_FX32(z);
    event = GSYS_GetNowEvent(gsys);
    GameEvent_ChainNext(event, EventMapChange_CreateGridDefault(gsys, field, zoneId, &pos, dir));
    return TRUE;
}

BOOL func_ov021_021aff7c(VM *vm, FieldScriptEnv *env) {
    RailPosition pos;
    GameSystem *gsys = ScriptWork_GetGameSystem(FieldScriptEnv_GetScriptWork(env));
    Field *field = GSYS_GetField(gsys);
    u16 zoneId = VM_Read16(vm);
    u16 a2 = VM_Read16(vm);
    u16 a3 = VM_Read16(vm);
    u16 a4 = VM_Read16(vm);
    u16 dir = VM_Read16(vm);
    GameEvent *event = GSYS_GetNowEvent(gsys);

    FieldNoGridMapper_CreatePosExternal(Field_GetNoGridMapper(field), zoneId, a2, a3, a4, &pos, 21);
    GameEvent_ChainNext(event, func_ov020_02184d90(gsys, field, zoneId, &pos, dir, FALSE));
    return TRUE;
}

BOOL func_ov021_021b0000(VM *vm, FieldScriptEnv *env) {
    RailPosition pos;
    GameSystem *gsys = ScriptWork_GetGameSystem(FieldScriptEnv_GetScriptWork(env));
    Field *field = GSYS_GetField(gsys);
    u16 zoneId = VM_Read16(vm);
    u16 a2 = VM_Read16(vm);
    u16 a3 = VM_Read16(vm);
    u16 a4 = VM_Read16(vm);
    u16 dir = VM_Read16(vm);
    GameEvent *event = GSYS_GetNowEvent(gsys);

    FieldNoGridMapper_CreatePosExternal(Field_GetNoGridMapper(field), zoneId, a2, a3, a4, &pos, 21);
    GameEvent_ChainNext(event, func_ov020_02184de8(gsys, field, zoneId, &pos, dir, FALSE));
    return TRUE;
}



