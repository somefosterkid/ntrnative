// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/game_data.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_GAME_DATA_H
#define BWPORT_SYSTEM_GAME_DATA_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

GameData *GameData_Create(HeapID heapId);
void GameData_Free(GameData *gameData);
// Lets the player into the zones of one version or the other
void SetAllowVersionSpecificZone(u32 side, u32 allow);
BagSave *GameData_GetBag(GameData *gameData);
EncountState *GameData_GetEncountState(GameData *gameData);
PlayerState *GameData_GetPlayerState(GameData *gameData);
EventWork *GameData_GetEventWork(GameData *gameData);
FieldStatus *GameData_GetFieldStatus(GameData *gameData);
BoxSaveAccessor *GameData_GetBoxSaveAccessor(GameData *gameData);
PokeDexSave *GameData_GetPokedex(GameData *gameData);
PokeParty *GameData_GetParty(GameData *gameData);
MMSys *GameData_GetMMSys(GameData *gameData);
GameRecords *GameData_GetRecords(GameData *gameData);
// pokebw2's func_0201782c and func_02017850, which start the save and step it
void func_02012dac(GameData *gameData);
int func_02012dd0(GameData *gameData);
// pokebw2's func_020179e4
DreamWorldSave *func_02012f8c(GameData *gameData);
// pokebw2's GameData_GetPleasureBoatPtr
PleasureBoat **func_02012ed0(GameData *gameData);
void SetNowWeather(GameData *gameData, u8 weather);
SaveControl *GameData_GetSaveControl(GameData *gameData);
PlayerInfo *GetGameDataPlayerInfo(GameData *gameData);
// pokebw2's GameData_GetWifiList
WifiList *func_0201283c(GameData *gameData);
GimmickState *GameData_GetGimmickState(GameData *gameData);
void func_020129d0(GameData *gameData, u8 value);
// The pass power levels, by net ID, and this player's (pokebw2's func_020171e8)
PassPowerLevel *func_020127ac(GameData *gameData, u32 netId);
// Sets the first city state, and clears it
void func_02012800(GameData *gameData, const void *src);
void func_0201281c(GameData *gameData);
PassPowerLevel *func_020127b8(GameData *gameData);
// The Black City or White Forest state, by city, and the player's own
CityState *func_020127d0(GameData *gameData, u32 netId);
CityState *func_020127c4(GameData *gameData);
// The players' info, by net ID (pokebw2's func_02017378)
PlayerInfo *func_02012950(GameData *gameData, u32 netId);
// The player's net ID (pokebw2's func_020175cc)
u32 func_02012b58(GameData *gameData);
u32 func_02012b80(GameData *gameData);
// The season of a player, by net ID, and sets it
u8 func_02012b98(GameData *gameData, u32 netId);
void func_02012ba4(GameData *gameData, u32 netId, u8 season);
// The bytes at 0x1c6 and 0x1c7 of the game data, and their setters
u8 func_020129ac(GameData *gameData);
void func_020129b8(GameData *gameData, u8 value);
// The words at 0x688 and 0x6a8 of the game data, and their setters
BSubwayScrWork *func_02012ed8(GameData *gameData);
void func_02012ee4(GameData *gameData, BSubwayScrWork *value);
MusicalCommWork *func_02012f80(GameData *gameData);
void func_02012f74(GameData *gameData, MusicalCommWork *value);
// The work GameData_Create makes with func_0202751c
void *func_02012a54(GameData *gameData);
u8 GameData_GetSeason(GameData *gameData);
BOOL GameData_IsForceSeasonSync(GameData *gameData);
// Adds seconds to the play time
void GameData_UpdateTime(GameData *gameData, u32 seconds);

u16 GameData_GetDayPeriod(GameData *gameData);
u32 GameData_GetMonth(GameData *gameData);
u32 GameData_GetDay(GameData *gameData);
u32 getCurrentHour(GameData *gameData);
u32 getCurrentMinute(GameData *gameData);

u32 getCurrentDayOfWeek(GameData *gameData);

// Black 2's, not checked against Black
// The city of the player's version, which a key from Unova Link can switch
#define CITY_BLACK_CITY 0

// Black 2's, not checked against Black
#define CITY_WHITE_FOREST 1

ZoneSpawnInfo *GameData_GetEntralinkParentSpawnInfo(GameData *gameData);
ZoneSpawnInfo *GameData_GetEscapeRopeZone(GameData *gameData);
u32 GameData_GetLastSubscreen(GameData *gameData);
void GameData_SetEntralinkParentSpawnInfo(GameData *gameData, ZoneSpawnInfo *spawn);
void GameData_SetEscapeRopeZone(GameData *gameData, ZoneSpawnInfo *spawn);
void GameData_SetLastSubscreen(GameData *gameData, u8 subscreen);
void GameData_SetLastBtlResult(GameData *gameData, u32 result);
void SetGameDataOutboundWarpRememberInfo(GameData *gameData, ZoneSpawnInfo *spawn);
void SetGameDataNowSpawnZone(GameData *gameData, ZoneSpawnInfo *spawn);
u8 GameData_Is30FPSMode(GameData *gameData);
void func_02012fa4(GameData *gameData, u8 value);
PlayerState *func_02012798(GameData *gameData, s32 index);
// pokebw2's GetNowWeather
u32 func_02012a24(GameData *gameData);
void func_02012b64(GameData *gameData, u32 value);
void func_02012b8c(GameData *gameData, u32 value);
void func_02012b78(GameData *gameData, u8 value);
// Whether the game shows other players' beacons
BOOL func_02012bb0(GameData *gameData);
void func_02012bc8(GameData *gameData, BOOL flag);
// The Battle Subway's save data
void *func_02012ef0(GameData *gameData);

// Black 2's, not checked against Black
// What GameData_MakeBoxPkm makes a Pokémon from
struct BoxPkmCreateParams {
    u16 heapId;
    u16 pad;
    u32 species;
    u32 form;
    u32 level;
    u32 item;
    s32 ability;
    s32 sex;
    s32 param1C;
    u32 ball;
};

BOOL GameData_AddBoxPkm(GameData *gameData, BoxPkmCreateParams *params);
BOOL addPkmToParty(GameData *gameData, BoxPkmCreateParams *params);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BGMInfo *GameData_GetBGMInfo(GameData *gameData);
EventData *GameData_GetEventData(GameData *gameData);
FieldSound *GameData_GetFieldSoundSystem(GameData *gameData);
void *getChatterDataAddress(GameData *gameData);
MusicalSave *getMusicalInfoBlkAddress(GameData *gameData);
ZoneSpawnInfo *GameData_GetNextZone(GameData *gameData);
void *getTimeSigSaveBlock(GameData *gameData);
// Whether the key item is registered to Y, and registers it or not
BOOL GameData_IsShortcutRegistered(GameData *gameData, u32 item);
PartyPkm *GameData_MakeBoxPkm(GameData *gameData, BoxPkmCreateParams *params);
SaveControl *GameData_GetSaveControl_(GameData *gameData);
void GameData_SetSeason(GameData *gameData, u8 season);
u32 GameData_GetLastBtlResult(GameData *gameData);
void GameData_SetNextZone(GameData *gameData, ZoneSpawnInfo *spawn);
ZoneSpawnInfo *GetGameDataNowSpawnZone(GameData *gameData);
MapMatrix *GetMapMatrixSystem(GameData *gameData);
u16 GetReturnLocationIdx(GameData *gameData);
void SetCurrentTeleportOrDeathZone(GameData *gameData, u16 respawnLocation);
Calendar *GetCalendar(GameData *gameData);
u32 GetNowWeather(GameData *gameData);
// Where the Trial House work is kept
TrialHouseWork **GetTrialHouseWkPPtr(GameData *gameData);
// Whether a full day has passed since the last check
BOOL checkForMidnight(GameData *gameData);
BOOL GameData_CheckEventsPaused(GameData *gameData);
ZoneSpawnInfo *GetOutboundWarpRememberSpawnInfo(GameData *gameData);
void *func_02012924(GameData *gameData);
void GameData_SetKeyItemRegistration(GameData *gameData, u32 item, BOOL registered);
void func_02012e4c(GameData *gameData, u32 block);
u32 func_02012e7c(GameData *gameData, u32 block);
// The save's play time
PlayTime *func_02012fcc(GameData *gameData);
// The save's play time
void func_02012e08(GameData *gameData);
// Save block 0x40
void *func_02012f68(GameData *gameData);
// The Battle Subway's save data
void *func_02012efc(GameData *gameData);
PalPadFriendList *func_02012f14(GameData *gameData);
// pokebw2's GameData_GetCommPlayerSupport
// The support another player gave over communication (system/comm_player_support.h)
CommPlayerSupport *func_02012f20(GameData *gameData);
// Save block 0x41, the Pokémon traded in game, which save/traded_pokemon.h reads
void *GetTradedPokemonBlock(GameData *gameData);
u8 func_020129c4(GameData *gameData);

void *func_02012c0c(GameData *gameData);
void *func_020127e0(GameData *gameData);

#endif // BWPORT_SYSTEM_GAME_DATA_H
