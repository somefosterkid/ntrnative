#include "global.h"
struct UnkStruct_ov116_0226139C;

#include "gf_gfx_loader.h"
#include "palette.h"
#include "render_window.h"
#include "overlay_92_unk_struct_ov116_0226139_c.h"

void ov92_0225E9B4(struct UnkStruct_ov116_0226139C *arg0);

void ov92_0225E9B4(UnkStruct_ov116_0226139C *arg0) {
    NARC *temp_r4;
    BgConfig *temp_r5;
    PaletteData *temp_r6;

    temp_r5 = arg0->unk_48.unk_10;
    temp_r4 = arg0->unk_48.unk_00;
    temp_r6 = arg0->unk_48.unk_14;
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r4, 0x48, temp_r5, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, HEAP_ID_113);
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r4, 0x4B, temp_r5, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_113);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r4, 0x4A, temp_r5, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, HEAP_ID_113);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r4, 0x4C, temp_r5, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_113);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r4, 0x4D, temp_r5, GF_BG_LYR_SUB_2, 0U, 0U, 0, HEAP_ID_113);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r4, 0x4E, temp_r5, GF_BG_LYR_SUB_3, 0U, 0U, 0, HEAP_ID_113);
    PaletteData_LoadNarc(temp_r6, NARC_a_1_9_3, 0x49, HEAP_ID_113, PLTTBUF_SUB_BG, 0xA0U, 0U);
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r4, 5, temp_r5, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_113);
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r4, 8, temp_r5, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, HEAP_ID_113);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r4, 7, temp_r5, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_113);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r4, 9, temp_r5, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, HEAP_ID_113);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r4, 0xA, temp_r5, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, HEAP_ID_113);
    PaletteData_LoadNarc(temp_r6, NARC_a_1_9_3, 6, HEAP_ID_113, PLTTBUF_MAIN_BG, 0x20U, 0U);
    PaletteData_LoadNarc(temp_r6, NARC_graphic_font, 8, HEAP_ID_113, PLTTBUF_SUB_BG, 0x20U, 0xE0U);
    PaletteData_LoadNarc(temp_r6, NARC_a_0_3_8, (s32)sub_0200E3D8(), HEAP_ID_113, PLTTBUF_SUB_BG, 0x20U, 0xD0U);
    LoadUserFrameGfx1(temp_r5, GF_BG_LYR_SUB_3, 0xB4U, 0xDU, 0U, HEAP_ID_113);
}
