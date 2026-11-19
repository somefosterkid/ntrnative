#include "global.h"

#include "save.h"
#include "constants/heap.h"

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

enum SaveTableEntryID {
    SAVE_TABLE_ENTRY_SYSTEM = 0,
    SAVE_TABLE_ENTRY_PLAYER,
    SAVE_TABLE_ENTRY_PARTY,
    SAVE_TABLE_ENTRY_BAG,
    SAVE_TABLE_ENTRY_VARS_FLAGS,
    SAVE_TABLE_ENTRY_POKETCH,
    SAVE_TABLE_ENTRY_FIELD_PLAYER_STATE,
    SAVE_TABLE_ENTRY_POKEDEX,
    SAVE_TABLE_ENTRY_DAYCARE,
    SAVE_TABLE_ENTRY_PAL_PAD,
    SAVE_TABLE_ENTRY_MISC,
    SAVE_TABLE_ENTRY_FIELD_OVERWORLD_STATE,
    SAVE_TABLE_ENTRY_UNDERGROUND,
    SAVE_TABLE_ENTRY_REGULATION_BATTLES,
    SAVE_TABLE_ENTRY_IMAGE_CLIPS,
    SAVE_TABLE_ENTRY_MAILBOX,
    SAVE_TABLE_ENTRY_POFFINS,
    SAVE_TABLE_ENTRY_RECORD_MIXED_RNG,
    SAVE_TABLE_ENTRY_JOURNAL,
    SAVE_TABLE_ENTRY_TRAINER_CASE,
    SAVE_TABLE_ENTRY_GAME_RECORDS,
    SAVE_TABLE_ENTRY_SEAL_CASE,
    SAVE_TABLE_ENTRY_CHATOT,
    SAVE_TABLE_ENTRY_FRONTIER,
    SAVE_TABLE_ENTRY_RIBBONS,
    SAVE_TABLE_ENTRY_ENCOUNTERS,
    SAVE_TABLE_ENTRY_GLOBAL_TRADE,
    SAVE_TABLE_ENTRY_TV_BROADCAST,
    SAVE_TABLE_ENTRY_RANKINGS,
    SAVE_TABLE_ENTRY_WIFI_LIST,
    SAVE_TABLE_ENTRY_WIFI_HISTORY,
    SAVE_TABLE_ENTRY_MYSTERY_GIFT,
    SAVE_TABLE_ENTRY_PAL_PARK_TRANSFER,
    SAVE_TABLE_ENTRY_LINK_CONTEST_RECORDS,
    SAVE_TABLE_ENTRY_UNLOCKED_EASY_CHAT_WORDS,
    SAVE_TABLE_ENTRY_EMAIL,
    SAVE_TABLE_ENTRY_WIFI_PLAZA_QUESTIONS,
    SAVE_TABLE_ENTRY_PC_BOXES,

    SAVE_TABLE_ENTRY_MAX
};

BOOL ov73_021E7CD8(TVWifiEpisode *param0, int param1, const TVWifiEpisode *param2, int param3);

const void **ov73_021E7D54(int param0, const void **param1, const void **param2);

void ov73_021E7D74(TVWifiEpisode *param0, int param1, int param2, int param3, const void **param4, const void **param5, int param6);

struct TVBroadcast;
typedef struct TVBroadcast TVBroadcast;
typedef u16 charcode_t;
typedef struct {
    u8 unk_00;
    u8 padding_01;
    u16 species;
    u8 gender;
    u8 language;
    u8 metGame;
    u8 hasNickname;
    charcode_t nickname[10 + 1];
    u8 nature;
    u8 unk_1F;
    u8 unk_20;
    u8 padding_21;
    u16 unk_22;
} UnkStruct_0202E7E4;
typedef struct {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_0202E7F0;
typedef struct {
    u8 unk_00;
    u8 win;
    u16 winStreak;
} UnkStruct_0202E7FC;
typedef struct {
    u8 dummy;
    u8 padding_01;
    u16 species;
    u8 gender;
    u8 language;
    u8 metGame;
    u8 numPokemonCaught;
} UnkStruct_0202E808;
typedef struct {
    u8 unk_00;
    u8 padding_01;
    u16 species;
    u8 gender;
    u8 language;
    u8 metGame;
    u8 hasNickname;
    charcode_t nickname[10 + 1];
} UnkStruct_0202E810;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 gender;
    u8 language;
    u8 unk_04;
    u8 padding_05;
    u16 unk_06[8];
} UnkStruct_0202E81C;
typedef struct {
    u8 dummy;
    u8 padding_01[3];
    u32 battlePoints;
} TVBroadcast_BattlePointData;
typedef struct {
    u8 dummy;
    u8 padding_01;
    u16 tradedCount;
} TVBroadcast_GTSData;
typedef struct TVSegment_ContestHall_ShowcasedPokemon {
    u8 unk_00;
    u16 species;
    u8 gender;
    u8 language;
    u8 metGame;
    u8 contestRank;
    u8 contestType;
    u8 contestPlacement;
} TVSegment_ContestHall_ShowcasedPokemon;
struct TVBroadcast {
    u8 playedSegments[4];
    int timeSlotMinutesRemaining;
    u8 programFinished;
    TVSegmentInstance unk_0C[4];
    TVSegmentInstance unk_C4[4];
    TVSegmentInstance unk_17C[4];
    TVWifiEpisode unk_234[16];
    TVWifiEpisode unk_684[8];
    TVWifiEpisode unk_8AC[8];
    TVSegment_ContestHall_ShowcasedPokemon showcasedPokemon;
    UnkStruct_0202E7E4 unk_ADE;
    UnkStruct_0202E7F0 unk_B02;
    UnkStruct_0202E7FC unk_B04;
    UnkStruct_0202E808 safariGame;
    UnkStruct_0202E810 unk_B10;
    UnkStruct_0202E81C unk_B2E;
    TVBroadcast_BattlePointData battlePointData;
    TVBroadcast_GTSData gtsData;
};
void ov73_021E7E24(TVBroadcast *broadcast, int param1, int param2, const void **param3, const void **param4);

void ov73_021E7D74(TVWifiEpisode *param0, int param1, int param2, int param3, const void **param4, const void **param5, int param6) {
    u8 v0[16];
    int v1, v2, v3;
    const void **v4;

    for (v2 = 0; v2 < param2; v2++) {
        (void)0;
    }

    MI_CpuClear8(v0, 16);

    v1 = 0;

    for (v2 = 0; v2 < param2; v2++) {
        if (param4[v2] == NULL) {
            continue;
        }

        for (v3 = 0; v3 < 4; v3++) {
            while (param4[v1] == NULL || v1 == v2 || v0[v1] == 4) {
                v1++;
                v1 %= param2;
            }

            v0[v1]++;

            if (v1 != param3) {
                v4 = ov73_021E7D54(v1, param4, param5);

                if (v4 != NULL) {
                    ov73_021E7CD8(param0, param1, &((TVWifiEpisode *)v4[v1])[v3], param6);
                }
            }
        }
    }

    SaveSubstruct_UpdateCRC((enum SaveTableEntryID)23);
}

void ov73_021E7E24(TVBroadcast *broadcast, int param1, int param2, const void **param3, const void **param4) {
    ov73_021E7D74(broadcast->unk_8AC, 8, param1, param2, param3, param4, 0);
}
