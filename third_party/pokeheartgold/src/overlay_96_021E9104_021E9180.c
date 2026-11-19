#include "global.h"

u8 ov96_021E9180(u8 *arg0, s32 arg1);

u8 ov96_021E9180(u8 *arg0, s32 arg1) {
    u8 var_r2;
    u8 var_r4;
    u8 *var_r0;
    u8 var_r5;

    var_r2 = 0xFF;
    var_r0 = (unsigned char *)(arg0 + arg1);
    var_r4 = 0;
loop_1:
    var_r5 = 0;
loop_2:
    if (*var_r0 != 0) {
        var_r2 = var_r4;
    } else {
        var_r5 += 1;
        var_r0 += 1;
        if ((u32)var_r5 < 4U) {
            goto loop_2;
        }
    }
    if (var_r5 == 4) {
        var_r4 += 1;
        if ((u32)var_r4 < 8U) {
            goto loop_1;
        }
    }
    return var_r2;
}
