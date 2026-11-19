#include "global.h"

void sub_02013004(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 *arg6, s32 *arg7);

void sub_02013004(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 *arg6, s32 *arg7) {
    s32 sp10;
    s32 spC;
    s32 sp8;
    s32 sp4;
    s32 sp0;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r2;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r0_5;
    s32 var_r0_6;
    s32 var_r1;
    s32 var_r2;
    s32 var_r2_2;

    temp_r2 = (arg4 >> 4) << 1;
    sp10 = arg4;
    sp0 = arg1;
    var_r2_2 = temp_r2 + 1;
    spC = sp0 + ((s32)(0xFFFF * FX_SinCosTable_[var_r2_2]) >> 0xC);
    temp_r1 = arg2 + ((s32)(0xFFFF * FX_SinCosTable_[temp_r2]) >> 0xC);
    temp_r0 = (arg5 >> 4) << 1;
    var_r2_2 = temp_r0 + 1;
    sp8 = sp0 + ((s32)(0xFFFF * FX_SinCosTable_[var_r2_2]) >> 0xC);
    sp4 = arg2 + ((s32)(0xFFFF * FX_SinCosTable_[temp_r0]) >> 0xC);
    if ((arg5 - arg4) == (0xFFFFU >> 1)) {
        if ((arg3 < 0) || (arg3 >= arg2)) {
            return;
        }
        *arg6 = 0;
        *arg7 = 0xFF;
        return;
    }
    if ((arg5 >= 0) && (arg5 < (s32)(0xFFFFU >> 1))) {
        if (temp_r1 < arg2) {
            var_r0 = temp_r1;
        } else {
            var_r0 = arg2;
        }
        if (temp_r1 > arg2) {
            var_r2 = temp_r1;
        } else {
            var_r2 = arg2;
        }
        if ((var_r0 <= arg3) && (arg3 <= var_r2)) {
            *arg6 = spC + ((s32)((spC - sp0) * (arg3 - temp_r1)) / (s32)(temp_r1 - arg2));
        } else {
            *arg6 = sp8 + ((s32)((sp8 - sp0) * (arg3 - sp4)) / (s32)(sp4 - arg2));
        }
        var_r0_2 = *arg6;
        if (var_r0_2 > 0xFF) {
            var_r0_2 = 0xFF;
        } else if (var_r0_2 < 0) {
            var_r0_2 = 0;
        }
        *arg6 = var_r0_2;
        *arg7 = 0xFF;
        return;
    }
    if (temp_r1 < arg2) {
        var_r0_3 = temp_r1;
    } else {
        var_r0_3 = arg2;
    }
    if (temp_r1 > arg2) {
        var_r2_2 = temp_r1;
    } else {
        var_r2_2 = arg2;
    }
    if ((var_r0_3 <= arg3) && (arg3 <= var_r2_2)) {
        var_r0_4 = spC + ((s32)((spC - sp0) * (arg3 - temp_r1)) / (s32)(temp_r1 - arg2));
        *arg6 = var_r0_4;
        if (var_r0_4 > 0xFF) {
            var_r0_4 = 0xFF;
        } else if (var_r0_4 < 0) {
            var_r0_4 = 0;
        }
        *arg6 = var_r0_4;
    } else {
        *arg6 = 0;
    }
    if (sp4 < arg2) {
        var_r1 = sp4;
    } else {
        var_r1 = arg2;
    }
    if (sp4 > arg2) {
        var_r2_2 = sp4;
    } else {
        var_r2_2 = arg2;
    }
    var_r0_5 = var_r2_2;
    if ((var_r1 <= arg3) && (arg3 <= var_r0_5)) {
        var_r0_6 = sp8 + ((s32)((sp8 - sp0) * (arg3 - sp4)) / (s32)(sp4 - arg2));
        *arg7 = var_r0_6;
        if (var_r0_6 > 0xFF) {
            var_r0_6 = 0xFF;
        } else if (var_r0_6 < 0) {
            var_r0_6 = 0;
        }
        *arg7 = var_r0_6;
    } else {
        *arg7 = *arg6;
    }
    temp_r1_2 = *arg7;
    temp_r0_2 = *arg6;
    if (temp_r0_2 > temp_r1_2) {
        *arg6 = temp_r1_2;
        *arg7 = temp_r0_2;
    }
}
