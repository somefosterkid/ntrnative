#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "text.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE3C0(MailBoxWork *param_1);

void ov103_021EE3E4(Window *window, String *string, u32 arg2, u32 y, enum Font fontId, u32 color, s32 arg6);

void ov103_021EE3C0(MailBoxWork *param_1) {
    u32 uVar1;
    uVar1 = 0;
    do {
        RemoveWindow(&param_1->view->windows[uVar1]);
        uVar1 = uVar1 + 1;
    } while (uVar1 < 0x1e);
    FontID_Release(FONT_UNK_4);
    return;
}

void ov103_021EE3E4(Window *window, String *string, u32 arg2, u32 y, enum Font fontId, u32 color, s32 arg6) {
    u32 x;

    x = arg2;
    if (arg6 == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0U);
    } else if (arg6 == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0U) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFFU, color, NULL);
}
