#include "global.h"
#include "sys_task_api.h"
#include "overlay_71_trade3dmodel.h"
#include "bg_window.h"
#include "sprite.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_sprite_anim_resources.h"
#include "overlay_71_trade_ball_arrival_phase.h"

struct BallArrivalPathState;

struct BgConfig;

struct Sprite;

struct SpriteAnimResources;

struct SysTask;

struct SysTaskQueue;

struct Trade3DModel;

struct Trade3DScene;

struct TradeBallArrivalPhase;

struct TradeSequenceData;

typedef struct Trade3DScene Trade3DScene;

void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position);

void ov71_022476D4(const Trade3DModel *tradeModel, VecFx16 *outRotation);

void ov71_022476EC(Trade3DModel *tradeModel, const VecFx16 *rotation);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

void ov71_0224A6D8(BallArrivalPathState *bapState);

void ov71_0224A7D0(SysTask *task, void *param);

void ov71_0224A5C4(TradeBallArrivalPhase *arrivalPhase, BallArrivalPathState *bapState);

void ov71_0224A5C4(TradeBallArrivalPhase *arrivalPhase, BallArrivalPathState *bapState)
{
    bapState->arrivalPhase = arrivalPhase;
    bapState->model = arrivalPhase->model;
    bapState->scene = arrivalPhase->scene;

    bapState->orbitCenter.x = 0;
    bapState->orbitCenter.y = 4025344 ;
    bapState->orbitCenter.z = 0xFFE44000 ;
    bapState->rotationAngle = 55408 ;
    bapState->rotationSpeed = 62800 ;

    bapState->orbitCenterStepY = ((int)0xFFF3CE00 - 4025344 ) / 70 ;
    bapState->orbitCenterStepZ = (823296 - 0xFFE44000 ) / 70 ;

    bapState->subState = 0;
    bapState->exitTimer = 0;
    bapState->frameCount = 0;

    bapState->rotation.x = 8192 ;
    bapState->rotation.y = 0xe000;
    bapState->rotation.z = 0;

    ov71_022476EC(bapState->model, &bapState->rotation);

    bapState->scale = 512 ;
    bapState->orbitRadiusX = 2048 ;
    bapState->orbitRadiusY = 2048 ;
    bapState->transitionFrames = 0;

    ov71_0224A694(bapState, 75776 , 101376 , ((fx32) 0x0000000000001000L) , 12 );
    ov71_0224A6D8(bapState);
    ov71_022476C4(bapState->model, &(bapState->modelPos));
    ov71_022476D4(bapState->model, &(bapState->rotation));

    bapState->task = SysTask_CreateOnMainQueue(ov71_0224A7D0, bapState, 0);
}
