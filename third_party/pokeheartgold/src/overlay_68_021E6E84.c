#include "global.h"
#include "overlay_68_move_relearner.h"
#include "sprite_system.h"

void ov68_021E6E84(MoveRelearner *relearner);

void ov68_021E6E84(MoveRelearner *relearner) {
    u32 i;

    i = 0;
    do {
        Sprite_DeleteAndFreeResources(relearner->sprites[i]);
        i += 1;
    } while (i < MOVE_RELEARNER_NUM_SPRITES);
    SpriteSystem_FreeResourcesAndManager(relearner->spriteSystem, relearner->spriteManager);
    SpriteSystem_Free(relearner->spriteSystem);
}
