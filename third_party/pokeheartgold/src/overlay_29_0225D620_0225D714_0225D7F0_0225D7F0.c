#include "global.h"

#include "overlay_29.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_29_number_entry_data.h"

static void ov29_0225D840(NumberEntryData *data);
static void ov29_0225D864(NumberEntryData *data);

s32 ov29_0225D9C8(NumberEntryData *, void *);

s32 ov29_0225DC84(void *);

void ov29_0225D7F0(NumberEntryData *arg0, void *arg1);

void ov29_0225D7F0(NumberEntryData *arg0, void *arg1) {
    arg0->spriteList = G2dRenderer_Init(0xB, &arg0->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&arg0->renderer, 0, 0x100000);
    ov29_0225D840(arg0);
    ov29_0225D9C8(arg0, arg1);
    ov29_0225DC84(arg0);
}

s32 ov29_0225DB38(void *arg0);

void ov29_0225D828(NumberEntryData *arg0);

void ov29_0225D828(NumberEntryData *arg0) {
    ov29_0225DB38(arg0);
    ov29_0225D864(arg0);
    SpriteList_Delete(arg0->spriteList);
}

static void ov29_0225D840(NumberEntryData *data) {
    GfGfxResType type;
#if defined(SDK_PORT) || defined(NONMATCHING)
    type = GF_GFX_RES_TYPE_CHAR;
    do {
        data->resMans[type] = Create2DGfxResObjMan(2, type, HEAP_ID_8);
        type += 1;
    } while ((u32)type < 4U);
#else
    u8 *v0;

    v0 = (u8 *)data;
    type = GF_GFX_RES_TYPE_CHAR;
    do {
        *(GF_2DGfxResMan **)(v0 + offsetof(NumberEntryData, resMans)) = Create2DGfxResObjMan(2, type, HEAP_ID_8);
        type += 1;
        v0 += sizeof(GF_2DGfxResMan *);
    } while ((u32)type < 4U);
#endif
}

static void ov29_0225D864(NumberEntryData *data) {
    u32 v0;

    v0 = 0;
    do {
        Destroy2DGfxResObjMan(data->resMans[v0]);
        v0 += 1;
    } while (v0 < 4U);
}

void ov29_0225D880(SpriteResource **resObjs, GF_2DGfxResMan **resMans, NARC *narc, s32 fileId, s32 arg4, s32 arg5, s32 arg6, s32 pltt_num, s32 id, s32 arg9, s32 argA, s32 argB) {
    SpriteResource *charResObj;
    SpriteResource *plttResObj;

    charResObj = AddCharResObjFromOpenNarc(resMans[0], narc, fileId, 0, id, 2, HEAP_ID_8);
    resObjs[0] = charResObj;
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(charResObj);
    sub_0200A740(resObjs[0]);
    plttResObj = AddPlttResObjFromOpenNarc(resMans[1], narc, arg4, 0, arg9, 2, pltt_num, HEAP_ID_8);
    resObjs[1] = plttResObj;
    SpriteTransfer_CreatePlttTransferTask(plttResObj);
    sub_0200A740(resObjs[1]);
    resObjs[2] = AddCellOrAnimResObjFromOpenNarc(resMans[2], narc, arg5, 0, argA, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    resObjs[3] = AddCellOrAnimResObjFromOpenNarc(resMans[3], narc, arg6, 0, argB, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
}
