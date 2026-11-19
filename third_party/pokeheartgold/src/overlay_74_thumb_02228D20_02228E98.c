#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"

void ov74_02228E98(void *arg0);

void ov74_02228E98(void *arg0) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_8_OFFSET, 0x20U, *(enum HeapID *)arg0);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, *(BgConfig **)((u8 *)arg0 + 4), GF_BG_LYR_SUB_2_F, 0U, 0x1400U, 1, *(enum HeapID *)arg0);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 2, *(BgConfig **)((u8 *)arg0 + 4), GF_BG_LYR_SUB_2_F, 0U, 0x600U, 1, *(enum HeapID *)arg0);
    BgTilemapRectChangePalette(*(BgConfig **)((u8 *)arg0 + 4), 4U, 0U, 0U, 0x20U, 0x18U, 8U);
    BgCommitTilemapBufferToVram(*(BgConfig **)((u8 *)arg0 + 4), 4U);
}
