#include "global.h"

#include "bag.h"
#include "mail_misc.h"
#include "party.h"
#include "unk_0201956C.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

s32 ov103_021EDD98(MailBoxWork *arg0);

s32 ov103_021EDD98(MailBoxWork *arg0) {
    u32 v0;

    v0 = sub_02090F6C(arg0->childParam);
    arg0->mailGiven = v0;
    if (v0 == 1) {
        sub_02090F70(arg0->childParam, Party_GetMonByIndex(SaveArray_Party_Get(arg0->args->saveData), (s32)arg0->partyPos));
        Bag_TakeItem(Save_Bag_Get(arg0->args->saveData), arg0->item, 1U, HEAP_ID_156);
    }
    sub_02090F90(arg0->childParam);
    return 0;
}

s32 ov103_021ED00C(void *);

s32 ov103_021ED0A0(void *);

s32 ov103_021ED0C0(void *);

s32 ov103_021EDA70(void *, s8, s32);

s32 ov103_021EDC00(void *arg0);

s32 ov103_021EDC58(void *);

s32 ov103_021EE644(void *);

s32 ov103_021EE8A8(void *, s32);

void ov103_021EDDE4(MailBoxWork *arg0);

void ov103_021EDDE4(MailBoxWork *arg0) {
    u16 v1;
    MailBoxView *v0;
    ov103_021EDC00(arg0);
    ov103_021EDC58(arg0);
    ov103_021ED0A0(arg0);
    ov103_021ED00C(arg0);
    v0 = arg0->view;
    if ((u32) v0->mailCount <= 0xAU) {
        ov103_021EE0CC(v0, 0, 0);
        ov103_021EE0CC(arg0->view, 1, 0);
    }
    v1 = arg0->view->maxPage;
    if ((u32) arg0->page > (u32)v1) {
        arg0->page = v1;
        arg0->cursor = 0;
    }
    ov103_021EE644(arg0);
    ov103_021ED0C0(arg0);
    sub_0201980C(arg0->view->frames, 0xA);
    ov103_021EE8A8(arg0, 1);
    ov103_021EDA70(arg0, 1, 0x12);
}
