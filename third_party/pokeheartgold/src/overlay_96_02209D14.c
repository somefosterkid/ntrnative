#include "global.h"

#include "font.h"
#include "gf_gfx_loader.h"
#include "overlay_96_ov96_0220_data.h"

void ov96_02209D14(Ov96_0220Data *arg0);

void ov96_02209D14(Ov96_0220Data *arg0) {
    GfGfxLoader_LoadCharData(NARC_a_2_3_2, 1, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_2, 2, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_2, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapId);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, arg0->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_2, 4, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_2, 4, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_2, 5, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_2, 6, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_2, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapId);
}
