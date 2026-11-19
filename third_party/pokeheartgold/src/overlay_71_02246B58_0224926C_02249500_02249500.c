#include "global.h"
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

void ov71_02249538(TradeBallFlybyPhase *tbfPhase);

int ov71_02249558(TradeBallFlybyPhase *tbfPhase, int *unused);

int ov71_022495A0(TradeBallFlybyPhase *tbfPhase, int *subStateCounter);

BOOL ov71_02249500(void *param, int *state);

BOOL ov71_02249500(void *param, int *state) {
    static int (*const sStepFuncs[])(TradeBallFlybyPhase *, int *) = {
        ov71_02249558,
        ov71_022495A0
    };

    TradeBallFlybyPhase *tbfPhase = param;

    if ((*state) < NELEMS(sStepFuncs)) {
        if (sStepFuncs[*state](tbfPhase, &(tbfPhase->subStepCounter))) {
            (*state)++;
            tbfPhase->subStepCounter = 0;
        }

        ov71_02249538(tbfPhase);

        return FALSE;
    }

    return TRUE;
}
