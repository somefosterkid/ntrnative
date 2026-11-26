#include "global.h"

#include "bg_window.h"
#include "list_menu_items.h"
#include "overlay_42.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"

void ov43_0222BAB8(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    int i;

    Sprite_Delete(roster->deleteEffect);
    RemoveWindow(&roster->msgWindow);
    String_Delete(roster->msgText);
    for (i = 0; i < 2; i++) {
        ListMenuItems_Delete(roster->menuItems[i]);
    }
    RemoveWindow(&roster->returnWindow);
    ov42_02227274(roster->trainerIcons);
    ov42_02227114(roster->trainerIcons);
    for (i = 0; i < 3; i++) {
        ov43_0222C7EC(&roster->pages[i], gfx);
    }
}
