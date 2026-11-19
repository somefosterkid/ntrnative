#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

int ov81_02242F90(BattleFactoryAppPokeballSprite *ballSprite);
void ov81_02242F94(BattleFactoryAppPokeballSprite *ballSprite, u32 animID);

int ov81_02242F90(BattleFactoryAppPokeballSprite *ballSprite) {
    return ballSprite->restingY;
}

void ov81_02242F94(BattleFactoryAppPokeballSprite *ballSprite, u32 animID) {
    Sprite_SetAnimSpeed(ballSprite->sprite, FX32_ONE);
    Sprite_SetAnimCtrlSeq(ballSprite->sprite, animID);
}
