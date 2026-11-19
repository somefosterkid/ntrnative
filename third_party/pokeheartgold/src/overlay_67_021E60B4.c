#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "font.h"
#include "text.h"
#include "overlay_67_app.h"

void ov67_021E60B4(Ov67App *app) {
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x1C, app->heapID);
    app->msgFmt = MessageFormat_New_Custom(2U, 0x40U, app->heapID);
    app->string = String_New(0x200U, app->heapID);
    app->defaultNames[0] = NewString_ReadMsgData(app->msgData, 0x21);
    app->defaultNames[1] = NewString_ReadMsgData(app->msgData, 0x22);
}

void ov67_021E60F4(Ov67App *app) {
    String_Delete(app->defaultNames[1]);
    String_Delete(app->defaultNames[0]);
    String_Delete(app->string);
    MessageFormat_Delete(app->msgFmt);
    DestroyMsgData(app->msgData);
}

void ov67_021E6118(Window *window, String *string, u32 arg2, u32 y, enum Font fontId, u32 color, s32 arg6) {
    u32 x;

    x = arg2;
    if (arg6 == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0U);
    } else if (arg6 == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0U) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFFU, color, NULL);
}
