#include "global.h"

#include "overlay_106.h"
#include "sprite_system.h"
#include "overlay_106_arceus_cinematic.h"

void ov106_021E6634(ArceusCinematic *arceus, u32 index);

void ov106_021E664C(ArceusCinematic *arceus);

void ov106_021E664C(ArceusCinematic *arceus) {
    ov106_021E6634(arceus, 0);
    SpriteSystem_FreeResourcesAndManager(arceus->spriteSystem, arceus->spriteManager);
    SpriteSystem_Free(arceus->spriteSystem);
}

void ov106_021E6668(ArceusCinematic *arceus) {
    ManagedSprite *managedSprite;

    managedSprite = arceus->sprites[0];
    if (managedSprite != NULL) {
        ManagedSprite_TickFrame(managedSprite);
    }
}

void ov106_021E6678(void *arg0, u32 arg1, u32 arg2);

void ov106_021E6678(void *arg0, u32 offset, u32 arg2) {
    DC_FlushRange(arg0, arg2);
    GX_LoadOBJ(arg0, offset, arg2);
}

void ov106_021E6694(void *arg0, u32 arg1, u32 arg2);

void ov106_021E6694(void *arg0, u32 offset, u32 arg2) {
    DC_FlushRange(arg0, arg2);
    GX_LoadOBJPltt(arg0, offset, arg2);
}
