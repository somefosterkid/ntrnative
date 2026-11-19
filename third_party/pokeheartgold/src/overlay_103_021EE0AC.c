#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE0AC(MailBoxView *view, s32 idx, s32 anim) {
    ManagedSprite_SetAnimationFrame(view->sprites[idx], 0);
    ManagedSprite_SetAnim(view->sprites[idx], anim);
    return;
}

void ov103_021EE0CC(MailBoxView *view, s32 idx, s32 show) {
    if (show == 1) {
        ManagedSprite_SetDrawFlag(view->sprites[idx], 1);
        return;
    }
    ManagedSprite_SetDrawFlag(view->sprites[idx], 0);
}
