#include "global.h"

#include "sprite_system.h"

struct ManagedSprite;

struct ManagedSpriteTemplate;

struct SpriteManager;

struct SpriteSystem;

extern const ManagedSpriteTemplate ov112_021FF398;

ManagedSprite *ov112_021F1AF4(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int x, int y, u8 animation, u8 drawPriority);

ManagedSprite *ov112_021F1AF4(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int x, int y, u8 animation, u8 drawPriority) {
    ManagedSpriteTemplate tmpl = ov112_021FF398;
    ManagedSprite *sprite;

    tmpl.x = x;
    tmpl.y = y;
    tmpl.drawPriority = drawPriority;
    tmpl.animation = animation;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &tmpl, 0x20C000);
    ManagedSprite_SetAnimateFlag(sprite, 1);
    return sprite;
}

extern const ManagedSpriteTemplate ov112_021FF3CC;

ManagedSprite *ov112_021F1B44(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int x, int y, u8 animation, u8 drawPriority);

ManagedSprite *ov112_021F1B44(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int x, int y, u8 animation, u8 drawPriority) {
    ManagedSpriteTemplate tmpl = ov112_021FF3CC;
    ManagedSprite *sprite;

    tmpl.x = x;
    tmpl.y = y;
    tmpl.drawPriority = drawPriority;
    tmpl.animation = animation;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &tmpl, 0x20C000);
    ManagedSprite_SetAnimateFlag(sprite, 1);
    return sprite;
}
