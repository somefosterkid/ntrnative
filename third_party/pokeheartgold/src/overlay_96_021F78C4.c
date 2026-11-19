#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

void ov96_021F78C4(Window *param_1, enum HeapID param_2);

void ov96_021F78C4(Window *param_1, enum HeapID param_2) {
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *string;

    FillWindowPixelBuffer(param_1, 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, param_2);
    messageFormat = MessageFormat_New(param_2);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0x12f, param_2);
    AddTextPrinterParameterizedWithColor(param_1, FONT_SYSTEM, string, 0, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(param_1);
    return;
}
