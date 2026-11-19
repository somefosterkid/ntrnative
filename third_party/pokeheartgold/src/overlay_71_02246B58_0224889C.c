#include "global.h"

#include "bg_window.h"
#include "overlay_71_trade_tube_phase.h"

void ov71_02246D54(struct PaletteShimmerEffect *ptr);

void ov71_0224889C(TradeTubePhase *ttPhase);

void ov71_0224889C(TradeTubePhase *ttPhase) {
    if (ttPhase->shimmerActive != 0) {
        ov71_02246D54(ttPhase->shimmerEffect);
    }
    FreeBgTilemapBuffer(ttPhase->bgConfig, 2U);
    FreeBgTilemapBuffer(ttPhase->bgConfig, 6U);
    FreeBgTilemapBuffer(ttPhase->bgConfig, 3U);
    FreeBgTilemapBuffer(ttPhase->bgConfig, 7U);
}
