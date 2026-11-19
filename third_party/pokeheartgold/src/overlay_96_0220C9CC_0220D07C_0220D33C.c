#include "global.h"

#include "error_handling.h"

extern u8 ov96_0221CE5C[];

s32 ov96_0220D33C(s32 param_1, u32 param_2);

s32 ov96_0220D33C(s32 param_1, u32 param_2) {
    int new_var;
    s32 iVar1;
    new_var = (9 + 1) <= param_2;
    if (new_var) {
        GF_AssertFail();
    }
    iVar1 = param_1 - (*((s32 *)((param_2 * 4) + ((u8 *)ov96_0221CE5C))));
    if (iVar1 <= (1 - 1)) {
        iVar1 = 0;
    }
    return iVar1;
}
