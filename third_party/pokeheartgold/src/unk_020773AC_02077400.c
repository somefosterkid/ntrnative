#include "global.h"

#include "gf_3d_vramman.h"
#include "unk_020773AC.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "unk_02014DA0.h"
#include "unk_02026E30.h"

void sub_0207741C(void);

void sub_0207741C(void);

GF3DVramMan *sub_02077400(enum HeapID heapID) {
    return GF_3DVramMan_Create(heapID, 0, 2, 0, 2, sub_0207741C);
}

void sub_0207741C(void) {
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

void sub_020774A0(void) {
    NNSGfdTexKey v0;
    NNSGfdPlttKey v1;
    u32 v2, v3;

    v0 = NNS_GfdAllocTexVram(0x2000 * 4, 0, 0);
    v1 = NNS_GfdAllocPlttVram(0x20 * (4 + 1), 0, 0);

    ((v0 != (u32)0x0) ? (void)0 : GF_AssertFail());
    ((v1 != (u32)0x0) ? (void)0 : GF_AssertFail());

    v2 = NNS_GfdGetTexKeyAddr(v0);
    v3 = NNS_GfdGetPlttKeyAddr(v1);

    sub_02014DA0();
}

void sub_020774E0(void) {
    int result;

    Thunk_G3X_Reset();

    result = sub_0201543C();

    if (result > 0) {
        Thunk_G3X_Reset();
        NNS_G2dSetupSoftwareSpriteCamera();
    }

    sub_02015460();
    RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

u32 sub_02077504(u32 arg0, s32 arg1) {
    u32 texKey;

    texKey = NNS_GfdDefaultFuncAllocTexVram(arg0, arg1, 0U);
    sub_02015354(texKey);
    return (u32)(texKey << 0x10) >> 0xD;
}

u32 sub_02077520(u32 arg0, s32 arg1) {
    u32 plttKey;

    plttKey = NNS_GfdDefaultFuncAllocPlttVram(arg0, arg1, 0U);
    sub_02015394(plttKey);
    return (u32)(plttKey << 0x10) >> 0xD;
}
