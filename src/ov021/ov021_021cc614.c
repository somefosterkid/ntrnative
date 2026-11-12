// Black's code at ov021 0x021cc614-0x021cc720. No name is known: the module and address.

#include "types.h"
#include "field/field_pass_power.h"
#include "field/field_script.h"
#include "field/game_beacon_set.h"
#include "struct_decls.h"
#include "system/game_system.h"
#include "system/pass_power.h"
#include "save/high_link.h"

u32 func_ov021_021cc614(VM *param_1, FieldScriptEnv *param_2) {
    u16 result;
    u16 value;
    GameSystem *gsys;

    result = ScriptReadAny(param_1, param_2);
    value = ScriptReadAny(param_1, param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    GSYS_GetGameData(gsys);
    GSYS_GetField(gsys);
    FieldScriptEnv_GetScriptWork(param_2);
    switch(result) {
    case 0:
        func_ov010_0216530c(value);
        break;
    case 1:
        func_ov010_02165534(value);
        break;
    case 2:
        func_ov010_021653c4();
        break;
    case 3:
        func_ov010_021656a0();
        break;
    case 4:
        func_ov010_021656d0((u8)value);
        break;
    case 5:
        func_ov010_0216573c();
        break;
    case 6:
        func_ov010_02165604();
        break;
    case 7:
        func_ov010_02165634((u32)value);
        break;
    case 8:
        func_ov010_0216566c();
    }
    return 0;
}

BOOL func_ov021_021cc6a8(VM *vm, FieldScriptEnv *env) {
    u16 *passPowerId;
    u16 *result;
    void *data;

    passPowerId = ScriptReadVar(vm, env);
    result = ScriptReadVar(vm, env);
    *passPowerId = PassPower_GetDepletedID();
    if (*passPowerId != 0x30) {
        data = PassPowerData_Create(0x8015);
        PassPower_Deactivate(*passPowerId, data);
        PassPowerData_Free(data);
        *result = (PassPower_GetDepletedID() != 0x30) ? 1 : 0;
    } else {
        *result = 0;
    }
    return FALSE;
}


u32 func_ov021_021cc6fc(u32 param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *gsys;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    event = func_ov083_021f5340(gsys);
    ScriptWork_CallEvent(work, event);
    return 1;
}
