// Black's code at ov010 0x02166c7c-0x02166d30: pokebw2's ov012 townmap_util.c (its name a guess), which follows
// building_enter_effect.c there and the egg demo event here: TownMap_GetMapZone and TownMap_CheckFlag, Black's.
#include "types.h"
#include "constants/town_map.h"
#include "constants/zones.h"
#include "field/townmap_util.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "save/event_work.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/game_data.h"

// The zones the town map shows as another
typedef struct TownMapZoneAlias {
    u32 mapZone;
    u32 zoneId;
} TownMapZoneAlias;

static const TownMapZoneAlias data_ov010_02171f2c[2] = {
    { ZONE_POKEMON_LEAGUE, ZONE_NS_CASTLE },
    { ZONE_UNDELLA_BAY, ZONE_ABYSSAL_RUINS },
};

static inline u16 TownMap_GetAliasZone(u16 zoneId) {
    int i;

    for (i = 0; i < 2; i++) {
        if (zoneId == data_ov010_02171f2c[i].zoneId) {
            return data_ov010_02171f2c[i].mapZone;
        }
    }
    return 0xffff;
}

// The Union Room shows where the player entered it, N's Castle and the Abyssal Ruins as the places they lie at
u16 func_ov010_02166c7c(GameData *gameData, u16 zoneId) {
    u16 parent = func_02013e40(zoneId);
    u32 alias;

    if (parent == ZONE_UNION_ROOM) {
        return func_02013e40(GameData_GetNextZone(gameData)->zoneId);
    }
    alias = TownMap_GetAliasZone(parent);
    if (alias != 0xffff) {
        parent = alias;
    }
    return parent;
}

BOOL func_ov010_02166cd0(GameData *gameData, u16 flag) {
    EventWork *eventWork = GameData_GetEventWork(gameData);
    TrainerCardSave *trainerCard = getTrainerCardDataBlkAddress(gameData);
    PlayerInfo *info = GetGameDataPlayerInfo(gameData);
    UnityTowerSurveySave *survey = getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(gameData));
    BOOL result;

    switch (flag) {
    case TOWNMAP_FLAG_ONE_SHOT_DR:
        return isOneShotDRObtained(trainerCard, 1, info);
    case TOWNMAP_FLAG_UNITY_TOWER_VISITED:
        result = FALSE;
        if (func_02009590(survey, info, 0) >= 1) {
            result = TRUE;
        }
        return result;
    }
    return EventWork_FlagGet(eventWork, flag);
}
