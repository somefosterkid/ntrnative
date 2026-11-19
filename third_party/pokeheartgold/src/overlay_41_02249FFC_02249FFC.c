#include "global.h"
#include "filesystem_files_def.h"
#include "constants/heap.h"
#include "bg_window.h"

struct BgConfig;

struct SysTask;

struct SysTaskQueue;

void *GfGfxLoader_GetCharData(enum NarcId narcID, u32 narcMemberIdx, BOOL compressed, NNSG2dCharacterData **outCharData, enum HeapID heapID);

SysTask * SysTask_CreateOnVWaitQueue(SysTaskFunc func, void *data, u32 priority);

void ov41_0224A094(SysTask *param0, void *param1);

void ov22_02259B8C(enum NarcId narcID, u32 param1, int param2, u32 param3, u32 param4, enum HeapID heapID);

typedef struct {
    BgConfig *unk_00;
    NNSG2dCharacterData *unk_04;
    void *unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov22_02259B3C;

void ov41_02249FFC(enum NarcId narcID, u32 param1, BgConfig *param2, u32 param3, u32 param4, enum HeapID heapID);

void ov41_02249FFC(enum NarcId narcID, u32 param1, BgConfig *param2, u32 param3, u32 param4, enum HeapID heapID) {
    UnkStruct_ov22_02259B3C *v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov22_02259B3C));
    memset(v0, 0, sizeof(UnkStruct_ov22_02259B3C));

    v0->unk_08 = GfGfxLoader_GetCharData(narcID, param1, 0, &v0->unk_04, heapID);
    v0->unk_00 = param2;
    v0->unk_0C = param3;
    v0->unk_10 = param4;

    SysTask_CreateOnVWaitQueue(ov41_0224A094, v0, 128);
}
