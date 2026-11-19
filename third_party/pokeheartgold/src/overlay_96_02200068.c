#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_96_ov96_mon_icon_work.h"

void ov96_02200068(Ov96MonIconWork *work) {
    GfGfxLoader_LoadCharData(NARC_a_1_7_8, 6, work->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, work->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_7_8, 7, work->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, work->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_7_8, 8, work->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, work->heapID);
    work->scrnBuf = GfGfxLoader_GetScrnData(NARC_a_1_7_8, 9, 0, &work->scrnData, work->heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_7_8, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, work->heapID);
}
