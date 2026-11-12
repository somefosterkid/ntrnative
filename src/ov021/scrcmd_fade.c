// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_fade.c, ported by tools/port_file.py.
// The script commands that fade the screens: to and from black or white, quickly or slowly, and their waits. The ROM
// has no name for the file; scrcmd_fade.c is descriptive. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_script.h"
#include "field/scrcmd_fade.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "system/brightness.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"

enum {
    FIELD_FADE_IN_SLOW,
    FIELD_FADE_IN,
    FIELD_FADE_OUT_SLOW,
    FIELD_FADE_OUT,
};

typedef struct {
    Field *field;
    u32 mode;
    BOOL white;
} FieldFadeEvent;

BOOL letBrightnessFinishAdjusting(VM *vm, void *env);
BOOL func_ov021_021b2874(VM *vm, void *env);
static GameEventReturnCode func_ov036_021ab580(GameEvent *event, u32 *state, void *data);

BOOL s00B3_FadeEx(VM *vm, FieldScriptEnv *env) {
    u16 mode = VM_Read16(vm);
    u16 start = VM_Read16(vm);
    u16 end = VM_Read16(vm);
    u16 slowness = VM_Read16(vm);

    GFL_FadeSet(mode, start, end, slowness);
    return FALSE;
}

BOOL letBrightnessFinishAdjusting(VM *vm, void *env) {
    if (GFL_FadeIsRunning() == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov021_021b2874(VM *vm, void *env) {
    if (Field_GetFadeFlag(GSYS_GetField(FieldScriptEnv_GetGameSystem(env))) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL s01AA_CallSeasonBanner(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = GSYS_GetGameData(gsys);
    Field *field = GSYS_GetField(gsys);
    u16 season = GameData_GetSeason(gameData);

    ScriptWork_CallEvent(work, CallFieldMapEntranceInTransition(gsys, field, 3, 0, 0, season, season));
    return FALSE;
}

BOOL s01A8_FadeInBlackQ_(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    FieldFadeTCB_Start(gsys, GSYS_GetField(gsys), 0, 0, 0);
    return FALSE;
}

BOOL s01A3_FadeInBlackQ(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    FieldFadeTCB_Start(gsys, GSYS_GetField(gsys), 0, 0, 0);
    return FALSE;
}

BOOL s01A4_FadeOutBlackQ(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    FieldFadeTCB_Start(gsys, GSYS_GetField(gsys), 1, 0, 0);
    return FALSE;
}

BOOL s01A5_FadeInWhiteQ(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    FieldFadeTCB_Start(gsys, GSYS_GetField(gsys), 0, 1, 0);
    return FALSE;
}

BOOL s01A6_FadeOutWhiteQ(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    FieldFadeTCB_Start(gsys, GSYS_GetField(gsys), 1, 1, 0);
    return FALSE;
}

BOOL s01AB_FadeInBlack(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    FieldFadeTCB_Start(gsys, GSYS_GetField(gsys), 2, 0, 0);
    return FALSE;
}

BOOL s01AC_FadeOutBlack(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    FieldFadeTCB_Start(gsys, GSYS_GetField(gsys), 3, 0, 0);
    return FALSE;
}

BOOL s01AD_FadeInWhite(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    FieldFadeTCB_Start(gsys, GSYS_GetField(gsys), 2, 1, 0);
    return FALSE;
}

BOOL s01AE_FadeOutWhite(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    FieldFadeTCB_Start(gsys, GSYS_GetField(gsys), 3, 1, 0);
    return FALSE;
}

BOOL letBrightnessFinishAdjusting(VM *vm, void *env);
BOOL func_ov021_021b2a90(VM *vm, void *env) {
    VM_SetNativeCallback(vm, letBrightnessFinishAdjusting);
    return TRUE;
}

BOOL func_ov021_021b2aa0(VM *vm, void *env) {
    VM_SetNativeCallback(vm, func_ov021_021b2874);
    return TRUE;
}



