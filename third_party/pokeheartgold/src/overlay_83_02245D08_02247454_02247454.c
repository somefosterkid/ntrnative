#include "global.h"

#include "constants/heap.h"

#include "battle_castle_app_sprite.h"
#include "battle_castle_app_sprite_manager.h"
#include "heap.h"
#include "sprite.h"

Sprite *ov83_0224714C(BattleCastleAppSpriteManager *spriteMan, u32 charResourceID, u32 plttResourceID, u32 cellResourceID, u32 animID, u32 priority, int resourcePriority, u8 onSubScreen);

BattleCastleAppSprite *ov83_02247454(BattleCastleAppSpriteManager *spriteMan, u32 param_2, u32 param_3, u32 param_4, u32 param_5, s16 param_6, s16 param_7, u32 param_8, const u8 *unused);

BattleCastleAppSprite *ov83_02247454(BattleCastleAppSpriteManager *spriteMan, u32 param_2, u32 param_3, u32 param_4, u32 param_5, s16 param_6, s16 param_7, u32 param_8, const u8 *unused) {
    BattleCastleAppSprite *sprite;
    Sprite *uVar2;
    VecFx32 VStack_24;

    sprite = Heap_Alloc(HEAP_ID_107, sizeof(BattleCastleAppSprite));
    memset(sprite, 0, sizeof(BattleCastleAppSprite));
    sprite->x = param_6;
    sprite->y = param_7;
    uVar2 = ov83_0224714C(spriteMan, param_2, param_3, param_4, param_5, 0, param_8, 0);
    sprite->sprite = uVar2;
    VStack_24.x = (s32)param_6 << 0xc;
    VStack_24.y = (s32)param_7 << 0xc;
    Sprite_SetMatrix(sprite->sprite, &VStack_24);
    return sprite;
}

BattleCastleAppSprite *ov83_022474C4(BattleCastleAppSpriteManager *spriteMan, u32 param_2, u32 param_3, u32 param_4, u32 param_5, s16 param_6, s16 param_7, u32 param_8, const u8 *unused);

BattleCastleAppSprite *ov83_022474C4(BattleCastleAppSpriteManager *spriteMan, u32 param_2, u32 param_3, u32 param_4, u32 param_5, s16 param_6, s16 param_7, u32 param_8, const u8 *unused) {
    BattleCastleAppSprite *sprite;
    Sprite *uVar2;
    VecFx32 VStack_24;

    sprite = Heap_Alloc(HEAP_ID_107, sizeof(BattleCastleAppSprite));
    memset(sprite, 0, sizeof(BattleCastleAppSprite));
    sprite->x = param_6;
    sprite->y = param_7;
    uVar2 = ov83_0224714C(spriteMan, param_2, param_3, param_4, param_5, 0, param_8, 1);
    sprite->sprite = uVar2;
    VStack_24.x = (s32)param_6 << 0xc;
    VStack_24.y = param_7 * 0x1000 + 0xc0000;
    Sprite_SetMatrix(sprite->sprite, &VStack_24);
    return sprite;
}
