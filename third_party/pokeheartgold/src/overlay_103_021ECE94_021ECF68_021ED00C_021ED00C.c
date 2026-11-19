#include "global.h"

#include "constants/heap.h"

#include "mail.h"
#include "pokemon_types_def.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021ED00C(MailBoxWork *arg0);

void ov103_021ED00C(MailBoxWork *arg0) {
    s32 var_r4;
    u16 temp_r1_2;
    arg0->view->mailCount = 0U;
    var_r4 = 0;
    do {
        arg0->view->pageMails[var_r4] = 0xFF;
        arg0->view->mails[var_r4] = Mailbox_AllocAndFetchMailI(&arg0->mailbox->msgs[0], 0, var_r4, HEAP_ID_157);
        if (Mail_TypeIsValid(arg0->view->mails[var_r4]) == 1) {
            arg0->view->pageMails[arg0->view->mailCount] = (u8)var_r4;
            arg0->view->mailCount = (u16)(arg0->view->mailCount + 1);
        }
        var_r4 += 1;
    } while (((u32)var_r4) < 0x14U);
    temp_r1_2 = arg0->view->mailCount;
    if (temp_r1_2 == 0) {
        arg0->view->maxPage = 0;
        return;
    }
    arg0->view->maxPage = (s16)(((s32)(temp_r1_2 - 1)) / 10);
}
