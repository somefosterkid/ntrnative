#include "global.h"
#include "gf_gfx_planes.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_97_course_sub_screen.h"

void ov97_0221F7DC(Ov97CourseSubScreenSprites *arg0);

void ov97_0221F7DC(Ov97CourseSubScreenSprites *arg0) {
    GfGfxResType type;
    s32 i;

    arg0->spriteList = G2dRenderer_Init(0x42, &arg0->renderer, arg0->heapID);
    G2dRenderer_SetSubSurfaceCoords(&arg0->renderer, 0, FX32_CONST(256));
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    type = GF_GFX_RES_TYPE_CHAR;
    i = 0;
    do {
        arg0->resMan[i] = Create2DGfxResObjMan(3, type, arg0->heapID);
        i += 1;
        type += 1;
    } while (type < 6);
    arg0->resObj[0][0] = AddCharResObjFromNarc(arg0->resMan[0], NARC_a_2_4_1, 7, 1, 1, 2, arg0->heapID);
    arg0->resObj[0][1] = AddPlttResObjFromNarc(arg0->resMan[1], NARC_a_2_4_1, 4, 0, 1, 2, 4, arg0->heapID);
    arg0->resObj[0][2] = AddCellOrAnimResObjFromNarc(arg0->resMan[2], NARC_a_2_4_1, 6, 1, 1, GF_GFX_RES_TYPE_CELL, arg0->heapID);
    arg0->resObj[0][3] = AddCellOrAnimResObjFromNarc(arg0->resMan[3], NARC_a_2_4_1, 5, 1, 1, GF_GFX_RES_TYPE_ANIM, arg0->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(arg0->resObj[0][0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->resObj[0][1]);
    arg0->resObj[1][0] = AddCharResObjFromNarc(arg0->resMan[0], NARC_a_1_6_2, 0x42, 0, 2, 2, arg0->heapID);
    arg0->resObj[1][1] = AddPlttResObjFromNarc(arg0->resMan[1], NARC_a_1_6_2, 0x41, 0, 2, 2, 4, arg0->heapID);
    arg0->resObj[1][2] = AddCellOrAnimResObjFromNarc(arg0->resMan[2], NARC_a_1_6_2, 0x43, 0, 2, GF_GFX_RES_TYPE_CELL, arg0->heapID);
    arg0->resObj[1][3] = AddCellOrAnimResObjFromNarc(arg0->resMan[3], NARC_a_1_6_2, 0x44, 0, 2, GF_GFX_RES_TYPE_ANIM, arg0->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(arg0->resObj[1][0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->resObj[1][1]);
    arg0->resObj[2][0] = AddCharResObjFromNarc(arg0->resMan[0], NARC_application_choose_starter_choose_starter_sub_res, 9, 0, 3, 2, arg0->heapID);
    arg0->resObj[2][1] = AddPlttResObjFromNarc(arg0->resMan[1], NARC_application_choose_starter_choose_starter_sub_res, 6, 0, 3, 2, 4, arg0->heapID);
    arg0->resObj[2][2] = AddCellOrAnimResObjFromNarc(arg0->resMan[2], NARC_application_choose_starter_choose_starter_sub_res, 0xA, 0, 3, GF_GFX_RES_TYPE_CELL, arg0->heapID);
    arg0->resObj[2][3] = AddCellOrAnimResObjFromNarc(arg0->resMan[3], NARC_application_choose_starter_choose_starter_sub_res, 0xA, 0, 3, GF_GFX_RES_TYPE_ANIM, arg0->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(arg0->resObj[2][0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->resObj[2][1]);
}
