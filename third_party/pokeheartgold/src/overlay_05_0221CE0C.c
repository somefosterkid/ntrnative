#include "global.h"

#include "sprite_system.h"
#include "overlay_05.h"
#include "sprite.h"

void ov05_0221CE0C(void *arg0);

void ov05_0221CE0C(void *arg0) {
    int new_var;
    u32 v0;
    unsigned int new_var2;
    void *v1;
    v0 = 0;
    new_var2 = 0U;
    if (((u32)(*((u32 *)(((u8 *)arg0) + 0x210)))) > new_var2) {
        v1 = arg0;
        new_var = 0x210;
        do {
            Sprite_DeleteAndFreeResources(*((ManagedSprite **)(((u8 *)v1) + 0x198)));
            v0 += 1;
            v1 += 4;
        } while (v0 < ((u32)(*((u32 *)(((u8 *)arg0) + new_var)))));
    }
    SpriteSystem_FreeResourcesAndManager(*((SpriteSystem **)(((u8 *)arg0) + 0x190)), *((SpriteManager **)(((u8 *)arg0) + 0x194)));
    SpriteSystem_Free(*((SpriteSystem **)(((u8 *)arg0) + 0x190)));
}

void ov05_0221CE50(void *arg0) {
    u16 v0;

    v0 = 0;
    while ((u32)v0 < (u32) * (u32 *)((u8 *)arg0 + 0x210)) {
        Sprite_UpdateAnim(**(Sprite ***)((u8 *)(arg0 + (v0 * 4)) + 0x198), 0x1000);
        v0 += 1;
    }
}
