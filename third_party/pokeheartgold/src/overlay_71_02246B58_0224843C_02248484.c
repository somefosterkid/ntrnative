#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_71_trade_tube_phase.h"
#include "overlay_71_ball_path_state.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_palette_shimmer_effect.h"
#include "overlay_71_sprite_anim_resources.h"

struct BgScaleAnimation;

struct PaletteShimmerEffect;

struct Trade3DModel;

struct Trade3DScene;

struct TradeSequenceData;

typedef struct Trade3DScene Trade3DScene;

void ov71_02247704(Trade3DModel *tradeModel, BOOL enabled);

void ov71_02247708(Trade3DModel *tradeModel, int alpha);

typedef struct BgScaleAnimation BgScaleAnimation;

typedef struct AxisRotAnimator {
    fx16 currentValue;
    fx16 step;
    fx16 targetValue;
    u16 framesRemaining;
} AxisRotAnimator;

void ov71_02248B54(TradeTubePhase *ttPhase);

void ov71_02248E30(Trade3DModel *model, int currentValue, int targetValue, int framesRemaining);

SysTask *ov71_02248EB4(TradeTubePhase *ttPhase);

BOOL ov71_02249254(SysTask *task);

BOOL ov71_02249260(SysTask *task);

int ov71_02248484(TradeTubePhase *ttPhase, int *subStepCounter);

int ov71_02248484(TradeTubePhase *ttPhase, int *subStepCounter) {
    switch (*subStepCounter) {
    case 0:
        ttPhase->timer = 0;
        (*subStepCounter)++;
        break;
    case 1:
        if (++(ttPhase->timer) > 8) {
            ov71_02248E30(ttPhase->model, 0, 31, 24);
            ttPhase->task0 = ov71_02248EB4(ttPhase);
            ov71_02247708(ttPhase->model, 0);
            ov71_02247704(ttPhase->model, 1);
            (*subStepCounter)++;
        }
        break;
    case 2:
        if (ov71_02249260(ttPhase->task0)) {
            (*subStepCounter)++;
        }
        break;
    case 3:
        if (ov71_02249254(ttPhase->task0)) {
            (*subStepCounter)++;
        }
        break;
    case 4:
        ov71_02248B54(ttPhase);
        return 1;
    }

    return 0;
}
