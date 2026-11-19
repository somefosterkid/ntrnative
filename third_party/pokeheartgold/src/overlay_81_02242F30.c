#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

const VecFx32 *ov81_02242F30(BattleFactoryAppPokeballSprite *ballSprite);
void ov81_02242F3C(BattleFactoryAppPokeballSprite *ballSprite, int isSelected);

const VecFx32 *ov81_02242F30(BattleFactoryAppPokeballSprite *ballSprite) {
    return Sprite_GetMatrixPtr(ballSprite->sprite);
}

void ov81_02242F3C(BattleFactoryAppPokeballSprite *ballSprite, int isSelected) {
    ballSprite->isSelected = isSelected;
}
