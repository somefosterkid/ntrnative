#include "global.h"

u32 ov96_02207990(VecFx32 *param_1, VecFx32 *param_2, s32 param_3, s32 param_4);

u32 ov96_02207990(VecFx32 *param_1, VecFx32 *param_2, s32 param_3, s32 param_4) {
    fx32 fVar1;
    VecFx32 VStack_18;

    VEC_Subtract(param_1, param_2, &VStack_18);
    fVar1 = VEC_Mag(&VStack_18);
    if (fVar1 < param_3 + param_4) {
        return 1;
    }
    return 0;
}
