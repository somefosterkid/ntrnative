#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "field_system_def.h"
#include "overlay_02_02245B80.h"

void ov02_02246398(LegendFadeTask *arg0) {
    NARC *narc;
    u8 ev1;

    arg0->alpha = 0U;
    ev1 = arg0->alpha;
    G2_SetBlendAlpha(2, 5, (s32)ev1, 0x1F - ev1);
    narc = NARC_New(NARC_demo_legend, arg0->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0xD, arg0->fieldSystem->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0x40U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xE, arg0->fieldSystem->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, arg0->heapID);
    arg0->bgPriority0 = GetBgPriority(arg0->fieldSystem->bgConfig, 0U);
    arg0->bgPriority1 = GetBgPriority(arg0->fieldSystem->bgConfig, 1U);
    SetBgPriority(0U, 1U);
    SetBgPriority(1U, 0U);
    GfGfx_EngineATogglePlanes(2U, 1U);
    NARC_Delete(narc);
}

void ov02_02246444(LegendFadeTask *arg0) {
    GfGfx_EngineATogglePlanes(2U, 0U);
    SetBgPriority(0U, (u16) arg0->bgPriority0);
    SetBgPriority(1U, (u16) arg0->bgPriority1);
    BgClearTilemapBufferAndCommit(arg0->fieldSystem->bgConfig, 1U);
    BG_ClearCharDataRange(1U, 0x40U, 0U, arg0->heapID);
    G2_SetBlendAlpha(0, 0, 0x1F, 0);
}
