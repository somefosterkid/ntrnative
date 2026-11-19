#include "global.h"

#include "bg_window.h"
#include "list_menu_items.h"
#include "touchscreen_list_menu.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

s32 ov103_021ED2B8(void *arg0);

extern const TouchscreenListMenuTemplate _021EEB34;

void ov103_021ECF68(MailBoxWork *arg0);

void ov103_021ECF68(MailBoxWork *arg0) {
    TouchscreenListMenuHeader spC;

    spC.template = _021EEB34;
    spC.listMenuItems = arg0->view->menuItems;
    spC.bgConfig = arg0->view->bgConfig;
    spC.numWindows = ov103_021ED2B8(arg0);
    if (spC.numWindows == 4) {
        arg0->view->menu = TouchscreenListMenu_Create(arg0->view->menuSpawner, &spC, 0, 0x12, 5, 0xB, 0);
    } else {
        arg0->view->menu = TouchscreenListMenu_Create(arg0->view->menuSpawner, &spC, 0, 0x12, 0xB, 0xB, 0);
    }
}
