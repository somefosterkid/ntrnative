#include "global.h"

#include "screen_fade.h"
#include "sprite.h"
#include "render_window.h"
#include "yes_no_prompt.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_0223B12C(GTSApplicationState *arg0);

s32 ov70_02238C8C(GTSApplicationState *arg0);
s32 ov70_02238E50(void *, s32, s32);
s32 ov70_0223B258(void *);
s32 ov70_0223B3BC(void *, void *, s32);
s32 ov70_0223B3EC(void *, void *, s32);
s32 ov70_0223B1B8(GTSApplicationState *arg0);

s32 ov70_0223B12C(GTSApplicationState *arg0) {
    s32 v0;

    Sprite_SetDrawFlag(arg0->unk_F0C, 0);
    v0 = arg0->screenArgument;
    if (v0 != 0xF) {
        if (v0 == 6) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
        } else {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
        }
    }
    arg0->currentScreenInstruction = 0;
    return 4;
}

YesNoPrompt *ov70_02238C2C(BgConfig *, u8, s32, s32, s32, u8);

s32 ov70_0223B188(GTSApplicationState *arg0);

s32 ov70_0223B188(GTSApplicationState *arg0) {
    arg0->unk_11C8 = ov70_02238C2C(arg0->bgConfig, 0xA, 0x12E, 3, 4, 1);
    arg0->currentScreenInstruction = 5;
    return 3;
}

s32 ov70_0223B1B8(GTSApplicationState *arg0) {
    s32 v0;

    v0 = ov70_02238C8C(arg0);
    if (v0 == 1) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        arg0->currentScreenInstruction = 2;
        ov70_02238E50(arg0, 5, 6);
        ov70_0223B3BC(arg0->gtsMessageLoader, &arg0->infoWindows[27], 0x4D);
        ov70_0223B3BC(arg0->gtsMessageLoader, &arg0->infoWindows[22], 0x51);
        ov70_0223B258(arg0);
    } else if (v0 == 2) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        ClearFrameAndWindow2(&arg0->bottomInstructionWindow, 0);
        arg0->currentScreenInstruction = 1;
        ov70_0223B3EC(arg0->gtsMessageLoader, arg0->menuButtonWindows, 0x58);
        ov70_0223B3EC(arg0->gtsMessageLoader, &arg0->menuButtonWindows[1], 0x6D);
    }
    return 3;
}
