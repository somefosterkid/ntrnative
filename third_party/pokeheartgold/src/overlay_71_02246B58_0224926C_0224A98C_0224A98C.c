#include "global.h"
#include "overlay_71_trade_pokemon_reveal_phase.h"
#include "overlay_71_trade3dmodel.h"
#include "bg_window.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_palette_shimmer_effect.h"

struct BgConfig;

struct BgScaleAnimation;

struct PaletteShimmerEffect;

struct SysTask;

struct SysTaskQueue;

struct Trade3DModel;

struct Trade3DScene;

struct TradePokemonRevealPhase;

struct TradeSequenceData;

typedef struct Trade3DScene Trade3DScene;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct BgScaleAnimation BgScaleAnimation;

void ov71_0224A9C4(TradePokemonRevealPhase *tprPhase);

int ov71_0224A9F8(TradePokemonRevealPhase *tprPhase, int *unused);

int ov71_0224AA28(TradePokemonRevealPhase *tprPhase, int *subStepCounter);

int ov71_0224AAF8(TradePokemonRevealPhase *tprPhase, int *subStepCounter);

BOOL ov71_0224A98C(void *param, int *state);

BOOL ov71_0224A98C(void *param, int *state) {
    static int (*const sStepFuncs[])(TradePokemonRevealPhase *, int *) = {
        ov71_0224A9F8,
        ov71_0224AA28,
        ov71_0224AAF8
    };

    TradePokemonRevealPhase *tprPhase = param;

    if ((*state) < NELEMS(sStepFuncs)) {
        if (sStepFuncs[*state](tprPhase, &(tprPhase->subStepCounter))) {
            (*state)++;
            tprPhase->subStepCounter = 0;
        }

        ov71_0224A9C4(tprPhase);

        return FALSE;
    }

    return TRUE;
}
