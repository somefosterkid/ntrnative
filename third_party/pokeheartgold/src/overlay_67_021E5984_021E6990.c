#include "global.h"

#include "overlay_67_app.h"
#include "sprite_system.h"

void ov67_021E6990(Ov67App *app) {
    ManagedSprite *managedSprite;
    u32 v1;

    v1 = 0;
    do {
        managedSprite = app->sprites[v1];
        if (managedSprite != NULL) {
            Sprite_DeleteAndFreeResources(managedSprite);
            app->sprites[v1] = NULL;
        }
        v1 += 1;
    } while (v1 < 4U);
    SpriteSystem_FreeResourcesAndManager(app->spriteSys, app->spriteMan);
    SpriteSystem_Free(app->spriteSys);
}

void ov67_021E69D8(Ov67App *app) {
    u32 v0;

    v0 = 0;
    do {
        ManagedSprite_TickFrame(app->sprites[v0]);
        v0 += 1;
    } while (v0 < 4U);
    SpriteSystem_DrawSprites(app->spriteMan);
}
