#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

void ov96_02219BDC(Window *param_1, MessageFormat *param_2, MsgData *param_3, u32 param_4, enum HeapID param_5, s16 param_6, s16 param_7);

void ov96_02219BDC(Window *param_1, MessageFormat *param_2, MsgData *param_3, u32 param_4, enum HeapID param_5, s16 param_6, s16 param_7) {
    String *string;

    FillWindowPixelBuffer(param_1, 0);
    string = ReadMsgData_ExpandPlaceholders(param_2, param_3, param_4, param_5);
    AddTextPrinterParameterizedWithColor(param_1, FONT_SYSTEM, string, (s32)param_6, (s32)param_7, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram(param_1);
    return;
}
