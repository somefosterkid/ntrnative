#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "unk_0203A3B0.h"

void ov102_021E95E0(void **arg0, NARC *arg1);

void ov102_021E95E0(void **arg0, NARC *arg1) {
    NNSG2dImagePaletteProxy *sp1C;
    NNSG2dCellDataBank **sp18;
    NNSG2dAnimBankData **sp14;
    NNSG2dImageProxy *var_r7;
    void *var_r4;
    s32 var_r6;
    var_r4 = *arg0;
    sp1C = var_r4 + 0x198;
    var_r7 = var_r4 + 0x150;
    var_r6 = 0;
    sp18 = var_r4 + 0x1C0;
    sp14 = var_r4 + 0x1C8;
    do {
        NNS_G2dInitImagePaletteProxy(sp1C);
        NNS_G2dInitImageProxy(var_r7);
        GfGfxLoader_LoadImageMappingFromOpenNarc(arg1, 9, 1, GF_BG_LYR_MAIN_FIRST, 0U, (NNS_G2D_VRAM_TYPE)((u32)var_r6 + 1), 0U, HEAP_ID_35, var_r7);
        GfGfxLoader_PartiallyLoadPaletteFromOpenNarc(arg1, 0xA, (NNS_G2D_VRAM_TYPE)(var_r6 + 1), 0U, HEAP_ID_35, sp1C);
        *((void **)(((u8 *)var_r4) + 0x1D0)) = GfGfxLoader_GetCellBankFromOpenNarc(arg1, 7, 1, sp18, HEAP_ID_35);
        *((void **)(((u8 *)var_r4) + 0x1D8)) = GfGfxLoader_GetAnimBankFromOpenNarc(arg1, 8, 1, sp14, HEAP_ID_35);
        sp1C = (void *)(((u8 *)sp1C) + 0x14);
        var_r7 = (void *)(((u8 *)var_r7) + 0x24);
        sp18 += 1;
        var_r4 += 4;
        sp14 += 1;
        var_r6 += 1;
    } while (var_r6 < 2);
    sub_0203A964();
}
