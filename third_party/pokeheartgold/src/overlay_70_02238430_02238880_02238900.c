#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon_icon_idx.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"

#include "overlay_70_gtsapplication_state.h"

typedef enum GF_GraphicsResourceType GF_GraphicsResourceType;

void ov70_02238900(GTSApplicationState *param_1);

void ov70_02238900(GTSApplicationState *param_1) {
    s32 i;
    NARC *narc;
    SpriteList *pSVar1;
    GF_2DGfxResMan *pGVar2;
    SpriteResource *pSVar3;
    u32 memberNo;
    void *ptr;
    u32 uVar4;
    s32 iVar5_2;
    u16 *puVar6;
    NNSG2dPaletteData *apNStack_1c;
    narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);
    NNS_G2dInitOamManagerModule();
    *((u32 *)REG_DISPCNT_ADDR) = ((*((u32 *)REG_DISPCNT_ADDR)) & 0xffcfffef) | 0x10;
    *((u32 *)REG_DB_DISPCNT_ADDR) = ((*((u32 *)REG_DB_DISPCNT_ADDR)) & 0xffcfffef) | 0x10;
    OamManager_Create(0, 0x7a, 0, 0x20, 0, 0x7e, 0, 0x20, HEAP_ID_61);
    pSVar1 = G2dRenderer_Init(0x54, &param_1->g2dRenderer, HEAP_ID_61);
    param_1->spriteList = pSVar1;
    G2dRenderer_SetSubSurfaceCoords(&param_1->g2dRenderer, 0, 0x100000);
    for (i = 0; i < 4; i++) {
        pGVar2 = Create2DGfxResObjMan(3, (GfGfxResType)i, HEAP_ID_61);
        param_1->spriteResourceCollection[i] = pGVar2;
    }
    pSVar3 = AddCharResObjFromOpenNarc(param_1->spriteResourceCollection[0], narc, 0x15, 1, 0, 1, HEAP_ID_61);
    param_1->spriteResource[0][0] = pSVar3;
    pSVar3 = AddPlttResObjFromOpenNarc(param_1->spriteResourceCollection[1], narc, 10, 0, 0, 1, 3, HEAP_ID_61);
    param_1->spriteResource[0][1] = pSVar3;
    pSVar3 = AddCellOrAnimResObjFromOpenNarc(param_1->spriteResourceCollection[2], narc, 0x16, 1, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_61);
    param_1->spriteResource[0][2] = pSVar3;
    pSVar3 = AddCellOrAnimResObjFromOpenNarc(param_1->spriteResourceCollection[3], narc, 0x17, 1, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_61);
    param_1->spriteResource[0][3] = pSVar3;
    pSVar3 = AddCharResObjFromOpenNarc(param_1->spriteResourceCollection[0], narc, 0x2b, 1, 1, 2, HEAP_ID_61);
    param_1->spriteResource[1][0] = pSVar3;
    pSVar3 = AddPlttResObjFromOpenNarc(param_1->spriteResourceCollection[1], narc, 9, 0, 1, 2, 10, HEAP_ID_61);
    param_1->spriteResource[1][1] = pSVar3;
    pSVar3 = AddCellOrAnimResObjFromOpenNarc(param_1->spriteResourceCollection[2], narc, 0x2c, 1, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_61);
    param_1->spriteResource[1][2] = pSVar3;
    pSVar3 = AddCellOrAnimResObjFromOpenNarc(param_1->spriteResourceCollection[3], narc, 0x2d, 1, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_61);
    param_1->spriteResource[1][3] = pSVar3;
    SpriteTransfer_CreateCharTransferTask(param_1->spriteResource[0][0]);
    SpriteTransfer_CreateCharTransferTask(param_1->spriteResource[1][0]);
    SpriteTransfer_CreateExtPlttTransferTask(param_1->spriteResource[0][1]);
    SpriteTransfer_CreateExtPlttTransferTask(param_1->spriteResource[1][1]);
    memberNo = sub_02074490();
    ptr = GfGfxLoader_GetPlttData(NARC_poketool_icongra_poke_icon, memberNo, &apNStack_1c, HEAP_ID_61);
    DC_FlushRange(apNStack_1c->pRawData, 0x60);
    GX_LoadOBJPltt(apNStack_1c->pRawData, 0x60, 0x60);
    puVar6 = apNStack_1c->pRawData;
    iVar5_2 = 0;
    do {
        uVar4 = (u32)(*puVar6);
        iVar5_2 = iVar5_2 + 1;
        *puVar6 = (((((((s32)uVar4) >> 10) & 0x1f) / 2) << 10) | ((((((s32)uVar4) >> 5) & 0x1f) / 2) << 5)) | ((((s32)uVar4) & 0x1f) / 2);
        puVar6 = puVar6 + 1;
    } while (iVar5_2 < 0x30);
    DC_FlushRange(apNStack_1c->pRawData, 0x60);
    GX_LoadOBJPltt(apNStack_1c->pRawData, 0xc0, 0x60);
    Heap_Free(ptr);
    NARC_Delete(narc);
    return;
}
