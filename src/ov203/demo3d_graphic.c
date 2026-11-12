// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov293/demo3d_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/demo3d/demo3d_graphic.h"
#include "app/demo3d/demo3d_data.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The BGs, cell actors and 3D system of the 3D demos, named after the ROM's "demo3d_graphic.c". The file starts at the
// 3D system's set-up callback, as its siblings of the same template do (btl_rec_sel_graphic.c, psel_graphic.c). A
// demo that draws 3D on both screens has neither BGs nor cell actors

#define BG_COUNT 4
#define LIGHT_COUNT 4

typedef struct {
    u32 unk0;
} Demo3DBG;

typedef struct {
    ClActUnit *unit;
} Demo3DClAct;

typedef struct {
    u32 unk0;
    G3DLight *lights;
} Demo3DG3dSys;

struct Demo3DGraphic {
    Demo3DBG bg;
    Demo3DClAct clact;
    Demo3DG3dSys g3d;
    TCB *vblankTask;
    BOOL dual3D;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} Demo3DBGSetup;

static void func_ov203_021bdd40(void);
static void func_ov203_021bdfa8(TCB *tcb, void *data);
static void func_ov203_021bdfc0(Demo3DBG *bg, HeapID heapId);
static void func_ov203_021be02c(Demo3DBG *bg);
static void func_ov203_021be078(Demo3DBG *bg);
static void func_ov203_021be07c(Demo3DBG *bg);
static void func_ov203_021be084(Demo3DClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov203_021be0c4(Demo3DClAct *clact);
static void func_ov203_021be0e0(Demo3DClAct *clact);
static void func_ov203_021be0e8(Demo3DClAct *clact);
static ClActUnit *func_ov203_021be0f0(Demo3DClAct *clact);
static void func_ov203_021be0f4(Demo3DG3dSys *g3d, HeapID heapId);
static void func_ov203_021be12c(Demo3DG3dSys *g3d);
static void func_ov203_021be13c(Demo3DG3dSys *g3d);
static void func_ov203_021be148(Demo3DG3dSys *g3d);

static const LightSetup sDemo3DLightSetups[LIGHT_COUNT] = {
    { 0, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 1, { { FX16_ONE - 1, -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 2, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
    { 3, { { -(FX16_ONE - 1), -(FX16_ONE - 1), -(FX16_ONE - 1) }, GX_RGB(31, 31, 31) } },
};

static const LightSetupList data_ov203_021bfb48 = { sDemo3DLightSetups, LIGHT_COUNT };

static const BGSysLCDConfig data_ov203_021bfb50 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
static const ClActSysSetup data_ov203_021bfb60 = { 0, 0, 0, 512, 4, 124, 4, 112, 0, 32, 32, 32, 32, 16, 16 };

static const Light data_ov203_021bfb7c[LIGHT_COUNT] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// The VRAM banks of a demo with 3D on the upper screen only, and of one with 3D on both
static const BGSysVRAMConfig data_ov203_021bfbc4[] = {
    {
        GX_VRAM_BG_16_F, GX_VRAM_BGEXTPLTT_NONE, GX_VRAM_SUB_BG_128_C, GX_VRAM_SUB_BGEXTPLTT_NONE,
        GX_VRAM_OBJ_16_G, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I, GX_VRAM_SUB_OBJEXTPLTT_NONE,
        GX_VRAM_TEX_01_AB, GX_VRAM_TEXPLTT_0123_E, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K,
    },
    {
        GX_VRAM_BG_16_F, GX_VRAM_BGEXTPLTT_NONE, GX_VRAM_SUB_BG_32_H, GX_VRAM_SUB_BGEXTPLTT_NONE,
        GX_VRAM_OBJ_16_G, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I, GX_VRAM_SUB_OBJEXTPLTT_NONE,
        GX_VRAM_TEX_01_AB, GX_VRAM_TEXPLTT_0123_E, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K,
    },
};

// BG1 of the main screen and BG0 to BG2 of the sub screen, all hidden until a command or the demo's own code shows
// one
static const Demo3DBGSetup data_ov203_021bfc24[BG_COUNT] = {
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3800, GX_BG_CHARBASE_0x00000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      FALSE },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      FALSE },
    { 5,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      FALSE },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x10000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      FALSE },
};

// The 3D system calls this once it is set up
// pokebw2's Demo3DGraphic_G3DInit
static void func_ov203_021bdd40(void) {
    u32 i;

    GFL_BGSysSet3DBGPriority(0);
    G3X_SetShading(GX_SHADING_HIGHLIGHT);
    G3X_AntiAlias(FALSE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, 0, 0, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    for (i = 0; i < NELEMS(data_ov203_021bfb7c); i++) {
        GFL_G3DSysLightSet(i, &data_ov203_021bfb7c[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

// pokebw2's Demo3DGraphic_Create
Demo3DGraphic *func_ov203_021bddd8(u32 layout, u32 demoId, HeapID heapId) {
    Demo3DGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(Demo3DGraphic), FALSE, "demo3d_graphic.c", 346);

    MI_CpuFill8(graphic, 0, sizeof(Demo3DGraphic));
    graphic->dual3D = func_ov203_021be938(demoId);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov203_021bfbc4[graphic->dual3D]);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    if (!graphic->dual3D) {
        func_ov203_021bdfc0(&graphic->bg, heapId);
        func_ov203_021be084(&graphic->clact, &data_ov203_021bfbc4[0], heapId);
        graphic->vblankTask = GFL_VBlankTCBAdd(func_ov203_021bdfa8, graphic, 0);
    }
    func_ov203_021be0f4(&graphic->g3d, heapId);
    return graphic;
}

// pokebw2's Demo3DGraphic_Free
void func_ov203_021bde98(Demo3DGraphic *graphic) {
    if (!graphic->dual3D) {
        GFL_TCBRemove(graphic->vblankTask);
    }
    func_ov203_021be12c(&graphic->g3d);
    if (!graphic->dual3D) {
        func_ov203_021be0c4(&graphic->clact);
        func_ov203_021be02c(&graphic->bg);
    }
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(Demo3DGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's Demo3DGraphic_Update
void func_ov203_021bdf00(Demo3DGraphic *graphic) {
    if (!graphic->dual3D) {
        func_ov203_021be0e0(&graphic->clact);
        func_ov203_021be078(&graphic->bg);
    }
}

// pokebw2's Demo3DGraphic_Begin3D
void func_ov203_021bdf18(Demo3DGraphic *graphic) {
    func_ov203_021be13c(&graphic->g3d);
}

// pokebw2's Demo3DGraphic_End3D
void func_ov203_021bdf24(Demo3DGraphic *graphic) {
    func_ov203_021be148(&graphic->g3d);
}

// pokebw2's Demo3DGraphic_GetClActUnit
ClActUnit *func_ov203_021bdf30(Demo3DGraphic *graphic) {
    return func_ov203_021be0f0(&graphic->clact);
}

// pokebw2's Demo3DGraphic_SetLightColors
void func_ov203_021bdf3c(Demo3DGraphic *graphic, const GXRgb *colors, u32 unused) {
    Demo3DG3dSys *g3d = &graphic->g3d;

    GFL_G3DLightSetColor(g3d->lights, 0, &colors[0]);
    GFL_G3DLightSetColor(g3d->lights, 1, &colors[1]);
    GFL_G3DLightSetColor(g3d->lights, 2, &colors[2]);
    GFL_G3DLightSetColor(g3d->lights, 3, &colors[3]);
    GFL_G3DLightFlush(g3d->lights);
}

// pokebw2's Demo3DGraphic_SetLightColor
void func_ov203_021bdf74(Demo3DGraphic *graphic, u8 light, GXRgb color) {
    GFL_G3DLightSetColor(graphic->g3d.lights, light, &color);
}

// pokebw2's Demo3DGraphic_SetLightVector
void func_ov203_021bdf88(Demo3DGraphic *graphic, u8 light, const VecFx16 *direction) {
    VecFx16 normal;

    VEC_Fx16Normalize(direction, &normal);
    GFL_G3DLightSetDirVector(graphic->g3d.lights, light, &normal);
}

// pokebw2's Demo3DGraphic_VBlank
static void func_ov203_021bdfa8(TCB *tcb, void *data) {
    Demo3DGraphic *graphic = data;

    if (!graphic->dual3D) {
        func_ov203_021be07c(&graphic->bg);
        func_ov203_021be0e8(&graphic->clact);
    }
}

// pokebw2's Demo3DBG_Init
static void func_ov203_021bdfc0(Demo3DBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(Demo3DBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov203_021bfb50);
    for (i = 0; i < NELEMS(data_ov203_021bfc24); i++) {
        GFL_BGSysCreateBG(data_ov203_021bfc24[i].bg, &data_ov203_021bfc24[i].setup, data_ov203_021bfc24[i].mode);
        GFL_BGSysClearBG(data_ov203_021bfc24[i].bg);
        GFL_BGSysSetBGEnabled(data_ov203_021bfc24[i].bg, data_ov203_021bfc24[i].enabled);
    }
    GFL_BGSysFillChar(1, 0, 1, 0);
}

// pokebw2's Demo3DBG_Free
static void func_ov203_021be02c(Demo3DBG *bg) {
    u32 i;

    GFL_BGSysFreeFilledChar(1, 1, 0);
    for (i = 0; i < NELEMS(data_ov203_021bfc24); i++) {
        GFL_BGSysSetBGEnabled(data_ov203_021bfc24[i].bg, FALSE);
        GFL_BGSysReleaseBG(data_ov203_021bfc24[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(Demo3DBG));
}

// pokebw2's Demo3DBG_Update
static void func_ov203_021be078(Demo3DBG *bg) {
}

// pokebw2's Demo3DBG_VBlank
static void func_ov203_021be07c(Demo3DBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's Demo3DClAct_Init
static void func_ov203_021be084(Demo3DClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(Demo3DClAct));
    ClActSys_Create(&data_ov203_021bfb60, vramConfig, heapId);
    clact->unit = func_0204b100(32, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's Demo3DClAct_Free
static void func_ov203_021be0c4(Demo3DClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(Demo3DClAct));
}

// pokebw2's Demo3DClAct_Update
static void func_ov203_021be0e0(Demo3DClAct *clact) {
    func_0204a600();
}

// pokebw2's Demo3DClAct_VBlank
static void func_ov203_021be0e8(Demo3DClAct *clact) {
    func_0204a648();
}

// pokebw2's Demo3DClAct_GetUnit
static ClActUnit *func_ov203_021be0f0(Demo3DClAct *clact) {
    return clact->unit;
}

// pokebw2's Demo3DG3dSys_Init
static void func_ov203_021be0f4(Demo3DG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 2, FALSE, 4, 0, heapId, func_ov203_021bdd40);
    g3d->lights = GFL_G3DLightCreate(&data_ov203_021bfb48, heapId);
    GFL_G3DLightFlush(g3d->lights);
}

// pokebw2's Demo3DG3dSys_Free
static void func_ov203_021be12c(Demo3DG3dSys *g3d) {
    GFL_G3DLightFree(g3d->lights);
    GFL_G3DSysFree();
}

// pokebw2's Demo3DG3dSys_Begin
static void func_ov203_021be13c(Demo3DG3dSys *g3d) {
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
}

// pokebw2's Demo3DG3dSys_End
static void func_ov203_021be148(Demo3DG3dSys *g3d) {
    GFL_G3DSysReqSwapBuffers();
}
