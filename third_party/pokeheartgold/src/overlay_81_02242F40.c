#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

void ov81_02242F3C(BattleFactoryAppPokeballSprite *ballSprite, int isSelected);
u8 ov81_02242F40(BattleFactoryAppPokeballSprite *ballSprite);
void ov81_02242F48(BattleFactoryAppPokeballSprite *ballSprite);

u8 ov81_02242F40(BattleFactoryAppPokeballSprite *ballSprite) {
    return ballSprite->isSelected;
}

void ov81_02242F48(BattleFactoryAppPokeballSprite *ballSprite) {
    ov81_02242F3C(ballSprite, TRUE);
}
