#include "global.h"

#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "system.h"
#include "touchscreen.h"
#include "overlay_103_mail_box_view.h"
#include "overlay_103_mail_box_work.h"

void ov103_021EEAC8(MailBoxWork *arg0, s32 target);

extern TouchscreenHitbox ov103_021EEFB4;
s32 ov103_021EEB04(void);

void ov103_021EEAC8(MailBoxWork *arg0, s32 target) {
    DpadMenuBox *dpadMenuBox;
    MailBoxView *v0;
    dpadMenuBox = (struct DpadMenuBox *)GridInputHandler_GetDpadBox(arg0->view->gridInput, target);
    ov103_021EE0F8(arg0->view, 2, dpadMenuBox->left, dpadMenuBox->top);
    v0 = arg0->view;
    if (target == 0xA) {
        ov103_021EE0AC(arg0->view, 2, 4);
        return;
    }
    ov103_021EE0AC(arg0->view, 2, 5);
}

s32 ov103_021EEB04(void) {
    if (TouchscreenHitbox_FindRectAtTouchNew(&ov103_021EEFB4) != -1) {
        return 1;
    }
    if (3 & gSystem.newKeys) {
        return 1;
    }
    return 0;
}
