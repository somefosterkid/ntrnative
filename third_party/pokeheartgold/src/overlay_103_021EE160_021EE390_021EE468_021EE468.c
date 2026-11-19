#include "global.h"

#include "bg_window.h"
#include "mail.h"
#include "pm_string.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

struct Mail;

struct String;

struct Window;

extern void ov103_021EE3E4(Window *window, String *string, int a2, int a3, int a4, u32 color, int a6);

extern void sub_02019A60(void *a0, int a1, Window *window);

void ov103_021EE468(MailBoxWork *data, int a1, int windowIdx, int mailIdx);

void ov103_021EE468(MailBoxWork *data, int a1, int windowIdx, int mailIdx) {
    MailBoxView *view = data->view;
    Window *window = &view->windows[windowIdx];
    Mail *mail = view->mails[mailIdx];
    u32 i;
    u32 x;

    CopyU16ArrayToString(data->view->string, Mail_GetAuthorNamePtr(mail));
    for (i = 0, x = 0; i < 8; i++, x += 8) {
        BlitBitmapRectToWindow(window, data->view->tileA, 0, 0, 8, 8, (u16)x, 0, 8, 8);
        BlitBitmapRectToWindow(window, data->view->tileB, 0, 0, 8, 8, (u16)x, 8, 8, 8);
    }
    if (Mail_GetAuthorGender(mail) == 0) {
        ov103_021EE3E4(window, data->view->string, 0, 0, 4, 0x30200, 0);
    } else {
        ov103_021EE3E4(window, data->view->string, 0, 0, 4, 0x50400, 0);
    }
    CopyWindowPixelsToVram_TextMode(window);
    sub_02019A60(data->view->frames, a1, window);
}
