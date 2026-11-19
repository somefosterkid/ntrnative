#include "global.h"

#include "bg_window.h"
#include "overlay_27.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

u8 ov01_021EEF60(struct FieldMenuManager *menu);

void ov27_0225C6F8(Ov27TaskData *data) {
    s32 iVar1;
    s32 iVar2;
    Window *window;

    iVar1 = ov01_021EEF60(data->menu);
    BgClearTilemapBufferAndCommit(data->bgConfig, 4);
    BgClearTilemapBufferAndCommit(data->bgConfig, 5);
    iVar2 = 0;
    if (0 < iVar1) {
        window = data->menuWindows;
        do {
            RemoveWindow(window);
            iVar2 = iVar2 + 1;
            window = window + 1;
        } while (iVar2 < iVar1);
    }
    return;
}

void ov27_0225C72C(Ov27TaskData *data) {
    GfGfxResType type;
    void **v0;

    data->spriteList = G2dRenderer_Init(0x15, &data->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&data->renderer, 0, 0x100000);
    type = GF_GFX_RES_TYPE_CHAR;
#if defined(SDK_PORT) || defined(NONMATCHING)
    do {
        data->resMan[type] = Create2DGfxResObjMan(0xA, type, HEAP_ID_8);
        type += 1;
    } while (type < 4);
#else
    v0 = (void **)data;
    do {
        *(GF_2DGfxResMan **)((u8 *)v0 + offsetof(Ov27TaskData, resMan)) = Create2DGfxResObjMan(0xA, type, HEAP_ID_8);
        type += 1;
        v0 += 1;
    } while (type < 4);
#endif
    data->resObj[0] = AddCharResObjFromNarc(data->resMan[0], NARC_a_2_3_7, 0xC, 0, 0xA, 2, HEAP_ID_8);
    data->resObj[1] = AddPlttResObjFromNarc(data->resMan[1], NARC_a_2_3_7, 0xB, 0, 0xA, 2, 1, HEAP_ID_8);
    data->resObj[2] = AddCellOrAnimResObjFromNarc(data->resMan[2], NARC_a_2_3_7, 0xD, 0, 0xA, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    data->resObj[3] = AddCellOrAnimResObjFromNarc(data->resMan[3], NARC_a_2_3_7, 0xE, 0, 0xA, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(data->resObj[0]);
    SpriteTransfer_CreatePlttTransferTask(data->resObj[1]);
}
