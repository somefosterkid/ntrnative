#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

const VecFx32 *ov81_02242F30(BattleFactoryAppPokeballSprite *ballSprite);
void ov81_02242F3C(BattleFactoryAppPokeballSprite *ballSprite, int isSelected);
void ov81_02242F54(BattleFactoryAppPokeballSprite *ballSprite);
void ov81_02242F60(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_02242F54(BattleFactoryAppPokeballSprite *ballSprite) {
    ov81_02242F3C(ballSprite, FALSE);
}

void ov81_02242F60(BattleFactoryAppPokeballSprite *ballSprite) {
    const VecFx32 *oldPos = ov81_02242F30(ballSprite);

    VecFx32 newPos;
    newPos.x = ((oldPos->x / FX32_ONE) + 248) * FX32_ONE;
    newPos.y = oldPos->y;

    Sprite_SetMatrix(ballSprite->sprite, &newPos);
}
