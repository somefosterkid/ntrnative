#include "global.h"

#include "bag.h"
#include "bg_window.h"
#include "item.h"
#include "mail.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EDB60(MailBoxView *view, s32 mode) {
    if (mode == 1) {
        ov103_021EE110(view, 0, 1);
        ov103_021EE110(view, 1, 1);
        G2_SetBlendAlpha(4, 8, 4, 0xC);
        return;
    }
    ov103_021EE110(view, 0, 0);
    ov103_021EE110(view, 1, 0);
    reg_G2_BLDCNT = 0;
}

s32 ov103_021ED23C(void *arg0);

s32 ov103_021EE210(void *);

s32 ov103_021EE930(void *);

void ov103_021EDBB0(MailBoxWork *arg0);

void ov103_021EDBB0(MailBoxWork *arg0) {
    ov103_021ED23C(arg0);
    ov103_021EE930(arg0);
    ov103_021EE210(arg0);
}

void ov103_021EDBC8(MailBoxWork *arg0);

void ov103_021EDBC8(MailBoxWork *arg0) {
    ov103_021EE0CC(arg0->view, 4, 0);
    ov103_021EE0CC(arg0->view, 5, 0);
    ov103_021EE0CC(arg0->view, 6, 0);
    BgClearTilemapBufferAndCommit(arg0->view->bgConfig, 4U);
    BgClearTilemapBufferAndCommit(arg0->view->bgConfig, 5U);
}

void ov103_021EDC00(MailBoxWork *arg0);

void ov103_021EDC00(MailBoxWork *arg0) {
    Bag *bag;
    bag = Save_Bag_Get(arg0->args->saveData);
    arg0->item = MailToItemId(Mail_GetType(arg0->view->mails[arg0->mailIndex]));
    if (Bag_AddItem(bag, arg0->item, 1U, HEAP_ID_157) == 1) {
        arg0->bagFull = 0;
        return;
    }
    arg0->bagFull = 1;
}
