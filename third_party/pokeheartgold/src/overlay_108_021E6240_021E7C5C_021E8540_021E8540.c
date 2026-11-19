#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "heap.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

void ov108_021E84F8(enum HeapID *param_1, s32 param_2, s32 param_3);

u32 *ov108_021E8540(enum HeapID *param_1, s32 param_2, s32 param_3, u8 param_4, u8 param_5, u8 param_6, u8 param_7, NNS_G2D_VRAM_TYPE param_8);

u32 *ov108_021E8540(enum HeapID *param_1, s32 param_2, s32 param_3, u8 param_4, u8 param_5, u8 param_6, u8 param_7, NNS_G2D_VRAM_TYPE param_8) {
    u32 *dest;
    void *pvVar1;
    float fVar;
    Sprite *pSVar3;
    SpriteTemplate SStack_48;

    dest = Heap_Alloc(*param_1, 0x10);
    MI_CpuFill8(dest, 0, 0x10);
    pvVar1 = Heap_Alloc(*param_1, 8);
    dest[2] = (u32)(pvVar1);
    pvVar1 = Heap_Alloc(*param_1, 0x24);
    *(void **)dest[2] = pvVar1;
    dest[1] = *(u32 *)dest[2];
    CreateSpriteResourcesHeader((SpriteResourcesHeader *)dest[1], param_6 + 0xe000, 0xe000, 0xe000, 0xe000, -1, -1, 0, param_4, (GF_2DGfxResMan *)param_1[0x51], (GF_2DGfxResMan *)param_1[0x52], (GF_2DGfxResMan *)param_1[0x53], (GF_2DGfxResMan *)param_1[0x54], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    if (param_1[5] != 0) {
        SStack_48.spriteList = (SpriteList *)param_1[5];
    } else {
        SStack_48.spriteList = (SpriteList *)param_1[4];
    }
    SStack_48.header = (SpriteResourcesHeader *)dest[1];
    if (param_2 != 0) {
        fVar = 0.5f + (float)(param_2 << 0xc);
    } else {
        fVar = (float)(param_2 << 0xc) - 0.5f;
    }
    SStack_48.position.x = (fx32)fVar;
    if (param_3 != 0) {
        fVar = 0.5f + (float)(param_3 << 0xc);
    } else {
        fVar = (float)(param_3 << 0xc) - 0.5f;
    }
    SStack_48.position.y = (fx32)fVar;
    SStack_48.position.z = 0;
    if (param_8 == NNS_G2D_VRAM_TYPE_2DSUB) {
        SStack_48.position.y = SStack_48.position.y + 0xc0000;
    }
    SStack_48.scale.x = 0x1000;
    SStack_48.scale.y = 0x1000;
    SStack_48.scale.z = 0x1000;
    SStack_48.rotation = 0;
    SStack_48.drawPriority = (u32)param_5;
    SStack_48.whichScreen = param_8;
    SStack_48.heapID = *param_1;
    ov108_021E84F8(param_1, (u32)param_6, (u32)param_7);
    pSVar3 = Sprite_CreateAffine(&SStack_48);
    *dest = (u32)(pSVar3);
    if (pSVar3 == NULL) {
        GF_AssertFail();
    }
    return dest;
}
