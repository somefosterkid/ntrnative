#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

extern u32 ov103_021EEEC4;

void ov103_021EE390(MailBoxWork *param_1);

void ov103_021EE390(MailBoxWork *param_1) {
    WindowTemplate *template;
    u32 uVar2;
    FontID_Alloc(FONT_UNK_4, HEAP_ID_157);
    uVar2 = 0;
    template = (WindowTemplate *)&ov103_021EEEC4;
    do {
        AddWindow(param_1->view->bgConfig, &param_1->view->windows[uVar2], template);
        template = template + 1;
        uVar2 = uVar2 + 1;
    } while (uVar2 < 0x1e);
    return;
}
