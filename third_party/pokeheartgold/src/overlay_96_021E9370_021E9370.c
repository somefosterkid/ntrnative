#include "global.h"

#include "error_handling.h"

void *ov96_021E9370(s32 *arg0, u32 arg1);

void *ov96_021E9370(s32 *arg0, u32 arg1) {
    s32 *var_r5;
    u32 var_r4;

    var_r4 = arg1;
    var_r5 = arg0;
    if (var_r4 >= 4U) {
        GF_AssertFail();
    }
    if ((s32)var_r4 >= (s32)*var_r5) {
        GF_AssertFail();
    }
    var_r5 += 9;
    return var_r5 + var_r4 * 6;
}
