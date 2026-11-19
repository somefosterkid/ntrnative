#include "global.h"

#include "render_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "text.h"
#include "yes_no_prompt.h"
#include "overlay_70_gtsapplication_state.h"
void ov70_022420C4(void *, s16);

u32 ov70_0223EFE0(GTSApplicationState *param_1);

s32 ov70_02238D84(GTSApplicationState *, s32, s32);
s32 ov70_0223E8E8(GTSApplicationState *, s32, s32, s32, s32);
s32 ov70_0223F0B4(GTSApplicationState *arg0);
struct YesNoPrompt *ov70_02238C14(void *, s32, s32, s32, u8);
s32 ov70_0223F0DC(GTSApplicationState *arg0);
s32 ov70_02238C8C(GTSApplicationState *arg0);
s32 ov70_02238E50(void *, s32, s32);
s32 ov70_0223F7A4(void *, GTSApplicationState *);
s32 ov70_0223F108(GTSApplicationState *arg0);
s32 ov70_0223F164(GTSApplicationState *arg0);
s32 ov70_0223F1B8(GTSApplicationState *arg0);
s32 ov70_02245084(void *, String *, s32, s32, s32, s32);
void ov70_0223F1D8(void *arg0, MsgData *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void ov70_0223F20C(Window *arg0, MsgData *arg1, s32 arg2, u32 arg3, u32 arg4);
void ov70_0223F244(void *arg0, MsgData *arg1, MsgData *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern void *ov70_022465EC;
s32 ov70_0223F2A0(s32 arg0, s32 arg1);

u32 ov70_0223EFE0(GTSApplicationState *param_1) {
    ov70_022420C4(param_1->unk_11A8, 2);
    param_1->listMenuCursorIndex = 0xffff;
    param_1->currentScreenInstruction = 0xc;
    return 3;
}

s32 ov70_0223EDE4(void *, s32);

s32 ov70_0223F3D8(void *, void *, void *, void *, s32, s32, s32);

s32 ov70_0223F828(void *, s32, s32);

s32 ov70_0223F864(s32, s32, s32);

s32 ov70_02242144(void *);

s32 ov70_0223F008(GTSApplicationState *arg0);

s32 ov70_0223F008(GTSApplicationState *arg0) {
    u32 v0;

    v0 = ov70_02242144(arg0->unk_11A8);
    switch (v0) {
    case -1:
        break;
    case 12:
    case -2:
        ClearFrameAndWindow2((void *)&arg0->bottomInstructionWindow, 0);
        if (ov70_0223EDE4((void *)&arg0->depositRequirements.species, *(s32 *)((u8 *)(arg0->speciesMenuState) + 0x20)) != 0) {
            arg0->currentScreenInstruction = 0;
        } else {
            arg0->currentScreenInstruction = 7;
        }
        break;
    default:
        ov70_0223F828((void *)&arg0->depositRequirements.species, v0, 0);
        arg0->currentScreenInstruction = 0xD;
        ov70_0223F3D8(arg0->gtsMessageLoader, arg0->speciesMessageLoader, arg0->stringTemplate, (void *)&arg0->infoWindows[13], (s32) arg0->depositRequirements.species, (s32) arg0->depositRequirements.gender, ov70_0223F864(arg0->depositRequirements.level, arg0->depositRequirements.level2, 0));
        break;
    }
    return 3;
}

s32 ov70_0223F0B4(GTSApplicationState *arg0) {
    ov70_0223E8E8(arg0, 0x17, 1, 0, 0xF0F);
    ov70_02238D84(arg0, 0x10, 0xE);
    return 3;
}

s32 ov70_0223F0DC(GTSApplicationState *arg0) {
    arg0->unk_11C8 = ov70_02238C14(arg0->bgConfig, 0xC, 0x11A, 3, 0);
    arg0->currentScreenInstruction = 0xF;
    return 3;
}

s32 ov70_0223F108(GTSApplicationState *arg0) {
    s32 v0;

    v0 = ov70_02238C8C(arg0);
    if (v0 == 1) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        ov70_02238E50(arg0, 7, 7);
        arg0->currentScreenInstruction = 2;
        arg0->fadeBothScreens = 1;
        ov70_0223F7A4(&arg0->receivedListing, arg0);
    } else if (v0 == 2) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        ov70_02238E50(arg0, 5, 5);
        arg0->currentScreenInstruction = 2;
    }
    return 3;
}

s32 ov70_0223F164(GTSApplicationState *arg0) {
    if (arg0->nextScreen == 0) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
        arg0->fadeBothScreens = 1;
    } else {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
    }
    arg0->currentScreenInstruction = 0;
    return 4;
}

s32 ov70_0223F1B8(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinter) == 0) {
        arg0->currentScreenInstruction = (s32)arg0->nextScreenInstruction;
    }
    return 3;
}

void ov70_0223F1D8(void *arg0, MsgData *msgData, s32 strno, s32 arg3, s32 arg4, s32 arg5) {
    String *string;

    if (strno != 0) {
        string = NewString_ReadMsgData(msgData, strno);
        ov70_02245084(arg0, string, 0, arg4, arg3, arg5);
        String_Delete(string);
    }
}

void ov70_0223F20C(Window *window, MsgData *msgData, s32 strno, u32 y, u32 color) {
    String *string;

    if (strno != 0) {
        string = NewString_ReadMsgData(msgData, strno);
        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 0U, y, 0xFFU, color, NULL);
        String_Delete(string);
    }
}

void ov70_0223F244(void *arg0, MsgData *msgData, MsgData *arg2, s32 strno, s32 arg4, s32 arg5, s32 arg6) {
    String *string;
    String *v0;

    if (strno != 0) {
        string = NewString_ReadMsgData(msgData, strno);
        ov70_02245084(arg0, string, 0, arg5, arg4, arg6);
        String_Delete(string);
        return;
    }
    v0 = NewString_ReadMsgData(arg2, 0xAD);
    ov70_02245084(arg0, v0, 0, arg5, arg4, arg6);
    String_Delete(v0);
}

s32 ov70_0223F2A0(s32 arg0, s32 arg1) {
    s32 v0;

    v0 = arg1;
    if (arg0 == 1) {
        return *(s32 *)(&ov70_022465EC);
    }
    if (arg0 == 2) {
        v0 = *(s32 *)((u8 *)(&ov70_022465EC) + 4);
    }
    return v0;
}
