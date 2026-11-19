#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

void *ov81_02242EA4(BattleFactoryAppPokeballSprite *ballSprite);
void ov81_02242EB8(BattleFactoryAppPokeballSprite *ballSprite, BOOL draw);
VecFx32 ov81_02242EC4(BattleFactoryAppPokeballSprite *ballSprite, int x, int y);

void *ov81_02242EA4(BattleFactoryAppPokeballSprite *ballSprite) {
    Sprite_Delete(ballSprite->sprite);
    Heap_Free(ballSprite);

    return NULL;
}

void ov81_02242EB8(BattleFactoryAppPokeballSprite *ballSprite, BOOL draw) {
    Sprite_SetDrawFlag(ballSprite->sprite, draw);
}

VecFx32 ov81_02242EC4(BattleFactoryAppPokeballSprite *ballSprite, int x, int y) {
    VecFx32 position = *Sprite_GetMatrixPtr(ballSprite->sprite);
    position.x += x * FX32_ONE;
    position.y += y * FX32_ONE;

    Sprite_SetMatrix(ballSprite->sprite, &position);
    return *Sprite_GetMatrixPtr(ballSprite->sprite);
}
