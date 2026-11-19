#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

void ov96_021F70AC(UnkStruct_ov96_021F5018 *param_1, s32 param_2);

void ov96_021F70AC(UnkStruct_ov96_021F5018 *param_1, s32 param_2) {
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *string;

    FillWindowPixelBuffer(&param_1->windows[4], 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, param_1->heapID);
    messageFormat = MessageFormat_New(param_1->heapID);
    BufferIntegerAsString(messageFormat, 0, param_2, 3, PRINTING_MODE_RIGHT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0xa1, param_1->heapID);
    AddTextPrinterParameterizedWithColor(&param_1->windows[4], FONT_UNK_4, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(&param_1->windows[4]);
    return;
}

void ov96_021F7130(UnkStruct_ov96_021F5018_Ball *arg0);

void ov96_021F7130(UnkStruct_ov96_021F5018_Ball *arg0) {
    s32 v0;
    s32 v1;
    s32 v2;

    v2 = arg0->unk_18;
    if (v2 != 0) {
        if (v2 > 0) {
            v0 = v2 - 0x2000;
            arg0->unk_18 = v0;
            if (v0 < 0) {
                arg0->unk_18 = 0;
            }
        } else if (v2 < 0) {
            v1 = v2 + 0x2000;
            arg0->unk_18 = v1;
            if (v1 > 0) {
                arg0->unk_18 = 0;
            }
        }
    }
}
