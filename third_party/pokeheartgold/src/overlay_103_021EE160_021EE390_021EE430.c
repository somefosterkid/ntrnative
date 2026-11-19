#include "global.h"

#include "msgdata.h"
#include "pm_string.h"
#include "overlay_103_mail_box_view.h"

s32 ov103_021EE3E4(void *, String *, s32, s32, s32, s32, s32);

void ov103_021EE430(MailBoxView *view, MsgData *msgData, s32 windowIdx, s32 strno, u32 x, u32 y, enum Font font, u32 color, s32 align) {
    String *string;
    string = NewString_ReadMsgData(msgData, strno);
    ov103_021EE3E4(&view->windows[windowIdx], string, x, y, font, color, align);
    String_Delete(string);
}
