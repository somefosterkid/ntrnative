// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_gym.c, ported by tools/port_file.py.
// The script commands of the gyms' puzzles, which call into each gym's gimmick overlay. The ROM has no name for the
// file; scrcmd_gym.c is descriptive. Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/field_script.h"
#include "field/field_sound.h"
#include "field/gimmick_roller_coaster.h"
#include "field/gym_gimmick.h"
#include "field/gym_icirrus.h"
#include "field/scrcmd_gym.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"

BOOL func_ov021_021b2380(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u8 a1 = VM_Read16(vm);
    GameEvent *event = func_ov039_021f3e24(gsys, a1);

    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov021_021b23b8(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    u8 a1 = VM_Read16(vm);

    func_ov039_021f4134(gsys, a1);
    return FALSE;
}

extern void func_ov039_021f3d74(GameSystem *gsys);
BOOL func_ov021_021b23e0(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    func_ov039_021f3d74(gsys);
    return FALSE;
}

extern void func_ov040_021f37a4(Field *param_1);
BOOL func_ov021_021b23f0(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    func_ov040_021f37a4(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)));
    return FALSE;
}

extern u32 func_ov040_021f37c8(Field *param_1);
BOOL func_ov021_021b240c(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    Field *field = GSYS_GetField(FieldScriptEnv_GetGameSystem(env));
    u16 *var = ScriptReadVar(vm, env);

    *var = func_ov040_021f37c8(field) != 0;
    return FALSE;
}

extern GameEvent *func_ov040_021f37ec(GameSystem *gsys, u32 a1);
BOOL func_ov021_021b2444(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u8 a1 = VM_Read16(vm);
    GameEvent *event = func_ov040_021f37ec(gsys, a1);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

extern GameEvent *func_ov042_021f3834(GameSystem *gsys, u32 state, u8 flag);
BOOL func_ov021_021b247c(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u8 a1 = VM_Read16(vm);
    GameEvent *event = func_ov042_021f3834(gsys, a1, 1);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov021_021b24b8(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u8 a1 = VM_Read16(vm);
    GameEvent *event = func_ov042_021f3834(gsys, a1, 0);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

extern GameEvent *func_ov042_021f3888(GameSystem *gsys, u32 state);
BOOL func_ov021_021b24f4(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u8 a1 = VM_Read16(vm);
    GameEvent *event = func_ov042_021f3888(gsys, a1);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

extern GameEvent *func_ov038_021f4184(GameSystem *param_1, s32 param_2);
BOOL func_ov021_021b252c(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 *var = ScriptReadVar(vm, env);
    GameEvent *event = func_ov038_021f4184(gsys, *var);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

extern GameEvent *func_ov038_021f41b0(GameSystem *param_1, s32 param_2);
BOOL func_ov021_021b2568(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 *var = ScriptReadVar(vm, env);
    GameEvent *event = func_ov038_021f41b0(gsys, *var);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

extern GameEvent *func_ov038_021f41dc(GameSystem *gsys, u16 a1);
BOOL func_ov021_021b25a4(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 a1 = VM_Read16(vm);
    GameEvent *event = func_ov038_021f41dc(gsys, a1);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

extern GameEvent *func_ov043_021f398c(GameSystem *gsys, s32 param_2);
BOOL func_ov021_021b25dc(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 a1 = VM_Read16(vm);
    GameEvent *event = func_ov043_021f398c(gsys, a1);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}












