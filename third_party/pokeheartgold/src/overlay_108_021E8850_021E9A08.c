#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_108_021E8850.h"
#include "render_window.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021E9A08(SafariDecorationApp *arg0) {
    FreeBgTilemapBuffer(arg0->bgConfig, 7U);
    FreeBgTilemapBuffer(arg0->bgConfig, 6U);
    FreeBgTilemapBuffer(arg0->bgConfig, 5U);
    FreeBgTilemapBuffer(arg0->bgConfig, 4U);
    FreeBgTilemapBuffer(arg0->bgConfig, 3U);
    FreeBgTilemapBuffer(arg0->bgConfig, 2U);
    FreeBgTilemapBuffer(arg0->bgConfig, 1U);
    Heap_Free(arg0->bgConfig);
}

void ov108_021E9A60(SafariDecorationApp *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_a_2_2_3, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, arg0->heapID);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 3, arg0->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 0, arg0->heapID);
    arg0->scrnBuf0 = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 8, 0, &arg0->scrn0, arg0->heapID);
    arg0->scrnBuf1 = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 6, 0, &arg0->scrn1, arg0->heapID);
    arg0->scrnBuf2 = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 7, 0, &arg0->scrn2, arg0->heapID);
    NARC_Delete(narc);
    LoadUserFrameGfx2(arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0x3E2U, 0xCU, (u8)(s32)arg0->frame, arg0->heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, arg0->heapID);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 7U);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 3U);
    G2_SetBlendAlpha(1, 0x1E, 0x1C, 4);
}

void ov108_021E9BD4(SafariDecorationApp *arg0) {
    G2_SetBlendAlpha(0, 0, 0x1F, 0);
    Heap_Free(arg0->scrnBuf2);
    Heap_Free(arg0->scrnBuf1);
    Heap_Free(arg0->scrnBuf0);
}
