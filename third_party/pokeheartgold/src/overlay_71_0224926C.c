#include "global.h"

#include "camera.h"
#include "sys_task.h"
#include "overlay_71_ball_path_state.h"
#include "overlay_71_trade3dmodel.h"

struct Trade3DModel;

struct Trade3DScene;

typedef struct Trade3DScene Trade3DScene;

void ov71_02247704(Trade3DModel *tradeModel, BOOL enabled);

void ov71_02247730(Trade3DModel *tradeModel, fx32 scale);

void ov71_0224780C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle);

void ov71_0224782C(Trade3DScene *tradeScene, u8 projectionType);

typedef struct AxisRotAnimator {
    fx16 currentValue;
    fx16 step;
    fx16 targetValue;
    u16 framesRemaining;
} AxisRotAnimator;

void ov71_02248FC8(BallPathState *bpState, fx32 targetRadiusX, fx32 targetRadiusZ, int duration);

void ov71_02249004(BallPathState *bpState, int axis, fx32 targetValue, int duration);

void ov71_022492FC(SysTask *task, void *param);

void ov71_0224926C(SysTask *task);

s32 ov71_022476C4(Trade3DModel *, void *);
s32 ov71_022476EC(void *, void *);
s32 ov71_0224903C(void *);
void ov71_02249358(SysTask *task, void *data);
void ov71_022492FC(SysTask *arg0, void *arg1);
void ov71_022493B8(SysTask *task, void *data);
void ov71_02249414(SysTask *task, void *data);
void ov71_02249460(SysTask *task, void *data);

void ov71_0224926C(SysTask *task) {
    BallPathState *bpState = SysTask_GetData(task);
    CameraAngle cameraAngle;

    bpState->exitedView = 0;
    cameraAngle.x = cameraAngle.y = cameraAngle.z = 0;

    ov71_0224782C(bpState->scene, 1);
    ov71_0224780C(bpState->scene, &cameraAngle);

    bpState->yStep = (0xFFFEF000 - (-128 << 12)) / 20;
    bpState->currentY = (-128 << 12) + bpState->yStep;
    bpState->entryFramesRemaining = 20;
    bpState->currentPos.y = bpState->currentY;

    ov71_02248FC8(bpState, 0x0, 0x0, 20);
    ov71_02249004(bpState, 1, -(bpState->rotationSpeed), 30);
    ov71_02249004(bpState, 2, 0x100, 50);
    ov71_02247730(bpState->model, 8192);
    ov71_02247704(bpState->model, 1);

    SysTask_SetFunc(task, ov71_022492FC);
}

void ov71_022492FC(SysTask *arg0, void *arg1) {
    s32 v0;

    ov71_0224903C(arg1);
    if (*((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) != 0) {
        v0 = *((s32 *)&((BallPathState *)arg1)->currentY) + *((s32 *)&((BallPathState *)arg1)->yStep);
        *((s32 *)&((BallPathState *)arg1)->currentY) = v0;
        *((s32 *)&((BallPathState *)arg1)->currentPos.y) = v0;
        *((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) = (s32)(*((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) - 1);
        if (*((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) == 0) {
            SysTask_SetFunc(arg0, ov71_02249358);
        }
    }
    ov71_022476C4(*(void **)arg1, &((BallPathState *)arg1)->currentPos);
    ov71_022476EC(*(void **)arg1, &((BallPathState *)arg1)->rotation);
}

void ov71_02249358(SysTask *arg0, void *arg1) {
    ov71_0224903C(arg1);
    *((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) = (s32)(*((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) + 1);
    if ((s32) *((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) > 0x6E) {
        *((s32 *)&((BallPathState *)arg1)->yStep) = (s32)((s32)(0xFFFC5800 - *((s32 *)&((BallPathState *)arg1)->currentY)) / 53);
        *((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) = 0x35;
        SysTask_SetFunc(arg0, ov71_022493B8);
    }
    ov71_022476C4(*(void **)arg1, &((BallPathState *)arg1)->currentPos);
    ov71_022476EC(*(void **)arg1, &((BallPathState *)arg1)->rotation);
}

void ov71_022493B8(SysTask *arg0, void *arg1) {
    s32 v0;

    ov71_0224903C(arg1);
    if (*((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) != 0) {
        v0 = *((s32 *)&((BallPathState *)arg1)->currentY) + *((s32 *)&((BallPathState *)arg1)->yStep);
        *((s32 *)&((BallPathState *)arg1)->currentY) = v0;
        *((s32 *)&((BallPathState *)arg1)->currentPos.y) = v0;
        *((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) = (s32)(*((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) - 1);
        if (*((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) == 0) {
            SysTask_SetFunc(arg0, ov71_02249414);
        }
    }
    ov71_022476C4(*(void **)arg1, &((BallPathState *)arg1)->currentPos);
    ov71_022476EC(*(void **)arg1, &((BallPathState *)arg1)->rotation);
}

void ov71_02249414(SysTask *arg0, void *arg1) {
    ov71_0224903C(arg1);
    *((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) = (s32)(*((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) + 1);
    if ((s32) *((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) > 1) {
        *((s32 *)&((BallPathState *)arg1)->yStep) = 0x20000;
        SysTask_SetFunc(arg0, ov71_02249460);
    }
    ov71_022476C4(*(void **)arg1, &((BallPathState *)arg1)->currentPos);
    ov71_022476EC(*(void **)arg1, &((BallPathState *)arg1)->rotation);
}
