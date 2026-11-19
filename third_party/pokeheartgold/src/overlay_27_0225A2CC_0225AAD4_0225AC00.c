#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"

extern const u32 ov27_0225CEEC[][3];

extern const u32 ov27_0225D074[][2];

void ov27_0225AC00(BgConfig *bgConfig, int idx, Window *win1, Window *win2, Window *wins);

void ov27_0225AC00(BgConfig *bgConfig, int idx, Window *win1, Window *win2, Window *wins) {
    int i;
    GfGfxLoader_LoadCharData(NARC_a_0_1_4, ov27_0225CEEC[idx][1], bgConfig, GF_BG_LYR_SUB_2_F, 0, 0, TRUE, HEAP_ID_8);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_4, ov27_0225CEEC[idx][2], bgConfig, GF_BG_LYR_SUB_2_F, 0, 0, TRUE, HEAP_ID_8);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_4, ov27_0225CEEC[idx][0], GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x200, HEAP_ID_8);
    AddWindowParameterized(bgConfig, win1, 5, 0x18, 0x14, 8, 2, 4, 0xD2);
    FillWindowPixelBuffer(win1, 0);
    AddWindowParameterized(bgConfig, win2, 5, 9, 0, 0xA, 2, 4, 0xE2);
    FillWindowPixelBuffer(win2, 0);
    for (i = 0; i < 8; i++) {
        AddWindowParameterized(bgConfig, &wins[i], 5, ov27_0225D074[i][0], ov27_0225D074[i][1], 9, 2, 4, 0xF6 + i * 0x12);
        FillWindowPixelBuffer(&wins[i], 0);
    }
}
