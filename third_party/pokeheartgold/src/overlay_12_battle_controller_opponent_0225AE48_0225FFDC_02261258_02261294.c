#include "global.h"

#include "battle/battle.h"
#include "battle/battle_controller_opponent.h"

#include "sprite.h"
#include "sprite_system.h"

void ov12_02261294(OpponentData *opponentData, s32 flag) {
    ManagedSprite *v0;

    v0 = opponentData->managedSprite;
    if (v0 != NULL) {
        ManagedSprite_SetDrawFlag(v0, flag);
    }
}
