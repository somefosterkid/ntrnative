#include "global.h"
struct GTSApplicationState;

#include "bg_window.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "text.h"
#include "yes_no_prompt.h"
#include "overlay_70_gtsapplication_state.h"
#include "unk_struct_ov70_0223f8a8.h"

s32 ov70_0223CD28(s32);

s32 ov70_0223F244(void *, void *, void *, s32, s32, s32, s32);

s32 ov70_0223F8A8(UnkStruct_ov70_0223F8A8 *, s32);

s32 ov70_02242144(void *);

extern s32 ov70_0224590C;

s32 ov70_0223C818(UnkStruct_ov70_0223F8A8 *arg0);

s32 ov70_0223C818(UnkStruct_ov70_0223F8A8 *arg0) {
    s32 v0;

    v0 = ov70_02242144(arg0->unk_11A8);
    if (v0 != -1) {
        if ((v0 == -2) || (v0 == (ov70_0224590C + 1))) {
            ov70_0223CD28(0);
            ClearFrameAndWindow2((void *)&arg0->unk_F18, 0);
            arg0->unk_2C = 0;
        } else {
            ov70_0223CD28(0);
            ov70_0223F8A8(arg0, v0);
            arg0->unk_2C = 0;
            FillWindowPixelBuffer((void *)&arg0->unk_1188, 0U);
            ov70_0223F244((void *)&arg0->unk_1188, arg0->unk_BB0, arg0->unk_BA0, *((s32 *)&arg0->unk_12CC), 0, 0, 0x10200);
        }
    }
    return 3;
}

struct YesNoPrompt *ov70_02238C14(void *, s32, s32, s32, u8);

s32 ov70_0223C8B0(GTSApplicationState *arg0);

s32 ov70_0223C8B0(GTSApplicationState *arg0) {
    arg0->unk_11C8 = ov70_02238C14(arg0->bgConfig, 0xC, 0x120, 3, 1);
    arg0->currentScreenInstruction = 0x18;
    return 3;
}

s32 ov70_02238C8C(GTSApplicationState *arg0);

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_02241234(void *);

s32 ov70_0223C8DC(struct GTSApplicationState *arg0);

s32 ov70_0223C8DC(GTSApplicationState *arg0) {
    s32 v0;

    v0 = ov70_02238C8C(arg0);
    if (v0 == 1) {
        YesNoPrompt_Destroy(*((YesNoPrompt * *)&arg0->unk_11C8));
        *((s32 *)&arg0->currentScreenInstruction) = 2;
        ov70_02238E50(arg0, 1, 0);
        ov70_02241234(arg0);
        *((s32 *)&arg0->searchResultCount) = 0;
    } else if (v0 == 2) {
        YesNoPrompt_Destroy(*((YesNoPrompt * *)&arg0->unk_11C8));
        *((s32 *)&arg0->currentScreenInstruction) = 0;
    }
    return 3;
}

s32 ov70_0223C928(GTSApplicationState *arg0);

s32 ov70_0223C928(GTSApplicationState *arg0) {
    arg0->currentScreenInstruction = 1;
    return 3;
}

s32 ov70_02238D84(GTSApplicationState *, s32, s32);

s32 ov70_0223CAC4(GTSApplicationState *, s32, s32, s32, s32);

s32 ov70_0223C930(GTSApplicationState *arg0);

s32 ov70_0223C930(GTSApplicationState *arg0) {
    ov70_0223CAC4(arg0, 0x98, 1, 0, 0xF0F);
    ov70_02238D84(arg0, 0x15, 1);
    return 3;
}

s32 ov70_0223C958(GTSApplicationState *arg0);

s32 ov70_0223C958(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->textPrinter) == 0) {
        arg0->currentScreenInstruction = (s32) * &arg0->nextScreenInstruction;
    }
    return 3;
}

s32 ov70_0223C978(GTSApplicationState *arg0);

s32 ov70_0223C978(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->textPrinter) == 0) {
        arg0->unk_11C0 = (s32)(arg0->unk_11C0 + 1);
        if ((s32) * &arg0->unk_11C0 > 0x2D) {
            arg0->unk_11C0 = 0;
            arg0->currentScreenInstruction = (s32) * &arg0->nextScreenInstruction;
        }
    }
    return 3;
}

s32 ov70_0223C9AC(GTSApplicationState *arg0);

s32 ov70_0223C9AC(GTSApplicationState *arg0) {
    arg0->unk_F14 = (s32)(arg0->unk_F14 + 1);
    if (IsPaletteFadeFinished() != 0) {
        GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
        arg0->unk_F14 = -0x10;
        if (arg0->searchResultsVisible != 0) {
            Sprite_SetAnimCtrlSeq(arg0->avatarSprites[1], 0x10);
            arg0->currentScreenInstruction = 2;
            ov70_02238E50(arg0, 3, 0x10);
            arg0->selectedSearchResult = 0;
        }
        Sprite_SetDrawFlag(arg0->unk_F10, 0);
    }
    return 3;
}

s32 ov70_0223CA18(GTSApplicationState *arg0);

s32 ov70_0223CA18(GTSApplicationState *arg0) {
    arg0->unk_F14 = (s32)(arg0->unk_F14 - 1);
    if (IsPaletteFadeFinished() != 0) {
        arg0->unk_F14 = 0;
        arg0->currentScreenInstruction = 1;
    }
    return 3;
}
