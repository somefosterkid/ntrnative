#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "save.h"
#include "player_data.h"
#include "unk_02034354.h"

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

u32 ov73_021E7A74(const SaveData *saveData);

TVBroadcast *sub_020270C4(SaveData *saveData);

void ov73_021E7AC0(SaveData *saveData, TVWifiEpisode *param1, TVSegmentInstance *param2, int param3, int param4, int param5);

TVWifiEpisode *ov73_021E7B3C(SaveData *saveData, enum HeapID heapID, u32 param2);

s32 ov73_021E7D54(u32 arg0, s32 arg1, s32 arg2);

TVWifiEpisode *ov73_021E7B3C(SaveData *saveData, enum HeapID heapID, u32 param2) {
    TVWifiEpisode *v0 = Heap_AllocAtEnd(heapID, param2);
    TVBroadcast *broadcast = sub_020270C4(saveData);

    ((param2 == ov73_021E7A74(saveData)) ? (void)0 : GF_AssertFail());
    ov73_021E7AC0(saveData, v0, broadcast->unk_17C, 4, 1, 3);

    return v0;
}

u32 ov73_021E7A7C(const SaveData *saveData);

TVWifiEpisode *ov73_021E7B84(SaveData *saveData, enum HeapID heapID, u32 param2);

TVWifiEpisode *ov73_021E7B84(SaveData *saveData, enum HeapID heapID, u32 param2) {
    TVWifiEpisode *v0 = Heap_AllocAtEnd(heapID, param2);
    TVBroadcast *broadcast = sub_020270C4(saveData);

    ((param2 == ov73_021E7A7C(saveData)) ? (void)0 : GF_AssertFail());
    ov73_021E7AC0(saveData, v0, broadcast->unk_0C, 4, 1, 4);

    return v0;
}

u32 ov73_021E7A84(const SaveData *saveData);

TVWifiEpisode *ov73_021E7BC8(SaveData *saveData, enum HeapID heapID, u32 param2);

TVWifiEpisode *ov73_021E7BC8(SaveData *saveData, enum HeapID heapID, u32 param2) {
    TVWifiEpisode *v0 = Heap_AllocAtEnd(heapID, param2);
    TVBroadcast *broadcast = sub_020270C4(saveData);

    ((param2 == ov73_021E7A84(saveData)) ? (void)0 : GF_AssertFail());
    ov73_021E7AC0(saveData, v0, broadcast->unk_C4, 4, 1, 5);

    return v0;
}

TVWifiEpisode *ov73_021E7C0C(SaveData *saveData, enum HeapID heapID, u32 param2);

TVWifiEpisode *ov73_021E7C0C(SaveData *saveData, enum HeapID heapID, u32 param2) {
    TVWifiEpisode *v0 = Heap_AllocAtEnd(heapID, param2);
    TVBroadcast *broadcast = sub_020270C4(saveData);

    ((param2 == ov73_021E7A74(saveData)) ? (void)0 : GF_AssertFail());
    ov73_021E7AC0(saveData, v0, broadcast->unk_17C, 4, 0, 0);

    return v0;
}

TVWifiEpisode *ov73_021E7C50(SaveData *saveData, enum HeapID heapID, u32 param2);

TVWifiEpisode *ov73_021E7C50(SaveData *saveData, enum HeapID heapID, u32 param2) {
    TVWifiEpisode *v0 = Heap_AllocAtEnd(heapID, param2);
    TVBroadcast *broadcast = sub_020270C4(saveData);

    ((param2 == ov73_021E7A7C(saveData)) ? (void)0 : GF_AssertFail());
    ov73_021E7AC0(saveData, v0, broadcast->unk_0C, 4, 0, 1);

    return v0;
}

TVWifiEpisode *ov73_021E7C94(SaveData *saveData, enum HeapID heapID, u32 param2);

TVWifiEpisode *ov73_021E7C94(SaveData *saveData, enum HeapID heapID, u32 param2) {
    TVWifiEpisode *v0 = Heap_AllocAtEnd(heapID, param2);
    TVBroadcast *broadcast = sub_020270C4(saveData);

    ((param2 == ov73_021E7A84(saveData)) ? (void)0 : GF_AssertFail());
    ov73_021E7AC0(saveData, v0, broadcast->unk_C4, 4, 0, 2);

    return v0;
}

BOOL ov73_021E7A8C(int param0, int param1);

BOOL ov73_021E7CD8(TVWifiEpisode *param0, int param1, const TVWifiEpisode *param2, int param3);

BOOL ov73_021E7CD8(TVWifiEpisode *param0, int param1, const TVWifiEpisode *param2, int param3) {
    int v0;

    if (param2->details.segmentID == 0) {
        return 0;
    }

    if (ov73_021E7A8C(param3, param2->details.segmentID) == 0) {
        return -1;
    }

    for (v0 = 0; v0 < param1; v0++) {
        if (param0[v0].details.segmentID == 0) {
            param0[v0] = *param2;
            return 1;
        }
    }

    for (v0 = 0; v0 < param1; v0++) {
        if (param0[v0].details.timesPlayed >= 3) {
            param0[v0] = *param2;
            return 1;
        }
    }

    return 0;
}

s32 ov73_021E7D54(u32 a0, s32 arg1, s32 arg2) {
    PlayerProfile *profile;
    s32 v0;

    v0 = arg2;
    profile = sub_02034818(a0);
    if (profile == NULL) {
        return 0;
    }
    if (PlayerProfile_GetVersion(profile) != 0) {
        v0 = arg1;
    }
    return v0;
}
