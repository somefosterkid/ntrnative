#include "global.h"

#include "gf_gfx_loader.h"

void ov96_021EC3D8(BgConfig *arg0, enum HeapID arg1);

void ov96_021EC3D8(BgConfig *bgConfig, enum HeapID heapID) {
    GfGfxLoader_LoadCharData(NARC_a_1_5_4, 3, bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_5_4, 1, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_4, 6, bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_4, 4, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_5_4, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_5_4, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, heapID);
}

void ov96_021EC458(BgConfig *arg0, enum HeapID arg1);

void ov96_021EC458(BgConfig *bgConfig, enum HeapID heapID) {
    GfGfxLoader_LoadCharData(NARC_a_1_5_4, 3, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_4, 7, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, heapID);
}
