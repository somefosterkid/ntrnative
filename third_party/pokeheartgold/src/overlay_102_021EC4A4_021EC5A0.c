#include "global.h"

extern s32 ov102_021EC820;

u32 ov102_021EC5A0(u32 arg0, u32 arg1);

u32 ov102_021EC5A0(u32 arg0, u32 arg1) {
    u32 var_r3;
    u8 temp_r2;
    u8 temp_r2_2;
    s32 *var_r4;

    var_r4 = &ov102_021EC820;
    var_r3 = 0;
loop_1:
    temp_r2 = *(u8 *)((u8 *)var_r4 + 2);
    if (((u32)temp_r2 <= arg0) && (arg0 < (u32)(temp_r2 + 0x10))) {
        temp_r2_2 = *(u8 *)((u8 *)var_r4 + 3);
        if (((u32)temp_r2_2 <= arg1) && (arg1 < (u32)(temp_r2_2 + 0x10))) {
            return var_r3;
        }
    }
    var_r3 += 1;
    var_r4 = (void *)((u8 *)var_r4 + (0xA));
    if (var_r3 >= 0x1BU) {
        return -1U;
    }
    goto loop_1;
}
