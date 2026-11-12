// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_sound.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_SOUND_H
#define BWPORT_FIELD_FIELD_SOUND_H

// The field's sound system (src/system/field_sound_system.c) and the zone BGM choice (src/system/field_sound.c).
// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except FieldSnd_IsBGMChanging,
// FieldSnd_IsPopDone, FieldSnd_RingRingtone, FieldSnd_StopRingtone, FieldSnd_DuckVolume and FieldSnd_RestoreVolume

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Black 2's, not checked against Black
// A request to the field's sound system
typedef enum {
    FIELD_SND_CMD_NULL = 0x0,
    FIELD_SND_FADE_IN = 0x1,
    FIELD_SND_FADE_OUT = 0x2,
    FIELD_SND_BGM_PUSH = 0x3,
    FIELD_SND_BGM_POP = 0x4,
    FIELD_SND_BGM_CHANGE = 0x5,
    FIELD_SND_BGM_PREPARE = 0x6,
    FIELD_SND_BGM_PLAY = 0x7,
} FieldSoundCommand;

u32 FieldSnd_BGMGetStackIndex(FieldSound *fieldSound);
BOOL FieldSnd_IsBusy(FieldSound *fieldSound);
void FieldSnd_SendRequest(FieldSound *fieldSound, FieldSoundCommand command, u32 bgm, u16 fadeOutFrames,
                          u16 fadeInFrames);
void FieldSnd_SetPlayerVolumeFade(FieldSound *fieldSound, u8 volume, u8 duration);
void FieldSnd_PlayAmbienceEx(FieldSound *fieldSound, u32 se, u32 volume);
void FieldSnd_StopAmbienceAll(FieldSound *fieldSound, u32 flags);
void FieldSnd_ResumeAmbience(FieldSound *fieldSound, u32 flags);
// src/system/field_sound.c
// Changes to the zone's BGM, fading in only when the player is cycling
void FieldSnd_ChangeZoneBGM(FieldSound *fieldSound, GameData *gameData, u16 zoneId);
// Prepares the zone's BGM, for FieldSnd_FadeInImmediate to start
void FieldSnd_SetZoneBGM(FieldSound *fieldSound, GameData *gameData, u16 zoneId, u8 season);
void FieldSnd_FadeInImmediate(FieldSound *fieldSound, GameData *gameData);

u32 FieldSnd_GetNowBGM(const FieldSound *fieldSound);
// Changes to the BGM for the player's new state (walking, cycling or surfing)
void FieldSnd_SetBGMOnPlayerExStateChange(FieldSound *fieldSound, GameData *gameData, u16 zoneId);
// pokebw2's FieldSnd_DuckVolume
// Silences the 3D sound and lowers the BGM, such as for a menu over the field
void func_02029dbc(FieldSound *fieldSound, ISS *iss);
// pokebw2's FieldSnd_RestoreVolume
void func_02029dd8(FieldSound *fieldSound, ISS *iss);

void FieldSnd_PlayAmbience(FieldSound *fieldSound, u32 se);

// src/system/field_sound_system.c
FieldSound *FieldSnd_Create(GameData *gameData, HeapID heapId);
void FieldSnd_Free(FieldSound *fieldSound);

// Called every frame
void FieldSnd_Update(FieldSound *fieldSound);

void FieldSnd_SetAmbienceVolume(FieldSound *fieldSound, u32 se, u16 volume);
void FieldSnd_StopAmbience(FieldSound *fieldSound, u32 se);

// pokebw2's FieldSnd_RingRingtone
// Ring and stop the ringtone
void func_02028a64(FieldSound *fieldSound);

// Ambient sound effects: a looped one is played again after a stop by any of the flags is lifted. volume is 0 to 127,
// or FIELD_SND_AMBIENCE_VOLUME_DEFAULT for the sequence's own
#define FIELD_SND_AMBIENCE_VOLUME_DEFAULT 0xffff

// The stop flag that the BGM push sets
#define FIELD_SND_AMBIENCE_STOP_BGM_PUSH 0x1

// pokebw2's FieldSnd_IsBGMChanging
// Whether the BGM is changing, fading, being pushed or popped
BOOL func_02028988(FieldSound *fieldSound);
// pokebw2's FieldSnd_IsPopDone
// Whether no pop is still running or queued
BOOL func_020289d4(FieldSound *fieldSound);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void FieldSnd_Release(FieldSound *fieldSound, GameData *gameData);
u32 GetMapBGMIDByPlayerState2(GameData *gameData, s32 zoneId, u8 season);
// pokebw2's FieldSnd_StopRingtone
void func_02028a70(FieldSound *fieldSound);

u32 GameData_GetNowBGM(GameData *gameData);
u32 GameData_IsZoneBGMOverriden(GameData *gameData, s32 zoneId);

u16 GetTrainerClassEyeBGMID(u32 trainerClass);

#endif // BWPORT_FIELD_FIELD_SOUND_H
