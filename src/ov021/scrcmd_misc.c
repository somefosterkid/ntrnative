// Black's code at ov021 0x021b70d8-0x021b7400: script commands and tables of pokebw2's src/ov036/scrcmd_misc.c (its
// func_ov036_021b018c and the two after it).

#include "types.h"
#include "field/field.h"
#include "field/field_script.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/str_tool.h"
#include "save/save_control.h"
#include "save/bag.h"
#include "system/wordset.h"

static const u8 data_ov021_021d99d0[4] = { 0, 3, 2, 1 };
static const u16 data_ov021_021d99d4[5] = { 2561, 2574, 3102, 513, 1548 };
static const u16 data_ov021_021d99e8[12] = { 117, 117, 118, 133, 248, 249, 250, 251, 252, 253, 254, 0 };

typedef struct ItemCollectorPrice {
    u32 item;
    u32 price;
} ItemCollectorPrice;

u32 func_ov021_021b70d8(VM *param_1, FieldScriptEnv *param_2) {
    u16 heapID;
    u16 result;
    u16 result2;
    ScriptWork *work;
    ScriptFieldWork *fieldWork;
    ItemCollectorPrice *prices;
    u16 *ptr2;
    u32 i;
    u32 size;

    work = FieldScriptEnv_GetScriptWork(param_2);
    fieldWork = ScriptWork_GetFieldWork(work);
    heapID = Field_GetHeapID(fieldWork->field);
    result = ScriptReadAny(param_1, param_2);
    result2 = ScriptReadAny(param_1, param_2);
    ptr2 = ScriptReadVar(param_1, param_2);
    prices = GFL_ArcSysReadHeapNewLZGetLen(0xd0, data_ov021_021d99d0[result2], 0, (heapID & 0x7fff) | 0x8000, &size);
    i = 0;
    size /= sizeof(ItemCollectorPrice);
    *ptr2 = 0;
    while (i < size) {
        if ((u32)result == prices[i].item) {
            *ptr2 = (u16)prices[i].price;
            break;
        }
        i = i + 1;
    }
    GFL_HeapFree(prices);
    return 0;
}

u32 func_ov021_021b716c(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    HeapID heapId = Field_GetHeapID(fieldWork->field);
    u16 group = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);
    BagSave *bag = GameData_GetBag(gameData);
    u32 size;
    ItemCollectorPrice *prices;
    u32 i;

    prices = GFL_ArcSysReadHeapNewLZGetLen(0xd0, data_ov021_021d99d0[group], FALSE, HEAPID_TAIL(heapId), &size);
    size /= sizeof(ItemCollectorPrice);
    *result = FALSE;
    for (i = 0; i < size; i++) {
        if (BagSave_CheckAmount(bag, prices[i].item, 1, heapId)) {
            *result = TRUE;
            break;
        }
    }
    GFL_HeapFree(prices);
    return 0;
}

u32 func_ov021_021b721c(VM *vm, FieldScriptEnv *env) {
    u32 size;
    ItemCollectorPrice *prices;
    u32 i;
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    HeapID heapId = Field_GetHeapID(fieldWork->field);
    u16 item = ScriptReadAny(vm, env);
    u16 group = ScriptReadAny(vm, env);
    u16 wordIndex = ScriptReadAny(vm, env);
    u16 digits = ScriptReadAny(vm, env);
    WordSet *wordSet = ScriptWork_GetWordSet(work);

    prices = GFL_ArcSysReadHeapNewLZGetLen(0xd0, data_ov021_021d99d0[group], FALSE, HEAPID_TAIL(heapId), &size);
    size /= sizeof(ItemCollectorPrice);
    for (i = 0; i < size; i++) {
        if (item == prices[i].item) {
            WordSetNumber(wordSet, wordIndex, prices[i].price, digits, NUM_PAD_NONE, TRUE);
            break;
        }
    }
    GFL_HeapFree(prices);
    return 0;
}



u32 func_ov021_021b72cc(VM *vm, FieldScriptEnv *env) {
    u32 size;
    ItemCollectorPrice *prices;
    u32 i;
    GameData *gameData;
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));
    HeapID heapId = Field_GetHeapID(fieldWork->field);
    u16 item = ScriptReadAny(vm, env);
    u16 group = ScriptReadAny(vm, env);

    gameData = FieldScriptEnv_GetGameData(env);
    GameData_GetPlayerState(gameData);
    prices = GFL_ArcSysReadHeapNewLZGetLen(0xd0, data_ov021_021d99d0[group], FALSE, HEAPID_TAIL(heapId), &size);
    size /= sizeof(ItemCollectorPrice);
    for (i = 0; i < size; i++) {
        if (item == prices[i].item) {
            addCashToTotal(getTrainerCardDataBlkAddress(gameData), prices[i].price);
            break;
        }
    }
    GFL_HeapFree(prices);
    return 0;
}


u32 func_ov021_021b7364(VM *param_1, FieldScriptEnv *param_2) {
    u16 *ptr;
    u32 result;

    result = (u32)ScriptReadAny(param_1, param_2);
    ptr = ScriptReadVar(param_1, param_2);
    if (result >= 0xb) {
        result = 0;
    }
    *ptr = data_ov021_021d99e8[result];
    return 0;
}

u32 func_ov021_021b7390(u32 param_1, u32 param_2) {
    u32 i;
    u16 val;

    val = (u16)((param_1 << 8) | param_2);
    i = 0;
    do {
        if (val == data_ov021_021d99d4[i]) {
            return 1;
        }
        i = i + 1;
    } while (i < 5);
    return 0;
}

static const u8 data_ov021_021d99de[10] = { 9, 1, 3, 5, 4, 2, 6, 8, 0, 7 };
u32 func_ov021_021b73b8(VM *vm, FieldScriptEnv *env) {
    u16 *result = ScriptReadVar(vm, env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    u32 month = GameData_GetMonth(gameData);
    s32 day = GameData_GetDay(gameData);
    u16 value;

    if (func_ov021_021b7390(month, day) == 1) {
        value = 10;
    } else {
        value = data_ov021_021d99de[day % 10];
    }
    *result = value;
    return 0;
}

