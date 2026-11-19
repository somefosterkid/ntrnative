#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "overlay_120.h"

void ov120_0225F1E8(Ov120Shuffle *shuffle, u32 arg1) {
    u16 *arg0 = shuffle->order;
    u32 sp0;
    s32 temp_r2;
    unsigned int new_var;
    u16 *var_r1;
    u16 *var_r4;
    u16 temp_r0_2;
    u32 temp_r0;
    u32 var_r7;
    new_var = 0U;
    if (arg1 >= 0x80U) {
        GF_AssertFail();
    }
    temp_r2 = 0;
    if (arg1 > new_var) {
        var_r1 = arg0;
        do {
            *var_r1 = temp_r2;
            temp_r2 += 1;
            var_r1 += 1;
        } while (((u32)temp_r2) < arg1);
    }
    var_r7 = 0;
    temp_r0 = arg1 - 1;
    sp0 = temp_r0;
    if (temp_r0 != 0) {
        var_r4 = arg0;
        do {
            temp_r2 = (LCRandom() % arg1);
            temp_r0_2 = *var_r4;
            *var_r4 = *((u16 *)(arg0 + temp_r2));
            *((u16 *)(arg0 + temp_r2)) = temp_r0_2;
            var_r4 += 1;
            var_r7 += 1;
        } while (var_r7 < sp0);
    }
    shuffle->pos = 0;
    shuffle->count = arg1;
}
