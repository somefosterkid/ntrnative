#include "global.h"

#include "sprite_system.h"

s32 ov07_022222F0(void *, ManagedSprite *);

s32 ov07_02222748(void *, ManagedSprite *);

s32 ov07_02230F6C(ManagedSprite *arg0, void *arg1, void *arg2);

s32 ov07_02230F6C(ManagedSprite *managedSprite, void *arg1, void *arg2) {
    ov07_022222F0(arg1, managedSprite);
    if (ov07_02222748(arg2, managedSprite) != 0) {
        return 0;
    }
    ManagedSprite_SetDrawFlag(managedSprite, 0);
    return 1;
}
