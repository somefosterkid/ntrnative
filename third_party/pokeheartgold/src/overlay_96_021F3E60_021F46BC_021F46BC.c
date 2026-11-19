#include "global.h"

s32 ov96_021F46BC(u8 *arg0, s32 arg1);

s32 ov96_021F46BC(u8 *arg0, s32 arg1) {
    u8 sp0[9];
    s32 var_r0;
    s32 var_r2;
    u8 *var_r2_2;
    u8 *var_r3;
    void *temp_r7;

    var_r2 = 0;
    var_r3 = sp0;
    do {
        if (*(u8 *)(arg1 + var_r2) != 0) {
            temp_r7 = (void *)(arg0 + var_r2);
            if (*(u8 *)((u8 *)temp_r7 + 8) == 0) {
                *(u8 *)((u8 *)temp_r7 + 8) = 1U;
                *var_r3 = 1;
            } else {
                goto block_4;
            }
        } else {
block_4:
            *var_r3 = 0;
        }
        var_r2 += 1;
        var_r3 += 1;
    } while (var_r2 < 0xC);
    var_r0 = 0;
    var_r2_2 = sp0;
loop_7:
    if (*var_r2_2 == 0) {
        var_r0 += 1;
        var_r2_2 += 1;
        if (var_r0 >= 0xC) {
            var_r0 = 0xC;
        } else {
            goto loop_7;
        }
    }
    return var_r0;
}
