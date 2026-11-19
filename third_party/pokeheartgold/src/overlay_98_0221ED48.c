#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_98.h"
#include "overlay_98_ov98text_sys.h"

void ov98_0221ED48(Ov98TextSys *textSys, s32 windowIdx, s32 msgId, u32 x, u8 y) {
    String *src;

    src = NewString_ReadMsgData(textSys->msgData, msgId);
    StringExpandPlaceholders(textSys->msgFmt, textSys->string, src);
    ov98_0221EF14(textSys, windowIdx);
    AddTextPrinterParameterizedWithColor(&textSys->windows[windowIdx], FONT_SYSTEM, textSys->string, x, (u32)y, 0xff, 0x10200, (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(&textSys->windows[windowIdx]);
    String_Delete(src);
}

void ov98_0221EDA4(Ov98TextSys *textSys, s32 num, u32 numDigits, u32 idx) {
    BufferIntegerAsString(textSys->msgFmt, idx, num, numDigits, PRINTING_MODE_RIGHT_ALIGN, 1);
}
