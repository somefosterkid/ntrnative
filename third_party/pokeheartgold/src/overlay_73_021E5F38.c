#include "global.h"

#include "font.h"
#include "gf_gfx_loader.h"
#include "options.h"
#include "render_window.h"
#include "heap.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

void ov73_021E5F38(UnkStruct_020961E8 *arg0, NARC *arg1);

void ov73_021E6048(UnkStruct_020961E8 *arg0);

void ov73_021E5F38(UnkStruct_020961E8 *arg0, NARC *narc) {
    BgConfig *bgConfig;
    bgConfig = arg0->unk_00;
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_50);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_50);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_50);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_50);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, HEAP_ID_50);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, HEAP_ID_50);
    arg0->unk_310 = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 3, 1, &arg0->unk_30C, HEAP_ID_50);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_50);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_50);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_50);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_50);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_FIRST, 1U, 0xBU, (u8)((s32)Options_GetFrame(*((Options **)(8 + ((u8 *)(arg0->unk_08)))))), HEAP_ID_50);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_FIRST, 0x1FU, 0xAU, 0U, HEAP_ID_50);
}

void ov73_021E6048(UnkStruct_020961E8 *arg0) {
    Heap_Free(arg0->unk_310);
    arg0->unk_310 = NULL;
}
