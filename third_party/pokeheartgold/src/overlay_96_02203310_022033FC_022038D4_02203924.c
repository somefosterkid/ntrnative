#include "global.h"

extern s32 ov96_0221C95C;

s32 ov96_02203924(void *arg0, void *arg1);

s32 ov96_02203924(void *arg0, void *arg1) {
    s32 *var_r4;
    s32 temp_r2;
    s32 temp_r3;
    s32 var_r2;
    u8 temp_r0;
    u8 temp_r1;
    u8 var_r5;
    void *temp_r3_2;

    temp_r3 = *(s32 *)((u8 *)arg1 + 4);
    var_r5 = (u8) * (u16 *)arg0;
    temp_r2 = *(s32 *)((u8 *)arg0 + 4);
    if (temp_r2 > temp_r3) {
        var_r2 = -1;
    } else if (temp_r2 < temp_r3) {
        var_r2 = 1;
    } else {
        var_r4 = &ov96_0221C95C;
        temp_r3_2 = ((u8 *)&ov96_0221C95C) + (var_r5 * 0xC);
        temp_r1 = *(u8 *)(temp_r3_2 + *(u16 *)((u8 *)arg1 + 2));
        temp_r0 = *(u8 *)(temp_r3_2 + *(u16 *)((u8 *)arg0 + 2));
        if ((u32)temp_r0 < (u32)temp_r1) {
            var_r2 = -1;
        } else if ((u32)temp_r0 > (u32)temp_r1) {
            var_r2 = 1;
        } else {
            var_r2 = 0;
        }
    }
    return var_r2;
}
