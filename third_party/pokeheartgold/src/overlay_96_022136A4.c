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

#include "overlay_96_app_02210C98.h"

void ov96_022136A4(Ov96App_02210C98 *param_1, s32 param_2);

void ov96_022136A4(Ov96App_02210C98 *param_1, s32 param_2) {
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *string;

    FillWindowPixelBuffer(&param_1->unk_8[0], 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, param_1->unk_58);
    messageFormat = MessageFormat_New(param_1->unk_58);
    BufferIntegerAsString(messageFormat, 0, param_2, 3, PRINTING_MODE_LEFT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0xa5, param_1->unk_58);
    AddTextPrinterParameterizedWithColor(&param_1->unk_8[0], FONT_UNK_4, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(&param_1->unk_8[0]);
    return;
}
