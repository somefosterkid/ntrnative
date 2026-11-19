#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sys_task.h"
#include "unk_0202D230.h"
#include "overlay_80_frontier_opponent.h"

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
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
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
static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}
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

void sub_0202D8A4(WifiBattleTowerDownloadData *downloadData, WifiBattleTowerMatchCandidate *matchCandidates, u8 rank, u8 opponentIdx);

void sub_0202D8A4(WifiBattleTowerDownloadData *downloadData, WifiBattleTowerMatchCandidate *matchCandidates, u8 rank, u8 opponentIdx) {
    MI_CpuCopy8(matchCandidates, &downloadData->matchCandidates, sizeof(WifiBattleTowerMatchCandidate) * 30);

    downloadData->storedMatchRank = rank;
    downloadData->storedMatchOpponentIdx = opponentIdx;
    downloadData->hasMatchListData = 1;
}

void sub_0202D8D0(WifiBattleTowerDownloadData *downloadData, WifiBattleTowerIndices *outIndices) {
    outIndices->rank = downloadData->storedMatchRank;
    outIndices->opponentIdx = downloadData->storedMatchOpponentIdx;
}

WifiBattleTowerMatchCandidate *sub_0202D8E4(WifiBattleTowerDownloadData *downloadData, enum HeapID heapID) {
    WifiBattleTowerMatchCandidate *candidates = Heap_Alloc(heapID, sizeof(WifiBattleTowerMatchCandidate) * 30);
    MI_CpuCopy8(downloadData->matchCandidates, candidates, sizeof(WifiBattleTowerMatchCandidate) * 30);

    return candidates;
}
