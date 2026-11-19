#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_96_ov96_course_hud.h"

typedef struct UnkStruct_ov96_02211A24 {
    u8 unk_0[0x4];
    BgConfig *unk_4;
    u8 unk_8[0x50];
    enum HeapID unk_58;
    u8 unk_5C[0x6F4];
    Ov96CourseHud *unk_750;
} UnkStruct_ov96_02211A24;

void ov96_02211A24(UnkStruct_ov96_02211A24 *arg0);

void ov96_02211A24(UnkStruct_ov96_02211A24 *arg0) {
    GfGfxLoader_LoadCharData(NARC_a_2_3_4, 1, arg0->unk_4, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, arg0->unk_58);
    GfGfxLoader_LoadCharData(NARC_a_2_3_4, 3, arg0->unk_4, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, arg0->unk_58);
    GfGfxLoader_LoadCharData(NARC_a_2_3_4, 6, arg0->unk_4, GF_BG_LYR_SUB_1, 0U, 0U, 0, arg0->unk_58);
    GfGfxLoader_LoadCharData(NARC_a_2_3_4, 7, arg0->unk_4, GF_BG_LYR_SUB_2, 0U, 0U, 0, arg0->unk_58);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_4, 2, arg0->unk_4, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, arg0->unk_58);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_4, 4, arg0->unk_4, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, arg0->unk_58);
    ov96_022146C0(arg0->unk_750);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_4, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x80U, arg0->unk_58);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_4, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, arg0->unk_58);
}
