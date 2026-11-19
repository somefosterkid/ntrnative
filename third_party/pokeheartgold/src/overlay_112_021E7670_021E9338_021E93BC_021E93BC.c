#include "global.h"

s32 ov112_021E935C(void *, s32, s32, s32);

s32 ov112_021E93BC(void *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

s32 ov112_021E93BC(void *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 sp14;
    s32 sp10;
    s32 spC;
    void *sp8;
    s32 sp4;
    void *sp0;
    s32 temp_r0;
    s32 new_var;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r1;
    s32 temp_r3;
    s32 var_r6;
    s32 new_var2;
    s32 var_r4;
    s32 var_r7;
    void *temp_r5;
    sp0 = arg0;
    sp4 = arg1;
    temp_r3 = arg6 * arg7;
    sp10 = 0;
    MIi_CpuClearFast(0U, arg3, (temp_r3 / 8) * 2);
    sp8 = arg3;
    sp14 = 0;
    temp_r0 = ((s32)(arg7 + (((u32)(arg7 >> 2)) >> 0x1D))) >> 3;
    spC = temp_r0;
    if (temp_r0 > 0) {
        var_r7 = sp14;
        new_var = arg4;
        do {
            var_r6 = 0;
            if (arg6 > 0) {
                do {
                    var_r4 = 0;
                    temp_r5 = sp8 + sp10;
loop_4:
                    temp_r0_2 = ov112_021E935C(sp0, sp4, var_r6 + new_var, (arg5 + var_r4) + (new_var2 = var_r7));

                    *((u8 *)temp_r5) = (u8)((*((u8 *)temp_r5)) | ((temp_r0_2 >> 1) << var_r4));
                    temp_r0_3 = temp_r0_2;
                    temp_r0_3 = (temp_r0_3 & 1) << var_r4;
                    *((u8 *)(((u8 *)temp_r5) + 1)) = (u8)(((temp_r0_2 & 1) << var_r4) | (*((u8 *)(((u8 *)temp_r5) + 1))));
                    var_r4 += 1;
                    if (var_r4 < 8) {
                        goto loop_4;
                    }
                    var_r6 += 1;
                    sp10 += 2;
                } while (var_r6 < arg6);
            }
            var_r7 += 8;
            temp_r1 = sp14 + 1;
            sp14 = temp_r1;
        } while (temp_r1 < spC);
    }
    return sp10;
}
