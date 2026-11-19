#include "global.h"
struct BattleCastleSelfApp;

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_02247998(Window *, String *, s32, s32, s32, s32, s32);

void ov83_02241DD8(struct BattleCastleSelfApp *arg0, Window *arg1, MsgData *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void ov83_02241DD8(BattleCastleSelfApp *arg0, Window *window, MsgData *msgData, s32 strno, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    StringExpandPlaceholders(arg0->strTemplate, arg0->displayStr, string);
    ov83_02247998(window, arg0->displayStr, arg4, arg5, arg6, arg7, arg8);
    String_Delete(string);
}
