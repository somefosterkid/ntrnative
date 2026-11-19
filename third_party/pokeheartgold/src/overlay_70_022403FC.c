#include "global.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_022403FC(GTSApplicationState *arg0);

void ov70_02238008(void *);
u32 ov70_02240410(GTSApplicationState *param_1);

s32 ov70_022403FC(GTSApplicationState *arg0) {
    ov70_02238E50(arg0, 1, 0);
    arg0->currentScreenInstruction = 0x24;
    return 3;
}

u32 ov70_02240410(GTSApplicationState * param_1) {
    ov70_02238008(&param_1->receivedListing);
    param_1->currentScreenInstruction = 0x1b;
    param_1->networkTimeoutCounter = 0;
    return 3;
}
