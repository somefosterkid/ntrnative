#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_67_app.h"

void ov67_021E5E84(Ov67App *app) {
    FreeBgTilemapBuffer(app->bgConfig, 5U);
    FreeBgTilemapBuffer(app->bgConfig, 4U);
    FreeBgTilemapBuffer(app->bgConfig, 1U);
    FreeBgTilemapBuffer(app->bgConfig, 0U);
    Heap_Free(app->bgConfig);
}

void ov67_021E5EB0(Ov67App *app) {
    NARC *narc;
    NARC *nARC;
    NARC *v0;

    if (app->isRankingsScreen == 0) {
        narc = NARC_New(NARC_a_1_1_8, app->heapID);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, app->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, app->heapID);
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 7, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, app->heapID);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, app->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, app->heapID);
        NARC_Delete(narc);
        nARC = NARC_New(NARC_a_1_8_3, app->heapID);
        GfGfxLoader_LoadCharDataFromOpenNarc(nARC, 0x8E, app->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, app->heapID);
        GfGfxLoader_GXLoadPalFromOpenNarc(nARC, 0xC4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, app->heapID);
        GfGfxLoader_LoadScrnDataFromOpenNarc(nARC, 0x8D, app->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, app->heapID);
        NARC_Delete(nARC);
        return;
    }
    v0 = NARC_New(NARC_a_1_1_8, app->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(v0, 4, app->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, app->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(v0, 7, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, app->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(v0, 5, app->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, app->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(v0, 8, app->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, app->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(v0, 0xA, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, app->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(v0, 9, app->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, app->heapID);
    NARC_Delete(v0);
}
