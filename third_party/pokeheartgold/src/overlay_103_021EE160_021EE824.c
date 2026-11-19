#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

s32 ov103_021EE784(void *, s32);

s32 ov103_021EE7DC(void *);

void ov103_021EE824(MailBoxWork *arg0);

void ov103_021EE824(MailBoxWork *arg0) {
    String *string;
    MailBoxView *v0;
    ov103_021EE784(arg0, 0);
    string = NewString_ReadMsgData(arg0->view->msgData, 9);
    v0 = arg0->view;
    StringExpandPlaceholders(v0->msgFmt, v0->string, string);
    String_Delete(string);
    ov103_021EE7DC(arg0);
}
