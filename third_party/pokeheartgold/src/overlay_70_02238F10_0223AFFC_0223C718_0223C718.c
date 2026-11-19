#include "global.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_0223CD28(u32);

void ov70_022420C4(void *, s16);

u32 ov70_0223C718(GTSApplicationState *param_1);

u32 ov70_0223C718(GTSApplicationState *param_1) {
    param_1->listMenuCursorIndex = 0xffff;
    ov70_022420C4(param_1->unk_11A8, 2);
    ov70_0223CD28(9);
    param_1->currentScreenInstruction = 0xb;
    return 3;
}
