#include "global.h"

#include "gf_gfx_loader.h"
#include "palette.h"
#include "unk_02014DA0.h"

void ov95_021E5D44(BgConfig *arg0, PaletteData *arg1);

void ov95_021E5D44(BgConfig *bgConfig, PaletteData *data) {
    GfGfxLoader_LoadCharData(NARC_a_1_1_5, 0, bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_70);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_5, 1, bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_70);
    PaletteData_LoadNarc(data, NARC_a_1_1_5, 8, HEAP_ID_70, PLTTBUF_MAIN_BG, 0x40U, 0U);
}

u32 ov95_021E5D98(u32 arg0, s32 arg1);

u32 ov95_021E5D98(u32 arg0, s32 arg1) {
    u32 texKey;

    texKey = NNS_GfdDefaultFuncAllocTexVram(arg0, arg1, 0U);
    sub_02015354(texKey);
    return (u32)(texKey << 0x10) >> 0xD;
}

u32 ov95_021E5DB4(u32 arg0, s32 arg1);

u32 ov95_021E5DB4(u32 arg0, s32 arg1) {
    u32 plttKey;

    plttKey = NNS_GfdDefaultFuncAllocPlttVram(arg0, arg1, 0U);
    sub_02015394(plttKey);
    return (u32)(plttKey << 0x10) >> 0xD;
}
