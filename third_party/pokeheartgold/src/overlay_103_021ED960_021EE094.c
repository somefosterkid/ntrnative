#include "global.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE094(MailBoxView *view);

void ov103_021EE094(MailBoxView *view) {
    u32 v0;

    v0 = 0;
    do {
        ov103_021EE078(view, v0);
        v0 += 1;
    } while (v0 < 7U);
}
