#include "global.h"

#include "unk_0201956C.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_text.h"
#include "yes_no_prompt.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021ECE0C(MailBoxWork *arg0);

void ov103_021ECE80(MailBoxWork *arg0);

void ov103_021ECE94(MailBoxWork *arg0);

void ov103_021ECE0C(MailBoxWork *arg0) {
    sub_020195C0(arg0->view->frames);
}

void ov103_021ECE18(MailBoxView *arg0) {
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xE8, HEAP_ID_157);
    arg0->msgFmt = MessageFormat_New(HEAP_ID_157);
    arg0->string = String_New(0x400U, HEAP_ID_157);
    TextFlags_SetCanTouchSpeedUpPrint(1);
}

void ov103_021ECE54(MailBoxView *arg0) {
    TextFlags_SetCanTouchSpeedUpPrint(0);
    String_Delete(arg0->string);
    MessageFormat_Delete(arg0->msgFmt);
    DestroyMsgData(arg0->msgData);
}

void ov103_021ECE80(MailBoxWork *arg0) {
    arg0->view->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_157);
}

void ov103_021ECE94(MailBoxWork *arg0) {
    YesNoPrompt_Destroy(arg0->view->yesNoPrompt);
}
