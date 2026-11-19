#include "global.h"
#include "sys_task.h"

s32 ov71_02247708(void *, s32);

s32 ov71_0224B11C(void *);

void ov71_0224B0E8(SysTask *arg0, void *arg1);

void ov71_0224B0E8(SysTask *arg0, void *arg1) {
    s32 v0;
    s32 v1;

    v0 = *(s32 *)((u8 *)arg1 + 0x10) - 1;
    *(s32 *)((u8 *)arg1 + 0x10) = v0;
    if (v0 > 0) {
        v1 = *(s32 *)((u8 *)arg1 + 4) + *(s32 *)((u8 *)arg1 + 8);
        *(s32 *)((u8 *)arg1 + 4) = v1;
        ov71_02247708(*(void **)((u8 *)arg1 + 0x14), v1 >> 0xC);
        return;
    }
    ov71_02247708(*(void **)((u8 *)arg1 + 0x14), (s32) * (s32 *)((u8 *)arg1 + 0xC) >> 0xC);
    ov71_0224B11C(arg0);
}
