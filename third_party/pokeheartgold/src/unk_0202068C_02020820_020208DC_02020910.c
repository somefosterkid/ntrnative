#include "global.h"

s32 sub_020208CC(void *);

s32 sub_02020910(void *arg0, u32 arg1);

s32 sub_02020910(void *arg0, u32 arg1) {
    u16 temp_r4;
    u8 *new_var;
    u8 *temp_r2;
    u8 *var_r1;
    if (arg0 != 0L) {
        temp_r2 = (u8 *)(arg0 + 0x3C);
        if ((temp_r2 != 0) && (arg1 < ((u32)(*((u8 *)(((u8 *)arg0) + 0x3D)))))) {
            temp_r4 = *((u16 *)(((u8 *)arg0) + 0x42));
            new_var = temp_r2 + temp_r4;
            var_r1 = (new_var + 4) + ((*((u16 *)new_var)) * arg1);
        } else {
            var_r1 = 0;
        }
    } else {
        var_r1 = 0;
    }
    if (var_r1 == 0) {
        return 0;
    }
    return sub_020208CC(arg0);
}
