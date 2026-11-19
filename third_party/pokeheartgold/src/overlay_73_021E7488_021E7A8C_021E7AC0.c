#include "global.h"

#include "player_data.h"
#include "save.h"
#include "string_util.h"

typedef struct TVSegmentInstance {
    u8 segmentID;
    u8 timesPlayed;
    u32 timestamp;
    u8 segment[40];
} TVSegmentInstance;

typedef struct TVWifiEpisode {
    u32 trainerID;
    u8 gender;
    u8 gameVersion;
    u8 language;
    u16 name[8];
    TVSegmentInstance details;
} TVWifiEpisode;

void ov73_021E7AC0(SaveData *param_1, TVWifiEpisode *param_2, TVSegmentInstance *param_3, s32 param_4);

void ov73_021E7AC0(SaveData *param_1, TVWifiEpisode *param_2, TVSegmentInstance *param_3, s32 param_4) {
    PlayerProfile *profile;
    s32 i;

    profile = Save_PlayerData_GetProfile(param_1);

    i = 0;
    while (i < param_4) {
        MIi_CpuClearFast(0, param_2, sizeof(TVWifiEpisode));
        param_2->trainerID = PlayerProfile_GetTrainerID(profile);
        param_2->gender = PlayerProfile_GetTrainerGender(profile);
        param_2->gameVersion = GAME_VERSION;
        param_2->language = LANGUAGE_ENGLISH;
        CopyU16StringArrayN(param_2->name, PlayerProfile_GetNamePtr(profile), 8);
        param_2->details = *param_3;
        param_2->details.timesPlayed = 0;
        param_2->details.segmentID = 0;
        i++;
        param_3++;
        param_2++;
    }
}
