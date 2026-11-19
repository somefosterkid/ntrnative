#include "global.h"

#include "filesystem_files_def.h"
#include "camera.h"
#include "unk_0201F990.h"
#include "overlay_71_trade3dmodel.h"

void ov71_02247708(Trade3DModel *tradeModel, int alpha);

void ov71_02247730(Trade3DModel *tradeModel, fx32 scale);

static void inline inline_Trade3DModel_BuildRotationMatrix(MtxFx33 *matrix, const VecFx16 *rotation);
BOOL ov71_02247738(Trade3DModel *tradeModel);

void ov71_02247708(Trade3DModel *tradeModel, int alpha) {
    tradeModel->alpha = alpha;

    if (alpha != 31) {
        NNS_G3dMdlUseGlbAlpha(tradeModel->model);
    } else {
        NNS_G3dMdlUseMdlAlpha(tradeModel->model);
    }
}

void ov71_02247730(Trade3DModel *tradeModel, fx32 scale) {
    tradeModel->scale.x = scale;
    tradeModel->scale.y = scale;
    tradeModel->scale.z = scale;
}

static void inline inline_Trade3DModel_BuildRotationMatrix(MtxFx33 *matrix, const VecFx16 *rotation) {
    MtxFx33 tempMatrix;

    MTX_Identity33(matrix);
    MTX_RotX33(&tempMatrix, FX_SinIdx((u16)(rotation->x)), FX_CosIdx((u16)(rotation->x)));
    MTX_Concat33(&tempMatrix, matrix, matrix);
    MTX_RotY33(&tempMatrix, FX_SinIdx((u16)(rotation->y)), FX_CosIdx((u16)(rotation->y)));
    MTX_Concat33(&tempMatrix, matrix, matrix);
    MTX_RotZ33(&tempMatrix, FX_SinIdx((u16)(rotation->z)), FX_CosIdx((u16)(rotation->z)));
    MTX_Concat33(&tempMatrix, matrix, matrix);
}

BOOL ov71_02247738(Trade3DModel *tradeModel) {
    MtxFx33 rotMatrix;
    BOOL result;

    inline_Trade3DModel_BuildRotationMatrix(&rotMatrix, &(tradeModel->rotation));

    NNS_G3dGePushMtx();

    Camera_PushLookAtToNNSGlb();
    result = sub_0201F990(tradeModel->model, &tradeModel->position, &rotMatrix, &tradeModel->scale);

    NNS_G3dGePopMtx(1);

    return result;
}
