#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "error_handling.h"
#include "filesystem.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"

void ov91_022607C4(u8 *, u8 *, enum HeapID);

void sub_02018030(void *, NARC *, s32, enum HeapID);

extern u32 ov91_02261BF0;

void ov91_022604F4(u8 *param_1, NARC *param_2, enum HeapID param_3);

void ov91_022604F4(u8 *param_1, NARC *param_2, enum HeapID param_3) {
    SpriteResource *pSVar1;
    BOOL BVar2;
    u8 *puVar3;
    u8 *iVar4;
    s32 iVar5;
    s32 iStack_18;

    iStack_18 = 0;
    puVar3 = (unsigned char *)(&ov91_02261BF0);
    iVar4 = param_1 + 0x6fc;
    do {
        sub_02018030(iVar4, param_2, *puVar3, param_3);
        puVar3 = puVar3 + 1;
        iStack_18 = iStack_18 + 1;
        iVar4 = iVar4 + 0x10;
    } while (iStack_18 < 5);
    sub_02018030(param_1 + 0x74c, param_2, 0x2e, param_3);
    NNSi_G3dModifyPolygonAttrMask(((NNSG3dResMdl *)((UnkStruct_ov115_02261ADC *)param_1)->unk_760.unk_50.model), 0, 0x1f0000);
    pSVar1 = AddCharResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)param_1)->unk_1AC[0]), param_2, 7, 0, 100, 2, param_3);
    ((UnkStruct_ov115_02261ADC *)param_1)->unk_760.unk_60[0] = pSVar1;
    pSVar1 = AddPlttResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)param_1)->unk_1AC[1]), param_2, 4, 0, 100, 2, 4, param_3);
    ((UnkStruct_ov115_02261ADC *)param_1)->unk_760.unk_60[1] = pSVar1;
    pSVar1 = AddCellOrAnimResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)param_1)->unk_1AC[2]), param_2, 6, 0, 100, GF_GFX_RES_TYPE_CELL, param_3);
    ((UnkStruct_ov115_02261ADC *)param_1)->unk_760.unk_60[2] = pSVar1;
    pSVar1 = AddCellOrAnimResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)param_1)->unk_1AC[3]), param_2, 5, 0, 100, GF_GFX_RES_TYPE_ANIM, param_3);
    ((UnkStruct_ov115_02261ADC *)param_1)->unk_760.unk_60[3] = pSVar1;
    BVar2 = SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(((UnkStruct_ov115_02261ADC *)param_1)->unk_760.unk_60[0]);
    if (BVar2 == 0) {
        GF_AssertFail();
    }
    BVar2 = SpriteTransfer_CreatePlttTransferTask(((UnkStruct_ov115_02261ADC *)param_1)->unk_760.unk_60[1]);
    if (BVar2 == 0) {
        GF_AssertFail();
    }
    sub_0200A740(((UnkStruct_ov115_02261ADC *)param_1)->unk_760.unk_60[0]);
    sub_0200A740(((UnkStruct_ov115_02261ADC *)param_1)->unk_760.unk_60[1]);
    CreateSpriteResourcesHeader((SpriteResourcesHeader *)(param_1 + 0x76c), 100, 100, 100, 100, -1, -1, 0, 0, ((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)param_1)->unk_1AC[0]), ((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)param_1)->unk_1AC[1]), ((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)param_1)->unk_1AC[2]), ((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)param_1)->unk_1AC[3]), (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    iVar5 = 0;
    iVar4 = param_1 + 0x790;
    do {
        ov91_022607C4(param_1, iVar4, param_3);
        iVar4 = iVar4 + 0x108;
        iVar5 = iVar5 + 1;
    } while (iVar5 < 0x60);
    return;
}
