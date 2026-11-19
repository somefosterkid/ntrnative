#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_0220_data.h"

u16 ov96_0220AD4C(void *);

void ov96_02209BB0(Ov96_0220Data *arg0);

void ov96_02209BB0(Ov96_0220Data *arg0) {
    s32 v0;
    s32 v1;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    v1 = ov96_0220AD4C(arg0->unk_40);
    v0 = 0;
loop_3:
    Sprite_SetAnimCtrlSeq(arg0->unk_2C[(2 - v0)], (u16)(v1 % 10) + 1);
    if ((u32)v1 >= 0xAU) {
        v1 = (s32)(u16)(v1 / 10);
        v0 += 1;
        if (v0 < 3) {
            goto loop_3;
        }
    }
}

void ov96_02209BF8(Ov96_0220Data *param_1);

void ov96_02209BF8(Ov96_0220Data *param_1) {
    s32 iVar1;

    iVar1 = 0;
    do {
        ManagedSprite_SetAnimateFlag(param_1->unk_24[iVar1], 0);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 2);
    return;
}
