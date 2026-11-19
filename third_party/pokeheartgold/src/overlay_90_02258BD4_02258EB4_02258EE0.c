#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "error_handling.h"
#include "filesystem.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"

void ov90_02258EE0(UnkStruct_ov114_0225CCD0 *param0, UnkStruct_ov114_0225CDB4 *param1, NARC *param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9);

void ov90_02258EE0(UnkStruct_ov114_0225CCD0 *param0, UnkStruct_ov114_0225CDB4 *param1, NARC *param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9) {
    u32 fileId;
    NARC *narc;

    narc = param2;
    fileId = param3;
    if (param1->unk_00 != 0) {
        GF_AssertFail();
    }
    param1->unk_00 = 1;
    param1->unk_04[0] = AddCharResObjFromOpenNarc((GF_2DGfxResMan *)param0->unk_00[0], narc, (s32)param5, 0, (s32)param8, 1, (enum HeapID)param9);
    param1->unk_04[1] = AddPlttResObjFromOpenNarc((GF_2DGfxResMan *)param0->unk_00[1], narc, (s32)fileId, 0, (s32)param8, 1, (s32)param4, (enum HeapID)param9);
    param1->unk_04[2] = AddCellOrAnimResObjFromOpenNarc((GF_2DGfxResMan *)param0->unk_00[2], narc, (s32)param6, 0, (s32)param8, GF_GFX_RES_TYPE_CELL, (enum HeapID)param9);
    param1->unk_04[3] = AddCellOrAnimResObjFromOpenNarc((GF_2DGfxResMan *)param0->unk_00[3], narc, (s32)param7, 0, (s32)param8, GF_GFX_RES_TYPE_ANIM, (enum HeapID)param9);
    if (SpriteTransfer_CreateCharTransferTask_AllocAtEnd(param1->unk_04[0]) == 0) {
        GF_AssertFail();
    }
    sub_0200A740(param1->unk_04[0]);
    if (SpriteTransfer_CreatePlttTransferTask(param1->unk_04[1]) == 0) {
        GF_AssertFail();
    }
    sub_0200A740(param1->unk_04[1]);
    CreateSpriteResourcesHeader(&param1->unk_14, (s32)param8, (s32)param8, (s32)param8, (s32)param8, -1, -1, 0, 1, (GF_2DGfxResMan *)param0->unk_00[0], (GF_2DGfxResMan *)param0->unk_00[1], (GF_2DGfxResMan *)param0->unk_00[2], (GF_2DGfxResMan *)param0->unk_00[3], NULL, NULL);
}
