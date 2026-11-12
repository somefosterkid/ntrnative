// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov197/mystery_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mystery/mystery_graphic.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/particle.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// Mystery Gift's BGs, cell actors and 3D system, which it turns on for the effect of a received gift. Our names; swan
// has none for this overlay

typedef struct {
    u32 unk0;
} MysteryGraphicBG;

typedef struct {
    ClActUnit *unit;
    ClActRenderer *renderer;
} MysteryGraphicClAct;

typedef struct {
    G3DCamera *camera;
    BOOL active;
} MysteryG3dSys;

struct MysteryGraphic {
    MysteryGraphicBG bg;
    MysteryGraphicClAct clact;
    MysteryG3dSys g3d;
    TCB *vblankTask;
    u16 heapId;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} MysteryBGSetup;

static void func_ov121_021d9580(void);
static void func_ov121_021d97b4(TCB *tcb, void *data);
static void func_ov121_021d97c8(MysteryGraphicBG *bg, HeapID heapId);
static void func_ov121_021d9828(MysteryGraphicBG *bg);
static void func_ov121_021d9860(MysteryGraphicBG *bg);
static void func_ov121_021d9864(MysteryGraphicBG *bg);
static void func_ov121_021d986c(MysteryGraphicClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov121_021d98c0(MysteryGraphicClAct *clact);
static void func_ov121_021d98e0(MysteryGraphicClAct *clact);
static void func_ov121_021d98e8(MysteryGraphicClAct *clact);
static ClActUnit *func_ov121_021d98f0(MysteryGraphicClAct *clact);
static void func_ov121_021d98f4(MysteryG3dSys *g3d, HeapID heapId);
static void func_ov121_021d9960(MysteryG3dSys *g3d);
static void func_ov121_021d9974(MysteryG3dSys *g3d);
static void func_ov121_021d9994(MysteryG3dSys *g3d);

// pokebw2's sCameraUpVector
static const VecFx32 data_ov121_021df58c = { 0, FX32_ONE, 0 };
// pokebw2's sCameraTarget
static const VecFx32 data_ov121_021df580 = { 0, 0, 0 };
// pokebw2's sCameraPosition
static const VecFx32 data_ov121_021df574 = { 0, 0, FX32_CONST(70) };

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov121_021df598 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
// pokebw2's sClActSetup
static const ClActSysSetup data_ov121_021df5a8 = { 0, 0, 0, 192, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// pokebw2's sLights
static const Light data_ov121_021df5e4[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sSurfaceSetups
static const ClActSurfaceSetup data_ov121_021df5c4[] = {
    { 0, 0, 256, 384, 0, 0 },
    { 0, 512, 256, 192, 1, 0 },
};

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov121_021df604 = {
    GX_VRAM_BG_128_A, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_B,  GX_VRAM_TEXPLTT_01_FG,   GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sBGSetups
static const MysteryBGSetup data_ov121_021df634[7] = {
    { 0,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 2,
      { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 3,
      { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x0c000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 7,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// The 3D system calls this once it is set up
// pokebw2's MysteryGraphic_G3DInit
static void func_ov121_021d9580(void) {
    u32 i;

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, FALSE);
    G2_SetBG0Priority(0);
    G3X_SetShading(GX_SHADING_HIGHLIGHT);
    G3X_AntiAlias(FALSE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, 0, 0, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    for (i = 0; i < NELEMS(data_ov121_021df5e4); i++) {
        GFL_G3DSysLightSet(i, &data_ov121_021df5e4[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

// pokebw2's MysteryGraphic_Create
MysteryGraphic *func_ov121_021d9624(u32 layout, HeapID heapId) {
    MysteryGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(MysteryGraphic), FALSE, "mystery_graphic.c", 448);

    MI_CpuFill8(graphic, 0, sizeof(MysteryGraphic));
    graphic->heapId = heapId;
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov121_021df604);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov121_021d97c8(&graphic->bg, heapId);
    func_ov121_021d986c(&graphic->clact, &data_ov121_021df604, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov121_021d97b4, graphic, 0);
    return graphic;
}

// pokebw2's MysteryGraphic_Delete
void func_ov121_021d96c4(MysteryGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov121_021d98c0(&graphic->clact);
    func_ov121_021d9828(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(MysteryGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's MysteryGraphic_Update
void func_ov121_021d9718(MysteryGraphic *graphic) {
    func_ov121_021d98e0(&graphic->clact);
    func_ov121_021d9860(&graphic->bg);
}

// pokebw2's MysteryGraphic_BeginFrame3D
void func_ov121_021d972c(MysteryGraphic *graphic) {
    func_ov121_021d9974(&graphic->g3d);
}

// pokebw2's MysteryGraphic_EndFrame3D
void func_ov121_021d9738(MysteryGraphic *graphic) {
    func_ov121_021d9994(&graphic->g3d);
}

// pokebw2's MysteryGraphic_GetClactUnit
ClActUnit *func_ov121_021d9744(MysteryGraphic *graphic) {
    return func_ov121_021d98f0(&graphic->clact);
}

// pokebw2's MysteryGraphic_Start3D
void func_ov121_021d9750(MysteryGraphic *graphic) {
    BGSysLCDConfig lcdConfig;

    GFL_BGSysSetBGEnabled(0, FALSE);
    lcdConfig = data_ov121_021df598;
    lcdConfig.bg0Is3D = GX_BG0_AS_3D;
    GFL_BGSysSetLCDConfig(&lcdConfig);
    func_ov121_021d98f4(&graphic->g3d, graphic->heapId);
    func_02050b80(graphic->heapId);
}

// pokebw2's MysteryGraphic_End3D
void func_ov121_021d9790(MysteryGraphic *graphic) {
    GFL_BGSysSetBGEnabled(0, FALSE);
    func_02050ed4();
    func_ov121_021d9960(&graphic->g3d);
    GFL_BGSysSetLCDConfig(&data_ov121_021df598);
}

// pokebw2's MysteryGraphic_VBlank
static void func_ov121_021d97b4(TCB *tcb, void *data) {
    MysteryGraphic *graphic = data;

    func_ov121_021d9864(&graphic->bg);
    func_ov121_021d98e8(&graphic->clact);
}

// pokebw2's MysteryGraphicBG_Init
static void func_ov121_021d97c8(MysteryGraphicBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(MysteryGraphicBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov121_021df598);
    for (i = 0; i < NELEMS(data_ov121_021df634); i++) {
        GFL_BGSysCreateBG(data_ov121_021df634[i].bg, &data_ov121_021df634[i].setup, data_ov121_021df634[i].mode);
        GFL_BGSysClearBG(data_ov121_021df634[i].bg);
        GFL_BGSysSetBGEnabled(data_ov121_021df634[i].bg, data_ov121_021df634[i].enabled);
    }
}

// pokebw2's MysteryGraphicBG_Free
static void func_ov121_021d9828(MysteryGraphicBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov121_021df634); i++) {
        GFL_BGSysReleaseBG(data_ov121_021df634[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(MysteryGraphicBG));
}

// pokebw2's MysteryGraphicBG_Update
static void func_ov121_021d9860(MysteryGraphicBG *bg) {
}

// pokebw2's MysteryGraphicBG_VBlank
static void func_ov121_021d9864(MysteryGraphicBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's MysteryGraphicClAct_Init
static void func_ov121_021d986c(MysteryGraphicClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(MysteryGraphicClAct));
    ClActSys_Create(&data_ov121_021df5a8, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    clact->renderer = func_0204b034(data_ov121_021df5c4, NELEMS(data_ov121_021df5c4), heapId);
    func_0204b258(clact->unit, clact->renderer);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's MysteryGraphicClAct_Free
static void func_ov121_021d98c0(MysteryGraphicClAct *clact) {
    func_0204b084(clact->renderer);
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(MysteryGraphicClAct));
}

// pokebw2's MysteryGraphicClAct_Update
static void func_ov121_021d98e0(MysteryGraphicClAct *clact) {
    func_0204a600();
}

// pokebw2's MysteryGraphicClAct_VBlank
static void func_ov121_021d98e8(MysteryGraphicClAct *clact) {
    func_0204a648();
}

// pokebw2's MysteryGraphicClAct_GetUnit
static ClActUnit *func_ov121_021d98f0(MysteryGraphicClAct *clact) {
    return clact->unit;
}

// pokebw2's MysteryG3dSys_Init
static void func_ov121_021d98f4(MysteryG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 1, 0, 1, 0, heapId, func_ov121_021d9580);
    g3d->camera =
        GFL_G3DCameraCreate(G3DCAM_PROJECTION_ORTHO, FX32_CONST(24), -FX32_CONST(24), -FX32_CONST(32), FX32_CONST(32),
                            FX32_ONE, FX32_CONST(1024), 0, &data_ov121_021df574, &data_ov121_021df58c, &data_ov121_021df580, heapId);
    g3d->active = TRUE;
}

// pokebw2's MysteryG3dSys_Free
static void func_ov121_021d9960(MysteryG3dSys *g3d) {
    g3d->active = FALSE;
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}

// pokebw2's MysteryG3dSys_Begin
static void func_ov121_021d9974(MysteryG3dSys *g3d) {
    if (g3d->active) {
        GFL_G3DSysReset();
        GFL_G3DCameraFlush(g3d->camera);
        GFL_G3DSysMtxViewFlush();
        func_02050be8();
    }
}

// pokebw2's MysteryG3dSys_End
static void func_ov121_021d9994(MysteryG3dSys *g3d) {
    if (g3d->active) {
        GFL_G3DSysReqSwapBuffers();
    }
}
