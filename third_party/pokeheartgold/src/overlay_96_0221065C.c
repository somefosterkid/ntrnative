#include "global.h"

#include "sprite_system.h"
#include "overlay_96_app_0221040c.h"

ManagedSprite *ov96_022104D8(SpriteSystem *, SpriteManager *, s16, s32, s32, s32);

void ov96_0221065C(Ov96App_0221040C *arg0) {
    ManagedSprite *managedSprite;
    u32 v0;
    s32 v2;

    v2 = 0;
    v0 = 0xE6;
    do {
        managedSprite = ov96_022104D8(arg0->spriteSystem, arg0->spriteManager, v0, 0x4C, (s32)(u16)(v2 + 9), 0);
        arg0->sprites[v2 + 1] = managedSprite;
        ManagedSprite_SetPaletteOverride(managedSprite, 1);
        v0 += 0x10;
        v2 += 1;
    } while (v2 < 2);
}
