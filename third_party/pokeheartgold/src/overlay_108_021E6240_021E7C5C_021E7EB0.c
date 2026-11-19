#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "text.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E7EB0(SafariAreaCustomizer *base);

void ov108_021E7EB0(SafariAreaCustomizer *base) {
    Window *win = &base->windows[4];
    u32 x;

    FillWindowPixelBuffer(win, 0xCU);
    ReadMsgDataIntoString(base->msgData, base->areaSet.areas[(base->areaIdx)].area_no + 0x10, base->string);
    x = (u32)((0x60 - FontID_String_GetWidth(FONT_SYSTEM, base->string, 0U)) << 23) >> 24;
    AddTextPrinterParameterizedWithColor(win, FONT_SYSTEM, base->string, x, 4U, 0xFFU, 0x80B0CU, NULL);
    if (base->cursor < 6U) {
        ReadMsgDataIntoString(base->msgData, base->cursor + 0x10 + (6 * base->page), base->string);
        x = (u32)((0x60 - FontID_String_GetWidth(FONT_SYSTEM, base->string, 0U)) << 23) >> 24;
        AddTextPrinterParameterizedWithColor(win, FONT_SYSTEM, base->string, x + 0x90, 4U, 0xFFU, 0x80B0CU, NULL);
    }
    ScheduleWindowCopyToVram(win);
}
