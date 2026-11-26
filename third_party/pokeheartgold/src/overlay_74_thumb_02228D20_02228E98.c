#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "overlay_74_ranger_link_manager.h"

void ov74_02228E98(RangerLinkManager *arg0);

void ov74_02228E98(RangerLinkManager *arg0) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_8_OFFSET, 0x20U, arg0->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0x1400U, 1, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 2, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0x600U, 1, arg0->heapID);
    BgTilemapRectChangePalette(arg0->bgConfig, 4U, 0U, 0U, 0x20U, 0x18U, 8U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 4U);
}
