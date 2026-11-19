#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "overlay_96_ov96_pic_panel.h"

void ov96_02203A64(Ov96PicPanel *panel, s32 arg1) {
    GfGfxLoader_LoadCharData(NARC_a_2_1_7, 4, panel->bgConfig, GF_BG_LYR_SUB_3, 0U, 0x2000U, 0, panel->heapID);
    GfGfxLoader_LoadCharData(NARC_a_2_1_7, 5, panel->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, panel->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 6, panel->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 0, panel->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 7, panel->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, panel->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 8, panel->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, panel->heapID);
    panel->scrnBuf = GfGfxLoader_GetScrnData(NARC_a_2_1_7, 9, 0, &panel->scrnData, panel->heapID);
    GfGfxLoader_GXLoadPal(NARC_a_2_1_7, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x80U, panel->heapID);
    CopyRectToBgTilemapRect(panel->bgConfig, 5U, 0U, 0x12U, 4U, 4U, (const void *)panel->scrnData->rawData, (u8)(u8)(arg1 * 4), 0x10U, 0x14U, 0x14U);
    ScheduleBgTilemapBufferTransfer(panel->bgConfig, 5U);
}
