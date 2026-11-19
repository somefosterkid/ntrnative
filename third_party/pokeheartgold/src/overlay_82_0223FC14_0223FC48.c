#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "sprite.h"

struct BattleHallAppSpriteManager;

typedef struct BattleHallAppSpriteManager BattleHallAppSpriteManager;

Sprite *ov82_0223FB08(BattleHallAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u32 priority, u8 onSubScreen);

typedef struct BattleHallAppSprite {
    u32 unused[3];
    const u8 *isAlwaysNull;
    Sprite *sprite;
} BattleHallAppSprite;

BattleHallAppSprite *ov82_0223FC48(BattleHallAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u16 x, u16 y, const u8 *null);

BattleHallAppSprite *ov82_0223FC48(BattleHallAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u16 x, u16 y, const u8 *null) {
    BattleHallAppSprite *sprite = Heap_Alloc(HEAP_ID_105, sizeof(BattleHallAppSprite));
    memset(sprite, 0, sizeof(BattleHallAppSprite));

    sprite->isAlwaysNull = null;
    sprite->sprite = ov82_0223FB08(spriteMan, resourceID, animID, 0, FALSE);

    VecFx32 position;
    position.x = x * FX32_ONE;
    position.y = y * FX32_ONE;

    Sprite_SetMatrix(sprite->sprite, &position);

    return sprite;
}
