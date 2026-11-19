#include "global.h"

#include "sprite_system.h"

void ov96_021EC51C(void *arg0);

void ov96_021EC51C(void *arg0) {
    ManagedSprite *managedSprite;
    u8 v1;
    void *v0;

    v1 = 0;
    do {
        v0 = arg0 + (v1 * 4);
        managedSprite = *(ManagedSprite **)((u8 *)v0 + 0x20);
        if (managedSprite != NULL) {
            Sprite_DeleteAndFreeResources(managedSprite);
            *(ManagedSprite **)((u8 *)v0 + 0x20) = NULL;
        }
        v1 += 1;
    } while ((u32)v1 < 0x1AU);
    SpriteSystem_FreeResourcesAndManager(*(SpriteSystem **)((u8 *)arg0 + 0x18), *(SpriteManager **)((u8 *)arg0 + 0x1C));
    SpriteSystem_Free(*(SpriteSystem **)((u8 *)arg0 + 0x18));
}
