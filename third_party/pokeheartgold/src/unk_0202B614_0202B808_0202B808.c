#include "global.h"

#include "error_handling.h"

void sub_0202B808(u8 *flags, u8 count, u8 accessoryID);

void sub_0202B808(u8 *flags, u8 count, u8 accessoryID) {
    u32 temp_r0;
    u8 temp_r1;

    if ((u32)count >= 2U) {
        GF_AssertFail();
    }
    temp_r1 = (accessoryID % 32);
    temp_r0 = (accessoryID / 32) << 0x18;
    temp_r0 >>= 0x16;
    *(s32 *)(flags + temp_r0) = ~(1 << temp_r1) & *(s32 *)(flags + temp_r0);
    *(s32 *)(flags + temp_r0) = *(s32 *)(flags + temp_r0) | (count << temp_r1);
}

u8 sub_0202B848(u8 *flags, u8 accessoryID);

u8 sub_0202B848(u8 *flags, u8 accessoryID) {

    return (u8)(1 & ((u32) * (s32 *)(flags + ((u32)((accessoryID / 32) << 0x18) >> 0x16)) >> (u8)(accessoryID % 32)));
}
