// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/unity_tower.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_UNITY_TOWER_H
#define BWPORT_FIELD_UNITY_TOWER_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "save/player_info.h"

// The visitors on the floor of the Unity Tower the player is on, which the game data keeps
typedef struct {
    u8 floor;
    u8 count;
    u8 value;
    // The trainer class and the index of each visitor
    u8 trainerClasses[5];
    u8 visitors[5];
} UnityTowerFloor;

UnityTowerFloor *GameData_GetUnityTowerSave(GameData *gameData);

void *UnityTower_GetVisitor(UnityTowerSurveySave *save, s32 index);
u8 UnityTowerVisitor_GetProvince(PlayerInfo *playerInfo);
u8 UnityTowerVisitor_GetCountry(PlayerInfo *playerInfo);
u32 UnityTower_GetVisitorParam(UnityTowerSurveySave *save, u32 index, u32 param);
u8 getPlayerSurveys(UnityTowerSurveySave *save);
void func_ov021_021cfc4c(GameSystem *gsys, s32 floor, u32 value);
u32 func_ov021_021cfcf4(UnityTowerFloor *output, u32 index);
void UnityTowerSave_Init(UnityTowerFloor *save);
u32 func_ov021_021cfd08(WordSet *wordSet, GameSystem *gsys, UnityTowerFloor *save, s32 index, u32 param);

BOOL s01CF_UnityTowerGetStateParam(VM *vm, FieldScriptEnv *env);
BOOL s01D0_UnityTowerCallFloorSelect(VM *vm, FieldScriptEnv *env);
// Whether a visitor from the country has come
BOOL func_02009794(UnityTowerSurveySave *save, u32 country);
// How many visitors are listed
u8 func_020095c8(UnityTowerSurveySave *save);
BOOL func_ov010_02166980(VM *vm, FieldScriptEnv *env);

// A day has passed: every region recorded today becomes one recorded before
void func_0200952c(UnityTowerSurveySave *save);

// Black 2's; twinned code confirms the offsets of info, sentSpecies, receivedSpecies, hobby, countryCount, unk26_0,
// scriptFlag and valid in Black
// A trainer met through a trade, as the survey keeps them: what a trade (or the GTS) sends of its player for the
// other's records
typedef struct {
    PlayerInfo info;
    u16 sentSpecies;
    u16 receivedSpecies;
    // The trainer's hobby, which getPlayerSurveys gives
    u8 hobby;
    // The countries the trainer had met
    u8 countryCount;
    u8 unk26_0 : 3;
    // A flag of the Unity Tower's scripts, which a new record of the same trainer keeps
    u8 scriptFlag : 1;
    // Whether the entry of the survey's list is used
    u8 valid : 1;
    u8 unk26_5 : 3;
    u8 unk27;
} UnityTowerVisitor;

// The survey's visitors, the latest last, which start the survey
#define UNITY_TOWER_VISITOR_MAX 20

// Records the country met, returning whether it is a new one
BOOL func_02009750(UnityTowerSurveySave *save, u32 country);
// Counts one more country met
void func_020095e8(UnityTowerSurveySave *save);
// pokebw2's UnityTowerSurvey_RegisterTrade
// Records a trade's partner as the latest visitor, unless they have no country: 1 for a visitor met before, who
// moves to the end of the list, 2 for a new one, who takes the place of the first from the same country once five
// are listed, or of the first visitor once the list is full
u32 func_0202eb7c(UnityTowerSurveySave *save, UnityTowerVisitor *visitor);

// What UnityTower_GetVisitorParam gives of a visitor
#define UNITY_TOWER_VISITOR_PARAM_SENT_SPECIES 0

#define UNITY_TOWER_VISITOR_PARAM_RECEIVED_SPECIES 1
#define UNITY_TOWER_VISITOR_PARAM_HOBBY 2
#define UNITY_TOWER_VISITOR_PARAM_COUNTRY_COUNT 3
#define UNITY_TOWER_VISITOR_PARAM_UNK26_0 4
#define UNITY_TOWER_VISITOR_PARAM_SCRIPT_FLAG 5
#define UNITY_TOWER_VISITOR_PARAM_VALID 6

// The two bits the survey keeps per region of a country
#define UNITY_TOWER_REGION_NONE 0

#define UNITY_TOWER_REGION_TODAY 1
#define UNITY_TOWER_REGION_BEFORE 2
#define UNITY_TOWER_REGION_OWN 3

// 64 regions of each country from 1 on, two bits each
#define UNITY_TOWER_REGION_BYTES (255 * 16)

// Black 2's; matching code confirms the offsets of visitors, countriesMet, foreignRegionMet, countryCount, hobby,
// unk347 and regions, and its size in Black
// The survey's save block 0x1d: the trainers met through trades and the GTS, and the countries and regions they
// came from
struct UnityTowerSurveySave {
    UnityTowerVisitor visitors[UNITY_TOWER_VISITOR_MAX];
    // A bit per country a visitor has come from
    u8 countriesMet[32];
    u8 unk340[4];
    // Set once a region outside Japan is recorded
    u8 foreignRegionMet;
    u8 countryCount;
    // The player's own answer to the survey, below 9
    u8 hobby;
    // Below 5, sent in trades as the visitor's unk26_0
    u8 unk347;
    // UNITY_TOWER_REGION_* for each of 64 regions of each country from 1 on
    u8 regions[UNITY_TOWER_REGION_BYTES];
}; // size: 0x1338

void setPlayerSurveys(UnityTowerSurveySave *save, s32 hobby);
// The size of the survey's block and its init
u32 func_02009454(void);
void func_0200945c(UnityTowerSurveySave *save);
// Gives the player the country and region and records them as the player's own
void func_0200947c(UnityTowerSurveySave *save, PlayerInfo *info, int country, int region);
// The UNITY_TOWER_REGION_* state of a country's region, and recording one
u32 func_0200949c(UnityTowerSurveySave *save, int country, int region);
u8 func_02009524(UnityTowerSurveySave *save);
// The countries met, and without the player's own unless withOwnCountry
u8 func_02009584(UnityTowerSurveySave *save);
u8 func_02009590(UnityTowerSurveySave *save, PlayerInfo *playerInfo, BOOL withOwnCountry);
void func_020095fc(UnityTowerSurveySave *save, s32 value);
u8 func_0200960c(UnityTowerSurveySave *save);
// Sets a visitor's UNITY_TOWER_VISITOR_PARAM_UNK26_0, SCRIPT_FLAG or VALID
void func_02009698(UnityTowerSurveySave *save, int index, u32 param, u32 value);

// The two bits the survey keeps per region of a country


// Gives the player the country and region and records them as the player's own
// The UNITY_TOWER_REGION_* state of a country's region, and recording one

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
UnityTowerVisitor *func_0200974c(UnityTowerSurveySave *save);
void func_020094d8(UnityTowerSurveySave *save, int country, int region, u32 state);

#endif // BWPORT_FIELD_UNITY_TOWER_H
