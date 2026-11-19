#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

void ov70_02238B54(SpriteTemplate *template, struct GTSApplicationState *appState, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen);

extern u32 ov70_02245240;

extern u32 ov70_02245242;

void ov70_02239414(GTSApplicationState *param_1);

void ov70_02239414(GTSApplicationState *param_1) {
    Sprite *pSVar1;
    SpriteTemplate SStack_38;
    SpriteResourcesHeader SStack_5c;

    CreateSpriteResourcesHeader(&SStack_5c, 0, 0, 0, 0, -1, -1, 0, 1, param_1->spriteResourceCollection[0], param_1->spriteResourceCollection[1], param_1->spriteResourceCollection[2], param_1->spriteResourceCollection[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    ov70_02238B54(&SStack_38, param_1, &SStack_5c, NNS_G2D_VRAM_TYPE_2DMAIN);
    SStack_38.position.x = (u32) * (u16 *)(&ov70_02245240 + (u32) param_1->mainMenuSelectedOption) << 0xc;
    SStack_38.position.y = (u32) * (u16 *)(&ov70_02245242 + (u32) param_1->mainMenuSelectedOption) << 0xc;
    pSVar1 = Sprite_CreateAffine(&SStack_38);
    param_1->cursorSprite = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->cursorSprite, 1);
    Sprite_SetAnimCtrlSeq(param_1->cursorSprite, 0x2b);
    return;
}
