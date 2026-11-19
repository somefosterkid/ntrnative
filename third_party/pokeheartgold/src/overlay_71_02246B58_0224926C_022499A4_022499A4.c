#include "global.h"
#include "sys_task_api.h"
#include "overlay_71_trade_ball_flyby_phase.h"
#include "overlay_71_trade3dmodel.h"
#include "bg_window.h"
#include "sprite.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_sprite_anim_resources.h"

struct BallOrbitState;

struct BgConfig;

struct Sprite;

struct SpriteAnimResources;

struct SysTask;

struct SysTaskQueue;

struct Trade3DModel;

struct Trade3DScene;

struct TradeBallFlybyPhase;

struct TradeSequenceData;

typedef struct Trade3DScene Trade3DScene;

void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position);

void ov71_022476D4(const Trade3DModel *tradeModel, VecFx16 *outRotation);

void ov71_022476EC(Trade3DModel *tradeModel, const VecFx16 *rotation);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

void ov71_02249A98(BallOrbitState *boState);

void ov71_02249B64(SysTask *task, void *param);

void ov71_022499A4(TradeBallFlybyPhase *tbfPhase, BallOrbitState *boState);

void ov71_022499A4(TradeBallFlybyPhase *tbfPhase, BallOrbitState *boState)
{
    boState->phase = tbfPhase;
    boState->model = tbfPhase->model;
    boState->scene = tbfPhase->scene;

    boState->center.x = 0;
    boState->center.y = 0xFFF50E00 ;
    boState->center.z = 925696 ;
    boState->angle = 55408 ;
    boState->angleSpeed = 62800 ;

    boState->centerYStep = (4025344 - 0xFFF50E00 ) / 60 ;
    boState->centerZStep = ((int)0xFFE44000 - 925696 ) / 60 ;

    boState->state = 0;
    boState->timer = 0;
    boState->frameCount = 0;

    boState->rotation.x = 8192 ;
    boState->rotation.y = 0xe000;
    boState->rotation.z = 0;

    ov71_022476EC(boState->model, &boState->rotation);

    boState->scale = ((fx32) 0x0000000000001000L) ;
    boState->orbitRadiusX = 75776 ;
    boState->orbitRadiusY = 101376 ;
    boState->transitionFrames = 0;

    ov71_02249A98(boState);
    ov71_022476C4(boState->model, &(boState->currentPos));
    ov71_022476D4(boState->model, &(boState->rotation));

    boState->taskHandle = SysTask_CreateOnMainQueue(ov71_02249B64, boState, 0);
}
