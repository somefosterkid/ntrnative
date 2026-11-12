// After pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov052/scrcmd_pokemon_league.c, Black 2's
// script plugin for the Pokémon League's rooms. In Black the commands are in the main command table
// (FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS, entries 0x1b1 to 0x1b3) and the rooms' gimmicks in overlay 56. The second command is the same
// code as Black 2's; the others are written from Black's assembly.
#include "types.h"
#include "field/field.h"
#include "field/field_async_proc.h"
#include "field/field_camera.h"
#include "field/field_script.h"
#include "field/pokemon_league_gimmicks.h"
#include "gfl/overlay.h"
#include "system/game_system.h"

BOOL func_ov021_021b5610(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    Field *field = GSYS_GetField(gsys);
    u16 room = ScriptReadAny(vm, env);

    ScriptWork_CallEvent(work, func_ov056_021f36c0(gsys, field, room - 1));
    return TRUE;
}

// Sets the camera of an Elite Four's room
BOOL func_ov021_021b5650(VM *vm, FieldScriptEnv *env) {
    Field *field = GSYS_GetField(FieldScriptEnv_GetGameSystem(env));
    u16 room = ScriptReadAny(vm, env);
    FieldCamera *camera = Field_GetCameraSystem(field);

    FieldCamera_SetDefaultsIndex(camera, func_ov056_021f36c8(room - 1));
    return FALSE;
}

// Starts the process of an Elite Four's room
BOOL func_ov021_021b568c(VM *vm, FieldScriptEnv *env) {
    FieldAsyncProcManager *mgr = Field_GetAsyncProcMgr(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)));

    switch (ScriptReadAny(vm, env)) {
    case 1:
        FieldAsyncProcManager_AddProc(OVERLAY_ID(56), mgr, &data_ov056_021f4344);
        break;
    case 2:
        FieldAsyncProcManager_AddProc(OVERLAY_ID(56), mgr, &data_ov056_021f4374);
        break;
    case 3:
        FieldAsyncProcManager_AddProc(OVERLAY_ID(56), mgr, &data_ov056_021f438c);
        break;
    case 4:
        FieldAsyncProcManager_AddProc(OVERLAY_ID(56), mgr, &data_ov056_021f435c);
        break;
    }
    return FALSE;
}
