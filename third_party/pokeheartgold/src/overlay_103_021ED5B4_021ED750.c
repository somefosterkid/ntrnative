#include "global.h"
#include "overlay_103_mail_box_work.h"

s32 ov103_021EDA40(void *, s32);

void ov103_021ED750(MailBoxWork *arg0);

void ov103_021ED750(MailBoxWork *arg0) {
    arg0->nextState = 0x19;
    ov103_021EDA40(arg0, 1);
}
