#include "global.h"

void ov96_021F208C(s32 param_1, s32 param_2, VecFx32 *param_3, s32 param_4, s32 *param_5);

void ov96_021F208C(s32 param_1, s32 param_2, VecFx32 *param_3, s32 param_4, s32 *param_5) {
    fx32 fVar1;
    VecFx32 VStack_20;
    VStack_20.x = param_1 << 0xc;
    VStack_20.y = param_2 << 0xc;
    VStack_20.z = 0;
    VEC_Subtract(&VStack_20, param_3, &VStack_20);
    fVar1 = VEC_Mag(&VStack_20);
    if (fVar1 > param_4) {
        *param_5 = param_1 << 0xc;
        param_5[1] = param_2 << 0xc;
        param_5[2] = 0;
    }
    return;
}
