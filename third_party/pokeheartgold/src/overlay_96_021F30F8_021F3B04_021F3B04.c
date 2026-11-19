#include "global.h"

s32 ov96_021F3B04(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3);

s32 ov96_021F3B04(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3) {
    s32 temp_r0;
    s32 temp_r1;

    temp_r0 = arg0 - 0x10;
    temp_r1 = arg1 - 0x10;
    *arg2 = (s16)(temp_r0 / 0x20);
    *arg3 = (s16)((s32)(temp_r1 + ((u32)(temp_r1 >> 4) >> 0x1B)) >> 5);
    if (((s32)*arg2 < 0) || ((s32)*arg3 < 0)) {
        return 0;
    }
    return 1;
}
