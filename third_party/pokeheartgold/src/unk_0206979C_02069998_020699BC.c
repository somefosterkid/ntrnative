#include "global.h"

#include "fx_util.h"

void sub_020699AC(NNSG3dRenderObj *param_1, VecFx32 *param_2, VecFx32 *param_3, MtxFx33 *param_4);

extern u32 _020FE8A4;

extern u32 _020FE8B0;

void sub_020699BC(void *a0, VecFx32 *a1);

void sub_020699BC(void *a0, VecFx32 *a1) {
    sub_020699AC((NNSG3dRenderObj *)a0, a1, (VecFx32 *)&_020FE8A4, (MtxFx33 *)&_020FE8B0);
    return;
}

void sub_020699D0(NNSG3dRenderObj *param_1, VecFx32 *param_2, VecFx32 *param_3, u16 *param_4);

void sub_020699D0(NNSG3dRenderObj *param_1, VecFx32 *param_2, VecFx32 *param_3, u16 *param_4) {
    MtxFx33 MStack_38;

    MTX_Rot33Angles(&MStack_38, *param_4, param_4[1], param_4[2]);
    sub_020699AC(param_1, param_2, param_3, &MStack_38);
    return;
}
