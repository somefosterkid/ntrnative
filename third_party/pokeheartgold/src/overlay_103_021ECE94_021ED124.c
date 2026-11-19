#include "global.h"

#include "unk_0201956C.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

s32 ov103_021EE468(void *, s32, s32, int);

void ov103_021ED124(MailBoxWork *arg0);

void ov103_021ED124(MailBoxWork *arg0) {
    ov103_021EE468(arg0, 0xA, 0x14, arg0->mailIndex);
    sub_020196E8(arg0->view->frames, 0xA, 4, 7);
}
