#include "global.h"

#include "gf_gfx_planes.h"
#include "gf_3d_vramman.h"

void ov93_0225CF34(void);

void ov93_0225CF34(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);

    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(1);
    G3X_AlphaTest(0, 0);
    G3X_AlphaBlend(1);
    G3X_EdgeMarking(0);
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, 0);
    G3_ViewPort(0, 0, 255, 191);
}

void GF_3DVramMan_Delete(GF3DVramMan *vramMan);

void ov93_0225CFB8(GF3DVramMan *vramMan);

void ov93_0225CFB8(GF3DVramMan *vramMan) {
    GF_3DVramMan_Delete(vramMan);
}
