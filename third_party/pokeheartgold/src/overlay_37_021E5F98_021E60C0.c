#include "global.h"

#include "unk_02095EAC.h"

#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"

void ov37_021E60C0(UnkStruct_02095EAC *arg0, NARC *arg1);

void ov37_021E60C0(UnkStruct_02095EAC *arg0, NARC *narc) {
    GfGfxResType type;
    void *v0;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_39);
    arg0->unk_34 = G2dRenderer_Init(0x35, &arg0->unk_38, HEAP_ID_39);
    G2dRenderer_SetSubSurfaceCoords(&arg0->unk_38, 0, 0x100000);
    type = GF_GFX_RES_TYPE_CHAR;
    v0 = arg0;
    do {
        *(GF_2DGfxResMan **)((u8 *)v0 + 0x160) = Create2DGfxResObjMan(2, type, HEAP_ID_39); /* TODO(hk205): walker over unk_1C4; typed pointer/index forms change the code */
        v0 += 4;
        type += 1;
    } while (type < 4);
    arg0->unk_1D4[0][0] = AddCharResObjFromOpenNarc(arg0->unk_1C4[0], narc, 6, 1, 0, 1, HEAP_ID_39);
    arg0->unk_1D4[0][1] = AddPlttResObjFromOpenNarc(arg0->unk_1C4[1], narc, 0, 0, 0, 1, 7, HEAP_ID_39);
    arg0->unk_1D4[0][2] = AddCellOrAnimResObjFromOpenNarc(arg0->unk_1C4[2], narc, 7, 1, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_39);
    arg0->unk_1D4[0][3] = AddCellOrAnimResObjFromOpenNarc(arg0->unk_1C4[3], narc, 8, 1, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_39);
    arg0->unk_1D4[1][0] = AddCharResObjFromOpenNarc(arg0->unk_1C4[0], narc, 6, 1, 1, 2, HEAP_ID_39);
    arg0->unk_1D4[1][1] = AddPlttResObjFromOpenNarc(arg0->unk_1C4[1], narc, 0, 0, 1, 2, 3, HEAP_ID_39);
    arg0->unk_1D4[1][2] = AddCellOrAnimResObjFromOpenNarc(arg0->unk_1C4[2], narc, 7, 1, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_39);
    arg0->unk_1D4[1][3] = AddCellOrAnimResObjFromOpenNarc(arg0->unk_1C4[3], narc, 8, 1, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_39);
    SpriteTransfer_CreateCharTransferTask(arg0->unk_1D4[0][0]);
    SpriteTransfer_CreateCharTransferTask(arg0->unk_1D4[1][0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->unk_1D4[0][1]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->unk_1D4[1][1]);
}
