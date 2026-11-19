#include "global.h"

extern u16 ov07_02236864;

void ov07_02231A20(s32 arg0, s32 arg1, void *arg2);

void ov07_02231A20(s32 arg0, s32 arg1, void *arg2) {
    s32 sp0[6];
    u32 var_r3;
    u8 *var_r4;
    int new_var;
    u16 *var_r5;
    u16 temp_r1;
    var_r5 = &ov07_02236864;
    var_r4 = (u8 *)&sp0[0];
    var_r3 = 0xC;
    new_var = 0;
    do {
        temp_r1 = *var_r5;
        var_r5 += 1;
        *((u16 *)var_r4) = temp_r1;
        var_r4 += 2;
        var_r3 -= 1;
    } while (var_r3 != 0);
    *((u16 *)arg2) = (u16)(*((u16 *)(&(&sp0[0])[arg0])));
    *((u16 *)(((u8 *)arg2) + 2)) = (u16)(*((u16 *)(((u8 *)(&(&sp0[new_var])[arg0])) + 2)));
}
