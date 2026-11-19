#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "bg_window.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"

s32 ov70_022392BC(BgConfig *bgConfig);

void ov70_0223B8B4(BgConfig *arg0);

void ov70_02239CF8(GTSApplicationState *);
void ov70_0223B8E0(GTSApplicationState *param_1);

void ov70_0223B8B4(BgConfig *bgConfig) {
    ov70_022392BC(bgConfig);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
    FreeBgTilemapBuffer(bgConfig, 3U);
}

void ov70_0223B8E0(GTSApplicationState *param_1) {
    NARC *narc;
    u32 uVar1;
    BgConfig *bgConfig;

    bgConfig = (BgConfig *)param_1->bgConfig;
    narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x100, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    uVar1 = Options_GetFrame(*(Options **)((u8 *)param_1->playerData + 0x24));
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 0xe, (u8)uVar1, HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1f, 0xb, 0, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0xe, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xa00, 1, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x1d, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, 1, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0xf, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, 1, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_61);
    ov70_02239CF8(param_1);
    NARC_Delete(narc);
    return;
}
