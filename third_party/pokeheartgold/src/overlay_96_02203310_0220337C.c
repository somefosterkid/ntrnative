#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "overlay_96_ov96_02203468work.h"

void ov96_0220337C(Ov96_02203468Work *param_1, s32 param_2);

void ov96_0220337C(Ov96_02203468Work *param_1, s32 param_2) {
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *string;

    FillWindowPixelBuffer(&param_1->windows[0], 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, param_1->heapID);
    messageFormat = MessageFormat_New(param_1->heapID);
    BufferIntegerAsString(messageFormat, 0, param_2, 3, PRINTING_MODE_LEFT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0xa4, param_1->heapID);
    AddTextPrinterParameterizedWithColor(&param_1->windows[0], FONT_UNK_4, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(&param_1->windows[0]);
    return;
}
