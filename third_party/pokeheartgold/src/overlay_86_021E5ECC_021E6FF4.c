#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "sprite_system.h"

void ov86_021E6FF4(FrontierRecordsApp *arg0);

void ov86_021E6FF4(FrontierRecordsApp *arg0) {
    void *v0;
    ManagedSprite *managedSprite;
    u32 v1;
    void *v2;

    v1 = 0;
    v0 = arg0;
    v2 = arg0;
    do {
        managedSprite = *(ManagedSprite **)((u8 *)v2 + 0x234);
        if (managedSprite != NULL) {
            Sprite_DeleteAndFreeResources(managedSprite);
            *(ManagedSprite **)((u8 *)v2 + 0x234) = NULL;
        }
        v1 += 1;
        v2 += 4;
    } while (v1 < 2U);
    SpriteSystem_FreeResourcesAndManager(*(SpriteSystem **)((u8 *)v0 + 0x22C), *(SpriteManager **)((u8 *)v0 + 0x230));
    SpriteSystem_Free(*(SpriteSystem **)((u8 *)v0 + 0x22C));
}

void ov86_021E703C(FrontierRecordsApp *arg0);

void ov86_021E703C(FrontierRecordsApp *arg0) {
    ManagedSprite *managedSprite;
    u32 v0;
    void *v1;

    v1 = arg0;
    v0 = 0;
    do {
        managedSprite = *(ManagedSprite **)((u8 *)v1 + 0x234);
        if (managedSprite != NULL) {
            ManagedSprite_TickFrame(managedSprite);
        }
        v0 += 1;
        v1 += 4;
    } while (v0 < 2U);
}
