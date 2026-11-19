#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

Sprite *ov81_02242A8C(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u32 priority, int resourcePriority, u8 onSubScreen);
void ov81_02242F10(BattleFactoryAppPokeballSprite *ballSprite, int x, int y);
BattleFactoryAppPokeballSprite *ov81_02242E50(BattleFactoryAppSpriteManager *spriteMan, int x, int y, enum HeapID heapID);

BattleFactoryAppPokeballSprite *ov81_02242E50(BattleFactoryAppSpriteManager *spriteMan, int x, int y, enum HeapID heapID) {
    BattleFactoryAppPokeballSprite *ballSprite = Heap_Alloc(heapID, sizeof(BattleFactoryAppPokeballSprite));
    memset(ballSprite, 0, sizeof(BattleFactoryAppPokeballSprite));
    ballSprite->isSelected = FALSE;
    ballSprite->restingX = x;
    ballSprite->restingY = y;
    ballSprite->sprite = ov81_02242A8C(spriteMan, 0, 0, 10, 0, TRUE);
    ov81_02242F10(ballSprite, x, y);
    Sprite_SetPaletteOverride(ballSprite->sprite, 1);
    return ballSprite;
}
