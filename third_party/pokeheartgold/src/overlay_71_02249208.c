#include "global.h"

#include "sys_task.h"

struct Trade3DModel;

struct Trade3DScene;

typedef struct Trade3DModel Trade3DModel;

typedef struct Trade3DScene Trade3DScene;

void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position);

void ov71_022476EC(Trade3DModel *tradeModel, const VecFx16 *rotation);

void ov71_02247704(Trade3DModel *tradeModel, BOOL enabled);

BOOL ov71_02247738(Trade3DModel *tradeModel);

typedef struct AxisRotAnimator {
    fx16 currentValue;
    fx16 step;
    fx16 targetValue;
    u16 framesRemaining;
} AxisRotAnimator;

typedef struct BallPathState {
    Trade3DModel *model;
    Trade3DScene *scene;
    VecFx32 orbitCenter;
    VecFx32 currentPos;
    VecFx16 rotation;
    u8 pad_26[2];
    fx32 currentY;
    fx32 yStep;
    fx32 yAccel;
    fx32 unused_34;
    fx32 orbitRadiusX;
    fx32 orbitRadiusZ;
    fx32 orbitRadiusXStep;
    fx32 orbitRadiusZStep;
    fx32 targetOrbitRadiusX;
    fx32 targetOrbitRadiusZ;
    u32 orbitTransitionFrames;
    s16 angleAccum;
    s16 rotationSpeed;
    s16 targetRotationSpeed;
    s16 rotationSpeedStep;
    AxisRotAnimator axisRotAnimators[3];
    u32 rotSpeedTransitionFrames;
    BOOL exitedView;
    BOOL midpointReached;
    int entryFramesRemaining;
} BallPathState;

void ov71_0224903C(BallPathState *bpState);

void ov71_02249208(SysTask *unused, void *param);

s32 ov71_02249254(SysTask *arg0);

s32 ov71_02249260(SysTask *arg0);

void ov71_02249208(SysTask *unused, void *param) {
    BallPathState *bpState = param;

    if (bpState->exitedView == 0) {
        bpState->yStep += bpState->yAccel;
        bpState->currentY += bpState->yStep;
        bpState->currentPos.y = bpState->currentY;

        ov71_0224903C(bpState);
        ov71_022476C4(bpState->model, &(bpState->currentPos));
        ov71_022476EC(bpState->model, &(bpState->rotation));

        if (ov71_02247738(bpState->model) == 0) {
            ov71_02247704(bpState->model, 0);
            bpState->exitedView = 1;
        }
    }
}

s32 ov71_02249254(SysTask *task) {
    return *(s32 *)((u8 *)(SysTask_GetData(task)) + 0x78);
}

s32 ov71_02249260(SysTask *task) {
    return *(s32 *)((u8 *)(SysTask_GetData(task)) + 0x7C);
}
