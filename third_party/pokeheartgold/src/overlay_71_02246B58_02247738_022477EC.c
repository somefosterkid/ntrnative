#include "global.h"

#include "camera.h"
#include "filesystem_files_def.h"
#include "overlay_71_trade3dmodel.h"

struct Trade3DModel;

typedef struct Trade3DScene {
    Camera *camera;
    VecFx32 target;
    CameraAngle cameraAngle;
    Trade3DModel *models;
    u32 modelCount;
} Trade3DScene;

void ov71_022477EC(Trade3DScene *tradeScene, VecFx32 *outPosition);

void ov71_022477EC(Trade3DScene *tradeScene, VecFx32 *outPosition) {
    *outPosition = Camera_GetLookAtCamPos(tradeScene->camera);
}

void ov71_0224780C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle);

void ov71_0224780C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle) {
    Camera_SetAnglePos(cameraAngle, tradeScene->camera);
}
