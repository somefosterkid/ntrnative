#include "global.h"

#include "overlay_96_hurdle_cpu.h"


BOOL ov96_022090A8(Ov96HurdleCpuCell *arg0, u16 arg1, u16 arg2) {
    s32 a = arg0->px;
    s32 aLo = a - 0x10;
    s32 aHi = a + 0x10;
    s32 c = arg0->py;
    s32 cLo = c - 0x10;
    s32 cHi = c + 0x10;
    if ((aLo <= arg1) && (arg1 < aHi) && (cLo <= arg2) && (arg2 < cHi)) {
        return 1;
    }
    return 0;
}


BOOL ov96_022090D8(Ov96HurdleCpuCell *arg0) {
    s32 temp_r2;
    s32 temp_r1;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r2;
    s32 var_r2_2;

    temp_r2 = arg0->py;
    if (temp_r2 < 0x68) {
        return 0;
    }
    if (temp_r2 <= 0x188) {
        temp_r1 = arg0->px;
        if (temp_r1 >= 0x90) {
            var_r2 = 1;
        } else {
            var_r2 = 0;
        }
        if (temp_r1 <= 0x170) {
            var_r0 = 1;
        } else {
            var_r0 = 0;
        }
        if (var_r0 & var_r2) {
            return 1;
        }
    } else {
        temp_r1 = arg0->px;
        if (temp_r1 >= 0xD0) {
            var_r2_2 = 1;
        } else {
            var_r2_2 = 0;
        }
        if (temp_r1 <= 0x130) {
            var_r0_2 = 1;
        } else {
            var_r0_2 = 0;
        }
        if (var_r0_2 & var_r2_2) {
            return 1;
        }
    }
    return 0;
}
