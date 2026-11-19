#include "global.h"

#include "error_handling.h"
#include "overlay_85.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "overlay_85_ov109_021D5140.h"

void ov85_021E8F88(UnkStruct_ov109_021D5140 *arg0, NARC *arg1) {
    NARC *narc;
    SpriteList *spriteList;
    GfGfxResType type;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    void *v0;
#endif

    narc = arg1;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_102);
    spriteList = G2dRenderer_Init(0x1E, &arg0->unk_64, HEAP_ID_102);
    arg0->unk_60 = spriteList;
    if (spriteList == NULL) {
        GF_AssertFail();
    }
    G2dRenderer_SetSubSurfaceCoords(&arg0->unk_64, 0, 0x100000);
    type = GF_GFX_RES_TYPE_CHAR;
#if defined(SDK_PORT) || defined(NONMATCHING)
    do {
        arg0->unk_1F0[type] = Create2DGfxResObjMan(3, type, HEAP_ID_102);
        type += 1;
    } while (type < 4);
#else
    v0 = arg0;
    do {
        *(GF_2DGfxResMan **)((u8 *)v0 + 0x18C) = Create2DGfxResObjMan(3, type, HEAP_ID_102);
        type += 1;
        v0 += 4;
    } while (type < 4);
#endif
    arg0->unk_200[2][0] = AddCharResObjFromOpenNarc(arg0->unk_1F0[0], narc, 0xC, 1, 2, 1, HEAP_ID_102);
    arg0->unk_200[2][1] = AddPlttResObjFromOpenNarc(arg0->unk_1F0[1], narc, 1, 0, 2, 1, 0xF, HEAP_ID_102);
    arg0->unk_200[2][2] = AddCellOrAnimResObjFromOpenNarc(arg0->unk_1F0[2], narc, 0xD, 1, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_102);
    arg0->unk_200[2][3] = AddCellOrAnimResObjFromOpenNarc(arg0->unk_1F0[3], narc, 0xE, 1, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_102);
    SpriteTransfer_CreateCharTransferTask(arg0->unk_200[2][0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->unk_200[2][1]);
}
