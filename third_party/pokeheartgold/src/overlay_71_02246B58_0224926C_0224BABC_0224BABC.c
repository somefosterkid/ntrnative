#include "global.h"

#include "sys_task.h"
#include "unk_02005D10.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_trade_receive_phase.h"

struct Trade3DModel;

struct Trade3DScene;

struct TradeReceivePhase;

typedef struct Trade3DScene Trade3DScene;

void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position);

void ov71_02247708(Trade3DModel *tradeModel, int alpha);

typedef struct BallOpenState {
    SysTask **taskHandle;
    int subState;
    void *phase;
    int timer;
    Trade3DScene *scene;
    Trade3DModel *model;
    fx32 currentAlpha;
    fx32 alphaStep;
    int alphaFrames;
    VecFx32 position;
    fx32 velocityY;
    VecFx16 rotation;
    fx16 rotationSpeed;
    fx16 rotationSpeedStep;
    fx16 targetRotationSpeed;
    int rotationTransitionFrames;
} BallOpenState;

void ov71_0224B910(TradeReceivePhase *trPhase, int startValue, int targetValue, int duration);

BOOL ov71_0224B960(TradeReceivePhase *trPhase);

void ov71_0224BA48(SysTask *task);

void ov71_0224BA64(BallOpenState *boState);

void ov71_0224BAA0(BallOpenState *boState, fx32 targetSpeed, int duration);

void ov71_0224BABC(SysTask *task, void *param);

void ov71_0224BABC(SysTask *task, void *param) {
    BallOpenState *boState = param;

    ov71_0224BA64(boState);

    switch (boState->subState) {
    case 0:
        if (boState->alphaFrames) {
            boState->currentAlpha += boState->alphaStep;
            boState->alphaFrames--;
            ov71_02247708(boState->model, boState->currentAlpha >> 12);
        } else {
            ov71_02247708(boState->model, 31);
            boState->subState++;
        }
        break;
    case 1:
        ov71_0224BAA0(boState, 384, 30);
        boState->timer = 0;
        boState->subState++;
        break;
    case 2:
        if (++(boState->timer) > 10) {
            boState->velocityY = 0;
            boState->subState++;
        }
        break;
    case 3:
        boState->position.y += boState->velocityY;
        boState->velocityY += -1920;

        ov71_022476C4(boState->model, &(boState->position));

        if (boState->position.y < (int)0xffff5000) {
            PlaySE(1510);
            boState->velocityY *= -1;

            ov71_0224B910(boState->phase, 0, 16, 8);
            boState->subState++;
        }
        break;
    case 4:
        boState->position.y += boState->velocityY;

        if (boState->position.y >= 77824) {
            boState->position.y = 77824;
        }

        ov71_022476C4(boState->model, &(boState->position));

        if (boState->position.y == 77824) {
            boState->subState++;
        }
        break;
    case 5:
        if (ov71_0224B960(boState->phase)) {
            ov71_0224BA48(task);
        }
    }
}
