#include "global.h"

s32 ov96_021F38FC(s32 arg0, s32 arg1);

s32 ov96_021F38FC(s32 arg0, s32 arg1) {
    s32 temp_r0;
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r3;
    s32 var_r0;

    temp_r0 = arg0 - 0x10;
    temp_r1 = arg1 - 0x10;
    temp_r3 = (temp_r0 / 0x20);
    temp_r2 = (s32)(temp_r1 + ((u32)(temp_r1 >> 4) >> 0x1B)) >> 5;
    if ((temp_r3 < 0) || (temp_r2 < 0)) {
        return -1;
    }
    var_r0 = temp_r3 + (temp_r2 * 7);
    if (var_r0 > 0x22) {
        var_r0 = -1;
    }
    return var_r0;
}
