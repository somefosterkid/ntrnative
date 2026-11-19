#include "global.h"

void ov96_021EB0A4(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);

void ov96_021EB0A4(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    int new_var4;
    s32 temp_r1;
    s32 var_r4;
    u32 new_var2;
    u8 *new_var3;
    u32 temp_r3;
    s32 new_var;
    new_var3 = (u8 *)arg0;
    var_r4 = arg2 + *((s32 *)(new_var3 + 0x10));
    new_var2 = 0x28;
    new_var = *((s32 *)(new_var3 + 0x18));
    new_var4 = new_var * 2;
    *arg3 = arg1;
    temp_r1 = *((s32 *)(new_var3 + new_var2));
    temp_r3 = ((*((s32 *)(new_var3 + 0x18))) * 2) - temp_r1;
    *arg4 = temp_r1 + ((var_r4 - new_var4) + (((s32)(temp_r3 + (temp_r3 >> 0x1F))) >> 1));
}
