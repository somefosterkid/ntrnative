#include "global.h"

#include "overlay_96_ov96_rower_work.h"

void ov96_021FF0BC(Ov96RowerWork *arg0, s32 arg1, void **arg2, void **arg3, u8 *arg4, u8 *arg5);

void ov96_021FF0BC(Ov96RowerWork *arg0, s32 arg1, void **arg2, void **arg3, u8 *arg4, u8 *arg5) {
    s32 sp10;
    s32 spC;
    s32 temp_r4;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r7;
    u8 temp_r2;
    u8 temp_r2_2;

    for (var_r0 = 0; var_r0 < 0x1E; var_r0++) {
        arg2[var_r0] = NULL;
        arg3[var_r0] = NULL;
    }
    *arg4 = 0;
    *arg5 = 0;
    sp10 = arg1 / 4096;
    for (var_r7 = 0; var_r7 < 0x1E; var_r7++) {
        if (arg0->obstacles[var_r7].unk0 == 0) {
            break;
        }
        spC = (s32)(u8)arg0->unk45C[var_r7];
        temp_r4 = sp10 - arg0->obstacles[var_r7].unk8;
        if ((double)temp_r4 >= 768.0) {
            var_r0_2 = (s32)(80.0 + (1024.0 - (double)temp_r4));
        } else if ((double)temp_r4 <= -768.0) {
            var_r0_2 = (s32)(80.0 - (1024.0 + (double)temp_r4));
        } else {
            var_r0_2 = 0x50 - temp_r4;
        }
        if ((var_r0_2 >= -0x20) && (var_r0_2 <= 0x120)) {
            if (spC == 3) {
                temp_r2 = *arg5;
                *arg5 = temp_r2 + 1;
                arg3[temp_r2] = &arg0->unk45C[var_r7];
            } else {
                temp_r2_2 = *arg4;
                *arg4 = temp_r2_2 + 1;
                arg2[temp_r2_2] = &arg0->unk45C[var_r7];
            }
        }
    }
}
