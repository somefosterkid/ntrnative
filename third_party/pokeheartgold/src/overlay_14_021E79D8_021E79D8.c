#include "global.h"
#include "overlay_14_box_funcs.h"

extern s32 ov14_021F7BD8;

u32 ov14_021E79D8(s32 arg0, s32 arg1) {
    s32 var_r7;
    s32 var_r6;
    u32 var_r4;
    s32 *var_r5;

    var_r5 = &ov14_021F7BD8;
    var_r6 = arg0;
    var_r7 = arg1;
    var_r4 = 0;
loop_1:
    if (ov14_021E7940(var_r6, var_r7, var_r5) == 1) {
        var_r4 += 0x80;
        return var_r4;
    }
    var_r4 += 1;
    var_r5 += 1;
    if (var_r4 >= 6U) {
        return 0xFFU;
    }
    goto loop_1;
}
