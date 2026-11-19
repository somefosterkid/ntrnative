#include "global.h"

#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

s32 ov71_02248CF4(SysTask *);

void ov71_02248E04(SysTask *arg0);

void ov71_02248E04(SysTask *arg0) {
    void *ptr;

    if (arg0 != NULL) {
        ptr = SysTask_GetData(arg0);
        **(s32 **)((u8 *)ptr + 0x168) = 0;
        ov71_02248CF4(*(void **)((u8 *)ptr + 0x16C));
        Heap_Free(ptr);
        SysTask_Destroy(arg0);
    }
}

s32 ov71_02247708(void *, s32);

static void ov71_02248E78(SysTask *task, void *data);

void ov71_02248E30(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov71_02248E30(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *v0;
    s32 v2;
    void *v1;

    v0 = arg0;
    v1 = Heap_Alloc(HEAP_ID_57, 0x14U);
    if (v1 != NULL) {
        *(s32 *)v1 = (s32)(arg1 << 0xC);
        v2 = arg2 << 0xC;
        *(s32 *)((u8 *)v1 + 8) = v2;
        *(s32 *)((u8 *)v1 + 4) = (s32)((s32)(v2 - *(s32 *)v1) / arg3);
        *(s32 *)((u8 *)v1 + 0xC) = arg3;
        *(void **)((u8 *)v1 + 0x10) = v0;
        ov71_02247708(v0, arg1);
        SysTask_CreateOnMainQueue(ov71_02248E78, v1, 0U);
    }
}

static void ov71_02248E78(SysTask *arg0, void *ptr) {
    s32 v0;
    s32 v1;

    v0 = *(s32 *)((u8 *)ptr + 0xC) - 1;
    *(s32 *)((u8 *)ptr + 0xC) = v0;
    if (v0 > 0) {
        v1 = *(s32 *)ptr + *(s32 *)((u8 *)ptr + 4);
        *(s32 *)ptr = v1;
        ov71_02247708(*(void **)((u8 *)ptr + 0x10), v1 >> 0xC);
        return;
    }
    ov71_02247708(*(void **)((u8 *)ptr + 0x10), (s32) * (s32 *)((u8 *)ptr + 8) >> 0xC);
    Heap_Free(ptr);
    SysTask_Destroy(arg0);
}
