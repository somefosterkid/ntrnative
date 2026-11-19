#include "global.h"
#include "gf_gfx_loader.h"
#include "overlay_99_obj99.h"

void ov99_021E69D8(Obj99 *arg0);

void ov99_021E69D8(Obj99 *arg0) {
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0x38, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, arg0->heap);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x39, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, arg0->heap);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x3A, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, arg0->heap);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0x3B, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, arg0->heap);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x3C, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, arg0->heap);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x3D, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, arg0->heap);
}
