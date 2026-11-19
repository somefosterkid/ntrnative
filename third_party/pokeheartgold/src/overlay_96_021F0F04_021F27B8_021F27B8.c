#include "global.h"

s32 _ll_mul(s32, s32, s32, s32);

void ov96_021F27B8(VecFx32 *arg0, VecFx32 *arg1, VecFx32 *arg2);

void ov96_021F27B8(VecFx32 *arg0, VecFx32 *arg1, VecFx32 *arg2) {
    VecFx32 norm0;
    VecFx32 norm1;
    fx32 dot;
    fx32 mag;

    VEC_Normalize(arg0, &norm0);
    VEC_Normalize(arg1, &norm1);
    dot = VEC_DotProduct(&norm0, &norm1);
    mag = VEC_Mag(arg0);
    VecFx32 zero = { 0, 0, 0 };
    VEC_MultAdd(FX32_CAST((((fx64)dot * mag) + 0x800LL) >> 12), &norm1, &zero, arg2);
}
