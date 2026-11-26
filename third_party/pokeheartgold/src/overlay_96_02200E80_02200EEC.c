#include "global.h"

#include "error_handling.h"
#include "sys_task.h"

void Heap_Free(void *ptr);

void ov96_02200EEC(void *ptr);

void ov96_02200EEC(void *ptr) {
    Heap_Free(ptr);
}

s32 ov96_02200F84(void *, void *, u32, s32);

u8 ov96_02200EF4(void *arg0, void *arg1, s32 arg2);

u8 ov96_02200EF4(void *arg0, void *arg1, s32 arg2) {
    s32 v1;
    u8 v0;
    u8 v2;

    v2 = 4;
    if (*(u8 *)((u8 *)arg1 + 0x3E5) == 4) {
        return 0U;
    }
    *(u8 *)((u8 *)arg1 + 0x3E2) = (u8)(*(u8 *)((u8 *)arg1 + 0x3E2) + 1);
    if ((u32) * (u8 *)((u8 *)arg1 + 0x3E2) >= (u32) * (u8 *)((u8 *)arg1 + 0x3E3)) {
        *(s32 *)((u8 *)arg1 + 0x3DC) = 1;
        *(u8 *)((u8 *)arg1 + 0x3E2) = 0U;
    }
    if (*(s32 *)((u8 *)arg1 + 0x3DC) != 0) {
        v0 = *(u8 *)((u8 *)arg1 + 0x3E4);
        v1 = v0 + *(u8 *)((u8 *)arg1 + 0x3E1);
        if (v1 < (s32)v0) {
            GF_AssertFail();
        }
        ov96_02200F84(arg0, arg1, (u8)v1, arg2);
        v2 = (u8)v1;
        *(u8 *)((u8 *)arg1 + 0x3E1) = (u8)(*(u8 *)((u8 *)arg1 + 0x3E1) + 1);
        if ((u32) * (u8 *)((u8 *)arg1 + 0x3E1) >= (u32) * (u8 *)((u8 *)arg1 + 0x3E0)) {
            *(u8 *)((u8 *)arg1 + 0x3E1) = 0U;
            *(s32 *)((u8 *)arg1 + 0x3DC) = 0;
        }
    }
    return v2;
}
