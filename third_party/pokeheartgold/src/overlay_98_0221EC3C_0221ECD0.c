#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_98.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

void ov98_0221ECD0(Ov98TextSys *textSys, s32 windowIdx, s32 msgId, s32 num, u32 numDigits, u32 idx) {
    String *src;

    BufferIntegerAsString(textSys->msgFmt, idx, num, numDigits, PRINTING_MODE_RIGHT_ALIGN, 1);
    src = NewString_ReadMsgData(textSys->msgData, msgId);
    StringExpandPlaceholders(textSys->msgFmt, textSys->string, src);
    ov98_0221EF14(textSys, windowIdx);
    AddTextPrinterParameterizedWithColor(&textSys->windows[windowIdx], FONT_SYSTEM, textSys->string, 0, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(&textSys->windows[windowIdx]);
    String_Delete(src);
}
