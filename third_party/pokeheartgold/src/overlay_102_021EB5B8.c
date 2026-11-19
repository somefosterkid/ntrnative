#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "sys_task_api.h"

s32 ov102_021EB570(void *, u8, s32);

void ov102_021EB5B8(SysTask *arg0, void *arg1);

void ov102_021EB67C(void *arg0);
void *ov102_021E8F68(void *);
void *ov102_021EA268(void *);
struct EasyChatApp;
struct EasyChatWordList;

void *ov102_021EB694(void *arg0, struct EasyChatApp *arg1, struct EasyChatWordList *arg2);

void ov102_021EB5B8(SysTask *task, void *ptr) {
    s32 v0;

    v0 = *(s32 *)((u8 *)ptr + 4);
    switch (v0) {
    case 0:
        ov102_021EB570(*(void **)ptr, (u8) * (u16 *)((u8 *)ptr + 0xA), 1);
        ov102_021EB570(*(void **)ptr, (u8)(1 ^ *(u16 *)((u8 *)ptr + 0xA)), 0);
        ScheduleBgTilemapBufferTransfer(*(BgConfig **)((u8 *)(*(void **)ptr) + 0x10), 3U);
        *(u16 *)((u8 *)ptr + 0xC) = (u16)(*(u16 *)((u8 *)ptr + 0xC) + 1);
        *(s32 *)((u8 *)ptr + 4) = (s32)(*(s32 *)((u8 *)ptr + 4) + 1);
        return;
    case 1:
        *(u16 *)((u8 *)ptr + 8) = (u16)(*(u16 *)((u8 *)ptr + 8) + 1);
        if ((u32) * (u16 *)((u8 *)ptr + 8) >= 4U) {
            *(s32 *)((u8 *)(*(void **)ptr) + 0x64) = 0;
            Heap_Free(ptr);
            SysTask_Destroy(task);
        }
        return;
    }
}

u8 ov102_021E8F7C(void *);

void ov102_021EB624(void *arg0);

void ov102_021EB624(void *arg0) {
    s16 v0;
    void *data;

    v0 = ov102_021E8F7C(*(void **)((u8 *)arg0 + 4));
    data = Heap_Alloc(HEAP_ID_35, 0x1CU);
    *(void **)data = arg0;
    *(s32 *)((u8 *)data + 4) = 0;
    *(s16 *)((u8 *)data + 8) = 0;
    *(s16 *)((u8 *)data + 0xA) = v0;
    *(SysTask **)((u8 *)arg0 + 0x64) = SysTask_CreateOnMainQueue(ov102_021EB5B8, data, 1U);
}

s32 ov102_021EB654(void *arg0);

s32 ov102_021EB654(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x64) == 0) {
        return 1;
    }
    return 0;
}

s32 ov102_021EB2EC(void *, s32, s32);

s32 ov102_021EB3B4(void *, s32);

void ov102_021EB664(void *arg0);

void ov102_021EB664(void *arg0) {
    ov102_021EB3B4(arg0, 1);
    ov102_021EB2EC(arg0, 0, 0);
}

void ov102_021EB67C(void *arg0) {
    ov102_021EB3B4(arg0, 0);
    ov102_021EB2EC(arg0, 1, 0);
}

void *ov102_021EB694(void *arg0, struct EasyChatApp *arg1, struct EasyChatWordList *arg2) {
    void *v0;

    v0 = Heap_Alloc(HEAP_ID_35, 0x64U);
    *(void **)v0 = arg0;
    *(struct EasyChatApp **)((u8 *)v0 + 4) = arg1;
    *(struct EasyChatWordList **)((u8 *)v0 + 8) = arg2;
    *(void **)((u8 *)v0 + 0xC) = ov102_021EA268(arg0);
    *(s32 *)((u8 *)v0 + 0x10) = 0;
    *(s32 *)((u8 *)v0 + 0x18) = 0;
    *(s32 *)((u8 *)v0 + 0x5C) = 0;
    *(void **)((u8 *)v0 + 0x60) = ov102_021E8F68(*(void **)((u8 *)v0 + 4));
    return v0;
}
