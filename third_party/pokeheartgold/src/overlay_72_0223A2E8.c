#include "global.h"

#include "overlay_00_thumb.h"
#include "bg_window.h"
#include "msgdata.h"
#include "render_window.h"
#include "text.h"
#include "overlay_72_wfc_state.h"
#include "overlay_72_wfc_connect.h"

void ov72_0223A4DC(NintendoWfcConnect *, s32, s32);

u32 ov72_0223A2E8(NintendoWfcConnect *param_1);

s32 ov72_0223A314(NintendoWfcConnect *arg0);
void ov72_0223A350(NintendoWfcConnect *arg0, MsgData *arg1, s32 arg2, u32 arg3);

u32 ov72_0223A2E8(NintendoWfcConnect *param_1) {
    s32 sVar1;

    sVar1 = ov00_021E6A70(-param_1->dwcErrorCode, param_1->dwcErrorType);
    ov72_0223A4DC(param_1, sVar1, -param_1->dwcErrorCode);
    param_1->state = NINTENDO_WFC_STATE_WAIT_ERROR_DISMISS;
    return 3;
}

s32 ov72_0223A314(NintendoWfcConnect *arg0) {
    if ((1 & gSystem.newKeys) || (2 & gSystem.newKeys) || (gSystem.touchNew != 0)) {
        ClearWindowPixelBuffer(&arg0->errorWindow, 0);
        arg0->state = NINTENDO_WFC_STATE_ASK_CONNECT;
    }
    return 3;
}

void ov72_0223A350(NintendoWfcConnect *arg0, MsgData *msgData, s32 strno, u32 textSpeed) {
    ReadMsgDataIntoString(msgData, strno, arg0->msgString);
    FillWindowPixelBuffer(&arg0->msgWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->msgWindow, 0, 1U, 0xEU);
    arg0->textPrinterId = (s32)AddTextPrinterParameterized(&arg0->msgWindow, FONT_MESSAGE, arg0->msgString, 0U, 0U, textSpeed, NULL);
}
