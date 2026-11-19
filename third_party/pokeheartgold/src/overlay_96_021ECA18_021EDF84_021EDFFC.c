#include "global.h"

#include "error_handling.h"
#include "math_util.h"

extern u32 ov96_0221B124;

void ov96_021EDFFC(u8 *arg0, u32 *arg1);

void ov96_021EDFFC(u8 *arg0, u32 *arg1) {
    s32 var_r4;
    u32 *var_r5;
    u32 *var_r7;
    u32 temp_r0;
    var_r5 = &ov96_0221B124;
    var_r7 = arg1;
    var_r4 = 0;
    do {
        if ((*var_r5) == 0) {
            GF_AssertFail();
        }
        *((u8 *)(arg0 + var_r4)) = MTRandom() % 3U;
        temp_r0 = *var_r7;
        if (temp_r0 != 0) {
            *((u8 *)(arg0 + var_r4)) = (*((u8 *)(arg0 + var_r4))) + (temp_r0 / ((u32)(*var_r5)));
        }
        var_r4 += 1;
        var_r5 += 1;
        var_r7 += 1;
    } while (var_r4 < 0x1D);
}
