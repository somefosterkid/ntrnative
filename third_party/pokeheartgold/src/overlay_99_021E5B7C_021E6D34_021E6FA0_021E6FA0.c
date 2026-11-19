#include "global.h"

#include "error_handling.h"

u32 ov99_021E6FA0(s32 arg0);

u32 ov99_021E6FA0(s32 arg0) {
    s32 var_r4;
    s32 var_r6;
    s32 var_r7;
    u32 temp_r0;
    u32 var_r5;

    var_r6 = arg0;
    var_r5 = 0;
    if (var_r6 == 0) {
        GF_AssertFail();
    }
    var_r7 = 0x1ED;
    var_r4 = 0;
    do {
        temp_r0 = (var_r5 + MATH_CountPopulation((u32) * (u8 *)(var_r6 + var_r4))) << 0x10;
        var_r4 += 1;
        var_r5 = temp_r0 >> 0x10;
    } while (var_r4 < 0x1ED);
    return var_r5;
}
