#include "global.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

s32 ov103_021ED0C0(void *);

s32 ov103_021EE644(void *);

s32 ov103_021ED760(MailBoxWork *arg0);

s32 ov103_021ED124(void *);
s32 ov103_021EDA70(void *, s8, s32);
s32 ov103_021EE824(void *);
s32 ov103_021EE860(void *);
void ov103_021ED7A4(MailBoxWork *arg0);
void ov103_021ECF68(MailBoxWork *arg0);
s32 ov103_021ED7D8(MailBoxWork *arg0);

s32 ov103_021ED760(MailBoxWork *arg0) {
    if ((arg0->page) == 0) {
        ov103_021EE0CC(arg0->view, 0, 0);
        ov103_021EE0CC(arg0->view, 1, 1);
    } else {
        ov103_021EE0CC(arg0->view, 0, 1);
        ov103_021EE0CC(arg0->view, 1, 0);
    }
    ov103_021EE644(arg0);
    ov103_021ED0C0(arg0);
    return 9;
}

void ov103_021ED7A4(MailBoxWork *arg0) {
    ov103_021EE0CC(arg0->view, 2, 0);
    ov103_021EDB60(arg0->view, 1);
    ov103_021ED124(arg0);
    ov103_021EE860(arg0);
    ov103_021EE824(arg0);
    ov103_021EDA70(arg0, 0, 0xD);
}

s32 ov103_021ED7D8(MailBoxWork *arg0) {
    ov103_021ECF68(arg0);
    return 0xE;
}
