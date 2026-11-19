#include "global.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"
#include "overlay_56_mail_viewer_app.h"

void ov56_021E6D90(struct MailViewerApp *arg0);

void ov56_021E6D90(MailViewerApp *arg0) {
    ManagedSprite *managedSprite;
    s32 v0;

    if (arg0->mode == 0) {
        v0 = 0;
        do {
            managedSprite = arg0->managedSprites[v0];
            if (managedSprite != NULL) {
                Sprite_DeleteAndFreeResources(managedSprite);
            }
            v0 += 1;
        } while (v0 < 3);
        SpriteSystem_FreeResourcesAndManager(arg0->spriteSys, arg0->spriteMan);
        SpriteSystem_Free(arg0->spriteSys);
        GF_DestroyVramTransferManager();
    }
}
