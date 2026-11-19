#include "global.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

s32 ov103_021EDA70(void *, s8, s32);

s32 ov103_021EE8A8(void *, s32);

void ov103_021EDE7C(MailBoxWork *arg0);

void ov103_021EDE7C(MailBoxWork *arg0) {
    if (arg0->bagFull == 0) {
        ov103_021EE8A8(arg0, 3);
    } else {
        ov103_021EE8A8(arg0, 4);
    }
    ov103_021EDA70(arg0, 1, 0x18);
}
