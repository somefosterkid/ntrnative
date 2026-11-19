#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "overlay_29_number_entry_data.h"

void ov29_0225D6B4(BgConfig *arg0);

void ov29_0225D6B4(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
}

void ov29_0225D6C8(NumberEntryData *data, NARC *narc);

void ov29_0225D6C8(NumberEntryData *data, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 9, data->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xA, data->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_8);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 8, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_8);
}
