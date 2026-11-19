#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

void ov96_021F459C(enum HeapID *param_1, s32 param_2);

void ov96_021F459C(enum HeapID *param_1, s32 param_2) {
    String *string;

    FillWindowPixelBuffer((Window *)(param_1 + 0x13), 0);
    BufferIntegerAsString((MessageFormat *)param_1[0x18], 0, param_2, 2, PRINTING_MODE_LEFT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders((MessageFormat *)param_1[0x18], (MsgData *)param_1[0x17], 0x98, *param_1);
    AddTextPrinterParameterizedWithColor((Window *)(param_1 + 0x13), FONT_SYSTEM, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    return;
}
