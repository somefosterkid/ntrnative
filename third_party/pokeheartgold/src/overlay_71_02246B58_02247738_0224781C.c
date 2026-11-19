#include "global.h"

#include "filesystem_files_def.h"
#include "overlay_71_trade3dmodel.h"

struct Camera;

struct Trade3DModel;

typedef struct CameraAngle {
    u16 x;
    u16 y;
    u16 z;
    u16 padding_06;
} CameraAngle;

typedef struct Camera Camera;

void Camera_AdjustAngleTarget(const CameraAngle *angle, Camera *camera);

typedef struct Trade3DScene {
    Camera *camera;
    VecFx32 target;
    CameraAngle cameraAngle;
    Trade3DModel *models;
    u32 modelCount;
} Trade3DScene;

void ov71_0224781C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle);

void ov71_0224781C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle) {
    Camera_AdjustAngleTarget(cameraAngle, tradeScene->camera);
}

void Camera_ApplyPerspectiveType(const CameraAngle *angle, Camera *camera);

void ov71_0224782C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle);

void ov71_0224782C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle) {
    Camera_ApplyPerspectiveType(cameraAngle, tradeScene->camera);
}

void Camera_SetPerspectiveAngle(const CameraAngle *angle, Camera *camera);

void ov71_0224783C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle);

void ov71_0224783C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle) {
    Camera_SetPerspectiveAngle(cameraAngle, tradeScene->camera);
}
