#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

void ov96_02209F40(Window *param_1, MessageFormat *param_2, MsgData *param_3, u32 param_4, enum HeapID param_5);

void ov96_02209F40(Window *param_1, MessageFormat *param_2, MsgData *param_3, u32 param_4, enum HeapID param_5) {
    String *string;

    FillWindowPixelBuffer(param_1, 0);
    string = ReadMsgData_ExpandPlaceholders(param_2, param_3, param_4, param_5);
    AddTextPrinterParameterizedWithColor(param_1, FONT_SYSTEM, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram(param_1);
    return;
}
