#include "global.h"

fx32 ov96_02207390(VecFx32 *param_1, VecFx32 *param_2, VecFx32 *param_3, s32 param_4);

fx32 ov96_02207390(VecFx32 *param_1, VecFx32 *param_2, VecFx32 *param_3, s32 param_4) {
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

void _ffltu(u8);

void ov96_022073F0(void *param_1, s32 param_2);

void ov96_022073F0(void *param_1, s32 param_2) {
    _ffltu(*(u8 *)(param_1 + param_2 * 0x14 + 0x1c));
    return;
}

void ov96_02207400(s32 *param_1, s32 *param_2, s32 *param_3);

void ov96_02207400(s32 *param_1, s32 *param_2, s32 *param_3) {
    *param_3 = *param_1 + *param_2;
    param_3[1] = param_1[1] + param_2[1];
    return;
}
