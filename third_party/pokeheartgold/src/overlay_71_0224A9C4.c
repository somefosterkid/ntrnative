#include "global.h"

#include "screen_fade.h"
#include "unk_02026E30.h"
#include "bg_window.h"
#include "sys_task.h"
#include "overlay_71_trade_pokemon_reveal_phase.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_palette_shimmer_effect.h"

s32 ov71_022474CC(void *);

s32 ov71_022476EC(void *, void *);

void ov71_0224A9C4(void *arg0);

struct BgScaleAnimation;
struct PaletteShimmerEffect;
struct Trade3DModel;
struct Trade3DScene;
struct TradeSequenceData;
typedef struct Trade3DScene Trade3DScene;
typedef struct BgScaleAnimation BgScaleAnimation;
PaletteShimmerEffect *ov71_02246C6C(BOOL *isAlive, int direction);
void ov71_02246D40(PaletteShimmerEffect *shimmerEffect);
void ov71_0224AF08(TradePokemonRevealPhase *tprPhase, SysTask **task);
void ov71_0224AFD4(TradePokemonRevealPhase *tprPhase, SysTask **task);
void ov71_0224B084(Trade3DModel *model, int startValue, int targetValue, int duration, SysTask **task);
int ov71_0224AA28(TradePokemonRevealPhase *tprPhase, int *subStepCounter);

void ov71_0224A9C4(void *arg0) {
    if (*((s32 *)&((TradePokemonRevealPhase *)arg0)->scene) != 0) {
        *((s16 *)&((TradePokemonRevealPhase *)arg0)->modelRotation.y) = (s16)(*((s16 *)&((TradePokemonRevealPhase *)arg0)->modelRotation.y) + 0x1000);
        ov71_022476EC(*((void **)&((TradePokemonRevealPhase *)arg0)->model), arg0 + 0x28);
        G3X_Reset();
        ov71_022474CC(*((void **)&((TradePokemonRevealPhase *)arg0)->scene));
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

s32 ov71_0224AB7C();

s32 ov71_0224ADE4(void *);

s32 ov71_0224A9F8(void *arg0);

s32 ov71_0224A9F8(void *arg0) {
    ov71_0224AB7C();
    ov71_0224ADE4(arg0);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 8, 1, HEAP_ID_57);
    return 1;
}

int ov71_0224AA28(TradePokemonRevealPhase *tprPhase, int *subStepCounter) {
    switch (*subStepCounter) {
    case 0:
        if (IsPaletteFadeFinished()) {
            tprPhase->timer = 0;
            ov71_0224AFD4(tprPhase, &(tprPhase->task0));
            (*subStepCounter)++;
        }
        break;
    case 1:
        if (++(tprPhase->timer) > 8) {
            ov71_0224AF08(tprPhase, &(tprPhase->task1));
            (*subStepCounter)++;
        }
        break;
    case 2:
        if ((tprPhase->task1 == 0L) && (tprPhase->task0 == 0L)) {
            (*subStepCounter)++;
        }
        break;
    case 3:
        tprPhase->shimmerEffect = ov71_02246C6C(&(tprPhase->shimmerActive), -1);
        tprPhase->timer = 0;
        (*subStepCounter)++;
        break;
    case 4:
        if (++(tprPhase->timer) > 30) {
            ov71_0224B084(tprPhase->model, 31, 0, 40, &(tprPhase->task2));
            (*subStepCounter)++;
        }
        break;
    case 5:
        if (tprPhase->task2 == 0L) {
            ov71_02246D40(tprPhase->shimmerEffect);
            (*subStepCounter)++;
        }
        break;
    case 6:
        if (tprPhase->shimmerActive == 0) {
            return 1;
        }
        break;
    }

    return 0;
}
