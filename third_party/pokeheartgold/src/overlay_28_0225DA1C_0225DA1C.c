#include "global.h"
#include "constants/heap.h"
#include "overlay_28.h"
#include "sprite.h"
#include "unk_02009D48.h"

void ov28_0225DA1C(DowsingMchnApp *app, s32 idx, SpriteResourcesHeader *header, const DowsingMchnSpriteDef *def) {
    Sprite **ppSprite;
    SpriteTemplate SStack_40;

    ppSprite = app->sprites;
    SStack_40.spriteList = app->spriteList;
    SStack_40.header = header;
    SStack_40.position.x = (u32)def->x << 0xc;
    SStack_40.position.y = (u32)def->y * 0x1000 + 0x100000;
    SStack_40.position.z = 0;
    SStack_40.scale.x = 0x1000;
    SStack_40.scale.y = 0x1000;
    SStack_40.scale.z = 0x1000;
    SStack_40.rotation = 0;
    SStack_40.drawPriority = (u32)def->drawPriority;
    SStack_40.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    SStack_40.heapID = HEAP_ID_8;
    ppSprite[idx] = Sprite_CreateAffine(&SStack_40);
    Sprite_SetAnimCtrlSeq(ppSprite[idx], (u32)def->animSeq);
    return;
}
