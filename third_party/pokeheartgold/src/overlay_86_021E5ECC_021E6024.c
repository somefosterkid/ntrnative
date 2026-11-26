#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_86_frontier_records_app.h"
#include "pm_string.h"

void ov86_021E6024(FrontierRecordsApp *arg0, s32 arg1, s32 strno, s32 arg3, s32 arg4, enum Font font, s32 arg6, s32 arg7) {
    String *string;

    string = NewString_ReadMsgData(arg0->msgData, strno);
    ov86_021E5FD8(&arg0->windows[arg1], string, arg3, arg4, font, arg6, arg7);
    String_Delete(string);
}

void ov86_021E6064(FrontierRecordsApp *arg0, s32 arg1, s32 strno, s32 arg3, s32 arg4, enum Font font, s32 arg6, s32 arg7) {
    String *string;

    string = NewString_ReadMsgData(arg0->msgData, strno);
    StringExpandPlaceholders(arg0->msgFmt, arg0->string, string);
    ov86_021E5FD8(&arg0->windows[arg1], arg0->string, arg3, arg4, font, arg6, arg7);
    String_Delete(string);
}
