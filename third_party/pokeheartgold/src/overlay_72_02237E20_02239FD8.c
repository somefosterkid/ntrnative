#include "global.h"
#include "overlay_72_wfc_state.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "screen_fade.h"
#include "text.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "overlay_72_wfc_connect.h"
struct NintendoWfcConnect;

s32 ov72_02238680(void *, s32, s32);

s32 ov72_0223A350(void *, MsgData *, s32, s32, s32);

s32 ov72_02239FD8(NintendoWfcConnect *arg0);

s32 ov72_02239FD8(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataError, 0x1B, 1, 0xF0F);
    ov72_02238680(arg0, 0x31, 0x2E);
    arg0->messageTimer = 0;
    return 3;
}

s32 ov72_0223A010(void);

s32 ov72_0223A010(void) {
    return 3;
}

s32 ov72_0223A014(NintendoWfcConnect *arg0);

s32 ov72_0223A014(NintendoWfcConnect *arg0) {
    sub_0203A914();
    sub_0203946C();
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 8, 1, HEAP_ID_67);
    arg0->state = NINTENDO_WFC_STATE_ASK_CONNECT;
    return 4;
}

s32 ov72_02238668(YesNoPrompt *);

s32 ov72_0223A044(NintendoWfcConnect *arg0);

s32 ov72_0223A044(NintendoWfcConnect *arg0) {
    s32 v0;

    v0 = ov72_02238668(arg0->yesNoPrompt);
    if (v0 != 0) {
        if (v0 == 2) {
            arg0->state = NINTENDO_WFC_STATE_ASK_CONNECT;
        } else {
            arg0->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
        }
    }
    return 3;
}

s32 ov72_0223A444(NintendoWfcConnect *);

s32 ov72_0223A06C(NintendoWfcConnect *arg0);

s32 ov72_0223A06C(NintendoWfcConnect *arg0) {
    s32 v1;
    u32 v0;

    v1 = 0;
    v0 = arg0->httpResult + 7;
    switch (v0) {
    case 1:
    case 5:
        v1 = 0xF;
        break;
    case 8:
        v1 = 0xD;
        break;
    case 6:
    case 9:
        v1 = 0xE;
        break;
    case 0:
    case 3:
        v1 = 0x10;
        break;
    case 4:
        v1 = 0x11;
        break;
    }
    ov72_0223A444(arg0);
    ov72_0223A350(arg0, arg0->msgDataTower, v1, 1, 0xF0F);
    ov72_02238680(arg0, 0x2F, 0x36);
    return 3;
}

s32 ov72_022387D0(struct NintendoWfcConnect *, s32, s32);

s32 ov72_0223A0E0(NintendoWfcConnect *arg0);

s32 ov72_0223A0E0(NintendoWfcConnect *arg0) {
    s16 v0;

    v0 = arg0->subState;
    switch (v0) {
    case 0:
        ov72_0223A350(arg0, arg0->msgDataTower, 0x12, 1, 0xF0F);
        arg0->subState = (s16)(arg0->subState + 1);
        break;
    case 1:
        if (TextPrinterCheckActive((u8) arg0->textPrinterId) == 0) {
            sub_0203946C();
            DWC_CleanupInet();
            arg0->subState = (s16)(arg0->subState + 1);
        }
        break;
    case 2:
        ov72_0223A350(arg0, arg0->msgDataTower, 0x13, 1, 0xF0F);
        arg0->subState = (s16)(arg0->subState + 1);
        break;
    case 3:
        if (TextPrinterCheckActive((u8) arg0->textPrinterId) == 0) {
            arg0->subState = (s16)(arg0->subState + 1);
        }
        break;
    default:
        arg0->subTimer = (s16)(arg0->subTimer + 1);
        if ((s32) arg0->subTimer > 0x1E) {
            ov72_022387D0(arg0, 0, 0);
            arg0->state = NINTENDO_WFC_STATE_START_EXIT;
        }
        break;
    }
    return 3;
}

s32 ov72_0223A1AC(NintendoWfcConnect *arg0);

s32 ov72_0223A1AC(NintendoWfcConnect *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinterId) == 0) {
        arg0->state = arg0->nextState;
    }
    return 3;
}

s32 ov72_0223A1CC(NintendoWfcConnect *arg0);

s32 ov72_0223A1CC(NintendoWfcConnect *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinterId) == 0) {
        if ((s32)arg0->messageTimer > 0x1E) {
            arg0->state = arg0->nextState;
        }
        arg0->messageTimer = (s32)(arg0->messageTimer + 1);
    }
    return 3;
}

s32 ov72_0223AF1C(NintendoWfcYesNoMenu *);

s32 ov72_0223A200(NintendoWfcConnect *arg0);

s32 ov72_0223A200(NintendoWfcConnect *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinterId) == 0) {
        arg0->yesNoMenu.cursorSprite = arg0->sprites[8];
        arg0->yesNoMenu.bgConfig = arg0->bgConfig;
        arg0->yesNoMenu.heapID = HEAP_ID_67;
        arg0->yesNoMenu.initialChoice = 0;
        arg0->yesNoMenu.bgPltt = 0xA;
        arg0->yesNoMenu.bgLayer = 6;
        arg0->yesNoMenu.textBgLayer = 4;
        arg0->yesNoMenu.fontPltt = 0xB;
        arg0->yesNoMenu.baseTile = 1;
        arg0->yesNoMenu.cursorOffsetY = 0x100;
        ov72_0223AF1C(&arg0->yesNoMenu);
        arg0->state = arg0->nextState;
    }
    return 3;
}

void ov72_0223A280(NintendoWfcConnect *arg0, MsgData *arg1, s32 arg2, u32 arg3);

void ov72_0223A280(NintendoWfcConnect *arg0, MsgData *msgData, s32 strno, u32 textSpeed) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    StringExpandPlaceholders(arg0->msgFormat, arg0->msgString, string);
    FillWindowPixelBuffer(&arg0->msgWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->msgWindow, 0, 1U, 0xEU);
    arg0->textPrinterId = (s32)AddTextPrinterParameterized(&arg0->msgWindow, FONT_MESSAGE, arg0->msgString, 0U, 0U, textSpeed, NULL);
    String_Delete(string);
}
