#include "global.h"

#include "overlay_72_wfc_connect.h"

s32 ov72_0223A7F4(NintendoWfcNumberInput *arg0, s32 arg1);

s32 ov72_0223A7F4(NintendoWfcNumberInput *arg0, s32 arg1) {
    s32 sp18[3];
    s32 spC[3];
    s32 sp8;
    s32 sp4;
    s32 *var_r5_2;
    s32 *var_r6_2;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r2;
    s32 var_r0;
    s32 var_r1;
    s32 var_r4;
    s32 var_r5;
    s32 var_r6;
    s32 *var_r2;
    s8 temp_r5;
    var_r0 = 0x64;
    var_r4 = 3;
    var_r1 = 0;
    var_r2 = &sp18[0];
    do {
        temp_r5 = arg0->digits[var_r1];
        var_r1 += 1;
        *var_r2 = (s32)temp_r5;
        var_r2 += 1;
    } while (var_r1 < 3);
    if (arg1 != (-1)) {
        (&sp18[0])[arg0->digitPos] = arg1;
    }
    sp4 = 0;
    sp8 = 0;
    var_r5 = 0;
    var_r6_2 = &sp18[0];
    do {
        if ((((arg0->digits[var_r5]) == 0) && (sp8 == 0)) || (temp_r2 = *var_r6_2, temp_r2 < 0)) {
            var_r0 = var_r0 / 10;
            var_r4 -= 1;
        } else {
            sp8 = 1;
            temp_r1 = sp4;
            sp4 += 1;
            (&spC[0])[temp_r1] = temp_r2;
        }
        var_r5 += 1;
        var_r6_2 += 1;
    } while (var_r5 < 3);
    var_r6 = 0;
    sp4 = 0;
    if (var_r4 > 0) {
        var_r5_2 = &spC[0];
        do {
            var_r6 += var_r0 * (*var_r5_2);
            var_r0 = var_r0 / 10;
            var_r5_2 += 1;
            temp_r1_2 = sp4 + 1;
            sp4 = temp_r1_2;
        } while (temp_r1_2 < var_r4);
    }
    if (var_r6 > ((s32)(arg0->maxValue))) {
        return -1;
    }
    return var_r6;
}
