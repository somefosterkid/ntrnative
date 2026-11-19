#include "global.h"
#include "overlay_71_trade_tube_phase.h"
#include "overlay_71_ball_path_state.h"
#include "overlay_71_trade3dmodel.h"
#include "bg_window.h"
#include "sprite.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_palette_shimmer_effect.h"
#include "overlay_71_sprite_anim_resources.h"

struct AxisRotAnimator;

struct BallPathState;

struct BgConfig;

struct BgScaleAnimation;

struct PaletteShimmerEffect;

struct Sprite;

struct SpriteAnimResources;

struct SysTask;

struct SysTaskQueue;

struct Trade3DModel;

struct Trade3DScene;

struct TradeSequenceData;

struct TradeTubePhase;

typedef struct Trade3DScene Trade3DScene;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct BgScaleAnimation BgScaleAnimation;

typedef struct AxisRotAnimator {
    fx16 currentValue;
    fx16 step;
    fx16 targetValue;
    u16 framesRemaining;
} AxisRotAnimator;

int ov71_02248408(TradeTubePhase *ttPhase, int *unused);

int ov71_0224843C(TradeTubePhase *ttPhase, int *subStepCounter);

int ov71_02248484(TradeTubePhase *ttPhase, int *subStepCounter);

int ov71_02248530(TradeTubePhase *ttPhase, int *subStepCounter);

void ov71_022479C8(void *arg0);

BOOL ov71_02247990(void *param, int *state);

BOOL ov71_02247990(void *param, int *state) {
    extern int (*const ov71_0224BC64[4])(TradeTubePhase *, int *);

    TradeTubePhase *ttPhase = param;

    if ((*state) < NELEMS(ov71_0224BC64)) {
        if (ov71_0224BC64[*state](ttPhase, &(ttPhase->subStepCounter))) {
            (*state)++;
            ttPhase->subStepCounter = 0;
        }

        ov71_022479C8(ttPhase);

        return FALSE;
    }

    return TRUE;
}
