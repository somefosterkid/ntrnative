#include "global.h"

#include "bg_window.h"
#include "options.h"
#include "text.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE7DC(MailBoxWork *arg0);

void ov103_021EE7DC(MailBoxWork *arg0) {
    MailBoxView *v0;
    Window *sum;

    v0 = arg0->view;
    sum = &v0->windows[24];
    FillWindowPixelBuffer(sum, 0xFU);
    arg0->view->textPrinterId = AddTextPrinterParameterizedWithColor(sum, FONT_MESSAGE, arg0->view->string, 0U, 0U, (u32)Options_GetTextFrameDelay(arg0->options), 0x1020FU, NULL);
}
