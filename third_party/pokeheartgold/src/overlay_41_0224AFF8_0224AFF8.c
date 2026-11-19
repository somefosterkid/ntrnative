#include "global.h"

#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"

void ov41_0224AFF8(void *arg0, void *arg1, enum HeapID arg2, NARC *arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);

void ov41_0224AFF8(void *arg0, void *arg1, enum HeapID heapID, NARC *narc, s32 fileId, s32 arg5, s32 arg6, s32 arg7, s32 pltt_num, s32 arg9) {
    NARC *nARC;
    SpriteResource *charResObj;
    SpriteResource *plttResObj;

    nARC = narc;
    charResObj = AddCharResObjFromOpenNarc(*(GF_2DGfxResMan **)arg1, narc, fileId, 0, arg9 + fileId, 2, heapID);
    *(SpriteResource **)arg0 = charResObj;
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(charResObj);
    sub_0200A740(*(SpriteResource **)arg0);
    plttResObj = AddPlttResObjFromOpenNarc((((SpriteResourceCollection *)arg1)->resources), nARC, arg5, 0, arg9 + arg5, 2, pltt_num, heapID);
    ((UnkStruct_ov22_0225AB54 *)arg0)->unk_00[1] = plttResObj;
    SpriteTransfer_CreatePlttTransferTask(plttResObj);
    sub_0200A740(((UnkStruct_ov22_0225AB54 *)arg0)->unk_00[1]);
    ((UnkStruct_ov22_0225AB54 *)arg0)->unk_00[2] = AddCellOrAnimResObjFromOpenNarc(*((GF_2DGfxResMan **)&((SpriteResourceCollection *)arg1)->capacity), nARC, arg6, 0, arg9 + arg6, GF_GFX_RES_TYPE_CELL, heapID);
    ((UnkStruct_ov22_0225AB54 *)arg0)->unk_00[3] = AddCellOrAnimResObjFromOpenNarc(*((GF_2DGfxResMan **)&((SpriteResourceCollection *)arg1)->count), nARC, arg7, 0, arg9 + arg7, GF_GFX_RES_TYPE_ANIM, heapID);
}
