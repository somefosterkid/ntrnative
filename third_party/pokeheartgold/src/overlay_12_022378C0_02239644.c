#include "global.h"

#include "constants/heap.h"

#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "overlay_12_022378C0.h"
#include "sys_task.h"
#include "unk_02014DA0.h"

GF3DVramMan *ov12_02239644(void);

static void ov12_02239664(void);

GF3DVramMan *ov12_02239644(void) {
    return GF_3DVramMan_Create(
        HEAP_ID_BATTLE,
        GF_3D_TEXALLOC_LNK,
        2, // 256K texture VRAM
        GF_3D_PLTTALLOC_LNK,
        2, // 32K palette VRAM
        ov12_02239664);
}

static void ov12_02239664(void) {
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

void ov12_022396E8(GF3DVramMan *vramMan) {
    GF_3DVramMan_Delete(vramMan);
}

void ov12_022396F0(void) {
    NNSGfdTexKey texKey = NNS_GfdAllocTexVram(0x2000 * 4, 0, 0);
    NNSGfdPlttKey plttKey = NNS_GfdAllocPlttVram(0x20 * (4 + 2), 0, 0);

    ((texKey != (u32)0x0) ? (void)0 : GF_AssertFail());
    ((plttKey != (u32)0x0) ? (void)0 : GF_AssertFail());

    sub_02014DA0();
}
