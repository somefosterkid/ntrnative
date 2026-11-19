#include "global.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238D84(GTSApplicationState *, s32, s32);

s32 ov70_0223CAC4(GTSApplicationState *, s32, s32, s32, s32);

s32 ov70_0223C7C4(GTSApplicationState *arg0);

s32 ov70_0223C7C4(GTSApplicationState *arg0) {
    ov70_0223CAC4(arg0, 0xB0, 1, 0, 0xF0F);
    ov70_02238D84(arg0, 0x15, 0xD);
    return 3;
}
