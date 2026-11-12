// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/field_sound.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The field's choice of BGM for a zone: the zone's own, the cycling or surfing BGM, or one that an event flag puts on
// it. The file's name is descriptive, a guess: the ROM has no string for it. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except ZoneBGMOverride, data_0209fe94, func_02029dbc and
// func_02029dd8

#include "types.h"
#include "constants/sound.h"
#include "field/field_sound.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "save/event_work.h"
#include "system/game_data.h"
#include "system/iss_3ds_sys.h"
#include "system/iss_sys.h"

// What GetZoneBGMOverrideID returns when nothing overrides the zone's BGM
#define NO_BGM_OVERRIDE 0xFFFFFFFF

// A BGM that plays in a zone while an event flag is set
typedef struct {
    u16 flag;
    u16 zoneId;
    u32 bgm;
} ZoneBGMOverride;

static u32 GetPlayerLandSurfCycleState(GameData *gameData);
static u32 GetRegularZoneBGMID(GameData *gameData, s32 zoneId, u8 season);
static u32 GetZoneBGMOverrideID(GameData *gameData, s32 zoneId);
static u32 GetMapBGMIDByPlayerState(GameData *gameData, s32 zoneId, u8 season);

// pokebw2's ZONE_WK_BGM_MAP
static const ZoneBGMOverride data_0209fe94[8] = {
    { 0x970, 0x11, SEQ_BGM_E_PLASMA },
    { 0x970, 0x9b, SEQ_BGM_E_PLASMA },
    { 0x97c, 0x90, SEQ_BGM_N_CASTLE_BRIDGE },
    { 0x97d, 0x71, SEQ_BGM_E_DOCTOR2 },
    { 0x97e, 0x18c, SEQ_BGM_E_DOCTOR },
    { 0x982, 0x187, SEQ_BGM_E_FRIEND },
    { 0x983, 0x30, SEQ_BGM_E_SOUND_DESIGNER },
    { 0x984, 0x144, SEQ_BGM_E_PLASMA },
};

void FieldSnd_ChangeZoneBGM(FieldSound *fieldSound, GameData *gameData, u16 zoneId) {
    u8 season = GameData_GetSeason(gameData);
    u32 bgm = GetZoneBGMOverrideID(gameData, zoneId);
    u16 fadeOutFrames;
    u16 fadeInFrames;

    if (bgm == NO_BGM_OVERRIDE) {
        bgm = GetRegularZoneBGMID(gameData, zoneId, season);
    }
    if (GetPlayerLandSurfCycleState(gameData) == FLD_PLAYER_EXSTATE_CYCLING) {
        fadeOutFrames = 60;
        fadeInFrames = 60;
    } else {
        fadeOutFrames = 60;
        fadeInFrames = 0;
    }
    FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_CHANGE, bgm, fadeOutFrames, fadeInFrames);
}

void FieldSnd_SetZoneBGM(FieldSound *fieldSound, GameData *gameData, u16 zoneId, u8 season) {
    u32 bgm = GetZoneBGMOverrideID(gameData, zoneId);

    if (bgm == NO_BGM_OVERRIDE) {
        bgm = GetRegularZoneBGMID(gameData, zoneId, season);
    }
    FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_PREPARE, bgm, 60, 0);
}

void FieldSnd_FadeInImmediate(FieldSound *fieldSound, GameData *gameData) {
    FieldSnd_SendRequest(fieldSound, FIELD_SND_FADE_IN, 0, 0, 0);
}

void FieldSnd_SetBGMOnPlayerExStateChange(FieldSound *fieldSound, GameData *gameData, u16 zoneId) {
    u8 season = GameData_GetSeason(gameData);
    u32 bgm = GetMapBGMIDByPlayerState(gameData, zoneId, season);

    FieldSnd_SendRequest(fieldSound, FIELD_SND_BGM_CHANGE, bgm, 30, 0);
}

// pokebw2's FieldSnd_DuckVolume
void func_02029dbc(FieldSound *fieldSound, ISS *iss) {
    ISS3DSoundSys_ReqChangeMasterVolume(ISS_Get3DSoundSys(iss), 0);
    FieldSnd_SetPlayerVolumeFade(fieldSound, 64, 6);
}

// pokebw2's FieldSnd_RestoreVolume
void func_02029dd8(FieldSound *fieldSound, ISS *iss) {
    ISS3DSoundSys_ReqChangeMasterVolume(ISS_Get3DSoundSys(iss), 127);
    FieldSnd_SetPlayerVolumeFade(fieldSound, 127, 6);
}

u32 GetMapBGMIDByPlayerState2(GameData *gameData, s32 zoneId, u8 season) {
    return GetMapBGMIDByPlayerState(gameData, zoneId, season);
}

u32 GameData_GetNowBGM(GameData *gameData) {
    return FieldSnd_GetNowBGM(GameData_GetFieldSoundSystem(gameData));
}

u32 GameData_IsZoneBGMOverriden(GameData *gameData, s32 zoneId) {
    if (GetZoneBGMOverrideID(gameData, zoneId) != NO_BGM_OVERRIDE) {
        return 1;
    }
    return 0;
}

// The BGM that plays when a trainer of a class spots the player
typedef struct {
    u16 trainerClass;
    u16 bgm;
} TrainerEyeBGM;

static TrainerEyeBGM data_020a75a8[] = {
    { 0x2, SEQ_BGM_EYE_01 },
    { 0x3, SEQ_BGM_EYE_02 },
    { 0x4, SEQ_BGM_EYE_01 },
    { 0x5, SEQ_BGM_EYE_02 },
    { 0x32, SEQ_BGM_EYE_04 },
    { 0x31, SEQ_BGM_EYE_04 },
    { 0x46, SEQ_BGM_EYE_04 },
    { 0x47, SEQ_BGM_EYE_04 },
    { 0x19, SEQ_BGM_EYE_04 },
    { 0x18, SEQ_BGM_EYE_04 },
    { 0x12, SEQ_BGM_EYE_06 },
    { 0x11, SEQ_BGM_EYE_01 },
    { 0x2e, SEQ_BGM_EYE_09 },
    { 0x34, SEQ_BGM_EYE_09 },
    { 0x2a, SEQ_BGM_EYE_02 },
    { 0x29, SEQ_BGM_EYE_01 },
    { 0x4b, SEQ_BGM_EYE_13 },
    { 0x4c, SEQ_BGM_EYE_13 },
    { 0x33, SEQ_BGM_EYE_08 },
    { 0x40, SEQ_BGM_EYE_08 },
    { 0x3c, SEQ_BGM_EYE_10 },
    { 0x3b, SEQ_BGM_EYE_10 },
    { 0x1d, SEQ_BGM_EYE_11 },
    { 0x5b, SEQ_BGM_EYE_07 },
    { 0x5a, SEQ_BGM_EYE_07 },
    { 0x55, SEQ_BGM_EYE_06 },
    { 0x54, SEQ_BGM_EYE_07 },
    { 0x6, SEQ_BGM_EYE_11 },
    { 0x49, SEQ_BGM_EYE_11 },
    { 0x35, SEQ_BGM_EYE_11 },
    { 0x7, SEQ_BGM_EYE_11 },
    { 0x2b, SEQ_BGM_EYE_12 },
    { 0x2c, SEQ_BGM_EYE_12 },
    { 0x1a, SEQ_BGM_EYE_12 },
    { 0x22, SEQ_BGM_EYE_12 },
    { 0xd, SEQ_BGM_EYE_06 },
    { 0x42, SEQ_BGM_EYE_12 },
    { 0x30, SEQ_BGM_EYE_06 },
    { 0x43, SEQ_BGM_EYE_12 },
    { 0x3a, SEQ_BGM_EYE_12 },
    { 0x1b, SEQ_BGM_EYE_11 },
    { 0x1c, SEQ_BGM_EYE_11 },
    { 0x3d, SEQ_BGM_EYE_12 },
    { 0x3e, SEQ_BGM_EYE_12 },
    { 0x23, SEQ_BGM_EYE_08 },
    { 0x24, SEQ_BGM_EYE_08 },
    { 0x53, SEQ_BGM_EYE_12 },
    { 0x39, SEQ_BGM_EYE_05 },
    { 0x48, SEQ_BGM_EYE_08 },
    { 0x1f, SEQ_BGM_EYE_05 },
    { 0x1e, SEQ_BGM_EYE_10 },
    { 0x20, SEQ_BGM_EYE_10 },
    { 0x21, SEQ_BGM_EYE_13 },
    { 0x56, SEQ_BGM_EYE_12 },
    { 0x57, SEQ_BGM_EYE_13 },
    { 0x41, SEQ_BGM_EYE_06 },
    { 0x4a, SEQ_BGM_EYE_11 },
    { 0x10, SEQ_BGM_EYE_03 },
    { 0x3f, SEQ_BGM_EYE_05 },
    { 0x44, SEQ_BGM_EYE_10 },
    { 0x45, SEQ_BGM_EYE_10 },
    { 0x27, SEQ_BGM_EYE_PLASMA },
    { 0x4d, SEQ_BGM_EYE_PLASMA },
    { 0x2d, SEQ_BGM_EYE_11 },
    { 0xf, SEQ_BGM_EYE_03 },
    { 0xe, SEQ_BGM_EYE_03 },
    { 0x8, SEQ_BGM_EYE_12 },
    { 0x9, SEQ_BGM_EYE_12 },
    { 0x69, 0 },
};
u16 GetTrainerClassEyeBGMID(u32 arg) {
    u32 i;
    for (i = 0; data_020a75a8[i].trainerClass != 0x69; i++) {
        if (data_020a75a8[i].trainerClass == arg) {
            return data_020a75a8[i].bgm;
        }
    }
    return SEQ_BGM_EYE_01;
}

static u32 GetPlayerLandSurfCycleState(GameData *gameData) {
    return FieldPlayerState_GetExState(GameData_GetPlayerState(gameData));
}

static u32 GetRegularZoneBGMID(GameData *gameData, s32 zoneId, u8 season) {
    return DecideZoneHeaderBGMID(zoneId, season);
}

static u32 GetZoneBGMOverrideID(GameData *gameData, s32 zoneId) {
    EventWork *eventWork = GameData_GetEventWork(gameData);
    u32 i;

    for (i = 0; i < 8; i++) {
        if (zoneId == data_0209fe94[i].zoneId && EventWork_FlagGet(eventWork, data_0209fe94[i].flag) == TRUE) {
            return data_0209fe94[i].bgm;
        }
    }

    return NO_BGM_OVERRIDE;
}


static u32 GetMapBGMIDByPlayerState(GameData *gameData, s32 zoneId, u8 season) {
    u32 exState = GetPlayerLandSurfCycleState(gameData);
    // Unused: the original fetches the sound system here and drops it
    GameData_GetFieldSoundSystem(gameData);
    u32 bgm = GetZoneBGMOverrideID(gameData, zoneId);

    if (bgm != NO_BGM_OVERRIDE) {
        return bgm;
    }
    if (exState == FLD_PLAYER_EXSTATE_SURF && GetZoneFlagsEnableCycleSurfBGM(zoneId)) {
        return SEQ_BGM_NAMINORI;
    }
    if (exState == FLD_PLAYER_EXSTATE_CYCLING && GetZoneFlagsEnableCycleSurfBGM(zoneId)) {
        return SEQ_BGM_BICYCLE;
    }
    return GetRegularZoneBGMID(gameData, zoneId, season);
}
