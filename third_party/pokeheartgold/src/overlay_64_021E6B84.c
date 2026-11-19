#include "global.h"

#include "overlay_64_hall_of_fame_showcase.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

u32 ov64_021E6B84(HallOfFameShowcase *showcase);

void ov64_021E6BD8(HallOfFameShowcase *showcase, s32 arg1);

u32 ov64_021E6B84(HallOfFameShowcase *showcase) {
    s32 iVar1;

    iVar1 = showcase->buttonAnimTimer;
    if (iVar1 == 0) {
        ManagedSprite_SetPaletteOverride(showcase->sprites[showcase->pressedButton], 1);
    } else if (iVar1 == 4) {
        ManagedSprite_SetPaletteOverride(showcase->sprites[showcase->pressedButton], 0);
    } else if (iVar1 == 6) {
        showcase->buttonAnimTimer = 0;
        return 0;
    }
    showcase->buttonAnimTimer = showcase->buttonAnimTimer + 1;
    return 1;
}

void ov64_021E6BD8(HallOfFameShowcase *showcase, s32 target) {
    DpadMenuBox *dpadMenuBox;
    s32 seqno;

    dpadMenuBox = (struct DpadMenuBox *)(GridInputHandler_GetDpadBox(showcase->gridInput, target));
    ManagedSprite_SetPositionXY(showcase->sprites[14], (s16)dpadMenuBox->left, (s16)dpadMenuBox->top);
    if (target == 6) {
        seqno = 4;
    } else {
        seqno = 3;
    }
    ManagedSprite_SetAnimationFrame(showcase->sprites[14], 0U);
    ManagedSprite_SetAnim(showcase->sprites[14], seqno);
}
