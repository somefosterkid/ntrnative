#include "global.h"
#include "overlay_28.h"

extern u8 ov28_0225EA91[];

s32 ov28_0225E51C(DowsingMchnApp *app, s32 arg1, s32 arg2) {
    s32 temp_r2;
    s32 temp_r6;
    s32 var_r1;

    temp_r2 = app->proximity;
    if (temp_r2 == 4) {
        temp_r6 = abs((app->nearestX + 7) - arg1);
        var_r1 = 0x10 - (temp_r6 + abs((app->nearestY + 7) - arg2));
        if (var_r1 < 5) {
            var_r1 = 5;
        } else if (var_r1 > 0x10) {
            var_r1 = 0x10;
        }
        return (u8)((0xA * var_r1) - 1);
    }
    return ov28_0225EA91[temp_r2 * 2];
}
