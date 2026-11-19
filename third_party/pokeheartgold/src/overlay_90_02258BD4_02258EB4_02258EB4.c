#include "global.h"

#include "constants/heap.h"

#include "sprite.h"
#include "unk_02009D48.h"

void ov90_02258EB4(u8 *param_1, SpriteList *param_2, s32 param_3, s32 param_4, u16 param_5, enum HeapID param_6);

void ov90_02258EB4(u8 *param_1, SpriteList *param_2, s32 param_3, s32 param_4, u16 param_5, enum HeapID param_6) {
    SimpleSpriteTemplate SStack_28;

    SStack_28.header = (SpriteResourcesHeader *)(param_1 + 0x14);
    SStack_28.position.x = param_3 << 0xc;
    SStack_28.position.y = param_4 << 0xc;
    SStack_28.spriteList = param_2;
    SStack_28.priority = (u32)param_5;
    SStack_28.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    SStack_28.heapID = param_6;
    Sprite_Create(&SStack_28);
    return;
}
