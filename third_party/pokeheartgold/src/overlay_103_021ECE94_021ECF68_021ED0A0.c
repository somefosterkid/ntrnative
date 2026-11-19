#include "global.h"

#include "heap.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021ED0A0(MailBoxWork *arg0);

void ov103_021ED0A0(MailBoxWork *arg0) {
    u32 v0;
    v0 = 0;
    do {
        Heap_Free(arg0->view->mails[v0]);
        v0 += 1;
    } while (v0 < 0x14U);
}
