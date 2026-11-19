#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "gf_gfx_loader.h"
#include "overlay_96_unk_struct_ov96_021_f7934.h"

void ov96_021F7DA8(UnkStruct_ov96_021F7934 *arg0);

void ov96_021F7DA8(UnkStruct_ov96_021F7934 *arg0) {
    GfGfxLoader_LoadCharData(NARC_a_1_6_0, 3, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_0, 5, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_6_0, 3, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_0, 4, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_0, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_6_0, 2, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_0, 1, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_0, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapID);
}

u8 ov96_021F7E64(PokeathlonCourseData *param_1);

u8 ov96_021F7E64(PokeathlonCourseData *param_1) {
    u8 *puVar1;

    puVar1 = PokeathlonCourse_GetDataCopyArea(param_1);
    puVar1 = ov96_021E8A20(puVar1 + 0xf0);
    return puVar1[2];
}
