// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_item.c, ported by tools/port_file.py.
// The script commands of items: adding to and taking from the Bag, counting, and an item's pocket and kind. The ROM
// has no name for the file; scrcmd_item.c is descriptive. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "constants/items.h"
#include "field/field_script.h"
#include "field/scrcmd_item.h"
#include "pml/item.h"
#include "save/bag.h"
#include "system/game_data.h"
#include "system/vm.h"

static u16 isMoveMachine(u16 item);

BOOL s00B5_ItemAdd(VM *vm, FieldScriptEnv *env) {
    HeapID heapId = FieldScriptEnv_GetHeapID(env);
    BagSave *bag = GameData_GetBag(FieldScriptEnv_GetGameData(env));
    u16 item = ScriptReadAny(vm, env);
    u16 count = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);

    *result = BagSave_AddItem(bag, item, count, heapId);
    return FALSE;
}

BOOL s00B6_ItemSub(VM *vm, FieldScriptEnv *env) {
    HeapID heapId = FieldScriptEnv_GetHeapID(env);
    BagSave *bag = GameData_GetBag(FieldScriptEnv_GetGameData(env));
    u16 item = ScriptReadAny(vm, env);
    u16 count = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);

    *result = BagSave_SubItem(bag, item, count, heapId);
    return FALSE;
}

BOOL s00B7_ItemCheckSpace(VM *vm, FieldScriptEnv *env) {
    HeapID heapId = FieldScriptEnv_GetHeapID(env);
    BagSave *bag = GameData_GetBag(FieldScriptEnv_GetGameData(env));
    u16 item = ScriptReadAny(vm, env);
    u16 count = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);

    *result = BagSave_CheckAvailItemSpace(bag, item, count, heapId);
    return FALSE;
}

BOOL s00B8_ItemCheckAmount(VM *vm, FieldScriptEnv *env) {
    HeapID heapId = FieldScriptEnv_GetHeapID(env);
    BagSave *bag = GameData_GetBag(FieldScriptEnv_GetGameData(env));
    u16 item = ScriptReadAny(vm, env);
    u16 count = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);

    *result = BagSave_CheckAmount(bag, item, count, heapId);
    return FALSE;
}

BOOL s00B9_ItemGetCount(VM *vm, FieldScriptEnv *env) {
    HeapID heapId = FieldScriptEnv_GetHeapID(env);
    BagSave *bag = GameData_GetBag(FieldScriptEnv_GetGameData(env));
    u16 item = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);

    *result = BagSave_GetItemCountByID(bag, item, heapId);
    return FALSE;
}

BOOL s00BA_ItemIsTMHM(VM *vm, FieldScriptEnv *env) {
    u16 item = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);

    *result = isMoveMachine(item);
    return FALSE;
}

BOOL s00BB_ItemGetPocket(VM *vm, FieldScriptEnv *env) {
    HeapID heapId = FieldScriptEnv_GetHeapID(env);
    u16 item = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);

    *result = GetItemParam(item, ITEM_PARAM_FIELD_POCKET, heapId);
    return FALSE;
}

BOOL s00BD_ItemGetClass(VM *vm, FieldScriptEnv *env) {
    HeapID heapId = FieldScriptEnv_GetHeapID(env);
    u16 item = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);

    *result = GetItemParam(item, ITEM_PARAM_KIND, heapId);
    return FALSE;
}

static u16 isMoveMachine(u16 item) {
    return PML_ItemIsTMHM(item);
}
