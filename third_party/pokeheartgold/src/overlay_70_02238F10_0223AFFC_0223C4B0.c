#include "global.h"

#include "screen_fade.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_0223C4B0(GTSApplicationState *arg0);

s32 ov70_0223C4B0(GTSApplicationState *arg0) {
    if (arg0->unk_F14 == 0) {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
    }
    arg0->currentScreenInstruction = 0;
    return 4;
}

s32 ov70_02238D84(GTSApplicationState *, s32, s32);

s32 ov70_0223CAC4(GTSApplicationState *, s32, s32, s32, s32);

s32 ov70_0223C4E4(GTSApplicationState *arg0);

s32 ov70_0223C4E4(GTSApplicationState *arg0) {
    ov70_0223CAC4(arg0, 9, 1, 0, 0xF0F);
    ov70_02238D84(arg0, 0x15, 4);
    return 3;
}

s32 ov70_0223CD28(s32);

s32 ov70_022420C4(void *, s16);

s32 ov70_0223C50C(GTSApplicationState *arg0);

s32 ov70_0223C50C(GTSApplicationState *arg0) {
    ov70_022420C4(arg0->unk_11A8, 0);
    ov70_0223CD28(9);
    arg0->currentScreenInstruction = 5;
    return 3;
}
