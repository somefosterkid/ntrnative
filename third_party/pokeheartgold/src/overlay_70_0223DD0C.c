#include "global.h"

#include "screen_fade.h"
#include "yes_no_prompt.h"
#include "render_window.h"
#include "overlay_70_gtsapplication_state.h"
s32 ov70_0223DD0C(GTSApplicationState *arg0);

s32 ov70_02238C8C(GTSApplicationState *arg0);
s32 ov70_02238E50(void *, s32, s32);
s32 ov70_0223DD94(GTSApplicationState *arg0);

s32 ov70_02238D84(void *, s32, s32);
s32 ov70_0223E01C(void *, s32, s32, s32, u16, s32);
s32 ov70_0223DDD0(void *arg0);
s32 ov70_0223DDFC(GTSApplicationState *arg0);
s32 ov70_0223DE6C(void *);
s32 ov70_0223DE28(void *arg0);

s32 ov70_0223DD0C(GTSApplicationState *arg0) {
    s32 v0;

    v0 = arg0->nextScreen;
    if ((v0 == 0) || (v0 == 8) || (v0 == 3)) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
        arg0->fadeBothScreens = 1;
    } else {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
    }
    arg0->currentScreenInstruction = 0;
    return 4;
}

struct YesNoPrompt *ov70_02238C14(void *, s32, s32, s32, u8);

s32 ov70_0223DD68(GTSApplicationState *arg0);

s32 ov70_0223DD68(GTSApplicationState *arg0) {
    arg0->unk_11C8 = ov70_02238C14(arg0->bgConfig, 0xC, 0x1AD, 8, 1);
    arg0->currentScreenInstruction = 6;
    return 3;
}

s32 ov70_0223DD94(GTSApplicationState *arg0) {
    s32 v0;

    v0 = ov70_02238C8C(arg0);
    if (v0 == 1) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        arg0->currentScreenInstruction = 2;
        ov70_02238E50(arg0, 0, 0);
    } else if (v0 == 2) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        arg0->currentScreenInstruction = 0;
    }
    return 3;
}

s32 ov70_0223DDD0(void *arg0) {
    ov70_0223E01C(arg0, 0x19, 1, 0, 0xF0F, 1);
    ov70_02238D84(arg0, 3, 0xC);
    return 3;
}

s32 ov70_0223DDFC(GTSApplicationState *arg0) {
    arg0->unk_11C8 = ov70_02238C14(arg0->bgConfig, 0xA, 0x1AD, 8, 1);
    arg0->currentScreenInstruction = 0xD;
    return 3;
}

s32 ov70_0223DE28(void *arg0) {
    s32 v0;

    v0 = ov70_02238C8C(arg0);
    if (v0 == 1) {
        YesNoPrompt_Destroy(((GTSApplicationState *)arg0)->unk_11C8);
        ov70_0223DE6C(arg0);
    } else if (v0 == 2) {
        YesNoPrompt_Destroy(((GTSApplicationState *)arg0)->unk_11C8);
        ClearFrameAndWindow2(arg0 + 0x1158, 0);
        *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 1;
    }
    return 3;
}
