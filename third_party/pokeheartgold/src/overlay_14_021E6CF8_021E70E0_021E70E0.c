#include "global.h"

s32 ov14_021E6B48();

void ov14_021E70E0(void *arg0, s32 arg1);

void ov14_021E70E0(void *arg0, s32 arg1) {
    void *var_r4;
    s32 temp_r0;
    u32 var_r2;
    u32 var_r5;
    void *temp_r1;
    void *temp_r7;
    void *var_r3;
    s32 sp0;
    sp0 = arg1;
    temp_r7 = *((void **)(((u8 *)(*((void **)(((u8 *)arg0) + 0x34)))) + 0xC));
    var_r2 = 0;
    sp0 = arg1 - 0x1E;
    var_r3 = temp_r7;
    do {
        temp_r0 = var_r2 + 0x1E;
        *((s32 *)(((u8 *)var_r3) + 4)) = temp_r0;
        *((s32 *)(((u8 *)var_r3) + 8)) = temp_r0;
        *((s32 *)(((u8 *)var_r3) + 0xC)) = 0;
        var_r2 += 1;
        var_r3 += 0x20;
    } while (var_r2 < 6U);
    *((s32 *)(((u8 *)(temp_r7 + (var_r2 << 5))) + 0xC)) = 0;
    var_r5 = sp0 + 1;
    if (var_r5 < 6U) {
        var_r4 = temp_r7 + (var_r5 << 5);
        do {
            *((s32 *)(((u8 *)var_r4) + 8)) = (s32)(var_r5 + 0x1D);
            *((s32 *)(((u8 *)var_r4) + 0xC)) = 1;
            ov14_021E6B48(arg0, var_r4);
            var_r5 += 1;
            var_r4 += 0x20;
        } while (var_r5 < 6U);
    }
    temp_r1 = temp_r7 + (sp0 << 5);
    *((s32 *)(((u8 *)temp_r1) + 8)) = 0x23;
    *((s32 *)(((u8 *)(temp_r7 + ((long long)(sp0 << 5)))) + 0xC)) = 1;
    ov14_021E6B48(arg0);
}
