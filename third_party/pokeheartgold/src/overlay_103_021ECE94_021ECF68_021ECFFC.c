#include "global.h"

#include "touchscreen_list_menu.h"
#include "overlay_103_mail_box_view.h"
#include "overlay_103_mail_box_work.h"

void ov103_021ECFFC(MailBoxWork *arg0);

void ov103_021ECFFC(MailBoxWork *arg0) {
    TouchscreenListMenu_Destroy(arg0->view->menu);
}
