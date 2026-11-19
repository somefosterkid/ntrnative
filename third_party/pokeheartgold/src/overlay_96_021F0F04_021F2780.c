#include "global.h"

#include "error_handling.h"

u8 ov96_021F2780(void *arg0, s8 arg1);

u8 ov96_021F2780(void *arg0, s8 arg1) {
    s32 v1;
    u8 v0;

    if ((s32) * (s32 *)((u8 *)arg0 + 0xC) >= 0xC) {
        GF_AssertFail();
    }
    v1 = *(s32 *)((u8 *)arg0 + 0xC);
    if (v1 >= 0xC) {
        return 0xFFU;
    }
    *(s32 *)((u8 *)arg0 + 0xC) = (s32)(v1 + 1);
    v0 = (u8)v1;
    *(u8 *)(arg0 + v0) = arg1;
    return v0;
}

u8 ov96_021F27A8(void *arg0);

u8 ov96_021F27A8(void *arg0) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 0xC) - 1;
    if (v0 < 0) {
        return 0xFFU;
    }
    *(s32 *)((u8 *)arg0 + 0xC) = v0;
    return *(u8 *)(arg0 + v0);
}
