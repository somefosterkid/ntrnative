#include "global.h"

s32 ov96_021FC164(s32 arg0, s32 arg1, s32 arg2);

s32 ov96_021FC164(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_r2;

    var_r2 = (arg2 / 8);
    if (*(u8 *)((u8 *)(arg0 + (arg1 << 9)) + var_r2 + 0x618) == 0) {
        var_r2 = -1;
    }
    return var_r2;
}
