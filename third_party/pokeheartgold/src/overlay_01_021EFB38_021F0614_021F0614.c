#include "global.h"

#include "overlay_01_021EFB38.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_01_unk_struct_ov5_021de47c.h"
#include "overlay_01_unk_struct_ov5_021de5a4.h"

void ov01_021F0614(NARC *narc, void *arg1, void *arg2, s32 arg3, s32 pltt_num, s32 fileId, s32 arg6, s32 arg7, s32 id) {
    s32 v0;

    v0 = arg3;
    *(SpriteResource **)arg2 = AddCharResObjFromOpenNarcWithAtEndFlag(((GF_2DGfxResMan *)((UnkStruct_ov5_021DE47C *)arg1)->unk_190[0]), narc, fileId, 0, id, 1, HEAP_ID_FIELD1, 1);
    ((UnkStruct_ov5_021DE5A4 *)arg2)->unk_00[1] = AddPlttResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov5_021DE47C *)arg1)->unk_190[1]), narc, v0, 0, id, 1, pltt_num, HEAP_ID_FIELD1);
    ((UnkStruct_ov5_021DE5A4 *)arg2)->unk_00[2] = AddCellOrAnimResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov5_021DE47C *)arg1)->unk_190[2]), narc, arg6, 0, id, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    ((UnkStruct_ov5_021DE5A4 *)arg2)->unk_00[3] = AddCellOrAnimResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov5_021DE47C *)arg1)->unk_190[3]), narc, arg7, 0, id, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(*(SpriteResource **)arg2);
    sub_0200A740(*(SpriteResource **)arg2);
    SpriteTransfer_CreatePlttTransferTask(((UnkStruct_ov5_021DE5A4 *)arg2)->unk_00[1]);
    CreateSpriteResourcesHeader(&((UnkStruct_ov5_021DE5A4 *)arg2)->unk_10, id, id, id, id, -1, -1, 0, 0, ((GF_2DGfxResMan *)((UnkStruct_ov5_021DE47C *)arg1)->unk_190[0]), ((GF_2DGfxResMan *)((UnkStruct_ov5_021DE47C *)arg1)->unk_190[1]), ((GF_2DGfxResMan *)((UnkStruct_ov5_021DE47C *)arg1)->unk_190[2]), ((GF_2DGfxResMan *)((UnkStruct_ov5_021DE47C *)arg1)->unk_190[3]), NULL, NULL);
}
