// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov307/egg_demo_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "demo/egg_demo.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The egg demo's BGs, cell actors and 3D system

#define BG_COUNT 6

typedef struct {
    ClActUnit *unit;
} EggDemoClAct;

typedef struct {
    G3DCamera *camera;
} EggDemoG3dSys;

struct EggDemoGraphic {
    EggDemoBG bg;
    EggDemoClAct clact;
    EggDemoG3dSys g3d;
    TCB *vblankTask;
};

void func_ov215_021f6508(TCB *tcb, void *data);

void func_ov215_021f657c(EggDemoBG *bg);
void func_ov215_021f65b4(EggDemoBG *bg);
void func_ov215_021f65b8(EggDemoBG *bg);
void func_ov215_021f661c(EggDemoClAct *clact);
ClActUnit *func_ov215_021f662c(EggDemoClAct *clact);
void func_ov215_021f6600(EggDemoClAct *clact);
void func_ov215_021f6624(EggDemoClAct *clact);
void func_ov215_021f669c(EggDemoG3dSys *g3d);
void func_ov215_021f66ac(EggDemoG3dSys *g3d);
void func_ov215_021f66c0(EggDemoG3dSys *g3d);
void func_ov215_021f651c(EggDemoBG *bg, HeapID heapId);
void func_ov215_021f65c0(EggDemoClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
void func_ov215_021f6630(EggDemoG3dSys *g3d, HeapID heapId);

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov215_021f71c4 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E,  GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_BD, GX_VRAM_TEXPLTT_01_FG,   GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sCameraPosition
static const VecFx32 data_ov215_021f7160 = { 0, FX32_CONST(10), FX32_CONST(100) };
// pokebw2's sCameraTarget
static const VecFx32 data_ov215_021f716c = { 0, 0, 0 };
// pokebw2's sCameraUpVector
static const VecFx32 data_ov215_021f7154 = { 0, FX32_ONE, 0 };

// pokebw2's sBGSetups
static const EggDemoBGSetup data_ov215_021f71f4[BG_COUNT] = {
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 2,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 3,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 5,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
};

// pokebw2's sLights
static const Light data_ov215_021f71a4[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov215_021f7178 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
// pokebw2's sClActSetup
static const ClActSysSetup data_ov215_021f7188 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// The 3D system calls this once it is set up
// pokebw2's EggDemoGraphic_G3DInit
void func_ov215_021f6328(void) {
    u32 i;

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    G2_SetBG0Priority(0);
    G3X_SetShading(GX_SHADING_HIGHLIGHT);
    G3X_AntiAlias(FALSE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, 0, 0, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    for (i = 0; i < NELEMS(data_ov215_021f71a4); i++) {
        GFL_G3DSysLightSet(i, &data_ov215_021f71a4[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

// pokebw2's EggDemoGraphic_Create
EggDemoGraphic *func_ov215_021f63cc(u32 layout, HeapID heapId) {
    EggDemoGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(EggDemoGraphic), FALSE, "egg_demo_graphic.c", 466);

    MI_CpuFill8(graphic, 0, sizeof(EggDemoGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov215_021f71c4);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov215_021f651c(&graphic->bg, heapId);
    func_ov215_021f65c0(&graphic->clact, &data_ov215_021f71c4, heapId);
    func_ov215_021f6630(&graphic->g3d, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov215_021f6508, graphic, 0);
    return graphic;
}

void func_ov215_021f6474(EggDemoGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov215_021f669c(&graphic->g3d);
    func_ov215_021f6600(&graphic->clact);
    func_ov215_021f657c(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(EggDemoGraphic));
    GFL_HeapFree(graphic);
}

void func_ov215_021f64d0(EggDemoGraphic *graphic) {
    func_ov215_021f661c(&graphic->clact);
    func_ov215_021f65b4(&graphic->bg);
}

void func_ov215_021f64e4(EggDemoGraphic *graphic) {
    func_ov215_021f66ac(&graphic->g3d);
}

void func_ov215_021f64f0(EggDemoGraphic *graphic) {
    func_ov215_021f66c0(&graphic->g3d);
}

ClActUnit *func_ov215_021f64fc(EggDemoGraphic *graphic) {
    return func_ov215_021f662c(&graphic->clact);
}

void func_ov215_021f6508(TCB *tcb, void *data) {
    EggDemoGraphic *graphic = data;

    func_ov215_021f65b8(&graphic->bg);
    func_ov215_021f6624(&graphic->clact);
}

// pokebw2's EggDemoBG_Init
void func_ov215_021f651c(EggDemoBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(EggDemoBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov215_021f7178);
    for (i = 0; i < NELEMS(data_ov215_021f71f4); i++) {
        GFL_BGSysCreateBG(data_ov215_021f71f4[i].bg, &data_ov215_021f71f4[i].setup, data_ov215_021f71f4[i].mode);
        GFL_BGSysClearBG(data_ov215_021f71f4[i].bg);
        GFL_BGSysSetBGEnabled(data_ov215_021f71f4[i].bg, data_ov215_021f71f4[i].enabled);
    }
}

// pokebw2's EggDemoBG_Free
void func_ov215_021f657c(EggDemoBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov215_021f71f4); i++) {
        GFL_BGSysReleaseBG(data_ov215_021f71f4[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(EggDemoBG));
}

// pokebw2's EggDemoBG_Update
void func_ov215_021f65b4(EggDemoBG *bg) {
}

// pokebw2's EggDemoBG_VBlank
void func_ov215_021f65b8(EggDemoBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's EggDemoClAct_Init
void func_ov215_021f65c0(EggDemoClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(EggDemoClAct));
    ClActSys_Create(&data_ov215_021f7188, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's EggDemoClAct_Free
void func_ov215_021f6600(EggDemoClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(EggDemoClAct));
}

// pokebw2's EggDemoClAct_Update
void func_ov215_021f661c(EggDemoClAct *clact) {
    func_0204a600();
}

// pokebw2's EggDemoClAct_VBlank
void func_ov215_021f6624(EggDemoClAct *clact) {
    func_0204a648();
}

// pokebw2's EggDemoClAct_GetUnit
ClActUnit *func_ov215_021f662c(EggDemoClAct *clact) {
    return clact->unit;
}

// pokebw2's EggDemoG3dSys_Init
void func_ov215_021f6630(EggDemoG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 2, 0, 1, 0, heapId, func_ov215_021f6328);
    // A 40 degree field of view
    g3d->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)),
                                      FX_CosIdx(DEG_TO_IDX(20)), FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024), 0,
                                      &data_ov215_021f7160, &data_ov215_021f7154, &data_ov215_021f716c, heapId);
}

// pokebw2's EggDemoG3dSys_Free
void func_ov215_021f669c(EggDemoG3dSys *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}

// pokebw2's EggDemoG3dSys_Begin
void func_ov215_021f66ac(EggDemoG3dSys *g3d) {
    GFL_G3DSysReset();
    GFL_G3DCameraFlush(g3d->camera);
    GFL_G3DSysMtxViewFlush();
}

// pokebw2's EggDemoG3dSys_End
void func_ov215_021f66c0(EggDemoG3dSys *g3d) {
    GFL_G3DSysReqSwapBuffers();
}
