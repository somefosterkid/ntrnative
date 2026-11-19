#include "global.h"

#include "error_handling.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"

void ov91_0225FB80(void *arg0, NARC *arg1, enum HeapID arg2);

void ov91_0225FB80(void *arg0, NARC *narc, enum HeapID heapID) {
    ((UnkStruct_ov115_02261ADC *)arg0)->unk_1E0[0] = AddCharResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)arg0)->unk_1AC[0]), narc, 2, 0, 0xC8, 1, heapID);
    ((UnkStruct_ov115_02261ADC *)arg0)->unk_1E0[1] = AddPlttResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)arg0)->unk_1AC[1]), narc, 3, 0, 0xC8, 1, 8, heapID);
    ((UnkStruct_ov115_02261ADC *)arg0)->unk_1E0[2] = AddCellOrAnimResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)arg0)->unk_1AC[2]), narc, 1, 0, 0xC8, GF_GFX_RES_TYPE_CELL, heapID);
    ((UnkStruct_ov115_02261ADC *)arg0)->unk_1E0[3] = AddCellOrAnimResObjFromOpenNarc(((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)arg0)->unk_1AC[3]), narc, 0, 0, 0xC8, GF_GFX_RES_TYPE_ANIM, heapID);
    if (SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(((UnkStruct_ov115_02261ADC *)arg0)->unk_1E0[0]) == 0) {
        GF_AssertFail();
    }
    if (SpriteTransfer_CreatePlttTransferTask(((UnkStruct_ov115_02261ADC *)arg0)->unk_1E0[1]) == 0) {
        GF_AssertFail();
    }
    sub_0200A740(((UnkStruct_ov115_02261ADC *)arg0)->unk_1E0[0]);
    sub_0200A740(((UnkStruct_ov115_02261ADC *)arg0)->unk_1E0[1]);
    CreateSpriteResourcesHeader(arg0 + 0x158, 0xC8, 0xC8, 0xC8, 0xC8, -1, -1, 0, 0, ((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)arg0)->unk_1AC[0]), ((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)arg0)->unk_1AC[1]), ((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)arg0)->unk_1AC[2]), ((GF_2DGfxResMan *)((UnkStruct_ov115_02261ADC *)arg0)->unk_1AC[3]), NULL, NULL);
}
