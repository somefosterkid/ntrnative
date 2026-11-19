#include "global.h"

s8 sub_020132E8(s32 arg0, s32 arg1);

s8 sub_020132E8(s32 arg0, s32 arg1) {
    s8 sp0[4];

    if (arg0 == 0) {
        if (arg1 == 0) {
            arg1 = *((u8 *)REG_WININ_ADDR);
            sp0[0] = arg1;
        } else {
            arg1 = *((u8 *)REG_DB_WININ_ADDR);
            sp0[0] = arg1;
        }
    } else if (arg1 == 0) {
        arg1 = *((u8 *)REG_WININ_ADDR + 1);
        sp0[0] = arg1;
    } else {
        arg1 = *((u8 *)REG_DB_WININ_ADDR + 1);
        sp0[0] = arg1;
    }
    return *(s8 *)&sp0[0];
}
