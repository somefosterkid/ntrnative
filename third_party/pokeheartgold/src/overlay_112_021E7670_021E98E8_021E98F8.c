#include "global.h"

u8 ov112_021E98F8(void *arg0, s32 arg1, s32 arg2);

u8 ov112_021E98F8(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_r3;
    int temp_r4;

    temp_r3 = (arg2 % 8);
    temp_r4 = arg1 * 2;
    temp_r4 += (arg2 / 8) << 7;
    return (u8)(((u32)(((s32) * (u8 *)(arg0 + (temp_r4 + 1)) >> temp_r3) << 0x1F) >> 0x1E) | (1 & ((s32) * (u8 *)(arg0 + temp_r4) >> temp_r3)));
}
