#include "global.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "overlay_74_wonder_cards_app2_data.h"

void ov74_0222F2D4(WonderCardsApp2Data *appData, u32 narcMemberIdx, u32 bgLayer, u32 size);

void ov74_0222F314(WonderCardsApp2Data *appData, int screen);

void ov74_0222F314(WonderCardsApp2Data *appData, int screen) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 7, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x100, appData->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 10, appData->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x3000, TRUE, appData->heapID);
    switch (screen) {
    case 0:
        GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 8, appData->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, appData->heapID);
        break;
    case 1:
        GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 9, appData->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, appData->heapID);
        break;
    }
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_11_OFFSET, 0x20, appData->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, appData->bgConfig, GF_BG_LYR_MAIN_3, 0, 0x200, TRUE, appData->heapID);
    ov74_0222F2D4(appData, 2, GF_BG_LYR_MAIN_3, 0x600);
    BgTilemapRectChangePalette(appData->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 0x20, 0x18, 0xB);
    BgCommitTilemapBufferToVram(appData->bgConfig, GF_BG_LYR_MAIN_3);
}
