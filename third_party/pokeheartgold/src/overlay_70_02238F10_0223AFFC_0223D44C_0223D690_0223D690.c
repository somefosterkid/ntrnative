#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "unk_02005D10.h"

void ov70_02238E50(void *, u32, u32);

void ov70_0223D690(GTSApplicationState *param_1, s32 param_2);

void ov70_0223D690(GTSApplicationState *param_1, s32 param_2) {
    if (param_2 == 5) {
        ov70_02238E50(param_1, 1, 0);
        param_1->currentScreenInstruction = 2;
        PlaySE(0x5dc);
        return;
    }
    if (param_2 == 6) {
        ov70_02238E50(param_1, 3, 0x11);
        param_1->currentScreenInstruction = 2;
        PlaySE(0x5dc);
    }
    return;
}
