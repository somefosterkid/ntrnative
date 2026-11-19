#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "overlay_72_wfc_connect.h"

void ov72_02238AEC(BgConfig *arg0);

void ov72_02238AEC(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
}

void ov72_02238B18(NintendoWfcConnect *arg0, s32 arg1);

void ov72_02238B18(NintendoWfcConnect *arg0, s32 arg1) {
    BgConfig *bgConfig;
    NARC *narc;
    NARC *nARC;

    bgConfig = arg0->bgConfig;
    if (arg1 == 0) {
        narc = NARC_New(NARC_a_0_8_8, HEAP_ID_67);
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x100U, HEAP_ID_67);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0xB, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_67);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xC, bgConfig, GF_BG_LYR_SUB_1, 0U, 0x600U, 0, HEAP_ID_67);
        arg0->paletteCycle.screens = 0;
        NARC_Delete(narc);
        return;
    }
    nARC = NARC_New(NARC_a_2_3_6, HEAP_ID_67);
    arg0->paletteCycle.screens = 1;
    GfGfxLoader_GXLoadPalFromOpenNarc(nARC, 8, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, HEAP_ID_67);
    GfGfxLoader_LoadCharDataFromOpenNarc(nARC, 9, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_67);
    GfGfxLoader_LoadScrnDataFromOpenNarc(nARC, 0xA, bgConfig, GF_BG_LYR_SUB_1, 0U, 0x600U, 0, HEAP_ID_67);
    NARC_Delete(nARC);
}
