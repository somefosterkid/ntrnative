#include "global.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_97_course_sub_screen.h"

void ov97_0221FB80(SpriteTemplate *, SpriteList *, SpriteResourcesHeader *, NNS_G2D_VRAM_TYPE, enum HeapID);

Sprite *ov97_0221FAEC(Ov97CourseSubScreenSprites *param_1, s32 param_2, u32 param_3, s32 param_4, s32 param_5, s32 param_6, s32 param_7, BOOL param_8);

Sprite *ov97_0221FAEC(Ov97CourseSubScreenSprites *param_1, s32 param_2, u32 param_3, s32 param_4, s32 param_5, s32 param_6, s32 param_7, BOOL param_8) {
    Sprite *sprite;
    SpriteTemplate SStack_40;
    SpriteResourcesHeader SStack_64;

    CreateSpriteResourcesHeader(&SStack_64, param_2, param_2, param_2, param_2, -1, -1, 0, param_4, param_1->resMan[0], param_1->resMan[1], param_1->resMan[2], param_1->resMan[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    ov97_0221FB80(&SStack_40, param_1->spriteList, &SStack_64, NNS_G2D_VRAM_TYPE_2DSUB, param_1->heapID);
    SStack_40.position.x = param_5 << 0xc;
    SStack_40.position.y = param_6 * 0x1000 + 0x100000;
    SStack_40.drawPriority = param_3;
    sprite = Sprite_CreateAffine(&SStack_40);
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, param_7);
    Sprite_SetPriority(sprite, 1);
    Sprite_SetDrawFlag(sprite, param_8);
    return sprite;
}
