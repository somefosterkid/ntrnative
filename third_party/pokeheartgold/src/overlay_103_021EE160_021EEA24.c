#include "global.h"
struct MailBoxWork;

#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

extern TouchscreenHitbox ov103_021EEFBC;

s32 ov103_021EEA24(MailBoxWork *arg0);

s32 ov103_021EEA24(MailBoxWork *arg0) {
    if (arg0->view->maxPage == 0) {
        return -1;
    }
    return TouchscreenHitbox_FindRectAtTouchNew(&ov103_021EEFBC);
}

s32 ov103_021EEAC8(void *, s32);

extern GridCallbacks ov103_021EEFC8;

extern TouchscreenHitbox ov103_021EEFD8;

extern DpadMenuBox ov103_021EF008;

void ov103_021EEA48(MailBoxWork *callbackParam);

void ov103_021EEA48(MailBoxWork *callbackParam) {
    u16 v0;
    u8 initialPos;
    v0 = callbackParam->view->maxPage;
    if ((u32) callbackParam->page > (u32)v0) {
        callbackParam->page = v0;
        initialPos = 0;
    } else {
        initialPos = callbackParam->cursor;
    }
    callbackParam->view->gridInput = GridInputHandler_Create(&ov103_021EEFD8, &ov103_021EF008, &ov103_021EEFC8, callbackParam, 1, (u8)(s32)initialPos, HEAP_ID_157);
    ov103_021EEAC8(callbackParam, initialPos);
}
