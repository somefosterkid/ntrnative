#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_pokewalker_app.h"

void *ov112_021E7668(void);

void ov112_021EA60C(Ov112PokewalkerApp *param_1) {
    u16 uVar1;
    void *iVar2;
    ManagedSprite *managedSprite;

    managedSprite = param_1->sprites[2];
    iVar2 = ov112_021E7668();
    if (iVar2 != 0) {
        uVar1 = ManagedSprite_GetActiveAnim(managedSprite);
        if (uVar1 == 2) {
            ManagedSprite_SetAnim(param_1->sprites[2], 1);
            return;
        }
    } else {
        uVar1 = ManagedSprite_GetActiveAnim(managedSprite);
        if (uVar1 == 1) {
            ManagedSprite_SetAnim(param_1->sprites[2], 2);
        }
    }
    return;
}

void ov112_021EA64C(Ov112PokewalkerApp *arg0) {
    ManagedSprite_SetDrawFlag(arg0->sprites[1], 0);
    ManagedSprite_SetDrawFlag(arg0->sprites[2], 0);
}

void ov112_021EA670(Ov112PokewalkerApp *arg0, s32 arg1) {
    ManagedSprite_SetDrawFlag(arg0->sprites[arg1], 1);
}

void ov112_021EA688(Ov112PokewalkerApp *arg0, s32 arg1) {
    ManagedSprite_SetDrawFlag(arg0->sprites[arg1], 0);
}

void ov112_021EA6A0(Ov112PokewalkerApp *arg0, s32 arg1, s32 seqno) {
    ManagedSprite_SetAnim(arg0->sprites[arg1], seqno);
}
