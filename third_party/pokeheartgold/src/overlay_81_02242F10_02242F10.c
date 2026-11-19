#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

void ov81_02242F10(BattleFactoryAppPokeballSprite *ballSprite, int x, int y);

void ov81_02242F10(BattleFactoryAppPokeballSprite *ballSprite, int x, int y) {
    VecFx32 position;

    position.x = x << FX32_SHIFT;
    position.y = y * FX32_ONE + FX32_CONST(512);
    Sprite_SetMatrix(ballSprite->sprite, &position);
}
