#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

void ov07_022311B0(ManagedSprite *sprite, int *outVisibleFrames, int *outDelay, int idx, int dir);
BOOL ov07_02231204(ManagedSprite *sprite, int *visibleFrames, int *delay);

void ov07_022311B0(ManagedSprite *sprite, int *outVisibleFrames, int *outDelay, int idx, int dir) {
    ManagedSprite_SetAnim(sprite, idx / 2);

    *outDelay = idx * 2;
    *outVisibleFrames = 16;

    s16 spriteX, spriteY;
    ManagedSprite_GetPositionXY(sprite, &spriteX, &spriteY);

    spriteX += (40 + (-12) * idx) * dir;
    spriteY += 40;

    ManagedSprite_SetPositionXY(sprite, spriteX, spriteY);
}

BOOL ov07_02231204(ManagedSprite *sprite, int *visibleFrames, int *delay) {
    if (*delay > 0) {
        (*delay)--;

        if (*delay == 0) {
            ManagedSprite_SetDrawFlag(sprite, 1);
        }
    } else {
        if (*visibleFrames > 0) {
            (*visibleFrames)--;
        } else {
            ManagedSprite_SetDrawFlag(sprite, 0);
            return 1;
        }
    }

    return 0;
}
