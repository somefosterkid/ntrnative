#include "global.h"
#include "overlay_73_unk_struct_020961_e8_t.h"
#include "save.h"
#include "player_data.h"

struct SaveData;

struct TVBroadcast;

struct TVSegmentInstance;

struct TVSegment_ContestHall_ShowcasedPokemon;

struct TVWifiEpisode;

struct PlayerProfile;



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

u32 ov59_021D2B44(const SaveData *saveData);

u32 ov59_021D2B4C(const SaveData *saveData);

u32 ov59_021D2B54(const SaveData *saveData);

TVWifiEpisode *ov73_021E7B3C(SaveData *saveData, enum HeapID heapID, u32 param2);

TVWifiEpisode *ov73_021E7B84(SaveData *saveData, enum HeapID heapID, u32 param2);

TVWifiEpisode *ov73_021E7BC8(SaveData *saveData, enum HeapID heapID, u32 param2);

TVWifiEpisode *ov73_021E7C0C(SaveData *saveData, enum HeapID heapID, u32 param2);

TVWifiEpisode *ov73_021E7C50(SaveData *saveData, enum HeapID heapID, u32 param2);

TVWifiEpisode *ov73_021E7C94(SaveData *saveData, enum HeapID heapID, u32 param2);

TVBroadcast *sub_020270C4(SaveData *saveData);

int sub_0202D230(void);

PlayerProfile *sub_02034818(int netId);

u16 sub_0203769C(void);

int Save_Rankings_GetMixingSize(void);

void sub_0202E43C(TVBroadcast *broadcast);

void sub_0202E474(TVBroadcast *broadcast);

typedef struct {
    enum HeapID heapID;
    SaveData *saveData;
    int unk_08;
    int unk_0C;
    const void **unk_10;
    const void **unk_14;
} UnkStruct_ov59_021D2FBC;

typedef u32 (*UnkFuncPtr_ov59_021D33FC)(SaveData *);

typedef void *(*UnkFuncPtr_ov59_021D33FC_1)(SaveData *, enum HeapID heapID, u32);

typedef void (*UnkFuncPtr_ov59_021D33FC_2)(const UnkStruct_ov59_021D2FBC *);

typedef struct {
    UnkFuncPtr_ov59_021D33FC unk_00;
    UnkFuncPtr_ov59_021D33FC_1 unk_04;
    UnkFuncPtr_ov59_021D33FC_2 unk_08;
} UnkStruct_ov59_021D33FC;

u32 ov59_021D2F88(SaveData *saveData);

void *ov73_021EA268(SaveData *saveData, enum HeapID heapID, u32 param2);

void ov73_021EA290(const UnkStruct_ov59_021D2FBC *param0);

u32 ov59_021D2FD4(SaveData *saveData);

void *ov73_021EA2B4(SaveData *saveData, enum HeapID heapID, u32 param2);

void ov73_021EA2E0(const UnkStruct_ov59_021D2FBC *param0);

void ov59_021D3028(const UnkStruct_ov59_021D2FBC *param0);

void ov59_021D3048(const UnkStruct_ov59_021D2FBC *param0);

void ov73_021EA2FC(const UnkStruct_ov59_021D2FBC *param0);

void *ov59_021D3088(SaveData *saveData, enum HeapID heapID, u32 param2);

void ov73_021EA324(const UnkStruct_ov59_021D2FBC *param0);

void *ov73_021EA348(SaveData *saveData, enum HeapID heapID, u32 param2);

void ov59_021D30DC(const UnkStruct_ov59_021D2FBC *param0);

extern const UnkStruct_ov59_021D33FC ov73_021EA744[10];

void ov73_021EA3D0(SaveData *saveData, const UnkStruct_ov59_021D30E0 *param1);

void ov73_021EA3D0(SaveData *saveData, const UnkStruct_ov59_021D30E0 *param1) {
    UnkStruct_ov59_021D2FBC v0;
    u32 v1;
    u32 v2 = 0;
    u32 v3 = 3000;
    int i, v5, v6;
    const void *v7[5];
    const void *v8[5];
    u32 v9[4 - 2 + 1];

    v0.heapID = HEAP_ID_50;
    v0.saveData = saveData;
    v0.unk_0C = sub_0203769C();
    v0.unk_08 = 5;
    v0.unk_10 = v7;
    v0.unk_14 = v8;

    v9[2 - 2] = 0;
    v9[3 - 2] = 0;
    v9[4 - 2] = 0;

    for (i = 0; i < 2; i++) {
        v9[2 - 2] += ov73_021EA744[i].unk_00(saveData);
    }

    for (i = 0; i < 3; i++) {
        v9[3 - 2] += ov73_021EA744[i].unk_00(saveData);
    }

    for (i = 0; i < 4; i++) {
        v9[4 - 2] += ov73_021EA744[i].unk_00(saveData);
    }

    for (i = 0; i < NELEMS(ov73_021EA744); i++) {
        v1 = ov73_021EA744[i].unk_00(saveData);
        GF_ASSERT(v3 > v1);

        for (v5 = 0; v5 < 5; v5++) {
            if (sub_02034818(v5) != NULL) {
                v0.unk_10[v5] = &(param1[v5].unk_00[v2]);
            } else {
                v0.unk_10[v5] = NULL;
            }
        }

        switch (i) {
        case 7:
        case 8:
        case 9:
            for (v6 = 0; v6 < 5; v6++) {
                if (sub_02034818(v6) != NULL) {
                    v0.unk_14[v6] = &(param1[v6].unk_00[v9[i - 7]]);
                } else {
                    v0.unk_14[v6] = NULL;
                }
            }
            break;
        }

        if (ov73_021EA744[i].unk_08 != NULL) {
            ov73_021EA744[i].unk_08(&v0);
        }

        v2 += v1;
        v3 -= v1;
    }

    TVBroadcast *broadcast = sub_020270C4(saveData);
    sub_0202E43C(broadcast);
    sub_0202E474(broadcast);
}
