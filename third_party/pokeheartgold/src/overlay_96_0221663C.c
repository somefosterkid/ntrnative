#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_0221663C(Ov96RdWork *work);

void ov96_0221663C(Ov96RdWork *work) {
    GfGfxLoader_LoadCharData(NARC_a_2_4_0, 1, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 1, work->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_4_0, 3, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 1, work->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_4_0, 5, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 1, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_0, 2, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 1, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_0, 4, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 1, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_0, 6, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 1, work->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_4_0, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, work->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_4_0, 9, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 1, work->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_4_0, 0xb, work->bgConfig, GF_BG_LYR_MAIN_CNT, 0, 0, 1, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_0, 0xc, work->bgConfig, GF_BG_LYR_MAIN_CNT, 0, 0, 1, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_0, 10, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 1, work->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_4_0, 8, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    return;
}
