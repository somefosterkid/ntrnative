#include "global.h"

s32 ov96_02210C48(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 unused4, s32 arg5);

s32 ov96_02210C48(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 unused4, s32 arg5) {
    s32 temp_r2;
    s32 var_r0;
    s32 var_r1;
    s32 var_r3;
    s32 var_r4;

    var_r4 = arg1 - *(s16 *)(&unused4);
    if (var_r4 < 0) {
        var_r1 = 0 - var_r4;
    } else {
        var_r1 = var_r4;
    }
    var_r4 = (var_r4 < 0) ? 0 - var_r4 : var_r4;
    var_r3 = arg0 - arg3;
    if (var_r3 < 0) {
        var_r0 = 0 - var_r3;
    } else {
        var_r0 = var_r3;
    }
    var_r3 = (var_r3 < 0) ? 0 - var_r3 : var_r3;
    temp_r2 = arg2 + arg5;
    if ((s32)(temp_r2 * temp_r2) >= (s32)((var_r3 * var_r0) + (var_r4 * var_r1))) {
        return 1;
    }
    return 0;
}
