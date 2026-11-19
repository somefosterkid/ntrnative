#include "global.h"
#include "sys_task.h"
#include "overlay_71_trade3dmodel.h"

typedef struct UnkStruct_ov71_0224A684 {
    u8 unk_0[0x4];
    s32 unk_4;
} UnkStruct_ov71_0224A684;

s32 ov71_0224A684(UnkStruct_ov71_0224A684 *arg0);

struct Trade3DModel;
struct Trade3DScene;
typedef struct Trade3DScene Trade3DScene;
typedef struct BallArrivalPathState {
    void *arrivalPhase;
    SysTask *task;
    Trade3DScene *scene;
    Trade3DModel *model;
    VecFx32 orbitCenter;
    VecFx32 orbitOffset;
    VecFx32 modelPos;
    VecFx16 rotation;
    fx32 orbitCenterStepY;
    fx32 orbitCenterStepZ;
    fx32 orbitRadiusX;
    fx32 orbitRadiusY;
    fx32 orbitRadiusXStep;
    fx32 orbitRadiusYStep;
    fx32 targetOrbitRadiusX;
    fx32 targetOrbitRadiusY;
    fx32 scale;
    fx32 scaleStep;
    fx32 targetScale;
    int transitionFrames;
    int subState;
    int exitTimer;
    int frameCount;
    fx16 rotationAngle;
    fx16 rotationSpeed;
} BallArrivalPathState;
void ov71_0224A694(BallArrivalPathState *bapState, fx32 targetRadiusX, fx32 targetRadiusY, fx32 targetScale, int duration);

s32 ov71_0224A684(UnkStruct_ov71_0224A684 *arg0) {
    if (arg0->unk_4 == 0) {
        return 1;
    }
    return 0;
}

void ov71_0224A694(BallArrivalPathState *bapState, fx32 targetRadiusX, fx32 targetRadiusY, fx32 targetScale, int duration) {
    if (duration) {
        bapState->orbitRadiusXStep = (targetRadiusX - bapState->orbitRadiusX) / duration;
        bapState->orbitRadiusYStep = (targetRadiusY - bapState->orbitRadiusY) / duration;
        bapState->scaleStep = (targetScale - bapState->scale) / duration;
        bapState->targetOrbitRadiusX = targetRadiusX;
        bapState->targetOrbitRadiusY = targetRadiusY;
        bapState->targetScale = targetScale;
        bapState->transitionFrames = duration;
    }
}
