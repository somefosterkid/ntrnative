#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sys_task.h"
#include "overlay_80_frontier_opponent.h"

enum PokemonStat {
    STAT_HP = 0,
    STAT_ATTACK = 1,
    STAT_DEFENSE = 2,
    STAT_SPEED = 3,
    STAT_SPECIAL_ATTACK = 4,
    STAT_SPECIAL_DEFENSE = 5,
    STAT_MAX = 6,
};

typedef struct WifiTrainerRecord {
    FrontierPokemon mons[3];
    u16 trainerName[7 + 1];
    u8 unk_B8;
    u8 unk_B9;
    u8 unk_BA;
    u8 unk_BB;
    u32 unk_BC;
    u16 unk_C0[4];
    union {
        struct {
            u8 isAnonymous : 1;
            u8 anonymousNameIdx : 1;
            u8 : 6;
        };
        u8 unk_C8_val2;
    };
    u8 trainerType;
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
    u16 unk_E2;
} WifiTrainerRecord;

typedef struct WifiBattleTowerMatchCandidate {
    u16 trainerName[8];
    u8 padding_10[2];
    u8 country;
    u8 region;
    u8 padding_14[4];
    u16 trainerKey[4];
    u8 isAnonymous : 1;
    u8 anonymousNameIdx : 1;
} WifiBattleTowerMatchCandidate;

typedef struct WifiBattleTowerDownloadData {
    u32 lastDownloadDate;
    u8 downloadedOpponents[250];
    u8 hasOpponentData;
    u8 hasMatchListData;
    u8 storedOpponentIdx;
    u8 storedRank;
    u8 storedMatchOpponentIdx;
    u8 storedMatchRank;
    WifiTrainerRecord trainerRecords[7];
    WifiBattleTowerMatchCandidate matchCandidates[30];
} WifiBattleTowerDownloadData;

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

void sub_0202D6DC(void *arg0);
s32 sub_0202D6F8(void *arg0, void *arg1);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

static inline u32 Date_EncodeParams(u32 year, u32 month, u32 day, int week) {
    return (year << 24) | ((month & 0xff) << 16) | ((day & 0xff) << 8) | week;
}

static inline u32 Date_Encode(RTCDate *rtcDate) {
    return Date_EncodeParams(rtcDate->year, rtcDate->month, rtcDate->day, rtcDate->week);
}

void sub_0202D678(WifiBattleTowerDownloadData *downloadData, u8 rank, u8 opponentIdx, RTCDate *date);

void sub_0202D678(WifiBattleTowerDownloadData *downloadData, u8 rank, u8 opponentIdx, RTCDate *date) {
    u8 byteIdx, bitIdx;
    u8 mask = 1;
    u16 flatIdx;

    if (opponentIdx == 0 || opponentIdx > 200) {
        return;
    }

    if (rank == 0 || rank > 10) {
        return;
    }

    flatIdx = (rank - 1) * 200 + (opponentIdx - 1);
    byteIdx = flatIdx / 8;
    bitIdx = flatIdx % 8;

    mask <<= bitIdx;

    downloadData->downloadedOpponents[byteIdx] |= mask;
    downloadData->lastDownloadDate = Date_Encode(date);
}

void sub_0202D6DC(void *dest) {
    MI_CpuFill8(dest + 4, 0U, 0xFAU);
    MI_CpuFill8(dest, 0U, 4U);
}

s32 sub_0202D6F8(void *arg0, void *arg1) {
    if ((u32) * (u32 *)arg0 > (u32) * (u32 *)arg1) {
        return 1;
    }
    if ((u32) * (u32 *)((u8 *)arg0 + 4) > (u32) * (u32 *)((u8 *)arg1 + 4)) {
        return 1;
    }
    if ((u32) * (u32 *)((u8 *)arg0 + 8) > (u32) * (u32 *)((u8 *)arg1 + 8)) {
        return 1;
    }
    return 0;
}
