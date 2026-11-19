#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_67_app.h"
#include "pm_string.h"

void ov67_021E6164(Ov67App *app, s32 windowIdx, s32 strno, s32 x, s32 y, enum Font fontId, s32 color, s32 align) {
    String *string;

    string = NewString_ReadMsgData(app->msgData, strno);
    ov67_021E6118(&app->windows[windowIdx], string, x, y, fontId, color, align);
    String_Delete(string);
}

void ov67_021E61A0(Ov67App *app, s32 windowIdx, s32 strno, s32 x, s32 y, enum Font fontId, s32 color, s32 align) {
    String *string;

    string = NewString_ReadMsgData(app->msgData, strno);
    StringExpandPlaceholders(app->msgFmt, app->string, string);
    ov67_021E6118(&app->windows[windowIdx], app->string, x, y, fontId, color, align);
    String_Delete(string);
}
