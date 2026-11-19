#include "global.h"
struct UnkStruct_ov22_0225A0E4;

#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

void ov41_022462E4(struct UnkStruct_ov22_0225A0E4 *arg0, NARC *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void ov41_022462E4(UnkStruct_ov22_0225A0E4 *arg0, NARC *narc, s32 fileId, s32 compressed, s32 vram, s32 id) {
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(AddCharResObjFromOpenNarc(*((GF_2DGfxResMan * *)&arg0->unk_48[0]), narc, fileId, compressed, id, vram, HEAP_ID_14));
}

void ov41_02246304(struct UnkStruct_ov22_0225A0E4 *arg0, NARC *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

void ov41_02246304(UnkStruct_ov22_0225A0E4 *arg0, NARC *narc, s32 fileId, s32 compressed, s32 vram, s32 pltt_num, s32 id) {
    SpriteTransfer_CreatePlttTransferTask(AddPlttResObjFromOpenNarc(*((GF_2DGfxResMan * *)&arg0->unk_48[1]), narc, fileId, compressed, id, vram, pltt_num, HEAP_ID_14));
}

void ov41_02246328(struct UnkStruct_ov22_0225A0E4 *arg0, NARC *arg1, s32 arg2, s32 arg3, s32 arg4);

void ov41_02246328(UnkStruct_ov22_0225A0E4 *arg0, NARC *narc, s32 fileId, s32 compressed, s32 id) {
    AddCellOrAnimResObjFromOpenNarc(*((GF_2DGfxResMan * *)&arg0->unk_48[2]), narc, fileId, compressed, id, GF_GFX_RES_TYPE_CELL, HEAP_ID_14);
}

void ov41_02246344(struct UnkStruct_ov22_0225A0E4 *arg0, NARC *arg1, s32 arg2, s32 arg3, s32 arg4);

void ov41_02246344(UnkStruct_ov22_0225A0E4 *arg0, NARC *narc, s32 fileId, s32 compressed, s32 id) {
    AddCellOrAnimResObjFromOpenNarc(*((GF_2DGfxResMan * *)&arg0->unk_48[3]), narc, fileId, compressed, id, GF_GFX_RES_TYPE_ANIM, HEAP_ID_14);
}
