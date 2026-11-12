// Black's code at ov021 0x021b2614-0x021b2830. No name is known: the module and address.

#include "types.h"
#include "field/field_script.h"
#include "field/gym_driftveil_lift.h"
#include "field/gym_icirrus.h"
#include "field/scrcmd_gym.h"
#include "field/sp_poke_gimmick.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"

struct GameEvent {
    GameEvent *parent;
    GameEventCallback callback;
    u32 state;
    void *data;
    GameSystem *gsys;
};

void func_ov043_021f42a8(GameSystem *param_1);
GameEvent *func_ov043_021f3a28(GameSystem *gsys, s32 param_2);
GameEvent *func_ov038_021f3a88(GameSystem *param_1, s32 param_2, s32 param_3);
GameEvent *func_ov046_021f3b5c(GameSystem *param_1, s32 param_2);
GameEvent *func_ov046_021f3ad0(GameSystem *gsys, s32 param_2, s32 param_3);
GameEvent *func_ov046_021f3b18(GameSystem *gsys, s32 param_2);



BOOL func_ov021_021b2614(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameEvent *event;
    BOOL flag;

    if (VM_Read16(vm) != 0) {
        flag = TRUE;
    } else {
        flag = FALSE;
    }
    event = func_ov043_021f3a28(gsys, flag);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov021_021b2650(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameEvent *event;
    BOOL flag;

    if (VM_Read16(vm) != 0) {
        flag = TRUE;
    } else {
        flag = FALSE;
    }
    event = func_ov044_021f3770(gsys, flag);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}


u32 func_ov021_021b268c(u32 param_1, FieldScriptEnv *param_2) {
    func_ov043_021f42a8(FieldScriptEnv_GetGameSystem(param_2));
    return 0U;
}

u32 func_ov021_021b269c(u32 param_1, FieldScriptEnv *param_2) {
    func_ov044_021f3970(FieldScriptEnv_GetGameSystem(param_2));
    return 0U;
}

u32 func_ov021_021b26ac(VM *param_1, FieldScriptEnv *param_2) {
    GameEvent *event;
    ScriptWork *work;

    work = FieldScriptEnv_GetScriptWork(param_2);
    event = func_ov045_021f384c(FieldScriptEnv_GetGameSystem(param_2), (s32) VM_Read16(param_1));
    if (event == NULL) {
        return 1U;
    }
    ScriptWork_CallEvent(work, event);
    return 1U;
}

u32 func_ov021_021b26e4(VM *param_1, FieldScriptEnv *param_2) {
    GameEvent *event;
    ScriptWork *work;

    work = FieldScriptEnv_GetScriptWork(param_2);
    event = func_ov042_021f3888(FieldScriptEnv_GetGameSystem(param_2), (u32) VM_Read16(param_1));
    if (event == NULL) {
        return 1U;
    }
    ScriptWork_CallEvent(work, event);
    return 1U;
}

BOOL func_ov021_021b271c(VM *vm, FieldScriptEnv *env) {
    GameSystem *pGVar2;

    pGVar2 = FieldScriptEnv_GetGameSystem(env);
    func_ov045_021f38c4(pGVar2, VM_Read16(vm));
    return 0;
}

BOOL func_ov021_021b273c(VM *vm, FieldScriptEnv *env) {
    GameEvent *event;
    ScriptWork *work;

    work = FieldScriptEnv_GetScriptWork(env);
    event = func_ov038_021f3a88(FieldScriptEnv_GetGameSystem(env), (s32) VM_Read16(vm), (s32) VM_Read16(vm));
    if (event == NULL) {
        return 1;
    }
    ScriptWork_CallEvent(work, event);
    return 1;
}

u32 func_ov021_021b277c(VM *param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *pGVar2;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    pGVar2 = FieldScriptEnv_GetGameSystem(param_2);
    event = (GameEvent *)func_ov046_021f3b5c(pGVar2, VM_Read16(param_1));
    if (event == NULL) {
        return 1;
    }
    ScriptWork_CallEvent(work, event);
    return 1;
}

BOOL func_ov021_021b27b4(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 a1 = ScriptReadAny(vm, env);
    u16 a2 = ScriptReadAny(vm, env);
    GameEvent *event = func_ov046_021f3ad0(gsys, a1, a2);

    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov021_021b27f8(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameEvent *event = func_ov046_021f3b18(gsys, ScriptReadAny(vm, env));

    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}


