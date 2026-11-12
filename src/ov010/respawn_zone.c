// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/respawn_zone.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/flags.h"
#include "field/zone.h"
#include "save/event_work.h"
#include "system/game_data.h"

const RespawnZoneInfo RESPAWN_ZONE_INFO[17] = {
    { EVENT_FLAG_ARRIVED_NUVEMA_TOWN, 1, 1, 0, 390, 6, 8, 389, 782, 748, 389, 782, 748 },
    { EVENT_FLAG_ARRIVED_ACCUMULA_TOWN, 1, 1, 0, 398, 7, 12, 397, 791, 658, 397, 791, 658 },
    { EVENT_FLAG_ARRIVED_STRIATON_CITY, 1, 1, 0, 8, 7, 12, 6, 1048, 107, 6, 1048, 107 },
    { EVENT_FLAG_ARRIVED_NACRENE_CITY, 1, 0, 0, 20, 7, 12, 16, 1309, 132, 16, 1309, 132 },
    { EVENT_FLAG_ARRIVED_CASTELIA_CITY, 1, 1, 0, 41, 7, 12, 28, 1418, 235, 28, 1418, 235 },
    { EVENT_FLAG_ARRIVED_NIMBASA_CITY, 1, 0, 0, 65, 7, 12, 62, 1297, 295, 62, 1297, 295 },
    { EVENT_FLAG_ARRIVED_DRIFTVEIL_CITY, 1, 0, 0, 99, 7, 12, 96, 1231, 238, 96, 1231, 238 },
    { EVENT_FLAG_ARRIVED_MISTRALTON_CITY, 1, 1, 0, 109, 7, 12, 107, 1209, 440, 107, 1209, 440 },
    { EVENT_FLAG_ARRIVED_ICIRRUS_CITY, 1, 1, 0, 115, 7, 12, 113, 1209, 440, 113, 1209, 440 },
    { EVENT_FLAG_ARRIVED_OPELUCID_CITY, 1, 0, 0, 122, 7, 12, 120, 1209, 440, 120, 1209, 440 },
    { EVENT_FLAG_ARRIVED_LACUNOSA_TOWN, 1, 1, 0, 407, 7, 12, 406, 1032, 263, 406, 1032, 263 },
    { EVENT_FLAG_ARRIVED_POKEMON_LEAGUE, 1, 1, 0, 146, 7, 12, 136, 17, 17, 136, 17, 17 },
    { EVENT_FLAG_ARRIVED_UNDELLA_TOWN, 1, 1, 0, 413, 7, 12, 412, 100, 100, 412, 100, 100 },
    { EVENT_FLAG_ARRIVED_UNITY_TOWER, 0, 1, 0, 147, 139, 236, 147, 395, 748, 147, 395, 748 },
    { EVENT_FLAG_ARRIVED_BLACK_CITY_WHITE_FOREST, 1, 1, 0, 1, 7, 12, 0, 100, 100, 0, 100, 100 },
    { EVENT_FLAG_ARRIVED_BLACK_CITY_WHITE_FOREST, 1, 1, 0, 425, 7, 12, 424, 100, 100, 424, 100, 100 },
    { EVENT_FLAG_ARRIVED_VICTORY_ROAD, 0, 1, 0, 214, 0, 0, 214, 0, 0, 214, 0, 0 },
};

BOOL RangeCheckTeleportZone(s32 index) {
    if (index <= 0 || (u32)index > 0x11) {
        return FALSE;
    }
    return TRUE;
}

u16 GetRespawnZoneMainZone(u16 index) {
    return RESPAWN_ZONE_INFO[GetActualRespawnZoneIdx(index)].mainZoneId;
}

void SetupTeleportZoneChange(u16 index, ZoneSpawnInfo *spawn) {
    u32 actualIndex = GetActualRespawnZoneIdx(index);
    const RespawnZoneInfo *info = &RESPAWN_ZONE_INFO[actualIndex];

    CreateRespawnZoneChangeData(spawn, RESPAWN_ZONE_INFO[actualIndex].zoneId, 0, info->x, info->z);
}

u32 GetRespawnLocationIndexForRespawnZone(s32 zoneId) {
    u32 i;

    for (i = 0; i < NELEMS(RESPAWN_ZONE_INFO); i++) {
        const RespawnZoneInfo *info = &RESPAWN_ZONE_INFO[i];

        if (zoneId == info->zoneId && info->canReturnHere) {
            return i + 1;
        }
    }
    return 0;
}

void SetTeleportZoneDiscover(GameData *gameData, s32 respawnZoneId) {
    u32 i;

    for (i = 0; i < 0x11; i++) {
        const RespawnZoneInfo *info = &RESPAWN_ZONE_INFO[i];

        if (respawnZoneId == info->mainZoneId && info->discoverOnVisit) {
            EventWork_FlagSet(GameData_GetEventWork(gameData), info->discoveryFlagId);
            return;
        }
    }
}

void CreateRespawnZoneChangeData(ZoneSpawnInfo *spawn, u16 zoneId, u32 unused, u16 x, u16 z) {
    CreateZoneChangeData(spawn, zoneId, 1, x << 16, 0, z << 16);
}

u32 GetActualRespawnZoneIdx(u32 index) {
    if (!RangeCheckTeleportZone(index)) {
        index = GetLeaguePokeCenReturnLocationIdx();
    }
    return index - 1;
}
