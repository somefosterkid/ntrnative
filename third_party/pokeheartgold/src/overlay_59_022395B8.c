#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "bg_window.h"
#include "heap.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "render_window.h"

void ov59_022395B8(Ov59ApricornApp *arg0);

void ov59_022395B8(Ov59ApricornApp *arg0) {
    FreeBgTilemapBuffer(arg0->bgConfig, 3U);
    FreeBgTilemapBuffer(arg0->bgConfig, 2U);
    FreeBgTilemapBuffer(arg0->bgConfig, 1U);
    FreeBgTilemapBuffer(arg0->bgConfig, 0U);
    FreeBgTilemapBuffer(arg0->bgConfig, 7U);
    FreeBgTilemapBuffer(arg0->bgConfig, 4U);
    Heap_Free(arg0->bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}

void ov59_02239604(Ov59ApricornApp *arg0);

void ov59_02239604(Ov59ApricornApp *arg0) {
    NARC *temp_r0;

    temp_r0 = NARC_New(NARC_a_2_1_8, arg0->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(temp_r0, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r0, 4, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 0, arg0->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r0, 5, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 0, arg0->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(temp_r0, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r0, 1, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 0, arg0->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r0, 2, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 0, arg0->heapId);
    arg0->narc = temp_r0;
    LoadUserFrameGfx2(arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 1U, 0xDU, (u8) (s32) arg0->frame, arg0->heapId);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, arg0->heapId);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, arg0->heapId);
    G2_SetBlendAlpha(0, 8, 6, 0x1A);
}

void ov59_022396E4(Ov59ApricornApp *arg0);

void ov59_022396E4(Ov59ApricornApp *arg0) {
    NARC_Delete(arg0->narc);
    G2_SetBlendAlpha(0, 0, 0x1F, 0);
}
