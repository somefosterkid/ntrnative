#include "global.h"

#include "gf_gfx_planes.h"

void ov112_021F0DF4(void);

void ov112_021F0DF4(void) {
    GraphicsBanks banks = {
        GX_VRAM_BG_64_E,
        GX_VRAM_BGEXTPLTT_NONE,
        GX_VRAM_SUB_BG_128_C,
        GX_VRAM_SUB_BGEXTPLTT_NONE,
        GX_VRAM_OBJ_256_AB,
        GX_VRAM_OBJEXTPLTT_NONE,
        GX_VRAM_SUB_OBJ_16_I,
        GX_VRAM_SUB_OBJEXTPLTT_NONE,
        GX_VRAM_TEX_NONE,
        GX_VRAM_TEXPLTT_NONE
    };

    GfGfx_SetBanks(&banks);
}
