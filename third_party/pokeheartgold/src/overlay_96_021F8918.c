#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

void ov96_021F8E94(u32 *param_1);

void ov96_021F8918(void *param_1);

void ov96_021F8918(void *param_1) {
    if (*(s32 *)((u8 *)param_1 + 0x30) == 0) {
        GF_AssertFail();
    }
    *(u32 *)((u8 *)param_1 + 0x30) = 0;
    ov96_021F8E94((u32 *)((u8 *)param_1 + 0x10));
    ov96_021F8E94((u32 *)((u8 *)param_1 + 0x20));
    return;
}

void ov96_021F893C(void *param_1, s32 *param_2, s32 *param_3);

void ov96_021F893C(void *param_1, s32 *param_2, s32 *param_3) {
    *param_2 = *(s32 *)(param_1 + 0x38);
    *param_3 = *(s32 *)(param_1 + 0x3c);
    return;
}

void ov96_021F8C88(s32 param_1, s32 param_2, u32 *param_3);

void ov96_021F8948(void *param_1, s32 param_2, s32 param_3);

void ov96_021F8948(void *param_1, s32 param_2, s32 param_3) {
    *(s32 *)(param_1 + 0x38) = param_2;
    *(s32 *)(param_1 + 0x3c) = param_3;
    ov96_021F8C88(param_2, param_3, (u32 *)param_1);
    return;
}

void ov96_021F8CFC(s32 param_1, s32 param_2, u32 *param_3);

void ov96_021F895C(void *param_1, s32 param_2, s32 param_3);

void ov96_021F895C(void *param_1, s32 param_2, s32 param_3) {
    *(s32 *)(param_1 + 0x38) = *(s32 *)(param_1 + 0x38) + param_2;
    *(s32 *)(param_1 + 0x3c) = *(s32 *)(param_1 + 0x3c) + param_3;
    ov96_021F8CFC(param_2, param_3, (u32 *)param_1);
    return;
}

u16 ov96_021F8978(void *param_1);

u16 ov96_021F8978(void *param_1) {
    return *(u16 *)((u8 *)param_1 + 0x44);
}

void ov96_021F8980(void *arg0);

void ov96_021F8980(void *arg0) {
    GfGfxResType type;
    void *v0;

    *(SpriteList **)((u8 *)arg0 + 0x17C) = G2dRenderer_Init(0x37, arg0 + 0x180, *(enum HeapID *)arg0);
    type = GF_GFX_RES_TYPE_CHAR;
    v0 = arg0;
    do {
        *(GF_2DGfxResMan **)((u8 *)v0 + 0x2A8) = Create2DGfxResObjMan(1, type, *(enum HeapID *)arg0);
        v0 += 4;
        type += 1;
    } while (type < 6);
    *(SpriteResource **)((u8 *)arg0 + 0x2C0) = AddCharResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x2A8), NARC_a_1_6_0, 9, 0, 0x67, 1, *(enum HeapID *)arg0);
    *(SpriteResource **)((u8 *)arg0 + 0x2C4) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x2AC), NARC_a_1_6_0, 6, 0, 0x67, 1, 6, *(enum HeapID *)arg0);
    *(SpriteResource **)((u8 *)arg0 + 0x2C8) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x2B0), NARC_a_1_6_0, 8, 0, 0x67, GF_GFX_RES_TYPE_CELL, *(enum HeapID *)arg0);
    *(SpriteResource **)((u8 *)arg0 + 0x2CC) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x2B4), NARC_a_1_6_0, 7, 0, 0x67, GF_GFX_RES_TYPE_ANIM, *(enum HeapID *)arg0);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(*(SpriteResource **)((u8 *)arg0 + 0x2C0));
    SpriteTransfer_CreateExtPlttTransferTask(*(SpriteResource **)((u8 *)arg0 + 0x2C4));
}

void ov96_021F8A50(void *param_1);

void ov96_021F8A50(void *param_1) {
    CreateSpriteResourcesHeader((SpriteResourcesHeader *)(param_1 + 0x2d8), 0x67, 0x67, 0x67, 0x67, -1, -1, 0, 0, *(GF_2DGfxResMan **)(param_1 + 0x2a8), *(GF_2DGfxResMan **)(param_1 + 0x2ac), *(GF_2DGfxResMan **)(param_1 + 0x2b0), *(GF_2DGfxResMan **)(param_1 + 0x2b4), (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    return;
}

void ov96_021F8A98(u32 *param_1, u32 *param_2, u32 param_3, u32 param_4);

void ov96_021F8A98(u32 *param_1, u32 *param_2, u32 param_3, u32 param_4) {
    *param_1 = param_2[0x5f];
    param_1[1] = (u32)(param_2 + 0xb6);
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[4] = 0;
    param_1[5] = 0x1000;
    param_1[6] = 0x1000;
    param_1[7] = 0x1000;
    *(u16 *)(param_1 + 8) = 0;
    param_1[9] = 2;
    param_1[10] = param_4;
    param_1[0xb] = *param_2;
    return;
}

Sprite *ov96_021F8ACC(SpriteTemplate *param_1, s32 param_2, u32 param_3);

Sprite *ov96_021F8ACC(SpriteTemplate *param_1, s32 param_2, u32 param_3) {
    Sprite *sprite;

    sprite = Sprite_CreateAffine(param_1);
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, param_2);
    Sprite_SetDrawFlag(sprite, 0);
    Sprite_SetDrawPriority(sprite, param_3);
    return sprite;
}
