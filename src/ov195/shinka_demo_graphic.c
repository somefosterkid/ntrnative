// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov284/shinka_demo_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "demo/shinka_demo.h"
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

// The evolution demo's BGs, cell actors and 3D system. The BGs of the sub screen are only created on request

#define BG_COUNT 6
// The last BG of the main screen and the first of the sub screen
#define BG_MAIN_LAST 3
#define BG_SUB_FIRST 4

typedef struct {
    u32 unk0;
} ShinkaDemoBG;

typedef struct {
    ClActUnit *unit;
} ShinkaDemoClAct;

typedef struct {
    G3DCamera *camera;
} ShinkaDemoG3dSys;

struct ShinkaDemoGraphic {
    ShinkaDemoBG bg;
    ShinkaDemoClAct clact;
    ShinkaDemoG3dSys g3d;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} ShinkaDemoBGSetup;

static void func_ov195_02205aac(TCB *tcb, void *data);
static void func_ov195_02205ac0(ShinkaDemoBG *bg, HeapID heapId);
static void func_ov195_02205b24(ShinkaDemoBG *bg);
static void func_ov195_02205b60(ShinkaDemoBG *bg);
static void func_ov195_02205b64(ShinkaDemoBG *bg);
static void func_ov195_02205b6c(ShinkaDemoClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov195_02205bac(ShinkaDemoClAct *clact);
static void func_ov195_02205bc8(ShinkaDemoClAct *clact);
static void func_ov195_02205bd0(ShinkaDemoClAct *clact);
static ClActUnit *func_ov195_02205bd8(ShinkaDemoClAct *clact);
static void func_ov195_02205bdc(ShinkaDemoG3dSys *g3d, HeapID heapId);
static void func_ov195_02205c48(ShinkaDemoG3dSys *g3d);
static void func_ov195_02205c58(ShinkaDemoG3dSys *g3d);
static void func_ov195_02205c6c(ShinkaDemoG3dSys *g3d);

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov195_02208cec = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E,  GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_BD, GX_VRAM_TEXPLTT_01_FG,   GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sCameraPosition
static const VecFx32 data_ov195_02208c88 = { 0, FX32_CONST(10), FX32_CONST(100) };
// pokebw2's sCameraTarget
static const VecFx32 data_ov195_02208c94 = { 0, 0, 0 };
// pokebw2's sCameraUpVector
static const VecFx32 data_ov195_02208c7c = { 0, FX32_ONE, 0 };

// pokebw2's sBGSetups
static const ShinkaDemoBGSetup data_ov195_02208d1c[BG_COUNT] = {
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
static const Light data_ov195_02208ccc[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov195_02208ca0 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
// pokebw2's sClActSetup
static const ClActSysSetup data_ov195_02208cb0 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// The 3D system calls this once it is set up
// pokebw2's ShinkaDemoGraphic_G3DInit
static void func_ov195_02205854(void) {
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
    for (i = 0; i < NELEMS(data_ov195_02208ccc); i++) {
        GFL_G3DSysLightSet(i, &data_ov195_02208ccc[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

// pokebw2's ShinkaDemoGraphic_Create
ShinkaDemoGraphic *func_ov195_022058f8(u32 layout, HeapID heapId) {
    ShinkaDemoGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(ShinkaDemoGraphic), FALSE, "shinka_demo_graphic.c", 479);

    MI_CpuFill8(graphic, 0, sizeof(ShinkaDemoGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov195_02208cec);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov195_02205ac0(&graphic->bg, heapId);
    func_ov195_02205b6c(&graphic->clact, &data_ov195_02208cec, heapId);
    func_ov195_02205bdc(&graphic->g3d, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov195_02205aac, graphic, 0);
    return graphic;
}

// pokebw2's ShinkaDemoGraphic_Free
void func_ov195_022059a0(ShinkaDemoGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov195_02205c48(&graphic->g3d);
    func_ov195_02205bac(&graphic->clact);
    func_ov195_02205b24(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(ShinkaDemoGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's ShinkaDemoGraphic_Update
void func_ov195_022059fc(ShinkaDemoGraphic *graphic) {
    func_ov195_02205bc8(&graphic->clact);
    func_ov195_02205b60(&graphic->bg);
}

// pokebw2's ShinkaDemoGraphic_Begin3D
void func_ov195_02205a10(ShinkaDemoGraphic *graphic) {
    func_ov195_02205c58(&graphic->g3d);
}

// pokebw2's ShinkaDemoGraphic_End3D
void func_ov195_02205a1c(ShinkaDemoGraphic *graphic) {
    func_ov195_02205c6c(&graphic->g3d);
}

// pokebw2's ShinkaDemoGraphic_GetClActUnit
ClActUnit *func_ov195_02205a28(ShinkaDemoGraphic *graphic) {
    return func_ov195_02205bd8(&graphic->clact);
}

// pokebw2's ShinkaDemoGraphic_InitSubBG
void func_ov195_02205a34(ShinkaDemoGraphic *graphic) {
    u32 i;

    GFL_BGSysSetLCDConfigForEngine(&data_ov195_02208ca0, BGSYS_ENGINE_SUB);
    for (i = 0; i < NELEMS(data_ov195_02208d1c); i++) {
        if (data_ov195_02208d1c[i].bg >= BG_SUB_FIRST) {
            GFL_BGSysCreateBG(data_ov195_02208d1c[i].bg, &data_ov195_02208d1c[i].setup, data_ov195_02208d1c[i].mode);
            GFL_BGSysClearBG(data_ov195_02208d1c[i].bg);
            GFL_BGSysSetBGEnabled(data_ov195_02208d1c[i].bg, data_ov195_02208d1c[i].enabled);
        }
    }
}

// pokebw2's ShinkaDemoGraphic_FreeSubBG
void func_ov195_02205a84(ShinkaDemoGraphic *graphic) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov195_02208d1c); i++) {
        if (data_ov195_02208d1c[i].bg >= BG_SUB_FIRST) {
            GFL_BGSysReleaseBG(data_ov195_02208d1c[i].bg);
        }
    }
}

// pokebw2's ShinkaDemoGraphic_VBlank
static void func_ov195_02205aac(TCB *tcb, void *data) {
    ShinkaDemoGraphic *graphic = data;

    func_ov195_02205b64(&graphic->bg);
    func_ov195_02205bd0(&graphic->clact);
}

// pokebw2's ShinkaDemoBG_Init
static void func_ov195_02205ac0(ShinkaDemoBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(ShinkaDemoBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov195_02208ca0);
    for (i = 0; i < NELEMS(data_ov195_02208d1c); i++) {
        if (data_ov195_02208d1c[i].bg <= BG_MAIN_LAST) {
            GFL_BGSysCreateBG(data_ov195_02208d1c[i].bg, &data_ov195_02208d1c[i].setup, data_ov195_02208d1c[i].mode);
            GFL_BGSysClearBG(data_ov195_02208d1c[i].bg);
            GFL_BGSysSetBGEnabled(data_ov195_02208d1c[i].bg, data_ov195_02208d1c[i].enabled);
        }
    }
}

// pokebw2's ShinkaDemoBG_Free
static void func_ov195_02205b24(ShinkaDemoBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov195_02208d1c); i++) {
        if (data_ov195_02208d1c[i].bg <= BG_MAIN_LAST) {
            GFL_BGSysReleaseBG(data_ov195_02208d1c[i].bg);
        }
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(ShinkaDemoBG));
}

// pokebw2's ShinkaDemoBG_Update
static void func_ov195_02205b60(ShinkaDemoBG *bg) {
}

// pokebw2's ShinkaDemoBG_VBlank
static void func_ov195_02205b64(ShinkaDemoBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's ShinkaDemoClAct_Init
static void func_ov195_02205b6c(ShinkaDemoClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(ShinkaDemoClAct));
    ClActSys_Create(&data_ov195_02208cb0, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's ShinkaDemoClAct_Free
static void func_ov195_02205bac(ShinkaDemoClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(ShinkaDemoClAct));
}

// pokebw2's ShinkaDemoClAct_Update
static void func_ov195_02205bc8(ShinkaDemoClAct *clact) {
    func_0204a600();
}

// pokebw2's ShinkaDemoClAct_VBlank
static void func_ov195_02205bd0(ShinkaDemoClAct *clact) {
    func_0204a648();
}

// pokebw2's ShinkaDemoClAct_GetUnit
static ClActUnit *func_ov195_02205bd8(ShinkaDemoClAct *clact) {
    return clact->unit;
}

// pokebw2's ShinkaDemoG3dSys_Init
static void func_ov195_02205bdc(ShinkaDemoG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 2, 0, 1, 0, heapId, func_ov195_02205854);
    // A 40 degree field of view
    g3d->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)),
                                      FX_CosIdx(DEG_TO_IDX(20)), FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024), 0,
                                      &data_ov195_02208c88, &data_ov195_02208c7c, &data_ov195_02208c94, heapId);
}

// pokebw2's ShinkaDemoG3dSys_Free
static void func_ov195_02205c48(ShinkaDemoG3dSys *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}

// pokebw2's ShinkaDemoG3dSys_Begin
static void func_ov195_02205c58(ShinkaDemoG3dSys *g3d) {
    GFL_G3DSysReset();
    GFL_G3DCameraFlush(g3d->camera);
    GFL_G3DSysMtxViewFlush();
}

// pokebw2's ShinkaDemoG3dSys_End
static void func_ov195_02205c6c(ShinkaDemoG3dSys *g3d) {
    GFL_G3DSysReqSwapBuffers();
}
