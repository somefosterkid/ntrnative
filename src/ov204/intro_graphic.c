// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov294/intro_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "demo/intro.h"
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

// The intro's BGs, cell actors and 3D system. Once the player or the rival has been named, the intro sets them up
// differently

typedef struct {
    u32 unk0;
} IntroBG;

typedef struct {
    ClActUnit *unit;
} IntroClAct;

typedef struct {
    G3DCamera *camera;
    G3DLight *light;
} IntroG3dSys;

struct IntroGraphic {
    IntroBG bg;
    IntroClAct clact;
    IntroG3dSys g3d;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} IntroBGSetup;

static void func_ov204_021bdd84(TCB *tcb, void *data);
static void func_ov204_021bdd98(IntroBG *bg, HeapID heapId);
static void func_ov204_021bddf8(IntroBG *bg);
static void func_ov204_021bde10(IntroBG *bg);
static void func_ov204_021bde28(IntroBG *bg);
static void func_ov204_021bde30(IntroBG *bg, HeapID heapId);
static void func_ov204_021bdf34(IntroClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov204_021bdf74(IntroClAct *clact);
static void func_ov204_021bdf90(IntroClAct *clact);
static void func_ov204_021bdf98(IntroClAct *clact);
static ClActUnit *func_ov204_021bdfa0(IntroClAct *clact);
static void func_ov204_021bdfa4(IntroG3dSys *g3d, HeapID heapId);
static void func_ov204_021be03c(IntroG3dSys *g3d, HeapID heapId);
static void func_ov204_021be0d4(IntroG3dSys *g3d);
void func_ov204_021be0ec(IntroG3dSys *g3d);
void func_ov204_021be100(IntroG3dSys *g3d);

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov204_021bfe1c = {
    GX_VRAM_BG_16_F,       GX_VRAM_BGEXTPLTT_NONE,     GX_VRAM_SUB_BG_32_H,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_16_G,      GX_VRAM_OBJEXTPLTT_NONE,    GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0123_ABCD, GX_VRAM_TEXPLTT_0123_E,     GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sVRAMConfigNamed
static const BGSysVRAMConfig data_ov204_021bfe4c = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,      GX_VRAM_SUB_BG_32_H,         GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE,     GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_CD, GX_VRAM_TEXPLTT_0123_E,      GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sCameraPosition
static const VecFx32 data_ov204_021bfd68 = { 0, FX32_CONST(0.043), FX32_CONST(70) };
// pokebw2's sCameraUpVector
static const VecFx32 data_ov204_021bfd5c = { 0, FX32_CONST(2.6), 0 };
// pokebw2's sCameraTarget
static const VecFx32 data_ov204_021bfd74 = { 0, FX32_ONE, 0 };

// pokebw2's sLights
static const Light data_ov204_021bfdac[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sBGSetups
static const IntroBGSetup data_ov204_021bff1c[] = {
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x00000, 0x4000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 3,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x00000, 0x4000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
};

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov204_021bfd80 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
// pokebw2's sClActSetup
static const ClActSysSetup data_ov204_021bfd90 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// The 3D system calls this once it is set up
// pokebw2's IntroGraphic_G3DInit
void func_ov204_021bdb70(void) {
    u32 i;

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_HIGHLIGHT);
    G3X_AntiAlias(FALSE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, 0, 0, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    for (i = 0; i < NELEMS(data_ov204_021bfdac); i++) {
        GFL_G3DSysLightSet(i, &data_ov204_021bfdac[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

IntroGraphic *func_ov204_021bdc18(u32 layout, u32 mode, HeapID heapId) {
    IntroGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(IntroGraphic), FALSE, "intro_graphic.c", 406);

    MI_CpuFill8(graphic, 0, sizeof(IntroGraphic));
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    if (mode == 7) {
        GFL_BGSysSetVRAMBanks(&data_ov204_021bfe4c);
        func_ov204_021bde30(&graphic->bg, heapId);
        func_ov204_021bdf34(&graphic->clact, &data_ov204_021bfe1c, heapId);
        func_ov204_021be03c(&graphic->g3d, heapId);
    } else {
        GFL_BGSysSetVRAMBanks(&data_ov204_021bfe1c);
        func_ov204_021bdd98(&graphic->bg, heapId);
        func_ov204_021bdf34(&graphic->clact, &data_ov204_021bfe1c, heapId);
        func_ov204_021bdfa4(&graphic->g3d, heapId);
    }
    func_0201dd60();
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov204_021bdd84, graphic, 0);
    return graphic;
}


// pokebw2's IntroGraphic_Free
void func_ov204_021bdcf0(IntroGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov204_021be0d4(&graphic->g3d);
    func_ov204_021bdf74(&graphic->clact);
    func_ov204_021bddf8(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(IntroGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's IntroGraphic_Update
void func_ov204_021bdd4c(IntroGraphic *graphic) {
    func_ov204_021bdf90(&graphic->clact);
    func_ov204_021bde10(&graphic->bg);
}

// pokebw2's IntroGraphic_Begin3D
void func_ov204_021bdd60(IntroGraphic *graphic) {
    func_ov204_021be0ec(&graphic->g3d);
}

// pokebw2's IntroGraphic_End3D
void func_ov204_021bdd6c(IntroGraphic *graphic) {
    func_ov204_021be100(&graphic->g3d);
}

// pokebw2's IntroGraphic_GetClActUnit
ClActUnit *func_ov204_021bdd78(IntroGraphic *graphic) {
    return func_ov204_021bdfa0(&graphic->clact);
}

// pokebw2's IntroGraphic_VBlank
static void func_ov204_021bdd84(TCB *tcb, void *data) {
    IntroGraphic *graphic = data;

    func_ov204_021bde28(&graphic->bg);
    func_ov204_021bdf98(&graphic->clact);
}

// pokebw2's IntroBG_Init
static void func_ov204_021bdd98(IntroBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(IntroBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov204_021bfd80);
    for (i = 0; i < NELEMS(data_ov204_021bff1c); i++) {
        GFL_BGSysCreateBG(data_ov204_021bff1c[i].bg, &data_ov204_021bff1c[i].setup, data_ov204_021bff1c[i].mode);
        GFL_BGSysClearBG(data_ov204_021bff1c[i].bg);
        GFL_BGSysSetBGEnabled(data_ov204_021bff1c[i].bg, data_ov204_021bff1c[i].enabled);
    }
}

// pokebw2's IntroBG_Free
static void func_ov204_021bddf8(IntroBG *bg) {
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(IntroBG));
}

// Scrolls BG 2 to the right, one pixel each frame
// pokebw2's IntroBG_Update
static void func_ov204_021bde10(IntroBG *bg) {
    if (GFL_BGSysGetEnabledBGsA() & GX_PLANEMASK_BG2) {
        GFL_BGSysMoveBGReq(2, BG_MOVE_RIGHT, 1);
    }
}

// pokebw2's IntroBG_VBlank
static void func_ov204_021bde28(IntroBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's IntroBG_InitNamed
static void func_ov204_021bde30(IntroBG *bg, HeapID heapId) {
    MI_CpuFill8(bg, 0, sizeof(IntroBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov204_021bfd80);
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,
                          GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800,
                          GX_BG_CHARBASE_0x10000, 0x8000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe000,
                          GX_BG_CHARBASE_0x18000, 0x8000, GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800,
                          GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7000,
                          GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE };
        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
    }
    GFL_BGSysSetBGEnabled(1, TRUE);
    GFL_BGSysSetBGEnabled(3, TRUE);
    GFL_BGSysSetBGEnabled(4, TRUE);
    GFL_BGSysSetBGEnabled(6, TRUE);
}

// pokebw2's IntroClAct_Init
static void func_ov204_021bdf34(IntroClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(IntroClAct));
    ClActSys_Create(&data_ov204_021bfd90, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's IntroClAct_Free
static void func_ov204_021bdf74(IntroClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(IntroClAct));
}

// pokebw2's IntroClAct_Update
static void func_ov204_021bdf90(IntroClAct *clact) {
    func_0204a600();
}

// pokebw2's IntroClAct_VBlank
static void func_ov204_021bdf98(IntroClAct *clact) {
    func_0204a648();
}

// pokebw2's IntroClAct_GetUnit
static ClActUnit *func_ov204_021bdfa0(IntroClAct *clact) {
    return clact->unit;
}

// The compiler lays out data by size, breaking ties in an order that depends on where each object is declared. These
// are declared after func_ov204_021bde30's BG setups to keep the original layout
// pokebw2's sLightSetups
static const LightSetup data_ov204_021bfdf4[] = {
    { 0, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 1, { { FX16_ONE - 1, -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 2, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 3, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
};

// pokebw2's sLightSetupList
static const LightSetupList data_ov204_021bfd4c = { data_ov204_021bfdf4, NELEMS(data_ov204_021bfdf4) };

// pokebw2's sLightSetupsNamed
static const LightSetup data_ov204_021bfdcc[] = {
    { 0, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 1, { { FX16_ONE - 1, -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 2, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 3, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
};

// pokebw2's sLightSetupListNamed
static const LightSetupList data_ov204_021bfd54 = { data_ov204_021bfdcc, NELEMS(data_ov204_021bfdcc) };

// pokebw2's IntroG3dSys_Init
static void func_ov204_021bdfa4(IntroG3dSys *g3d, HeapID heapId) {
    fx32 far;
    fx32 near;

    GFL_G3DSysCreate(FALSE, 4, 0, 2, 0, heapId, func_ov204_021bdb70);
    // A 40 degree field of view
    g3d->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)),
                                      FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024), 0, &data_ov204_021bfd68,
                                      &data_ov204_021bfd5c, &data_ov204_021bfd74, heapId);
    far = FX32_CONST(2048);
    near = FX32_CONST(0.1);
    GFL_G3DCameraSetProjectionZFar(g3d->camera, &far);
    GFL_G3DCameraSetProjectionZNear(g3d->camera, &near);
    g3d->light = GFL_G3DLightCreate(&data_ov204_021bfd4c, heapId);
    GFL_G3DLightFlush(g3d->light);
}

// pokebw2's IntroG3dSys_InitNamed
static void func_ov204_021be03c(IntroG3dSys *g3d, HeapID heapId) {
    fx32 far;
    fx32 near;

    GFL_G3DSysCreate(FALSE, 2, 0, 2, 0, heapId, func_ov204_021bdb70);
    g3d->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)),
                                      FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024), 0, &data_ov204_021bfd68,
                                      &data_ov204_021bfd5c, &data_ov204_021bfd74, heapId);
    far = FX32_CONST(2048);
    near = FX32_CONST(0.1);
    GFL_G3DCameraSetProjectionZFar(g3d->camera, &far);
    GFL_G3DCameraSetProjectionZNear(g3d->camera, &near);
    g3d->light = GFL_G3DLightCreate(&data_ov204_021bfd54, heapId);
    GFL_G3DLightFlush(g3d->light);
}

// pokebw2's IntroG3dSys_Free
static void func_ov204_021be0d4(IntroG3dSys *g3d) {
    GFL_G3DLightFree(g3d->light);
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}

void func_ov204_021be0ec(IntroG3dSys *g3d) {
    GFL_G3DCameraFlush(g3d->camera);
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
}

void func_ov204_021be100(IntroG3dSys *g3d) {
    GFL_G3DSysReqSwapBuffers();
}

