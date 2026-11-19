#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "sys_task.h"
#include "heap.h"
#include "unk_02069528.h"
#include "error_handling.h"

SysTask *Save_GetPlayerMixingRankingEntry(SysTaskFunc callback, void *param, u32 priority);

SysTask *ov73_021EA31C(SysTaskFunc callback, void *param, int priority);

UnkStruct_02069528 *ov73_021EA348(SaveData *arg0, enum HeapID arg1, u32 arg2);
void ov73_021EA370(void);

extern void *ov73_021EA744;
void ov73_021EA374(u32 param_1, void *param_2);

SysTask *ov73_021EA31C(SysTaskFunc callback, void *param, int priority) {
    return Save_GetPlayerMixingRankingEntry(callback, param, priority);
}

struct Rankings;

typedef struct Rankings Rankings;

Rankings *Save_Rankings_Get(SaveData *saveData);

void Save_UpdateRankingsFromMixing(SaveData *saveData, int participantIndex, u8 participantCount, const void **rankingData, enum HeapID heapID);

typedef struct {
    enum HeapID heapID;
    SaveData *saveData;
    int unk_08;
    int unk_0C;
    const void **unk_10;
    const void **unk_14;
} UnkStruct_ov59_021D2FBC;

void ov73_021EA324(const UnkStruct_ov59_021D2FBC *param0);

void ov73_021EA324(const UnkStruct_ov59_021D2FBC *param0) {
    Rankings *v0 = Save_Rankings_Get(param0->saveData);

    Save_UpdateRankingsFromMixing(param0->saveData, param0->unk_0C, param0->unk_08, param0->unk_10, param0->heapID);
}

UnkStruct_02069528 *ov73_021EA348(SaveData *saveData, enum HeapID heapID, u32 size) {
    UnkStruct_02069528 *v0;

    v0 = Heap_AllocAtEnd(heapID, size);
    MI_CpuFill8(v0, 0U, size);
    sub_02069528(saveData, 0, v0);
    return v0;
}

void ov73_021EA370(void) {
}

void ov73_021EA374(u32 param_1, void *param_2) {
    u32 size;
    void *srcp;
    u32 *puVar1;
    u32 uVar2;
    void *pvStack_1c;
    s32 iStack_18;

    uVar2 = 3000;
    puVar1 = (u32 *)&ov73_021EA744;
    iStack_18 = 0;
    pvStack_1c = param_2;
    do {
        size = (*(u32(*)(u32)) * puVar1)(param_1);
        srcp = (void *)(*(void *(*)(u32, s32, u32))puVar1[1])(param_1, 0x32, size);
        if (uVar2 <= size) {
            GF_AssertFail();
        }
        MIi_CpuCopyFast(srcp, pvStack_1c, size);
        Heap_Free(srcp);
        uVar2 = uVar2 - size;
        pvStack_1c = (void *)((u8 *)pvStack_1c + size);
        puVar1 = puVar1 + 3;
        iStack_18 = iStack_18 + 1;
    } while (iStack_18 < 10);
    return;
}
