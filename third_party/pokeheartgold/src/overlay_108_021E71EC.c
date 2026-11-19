#include "global.h"

#include "bg_window.h"
#include "yes_no_prompt.h"
#include "list_menu_items.h"
#include "touchscreen_list_menu.h"
#include "unk_02013534.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E71EC(SafariAreaCustomizer *param_1);

void ov108_021E7224(SafariAreaCustomizer *arg0);
void ov108_021E72A4(SafariAreaCustomizer *arg0);
void ov108_021E838C(SafariAreaCustomizer *arg0);
void ov108_021E83F0(SafariAreaCustomizer *arg0);
void *ov108_021E84A4(enum HeapID, s16, s16, s32);
void ov108_021E72CC(SafariAreaCustomizer *arg0);
void ov108_021E83C8(SafariAreaCustomizer *arg0);
void ov108_021E844C(SafariAreaCustomizer *arg0);
s32 ov108_021E84DC(void *);
void ov108_021E730C(SafariAreaCustomizer *arg0);

void ov108_021E71EC(SafariAreaCustomizer *param_1) {
    s32 iVar1;
    Window *window;

    YesNoPrompt_Destroy(param_1->yesNoPrompt);
    RemoveWindow(&param_1->textWindow);
    iVar1 = 0;
    window = &param_1->windows[0];
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 0xf);
    return;
}

void ov108_021E7224(SafariAreaCustomizer *arg0) {
    arg0->unk_4B4 = ListMenuItems_New(3U, arg0->heapID);
    arg0->unk_4B8 = ListMenuItems_New(2U, arg0->heapID);
    ListMenuItems_AppendFromMsgData(arg0->unk_4B4, arg0->msgData, 7, 0);
    ListMenuItems_AppendFromMsgData(arg0->unk_4B4, arg0->msgData, 8, 1);
    ListMenuItems_AppendFromMsgData(arg0->unk_4B4, arg0->msgData, 6, 2);
    ListMenuItems_AppendFromMsgData(arg0->unk_4B8, arg0->msgData, 5, 0);
    ListMenuItems_AppendFromMsgData(arg0->unk_4B8, arg0->msgData, 6, 1);
}

void ov108_021E72A4(SafariAreaCustomizer *arg0) {
    ListMenuItems_Delete(arg0->unk_4B8);
    ListMenuItems_Delete(arg0->unk_4B4);
    arg0->unk_4B4 = NULL;
    arg0->unk_4B8 = NULL;
}

void ov108_021E72CC(SafariAreaCustomizer *arg0) {
    ov108_021E838C(arg0);
    arg0->unk_348 = ov108_021E84A4(arg0->heapID, 0xC, 1, 2);
    ov108_021E83F0(arg0);
    arg0->listMenuSpawner = TouchscreenListMenuSpawner_Create(arg0->heapID, NULL);
    arg0->unk_39C = FontSystem_NewInit(1, arg0->heapID);
}

void ov108_021E730C(SafariAreaCustomizer *arg0) {
    sub_020135AC(arg0->unk_39C);
    TouchscreenListMenuSpawner_Destroy(arg0->listMenuSpawner);
    ov108_021E844C(arg0);
    ov108_021E84DC(arg0->unk_348);
    ov108_021E83C8(arg0);
}
