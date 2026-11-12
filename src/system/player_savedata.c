// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/player_savedata.c, ported by tools/port_file.py.
#include "types.h"
#include "save/player_savedata.h"
#include "field/field_status.h"
#include "field/pair_sys.h"
#include "field/player_state.h"
#include "gfl/std.h"
#include "save/save_control.h"

// The save block with the player's place, the places to return to, the follower and the step counters. The ROM has no
// string for the file, so the name is a guess. Block 0x1c.

#define SAVE_BLOCK_PLAYER 0x1c
#define STEP_COUNTER_NONE 0xffff
#define STEP_COUNTER_MAX 9999

u32 PlayerSave_GetBlockSize(void) {
    return 0x9c;
}

void PlayerSave_Init(PlayerSave *playerSave) {
    MI_CpuFill8(playerSave, 0, 0x9c);
    playerSave->placeNameZoneId = 0x1ab;
}

ZoneSpawnInfo *PlayerSave_GetNowSpawnZone(PlayerSave *playerSave) {
    return &playerSave->nowSpawn;
}

ZoneSpawnInfo *PlayerSave_GetNextSpawnZone(PlayerSave *playerSave) {
    return &playerSave->nextSpawn;
}

ZoneSpawnInfo *PlayerSave_GetEscapeRopeZone(PlayerSave *playerSave) {
    return &playerSave->escapeRopeSpawn;
}

u16 *PlayerSave_GetReturnZoneIDPtr(PlayerSave *playerSave) {
    return &playerSave->returnZoneId;
}

PlayerSave *SaveControl_GetPlayerSave(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_PLAYER);
}

void SaveControl_SavePlayerState(SaveControl *save, const PlayerState *state) {
    PlayerSave *playerSave = SaveControl_GetPlayerSave(save);

    playerSave->location.zoneId = state->zoneId;
    playerSave->location.pos = state->position;
    playerSave->location.unk10 = state->unk10;
    playerSave->location.unk12 = state->unk12;
    playerSave->location.unk14 = state->unk14;
    playerSave->location.unk16 = state->unk16;
    playerSave->location.unk18 = state->unk18;
    playerSave->location.unk1A = state->unk1B;
    playerSave->location.exState = state->exState;
}

void SaveControl_LoadPlayerState(SaveControl *save, PlayerState *state) {
    PlayerSave *playerSave = SaveControl_GetPlayerSave(save);

    state->zoneId = playerSave->location.zoneId;
    state->position = playerSave->location.pos;
    state->unk10 = playerSave->location.unk10;
    state->unk12 = playerSave->location.unk12;
    state->unk14 = playerSave->location.unk14;
    state->unk16 = playerSave->location.unk16;
    state->unk18 = playerSave->location.unk18;
    state->unk1B = playerSave->location.unk1A;
    state->exState = playerSave->location.exState;
}

void func_0200890c(SaveControl *save, SaveLocation *location) {
    PlayerSave *playerSave = SaveControl_GetPlayerSave(save);
    *location = playerSave->location;
}

void PlayerSave_SaveFieldStatus(PlayerSave *playerSave, FieldStatus *status) {
    playerSave->flashUsed = FieldStatus_CheckFlashUsed(status);
}

void PlayerSave_LoadFieldStatus(PlayerSave *playerSave, FieldStatus *status) {
    FieldStatus_SetFlashUsed(status, playerSave->flashUsed);
}

void PlayerSave_SaveSeason(PlayerSave *playerSave, u8 season) {
    playerSave->season = season;
}

void PlayerSave_LoadSeason(PlayerSave *playerSave, u8 *season) {
    *season = playerSave->season;
}

void PlayerSave_SaveWeather(PlayerSave *playerSave, u8 weather) {
    playerSave->weather = weather;
}

void PlayerSave_LoadWeather(PlayerSave *playerSave, u8 *weather) {
    *weather = playerSave->weather;
}

void PlayerSave_SetEggCycleSubsteps(PlayerSave *playerSave, u32 value) {
    ((u32 *)playerSave)[0x1E] = value;
}


void PlayerSave_GetEggCycleSubsteps(PlayerSave *playerSave, u32 *substeps) {
    *substeps = playerSave->eggCycleSubsteps;
}

u16 PlayerSave_GetHappinesStepCounter(PlayerSave *playerSave) {
    return playerSave->happinessStepCounter;
}

void PlayerSave_SetHappinessStepCounter(PlayerSave *playerSave, u16 count) {
    playerSave->happinessStepCounter = count;
}

void PlayerSave_SetAbyssalRuinsStepCounter(PlayerSave *playerSave, u16 count) {
    playerSave->abyssalRuinsStepCounter = count;
}

u16 PlayerSave_GetAbyssalRuinsStepCounter(PlayerSave *playerSave) {
    return playerSave->abyssalRuinsStepCounter;
}

u16 PlayerSave_GetPlaceNameZoneID(PlayerSave *playerSave) {
    return playerSave->placeNameZoneId;
}

void PlayerSave_SetPlaceNameZoneID(PlayerSave *playerSave, u16 zoneId) {
    playerSave->placeNameZoneId = zoneId;
}

