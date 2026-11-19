#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "render_window.h"
#include "text.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov73_021E6A70(s32, s32);

s32 ov73_021EA218(WifiBattleTowerAppState *, s32, s32);

s32 ov73_021E9F9C(WifiBattleTowerAppState *arg0);

s32 ov73_021E9FC8(WifiBattleTowerAppState *arg0);
void ov73_021E9FF8(WifiBattleTowerAppState *arg0, MsgData *arg1, s32 arg2, u32 arg3);

s32 ov73_021E9F9C(WifiBattleTowerAppState *arg0) {
    s32 tmp;
    tmp = ov73_021E6A70(0 - arg0->dwcErrorDetail, arg0->dwcErrorType);
    ov73_021EA218(arg0, tmp, 0 - arg0->dwcErrorDetail);
    arg0->state = NINTENDO_WFC_STATE_WAIT_ERROR_DISMISS;
    return 3;
}

s32 ov73_021E9FC8(WifiBattleTowerAppState *arg0) {
    if ((1 & gSystem.newKeys) || (2 & gSystem.newKeys)) {
        ClearWindowPixelBuffer(&arg0->popupWindow, 0);
        arg0->state = NINTENDO_WFC_STATE_ASK_CONNECT;
    }
    return 3;
}

void ov73_021E9FF8(WifiBattleTowerAppState *arg0, MsgData *msgData, s32 strno, u32 textSpeed) {
    ReadMsgDataIntoString(msgData, strno, arg0->msgString);
    FillWindowPixelBuffer(&arg0->msgWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->msgWindow, 0, 1U, 0xEU);
    arg0->textPrinterID = (s32)AddTextPrinterParameterized(&arg0->msgWindow, FONT_MESSAGE, arg0->msgString, 0U, 0U, textSpeed, NULL);
}
