// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_ochiba.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/arc.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_g3dobj.h"
#include "field/field_script.h"
#include "field/medal.h"
#include "field/scrcmd_ochiba.h"
#include "field/unity_tower.h"
#include "field/wbt.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/rtc.h"
#include "pml/poke_party.h"
#include "save/medal_box.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"
#include "gfl/rtc_cache.h"

BOOL CheckHabitatList(const HabitatList *list, PokeDexSave *pokedex, u8 time, u32 caught);
void DiscoverInitialMedalsCore(MedalBox *box, HeapID heapId);

BOOL func_ov021_021cfea0(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u16 index;
    u16 *result;
    GameData *gameData;
    UnityTowerSurveySave *survey;

    FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    index = ScriptReadAny(vm, env);
    result = ScriptReadVar(vm, env);
    gameData = GSYS_GetGameData(gsys);
    survey = getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(gameData));
    *result = func_02009590(survey, GetGameDataPlayerInfo(gameData), index);
    return FALSE;
}

BOOL s02DD_UnityTowerGetVisitorCount(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u16 *result;

    FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    result = ScriptReadVar(vm, env);
    *result = func_020095c8(getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(GSYS_GetGameData(gsys))));
    return FALSE;
}

BOOL func_ov021_021cff28(VM *vm, FieldScriptEnv *env) {
    GameData *gameData;
    UnityTowerFloor *unityTower;
    u8 visitor;
    u32 index;

    FieldScriptEnv_GetScriptWork(env);
    gameData = GSYS_GetGameData(FieldScriptEnv_GetGameSystem(env));
    unityTower = GameData_GetUnityTowerSave(gameData);
    visitor = ScriptReadAny(vm, env);
    index = func_ov021_021cfcf4(unityTower, visitor);
    if (index != 0xff) {
        func_02009698(getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(gameData)), index, 5, 1);
    }
    return FALSE;
}

BOOL func_ov021_021cff78(VM *vm, FieldScriptEnv *env) {
    GameData *gameData;
    UnityTowerFloor *unityTower;
    u16 visitor;
    u16 *result;
    u32 index;

    FieldScriptEnv_GetScriptWork(env);
    gameData = GSYS_GetGameData(FieldScriptEnv_GetGameSystem(env));
    unityTower = GameData_GetUnityTowerSave(gameData);
    visitor = ScriptReadAny(vm, env);
    result = ScriptReadVar(vm, env);
    *result = FALSE;
    index = func_ov021_021cfcf4(unityTower, visitor);
    if (index != 0xff &&
        (u8)UnityTower_GetVisitorParam(getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(gameData)), index, 5)) {
        *result = TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021cffdc(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u8 index;

    FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    index = ScriptReadAny(vm, env);
    func_020095fc(getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(GSYS_GetGameData(gsys))), index);
    return FALSE;
}

BOOL s02DE_UnityTowerSetHobby(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u8 hobby;
    UnityTowerSurveySave *save;

    FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    hobby = ScriptReadAny(vm, env);
    save = getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(GSYS_GetGameData(gsys)));
    setPlayerSurveys(save, hobby);
    return FALSE;
}

BOOL s02DF_UnityTowerGetHobby(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u16 *result;
    UnityTowerSurveySave *save;

    FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    result = ScriptReadVar(vm, env);
    save = getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(GSYS_GetGameData(gsys)));
    *result = getPlayerSurveys(save);
    return FALSE;
}

BOOL s02DB_UnityTowerSetFloor(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u16 floor;
    u16 value;

    FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    floor = ScriptReadAny(vm, env);
    value = ScriptReadAny(vm, env);
    func_ov021_021cfc4c(gsys, floor, value);
    return FALSE;
}

BOOL s02DC_UnityTowerInitVisitorMessage(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    GameSystem *gsys;
    UnityTowerFloor *save;
    WordSet *wordSet;
    u16 index;
    u16 param;
    u16 *result;

    work = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    save = GameData_GetUnityTowerSave(GSYS_GetGameData(gsys));
    wordSet = ScriptWork_GetWordSet(work);
    index = ScriptReadAny(vm, env);
    param = VM_Read16(vm);
    result = ScriptReadVar(vm, env);
    *result = func_ov021_021cfd08(wordSet, gsys, save, index, param);
    return FALSE;
}

BOOL func_ov021_021d0114(VM *vm, FieldScriptEnv *env) {
    u16 *result;
    GameSystem *gsys;

    result = ScriptReadVar(vm, env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    *result = UnityTowerVisitor_GetCountry(GetGameDataPlayerInfo(GSYS_GetGameData(gsys))) != 0;
    return FALSE;
}

