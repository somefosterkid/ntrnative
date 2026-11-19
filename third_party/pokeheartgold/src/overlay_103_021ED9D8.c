#include "global.h"
struct UnkStruct_ov70_022630A4_t;

#include "screen_fade.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE888(MailBoxWork *param_1);

s32 ov103_021EEAC8(void *, s32);

s32 ov103_021ED9D8(MailBoxWork *arg0);

void ov103_021ECEA4(void *);
u32 ov103_021EDA84(MailBoxWork *param_1, u32 param_2);

s32 ov103_021ED9D8(MailBoxWork *arg0) {
    ov103_021EE888(arg0);
    sub_0201980C(arg0->view->frames, 0xA);
    ov103_021EEAC8(arg0, GridInputHandler_GetNextInput(arg0->view->gridInput));
    ov103_021EE0CC(arg0->view, 2, 1);
    ov103_021EDB60(arg0->view, 0);
    return 9;
}

s32 ov103_021EDA14(MailBoxWork *arg0, s32 arg1);

s32 ov103_021EDA14(MailBoxWork *arg0, s32 arg1) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_157);
    arg0->view->wipeNextState = arg1;
    return 2;
}

s32 ov103_021EDA40(MailBoxWork *arg0, s32 arg1);

s32 ov103_021EDA40(MailBoxWork *arg0, s32 arg1) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_157);
    arg0->view->wipeNextState = arg1;
    return 2;
}

s32 ov103_021EDA70(MailBoxWork *arg0, s8 arg1, s32 arg2);

s32 ov103_021EDA70(MailBoxWork *arg0, s8 arg1, s32 arg2) {
    arg0->view->waitForInput = arg1;
    arg0->nextState = arg2;
    return 3;
}

u32 ov103_021EDA84(MailBoxWork *param_1, u32 param_2) {
    param_1->view->yesNoIndex = param_2;
    ov103_021ECEA4(param_1);
    return 4;
}
