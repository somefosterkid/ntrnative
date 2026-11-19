#include "global.h"

#include "bg_window.h"
#include "message_printer.h"
#include "overlay_111.h"
#include "unk_0200CE7C.h"

void ov111_021E69A0(SwapMonText *text, Window *window, u32 num, u32 digits, s32 useStatFont, s32 unk) {
    FillWindowPixelBuffer(window, 0U);
    if (useStatFont != 0) {
        FontSpecialChars_DrawPartyScreenText(text->msgPrinter, unk, num, digits, PRINTING_MODE_RIGHT_ALIGN, window, 0U, 0U);
    } else {
        PrintUIntOnWindow(text->msgPrinter, num, digits, PRINTING_MODE_RIGHT_ALIGN, window, 0U, 0U);
    }
    ScheduleWindowCopyToVram(window);
}
