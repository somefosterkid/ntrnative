#include "global.h"

#include "sprite_system.h"

void ov111_021E6170(SpriteSystem *spriteSystem, SpriteManager *spriteManager);

void ov111_021E6170(SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    SpriteSystem_FreeResourcesAndManager(spriteSystem, spriteManager);
    SpriteSystem_Free(spriteSystem);
}
