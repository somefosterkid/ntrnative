#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_27.h"
#include "pm_string.h"
#include "text.h"

void ov27_0225C8D0(Window *window, MsgData *msgData, s32 strno) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    FillWindowPixelBuffer(window, 0U);
    AddTextPrinterParameterizedWithColor(window, FONT_UNK_4, string, 0U, 0U, 0xFFU, 0x20100U, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(string);
}

void ov27_0225C914(Ov27TaskData *data, u32 unused) {
    data->msgFormat = MessageFormat_New(HEAP_ID_8);
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xBF, HEAP_ID_8);
}

void ov27_0225C930(Ov27TaskData *data) {
    DestroyMsgData(data->msgData);
    MessageFormat_Delete(data->msgFormat);
}

s32 ov27_0225C944(Ov27TaskData *data) {
    data->state = 1;
    return 0;
}
