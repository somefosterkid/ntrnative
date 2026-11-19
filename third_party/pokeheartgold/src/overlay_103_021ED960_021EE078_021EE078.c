#include "global.h"

#include "overlay_103_mail_box_view.h"

void ov103_021EE078(MailBoxView *view, int slot) {
    if (view->sprites[slot] != NULL) {
        Sprite_DeleteAndFreeResources(view->sprites[slot]);
        view->sprites[slot] = NULL;
    }
}
