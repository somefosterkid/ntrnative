#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "error_handling.h"
#include "filesystem_files_def.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "overlay_108_ov108sprite_manager.h"

typedef struct UnkStruct_ov108_021E8758_pvVar5 {
    u8 unk_0[0x4];
    u8 unk_4;
    u8 unk_5[0x147];
    GF_2DGfxResMan *unk_14C;
    u8 unk_150[0xC];
    u32 *unk_15C;
} UnkStruct_ov108_021E8758_pvVar5;

s32 ov108_021E8758(void *param_1);

s32 ov108_021E8758(void *param_1) {
    SpriteResource *pSVar1;
    BOOL BVar2;
    s32 iVar3;
    s32 *piVar4;
    UnkStruct_ov108_021E8758_pvVar5 *pvVar5;
    s32 iVar6;
    u32 *puVar7;

    iVar6 = 0;
    piVar4 = ((s32 *)((Ov108SpriteManager *)param_1)->resList[0]);
    iVar3 = 0;
    do {
        pSVar1 = AddCharResObjFromNarc(((Ov108SpriteManager *)param_1)->resMan[0], NARC_a_1_6_6, iVar6 + 0x12, 0, iVar6 + 0xe000, (u32) * (u16 *)((u8 *)param_1 + 0xe), *(enum HeapID *)param_1);
        *(SpriteResource **)(*piVar4 + iVar3) = pSVar1;
        if (*(s32 *)(*piVar4 + iVar3) == 0) {
            GF_AssertFail();
        }
        switch (*(s32 *)((u8 *)param_1 + 4)) {
        case 0:
            SpriteTransfer_CreateCharTransferTask_AllocAtEnd(*(SpriteResource **)(*piVar4 + iVar3));
            break;
        case 1:
            SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(*(SpriteResource **)(*piVar4 + iVar3));
            break;
        case 2:
        default:
            SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW(*(SpriteResource **)(*piVar4 + iVar3));
            break;
        }
        iVar6 = iVar6 + 1;
        iVar3 = iVar3 + 4;
    } while (iVar6 < 0xc);
    iVar3 = 0;
    pvVar5 = param_1;
    do {
        puVar7 = pvVar5->unk_15C;
        pSVar1 = AddCellOrAnimResObjFromNarc(pvVar5->unk_14C, NARC_a_1_6_6, iVar3 + 0x10, 0, 0xe000, (GfGfxResType)(iVar3 + GF_GFX_RES_TYPE_CELL), *(enum HeapID *)param_1);
        *(SpriteResource **)*puVar7 = pSVar1;
        if (*(s32 *)*puVar7 == 0) {
            GF_AssertFail();
        }
        iVar3 = iVar3 + 1;
        pvVar5 = (void *)((u8 *)&pvVar5->unk_4);
    } while (iVar3 < 2);
    puVar7 = ((u32 *)((Ov108SpriteManager *)param_1)->resList[1]);
    pSVar1 = AddPlttResObjFromNarc(((Ov108SpriteManager *)param_1)->resMan[1], NARC_a_1_6_6, 0xf, 0, 0xe000, (u32) * (u16 *)((u8 *)param_1 + 0xe), 4, *(enum HeapID *)param_1);
    *(SpriteResource **)*puVar7 = pSVar1;
    if (*(s32 *)*puVar7 == 0) {
        GF_AssertFail();
    }
    BVar2 = SpriteTransfer_CreatePlttTransferTask(*(SpriteResource **)*puVar7);
    return BVar2;
}
