#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "screen_fade.h"
#include "text.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov73_021E83EC(void *, s32, s32);

s32 ov73_021E9FF8(void *, MsgData *, s32, s32, s32);

s32 ov73_021E9CD8(WifiBattleTowerAppState *arg0);

s32 ov73_021E9CD8(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader3, 0x1B, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x31, 0x2E);
    arg0->waitTimer = 0;
    return 3;
}

s32 ov73_021E9D10(void);

s32 ov73_021E9D10(void) {
    return 3;
}

s32 ov73_021E9D14(WifiBattleTowerAppState *arg0);

s32 ov73_021E9D14(WifiBattleTowerAppState *arg0) {
    sub_0203A914();
    sub_0203946C();
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 8, 1, HEAP_ID_150);
    arg0->state = NINTENDO_WFC_STATE_ASK_CONNECT;
    return 4;
}

s32 ov73_021E83D4(YesNoPrompt *);

s32 ov73_021E9D44(WifiBattleTowerAppState *arg0);

s32 ov73_021E9D44(WifiBattleTowerAppState *arg0) {
    s32 v0;

    v0 = ov73_021E83D4(arg0->yesNoPrompt);
    if (v0 != 0) {
        if (v0 == 2) {
            arg0->state = NINTENDO_WFC_STATE_ASK_CONNECT;
        } else {
            arg0->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
        }
    }
    return 3;
}

s32 ov73_021EA180(void *);

s32 ov73_021E9D6C(WifiBattleTowerAppState *arg0);

s32 ov73_021E9D6C(WifiBattleTowerAppState *arg0) {
    s32 v1;
    u32 v0;

    v1 = 0;
    v0 = arg0->asyncResult + 7;
    switch (v0) {
    case 1:
    case 5:
        v1 = 0xD;
        break;
    case 8:
        v1 = 0xB;
        break;
    case 6:
    case 9:
        v1 = 0xC;
        break;
    case 0:
    case 3:
        v1 = 0xE;
        break;
    case 4:
        v1 = 0xF;
        break;
    }
    ov73_021EA180(arg0);
    ov73_021E9FF8(arg0, arg0->msgLoader1, v1, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0x36);
    return 3;
}

s32 ov73_021E844C(void *, s32, s32);

s32 ov73_021E9DE0(WifiBattleTowerAppState *arg0);

s32 ov73_021E9DE0(WifiBattleTowerAppState *arg0) {
    s16 v0;

    v0 = arg0->subStep;
    switch (v0) {
    case 0:
        ov73_021E9FF8(arg0, arg0->msgLoader1, 0x10, 1, 0xF0F);
        arg0->subStep = (s16)(arg0->subStep + 1);
        break;
    case 1:
        if (TextPrinterCheckActive((u8) arg0->textPrinterID) == 0) {
            sub_0203946C();
            DWC_CleanupInet();
            arg0->subStep = (s16)(arg0->subStep + 1);
        }
        break;
    case 2:
        ov73_021E9FF8(arg0, arg0->msgLoader1, 0x11, 1, 0xF0F);
        arg0->subStep = (s16)(arg0->subStep + 1);
        break;
    case 3:
        if (TextPrinterCheckActive((u8) arg0->textPrinterID) == 0) {
            arg0->subStep = (s16)(arg0->subStep + 1);
        }
        break;
    default:
        arg0->subTimer = (s16)(arg0->subTimer + 1);
        if ((s32) arg0->subTimer > 0x1E) {
            ov73_021E844C(arg0, 0, 0);
            arg0->state = NINTENDO_WFC_STATE_START_EXIT;
        }
        break;
    }
    return 3;
}

s32 ov73_021E9EAC(WifiBattleTowerAppState *arg0);

s32 ov73_021E9EAC(WifiBattleTowerAppState *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->textPrinterID) == 0) {
        arg0->state = arg0->nextState;
    }
    return 3;
}

s32 ov73_021E9ECC(WifiBattleTowerAppState *arg0);

s32 ov73_021E9ECC(WifiBattleTowerAppState *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->textPrinterID) == 0) {
        if ((s32) * &arg0->waitTimer > 0x1E) {
            arg0->state = arg0->nextState;
        }
        arg0->waitTimer = (s32)(arg0->waitTimer + 1);
    }
    return 3;
}

YesNoPrompt *ov73_021E8390(BgConfig *, u8, s32);

s32 ov73_021E9F00(WifiBattleTowerAppState *arg0);

s32 ov73_021E9F00(WifiBattleTowerAppState *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->textPrinterID) == 0) {
        arg0->yesNoPrompt = ov73_021E8390(arg0->bgConfig, 0xA, 0x244);
        arg0->state = arg0->nextState;
    }
    return 3;
}

void ov73_021E9F34(WifiBattleTowerAppState *arg0, MsgData *arg1, s32 arg2, u32 arg3);

void ov73_021E9F34(WifiBattleTowerAppState *arg0, MsgData *msgData, s32 strno, u32 textSpeed) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    StringExpandPlaceholders(arg0->stringTemplate, arg0->msgString, string);
    FillWindowPixelBuffer(&arg0->msgWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->msgWindow, 0, 1U, 0xEU);
    arg0->textPrinterID = (s32)AddTextPrinterParameterized(&arg0->msgWindow, FONT_MESSAGE, arg0->msgString, 0U, 0U, textSpeed, NULL);
    String_Delete(string);
}
