#include "global.h"
#include "save.h"

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

u32 sub_0202E41C(void);

void sub_0202E424(void *arg0);

u32 sub_0202E41C(void) {
    return sizeof(TVBroadcast);
}

void sub_0202E424(void *destp) {
    MIi_CpuClearFast(0U, destp, 0xBC8U);
    SaveSubstruct_UpdateCRC(0x17);
}
