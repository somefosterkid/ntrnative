#include "global.h"

#include "mail_message.h"
#include "pm_string.h"

/* HDR-OVERLAY {"Mail_GetUnk20Array": {"file": "include/mail.h", "new": "MailMessage *Mail_GetUnk20Array(Mail *mail, u8 i);", "old": "MailMessage *Mail_GetUnk20Array(Mail *mail, int i);"}} */
#define Mail_GetUnk20Array Mail_GetUnk20Array__hdr

/* HDR-OVERLAY defines done */
#include "mail.h"

/* HDR-OVERLAY end */
#undef Mail_GetUnk20Array

MailMessage *Mail_GetUnk20Array(Mail *mail, u8 i);

/* HDR-OVERLAY done */

#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"
s32 ov103_021EE3E4(void *, String *, s32, s32, s32, s32, s32);

void ov103_021EE9C8(MailBoxWork *arg0, s32 arg1, u8 arg2);

void ov103_021EE9C8(MailBoxWork *arg0, s32 arg1, u8 arg2) {
    MailMessage *temp_r0;
    String *temp_r0_2;

    temp_r0 = Mail_GetUnk20Array(arg0->view->mails[arg0->mailIndex], (s32)arg2);
    if (MailMsg_IsInit(temp_r0) != 0) {
        temp_r0_2 = MailMsg_GetExpandedString(temp_r0, HEAP_ID_157);
        ov103_021EE3E4(&arg0->view->windows[arg1], temp_r0_2, 0, 0, 1, 0x10200, 0);
        String_Delete(temp_r0_2);
    }
}
