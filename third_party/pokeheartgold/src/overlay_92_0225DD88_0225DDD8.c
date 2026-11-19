#include "global.h"

void ov92_0225DDD8(void *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5);

void ov92_0225DDD8(void *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5) {
    *(s16 *)arg0 = arg1;
    *(s16 *)((u8 *)arg0 + 2) = arg2;
    *(s16 *)((u8 *)arg0 + 4) = 0;
    *(s16 *)((u8 *)arg0 + 6) = 0;
    *(s32 *)((u8 *)arg0 + 8) = 0;
    *(s32 *)((u8 *)arg0 + 0x10) = arg3;
    *(s32 *)((u8 *)arg0 + 0x2C) = 2;
    *(s32 *)((u8 *)arg0 + 0x30) = 0;
    *(s32 *)((u8 *)arg0 + 0xC) = arg4;
    *(s32 *)((u8 *)arg0 + 0x14) = arg5;
    *(s32 *)((u8 *)arg0 + 0x18) = arg5;
    *(s32 *)((u8 *)arg0 + 0x1C) = arg5;
    *(s32 *)((u8 *)arg0 + 0x20) = arg5;
    *(s32 *)((u8 *)arg0 + 0x24) = -1;
    *(s32 *)((u8 *)arg0 + 0x28) = -1;
}
