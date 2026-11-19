#include "global.h"
struct TradeTubePhase;

#include "screen_fade.h"
#include "unk_02026E30.h"
#include "overlay_71_trade_tube_phase.h"

s32 ov71_022474CC(void *);

void ov71_022483E4(struct TradeTubePhase *arg0);

void ov71_022483E4(TradeTubePhase *arg0) {
    if (*((s32 *)&arg0->scene) != 0) {
        G3X_Reset();
        ov71_022474CC(*((void **)&arg0->scene));
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

s32 ov71_02248604(TradeTubePhase *ttPhase);

s32 ov71_022488E4(TradeTubePhase *);

s32 ov71_02248A08(void *);

s32 ov71_02248408(void *arg0);

s32 ov71_02248408(void *arg0) {
    ov71_02248604(arg0);
    ov71_022488E4(arg0);
    ov71_02248A08(arg0);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 8, 1, HEAP_ID_57);
    return 1;
}
