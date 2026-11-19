#include "global.h"

void ov96_02200E80(void *arg0, s32 *arg1);

void ov96_02200E80(void *arg0, s32 *arg1) {
    s32 var_r5;
    s32 *var_r1;
    u8 temp_r3;
    u16 temp_r3_2;
    s32 temp_r4_2;
    s32 temp_r4_3;
    s32 temp_r7;
    u8 temp_r4;
    void *var_r0;

    var_r0 = arg0;
    var_r1 = arg1;
    var_r5 = 0;
    do {
        temp_r4 = (u8)*var_r1;
        *(s32 *)((u8 *)var_r0 + 0x4C) = (s32)temp_r4;
        temp_r3 = (u8)(*var_r1 >> 8);
        temp_r3_2 = (u16)(*var_r1 >> 16);
        if (temp_r4 != 0) {
            *(s32 *)((u8 *)var_r0 + 0x40) = 1;
            temp_r4_2 = *(s32 *)((u8 *)var_r0 + 0x4C);
            if (temp_r4_2 == 2) {
                *(s32 *)((u8 *)var_r0 + 0x50) = 0x20;
            } else if (temp_r4_2 == 1) {
                *(s32 *)((u8 *)var_r0 + 0x50) = 0x30;
            } else {
                *(s32 *)((u8 *)var_r0 + 0x50) = 0x40;
            }
            temp_r7 = *(s32 *)((u8 *)var_r0 + 0x50);
            *(s32 *)((u8 *)var_r0 + 0x44) = (s32)(temp_r3_2 - (temp_r7 / 2));
            temp_r4_3 = *(s32 *)((u8 *)var_r0 + 0x50);
            *(s32 *)((u8 *)var_r0 + 0x48) = (s32)((temp_r3 + 0x20) - (temp_r4_3 / 2));
        } else {
            *(s32 *)((u8 *)var_r0 + 0x40) = 0;
        }
        var_r5 += 1;
        var_r1 += 1;
        var_r0 += 0x14;
    } while (var_r5 < 0x1E);
}
