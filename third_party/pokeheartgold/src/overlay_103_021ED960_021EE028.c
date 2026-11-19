#include "global.h"

#include "sprite_system.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE028(MailBoxView *view);

void ov103_021EE028(MailBoxView *view) {
    SpriteSystem_FreeResourcesAndManager(view->spriteSystem, view->spriteManager);
    SpriteSystem_Free(view->spriteSystem);
}

ManagedSprite *ov103_021EE048(MailBoxView *view, const ManagedSpriteTemplate *unkTemplate);

ManagedSprite *ov103_021EE048(MailBoxView *view, const ManagedSpriteTemplate *unkTemplate) {
    if (unkTemplate->vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        return SpriteSystem_NewSprite(view->spriteSystem, view->spriteManager, unkTemplate);
    }
    return SpriteSystem_NewSpriteWithYOffset(view->spriteSystem, view->spriteManager, unkTemplate, 0x200000);
}
