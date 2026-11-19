#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"

void ov96_021F9FE8(BgConfig *param_1, enum HeapID param_2);

void ov96_021F9FE8(BgConfig *param_1, enum HeapID param_2) {
    GfGfxLoader_LoadCharData(NARC_a_1_5_6, 1, param_1, GF_BG_LYR_MAIN_0, 0, 0, 0, param_2);
    GfGfxLoader_LoadCharData(NARC_a_1_5_6, 1, param_1, GF_BG_LYR_MAIN_CNT, 0, 0, 0, param_2);
    return;
}

void ov96_021FA020(BgConfig *arg0, enum HeapID arg1);

void ov96_021FA020(BgConfig *bgConfig, enum HeapID heapID) {
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 2, bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 3, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 4, bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 2, bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 3, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 4, bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 5, bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 0, heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_5_6, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_5_6, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, heapID);
}
