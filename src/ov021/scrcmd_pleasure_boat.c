// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov051/scrcmd_pleasure_boat.c, the script
// plugin of Black 2/White 2's overlay 51. Black's commands are in the main command table, and its first sets up the
// cruise without a flag; the others are the same code. Names are dsd's: pokebw2's are its own.
#include "types.h"
#include "field/field.h"
#include "field/field_script.h"
#include "field/pleasure_boat.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"

// The script commands of the Royal Unova, six of entries 0x1b4 to 0x1bb of the command table FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS

// What func_ov021_021c88ec sets
enum {
    PLEASURE_BOAT_TRAINER_INFO_DEFEATED = 8,
};

// Sets up the cruise, if it isn't already
BOOL func_ov021_021c881c(VM *vm, FieldScriptEnv *env) {
    PleasureBoat **boat = func_02012ed0(
        GSYS_GetGameData(Field_GetGameSystem(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)))));

    if (*boat == NULL) {
        *boat = func_ov021_021c8608();
    }
    return FALSE;
}

BOOL func_ov021_021c8848(VM *vm, FieldScriptEnv *env) {
    func_ov010_021666a4(func_02012ed0(
        GSYS_GetGameData(Field_GetGameSystem(GSYS_GetField(FieldScriptEnv_GetGameSystem(env))))));
    return FALSE;
}

BOOL func_ov021_021c8868(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    PleasureBoat **boat = func_02012ed0(
        GSYS_GetGameData(Field_GetGameSystem(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)))));
    u16 info = VM_Read16(vm);
    u16 *var = ScriptReadVar(vm, env);

    *var = func_ov021_021c8738(*boat, info);
    return FALSE;
}

BOOL func_ov021_021c88ac(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    PleasureBoat **boat = func_02012ed0(
        GSYS_GetGameData(Field_GetGameSystem(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)))));
    u16 steps = VM_Read16(vm);
    u16 stopBefore = VM_Read16(vm);

    func_ov021_021c86e8(*boat, steps, stopBefore);
    return FALSE;
}

// Sets something about a Trainer aboard, of which only PLEASURE_BOAT_TRAINER_INFO_DEFEATED does anything
BOOL func_ov021_021c88ec(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    PleasureBoat **boat = func_02012ed0(GSYS_GetGameData(FieldScriptEnv_GetGameSystem(env)));
    u16 trainer = ScriptReadAny(vm, env);
    u16 info = VM_Read16(vm);
    VM_Read16(vm);
    u16 value = ScriptReadAny(vm, env);

    switch (info) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    case PLEASURE_BOAT_TRAINER_INFO_DEFEATED:
        func_ov021_021c87d4(*boat, trainer, value == TRUE);
        break;
    }
    return FALSE;
}

BOOL func_ov021_021c8960(VM *vm, FieldScriptEnv *env) {
    func_ov021_021c8810(*func_02012ed0(
        GSYS_GetGameData(Field_GetGameSystem(GSYS_GetField(FieldScriptEnv_GetGameSystem(env))))));
    return FALSE;
}
