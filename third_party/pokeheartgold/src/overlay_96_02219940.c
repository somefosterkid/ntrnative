#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_digit_work.h"

u32 ov96_02219940(s32 param_1, u32 param_2);

s32 ov96_02219DD0(s32, u32, u32);
s32 ov96_02219F50(s32, u32, s32);
void ov96_0221996C(s32 arg0, u32 arg1, u32 arg2, s32 arg3);

u32 ov96_02219940(s32 param_1, u32 param_2) {
    s32 iVar1;
    if ((param_2 == 0) || ((3 + 1) <= param_2)) {
        GF_AssertFail();
    }
    iVar1 = param_1 + 0xbc;
    *((s8 *)(iVar1 + param_2)) = (*((s8 *)(iVar1 + param_2))) + 1;
    if (0xc < (*((s8 *)(iVar1 + param_2)))) {
        *((u8 *)(iVar1 + param_2)) = 0;
        return 1;
    }
    return 0;
}

void ov96_0221996C(s32 arg0, u32 arg1, u32 arg2, s32 arg3) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    if (arg1 >= 4U) {
        GF_AssertFail();
    }
    if (arg2 >= 3U) {
        GF_AssertFail();
    }
    if (arg3 == 0) {
        ov96_02219DD0(arg0, arg1, arg2);
    }
    ov96_02219F50(arg0, arg1, arg3);
}
