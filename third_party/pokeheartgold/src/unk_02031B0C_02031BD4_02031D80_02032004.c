#include "global.h"

s32 sub_02031DA0(u8 *, s32 *, s32, s32);

s32 sub_02032004(void *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 sub_02032004(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp0[4];

    sub_02031DA0(arg0, sp0, arg2, arg3);
    if (*(u8 *)((u8 *)(sp0) + 0xC) == 0) {
        return 0;
    }
    return *(u8 *)((u8 *)(sp0) + 6) + 1;
}
