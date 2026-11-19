#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

ManagedSprite *ov112_021F1AF4(SpriteSystem *, SpriteManager *, int, int, u8, u8);

extern ManagedSpriteTemplate ov112_021FF400;

void ov112_021F2000(Ov112SpriteRec *rec, SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 anim, int resBase);

void ov112_021F2000(Ov112SpriteRec *rec, SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 anim, int resBase) {
    ManagedSpriteTemplate tmpl;
    int i;

    tmpl = ov112_021FF400;
    for (i = 0; i < 2; i++) {
        tmpl.x = x;
        tmpl.y = y;
        tmpl.drawPriority = 3;
        tmpl.animation = anim;
        tmpl.resIdList[1] = resBase + 2;
        tmpl.resIdList[2] = i + 2;
        tmpl.resIdList[3] = i + 2;
        tmpl.resIdList[0] = resBase + 2 + i * 2;
        rec->sprites[i] = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &tmpl, FX32_CONST(524));
        ManagedSprite_SetAnimateFlag(rec->sprites[i], 1);
        ManagedSprite_SetDrawFlag(rec->sprites[i], 0);
    }
    rec->sprites[2] = ov112_021F1AF4(spriteSystem, spriteManager, x, y, 0x17, 4);
}
