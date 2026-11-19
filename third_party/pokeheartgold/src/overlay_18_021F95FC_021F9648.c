#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"

s32 ov18_021F95FC(Window *, String *, s32, s32, s32, s32, s32);

void ov18_021F9648(Window *arg0, MsgData *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

void ov18_021F9648(Window *arg0, MsgData *msgData, s32 strno, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    ov18_021F95FC(arg0, string, arg3, arg4, arg5, arg6, arg7);
    String_Delete(string);
}
