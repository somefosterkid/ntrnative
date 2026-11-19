#include "global.h"

#include "gf_3d_render.h"
#include "overlay_01_021F1348.h"
#include "unk_0206979C.h"

void sub_0206998C(NNSG3dRenderObj *, void *);

void sub_02069998(NNSG3dRenderObj *param_1, Simple3DModel *param_2, void *param_3);

void sub_02069998(NNSG3dRenderObj *param_1, Simple3DModel *param_2, void *param_3) {
    sub_02069978(param_1, param_2);
    sub_0206998C(param_1, param_3);
    return;
}

void sub_020699AC(NNSG3dRenderObj *param_1, VecFx32 *param_2, VecFx32 *param_3, MtxFx33 *param_4);

void sub_020699AC(NNSG3dRenderObj *param_1, VecFx32 *param_2, VecFx32 *param_3, MtxFx33 *param_4) {
    GF3dRender_DrawModel(param_1, param_2, param_4, param_3);
    return;
}
