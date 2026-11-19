#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "sprite.h"
#include "unk_0203A3B0.h"

void ov70_02238B54(SpriteTemplate *, GTSApplicationState *, SpriteResourcesHeader *, u32);

extern u8 ov70_022464F0;

void ov70_0223B9C8(GTSApplicationState *param_1);

void ov70_0223B9C8(GTSApplicationState *param_1) {
    u32 new_var;
    Sprite *pSVar1;
    int new_var2;
    u32 new_var4;
    SpriteTemplate SStack_38;
    new_var2 = 0x2d;
    ov70_02238B54(&SStack_38, param_1, &param_1->cursorSpriteResourceHeader, 1);
    new_var4 = ((u32)((u16 *)(&ov70_022464F0))[7]) << 0xc;
    SStack_38.position.y = (new_var = ((u32)((u16 *)(&ov70_022464F0))[8]) << 0xc);
    SStack_38.position.x = new_var4;
    pSVar1 = Sprite_CreateAffine(&SStack_38);
    param_1->cursorSprite = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->cursorSprite, 1);
    Sprite_SetAnimCtrlSeq(param_1->cursorSprite, new_var2);
    Sprite_SetPriority(param_1->cursorSprite, 1);
    Sprite_SetOamMode(param_1->cursorSprite, GX_OAM_MODE_XLU);
    SStack_38.position.x = 0xa0000;
    SStack_38.position.y = 0x20000;
    pSVar1 = Sprite_CreateAffine(&SStack_38);
    param_1->unused_DD0 = pSVar1;
    Sprite_SetAnimCtrlSeq(param_1->unused_DD0, 0x2f);
    Sprite_SetDrawFlag(param_1->unused_DD0, 0);
    SStack_38.position.x = 0xe4000;
    SStack_38.position.y = 0x75000;
    pSVar1 = Sprite_CreateAffine(&SStack_38);
    param_1->boxArrowSprites[0] = pSVar1;
    Sprite_SetAnimCtrlSeq(param_1->boxArrowSprites[0], 0x26);
    Sprite_SetDrawFlag(param_1->boxArrowSprites[0], 0);
    SStack_38.position.x = 0x8c000;
    pSVar1 = Sprite_CreateAffine(&SStack_38);
    param_1->boxArrowSprites[1] = pSVar1;
    Sprite_SetAnimCtrlSeq(param_1->boxArrowSprites[1], 0x27);
    Sprite_SetDrawFlag(param_1->boxArrowSprites[1], 0);
    sub_0203A880();
    return;
}
