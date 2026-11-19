#include "global.h"

#include "math_util.h"
#include "sys_task_api.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_trade_ball_arrival_phase.h"

struct Trade3DModel;

struct Trade3DScene;

typedef struct Trade3DScene Trade3DScene;

void ov71_022476EC(Trade3DModel *tradeModel, const VecFx16 *rotation);

void ov71_02247730(Trade3DModel *tradeModel, fx32 scale);

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

extern BOOL _0224C020;

void ov71_0224A6D8(BallArrivalPathState *bapState);

void ov71_0224A6D8(BallArrivalPathState *bapState) {
    if (_0224C020) {
        int angleDegrees;

        angleDegrees = (u16)(bapState->rotationAngle) / (65536 / 360);

        if (angleDegrees >= 360) {
            angleDegrees -= 360;
        }

        bapState->rotationAngle -= bapState->rotationSpeed;

        if (bapState->transitionFrames) {
            bapState->transitionFrames--;

            if (bapState->transitionFrames) {
                bapState->orbitRadiusX += bapState->orbitRadiusXStep;
                bapState->orbitRadiusY += bapState->orbitRadiusYStep;
                bapState->scale += bapState->scaleStep;
            } else {
                bapState->orbitRadiusX = bapState->targetOrbitRadiusX;
                bapState->orbitRadiusY = bapState->targetOrbitRadiusY;
                bapState->scale = bapState->targetScale;
            }

            ov71_02247730(bapState->model, bapState->scale);
        }

        bapState->orbitOffset.x = FX_Mul(GF_CosDegNoWrap(angleDegrees), bapState->orbitRadiusX);
        bapState->orbitOffset.y = FX_Mul(GF_SinDegNoWrap(angleDegrees), bapState->orbitRadiusY);
        bapState->orbitOffset.z = 0;
        bapState->rotation.x -= 0x300;

        ov71_022476EC(bapState->model, &(bapState->rotation));
        VEC_Add(&bapState->orbitOffset, &bapState->orbitCenter, &bapState->modelPos);
    } else {
        bapState->modelPos = bapState->orbitCenter;
    }
}

struct TradeBallArrivalPhase;

void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position);

void ov71_02247704(Trade3DModel *tradeModel, BOOL enabled);

BOOL ov71_02247738(Trade3DModel *tradeModel);

void ov71_0224A8A8(TradeBallArrivalPhase *arrivalPhase);

void ov71_0224A7D0(SysTask *task, void *param);

void ov71_0224A7D0(SysTask *task, void *param) {
    BallArrivalPathState *bapState = param;

    if (bapState->frameCount < 70) {
        bapState->frameCount++;
    }

    switch (bapState->subState) {
    case 0:
        ov71_0224A6D8(bapState);
        ov71_022476C4(bapState->model, &(bapState->modelPos));

        if (++(bapState->frameCount) >= 12) {
            bapState->frameCount = 0;
            bapState->subState++;
        }
        break;
    case 1:
        if (ov71_02247738(bapState->model) == 0) {
            ov71_0224A8A8(bapState->arrivalPhase);
            bapState->subState++;
        } else {
            bapState->orbitCenter.y += bapState->orbitCenterStepY;
            bapState->orbitCenter.z += bapState->orbitCenterStepZ;
            ov71_0224A6D8(bapState);
            ov71_022476C4(bapState->model, &(bapState->modelPos));
        }
        break;
    case 2:
        bapState->orbitCenter.y += bapState->orbitCenterStepY;
        bapState->orbitCenter.z += bapState->orbitCenterStepZ;

        ov71_0224A6D8(bapState);
        ov71_022476C4(bapState->model, &(bapState->modelPos));

        if (bapState->frameCount >= 70) {
            ov71_02247704(bapState->model, 0);
            bapState->exitTimer = 0;
            bapState->subState++;
        }
        break;
    case 3:
        if (++(bapState->exitTimer) > 8) {
            bapState->task = 0L;
            SysTask_Destroy(task);
        }
        break;
    }
}
