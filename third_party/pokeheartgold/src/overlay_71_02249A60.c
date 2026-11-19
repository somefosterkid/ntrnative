#include "global.h"

#include "math_util.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_trade_ball_flyby_phase.h"

struct Trade3DModel;

struct Trade3DScene;

typedef struct Trade3DScene Trade3DScene;

typedef struct BallOrbitState {
    void *phase;
    SysTask *taskHandle;
    Trade3DScene *scene;
    Trade3DModel *model;
    VecFx32 center;
    VecFx32 orbitOffset;
    VecFx32 currentPos;
    VecFx16 rotation;
    fx32 centerYStep;
    fx32 centerZStep;
    fx32 orbitRadiusX;
    fx32 orbitRadiusY;
    fx32 orbitRadiusXStep;
    fx32 orbitRadiusYStep;
    fx32 scale;
    fx32 scaleStep;
    int transitionFrames;
    int state;
    int timer;
    int frameCount;
    fx16 angle;
    fx16 angleSpeed;
} BallOrbitState;

void ov71_02249A60(BallOrbitState *boState, fx32 targetRadiusX, fx32 targetRadiusY, fx32 targetScale, int duration);

struct TradeBallFlybyPhase;
void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position);
void ov71_02247704(Trade3DModel *tradeModel, BOOL enabled);
BOOL ov71_02247738(Trade3DModel *tradeModel);
void ov71_02249C90(TradeBallFlybyPhase *tbfPhase);
void ov71_02249B64(SysTask *task, void *param);

void ov71_02249A60(BallOrbitState *boState, fx32 targetRadiusX, fx32 targetRadiusY, fx32 targetScale, int duration) {
    if (duration) {
        boState->orbitRadiusXStep = (targetRadiusX - boState->orbitRadiusX) / duration;
        boState->orbitRadiusYStep = (targetRadiusY - boState->orbitRadiusY) / duration;
        boState->scaleStep = (targetScale - boState->scale) / duration;
        boState->transitionFrames = duration;
    }
}

void ov71_022476EC(Trade3DModel *tradeModel, const VecFx16 *rotation);

void ov71_02247730(Trade3DModel *tradeModel, fx32 scale);

void ov71_02249A98(BallOrbitState *boState);

void ov71_02249A98(BallOrbitState *boState) {
    int angleDegrees = (u16)(boState->angle) / (65536 / 360);

    if (angleDegrees >= 360) {
        angleDegrees -= 360;
    }

    boState->angle += boState->angleSpeed;

    if (boState->transitionFrames) {
        boState->orbitRadiusX += boState->orbitRadiusXStep;
        boState->orbitRadiusY += boState->orbitRadiusYStep;
        boState->scale += boState->scaleStep;
        ov71_02247730(boState->model, boState->scale);
        boState->transitionFrames--;
    }

    boState->orbitOffset.x = FX_Mul(GF_CosDegNoWrap(angleDegrees), boState->orbitRadiusX);
    boState->orbitOffset.y = FX_Mul(GF_SinDegNoWrap(angleDegrees), boState->orbitRadiusY);
    boState->orbitOffset.z = 0;
    boState->rotation.x += 0x300;

    ov71_022476EC(boState->model, &(boState->rotation));
    VEC_Add(&boState->orbitOffset, &boState->center, &boState->currentPos);
}

void ov71_02249B64(SysTask *task, void *param) {
    BallOrbitState *boState = param;

    if (boState->frameCount < 60) {
        boState->frameCount++;
    }

    switch (boState->state) {
    case 0:
        boState->center.y += boState->centerYStep;
        boState->center.z += boState->centerZStep;

        ov71_02249A98(boState);
        ov71_022476C4(boState->model, &(boState->currentPos));

        if (ov71_02247738(boState->model)) {
            boState->state++;
        }
        break;
    case 1:
        if (ov71_02247738(boState->model) == 0) {
            ov71_02249C90(boState->phase);
            boState->state++;
        } else {
            boState->center.y += boState->centerYStep;
            boState->center.z += boState->centerZStep;
            ov71_02249A98(boState);
            ov71_022476C4(boState->model, &(boState->currentPos));
        }
        break;
    case 2:
        boState->center.y += boState->centerYStep;
        boState->center.z += boState->centerZStep;

        ov71_02249A98(boState);
        ov71_022476C4(boState->model, &(boState->currentPos));

        if (boState->frameCount >= 60) {
            ov71_02249A60(boState, 2048, 2048, 512, 12);
            boState->timer = 0;
            boState->state++;
        }
        break;
    case 3:
        ov71_02249A98(boState);
        ov71_022476C4(boState->model, &(boState->currentPos));

        if (++(boState->timer) >= 12) {
            ov71_02247704(boState->model, 0);
            boState->timer = 0;
            boState->state++;
        }
        break;
    case 4:
        if (++(boState->timer) > 8) {
            boState->taskHandle = 0L;
            SysTask_Destroy(task);
        }
        break;
    }
}
