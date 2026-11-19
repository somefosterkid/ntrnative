#include "global.h"

#include "render_window.h"
#include "text.h"
#include "yes_no_prompt.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238D84(GTSApplicationState *, s32, s32);

s32 ov70_0223E01C(GTSApplicationState *, s32, s32, s32, u16, s32);

s32 ov70_0223DF14(GTSApplicationState *arg0);

s32 ov70_0223DF14(GTSApplicationState *arg0) {
    ov70_0223E01C(arg0, 0x19, 1, 0, 0xF0F, 1);
    ov70_02238D84(arg0, 3, 0xF);
    return 3;
}

struct YesNoPrompt *ov70_02238C14(void *, s32, s32, s32, u8);

s32 ov70_0223DF40(GTSApplicationState *arg0);

s32 ov70_0223DF40(GTSApplicationState *arg0) {
    arg0->unk_11C8 = ov70_02238C14(arg0->bgConfig, 0xA, 0x1AD, 8, 1);
    arg0->currentScreenInstruction = 0x10;
    return 3;
}

s32 ov70_02238C8C(GTSApplicationState *arg0);

s32 ov70_02238E50(void *, s32, s32);

void *ov70_0223E49C(void *, void *, u32, s32);

s32 ov70_0223DF6C(GTSApplicationState *arg0);

s32 ov70_0223DF6C(GTSApplicationState *arg0) {
    s32 v0;

    v0 = ov70_02238C8C(arg0);
    if (v0 == 1) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        arg0->selectedBoxMon = ov70_0223E49C(*(void **)((u8 *)arg0->playerData + 8), *(void **)((u8 *)arg0->playerData + 0xC), arg0->selectedBoxId, arg0->partySlotIndex);
        arg0->currentScreenInstruction = 2;
        ov70_02238E50(arg0, 6, 0);
    } else if (v0 == 2) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        ClearFrameAndWindow2(&arg0->confirmationWindow, 0);
        arg0->currentScreenInstruction = 1;
    }
    return 3;
}

s32 ov70_0223DFD0(GTSApplicationState *arg0);

s32 ov70_0223DFD0(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinter) == 0) {
        arg0->currentScreenInstruction = (s32)arg0->nextScreenInstruction;
    }
    return 3;
}

s32 ov70_0223DFF0(GTSApplicationState *arg0);

s32 ov70_0223DFF0(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinter) == 0) {
        ClearFrameAndWindow2(&arg0->confirmationWindow, 0);
        arg0->currentScreenInstruction = (s32)arg0->nextScreenInstruction;
    }
    return 3;
}
