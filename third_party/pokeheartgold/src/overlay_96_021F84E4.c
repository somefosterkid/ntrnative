#include "global.h"

#include "overlay_96_unk_struct_ov96_021_f7934.h"
#include "sprite_system.h"

void ov96_021F84E4(UnkStruct_ov96_021F7934 *arg0);

void ov96_021F84E4(UnkStruct_ov96_021F7934 *arg0) {
    ManagedSprite *managedSprite;
    u8 v1;

    v1 = 0;
    do {
        managedSprite = arg0->sprites[v1];
        if (managedSprite != NULL) {
            Sprite_DeleteAndFreeResources(managedSprite);
            arg0->sprites[v1] = NULL;
        }
        v1 += 1;
    } while ((u32)v1 < 2U);
    SpriteSystem_FreeResourcesAndManager(arg0->spriteSystem, arg0->spriteManager);
    SpriteSystem_Free(arg0->spriteSystem);
}
