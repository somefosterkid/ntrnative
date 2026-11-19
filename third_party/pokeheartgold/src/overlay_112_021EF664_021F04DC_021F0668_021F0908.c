#include "global.h"

#include "sprite.h"
#include "overlay_112_sub_screen.h"

void ov112_021F0D04(Ov112SubScreenSprites *arg0);
void ov112_021F0C8C(Ov112SubScreenSprites *arg0, void *arg1);
void ov112_021F0908(Ov112SubScreen *param_1, u8 *param_2);

void ov112_021F0908(Ov112SubScreen *param_1, u8 *param_2) {
    u32 uVar1;
    Ov112SubScreenSprites *iVar2;

    iVar2 = &param_1->sprites;
    ov112_021F0C8C(iVar2, param_2);
    ov112_021F0D04(&param_1->sprites);
    Sprite_SetDrawFlag(param_1->sprites.monSprite, 1);
    uVar1 = 0;
    do {
        if (*(u8 *)(param_2 + uVar1 + 0x20) != 0) {
            Sprite_SetDrawFlag(iVar2->slotSprites[uVar1], 1);
            Sprite_SetAnimCtrlSeq(iVar2->slotSprites[uVar1], uVar1 * 2 + 1);
        } else {
            Sprite_SetDrawFlag(iVar2->slotSprites[uVar1], 1);
            Sprite_SetAnimCtrlSeq(iVar2->slotSprites[uVar1], uVar1 << 1);
        }
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 6);
    return;
}
