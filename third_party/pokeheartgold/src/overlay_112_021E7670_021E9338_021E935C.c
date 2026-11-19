#include "global.h"

s32 ov112_021E935C(void *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov112_021E935C(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r4;
    s32 temp_r2;

    temp_r4 = (arg2 / 8) + ((arg3 / 8) * arg1);
    temp_r2 = (arg2 % 8) + ((arg3 % 8) * 8);
    if ((1 & temp_r2) == 1) {
        return (s32) * (u8 *)(arg0 + ((temp_r2 / 2) + (temp_r4 << 5))) >> 4;
    }
    return 0xF & *(u8 *)(arg0 + ((temp_r2 / 2) + (temp_r4 << 5)));
}
