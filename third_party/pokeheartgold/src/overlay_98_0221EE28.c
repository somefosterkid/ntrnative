#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "msgdata.h"
#include "overlay_98.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_98_ov98text_sys.h"

void ov98_0221EE28(Ov98TextSys *textSys, s32 windowIdx, u32 msgId) {
    MsgData *msgData;

    msgData = textSys->commonMsgData;
    if (0x1ed < msgId) {
        GF_AssertFail();
    }
    ReadMsgDataIntoString(msgData, msgId, textSys->string);
    FillWindowPixelBuffer(&textSys->windows[windowIdx], 0);
    AddTextPrinterParameterizedWithColor(&textSys->windows[windowIdx], FONT_SYSTEM, textSys->string, 0, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(&textSys->windows[windowIdx]);
}

void ov98_0221EE84(Ov98TextSys *textSys, s32 windowIdx) {
    ov98_0221EF14(textSys, windowIdx);
    ScheduleWindowCopyToVram(&textSys->windows[windowIdx]);
}

void ov98_0221EE9C(Ov98TextSys *textSys, s32 windowIdx, String *string, u32 x, u8 y) {
    ov98_0221EF14(textSys, windowIdx);
    AddTextPrinterParameterizedWithColor(&textSys->windows[windowIdx], FONT_SYSTEM, string, x, (u32)y, 0xff, 0x10200, (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(&textSys->windows[windowIdx]);
}

void ov98_0221EEDC(Ov98TextSys *textSys, s32 windowIdx) {
    ClearWindowTilemapAndScheduleTransfer(&textSys->windows[windowIdx]);
}

void ov98_0221EEEC(Ov98TextSys *textSys, s32 windowIdx, u8 y) {
    SetWindowY(&textSys->windows[windowIdx], y);
}

MessageFormat *ov98_0221EEFC(Ov98TextSys *textSys) {
    if (textSys == NULL) {
        GF_AssertFail();
    }
    if (textSys->msgFmt == NULL) {
        GF_AssertFail();
    }
    return textSys->msgFmt;
}
