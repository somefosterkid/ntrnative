#include "global.h"

#include "sprite_system.h"
#include "error_handling.h"
#include "sprite.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
ManagedSprite *ov96_0220D13C(SpriteSystem *, SpriteManager *, s16, u8, u16, u16);
#else
ManagedSprite *ov96_0220D13C(SpriteSystem *, SpriteManager *, s32, s32, s32, s32);
#endif

void ov96_0220B178(ManagedSprite **arg0, SpriteSystem *arg1, SpriteManager *arg2);

void ov96_0220B1B8(void **param_1);

void ov96_0220B178(ManagedSprite **arg0, SpriteSystem *arg1, SpriteManager *arg2) {
    ManagedSprite **v1;
    ManagedSprite *managedSprite;
    s32 v0;

    v1 = arg0;
    v0 = 0;
    do {
        managedSprite = ov96_0220D13C(arg1, arg2, 0, 0, (s32)(u16)((v0 % 3) + 0x1C), 1);
        *v1 = managedSprite;
        ManagedSprite_SetDrawFlag(managedSprite, 0);
        v0 += 1;
        v1 += 5;
    } while (v0 < 0x10);
}

void ov96_0220B1B8(void **param_1) {
    s32 iVar1;

    iVar1 = 0;
    do {
        if (*param_1 == 0) {
            GF_AssertFail();
        }
        Sprite_DeleteAndFreeResources((ManagedSprite *)*param_1);
        iVar1 = iVar1 + 1;
        param_1 = param_1 + 5;
    } while (iVar1 < 0x10);
    return;
}
