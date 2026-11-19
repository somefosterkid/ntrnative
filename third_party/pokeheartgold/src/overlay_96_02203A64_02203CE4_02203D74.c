#include "global.h"

s32 ov96_021E634C(void *, s32, s32, s32, s32, u8, s32 *);

void *ov96_021EB594(void *);

void ov96_02203D74(void *arg0, s32 arg1, s32 arg2);

void ov96_02203D74(void *arg0, s32 arg1, s32 arg2) {
    s32 sp18[3];
    u8 *new_var[2];
    s32 sp10;
    s32 spC;
    s32 temp_r2;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 var_r6;
    void *temp_r0;
    void *var_r4;
    new_var[1] = (u8 *)sp18;
    spC = arg1;
    sp10 = arg2;
    var_r6 = 0;
    var_r4 = arg0;
    new_var[0] = new_var[1];
    do {
        temp_r0 = ov96_021EB594(*((void **)(((u8 *)var_r4) + 0x54)));
        temp_r1 = *((s32 *)temp_r0);
        temp_r2 = temp_r1 / 4096;
        *((s16 *)new_var[1]) = (s16)temp_r2;
        temp_r1_2 = *((s32 *)(((u8 *)temp_r0) + 4));
        *((s16 *)(((u8 *)new_var[1]) + 2)) = (s16)(temp_r1_2 / 4096);
        ov96_021E634C(*((void **)(((u8 *)arg0) + 4)), 0, sp10, spC, 2, 3, (s32 *)new_var[0]);
        var_r4 += 4;
        new_var[1] += 4;
        var_r6 += 1;
    } while (var_r6 < 3);
}
