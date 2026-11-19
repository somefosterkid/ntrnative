#include "global.h"
#include "overlay_71_trade_pokemon_reveal_phase.h"
#include "overlay_71_trade3dmodel.h"
#include "sys_task.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_palette_shimmer_effect.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "screen_fade.h"

struct BgConfig;

struct BgScaleAnimation;

struct SysTask;

struct SysTaskQueue;

struct Trade3DScene;

struct TradePokemonRevealPhase;

typedef struct Trade3DScene Trade3DScene;

typedef struct BgScaleAnimation BgScaleAnimation;

BgScaleAnimation *ov71_02247000(BgConfig *bgConfig, fx32 currentScale, fx32 targetScale, fx32 rate, fx32 acceleration, int centerIndex, volatile BOOL *done);

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

int ov71_0224AAF8(TradePokemonRevealPhase *tprPhase, int *subStepCounter);

int ov71_0224AAF8(TradePokemonRevealPhase *tprPhase, int *subStepCounter) {
    switch (*subStepCounter) {
    case 0:
        tprPhase->bgScaleAnim = ov71_02247000(tprPhase->bgConfig, 0x1000, 1024, 1, 296, 1, &(tprPhase->bgScaleAnimDone));
        tprPhase->timer = 0;
        (*subStepCounter)++;
        break;
    case 1:
        if (++(tprPhase->timer) > 13) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 8, 1, (enum HeapID)57);
            (*subStepCounter)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() && (tprPhase->bgScaleAnimDone)) {
            return 1;
        }
        break;
    }

    return 0;
}
