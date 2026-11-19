#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"

void ov74_0222A7A0(BgConfig *arg0);

void ov74_0222A7A0(BgConfig *bgConfig) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_8_OFFSET, 0x20U, HEAP_ID_84);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, bgConfig, GF_BG_LYR_SUB_1, 0U, 0x1400U, 1, HEAP_ID_84);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 2, bgConfig, GF_BG_LYR_SUB_1, 0U, 0x600U, 1, HEAP_ID_84);
    BgTilemapRectChangePalette(bgConfig, 5U, 0U, 0U, 0x20U, 0x18U, 8U);
    BgCommitTilemapBufferToVram(bgConfig, 5U);
}

void ov74_0222A81C(BgConfig *arg0);

void ov74_0222A81C(BgConfig *bgConfig) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_8_OFFSET, 0x20U, HEAP_ID_84);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0x1400U, 1, HEAP_ID_84);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 2, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0x600U, 1, HEAP_ID_84);
    BgTilemapRectChangePalette(bgConfig, 1U, 0U, 0U, 0x20U, 0x18U, 8U);
    BgCommitTilemapBufferToVram(bgConfig, 1U);
    ov74_0222A7A0(bgConfig);
}
