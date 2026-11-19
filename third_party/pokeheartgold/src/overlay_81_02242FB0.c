#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

void ov81_02242FB0(BattleFactoryAppPokeballSprite *ballSprite, int palIndex);
void ov81_02242FBC(BattleFactoryAppPokeballSprite *ballSprite, GXOamMode mode);
void *ov81_02242FC8(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_02242FB0(BattleFactoryAppPokeballSprite *ballSprite, int palIndex) {
    Sprite_SetPaletteOverride(ballSprite->sprite, palIndex);
}

void ov81_02242FBC(BattleFactoryAppPokeballSprite *ballSprite, GXOamMode mode) {
    Sprite_SetOamMode(ballSprite->sprite, mode);
}

void *ov81_02242FC8(BattleFactoryAppPokeballSprite *ballSprite) {
    Sprite_Delete(ballSprite->sprite);
    Heap_Free(ballSprite);

    return NULL;
}
