#include "global.h"

#include "error_handling.h"

void ov96_021FB5C8(s32 arg0, u32 arg1, u32 arg2);

void ov96_021FB5C8(s32 arg0, u32 arg1, u32 arg2) {
    s32 var_r5;
    u32 temp_r1;
    s32 var_r4;
    u32 var_r6;

    var_r4 = arg1;
    var_r5 = arg0;
    var_r6 = arg2;
    if (var_r4 >= 0xCU) {
        GF_AssertFail();
    }
    if ((var_r6 == 0) || (var_r6 > 0xCU)) {
        GF_AssertFail();
    }
    var_r5 += 0x1C;
    temp_r1 = (u32)(var_r4 << 0x17) >> 0x18;
    *(u8 *)(var_r5 + temp_r1) = *(u8 *)(var_r5 + temp_r1) | (u8)(var_r6 << ((u32)((var_r4 % 2) << 0x18) >> 0x16));
}
