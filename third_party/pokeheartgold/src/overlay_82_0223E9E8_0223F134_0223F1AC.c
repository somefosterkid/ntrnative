#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

void ov82_0223F1AC(void *param_1, Window *window, s32 param_3, s32 param_4, u8 param_5, u8 param_6, u8 param_7, u8 param_8);

void ov82_0223F1AC(void *param_1, Window *window, s32 param_3, s32 param_4, u8 param_5, u8 param_6, u8 param_7, u8 param_8) {
    MsgData *msgData;
    String *string;

    FillWindowPixelBuffer(window, param_7);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1b9, HEAP_ID_105);
    string = NewString_ReadMsgData(msgData, 0x25);
    FillWindowPixelBuffer(window, 0xf);
    AddTextPrinterParameterizedWithColor(window, (enum Font)param_8, string, param_3, param_4, 0, (param_5 & 0xff) << 16 | (param_6 & 0xff) << 8 | param_7 & 0xff, NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}
