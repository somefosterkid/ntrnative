#include "global.h"

#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

void ov73_021E6090(UnkStruct_020961E8 *arg0, NARC *arg1);

void ov73_021E6090(UnkStruct_020961E8 *arg0, NARC *arg1) {
    NARC *narc;
    GfGfxResType type;
    s32 i;

    narc = arg1;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_50);
    arg0->unk_50 = G2dRenderer_Init(0x1E, &arg0->unk_54, HEAP_ID_50);
    G2dRenderer_SetSubSurfaceCoords(&arg0->unk_54, 0, 0x100000);
    type = GF_GFX_RES_TYPE_CHAR;
    i = 0;
    do {
        arg0->unk_1E0[i] = Create2DGfxResObjMan(3, type, HEAP_ID_50);
        type += 1;
        i += 1;
    } while (type < 4);
    arg0->unk_1F0[2][0] = AddCharResObjFromOpenNarc(arg0->unk_1E0[0], narc, 8, 1, 2, 1, HEAP_ID_50);
    arg0->unk_1F0[2][1] = AddPlttResObjFromOpenNarc(arg0->unk_1E0[1], narc, 7, 0, 2, 1, 0xF, HEAP_ID_50);
    arg0->unk_1F0[2][2] = AddCellOrAnimResObjFromOpenNarc(arg0->unk_1E0[2], narc, 9, 1, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_50);
    arg0->unk_1F0[2][3] = AddCellOrAnimResObjFromOpenNarc(arg0->unk_1E0[3], narc, 0xA, 1, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_50);
    SpriteTransfer_CreateCharTransferTask(arg0->unk_1F0[2][0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->unk_1F0[2][1]);
}
