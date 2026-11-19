#include "global.h"

#include "battle/battle_message_structs.h"
#include "battle/battle_system.h"
#include "battle/battler_sprite_context.h"

void ov12_02261CA8(BattleSystem *battleSys, MoveAnimation *animation, BattlerSpriteContext *battlerSpriteCtx, int battler);

void ov12_02261CA8(BattleSystem *battleSys, MoveAnimation *animation, BattlerSpriteContext *battlerSpriteCtx, int battler) {
    int i;

    battlerSpriteCtx->targetBattler = battler;
    battlerSpriteCtx->sourceBattler = battler;

    for (i = 0; i < 4; i++) {
        battlerSpriteCtx->pokemonSpriteData[i] = ov12_0223BB88(battleSys, i);
        battlerSpriteCtx->species[i] = animation->species[i];
        battlerSpriteCtx->genders[i] = animation->genders[i];
        battlerSpriteCtx->shinyFlags[i] = animation->isShiny[i];
        battlerSpriteCtx->forms[i] = animation->formNums[i];
        battlerSpriteCtx->personalities[i] = animation->personalities[i];
    }

    ov12_0223C1C4(battleSys, battlerSpriteCtx->types);
    ov12_0223C1F4(battleSys, battlerSpriteCtx->sprites);
}
