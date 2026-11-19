#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "msgdata.h"
#include "overlay_28.h"
#include "pm_string.h"

void ov28_0225D6E0(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
}

void ov28_0225D6FC(DowsingMchnApp *app, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, app->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, app->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 1, app->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_8);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_8);
}

void ov28_0225D764(DowsingMchnApp *app) {
    MsgData *msgData;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xC4, HEAP_ID_8);
    app->strings[1] = NewString_ReadMsgData(msgData, 0x11);
    app->strings[0] = NewString_ReadMsgData(msgData, 0x12);
    app->strings[2] = NewString_ReadMsgData(msgData, 0x16);
    app->strings[3] = NewString_ReadMsgData(msgData, 0x17);
    app->strings[4] = NewString_ReadMsgData(msgData, 0x15);
    DestroyMsgData(msgData);
}

void ov28_0225D7C4(DowsingMchnApp *app) {
    u32 v0;

    v0 = 0;
    do {
        String_Delete(app->strings[v0]);
        v0 += 1;
    } while (v0 < 5U);
}
