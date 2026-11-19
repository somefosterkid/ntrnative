#include "global.h"

fx32 ov96_021FF67C(VecFx32 *param_1, VecFx32 *param_2, VecFx32 *param_3, s32 param_4);

fx32 ov96_021FF67C(VecFx32 *param_1, VecFx32 *param_2, VecFx32 *param_3, s32 param_4) {
    fx32 fVar1;
    fx32 fVar2;
    fx32 fVar3;
    s32 denom;
    VecFx32 VStack_20;

    VEC_Subtract(param_2, param_1, &VStack_20);
    fVar1 = VEC_Mag(&VStack_20);
    denom = fVar1 * fVar1;
    fVar1 = VEC_DotProduct(&VStack_20, param_3);
    fVar2 = VEC_Mag(param_3);
    fVar2 = FX_Sqrt(fVar1 - denom * (fVar2 * fVar2 - param_4 * param_4));
    fVar3 = FX_Div(fVar2 - fVar1, denom);
    fVar1 = FX_Div(-(fVar1 + fVar2), denom);
    if (fVar3 <= fVar1) {
        fVar1 = fVar3;
    }
    return fVar1;
}
