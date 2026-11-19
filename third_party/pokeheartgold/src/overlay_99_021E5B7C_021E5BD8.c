#include "global.h"

#include "error_handling.h"

u16 ov99_021E5BD8(u32 *param_1, s32 param_2, s32 param_3);

u16 ov99_021E5BD8(u32 *param_1, s32 param_2, s32 param_3) {
    u16 *p;
    u32 new_var;
    if (param_1 == 0L) {
        GF_AssertFail();
    }
    if (param_3 == 3) {
        u32 off = param_2 * 0x2c;
        p = (u16 *)(off + (*param_1));
        new_var = *param_1;
        return (u16)(((u16)(((u16)(((u16 *)(off + new_var))[3] - ((u16 *)(new_var + off))[0])) - ((u16 *)(off + (*param_1)))[1])) - p[2]);
    }
    return *((u16 *)(((*param_1) + (param_2 * 0x2c)) + (param_3 * 2)));
}
