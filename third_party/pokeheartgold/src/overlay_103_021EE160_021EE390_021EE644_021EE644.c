#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

u8 ov103_021EE3E4(Window *param_1, String *param_2, u32 param_3, u32 param_4, enum Font param_5, u32 param_6, s32 param_7);

void ov103_021EE644(MailBoxWork *o);

void ov103_021EE644(MailBoxWork *o) {
    String *str;
    u32 w1;
    u16 w2;
    u32 half;
    MailBoxView *q;
    FillWindowPixelBuffer(&o->view->windows[23], 0);
    ReadMsgDataIntoString(o->view->msgData, 2, o->view->string);
    ov103_021EE3E4(&o->view->windows[23], o->view->string, 0x14, 0, FONT_SYSTEM, 0x10200, 2);
    w1 = FontID_String_GetWidth(FONT_SYSTEM, o->view->string, 0) << 16;
    BufferIntegerAsString(o->view->msgFmt, 0, o->page + 1, 1, PRINTING_MODE_LEFT_ALIGN, 1);
    str = NewString_ReadMsgData(o->view->msgData, 1);
    StringExpandPlaceholders(o->view->msgFmt, o->view->string, str);
    String_Delete(str);
    w2 = FontID_String_GetWidth(FONT_SYSTEM, o->view->string, 0);
    half = w1 >> 17;
    ov103_021EE3E4(&o->view->windows[23], o->view->string, (0x14 - half) - w2, 0, FONT_SYSTEM, 0x10200, 0);
    q = o->view;
    BufferIntegerAsString(q->msgFmt, 0, q->maxPage + 1, 1, PRINTING_MODE_LEFT_ALIGN, 1);
    str = NewString_ReadMsgData(o->view->msgData, 1);
    StringExpandPlaceholders(o->view->msgFmt, o->view->string, str);
    String_Delete(str);
    ov103_021EE3E4(&o->view->windows[23], o->view->string, half + 0x14, 0, FONT_SYSTEM, 0x10200, 0);
    ScheduleWindowCopyToVram(&o->view->windows[23]);
}
