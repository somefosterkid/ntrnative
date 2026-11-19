#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "unk_02035900.h"
#include "heap.h"
#include "sys_task_api.h"


void ov96_021E8884(SysTask *task, void *data);
void *ov96_021E883C(s32 arg0, s32 arg1, void *arg2, enum HeapID arg3);

BOOL ov96_021E8828(void *param_1) {
    u16 uVar1;
    uVar1 = *((s16 *)param_1);
    if (uVar1 == 1) {
        uVar1 = sub_0203769C();
        return (u32)uVar1;
    }
    return 0;
}

void *ov96_021E883C(s32 arg0, s32 arg1, void *arg2, enum HeapID heapID) {
    void *v0;

    v0 = Heap_Alloc(heapID, 0x1D0U);
    memset(v0, 0, 0x1D0U);
    *(s32 *)((u8 *)v0 + 0x1C0) = arg0;
    *(s32 *)((u8 *)v0 + 0x1C4) = arg1;
    *(void **)((u8 *)v0 + 0x1C8) = arg2;
    *(SysTask **)((u8 *)v0 + 0x1CC) = SysTask_CreateOnVWaitQueue(ov96_021E8884, v0, 0x1388U);
    return v0;
}
