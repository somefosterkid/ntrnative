#include "global.h"

#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

ManagedSprite *ov112_021F1AF4(SpriteSystem *, SpriteManager *, s32, s32, u8, u8);

ManagedSprite *ov112_021F1B44(SpriteSystem *, SpriteManager *, s32, s32, u8, u8);

s32 ov112_021F1CC8(void *, s32);

void ov112_021F1F80(Ov112SpriteApp *);

void ov112_021F1B94(Ov112SpriteApp *arg0);

void ov112_021F1B94(Ov112SpriteApp *arg0) {
    s32 flag;

    arg0->sprites[0] = ov112_021F1AF4(arg0->spriteSystem, arg0->spriteManager, 0x20, 0xB0, 8, 0);
    arg0->sprites[1] = ov112_021F1AF4(arg0->spriteSystem, arg0->spriteManager, 0x50, 0xB0, 9, 0);
    arg0->sprites[2] = ov112_021F1AF4(arg0->spriteSystem, arg0->spriteManager, 0xD8, 0xB0, 0xA, 0);
    arg0->sprites[3] = ov112_021F1AF4(arg0->spriteSystem, arg0->spriteManager, 0x80, 0x44, 0, 5);
    arg0->sprites[4] = ov112_021F1AF4(arg0->spriteSystem, arg0->spriteManager, 0x68, 0x38, 0xC, 2);
    arg0->sprites[5] = ov112_021F1AF4(arg0->spriteSystem, arg0->spriteManager, 0x98, 0x38, 0xC, 1);
    arg0->sprites[9] = ov112_021F1AF4(arg0->spriteSystem, arg0->spriteManager, 0x68, 0x28, 0x1A, 2);
    arg0->sprites[6] = ov112_021F1B44(arg0->spriteSystem, arg0->spriteManager, 0x68, 0x28, 8, 2);
    arg0->sprites[7] = ov112_021F1B44(arg0->spriteSystem, arg0->spriteManager, 0x98, 0x28, 8, 2);
    arg0->sprites[8] = ov112_021F1B44(arg0->spriteSystem, arg0->spriteManager, 0x80, 0x28, 8, 2);
    ov112_021F1F80(arg0);
    ov112_021F1CC8(arg0, 0);
    ManagedSprite_SetDrawFlag(arg0->sprites[0], 0);
    if ((s32) arg0->curIdx < (s32)(arg0->count - 1)) {
        flag = 1;
    } else {
        flag = 0;
    }
    ManagedSprite_SetDrawFlag(arg0->sprites[1], flag);
}
