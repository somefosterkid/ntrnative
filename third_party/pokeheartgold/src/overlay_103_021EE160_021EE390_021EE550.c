#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE550(MailBoxWork *param_1);

void ov103_021EE550(MailBoxWork *param_1) {
    FillWindowPixelBuffer(&param_1->view->windows[25], 0);
    ov103_021EE430(param_1->view, param_1->view->msgData, 0x19, 0, 0x2c, 0, FONT_SYSTEM, 0x10200, 2);
    ScheduleWindowCopyToVram(&param_1->view->windows[25]);
    FillWindowPixelBuffer(&param_1->view->windows[21], 0);
    ov103_021EE430(param_1->view, param_1->view->msgData, 0x15, 3, 0x18, 0, FONT_UNK_4, 0xf0100, 2);
    ScheduleWindowCopyToVram(&param_1->view->windows[21]);
    FillWindowPixelBuffer(&param_1->view->windows[22], 0);
    ov103_021EE430(param_1->view, param_1->view->msgData, 0x16, 4, 0x18, 0, FONT_UNK_4, 0xf0100, 2);
    return;
}

void ov103_021EE60C(MailBoxView *param_1) {
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[21]);
    ScheduleWindowCopyToVram(&param_1->windows[22]);
    return;
}

void ov103_021EE628(MailBoxView *param_1) {
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[22]);
    ScheduleWindowCopyToVram(&param_1->windows[21]);
    return;
}
