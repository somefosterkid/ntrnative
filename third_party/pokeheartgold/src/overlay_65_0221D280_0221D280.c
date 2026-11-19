#include "global.h"
struct TradeRoom;

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221D204(NARC *, s32, BgConfig *, u16, s32, s32, s32, s32);

void ov65_0221D280(struct TradeRoom *arg0, NARC *arg1);

void ov65_0221D280(TradeRoom *arg0, NARC *narc) {
    BgConfig *bgConfig;

    bgConfig = arg0->bgConfig;
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x120U, HEAP_ID_26);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x120U, HEAP_ID_26);
    BG_SetMaskColor(0U, 0U);
    BG_SetMaskColor(4U, 0U);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_26);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0x2400U, 1, HEAP_ID_26);
    ov65_0221D204(narc, 3, bgConfig, 2, 0, 0x600, 1, 0x1A);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, bgConfig, GF_BG_LYR_MAIN_3, 0U, 0x600U, 1, HEAP_ID_26);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_2_OFFSET, HEAP_ID_26);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_3_OFFSET, HEAP_ID_26);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_SUB_1, 0U, 0x800U, 1, HEAP_ID_26);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 5, bgConfig, GF_BG_LYR_SUB_1, 0U, 0x600U, 1, HEAP_ID_26);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_SUB_2, 0U, 0x600U, 1, HEAP_ID_26);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_2_OFFSET, HEAP_ID_26);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_3_OFFSET, HEAP_ID_26);
    BgClearTilemapBufferAndCommit(bgConfig, 4U);
}
