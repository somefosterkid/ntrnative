#include "global.h"

extern s32 _020F5730;

int GBSounds_GetDSSeqNoByGBSeqNo(u16 arg0);

int GBSounds_GetDSSeqNoByGBSeqNo(u16 arg0) {
    s32 new_var;
    s32 *new_var2;
    s32 temp_r3;
    u16 var_r4;
    var_r4 = 0;
loop_1:
    temp_r3 = var_r4;

    new_var = temp_r3;
    if (arg0 == (*((u16 *)(((u8 *)((new_var2 = &_020F5730) + new_var)) + 2)))) {
        return *((u16 *)((&_020F5730) + new_var));
    }
    var_r4 += 1;
    if (((u32)var_r4) >= 0x88U) {
        return arg0;
    }
    goto loop_1;
}
