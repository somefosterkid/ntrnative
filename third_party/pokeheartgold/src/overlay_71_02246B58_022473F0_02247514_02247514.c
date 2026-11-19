#include "global.h"
#include "camera.h"
#include "overlay_71_trade3dmodel.h"
#include "filesystem_files_def.h"

struct Camera;

struct CameraAngle;

struct CameraPerspective;

struct CameraPositionHistory;

struct Trade3DModel;

struct Trade3DScene;

typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;

typedef struct Trade3DScene {
    Camera *camera;
    VecFx32 target;
    CameraAngle cameraAngle;
    Trade3DModel *models;
    u32 modelCount;
} Trade3DScene;

void GF3dRender_DrawModel(NNSG3dRenderObj *renderObj, const VecFx32 *pos, const MtxFx33 *rot, const VecFx32 *scale);

static void inline inline_Trade3DModel_BuildRotationMatrix(MtxFx33 *matrix, const VecFx16 *rotation);

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

Trade3DModel *Trade3DModel_Load(Trade3DScene *tradeScene, int modelIndex, enum NarcId narcID, u32 memberIndex, fx32 x, fx32 y, fx32 z, BOOL enabled);

void ov71_02247514(Trade3DModel *tradeModel);

void ov71_02247514(Trade3DModel *tradeModel) {
    MtxFx33 mtxFx;

    inline_Trade3DModel_BuildRotationMatrix(&mtxFx, &(tradeModel->rotation));

    if (tradeModel->alpha != 31) {
        NNS_G3dGlbPolygonAttr((int)(0), (GXPolygonMode)(0), (GXCull)(0), 0, tradeModel->alpha, 0);
    }

    GF3dRender_DrawModel(&tradeModel->renderObj, &tradeModel->position, &mtxFx, &tradeModel->scale);
}
