#include "global.h"

s32 _ll_mul(s32, s32, s32, s32);

void ov96_021FF72C(VecFx32 *arg0, VecFx32 *arg1, VecFx32 *arg2);

void ov96_021FF72C(VecFx32 *arg0, VecFx32 *arg1, VecFx32 *arg2) {
    s32 var_r4;
    int new_var;
    VecFx32 *var_r5;
    var_r5 = arg2;
    new_var = 0x80000000;
    var_r5->x = (s32)(arg0->x + arg1->x);
    var_r4 = arg1->y;
    new_var = arg0->y;
    if (FX32_CAST(((((fx64)new_var) * var_r4) + 0x800LL) >> 12) >= 0) {
        var_r5->y = (s32)(new_var + var_r4);
        return;
    }
    var_r5->y = var_r4;
}
