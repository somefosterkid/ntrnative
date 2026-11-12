// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov033/event_phrase_input.c, the script
// commands at its end, which are the same code in Black 2/White 2's overlay 33. Black's file holds only these three
// Dream World commands, entries 0x218 to 0x21a of the command table FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS, without the name entry event
// before them in Black 2. pokebw2 names the file descriptively.
#include "types.h"
#include "field/field_script.h"
#include "gfl/heap.h"
#include "gfl/str.h"
#include "save/dream_world.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/vm.h"
#include "system/wordset.h"

BOOL func_ov021_021d1670(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    GameSystem *gsys;
    GameData *gameData;
    u16 input;
    u16 *result;
    DreamWorldSave *save;

    work = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    gameData = FieldScriptEnv_GetGameData(env);
    input = ScriptReadAny(vm, env);
    ScriptReadAny(vm, env);
    ScriptReadAny(vm, env);
    result = ScriptReadVar(vm, env);
    save = func_02012f8c(gameData);
    if (input == 0) {
        if (func_020092ec(save) == 1 && func_020092d8(save) == 1) {
            *result = 1;
        } else {
            *result = 0;
        }
    }
    return FALSE;
}

BOOL func_ov021_021d16d8(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    GameSystem *gsys;
    GameData *gameData;
    u16 input;
    u16 input2;
    u16 *result;
    DreamWorldSave *save;
    u8 i;
    u16 *value;

    work = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    gameData = FieldScriptEnv_GetGameData(env);
    input = ScriptReadAny(vm, env);
    input2 = ScriptReadAny(vm, env);
    ScriptReadAny(vm, env);
    result = ScriptReadVar(vm, env);
    save = func_02012f8c(gameData);
    switch (input) {
    case 0:
        *result = 1;
        for (i = 0; i < 5; i++) {
            value = func_02009390(save, i);
            if (value == NULL) {
                *result = 0;
            } else if (*value == 0x7e || *value == 0) {
                *result = 0;
            }
        }
        break;
    case 1:
        if (func_020093d8(save) == 0x7f) {
            *result = 0;
        } else {
            *result = 1;
        }
        break;
    case 2:
        if (func_02009418(save) == 1) {
            *result = 1;
        } else {
            *result = 0;
        }
        break;
    case 3:
        func_02009428(save, 0);
        break;
    case 4:
        func_020093f0(save, input2);
        break;
    }
    return FALSE;
}

BOOL func_ov021_021d179c(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    GameSystem *gsys;
    GameData *gameData;
    HeapID heapId;
    WordSet *wordSet;
    DreamWorldSave *save;
    u16 input;
    u16 index;
    u16 *phrase;
    StrBuf *strbuf;

    work = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    gameData = FieldScriptEnv_GetGameData(env);
    heapId = FieldScriptEnv_GetHeapID(env);
    wordSet = ScriptWork_GetWordSet(work);
    save = func_02012f8c(gameData);
    input = ScriptReadAny(vm, env);
    index = ScriptReadAny(vm, env);
    phrase = func_02009390(save, index);
    if (phrase != NULL && *phrase != 0x7e && *phrase != 0) {
        strbuf = GFL_StrBufCreate(14, heapId);
        GFL_StrBufLoadFixedString(strbuf, phrase + 1, 13);
        func_0201ee0c(wordSet, input, strbuf, 0, 0, 2);
        GFL_StrBufFree(strbuf);
    }
    return FALSE;
}
