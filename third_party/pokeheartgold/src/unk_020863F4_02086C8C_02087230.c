#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

void sub_02087230(Window *arg0, s32 arg1);

void sub_02087230(Window *window, s32 strno) {
    MsgData *msgData;
    String *string;

    FillWindowPixelBuffer(window, 0xFU);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x26, HEAP_ID_108);
    string = NewString_ReadMsgData(msgData, strno);
    FillWindowPixelBuffer(window, 0xFU);
    AddTextPrinterParameterized(window, FONT_MESSAGE, string, 0U, 0U, 0U, NULL);
    CopyWindowToVram(window);
    String_Delete(string);
    DestroyMsgData(msgData);
}
