#include "global.h"

#include "constants/heap.h"

#include "frontier/frontier.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "frontier_map.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "palette.h"
#include "render_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "frontier_map_internal.h"


void FrontierMap_LoadPaletteData(void **param_1) {
    FrontierLaunchArgs *pFVar1;
    u32 uVar2;

    PaletteData_LoadNarc((PaletteData *)param_1[1], NARC_graphic_font, 7, HEAP_ID_101, PLTTBUF_MAIN_BG, 0x20, 0xe0);
    PaletteData_LoadNarc((PaletteData *)param_1[1], NARC_graphic_font, 8, HEAP_ID_101, PLTTBUF_MAIN_BG, 0x20, 0xd0);
    pFVar1 = Frontier_GetLaunchArgs((void *)param_1[2]);
    uVar2 = Options_GetFrame(pFVar1->options);
    LoadUserFrameGfx2((BgConfig *)*param_1, GF_BG_LYR_MAIN_1, 0x3e2, 0xb, (u8)uVar2, HEAP_ID_101);
    PaletteData_LoadPaletteSlotFromHardware((PaletteData *)param_1[1], PLTTBUF_MAIN_BG, 0xb0, 0x20);
    LoadUserFrameGfx1((BgConfig *)*param_1, GF_BG_LYR_MAIN_1, 0x3d9, 0xc, 0, HEAP_ID_101);
    PaletteData_LoadPaletteSlotFromHardware((PaletteData *)param_1[1], PLTTBUF_MAIN_BG, 0xc0, 0x20);
    return;
}

void ov80_02238FA0(void *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_a_1_8_3, HEAP_ID_101);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x81, *(BgConfig **)arg0, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, HEAP_ID_101);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x82, *(BgConfig **)arg0, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, HEAP_ID_101);
    PaletteData_LoadNarc(((FrontierMapInternal *)arg0)->paletteData, NARC_a_1_8_3, 0xBE, HEAP_ID_101, PLTTBUF_SUB_BG, 0x20U, 0U);
    NARC_Delete(narc);
}
