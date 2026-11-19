#include "global.h"
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

int ov71_0224A18C(TradeBallArrivalPhase *arrivalPhase, int *unused);

int ov71_0224A1C4(TradeBallArrivalPhase *arrivalPhase, int *state);

BOOL ov71_0224A15C(void *param, int *state);

BOOL ov71_0224A15C(void *param, int *state) {
    static int (*const sSubSteps[])(TradeBallArrivalPhase *, int *) = {
        ov71_0224A18C,
        ov71_0224A1C4,
    };

    TradeBallArrivalPhase *arrivalPhase = param;

    if ((*state) < NELEMS(sSubSteps)) {
        if (sSubSteps[*state](arrivalPhase, &(arrivalPhase->subStepCounter))) {
            (*state)++;
            arrivalPhase->subStepCounter = 0;
        }

        return FALSE;
    }

    return TRUE;
}
