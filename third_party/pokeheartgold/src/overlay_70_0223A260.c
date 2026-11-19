#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "list_menu_items.h"
#include "render_window.h"
#include "screen_fade.h"
#include "yes_no_prompt.h"
#include "overlay_70_gtsapplication_state.h"
s32 ov70_02238D84(void *, s32, s32);

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_0223A4F4(void *, s32, s32, s32, s32, void *);

s32 ov70_0223A260(GTSApplicationState *arg0);

s32 ov70_0223A2D0(GTSApplicationState *arg0);
struct YesNoPrompt *ov70_02238C14(void *, s32, s32, s32, u8);
s32 ov70_0223A2F8(GTSApplicationState *arg0);
s32 ov70_02238C8C(GTSApplicationState *arg0);
s32 ov70_0223A874(void *);
s32 ov70_0223A324(GTSApplicationState *arg0);
void *ov70_02238CAC(GTSApplicationState *, u8, u8);
s32 ov70_02238FB4(s32);
s32 ov70_0223A384(GTSApplicationState *arg0);

s32 ov70_0223A260(GTSApplicationState *arg0) {
    if ((gSystem.newKeys & 1) || (gSystem.touchNew != 0)) {
        ov70_0223A4F4(arg0, 5, 1, 0, 0xF0F, &arg0->receivedListing);
        ov70_02238D84(arg0, 3, 7);
        PlaySE(0x5DCU);
    } else if (gSystem.newKeys & 2) {
        arg0->currentScreenInstruction = 2;
        ov70_02238E50(arg0, 1, 0);
        PlaySE(0x5DCU);
    }
    return 3;
}

s32 ov70_0223A2D0(GTSApplicationState *arg0) {
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
    arg0->currentScreenInstruction = 0;
    return 4;
}

s32 ov70_0223A2F8(GTSApplicationState *arg0) {
    arg0->unk_11C8 = ov70_02238C14(arg0->bgConfig, 0xC, 0x16A, 8, 1);
    arg0->currentScreenInstruction = 6;
    return 3;
}

s32 ov70_0223A324(GTSApplicationState *arg0) {
    s32 v0;

    v0 = ov70_02238C8C(arg0);
    if (v0 == 1) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        arg0->currentScreenInstruction = 2;
        arg0->fadeBothScreens = 1;
        ov70_02238E50(arg0, 7, 8);
        ov70_0223A874(arg0);
    } else if (v0 == 2) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        arg0->currentScreenInstruction = 0;
        ClearFrameAndWindow2(&arg0->bottomInstructionWindow, 0);
        ov70_0223A874(arg0);
    }
    return 3;
}

s32 ov70_0223A384(GTSApplicationState *arg0) {
    arg0->unk_11AC = ListMenuItems_New(2U, HEAP_ID_61);
    ListMenuItems_AppendFromMsgData(arg0->unk_11AC, arg0->gtsMessageLoader, 0x37, 1);
    ListMenuItems_AppendFromMsgData(arg0->unk_11AC, arg0->gtsMessageLoader, 0x38, 2);
    arg0->unk_11D0 = ov70_02238CAC(arg0, 2, 0xD);
    ov70_02238FB4(1);
    arg0->currentScreenInstruction = 8;
    return 3;
}
