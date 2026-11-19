/* PORTOK hk39 */

#include "global.h"
#include "sprite_system.h"

typedef struct {
    u8 filler0[0x18];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[1];
} Ov96Work_021EC5C0;

void ov96_021EC5C0(Ov96Work_021EC5C0 *work) {
    ManagedSpriteTemplate tmpl = { 0 };
    int i, j, idx, x;

    idx = 5;
    x = 32;
    for (i = 0; i < 4; i++) {
        tmpl.resIdList[0] = 0x69;
        tmpl.resIdList[1] = 0x69;
        tmpl.resIdList[2] = 0x65;
        tmpl.resIdList[3] = 0x65;
        tmpl.bgPriority = 1;
        tmpl.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        tmpl.drawPriority = 2;
        tmpl.x = x;
        tmpl.y = 0x48;
        tmpl.animation = 0;
        tmpl.pal = i + 1;
        work->sprites[i * 4 + 5] = SpriteSystem_NewSpriteWithYOffset(work->spriteSystem, work->spriteManager, &tmpl, FX32_CONST(524));
        ManagedSprite_SetAnimateFlag(work->sprites[i * 4 + 5], 1);
        tmpl.pal = 0;
        tmpl.bgPriority = 1;
        tmpl.drawPriority = 1;
        tmpl.x -= 16;
        tmpl.animation = 1;
        for (j = 0; j < 3; j++) {
            work->sprites[i * 4 + 6 + j] = SpriteSystem_NewSpriteWithYOffset(work->spriteSystem, work->spriteManager, &tmpl, FX32_CONST(524));
            ManagedSprite_SetAnimateFlag(work->sprites[i * 4 + 6 + j], 1);
            tmpl.x += 16;
        }
        idx += 4;
        x += 64;
    }
}
