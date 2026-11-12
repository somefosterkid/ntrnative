// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov033/unity_visitor.c, which is the same code
// in Black 2/White 2's overlay 33. Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0); the
// rest are Black's dsd names.
#include "types.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "system/country_region.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/union_view.h"
#include "system/wordset.h"

static void func_ov021_021cfc80(UnityTowerSurveySave *survey, UnityTowerFloor *output, s32 floor, u32 value);
static u32 func_ov021_021cfd88(u32 gender, u32 id, u32 province, u32 a3, u32 hasProvince);
static void LoadUnityTowerVisitorWordSet(WordSet *wordSet, GameData *gameData, u32 index);

void func_ov021_021cfc4c(GameSystem *gsys, s32 floor, u32 value) {
    GameData *gameData;
    UnityTowerFloor *unityTowerSave;
    SaveControl *saveControl;

    gameData = GSYS_GetGameData(gsys);
    unityTowerSave = GameData_GetUnityTowerSave(gameData);
    if (floor < 0xe9) {
        saveControl = GameData_GetSaveControl(gameData);
        func_ov021_021cfc80(getUnityTower_SurveySaveBlkAddrress(saveControl), unityTowerSave, floor, value);
    } else {
        UnityTowerSave_Init(unityTowerSave);
    }
}

static void func_ov021_021cfc80(UnityTowerSurveySave *survey, UnityTowerFloor *output, s32 floor, u32 value) {
    void *visitor;
    u32 visitorValue;
    s32 country;
    s32 index;

    output->floor = floor;
    output->value = value;
    output->count = 0;
    for (index = 0; index < 20; index++) {
        if ((u8)UnityTower_GetVisitorParam(survey, index, 6) == 0) {
            continue;
        }
        visitor = UnityTower_GetVisitor(survey, index);
        if (visitor == NULL) {
            continue;
        }
        country = UnityTowerVisitor_GetCountry(visitor);
        if (country == floor) {
            visitorValue = func_02008554(visitor);
            if (visitorValue > 15) {
                visitorValue = getTrainerGender(visitor) != 0 ? 15 : 11;
            }
            output->visitors[output->count] = index;
            output->trainerClasses[output->count] = func_02025c74(visitorValue);
            output->count++;
            if (output->count >= 5) {
                break;
            }
        }
    }
}

u32 func_ov021_021cfcf4(UnityTowerFloor *output, u32 index) {
    u32 result;

    result = 0xff;
    if (index < 5 && index < output->count) {
        result = output->visitors[index];
    }
    return result;
}

u32 func_ov021_021cfd08(WordSet *wordSet, GameSystem *gsys, UnityTowerFloor *save, s32 index, u32 param) {
    GameData *gameData;
    UnityTowerSurveySave *surveySave;
    PlayerInfo *visitor;
    u32 visitorIndex;
    u32 gender;
    u32 id;
    u32 province;
    u32 hasProvince;

    hasProvince = 0;
    gameData = GSYS_GetGameData(gsys);
    surveySave = getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(gameData));
    if (index >= 5) {
        return 4;
    }
    visitorIndex = save->visitors[index];
    LoadUnityTowerVisitorWordSet(wordSet, gameData, visitorIndex);
    visitor = UnityTower_GetVisitor(surveySave, visitorIndex);
    gender = getTrainerGender(visitor);
    province = UnityTower_GetVisitorParam(surveySave, visitorIndex, 4);
    id = getIDAsUInt(visitor);
    if (CountryHasProvinces(GameData_GetUnityTowerSave(gameData)->floor) != 0) {
        hasProvince = 1;
    }
    return func_ov021_021cfd88(gender, id, province, param, hasProvince);
}

static u32 func_ov021_021cfd88(u32 gender, u32 id, u32 province, u32 a3, u32 hasProvince) {
    u32 messageId;

    if (hasProvince != 0) {
        messageId = 4;
    } else {
        messageId = 0x44;
    }
    if (province < 5) {
        messageId += province * 12;
    }
    if (gender != 0) {
        messageId += 6;
    }
    if (a3 == 0) {
        messageId++;
        messageId += id % 5;
    }
    return messageId;
}

static void LoadUnityTowerVisitorWordSet(WordSet *wordSet, GameData *gameData, u32 index) {
    SaveControl *save;
    UnityTowerSurveySave *survey;
    UnityTowerFloor *towerSave;
    u8 country;
    void *visitor;
    u32 value;
    u8 hobby;

    save = GameData_GetSaveControl(gameData);
    survey = getUnityTower_SurveySaveBlkAddrress(save);
    towerSave = GameData_GetUnityTowerSave(gameData);
    country = towerSave->floor;
    visitor = UnityTower_GetVisitor(survey, index);
    loadCountryToStrbuf(wordSet, 0, country);
    if (CountryHasProvinces(country)) {
        loadCountryAreaToStrbuf(wordSet, 1, country, UnityTowerVisitor_GetProvince(visitor));
    }
    copyVarForText(wordSet, 3, GetGameDataPlayerInfo(gameData));
    WordSet_LoadSpeciesName(wordSet, 5, (u16)UnityTower_GetVisitorParam(survey, index, 0));
    loadHobbyNameToStrbuf(wordSet, 6, getPlayerSurveys(survey));
    copyVarForText(wordSet, 7, visitor);
    value = UnityTower_GetVisitorParam(survey, index, 3);
    if (value == 0) {
        value = 1;
    }
    WordSetNumber(wordSet, 8, value, 3, 0, 1);
    WordSet_LoadSpeciesName(wordSet, 9, (u16)UnityTower_GetVisitorParam(survey, index, 1));
    hobby = UnityTower_GetVisitorParam(survey, index, 2);
    loadHobbyNameToStrbuf(wordSet, 10, hobby);
}
