#include "global.h"

#include "bg_window.h"
#include "mail.h"
#include "pm_string.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

s32 ov103_021EE3E4(void *, String *, s32, s32, s32, s32, s32);

s32 ov103_021EE9C8(MailBoxWork *, s32, u32);

void ov103_021EE930(MailBoxWork *arg0);

void ov103_021EE930(MailBoxWork *arg0) {
    s32 temp_r6;
    u32 var_r4;
    MailBoxView *temp_r4;
    var_r4 = 0;
    do {
#if defined(SDK_PORT) || defined(NONMATCHING)
        temp_r6 = var_r4 + 0x1A;
        FillWindowPixelBuffer(&arg0->view->windows[temp_r6], 0U);
        ov103_021EE9C8(arg0, var_r4 + 0x1A, var_r4);
        ScheduleWindowCopyToVram(&arg0->view->windows[temp_r6]);
#else
        temp_r6 = (var_r4 + 0x1A) * 0x10;
        FillWindowPixelBuffer(((void *)arg0->view + 0x48) + temp_r6, 0U);
        ov103_021EE9C8(arg0, var_r4 + 0x1A, var_r4);
        ScheduleWindowCopyToVram(((void *)arg0->view + 0x48) + temp_r6);
#endif
        var_r4 += 1;
    } while (var_r4 < 3U);
    FillWindowPixelBuffer(&arg0->view->windows[29], 0U);
    CopyU16ArrayToString(arg0->view->string, Mail_GetAuthorNamePtr(arg0->view->mails[arg0->mailIndex]));
    temp_r4 = arg0->view;
    ov103_021EE3E4(&temp_r4->windows[29], temp_r4->string, 0, 1, 1, 0x10200, 0);
    ScheduleWindowCopyToVram(&arg0->view->windows[29]);
}
