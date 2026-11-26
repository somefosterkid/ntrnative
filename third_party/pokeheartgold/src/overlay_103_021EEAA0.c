#include "global.h"

#include "unk_02019BA4.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EEAA0(MailBoxWork *arg0);

void ov103_021EEAB0(void);
void ov103_021EEAB4(void);
void ov103_021EEAC8(MailBoxWork *arg0, s32 target);
void ov103_021EEAB8(MailBoxWork *arg0, s32 target);

void ov103_021EEAA0(MailBoxWork *arg0) {
    GridInputHandler_Free(arg0->view->gridInput);
}

void ov103_021EEAB0(void) {
}

void ov103_021EEAB4(void) {
}

void ov103_021EEAB8(MailBoxWork *arg0, s32 target) {
    ov103_021EEAC8(arg0, target);
}
