#include "global.h"

s32 sub_02031DA0(u8 *, u16 *, s32, s32);

u32 sub_02031FE8(void *arg0, s32 arg1, s32 arg2, s32 arg3);

u32 sub_02031FE8(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u16 sp0[7];
    u32 var_r0;

    sub_02031DA0(arg0, sp0, arg2, arg3);
    var_r0 = (u32)(sp0[0] << 0x1C) >> 0x1C;
    if (var_r0 >= 5U) {
        var_r0 = 5;
    }
    return var_r0;
}
