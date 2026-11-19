#include "global.h"

#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "unk_02014DA0.h"

void ov95_021E5974(void);

GF3DVramMan *ov95_021E5954(void);

void ov95_021E5974(void);
void ov95_021E59F8(void);

GF3DVramMan *ov95_021E5954(void) {
    return GF_3DVramMan_Create(HEAP_ID_70, 0, 2, 0, 2, ov95_021E5974);
}

void ov95_021E5974(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);

    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(1);
    G3X_AlphaTest(0, 0);
    G3X_AlphaBlend(1);
    G3X_EdgeMarking(0);
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(((GXRgb)(((0) << (0)) | ((0) << (5)) | ((0) << (10)))), 0, 0x7fff, 63, 0);
    G3_ViewPort(0, 0, 255, 191);
}

void ov95_021E59F8(void) {
    NNSGfdTexKey texKey;
    NNSGfdPlttKey plttKey;
    u32 texKeyAddr, plttKeyAddr;

    texKey = NNS_GfdAllocTexVram(0x8000, 0, 0);
    plttKey = NNS_GfdAllocPlttVram(0x80, 0, 0);

    ((texKey != (u32)0x0) ? (void)0 : GF_AssertFail());
    ((plttKey != (u32)0x0) ? (void)0 : GF_AssertFail());

    texKeyAddr = NNS_GfdGetTexKeyAddr(texKey);
    plttKeyAddr = NNS_GfdGetPlttKeyAddr(plttKey);

    sub_02014DA0();
}
