#include "global.h"
#include "overlay_28.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"

static void ov28_0225D910(DowsingMchnApp *app);

void ov28_0225D8D0(DowsingMchnApp *app) {
    ov28_0225DBFC(app);
    ov28_0225DB54(app);
    ov28_0225D910(app);
    SpriteList_Delete(app->spriteList);
}

void ov28_0225D8EC(DowsingMchnApp *app) {
    GfGfxResType type;
    s32 i;

    type = GF_GFX_RES_TYPE_CHAR;
    i = 0;
    do {
        app->resMans[i] = Create2DGfxResObjMan(2, type, HEAP_ID_8);
        type += 1;
        i += 1;
    } while ((u32)type < 4U);
}

static void ov28_0225D910(DowsingMchnApp *app) {
    u32 v0;

    v0 = 0;
    do {
        Destroy2DGfxResObjMan(app->resMans[v0]);
        v0 += 1;
    } while (v0 < 4U);
}

void ov28_0225D92C(SpriteResource **resObjs, GF_2DGfxResMan **resMans, NARC *narc, s32 fileId, s32 arg4, s32 arg5, s32 arg6, s32 pltt_num, s32 id, s32 arg9, s32 argA, s32 argB) {
    SpriteResource *charResObj;
    SpriteResource *plttResObj;

    charResObj = AddCharResObjFromOpenNarc(resMans[0], narc, fileId, 1, id, 2, HEAP_ID_8);
    resObjs[0] = charResObj;
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(charResObj);
    sub_0200A740(resObjs[0]);
    plttResObj = AddPlttResObjFromOpenNarc(resMans[1], narc, arg4, 0, arg9, 2, pltt_num, HEAP_ID_8);
    resObjs[1] = plttResObj;
    SpriteTransfer_CreatePlttTransferTask(plttResObj);
    sub_0200A740(resObjs[1]);
    resObjs[2] = AddCellOrAnimResObjFromOpenNarc(resMans[2], narc, arg5, 1, argA, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    resObjs[3] = AddCellOrAnimResObjFromOpenNarc(resMans[3], narc, arg6, 1, argB, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
}
