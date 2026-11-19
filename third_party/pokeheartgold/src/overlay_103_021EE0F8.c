#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "filesystem.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE0F8(MailBoxView *view, s32 idx, s16 x, s16 y) {
    ManagedSprite_SetPositionXY(view->sprites[idx], x, y);
}

void ov103_021EE110(MailBoxView *view, s32 idx, s32 xlu) {
    if (xlu == 1) {
        ManagedSprite_SetOamMode(view->sprites[idx], GX_OAM_MODE_XLU);
        return;
    }
    ManagedSprite_SetOamMode(view->sprites[idx], GX_OAM_MODE_NORMAL);
}

void ov103_021EE13C(MailBoxView *view) {
    view->iconNarc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_157);
}
