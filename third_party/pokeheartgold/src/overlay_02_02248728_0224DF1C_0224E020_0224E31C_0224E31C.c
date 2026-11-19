#include "global.h"

s32 ov02_0224E31C(s32 arg0, s32 arg1);

s32 ov02_0224E31C(s32 arg0, s32 arg1) {
    s32 temp_r0;
    s32 temp_r1;
    s32 var_r0;

    temp_r0 = arg0 - 0x20;
    temp_r1 = arg1 - 0x20;
    var_r0 = (temp_r0 / 0x20) + (((s32)(temp_r1 + ((u32)(temp_r1 >> 4) >> 0x1B)) >> 5) * 3);
    if ((var_r0 < 0) || (var_r0 >= 6)) {
        var_r0 = 0;
    }
    return var_r0;
}
