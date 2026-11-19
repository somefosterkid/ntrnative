#include "global.h"

#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "overlay_112_sub_screen.h"

void ov112_021F06CC(Ov112SubScreen *, void *);
void ov112_021F0908(Ov112SubScreen *, u8 *);
void ov112_021F0980(Ov112SubScreen *);
void ov112_021F04DC(Ov112SubScreen *arg0, void *arg1);
void ov112_021F050C(Ov112SubScreen *arg0);
void ov112_021F05CC(Ov112SubScreenSprites *arg0);
void ov112_021F051C(Ov112SubScreen *arg0);
s32 ov112_021F0668(BgConfig *arg0);
void ov112_021F05FC(BgConfig *arg0, enum HeapID arg1);

void ov112_021F04DC(Ov112SubScreen *arg0, void *arg1) {
    if (*(u8 *)((u8 *)arg1 + 9) == 2) {
        ov112_021F0908(arg0, arg1);
        ov112_021F06CC(arg0, arg1);
        GfGfx_EngineBTogglePlanes(2U, 1U);
        return;
    }
    ov112_021F0980(arg0);
    GfGfx_EngineBTogglePlanes(2U, 0U);
}

void ov112_021F050C(Ov112SubScreen *arg0) {
    SpriteList *spriteList;

    spriteList = arg0->sprites.spriteList;
    if (spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(spriteList);
    }
}

void ov112_021F051C(Ov112SubScreen *ptr) {
    u8 v2;
    u8 v3;
    Ov112SubScreenSprites *v0;

    DestroyMsgData(ptr->msgData);
    MessageFormat_Delete(ptr->msgFormat);
    String_Delete(ptr->string);
    RemoveWindow(&ptr->windows[0]);
    RemoveWindow(&ptr->windows[1]);
    RemoveWindow(&ptr->windows[2]);
    RemoveWindow(&ptr->windows[3]);
    RemoveWindow(&ptr->windows[4]);
    RemoveWindow(&ptr->windows[5]);
    FreeBgTilemapBuffer(ptr->bgConfig, 5U);
    FreeBgTilemapBuffer(ptr->bgConfig, 7U);
    v0 = &ptr->sprites;
    ov112_021F05CC(v0);
    v2 = 0;
    do {
        SpriteTransfer_DeleteCharTransferTask(v0->resObj[v2][0]);
        SpriteTransfer_DeletePlttTransferTask(v0->resObj[v2][1]);
        v2 += 1;
    } while ((u32)v2 < 2U);
    v3 = 0;
    do {
        Destroy2DGfxResObjMan(v0->resMan[v3]);
        v3 += 1;
    } while ((u32)v3 < 6U);
    SpriteList_Delete(v0->spriteList);
    Heap_Free(ptr);
}

void ov112_021F05CC(Ov112SubScreenSprites *arg0) {
    s32 v0;

    v0 = 0;
    do {
        Sprite_Delete(arg0->slotSprites[v0]);
        v0 += 1;
    } while (v0 < 6);
    Sprite_Delete(arg0->monSprite);
    Heap_Free(arg0->charData);
}

void ov112_021F05FC(BgConfig *bgConfig, enum HeapID heapID) {
    ov112_021F0668(bgConfig);
    GfGfxLoader_GXLoadPal(NARC_a_2_5_7, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, heapID);
    GfGfxLoader_LoadCharData(NARC_a_2_5_7, 1, bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_2_5_7, 2, bgConfig, GF_BG_LYR_SUB_3, 0U, 0x600U, 1, heapID);
    BgCommitTilemapBufferToVram(bgConfig, 7U);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, heapID);
}
