// After pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov066/scrcmd_plugin14.c, Black 2's script
// plugin 14. Black's file holds the script commands of two gimmicks: entry 0x24d of the command table
// FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS drives overlay 59, and entries 0x24e and 0x264 to 0x267 overlay 64, which is Black 2's overlay
// 132. Those five are the same code as the end of Black 2's plugin, ported from it; the first is written from the
// assembly.
#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_script.h"
#include "field/ov059.h"
#include "field/ov064.h"
#include "system/game_system.h"
#include "system/vm.h"

BOOL func_ov021_021d2edc(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GSYS_GetField(gsys);
    u16 actorId = ScriptReadAny(vm, env);
    u16 a1 = ScriptReadAny(vm, env);
    VecFx32 pos;
    GameEvent *event;

    CopyActorWPos(FindFieldActor(GetScrEnvMMdlSys(env), actorId), &pos);
    event = func_ov059_021f3cfc(gsys, &pos, actorId, a1);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov021_021d2f44(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameEvent *event = func_ov064_021f37b8(FieldScriptEnv_GetGameSystem(env));

    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov021_021d2f6c(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    Field *field = GSYS_GetField(gsys);
    u16 actorId = ScriptReadAny(vm, env);
    u16 x = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    u16 z = ScriptReadAny(vm, env);
    u16 a4 = ScriptReadAny(vm, env);
    u16 a5 = ScriptReadAny(vm, env);
    FieldActor *actor = FindFieldActor(Field_GetActorSystem(field), actorId);
    VecFx32 from;
    VecFx32 to;
    GameEvent *event;

    if (actor != NULL) {
        CopyActorWPos(actor, &from);
    }
    to.x = x * (16 * FX32_ONE) + 8 * FX32_ONE;
    to.y = y * (FX32_ONE / 2) * 16;
    to.z = z * (16 * FX32_ONE) + 8 * FX32_ONE;
    event = func_ov064_021f3970(gsys, &from, &to, a4 * (FX32_ONE / 2) * 16, a5);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov021_021d3018(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 x = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    u16 z = ScriptReadAny(vm, env);
    VecFx32 pos;

    pos.x = x * (16 * FX32_ONE) + 8 * FX32_ONE;
    pos.y = y * (FX32_ONE / 2) * 16;
    pos.z = z * (16 * FX32_ONE) + 8 * FX32_ONE;
    func_ov064_021f3a44(gsys, &pos);
    return FALSE;
}

BOOL func_ov021_021d3068(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameEvent *event = func_ov064_021f3adc(FieldScriptEnv_GetGameSystem(env));

    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov021_021d3090(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameEvent *event = func_ov064_021f3ac0(FieldScriptEnv_GetGameSystem(env));

    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}
