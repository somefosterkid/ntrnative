// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/game_data.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "system/game_data.h"
#include "types.h"
#include "constants/directions.h"
#include "field/calender.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/field_actor.h"
#include "field/field_map.h"
#include "field/field_sound.h"
#include "field/field_status.h"
#include "field/musical.h"
#include "field/pair_sys.h"
#include "field/player_state.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/ui.h"
#include "save/bag.h"
#include "save/box.h"
#include "save/bsubway_save.h"
#include "save/chatter.h"
#include "save/config.h"
#include "save/field_actor_savedata.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/save_control.h"
#include "save/shortcut.h"
#include "save/timesig_savedata.h"
#include "save/trainer_data_savedata.h"
#include "save/wifi_list.h"
#include "system/comm_player_support.h"
#include "system/game_beacon.h"
#include "system/season.h"
#include "system/shortcut_util.h"
#include "system/wih_dwc.h"
#include "constants/pokemon.h"
#include "field/city_state.h"
#include "save/records.h"
#include "save/musical_savedata.h"

// The game's data while it runs: the save control, the works the field and its applications share, and the copies
// of the save's blocks that the report writes back. Names from swan (https://github.com/ds-pokemon-hacking/swan,
// GPL-3.0) where the functions have them; the fields' names are ours

#define SAVE_BLOCK_BAG 0x19
#define SAVE_BLOCK_PARTY 0x1a
#define SAVE_BLOCK_RECORDS 0x26
#define SAVE_BLOCK_FIELD_ACTORS 0x29
#define SAVE_BLOCK_MUSICAL 0x2a
#define SAVE_BLOCK_EVENT_WORK 0x2d
#define SAVE_BLOCK_STRENGTH_ROCKS 0x33
#define SAVE_BLOCK_40 0x40
#define SAVE_BLOCK_TRADED_POKEMON 0x43

// The players of the game data: the four of a communication, by net ID, then the player's own
#define GAME_DATA_PLAYER_MAX 5
#define GAME_DATA_PLAYER_MINE 4
// The pass power levels are kept for the four net IDs, and the cities for the other player and the player's own
#define GAME_DATA_LEVELS_MAX 4
#define GAME_DATA_LEVELS_MINE 3
#define GAME_DATA_CITY_MAX 2
#define GAME_DATA_CITY_MINE 1

#define GAME_DATA_PICK_MAX 3
#define GAME_DATA_PICK_TRIES 100
#define GAME_DATA_COUNTER_MAX 99

// The respawn locations of Black City and White Forest
#define RETURN_LOCATION_BLACK_CITY 15
#define RETURN_LOCATION_WHITE_FOREST 16

#define SAVE_STATUS_NEW 1
// What the stepwise save returns once it ends
#define SAVE_RESULT_OK 2
#define SAVE_RESULT_NG 3

#define SLEEP_BLOCK_SAVE 0x80
#define SOFT_RESET_BLOCK_SAVE 0x2

struct GameData {
    SaveControl *saveControl;
    PlayerState playerStates[GAME_DATA_PLAYER_MAX];
    EventData *eventData;
    PlayerSave *playerSave;
    ZoneSpawnInfo *nowSpawn;
    ZoneSpawnInfo *outboundWarpSpawn;
    ZoneSpawnInfo *nextSpawn;
    ZoneSpawnInfo *escapeRopeSpawn;
    ZoneSpawnInfo entralinkParentSpawn;
    // The battle bag's cursor memory (save/bag.h)
    void *bagCursor;
    BagSave *bag;
    PokeParty *party;
    Config *config;
    BoxSaveAccessor *boxes;
    WifiList *wifiList;
    BGMInfo *bgmInfo;
    MMSys *mmSys;
    EventWork *eventWork;
    MapMatrix *mapMatrix;
    FieldSound *fieldSound;
    FieldStatus *fieldStatus;
    u32 lastBtlResult;
    u32 unk1C0;
    u8 season;
    u8 weather;
    u8 unk1C6;
    u8 unk1C7;
    // Counted up by func_020129fc while counterEnabled is set
    u8 counter;
    u8 counterEnabled;
    u8 lastSubscreen;
    // The player's own net ID
    u8 netId;
    u8 forceSeasonSync;
    u8 savedCGearPowerRequest;
    // Set while the save is being written
    u8 saving;
    u8 is30FPSMode;
    // The season of each net ID
    u8 seasons[GAME_DATA_LEVELS_MAX];
    PassPowerLevel levels[GAME_DATA_LEVELS_MAX];
    CityState cityStates[GAME_DATA_CITY_MAX];
    EncountState *encountState;
    ShortcutMenuPos shortcutMenuPos;
    GimmickState gimmickState;
    PleasureBoat *pleasureBoat;
    CommPlayerSupport *commPlayerSupport;
    BSubwayScrWork *unk688;
    void *unk68C;
    Calendar *calendar;
    UnityTowerFloor unityTowerFloor;
    TrialHouseWork *trialHouseWork;
    MusicalCommWork *unk6A8;
    PlayTime *playTime;
}; // size: 0x6b0

void func_02012a60(PlayerState *playerState);
static void GameData_LoadBackup(GameData *gameData);
static void GameData_SaveBackup(GameData *gameData);
void *func_0200a890(void *save);
void *func_0200a898(void *save);
BOOL func_0200c44c(ShortcutSave *shortcutSave, u32 shortcut);
ShortcutSave *func_0200c40c(SaveControl *save);
void func_0200c418(ShortcutSave *shortcutSave, u32 shortcut, BOOL registered);

// The player's rotation facing each direction
static const u16 DIRECTION_TO_3D_ANGLE[] = {
    0x0000, // DIR_UP
    0x8000, // DIR_DOWN
    0x4000, // DIR_LEFT
    0xc000, // DIR_RIGHT
};

void func_02012ce0(GameData *gameData);
void func_02012c18(GameData *gameData);
EventData *EventData_Create(HeapID heapId);
MapMatrix *InitMapMatrix(HeapID heapId);
MMSys *FldActSys_Create(HeapID heapId, u32 capacity, void *save);
BoxSaveAccessor *BoxSaveAccessor_Create(HeapID heapId, SaveControl *save);
void func_0202e43c(ShortcutMenuPos *shortcutMenuPos);
void func_02027584(BeaconStatus *status);
void FldActSys_Free(MMSys *system);
void FreeMapMatrix(void *ptr);
void func_0200a884(void *save, void *value);
void func_0200e89c(SaveControl *save, UnityTowerFloor *unityTowerFloor);
void func_0200a870(void *param_1, void *param_2);
void func_0200e884(UnityTowerFloor *unityTowerFloor, SaveControl *save);
u32 func_020073b0(SaveControl *save, u32 extra_id);
void func_02007374(SaveControl *save, u32 block);
void func_0201042c(void);
u32 func_02007168(SaveControl *save);



GameData *GameData_Create(HeapID heapId) {
    GameData *gameData = GFL_HeapAllocate(heapId, sizeof(GameData), TRUE, "game_data.c", 0xad);
    int i;
    u32 size;
    PlayerInfo *playerInfo;

    gameData->saveControl = SaveControl_GetInstance();
    gameData->season = Season_GetRealTime();
    gameData->unk1C6 = 0;

    gameData->playerSave = SaveControl_GetPlayerSave(gameData->saveControl);
    gameData->nowSpawn = PlayerSave_GetNowSpawnZone(gameData->playerSave);
    gameData->outboundWarpSpawn = PlayerSave_GetNowSpawnZone(gameData->playerSave);
    gameData->nextSpawn = PlayerSave_GetNextSpawnZone(gameData->playerSave);
    gameData->escapeRopeSpawn = PlayerSave_GetEscapeRopeZone(gameData->playerSave);

    gameData->bgmInfo = BGMInfo_Create(heapId);

    for (i = 0; i < GAME_DATA_PLAYER_MAX; i++) {
        func_02012a60(&gameData->playerStates[i]);
    }
    size = func_02008468();
    playerInfo = GetGameDataPlayerInfo(gameData);
    MI_CpuCopy8(func_02008730(gameData->saveControl), playerInfo, size);

    gameData->eventData = EventData_Create(heapId);
    EventData_Reset(gameData->eventData);
    gameData->eventWork = getConstDataBlock(gameData->saveControl);
    gameData->mapMatrix = InitMapMatrix(heapId);
    gameData->mmSys = FldActSys_Create(heapId, 0x40, SaveControl_GetBlockPtr(gameData->saveControl, 0x33));
    gameData->fieldSound = FieldSnd_Create(gameData, heapId);
    gameData->fieldStatus = FieldStatus_Create(heapId);

    gameData->bagCursor = func_02008254(heapId);
    gameData->bag = SaveControl_GetBlockPtr(gameData->saveControl, SAVE_BLOCK_BAG);
    gameData->party = SaveControl_GetBlockPtr(gameData->saveControl, SAVE_BLOCK_PARTY);
    gameData->config = &getTrainerDataBlkAddress(gameData->saveControl)->config;
    gameData->boxes = BoxSaveAccessor_Create(heapId, gameData->saveControl);
    gameData->wifiList = populateWifiList(heapId);

    for (i = 0; i < GAME_DATA_LEVELS_MAX; i++) {
        setupProbabilityTable(&gameData->levels[i]);
    }
    for (i = 0; i < GAME_DATA_CITY_MAX; i++) {
        func_ov010_02161d98(&gameData->cityStates[i]);
    }

    gameData->playTime = setupPlaytimeCounters(heapId);
    gameData->unk1C0 = 0;
    SaveControl_GetStatus(gameData->saveControl);
    gameData->encountState = EncountState_Create(heapId);
    func_0202e43c(&gameData->shortcutMenuPos);
    func_02012c18(gameData);

    gameData->pleasureBoat = NULL;
    gameData->unk688 = 0;
    gameData->commPlayerSupport = func_0202e770(heapId);
    gameData->unk68C = func_0202751c(heapId, 0x8001);
    gameData->calendar = Calendar_Create(gameData, heapId);
    gameData->trialHouseWork = NULL;
    gameData->unk6A8 = 0;
    return gameData;
}

void GameData_Free(GameData *gameData) {
    GFL_HeapFree(gameData->playTime);
    Calendar_Free(gameData->calendar);
    func_02027584(gameData->unk68C);
    func_0202e794(gameData->commPlayerSupport);
    EncountState_Free(gameData->encountState);
    BGMInfo_Free(gameData->bgmInfo);
    GFL_HeapFree(gameData->wifiList);
    BoxSaveAccessor_Free(gameData->boxes);
    GFL_HeapFree(gameData->bagCursor);
    FldActSys_Free(gameData->mmSys);
    FreeMapMatrix(gameData->mapMatrix);
    EventData_Free(gameData->eventData);
    FieldStatus_Free(gameData->fieldStatus);
    FieldSnd_Free(gameData->fieldSound);
    GFL_HeapFree(gameData);
}

PlayerState *func_02012798(GameData *gameData, s32 index) {
    return &gameData->playerStates[index];
}

PlayerState *GameData_GetPlayerState(GameData *gameData) {
    return &gameData->playerStates[GAME_DATA_PLAYER_MINE];
}

PassPowerLevel *func_020127ac(GameData *gameData, u32 netId) {
    return &gameData->levels[netId];
}

PassPowerLevel *func_020127b8(GameData *gameData) {
    return func_020127ac(gameData, GAME_DATA_LEVELS_MINE);
}

CityState *func_020127c4(GameData *gameData) {
    return func_020127d0(gameData, GAME_DATA_CITY_MINE);
}

CityState *func_020127d0(GameData *gameData, u32 netId) {
    return &gameData->cityStates[netId];
}

void *func_020127e0(GameData *gameData) {
    return func_0200a890(func_0200a864(gameData->saveControl));
}

void *func_020127f0(GameData *gameData) {
    return func_0200a898(func_0200a864(gameData->saveControl));
}

void func_02012800(GameData *gameData, const void *src) {
    MI_CpuCopy8(src, &gameData->cityStates[0], 0x1e8);
    func_ov010_02161d64(&gameData->cityStates[0]);
}

void func_0201281c(GameData *gameData) {
    MI_CpuFill8(&gameData->cityStates[0], 0, 0x1e8);
    func_ov010_02161d64(&gameData->cityStates[1]);
}

// pokebw2's GameData_GetWifiList
WifiList *func_0201283c(GameData *gameData) {
    return gameData->wifiList;
}

EventData *GameData_GetEventData(GameData *gameData) {
    return gameData->eventData;
}

ZoneSpawnInfo *GetGameDataNowSpawnZone(GameData *gameData) {
    return gameData->nowSpawn;
}

void SetGameDataNowSpawnZone(GameData *gameData, ZoneSpawnInfo *spawn) {
    *gameData->nowSpawn = *spawn;
}

ZoneSpawnInfo *GetOutboundWarpRememberSpawnInfo(GameData *gameData) {
    return gameData->outboundWarpSpawn;
}

void SetGameDataOutboundWarpRememberInfo(GameData *gameData, ZoneSpawnInfo *spawn) {
    *gameData->outboundWarpSpawn = *spawn;
}

ZoneSpawnInfo *GameData_GetNextZone(GameData *gameData) {
    return gameData->nextSpawn;
}

void GameData_SetNextZone(GameData *gameData, ZoneSpawnInfo *spawn) {
    *gameData->nextSpawn = *spawn;
}

ZoneSpawnInfo *GameData_GetEscapeRopeZone(GameData *gameData) {
    return gameData->escapeRopeSpawn;
}

void GameData_SetEscapeRopeZone(GameData *gameData, ZoneSpawnInfo *spawn) {
    *gameData->escapeRopeSpawn = *spawn;
}

ZoneSpawnInfo *GameData_GetEntralinkParentSpawnInfo(GameData *gameData) {
    return &gameData->entralinkParentSpawn;
}

void GameData_SetEntralinkParentSpawnInfo(GameData *gameData, ZoneSpawnInfo *spawn) {
    gameData->entralinkParentSpawn = *spawn;
}

void SetCurrentTeleportOrDeathZone(GameData *gameData, u16 respawnLocation) {
    *PlayerSave_GetReturnZoneIDPtr(gameData->playerSave) = respawnLocation;
}

u16 GetReturnLocationIdx(GameData *gameData) {
    return *PlayerSave_GetReturnZoneIDPtr(gameData->playerSave);
}

void *func_02012924(GameData *gameData) {
    return gameData->bagCursor;
}

BagSave *GameData_GetBag(GameData *gameData) {
    return gameData->bag;
}

PokeParty *GameData_GetParty(GameData *gameData) {
    return gameData->party;
}

BoxSaveAccessor *GameData_GetBoxSaveAccessor(GameData *gameData) {
    return gameData->boxes;
}

PlayerInfo *GetGameDataPlayerInfo(GameData *gameData) {
    return &GameData_GetPlayerState(gameData)->playerInfo;
}

PlayerInfo *func_02012950(GameData *gameData, u32 netId) {
    return &gameData->playerStates[netId].playerInfo;
}

BGMInfo *GameData_GetBGMInfo(GameData *gameData) {
    return gameData->bgmInfo;
}

MMSys *GameData_GetMMSys(GameData *gameData) {
    return gameData->mmSys;
}

EventWork *GameData_GetEventWork(GameData *gameData) {
    return gameData->eventWork;
}

MapMatrix *GetMapMatrixSystem(GameData *gameData) {
    return gameData->mapMatrix;
}

Calendar *GetCalendar(GameData *gameData) {
    return gameData->calendar;
}

u8 GameData_GetSeason(GameData *gameData) {
    return gameData->season;
}

void GameData_SetSeason(GameData *gameData, u8 season) {
    gameData->season = season;
}

u32 GetNowWeather(GameData *gameData) {
    return gameData->weather;
}

void SetNowWeather(GameData *gameData, u8 weather) {
    gameData->weather = weather;
}

u8 func_020129ac(GameData *gameData) {
    return gameData->unk1C6;
}

void func_020129b8(GameData *gameData, u8 value) {
    gameData->unk1C6 = value;
}

u8 func_020129c4(GameData *gameData) {
    return gameData->unk1C7;
}

void func_020129d0(GameData *gameData, u8 value) {
    gameData->unk1C7 = value;
}

void func_020129dc(GameData *gameData, u8 enabled) {
    gameData->counterEnabled = enabled;
    if (enabled != 0) {
        gameData->counter = 0;
    }
}

void func_020129f0(GameData *gameData) {
    gameData->counter = 0;
}

u8 func_020129fc(GameData *gameData) {
    if (gameData->counterEnabled != 0) {
        u8 ret = gameData->counter;
        gameData->counter++;
        return ret;
    }
    return 0;
}

u32 func_02012a24(GameData *gameData) {
    return gameData->counterEnabled;
}

FieldSound *GameData_GetFieldSoundSystem(GameData *gameData) {
    return gameData->fieldSound;
}

EncountState *GameData_GetEncountState(GameData *gameData) {
    return gameData->encountState;
}

u32 GameData_GetLastBtlResult(GameData *gameData) {
    return gameData->lastBtlResult;
}

void GameData_SetLastBtlResult(GameData *gameData, u32 result) {
    gameData->lastBtlResult = result;
}

void *func_02012a54(GameData *gameData) {
    return gameData->unk68C;
}

void func_02012a60(PlayerState *playerState) {
    playerState->playerInfo.gender = 0;
    playerState->playerInfo.region = 0;
    playerState->zoneId = 0;
    playerState->position.x = 0;
    playerState->position.y = 0;
    playerState->position.z = 0;
    playerState->unk18 = 0;
    playerState->unk1B = FALSE;
    playerState->playerInfo.id = 0;
    ((RailPosition *)&playerState->unk10)->componentIsLine = 0;
    ((RailPosition *)&playerState->unk10)->railDirection = 0;
    ((RailPosition *)&playerState->unk10)->componentId = 0;
    ((RailPosition *)&playerState->unk10)->posFront = 0;
    ((RailPosition *)&playerState->unk10)->posSide = 0;
    playerState->objCode = 0xffff;
}

void PlayerState_SetWPos(PlayerState *playerState, VecFx32 *pos) {
    playerState->position = *pos;
}

VecFx32 *PlayerState_GetWPos(PlayerState *playerState) {
    return &playerState->position;
}

void PlayerState_SetRailPos(PlayerState *playerState, const RailPosition *pos) {
    *(RailPosition *)&playerState->unk10 = *pos;
}

RailPosition *PlayerState_GetRailPos(PlayerState *playerState) {
    return (RailPosition *)&playerState->unk10;
}

void PlayerState_SetIsRail(PlayerState *playerState, BOOL isRail) {
    playerState->unk1B = isRail;
}

u8 func_02012ac0(PlayerState *playerState) {
    return playerState->unk1B;
}

void PlayerState_SetZoneID(PlayerState *playerState, u16 zoneId) {
    playerState->zoneId = zoneId;
}

u16 PlayerState_GetZoneID(PlayerState *playerState) {
    return playerState->zoneId;
}

void PlayerState_SetRotation(PlayerState *playerState, u16 angle) {
    playerState->unk18 = angle;
}

u16 func_02012ad0(PlayerState *playerState) {
    return playerState->unk18;
}

void PlayerState_SetRotationByDir(PlayerState *playerState, u16 dir) {
    playerState->unk18 = DIRECTION_TO_3D_ANGLE[dir];
}

u16 PlayerState_CalcDirection(PlayerState *playerState) {
    u16 dir = DIR_UP;
    u16 angle = playerState->unk18;

    if (angle > 0x2000 && angle < 0x6000) {
        dir = DIR_LEFT;
    } else if (angle >= 0x6000 && angle <= 0xa000) {
        dir = DIR_DOWN;
    } else if (angle > 0xa000 && angle < 0xe000) {
        dir = DIR_RIGHT;
    }
    return dir;
}

u32 FieldPlayerState_GetExState(PlayerState *playerState) {
    return playerState->exState;
}

void SetPlayerSpecialState(PlayerState *playerState, u32 state) {
    playerState->exState = state;
}

void FieldPlayerState_SetObjCode(PlayerState *playerState, u16 objCode) {
    playerState->objCode = objCode;
}

u16 FieldPlayerState_GetObjCode(PlayerState *playerState) {
    return playerState->objCode;
}

FieldStatus *GameData_GetFieldStatus(GameData *gameData) {
    return gameData->fieldStatus;
}

u32 GameData_GetLastSubscreen(GameData *gameData) {
    return gameData->lastSubscreen;
}

void GameData_SetLastSubscreen(GameData *gameData, u8 subscreen) {
    gameData->lastSubscreen = subscreen;
}

u32 func_02012b58(GameData *gameData) {
    return gameData->netId;
}

void func_02012b64(GameData *gameData, u32 value) {
    gameData->netId = value;
}

BOOL GameData_IsForceSeasonSync(GameData *gameData) {
    return gameData->forceSeasonSync;
}

void func_02012b78(GameData *gameData, u8 value) {
    gameData->forceSeasonSync = value;
}

u32 func_02012b80(GameData *gameData) {
    return gameData->savedCGearPowerRequest;
}

void func_02012b8c(GameData *gameData, u32 value) {
    gameData->savedCGearPowerRequest = value;
}

u8 func_02012b98(GameData *gameData, u32 netId) {
    return gameData->seasons[netId];
}

void func_02012ba4(GameData *gameData, u32 netId, u8 season) {
    gameData->seasons[netId] = season;
}

BOOL func_02012bb0(GameData *gameData) {
    if (func_02008448(gameData->config) == 1) {
        return TRUE;
    }
    return FALSE;
}

void func_02012bc8(GameData *gameData, BOOL flag) {
    func_02008450(gameData->config, flag ? 1 : 0);
}

void GameData_SetKeyItemRegistration(GameData *gameData, u32 shortcut, BOOL registered) {
    func_0200c418(func_0200c40c(GameData_GetSaveControl(gameData)), shortcut, registered);
}

BOOL GameData_IsShortcutRegistered(GameData *gameData, u32 shortcut) {
    return func_0200c44c(SaveControl_GetShortcutSave(GameData_GetSaveControl_(gameData)), shortcut);
}

void *func_02012c0c(GameData *gameData) {
    return &gameData->shortcutMenuPos;
}

void func_02012c18(GameData *gameData) {
    if (SaveControl_GetStatus(gameData->saveControl) != 1) {
        PlayerState *playerState = GameData_GetPlayerState(gameData);
        func_02008808(gameData->saveControl, playerState);
        SaveControl_LoadPlayerState(gameData->saveControl, playerState);
        MMSys *mmSys = GameData_GetMMSys(gameData);
        FieldActorSave *actors = SaveControl_GetBlockPtr(gameData->saveControl, SAVE_BLOCK_FIELD_ACTORS);
        ActorSave_Load(mmSys, actors);
        PassPowerLevel *levels = func_020127b8(gameData);
        func_0200be6c(gameData->saveControl, levels);
        CityState *value = func_020127c4(gameData);
        void *musical = func_0200a864(gameData->saveControl);
        func_0200a884(musical, value);
        GimmickState_LoadSave(gameData->saveControl, &gameData->gimmickState);
        PlayerSave *playerSave = gameData->playerSave;
        PlayerSave_LoadFieldStatus(playerSave, gameData->fieldStatus);
        PlayerSave_LoadSeason(playerSave, &gameData->season);
        WifiList *dest = func_0201283c(gameData);
        WifiList *src = func_02009d08(gameData->saveControl);
        copyWifilist(src, dest);
        PlayerSave_LoadWeather(gameData->playerSave, &gameData->weather);
        func_0200e89c(gameData->saveControl, &gameData->unityTowerFloor);
        PlayTime *src2 = func_02008748(gameData->saveControl);
        func_0200863c(gameData->playTime, src2);
    }
}

void func_02012ce0(GameData *gameData) {
    PlayerState *playerState = GameData_GetPlayerState(gameData);
    func_020087e8(gameData->saveControl, playerState);
    SaveControl_SavePlayerState(gameData->saveControl, playerState);
    MMSys *mmSys = GameData_GetMMSys(gameData);
    FieldActorSave *actors = SaveControl_GetBlockPtr(gameData->saveControl, SAVE_BLOCK_FIELD_ACTORS);
    ActorSave_Save(mmSys, actors);
    PassPowerLevel *levels = func_020127b8(gameData);
    func_0200be54(gameData->saveControl, levels);
    CityState *value = func_020127c4(gameData);
    void *musical = func_0200a864(gameData->saveControl);
    func_0200a870(musical, value);
    GimmickState_StoreSave(&gameData->gimmickState, gameData->saveControl);
    PlayerSave *playerSave = gameData->playerSave;
    PlayerSave_SaveFieldStatus(playerSave, gameData->fieldStatus);
    PlayerSave_SaveSeason(playerSave, gameData->season);
    WifiList *src = func_0201283c(gameData);
    WifiList *dest = func_02009d08(gameData->saveControl);
    copyWifilist(src, dest);
    PlayerSave_SaveWeather(gameData->playerSave, gameData->weather);
    func_0200e884(&gameData->unityTowerFloor, gameData->saveControl);
    PlayTime *time = func_02008748(gameData->saveControl);
    func_02008658(gameData->playTime);
    func_0200863c(time, gameData->playTime);
}

void func_02012dac(GameData *gameData) {
    func_02012ce0(gameData);
    func_0201042c();
    gameData->saving = 1;
    func_0203516c(2);
    func_02007150(gameData->saveControl);
}

int func_02012dd0(GameData *gameData) {
    s32 state;

    if (func_02005c64() == 1) {
        return 0;
    }
    state = func_02007168(gameData->saveControl);
    if (state == 2 || state == 3) {
        func_020351a0(2);
        func_02010444();
        gameData->saving = 0;
    }
    return state;
}

void func_02012e08(GameData *gameData) {
    if (gameData->saving == 1) {
        func_020351a0(2);
        func_02010444();
        gameData->saving = 0;
        func_020071a8(gameData->saveControl);
        if (func_02034fe8(0x80) == 1) {
            func_02034fb8(0x80);
        }
    }
}

BOOL GameData_CheckEventsPaused(GameData *gameData) {
    return gameData->saving;
}

void func_02012e4c(GameData *gameData, u32 block) {
    func_02034f84(0x80);
    func_02012ce0(gameData);
    func_0201042c();
    gameData->saving = 1;
    func_0203516c(2);
    func_02007374(gameData->saveControl, block);
}

u32 func_02012e7c(GameData *gameData, u32 block) {
    u32 state;

    if (func_02005c64() == 1) {
        return 0;
    }
    state = func_020073b0(gameData->saveControl, block);
    if (state == 2 || state == 3) {
        func_020351a0(2);
        func_02010444();
        gameData->saving = 0;
        func_02034fb8(0x80);
    }
    return state;
}

SaveControl *GameData_GetSaveControl(GameData *gameData) {
    return gameData->saveControl;
}

SaveControl *GameData_GetSaveControl_(GameData *gameData) {
    return gameData->saveControl;
}

GimmickState *GameData_GetGimmickState(GameData *gameData) {
    return &gameData->gimmickState;
}

PleasureBoat **func_02012ed0(GameData *gameData) {
    return &gameData->pleasureBoat;
}

BSubwayScrWork *func_02012ed8(GameData *gameData) {
    return gameData->unk688;
}

void func_02012ee4(GameData *gameData, BSubwayScrWork *value) {
    gameData->unk688 = value;
}

void *func_02012ef0(GameData *gameData) {
    return SaveControl_GetBlockPtr(gameData->saveControl, SAVE_BLOCK_BSUBWAY_3A);
}

void *func_02012efc(GameData *gameData) {
    return SaveControl_GetBlockPtr(gameData->saveControl, 0x3b);
}

TrainerCardSave *getTrainerCardDataBlkAddress(GameData *gameData) {
    return getTrainerGameInfoAddress(gameData->saveControl);
}

PalPadFriendList *func_02012f14(GameData *gameData) {
    return getPalPadFriendListAddress(gameData->saveControl);
}

CommPlayerSupport *func_02012f20(GameData *gameData) {
    return gameData->commPlayerSupport;
}

GameRecords *GameData_GetRecords(GameData *gameData) {
    return SaveControl_GetBlockPtr(gameData->saveControl, SAVE_BLOCK_RECORDS);
}

UnityTowerFloor *GameData_GetUnityTowerSave(GameData *gameData) {
    return &gameData->unityTowerFloor;
}

MusicalSave *getMusicalInfoBlkAddress(GameData *gameData) {
    return SaveControl_GetBlockPtr(gameData->saveControl, SAVE_BLOCK_MUSICAL);
}

TrialHouseWork **GetTrialHouseWkPPtr(GameData *gameData) {
    return &gameData->trialHouseWork;
}

void *getTimeSigSaveBlock(GameData *gameData) {
    return getTimeSigBlkAddress(gameData->saveControl);
}

void *func_02012f68(GameData *gameData) {
    return SaveControl_GetBlockPtr(gameData->saveControl, 0x42);
}

void func_02012f74(GameData *gameData, MusicalCommWork *value) {
    gameData->unk6A8 = value;
}

MusicalCommWork *func_02012f80(GameData *gameData) {
    return gameData->unk6A8;
}

DreamWorldSave *func_02012f8c(GameData *gameData) {
    return getDreamWorldStuffAddress(gameData->saveControl);
}

void *getChatterDataAddress(GameData *gameData) {
    return getChatterBlockAddress(gameData->saveControl);
}

void func_02012fa4(GameData *gameData, u8 value) {
    gameData->is30FPSMode = value;
}

u8 GameData_Is30FPSMode(GameData *gameData) {
    return gameData->is30FPSMode;
}

void GameData_UpdateTime(GameData *gameData, u32 seconds) {
    updateGameTime(gameData->playTime, seconds);
}

PlayTime *func_02012fcc(GameData *gameData) {
    return gameData->playTime;
}

void *GetTradedPokemonBlock(GameData *gameData) {
    return SaveControl_GetBlockPtr(gameData->saveControl, SAVE_BLOCK_TRADED_POKEMON);
}

EventWork *getConstDataBlock(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_EVENT_WORK);
}

