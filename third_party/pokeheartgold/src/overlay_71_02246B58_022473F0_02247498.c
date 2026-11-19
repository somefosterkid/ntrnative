#include "global.h"

#include "camera.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_71_trade3dmodel.h"

typedef struct Trade3DScene {
    Camera *camera;
    VecFx32 target;
    CameraAngle cameraAngle;
    Trade3DModel *models;
    u32 modelCount;
} Trade3DScene;

void ov71_022475F8(Trade3DModel *tradeModel);

void ov71_022478B8(Trade3DScene *tradeScene);

void ov71_02247498(Trade3DScene *tradeScene);

void ov71_02247498(Trade3DScene *tradeScene) {
    for (int i = 0; i < tradeScene->modelCount; i++) {
        ov71_022475F8(&(tradeScene->models[i]));
    }

    Heap_Free(tradeScene->models);
    ov71_022478B8(tradeScene);
    Heap_Free(tradeScene);
}

void ov71_02247514(Trade3DModel *tradeModel);

void ov71_022474CC(Trade3DScene *tradeScene);

void ov71_022474CC(Trade3DScene *tradeScene) {
    NNS_G3dGePushMtx();
    Camera_PushLookAtToNNSGlb();

    for (int i = 0; i < tradeScene->modelCount; i++) {
        if (tradeScene->models[i].enabled) {
            ov71_02247514(&(tradeScene->models[i]));
        }
    }

    NNS_G3dGePopMtx(1);
}
