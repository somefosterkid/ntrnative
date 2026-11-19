#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_80_0222ACA0.h"
#include "frontier_graphics.h"

void ov80_0222AE28(FrontierGraphics *graphics, void **sceneData) {
}

void ov80_0222AE2C(FrontierGraphics *graphics, void **sceneData) {
}

struct FrontierGraphics;

void ov80_0222AE30(FrontierGraphics *graphics, void **sceneData);

void ov80_0222AE30(FrontierGraphics *graphics, void **sceneData) {
    SetBgPriority(GF_BG_LYR_MAIN_0, 1);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);

    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, FALSE);
}

void ov80_0222AE68(FrontierGraphics *graphics, void **sceneData) {
}
