#include "global.h"

#include "sprite_system.h"

void ov40_0222D288(ManagedSprite *arg0, s16 arg1, s16 arg2);

void ov40_0222D288(ManagedSprite *managedSprite, s16 x, s16 y) {
    ManagedSprite_SetPositionXYWithSubscreenOffset(managedSprite, x, y, 0x100000);
}

void ov40_0222D294(ManagedSprite *arg0, s16 *arg1, s16 *arg2);

void ov40_0222D294(ManagedSprite *managedSprite, s16 *x, s16 *y) {
    ManagedSprite_GetPositionXYWithSubscreenOffset(managedSprite, x, y, 0x100000);
}
