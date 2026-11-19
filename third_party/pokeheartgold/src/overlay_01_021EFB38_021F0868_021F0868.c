#include "global.h"

#include "error_handling.h"
#include "overlay_01_021EFB38.h"

s32 ov01_021EFE34(void *, s32, s32, s32);

void ov01_021F0868(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, u8 arg9) {
    if (*(u8 *)((u8 *)arg0 + 0x2E) != 0) {
        GF_AssertFail();
    }
    ov01_021EFE34(arg0, arg1, arg2, arg5);
    ov01_021EFE34(arg0 + 0x14, arg3, arg4, arg5);
    *(s32 *)((u8 *)arg0 + 0x28) = arg6;
    *(s8 *)((u8 *)arg0 + 0x2C) = (s8)arg7;
    *(s8 *)((u8 *)arg0 + 0x2D) = (s8)arg8;
    *(u8 *)((u8 *)arg0 + 0x2F) = (u8)arg9;
    *(u8 *)((u8 *)arg0 + 0x2E) = 1U;
}
