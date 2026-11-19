#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE860(MailBoxWork *param_1);

void ov103_021EE860(MailBoxWork *param_1) {
    DrawFrameAndWindow2(&param_1->view->windows[24], 1, 0x3e2, 0xd);
    ScheduleWindowCopyToVram(&param_1->view->windows[24]);
    return;
}

void ov103_021EE888(MailBoxWork *param_1);

void ov103_021EE888(MailBoxWork *param_1) {
    ClearFrameAndWindow2(&param_1->view->windows[24], 1);
    ScheduleBgTilemapBufferTransfer(param_1->view->bgConfig, 0);
    return;
}

void ov103_021EE7DC(void *);

void ov103_021EE8A8(MailBoxWork *param_1, u32 param_2);

void ov103_021EE8A8(MailBoxWork *param_1, u32 param_2) {
    switch (param_2) {
    case 0:
        ReadMsgDataIntoString(param_1->view->msgData, 10, param_1->view->string);
        break;
    case 1:
        ReadMsgDataIntoString(param_1->view->msgData, 0xb, param_1->view->string);
        break;
    case 2:
        ReadMsgDataIntoString(param_1->view->msgData, 0xc, param_1->view->string);
        break;
    case 3:
        ReadMsgDataIntoString(param_1->view->msgData, 0xd, param_1->view->string);
        break;
    case 4:
        ReadMsgDataIntoString(param_1->view->msgData, 0xe, param_1->view->string);
    }
    ov103_021EE7DC(param_1);
    return;
}
