#include "global.h"

#include "sprite_system.h"

struct ManagedSprite;

void ov08_02223F74(ManagedSprite *sprite, const int x, const int y);

void ov08_02223F74(ManagedSprite *sprite, const int x, const int y) {
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    ManagedSprite_SetPositionXY(sprite, x, y);
}
