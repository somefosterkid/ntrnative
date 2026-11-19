#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "bg_window.h"
#include "filesystem.h"

struct BgConfig;

struct NARC;

struct SysTask;

struct SysTaskQueue;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

void *GfGfxLoader_GetCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BOOL compressed, NNSG2dCharacterData **outCharData, enum HeapID heapID);

void *GfGfxLoader_GetScrnDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BOOL compressed, NNSG2dScreenData **outScreenData, enum HeapID heapID);

void Heap_Free(void *ptr);

SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);

typedef struct {
    SysTask *unk_00;
    BgConfig *unk_04;
    u16 unk_08[2][2048];
    u8 unk_2008[2][65536];
    u8 unk_22008;
    u8 unk_22009;
} UnkStruct_ov104_0223DB34;

void ov80_02239ED8(SysTask *param0, void *param1);

extern const u16 ov80_0223DB2C[2];

UnkStruct_ov104_0223DB34 *ov80_02239DD0(BgConfig *param0);

UnkStruct_ov104_0223DB34 *ov80_02239DD0(BgConfig *param0) {
    UnkStruct_ov104_0223DB34 *v0;

    ((2 == (sizeof(ov80_0223DB2C) / sizeof(*(ov80_0223DB2C)))) ? (void)0 : GF_AssertFail());

    v0 = Heap_Alloc(HEAP_ID_101, sizeof(UnkStruct_ov104_0223DB34));
    MI_CpuClear8(v0, sizeof(UnkStruct_ov104_0223DB34));
    v0->unk_04 = param0;

    {
        NARC *v1;
        void *v2;
        NNSG2dScreenData *v3;
        int v4;

        v1 = NARC_New((enum NarcId)183, HEAP_ID_101);

        for (v4 = 0; v4 < 2; v4++) {
            v2 = GfGfxLoader_GetScrnDataFromOpenNarc(v1, ov80_0223DB2C[v4], 1, &v3, HEAP_ID_101);
            MI_CpuCopy32(v3->rawData, v0->unk_08[v4], 0x1000);
            Heap_Free(v2);
        }

        NARC_Delete(v1);
    }

    {
        NARC *v5;
        void *v6;
        NNSG2dCharacterData *v7;

        v5 = NARC_New((enum NarcId)183, HEAP_ID_101);

        v6 = GfGfxLoader_GetCharDataFromOpenNarc(v5, 14, 1, &v7, HEAP_ID_101);
        MI_CpuCopy32(v7->pRawData, v0->unk_2008[0], v7->szByte);

        Heap_Free(v6);

        v6 = GfGfxLoader_GetCharDataFromOpenNarc(v5, 15, 1, &v7, HEAP_ID_101);
        MI_CpuCopy32(v7->pRawData, v0->unk_2008[1], v7->szByte);

        Heap_Free(v6);
        NARC_Delete(v5);
    }

    v0->unk_00 = SysTask_CreateOnMainQueue(ov80_02239ED8, v0, 80000 - 500);

    return v0;
}
