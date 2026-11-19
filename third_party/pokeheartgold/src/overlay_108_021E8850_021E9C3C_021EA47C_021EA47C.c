#include "global.h"
#include "overlay_108_021E8850.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021EA47C(SafariDecorationApp *arg0) {
    u8 v0;
    u8 v1;

    ov108_021EA624(arg0);
    FillWindowPixelBuffer(&arg0->windows[5], 0U);
    v0 = arg0->cursor;
    v1 = v0 + (6 * arg0->page);
    if (((u32)v0 >= 6U) || ((u32)v1 >= (u32) arg0->numObjects)) {
        ScheduleWindowCopyToVram(&arg0->windows[5]);
        return;
    }
    ReadMsgDataIntoString(arg0->msgData, arg0->objects[v1].id + 0x26, arg0->strBuf1);
    AddTextPrinterParameterizedWithColor(&arg0->windows[5], FONT_SYSTEM, arg0->strBuf1, 0U, 0U, 0xFFU, 0x10200U, NULL);
    ScheduleWindowCopyToVram(&arg0->windows[5]);
}
