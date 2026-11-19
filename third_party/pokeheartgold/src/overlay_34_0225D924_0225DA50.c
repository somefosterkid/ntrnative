#include "global.h"

#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_34.h"

void ov34_0225DA50(Ov34Work *work) {
    GfGfxResType type;
    s32 i;

    work->spriteList = G2dRenderer_Init(0xA, &work->renderer, HEAP_ID_FIELD1);
    type = GF_GFX_RES_TYPE_CHAR;
    i = 0;
    do {
        work->resMans[i] = Create2DGfxResObjMan(1, type, HEAP_ID_FIELD1);
        type += 1;
        i++;
    } while (type < 4);
    work->resObjs[0] = AddCharResObjFromNarc(work->resMans[0], NARC_a_0_7_3, 5, 1, 0x3E7, 2, HEAP_ID_FIELD1);
    work->resObjs[1] = AddPlttResObjFromNarc(work->resMans[1], NARC_a_0_7_3, 1, 0, 0x3E7, 2, 1, HEAP_ID_FIELD1);
    work->resObjs[2] = AddCellOrAnimResObjFromNarc(work->resMans[2], NARC_a_0_7_3, 6, 1, 0x3E7, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    work->resObjs[3] = AddCellOrAnimResObjFromNarc(work->resMans[3], NARC_a_0_7_3, 7, 1, 0x3E7, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(work->resObjs[0]);
    SpriteTransfer_CreatePlttTransferTask(work->resObjs[1]);
}
