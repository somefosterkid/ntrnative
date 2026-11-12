// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/player_savedata.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SAVE_PLAYER_SAVEDATA_H
#define BWPORT_SAVE_PLAYER_SAVEDATA_H

#include "types.h"
#include "struct_decls.h"
#include "field/zone.h"
#include "nitro/fx/fx.h"

PlayerSave *SaveControl_GetPlayerSave(SaveControl *save);

// Where the player saved
typedef struct {
    u16 zoneId;
    VecFx32 pos;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    s16 unk18;
    u8 unk1A;
    u8 exState;
} SaveLocation;

// Black 2's; matching code confirms the offsets of nowSpawn, nextSpawn, escapeRopeSpawn, returnZoneId, flashUsed,
// season, weather, placeNameZoneId, eggCycleSubsteps, happinessStepCounter, abyssalRuinsStepCounter and location in
// Black
// The save block 0x1c (player_savedata.c, a guessed name). The names of the fields are guessed.
struct PlayerSave {
    ZoneSpawnInfo nowSpawn;
    ZoneSpawnInfo unk1C;
    ZoneSpawnInfo nextSpawn;
    ZoneSpawnInfo escapeRopeSpawn;
    u16 returnZoneId;
    u8 flashUsed;
    u8 season;
    u8 weather;
    u8 unk75;
    u16 placeNameZoneId;
    u32 eggCycleSubsteps;
    u16 happinessStepCounter;
    u16 abyssalRuinsStepCounter;
    SaveLocation location;
    // What FieldFollowWk keeps of the follower
    u8 followIndex;
    u8 followOriginActorId;
    u16 followObjCode : 15;
    u16 followIsPersistent : 1;
    u16 followScrId;
    u16 followTrainerId;
    // 0xffff when no step counter is running
    u16 stepCounter;
    u16 unkA6;
};

void SaveControl_SavePlayerState(SaveControl *save, const PlayerState *state);
void SaveControl_LoadPlayerState(SaveControl *save, PlayerState *state);
void PlayerSave_SaveFieldStatus(PlayerSave *playerSave, FieldStatus *status);
void PlayerSave_LoadFieldStatus(PlayerSave *playerSave, FieldStatus *status);
void PlayerSave_SaveSeason(PlayerSave *playerSave, u8 season);
void PlayerSave_LoadSeason(PlayerSave *playerSave, u8 *season);
void PlayerSave_SaveWeather(PlayerSave *playerSave, u8 weather);
void PlayerSave_LoadWeather(PlayerSave *playerSave, u8 *weather);
void PlayerSave_SetEggCycleSubsteps(PlayerSave *playerSave, u32 value);
void PlayerSave_GetEggCycleSubsteps(PlayerSave *playerSave, u32 *substeps);
u16 PlayerSave_GetHappinesStepCounter(PlayerSave *playerSave);
void PlayerSave_SetHappinessStepCounter(PlayerSave *playerSave, u16 count);
void PlayerSave_SetAbyssalRuinsStepCounter(PlayerSave *playerSave, u16 count);
u16 PlayerSave_GetAbyssalRuinsStepCounter(PlayerSave *playerSave);
u16 PlayerSave_GetPlaceNameZoneID(PlayerSave *playerSave);
void PlayerSave_SetPlaceNameZoneID(PlayerSave *playerSave, u16 zoneId);
void func_0200890c(SaveControl *save, SaveLocation *location);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 PlayerSave_GetBlockSize(void);
void PlayerSave_Init(PlayerSave *playerSave);
ZoneSpawnInfo *PlayerSave_GetNowSpawnZone(PlayerSave *playerSave);
ZoneSpawnInfo *PlayerSave_GetNextSpawnZone(PlayerSave *playerSave);
ZoneSpawnInfo *PlayerSave_GetEscapeRopeZone(PlayerSave *playerSave);
u16 *PlayerSave_GetReturnZoneIDPtr(PlayerSave *playerSave);

#endif // BWPORT_SAVE_PLAYER_SAVEDATA_H
