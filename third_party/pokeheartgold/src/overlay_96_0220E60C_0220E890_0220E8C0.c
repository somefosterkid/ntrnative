#include "global.h"

#include "error_handling.h"

s32 ov96_0220E888(u32);

s32 ov96_0220E8C0(void *arg0, s32 arg1, f32 *arg2);

s32 ov96_0220E8C0(void *arg0, s32 arg1, f32 *arg2) {
    s32 sp4;
    f32 *sp0;
    f32 var_r0;
    s32 temp_r0_3;
    int new_var;
    s32 temp_r4;
    s32 temp_r4_2;
    s32 var_r6 = 0;
    f32 var_r7;
    u8 temp_r0;
    u8 temp_r0_2;
    f32 one = 1.0f;
    sp0 = arg2;
    var_r7 = one;
    if (arg0 == 0L) {
        GF_AssertFail();
    }
    temp_r0 = *((u8 *)arg0);
    sp4 = (s32)temp_r0;
    temp_r4 = ov96_0220E888(arg1 - temp_r0);
    temp_r4_2 = temp_r4 - ov96_0220E888(temp_r0);
    if (temp_r4_2 != 0) {
        temp_r0_2 = *((u8 *)(((u8 *)arg0) + 1));
        if (temp_r0_2 != 0) {
            var_r0 = 0.5f + ((f32)(temp_r0_2 << 0xC));
        } else {
            var_r0 = ((f32)(temp_r0_2 << 0xC)) - 0.5f;
        }
        temp_r0_3 = (s32)var_r0;
        if (temp_r0_3 != 0) {
            var_r6 = (temp_r0_3 / 10) * temp_r4_2;
            new_var = 0xFFFE8000;
            if (var_r6 <= new_var) {
                var_r6 = -0x18000;
            }
            var_r7 = one - (((f32)(((s32)(var_r6 - 0x4000)) / 100)) / 4096.0f);
        }
    }
    *sp0 = var_r7;
    return var_r6 >> 0xC;
}
