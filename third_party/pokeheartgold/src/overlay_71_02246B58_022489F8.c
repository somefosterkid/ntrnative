#include "global.h"

#include "overlay_71_trade_tube_phase.h"

void ov71_02247498(struct Trade3DScene *tradeScene);

void ov71_022489F8(TradeTubePhase *ttPhase);

void ov71_022489F8(TradeTubePhase *ttPhase) {
    if (ttPhase->scene != NULL) {
        ov71_02247498(ttPhase->scene);
    }
}
