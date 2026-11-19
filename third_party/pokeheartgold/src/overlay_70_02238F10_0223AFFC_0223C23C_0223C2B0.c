#include "global.h"
struct GTSApplicationState;

#include "unk_02005D10.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238D84(GTSApplicationState *, s32, s32);

s32 ov70_02238E50(GTSApplicationState *, s32, s32);

s32 ov70_0223CAC4(GTSApplicationState *, s32, s32, s32, s32);

s32 ov70_0223C2B0(GTSApplicationState *arg0);

s32 ov70_0223C2B0(GTSApplicationState *arg0) {
    ov70_0223CAC4(arg0, 0x9E, 4, 0, 0xF0F);
    ov70_02238D84(arg0, 0x16, 2);
    ov70_02238E50(arg0, 0, 0);
    PlaySE(0x5F3U);
    return 3;
}

s32 ov70_0223C2EC(struct GTSApplicationState *arg0);

s32 ov70_0223C2EC(GTSApplicationState *arg0) {
    void *v0;

    v0 = arg0->speciesMenuState;
    if (*(s32 *)((u8 *)v0 + 0x24) == 0) {
        return *(s32 *)((u8 *)v0 + 0x28);
    }
    return *(s32 *)((u8 *)v0 + 0x2C) + 4;
}
