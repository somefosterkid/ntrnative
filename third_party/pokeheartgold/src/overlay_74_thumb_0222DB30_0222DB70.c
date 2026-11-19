#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "overlay_74_wonder_cards_app_data.h"

void ov74_0222DB30(WonderCardsAppData *appData, u32 narcMemberIdx, u32 bgLayer, u32 size);
void ov74_0222DB70(WonderCardsAppData *appData, enum WonderCardsAppScreen screen);

void ov74_0222DB70(WonderCardsAppData *appData, enum WonderCardsAppScreen screen) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x100, appData->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 6, appData->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x3000, TRUE, appData->heapID);
    switch (screen) {
    case WC_SCREEN_WONDERCARD_FRONT:
        GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 4, appData->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, appData->heapID);
        break;
    case WC_SCREEN_WONDERCARD_BACK:
        GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 5, appData->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, appData->heapID);
        break;
    }
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_11_OFFSET, 0x20, appData->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, appData->bgConfig, GF_BG_LYR_MAIN_3, 0, 0x200, TRUE, appData->heapID);
    ov74_0222DB30(appData, 2, GF_BG_LYR_MAIN_3, 0x600);
    BgTilemapRectChangePalette(appData->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 0x20, 0x18, 0xB);
    BgCommitTilemapBufferToVram(appData->bgConfig, GF_BG_LYR_MAIN_3);
}
