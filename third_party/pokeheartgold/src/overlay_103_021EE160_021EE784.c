#include "global.h"

#include "mail.h"
#include "message_format.h"
#include "pm_string.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE784(MailBoxWork *arg0);

void ov103_021EE784(MailBoxWork *arg0) {
    Mail *mail;
    String *string;

    mail = arg0->view->mails[arg0->mailIndex];
    string = String_New(8U, HEAP_ID_157);
    CopyU16ArrayToString(string, Mail_GetAuthorNamePtr(mail));
    BufferString(arg0->view->msgFmt, 0U, string, (s32)Mail_GetAuthorGender(mail), 1, 2);
    String_Delete(string);
}
