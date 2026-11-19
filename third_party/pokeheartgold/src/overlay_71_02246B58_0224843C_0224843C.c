#include "global.h"

#include "screen_fade.h"
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

int ov71_0224843C(TradeTubePhase *ttPhase, int *subStepCounter);

int ov71_0224843C(TradeTubePhase *ttPhase, int *subStepCounter) {
    extern int ov71_0224C0C4;

    switch (*subStepCounter) {
    case 0:
        if (IsPaletteFadeFinished()) {
            ov71_0224C0C4 = 0;
            (*subStepCounter)++;
        }
        break;

    case 1:
        if (ov71_0224C0C4 < 30) {
            ov71_0224C0C4++;
        }

        if (ttPhase->bgScaleAnimDone) {
            return 1;
        }
        break;
    }

    return 0;
}
