#include "global.h"

#include "gf_gfx_planes.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "heap.h"
#include "sprite.h"
#include "overlay_112_sub_screen.h"

void ov112_021F09B4(Ov112SubScreenSprites *arg0);
Sprite *ov112_021F0B9C(Ov112SubScreenSprites *, s32, u32, s32, s32, s32, s32, BOOL);
void ov112_021F0C50(Ov112SubScreenSprites *);
void ov112_021F0B28(Ov112SubScreenSprites *arg0);

void ov112_021F09B4(Ov112SubScreenSprites *arg0) {
    GfGfxResType type;
    s32 i;

    arg0->spriteList = G2dRenderer_Init(7, &arg0->renderer, arg0->heapID);
    G2dRenderer_SetSubSurfaceCoords(&arg0->renderer, 0, FX32_CONST(256));
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    type = GF_GFX_RES_TYPE_CHAR;
    i = 0;
    do {
        arg0->resMan[i] = Create2DGfxResObjMan(2, type, arg0->heapID);
        i += 1;
        type += 1;
    } while (type < 6);
    arg0->resObj[0][0] = AddCharResObjFromNarc(arg0->resMan[0], NARC_a_2_5_7, 4, 1, 1, 2, arg0->heapID);
    arg0->resObj[0][1] = AddPlttResObjFromNarc(arg0->resMan[1], NARC_a_2_5_7, 3, 0, 1, 2, 4, arg0->heapID);
    arg0->resObj[0][2] = AddCellOrAnimResObjFromNarc(arg0->resMan[2], NARC_a_2_5_7, 6, 1, 1, GF_GFX_RES_TYPE_CELL, arg0->heapID);
    arg0->resObj[0][3] = AddCellOrAnimResObjFromNarc(arg0->resMan[3], NARC_a_2_5_7, 5, 1, 1, GF_GFX_RES_TYPE_ANIM, arg0->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(arg0->resObj[0][0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->resObj[0][1]);
    arg0->resObj[1][0] = AddCharResObjFromNarc(arg0->resMan[0], NARC_application_choose_starter_choose_starter_sub_res, 9, 0, 2, 2, arg0->heapID);
    arg0->resObj[1][1] = AddPlttResObjFromNarc(arg0->resMan[1], NARC_application_choose_starter_choose_starter_sub_res, 6, 0, 2, 2, 4, arg0->heapID);
    arg0->resObj[1][2] = AddCellOrAnimResObjFromNarc(arg0->resMan[2], NARC_application_choose_starter_choose_starter_sub_res, 0xA, 0, 2, GF_GFX_RES_TYPE_CELL, arg0->heapID);
    arg0->resObj[1][3] = AddCellOrAnimResObjFromNarc(arg0->resMan[3], NARC_application_choose_starter_choose_starter_sub_res, 0xA, 0, 2, GF_GFX_RES_TYPE_ANIM, arg0->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(arg0->resObj[1][0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->resObj[1][1]);
}

void ov112_021F0B28(Ov112SubScreenSprites *arg0) {
    u8 v0;

    v0 = 0;
    do {
        arg0->slotSprites[v0] = ov112_021F0B9C(arg0, 1, 1, 0, (v0 * 8) + 0x18, 0x90, v0 * 2, 0);
        v0 += 1;
    } while ((u32)v0 < 6U);
    arg0->monSprite = ov112_021F0B9C(arg0, 2, 0, 0, 0x30, 0x58, 0, 0);
    arg0->charData = Heap_Alloc(arg0->heapID, 0xC80U);
    ov112_021F0C50(arg0);
}
