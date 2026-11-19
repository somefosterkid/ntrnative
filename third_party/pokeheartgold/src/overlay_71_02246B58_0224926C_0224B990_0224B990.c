#include "global.h"

#include "sys_task.h"
#include "overlay_71_trade_receive_phase.h"

struct TradeReceivePhase;

typedef struct BrightnessAnimState {
    TradeReceivePhase *phase;
    fx32 currentValue;
    fx32 targetValue;
    fx32 step;
    int framesRemaining;
} BrightnessAnimState;

void ov71_0224B970(TradeReceivePhase *trPhase);

void ov71_0224B990(SysTask *unused, void *param);

void ov71_0224B990(SysTask *unused, void *param) {
    BrightnessAnimState *baState = param;
    int brightness;

    if (--(baState->framesRemaining) > 0) {
        baState->currentValue += baState->step;
        brightness = baState->currentValue >> 12;
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, brightness);
    } else {
        brightness = baState->targetValue >> 12;
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, brightness);
        ov71_0224B970(baState->phase);
    }
}
