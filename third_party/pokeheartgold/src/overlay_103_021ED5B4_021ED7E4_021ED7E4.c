#include "global.h"

#include "item.h"
#include "mail.h"
#include "touchscreen_list_menu.h"
#include "unk_0201956C.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021ECFFC(MailBoxWork *arg0);

void ov103_021EE888(void *);

s32 ov103_021EDBB0(void *);

s32 ov103_021EE8A8(void *, s32);

s32 ov103_021EDA70(void *, s8, s32);

s32 ov103_021EDC68(void *, s32, s32, s32);

s32 ov103_021EDA40(void *, s32);

s32 ov103_021ED7E4(MailBoxWork *work);

s32 ov103_021ED7E4(MailBoxWork *work) {
    u32 temp_r0;

    temp_r0 = (u32)TouchscreenListMenu_HandleInput(work->view->menu);
    switch (temp_r0) {
    case 0:
        ov103_021ECFFC(work);
        ov103_021EE888(work);
        ov103_021EE0CC(work->view, 2, 1);
        ov103_021EE60C(work->view);
        ov103_021EE374(work->view);
        ov103_021EDBB0(work);
        return 0xF;
    case 1:
        ov103_021ECFFC(work);
        ov103_021EE8A8(work, 0);
        return ov103_021EDA70(work, 0, 0x11);
    case 2:
        ov103_021ECFFC(work);
        ov103_021EDC68(work, 0xE, MailToItemId(Mail_GetType(work->view->mails[work->mailIndex])), 0);
        work->childProc = 0;
        work->nextState = 6;
        return ov103_021EDA40(work, 1);
    case 3:
    case 0xFFFFFFFE:
        ov103_021ECFFC(work);
        ov103_021EE888(work);
        sub_0201980C(work->view->frames, 0xA);
        ov103_021EDB60(work->view, 0);
        ov103_021EE0CC(work->view, 2, 1);
        return 9;
    case 0xFFFFFFFF:
        break;
    }
    return 0xE;
}
