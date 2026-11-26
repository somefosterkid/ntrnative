#include "global.h"

#include "math_util.h"

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_021ECA18_021EDCEC_021EDCEC.c (tools/pokehg_abi.py)
s32 ov96_021EDCEC(void *, unsigned int, unsigned int, unsigned int);
#else
s32 ov96_021EDCEC(void *, u8, u8, u8);
#endif

s32 ov96_021ED9CC(void *arg0);

s32 ov96_021ED9CC(void *arg0) {
    s32 spC;
    s32 sp8;
    s32 sp4;
    s32 sp0;
    s32 temp_r0;
    s32 temp_r0_3;
    int var_r5;
    sp4 = 0;
    sp0 = MTRandom() & 3;
    spC = 0;
loop_1:
    sp8 = 0;

loop_2:
    var_r5 = 0;

loop_3:
    if (ov96_021EDCEC(arg0, (u8)sp0, (u8)sp8, (u8)var_r5) != 0) {
        sp4 = 1;
    } else {
        var_r5 += 1;
        if (((s32)var_r5) < 3) {
            goto loop_3;
        }
    }

    if (sp4 == 0) {
        temp_r0 = sp8 + 1;
        sp8 = temp_r0;
        if (temp_r0 < 4) {
            goto loop_2;
        }
    }
    if (sp4 == 0) {
        sp0 = (sp0 + 1) % 4;
        temp_r0_3 = spC + 1;
        spC = temp_r0_3;
        if (temp_r0_3 < 4) {
            goto loop_1;
        }
    }
    if (sp4 == 0) {
        sp0 = 4;
    }
    return sp0;
}
