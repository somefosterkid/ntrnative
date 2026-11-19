#include "global.h"

#include "sprite_system.h"

struct ManagedSprite;

void ov08_02220A8C(ManagedSprite *sprite, const int x, const int y);

void ov08_02220A8C(ManagedSprite *sprite, const int x, const int y) {
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    ManagedSprite_SetPositionXY(sprite, x, y);
}
