#include "global.h"
struct UnkStruct_ov70_022630A4_t;

#include "bg_window.h"
#include "yes_no_prompt.h"
#include "list_menu_items.h"
#include "touchscreen_list_menu.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021ECEA4(MailBoxWork *param_1);

u32 ov103_021ED2B8(void *);
void ov103_021ECEEC(MailBoxWork *arg0);
void ov103_021ECF48(MailBoxWork *arg0);

void ov103_021ECEA4(MailBoxWork *param_1) {
    YesNoPromptTemplate YStack_20;
    YStack_20.bgConfig = param_1->view->bgConfig;
    YStack_20.bgId = 0;
    YStack_20.tileStart = 0x3a6;
    YStack_20.plttSlot = 10;
    YStack_20.x = 0x19;
    YStack_20.y = 0xa;
    YStack_20.ignoreTouchFlag = 0;
    YStack_20.initialCursorPos = 0;
    YStack_20.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(param_1->view->yesNoPrompt, &YStack_20);
    return;
}

void ov103_021ECEEC(MailBoxWork *arg0) {
    s32 value;
    u32 n;
    MailBoxView *v0;
    arg0->view->menuSpawner = TouchscreenListMenuSpawner_Create(HEAP_ID_157, NULL);
    n = ov103_021ED2B8(arg0);
    arg0->view->menuItems = ListMenuItems_New(n, HEAP_ID_157);
    value = 0;
    do {
        if ((n != 2) || ((value != 2) && (value != 1))) {
            v0 = arg0->view;
            ListMenuItems_AppendFromMsgData(v0->menuItems, v0->msgData, value + 5, value);
        }
        value = (s32)(u16)(value + 1);
    } while ((u32)value < 4U);
}

void ov103_021ECF48(MailBoxWork *arg0) {
    ListMenuItems_Delete(arg0->view->menuItems);
    TouchscreenListMenuSpawner_Destroy(arg0->view->menuSpawner);
}
