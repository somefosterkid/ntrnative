#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_96_ov96hurdle_hud.h"

void ov96_02208374(Ov96HurdleHud *hud, s32 value);

void ov96_02207DDC(Ov96HurdleHud *hud);

void ov96_02207DDC(Ov96HurdleHud *hud) {
    GfGfxLoader_LoadCharData(NARC_a_2_2_4, 5, hud->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, hud->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_2_4, 6, hud->bgConfig, GF_BG_LYR_SUB_2, 0U, 0x2000U, 0, hud->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_2_4, 7, hud->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, hud->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_2_4, 8, hud->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, hud->heapId);
    hud->scrnFile = GfGfxLoader_GetScrnData(NARC_a_2_2_4, 9, 0, &hud->scrnData, hud->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_2_4, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, hud->heapId);
    ov96_02208374(hud, 0);
}
