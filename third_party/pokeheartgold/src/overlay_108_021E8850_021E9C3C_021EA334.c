#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_108_021E8850.h"
#include "render_window.h"
#include "text.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021EA334(SafariDecorationApp *arg0, s32 arg1) {
    s32 strno;
    s32 v0;
    u8 num;

    v0 = 0;
    strno = arg1;
    FillWindowPixelBuffer(arg0->windows, 0U);
    switch (strno) {
    case 0:
        ReadMsgDataIntoString(arg0->msgData, arg0->selectedId + 0xE, arg0->strBuf0);
        BufferString(arg0->msgFormat, 0U, arg0->strBuf0, 1, 0, 2);
        v0 = 1;
        break;
    case 2:
        num = arg0->freeObjectSlots;
        if (num == 0) {
            strno = 3;
        } else {
            BufferIntegerAsString(arg0->msgFormat, 0U, (s32)num, 2U, PRINTING_MODE_RIGHT_ALIGN, 1);
            v0 = 1;
        }
        break;
    }
    if (v0 != 0) {
        ReadMsgDataIntoString(arg0->msgData, strno, arg0->strBuf0);
        StringExpandPlaceholders(arg0->msgFormat, arg0->strBuf1, arg0->strBuf0);
    } else {
        ReadMsgDataIntoString(arg0->msgData, strno, arg0->strBuf1);
    }
    AddTextPrinterParameterizedWithColor(arg0->windows, FONT_SYSTEM, arg0->strBuf1, 0U, 0U, 0xFFU, 0x30400U, NULL);
    ScheduleWindowCopyToVram(arg0->windows);
}

void ov108_021EA418(SafariDecorationApp *arg0, s32 arg1) {
    FillWindowPixelBuffer(&arg0->windows[1], 0xFU);
    ReadMsgDataIntoString(arg0->msgData, arg1 + 4, arg0->strBuf1);
    DrawFrameAndWindow2(&arg0->windows[1], 1, 0x3E2U, 0xCU);
    AddTextPrinterParameterizedWithColor(&arg0->windows[1], FONT_MESSAGE, arg0->strBuf1, 0U, 0U, 0xFFU, 0x1020FU, NULL);
    ScheduleWindowCopyToVram(&arg0->windows[1]);
}
