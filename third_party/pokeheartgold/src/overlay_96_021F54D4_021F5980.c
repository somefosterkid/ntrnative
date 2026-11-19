#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

void ov96_021F5980(UnkStruct_ov96_021F5018 *arg0);

void ov96_021F5980(UnkStruct_ov96_021F5018 *arg0) {
    GfGfxLoader_LoadCharData(NARC_a_1_6_8, 7, arg0->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_6_8, 5, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_6_8, 0xA, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_6_8, 0xC, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_8, 8, arg0->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_8, 6, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_8, 0xB, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_8, 0xD, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_8, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, arg0->heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_8, 9, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, arg0->heapID);
    arg0->unk_58 = GfGfxLoader_GetScrnData(NARC_a_1_6_8, 0xE, 0, &arg0->unk_5C, arg0->heapID);
}
