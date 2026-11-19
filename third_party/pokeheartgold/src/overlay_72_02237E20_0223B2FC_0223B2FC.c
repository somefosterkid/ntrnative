#include "global.h"

#include "sprite.h"
#include "overlay_72_wfc_connect.h"

extern u32 ov72_0223B4C0;

extern u32 ov72_0223B4C2;

void ov72_0223B2FC(NintendoWfcYesNoMenu *param_1, s32 param_2);

void ov72_0223B2FC(NintendoWfcYesNoMenu *param_1, s32 param_2) {
    VecFx32 VStack_10;

    VStack_10.x = (u32) * (u16 *)(&ov72_0223B4C0 + param_2) << 0xc;
    VStack_10.y = (param_1->cursorOffsetY + (u32) * (u16 *)(&ov72_0223B4C2 + param_2)) * 0x1000;
    VStack_10.z = 0;
    Sprite_SetMatrix(param_1->cursorSprite, &VStack_10);
    return;
}
