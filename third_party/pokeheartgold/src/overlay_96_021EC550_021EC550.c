#include "global.h"

#include "sprite_system.h"
#include "overlay_96_app_021ed1d0.h"

void ov96_021EC550(Ov96App_021ED1D0 *arg0);

void ov96_021EC550(Ov96App_021ED1D0 *arg0) {
    ManagedSpriteTemplate sp0 = { 0 };
    ManagedSprite *sprite;
    s32 i;
    s32 id;

    for (i = 0; i < 5; i++) {
        id = i + 0x64;
        sp0.resIdList[0] = id;
        sp0.resIdList[1] = id;
        sp0.resIdList[2] = 0x64;
        sp0.resIdList[3] = 0x64;
        sp0.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        sp0.bgPriority = 1;
        sp0.drawPriority = 1;
        sp0.x = 0x20 + i * 0x40;
        sp0.y = 0x88;
        if (i == 4) {
            sp0.x = 0x80;
            sp0.y = 0x70;
        }
        sprite = SpriteSystem_NewSpriteWithYOffset(arg0->spriteSystem, arg0->spriteManager, &sp0, FX32_CONST(524));
        arg0->sprites[i] = sprite;
        ManagedSprite_SetAnimateFlag(sprite, 1);
    }
}
