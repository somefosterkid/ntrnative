#include "global.h"

#include "filesystem.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "sprite.h"
#include "overlay_72_wfc_connect.h"

void ov72_02238194(NintendoWfcConnect *arg0);

void ov72_022383DC(SpriteTemplate *arg0, NintendoWfcConnect *arg1, SpriteResourcesHeader *arg2, s32 arg3);

void ov72_02238194(NintendoWfcConnect *arg0) {
    NARC *narc;
    GF_2DGfxResMan *v0;
    NARC *nARC;
    s32 i;

    narc = NARC_New(NARC_a_2_3_7, HEAP_ID_67);
    nARC = NARC_New(NARC_a_2_3_6, HEAP_ID_67);
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_67);
    arg0->spriteList = G2dRenderer_Init(0x14, &arg0->renderer, HEAP_ID_67);
    G2dRenderer_SetSubSurfaceCoords(&arg0->renderer, 0, 0x100000);
    i = GF_GFX_RES_TYPE_CHAR;
    do {
        v0 = Create2DGfxResObjMan(3, (GfGfxResType)i, HEAP_ID_67);
        arg0->resMan[i] = v0;
        i += 1;
    } while (i < 4);
    arg0->resObjs[0] = AddCharResObjFromOpenNarc(arg0->resMan[0], nARC, 1, 0, 0, 2, HEAP_ID_67);
    arg0->resObjs[1] = AddPlttResObjFromOpenNarc(arg0->resMan[1], nARC, 0, 0, 0, 2, 3, HEAP_ID_67);
    arg0->resObjs[2] = AddCellOrAnimResObjFromOpenNarc(arg0->resMan[2], nARC, 2, 0, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_67);
    arg0->resObjs[3] = AddCellOrAnimResObjFromOpenNarc(arg0->resMan[3], nARC, 3, 0, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_67);
    arg0->resObjs[4] = AddCharResObjFromOpenNarc(arg0->resMan[0], nARC, 5, 0, 1, 2, HEAP_ID_67);
    arg0->resObjs[5] = AddPlttResObjFromOpenNarc(arg0->resMan[1], nARC, 4, 0, 1, 2, 3, HEAP_ID_67);
    arg0->resObjs[6] = AddCellOrAnimResObjFromOpenNarc(arg0->resMan[2], nARC, 6, 0, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_67);
    arg0->resObjs[7] = AddCellOrAnimResObjFromOpenNarc(arg0->resMan[3], nARC, 7, 0, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_67);
    arg0->resObjs[8] = AddCharResObjFromOpenNarc(arg0->resMan[0], narc, 0xC, 0, 2, 2, HEAP_ID_67);
    arg0->resObjs[9] = AddPlttResObjFromOpenNarc(arg0->resMan[1], narc, 0xB, 0, 2, 2, 1, HEAP_ID_67);
    arg0->resObjs[10] = AddCellOrAnimResObjFromOpenNarc(arg0->resMan[2], narc, 0xD, 0, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_67);
    arg0->resObjs[11] = AddCellOrAnimResObjFromOpenNarc(arg0->resMan[3], narc, 0xE, 0, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_67);
    SpriteTransfer_CreateCharTransferTask(arg0->resObjs[0]);
    SpriteTransfer_CreateCharTransferTask(arg0->resObjs[4]);
    SpriteTransfer_CreateCharTransferTask(arg0->resObjs[8]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->resObjs[1]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->resObjs[5]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->resObjs[9]);
    NARC_Delete(nARC);
    NARC_Delete(narc);
}

void ov72_022383DC(SpriteTemplate *arg0, NintendoWfcConnect *arg1, SpriteResourcesHeader *arg2, s32 arg3) {
    arg0->spriteList = arg1->spriteList;
    arg0->header = arg2;
    *((s32 *)&((SpriteTemplate *)arg0)->position.z) = 0;
    *((s32 *)&((SpriteTemplate *)arg0)->scale.x) = 0x1000;
    *((s32 *)&((SpriteTemplate *)arg0)->scale.y) = 0x1000;
    *((s32 *)&((SpriteTemplate *)arg0)->scale.z) = 0x1000;
    *((s16 *)&((SpriteTemplate *)arg0)->rotation) = 0;
    *((s32 *)&((SpriteTemplate *)arg0)->drawPriority) = 1;
    *((s32 *)&((SpriteTemplate *)arg0)->whichScreen) = arg3;
    *((s32 *)&((SpriteTemplate *)arg0)->heapID) = 0x43;
}
