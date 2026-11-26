#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "sprite_system.h"

void ov86_021E6FF4(FrontierRecordsApp *arg0) {
    u32 i;

    i = 0;
    do {
        if (arg0->sprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(arg0->sprites[i]);
            arg0->sprites[i] = NULL;
        }
        i++;
    } while (i < 2U);
    SpriteSystem_FreeResourcesAndManager(arg0->spriteSystem, arg0->spriteManager);
    SpriteSystem_Free(arg0->spriteSystem);
}

void ov86_021E703C(FrontierRecordsApp *arg0) {
    u32 i;

    i = 0;
    do {
        if (arg0->sprites[i] != NULL) {
            ManagedSprite_TickFrame(arg0->sprites[i]);
        }
        i++;
    } while (i < 2U);
}
