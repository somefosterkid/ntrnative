#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "overlay_109_photo_album_app.h"

void ov109_021E6AFC(PhotoAlbumApp *app) {
    BgSetPosTextAndCommit(app->bgConfig, 7U, BG_POS_OP_SET_ROT, 0);
    FreeBgTilemapBuffer(app->bgConfig, 3U);
    FreeBgTilemapBuffer(app->bgConfig, 2U);
    FreeBgTilemapBuffer(app->bgConfig, 1U);
    FreeBgTilemapBuffer(app->bgConfig, 0U);
    FreeBgTilemapBuffer(app->bgConfig, 6U);
    FreeBgTilemapBuffer(app->bgConfig, 7U);
    FreeBgTilemapBuffer(app->bgConfig, 4U);
    Heap_Free(app->bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}

void ov109_021E6B5C(PhotoAlbumApp *app) {
    NARC *narc;

    narc = NARC_New(NARC_a_1_7_1, app->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, app->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, app->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 9, app->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, app->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, app->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 0, app->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xA, app->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, app->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, app->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 0, app->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, app->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 0, app->heapID);
    app->scrn[0].buf = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 8, 0, &app->scrn[0].data, app->heapID);
    app->scrn[1].buf = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 8, 0, &app->scrn[1].data, app->heapID);
    app->scrn[2].buf = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 7, 0, &app->scrn[2].data, app->heapID);
    NARC_Delete(narc);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, app->heapID);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_FIRST, 1U, 0xDU, (u8)(s32)app->frame, app->heapID);
}

void ov109_021E6C7C(PhotoAlbumApp *app) {
    Heap_Free(app->scrn[2].buf);
    Heap_Free(app->scrn[1].buf);
    Heap_Free(app->scrn[0].buf);
}

void ov109_021E6C9C(PhotoAlbumApp *app) {
    s32 v0;
    s32 v1;
    s32 value;
    String **v3;

    FontID_Alloc(FONT_UNK_4, app->heapID);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0, app->heapID);
    app->msgFmt = MessageFormat_New_Custom(6U, 0x16U, app->heapID);
    app->tmpString = String_New(0x80U, app->heapID);
    app->nameString = NewString_ReadMsgData(app->msgData, 0);
    v0 = 0;
    do {
        app->msgStrings[v0] = NewString_ReadMsgData(app->msgData, v0 + 5);
        v0 += 1;
    } while (v0 < 5);
    v1 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    v3 = app->photoStrings;
    do {
        *v3 = NewString_ReadMsgData(app->msgData, v1 + 0xA);
        v1 += 1;
        v3 += 1;
    } while (v1 < 2);
#else
    v3 = (String **)app;
    do {
        *(String **)((u8 *)v3 + offsetof(PhotoAlbumApp, photoStrings)) = NewString_ReadMsgData(app->msgData, v1 + 0xA);
        v1 += 1;
        v3 += 1;
    } while (v1 < 2);
#endif
    app->listMenuItems = ListMenuItems_New(4U, app->heapID);
    value = 0;
    do {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, value + 1, value);
        value += 1;
    } while (value < 4);
}
