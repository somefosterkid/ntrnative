#include "global.h"

#include "bg_window.h"
#include "overlay_80_0222ACA0.h"
#include "overlay_80_022384D8.h"
#include "frontier_graphics.h"
#include "gf_gfx_planes.h"

struct FrontierGraphics;

void ov80_0222AE6C(FrontierGraphics *graphics, void **sceneData);

void ov80_0222AE6C(FrontierGraphics *graphics, void **sceneData) {
    SetBgPriority(GF_BG_LYR_MAIN_2, 3);
    SetBgPriority(GF_BG_LYR_MAIN_3, 2);
}

void ov80_0222AE80(FrontierGraphics *graphics, void **sceneData) {
}

void ov80_0222AE84(FrontierGraphics *graphics, void **sceneData);

void ov80_0222AE84(FrontierGraphics *graphics, void **sceneData) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
}

void ov80_0222AE9C(FrontierGraphics *graphics, void **sceneData) {
}

void ov80_0222AEA0(FrontierGraphics *graphics, void **sceneData) {
    *sceneData = ov80_022384D8(graphics->bgConfig);
}

void ov80_0222AEB0(FrontierGraphics *graphics, void **sceneData);

void ov80_0222AEB0(FrontierGraphics *graphics, void **sceneData) {
    ov80_022385B0(*sceneData);
}

void ov80_0222AEBC(FrontierGraphics *graphics, void **sceneData);

void ov80_0222AEBC(FrontierGraphics *graphics, void **sceneData) {
    SetBgPriority(GF_BG_LYR_MAIN_1, 0);
    SetBgPriority(GF_BG_LYR_MAIN_0, 2);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);

    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
}

void ov80_0222AEF4(FrontierGraphics *graphics, void **sceneData) {
}
