#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"

extern GraphicsModes ov47_02259E48;

extern s32 ov47_02259E68;

extern BgTemplate ov47_02259F18;

void ov47_02258AA0(BgConfig **arg0, Options *arg1, enum HeapID arg2);

void ov47_02258AA0(BgConfig **arg0, Options *arg1, enum HeapID arg2) {
    s32 sp14;
    Options *sp10;
    BgTemplate *var_r6;
    u32 *var_r4;
    s32 temp_r0;
    sp10 = arg1;
    SetBothScreensModesAndDisable(&ov47_02259E48);
    *arg0 = BgConfig_Alloc(arg2);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    var_r6 = &ov47_02259F18;
    var_r4 = &ov47_02259E68;
    sp14 = 0;
    do {
        InitBgFromTemplate(*arg0, (u8)(*var_r4), var_r6, 0U);
        BG_ClearCharDataRange((u8)(*var_r4), 0x20U, 0U, arg2);
        BgClearTilemapBufferAndCommit(*arg0, (u8)(*var_r4));
        var_r6 = (void *)((u8 *)var_r6 + (0x1C));
        var_r4 = (void *)((u8 *)var_r4 + 4);
        temp_r0 = sp14 + 1;
        sp14 = temp_r0;
    } while (temp_r0 < 4);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_4_OFFSET, arg2);
    LoadUserFrameGfx2(*arg0, GF_BG_LYR_SUB_0_F, 1U, 5U, (u8)((s32)Options_GetFrame(sp10)), arg2);
    LoadUserFrameGfx1(*arg0, GF_BG_LYR_SUB_0_F, 0x1FU, 6U, 0U, arg2);
    GfGfxLoader_LoadCharData(NARC_a_1_9_6, 2, *arg0, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, arg2);
    GfGfxLoader_LoadScrnData(NARC_a_1_9_6, 6, *arg0, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, arg2);
    GfGfxLoader_GXLoadPal(NARC_a_1_9_6, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg2);
}
