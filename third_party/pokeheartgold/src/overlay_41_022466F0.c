#include "global.h"

#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"

void ov41_022466F0(void);

void ov41_022466F0(void) {
    NNS_G3dInit();
    G3X_InitMtxStack();

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);

    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(1);
    G3X_AlphaTest(0, 0);
    G3X_AlphaBlend(1);
    G3X_SetClearColor(((GXRgb)(((0) << (0)) | ((0) << (5)) | ((0) << (10)))), 0, 0x7fff, 63, 0);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);

    GF_3DVramMan_InitFrameTexVramManager(2, 1);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, 1);
}
