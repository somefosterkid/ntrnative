#include "global.h"
struct UnkStruct_ov115_02261ADC;

#include "constants/heap.h"

#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"

void ov91_0225F0C0(void **param_1, struct UnkStruct_ov115_02261ADC *param_2, enum HeapID param_3);

void ov91_0225F0C0(void **param_1, UnkStruct_ov115_02261ADC *param_2, enum HeapID param_3) {
    NARC *narc;
    SpriteResource *pSVar1;
    BOOL BVar2;
    Sprite *sprite;
    SimpleSpriteTemplate SStack_34;

    memset(param_1, 0, 0x40);
    narc = NARC_New(NARC_a_1_9_8, param_3);
    pSVar1 = AddCharResObjFromOpenNarc(*((GF_2DGfxResMan * *)&param_2->unk_1AC[0]), narc, 0x15, 0, 0x78, 2, param_3);
    *param_1 = (u8 *)(pSVar1);
    pSVar1 = AddPlttResObjFromOpenNarc(*((GF_2DGfxResMan * *)&param_2->unk_1AC[1]), narc, 0x14, 0, 0x78, 2, 2, param_3);
    param_1[1] = (u8 *)(pSVar1);
    pSVar1 = AddCellOrAnimResObjFromOpenNarc(*((GF_2DGfxResMan * *)&param_2->unk_1AC[2]), narc, 0x16, 0, 0x78, GF_GFX_RES_TYPE_CELL, param_3);
    param_1[2] = (u8 *)(pSVar1);
    pSVar1 = AddCellOrAnimResObjFromOpenNarc(*((GF_2DGfxResMan * *)&param_2->unk_1AC[3]), narc, 0x17, 0, 0x78, GF_GFX_RES_TYPE_ANIM, param_3);
    param_1[3] = (u8 *)(pSVar1);
    BVar2 = SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd((SpriteResource *)*param_1);
    if (BVar2 == 0) {
        GF_AssertFail();
    }
    BVar2 = SpriteTransfer_CreatePlttTransferTask((SpriteResource *)param_1[1]);
    if (BVar2 == 0) {
        GF_AssertFail();
    }
    sub_0200A740((SpriteResource *)*param_1);
    sub_0200A740((SpriteResource *)param_1[1]);
    CreateSpriteResourcesHeader((SpriteResourcesHeader *)(param_1 + 4), 0x78, 0x78, 0x78, 0x78, -1, -1, 0, 0, *((GF_2DGfxResMan * *)&param_2->unk_1AC[0]), *((GF_2DGfxResMan * *)&param_2->unk_1AC[1]), *((GF_2DGfxResMan * *)&param_2->unk_1AC[2]), *((GF_2DGfxResMan * *)&param_2->unk_1AC[3]), (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_34.spriteList = param_2->unk_1C;
    SStack_34.header = (SpriteResourcesHeader *)(param_1 + 4);
    SStack_34.position.x = 0x80000;
    SStack_34.position.y = 0x278000;
    SStack_34.priority = 0;
    SStack_34.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    SStack_34.heapID = param_3;
    sprite = Sprite_Create(&SStack_34);
    param_1[0xd] = (u8 *)(sprite);
    Sprite_SetDrawFlag(sprite, 0);
    NARC_Delete(narc);
    return;
}
