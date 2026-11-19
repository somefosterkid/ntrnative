#include "global.h"

#include "overlay_96_ov96_rower_work.h"

s32 ov96_021EABA8(void *, s32);

s32 ov96_021EB630(void *, s32);

void ov96_021FFB7C(Ov96Rower *arg0, u8 arg1, s32 arg2) {
    Ov96Rower *var_r5;
    s32 temp_r1;
    s32 var_r1;
    s32 var_r6;
    s32 var_r4;

    var_r5 = arg0;
    var_r1 = 0xA8 - arg1;
    if (var_r1 < 0) {
        var_r1 = 0;
    }
    temp_r1 = (s32)(var_r1 + ((u32)(var_r1 >> 2) >> 0x1D)) >> 3;
    if (arg2 != 0) {
        var_r4 = (0x1E * temp_r1) + 4;
    } else {
        var_r4 = (0x1E * temp_r1) + 7;
    }
    ov96_021EB630(var_r5->unk70, var_r4);
    ov96_021EB630(var_r5->unk78, var_r4 + 1);
    ov96_021EB630(var_r5->unk6C, var_r4 + 1);
    ov96_021EB630(var_r5->unk74, var_r4 + 2);
    for (var_r6 = 0; var_r6 < 3; var_r6++) {
        ov96_021EABA8(arg0->unk00[var_r6], var_r4 + 2);
    }
}
