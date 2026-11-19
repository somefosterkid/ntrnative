#include "global.h"

extern u32 ov96_0221CF1C;

s32 ov96_0220FF68(u32 arg0);

s32 ov96_0220FF68(u32 arg0) {
    s32 var_r2;
    u32 *var_r3;

    var_r3 = &ov96_0221CF1C;
    var_r2 = 0;
loop_1:
    if ((u32)*var_r3 <= arg0) {
        return var_r2;
    }
    var_r2 += 1;
    var_r3 += 1;
    if (var_r2 >= 4) {
        return -1;
    }
    goto loop_1;
}
