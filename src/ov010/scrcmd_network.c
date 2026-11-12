// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/scrcmd_network.c, which is the same code
// in Black 2/White 2's overlay 12, except the last command: Black's doesn't call func_0201740c. Command names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), which carry Black 2's command numbers; Black's are noted.
#include "types.h"
#include "field/field.h"
#include "field/field_script.h"
#include "gfl/heap.h"
#include "system/dsi.h"
#include "system/game_comm.h"
#include "system/game_system.h"
#include "system/vm.h"

// The script commands that start and end the field's communication

BOOL func_ov010_0215f168(VM *vm, void *arg) {
    FieldScriptEnv *env = arg;
    GameSystem *gsys;
    ScriptWork *work;
    GameCommSys *commSys;
    void *data;

    gsys = FieldScriptEnv_GetGameSystem(env);
    work = FieldScriptEnv_GetScriptWork(env);
    commSys = GSYS_GetGameCommSystem(gsys);
    GSYS_GetField(gsys);
    data = *ScriptWork_GetUserHeapPtr(work);
    switch (func_ov021_02189734(commSys)) {
    case 0:
        return FALSE;
    case 1:
        GFL_HeapFree(data);
        return TRUE;
    case 2:
        **(u16 **)data = 2;
        GFL_HeapFree(data);
        return TRUE;
    }
    return FALSE;
}

// Command 0x139 in Black too
BOOL s0139_GameCommDisconnect(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    void **slot;
    u16 *result;
    void *memory;

    work = FieldScriptEnv_GetScriptWork(env);
    slot = ScriptWork_GetUserHeapPtr(work);
    result = ScriptReadVar(vm, env);
    memory = GFL_HeapAllocate(4, sizeof(u16 *), TRUE, "scrcmd_network.c", 0x63);
    *slot = memory;
    *(u16 **)memory = result;
    *result = 0;
    VM_SetNativeCallback(vm, func_ov010_0215f168);
    return TRUE;
}

// Command 0x13B in Black too
BOOL s013B_GameCommCheckDSiWiFi(VM *vm, FieldScriptEnv *env) {
    u16 *result = ScriptReadVar(vm, env);
    *result = func_0202eb44();
    return FALSE;
}

BOOL func_ov010_0215f228(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;

    gsys = FieldScriptEnv_GetGameSystem(env);
    func_020121cc(gsys);
    return TRUE;
}

BOOL func_ov010_0215f238(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    GameCommSys *commSys;

    gsys = FieldScriptEnv_GetGameSystem(env);
    commSys = GSYS_GetGameCommSystem(gsys);
    switch (GameCommSys_BootCheck(commSys)) {
    case 0:
    case 3:
    case 4:
        break;
    case 1:
    case 2:
    case 5:
        func_02026400(commSys);
        break;
    }
    func_0201213c(gsys, 1);
    FieldScriptSubEvent_Register(11);
    return TRUE;
}

BOOL func_ov010_0215f280(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;

    gsys = FieldScriptEnv_GetGameSystem(env);
    func_0201213c(gsys, 0);
    func_020121cc(gsys);
    FieldScriptSubEvent_Unregister(11);
    return TRUE;
}

// Script sub-event 11, which func_ov010_0215f238 registers
BOOL func_ov010_0215f2a0(FinishScriptSubEventsWork *work, u32 *state) {
    GameSystem *gsys;

    gsys = work->gsys;
    func_0201213c(gsys, 0);
    func_020121cc(gsys);
    return TRUE;
}

BOOL func_ov010_0215f2b8(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u16 *value;

    gsys = FieldScriptEnv_GetGameSystem(env);
    value = ScriptReadVar(vm, env);
    *value = func_0201214c(gsys);
    return FALSE;
}

// Command 0x25E in Black
BOOL s024B_FieldSubscreenDisable(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    func_02012158(gsys, 0);
    return FALSE;
}
