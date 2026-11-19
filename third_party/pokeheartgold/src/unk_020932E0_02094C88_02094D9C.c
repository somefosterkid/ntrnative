#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"

s32 sub_02094DF8(BgConfig *param0);

void sub_02094D9C(BgConfig *arg0, enum HeapID arg1);

void sub_02094D9C(BgConfig *bgConfig, enum HeapID heapID) {
    sub_02094DF8(bgConfig);
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    GfGfxLoader_GXLoadPal(NARC_a_1_5_1, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0xC0U, heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_5_1, 2, bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0x1400U, 1, heapID);
    BG_FillCharDataRange(bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 1U, 0U);
}
