#include "global.h"

#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_anim_system.h"
#include "sprite_system.h"

void ov07_0221DA74(BattleAnimSystem *arg0);

void ov07_0221DA74(BattleAnimSystem *arg0) {
    SpriteManager *spriteManager;

    arg0->scriptPtr++;
    spriteManager = arg0->pokemonSpriteManager;
    if (spriteManager != NULL) {
        SpriteSystem_FreeResourcesAndManager(arg0->context->spriteSystem, spriteManager);
    }
    arg0->pokemonSpriteManager = NULL;
}
