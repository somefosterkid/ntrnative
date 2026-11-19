#include "global.h"

#include "wild_encounter.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "sys_task.h"
#include "overlay_80_frontier_opponent.h"
#include "unk_0202D230.h"
#include "save_frontier.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct FrontierPokemon;

struct NARC;

struct SysTask;

struct SysTaskQueue;

struct WifiBattleTowerDownloadData;

struct WifiBattleTowerMatchCandidate;

struct WifiTrainerRecord;

void Heap_Free(void *ptr);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

extern void sub_0202D6DC(WifiBattleTowerDownloadData *downloadData);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

static inline u8 Date_DecodeYear(u32 date) {
    return date >> 24;
}

static inline u8 Date_DecodeMonth(u32 date) {
    return (date >> 16) & 0xff;
}

static inline u8 Date_DecodeDay(u32 date) {
    return (date >> 8) & 0xff;
}

static inline u8 Date_DecodeWeek(u32 date) {
    return date & 0xff;
}

static inline void Date_Decode(u32 date, RTCDate *rtcDate) {
    rtcDate->year = Date_DecodeYear(date);
    rtcDate->month = Date_DecodeMonth(date);
    rtcDate->day = Date_DecodeDay(date);
    rtcDate->week = (RTCWeek)(Date_DecodeWeek(date));
}

BOOL sub_0202D6F8(RTCDate *dateA, RTCDate *dateB);

WifiBattleTowerMatchCandidate *sub_0202D8E4(WifiBattleTowerDownloadData *downloadData, enum HeapID heapID);

BOOL sub_0202D720(WifiBattleTowerDownloadData *downloadData, u8 rank, u8 opponentIdx, RTCDate *date);

BOOL sub_0202D720(WifiBattleTowerDownloadData *downloadData, u8 rank, u8 opponentIdx, RTCDate *date) {
    u8 byteIdx, bitIdx;
    u8 mask = 1;
    u16 flatIdx;
    RTCDate storedDate;

    if ((opponentIdx > 200) || (rank > 10)) {
        return 0;
    }

    Date_Decode(downloadData->lastDownloadDate, &storedDate);

    if (sub_0202D6F8(date, &storedDate)) {
        sub_0202D6DC(downloadData);
        return 0;
    }

    flatIdx = (rank - 1) * 200 + (opponentIdx - 1);
    byteIdx = flatIdx / 8;
    bitIdx = flatIdx % 8;

    mask <<= bitIdx;

    if (downloadData->downloadedOpponents[byteIdx] & mask) {
        return 1;
    }

    return 0;
}
