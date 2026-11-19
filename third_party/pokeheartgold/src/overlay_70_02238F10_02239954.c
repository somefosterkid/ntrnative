#include "global.h"

#include "bg_window.h"
#include "player_data.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "text.h"
#include "yes_no_prompt.h"
#include "overlay_70_gtsapplication_state.h"
#include "unk_struct_ov70_02241004.h"

s32 ov70_02241004(UnkStruct_ov70_02241004 *, u32);

s32 ov70_02239954(UnkStruct_ov70_02241004 *arg0);

s32 ov70_02239954(UnkStruct_ov70_02241004 *arg0) {
    ov70_02241004(arg0, PlayerProfile_GetTrainerGender(*(PlayerProfile **)((u8 *)(arg0->unk_0) + 0x1C)));
    arg0->unk_2C = 8;
    arg0->unk_11DC = 0;
    return 3;
}

s32 ov70_0223997C(GTSApplicationState *arg0);

s32 ov70_0223997C(GTSApplicationState *arg0) {
    if (arg0->hasAvatarFinishedMoving != 0) {
        arg0->currentScreenInstruction = 9;
    }
    return 3;
}

s32 ov70_02239990(void *arg0);

s32 ov70_02239990(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x18) == 0) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
    } else {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
    }
    *(s32 *)((u8 *)arg0 + 0x2C) = 0;
    return 4;
}

struct YesNoPrompt *ov70_02238C14(void *, s32, s32, s32, u8);

s32 ov70_022399D8(GTSApplicationState *arg0);

s32 ov70_022399D8(GTSApplicationState *arg0) {
    arg0->unk_11C8 = ov70_02238C14(arg0->bgConfig, 0xA, 0x102, 3, 1);
    arg0->currentScreenInstruction = 0xD;
    Sprite_SetDrawFlag(arg0->cursorSprite, 0);
    return 3;
}

s32 ov70_02238C8C(GTSApplicationState *arg0);

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_02239A10(GTSApplicationState *arg0);

s32 ov70_02239A10(GTSApplicationState *arg0) {
    s32 v0;

    v0 = ov70_02238C8C(arg0);
    if (v0 == 1) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        ClearFrameAndWindow2(&arg0->confirmationWindow, 1);
        ClearWindowTilemapAndCopyToVram(&arg0->confirmationWindow);
        ClearWindowTilemapAndCopyToVram(&arg0->unk_1198);
        ToggleBgLayer(6U, 0U);
        Sprite_SetDrawFlag(arg0->unk_F10, 0);
        ov70_02238E50(arg0, 0, 0);
        arg0->currentScreenInstruction = 7;
    } else if (v0 == 2) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        ClearFrameAndWindow2(&arg0->confirmationWindow, 1);
        ClearWindowTilemapAndCopyToVram(&arg0->confirmationWindow);
        Sprite_SetAnimActiveFlag(arg0->cursorSprite, 1);
        arg0->currentScreenInstruction = 5;
        Sprite_SetDrawFlag(arg0->cursorSprite, 1);
    }
    return 3;
}

s32 ov70_02239AAC(GTSApplicationState *arg0);

s32 ov70_02239AAC(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinter) == 0) {
        arg0->currentScreenInstruction = (s32)arg0->nextScreenInstruction;
    }
    return 3;
}

s32 ov70_02239ACC(GTSApplicationState *arg0);

s32 ov70_02239ACC(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinter) == 0) {
        arg0->unk_11C0 = (s32)(arg0->unk_11C0 + 1);
        if ((s32)arg0->unk_11C0 > 0x2D) {
            arg0->unk_11C0 = 0;
            arg0->currentScreenInstruction = (s32)arg0->nextScreenInstruction;
        }
    }
    return 3;
}
