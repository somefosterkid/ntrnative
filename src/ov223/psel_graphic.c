// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov316/psel_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/psel_graphic.h"
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
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The BGs, cell actors and 3D system of the starter Pokémon choice, named after the ROM's "psel_graphic.c"

typedef struct {
    u32 unk0;
} PselBG;

typedef struct {
    ClActUnit *unit;
} PselClAct;

typedef struct {
    G3DCamera *camera;
} PselG3dSys;

struct PselGraphic {
    PselBG bg;
    PselClAct clact;
    PselG3dSys g3d;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} PselBGSetup;

static void func_ov223_021bc538(TCB *tcb, void *data);
static void func_ov223_021bc54c(PselBG *bg, HeapID heapId);
static void func_ov223_021bc5ac(PselBG *bg);
static void func_ov223_021bc5e4(PselBG *bg);
static void func_ov223_021bc5e8(PselBG *bg);
static void func_ov223_021bc5f0(PselClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov223_021bc630(PselClAct *clact);
static void func_ov223_021bc64c(PselClAct *clact);
static void func_ov223_021bc654(PselClAct *clact);
static ClActUnit *func_ov223_021bc65c(PselClAct *clact);
static void func_ov223_021bc660(PselG3dSys *g3d, HeapID heapId);
static void func_ov223_021bc6cc(PselG3dSys *g3d);
static void func_ov223_021bc6dc(PselG3dSys *g3d);
static void func_ov223_021bc6f0(PselG3dSys *g3d);
static G3DCamera *func_ov223_021bc6f8(PselG3dSys *g3d);

static const VecFx32 data_ov223_021bc928 = { 0, FX32_ONE, 0 };
static const VecFx32 data_ov223_021bc91c = { FX32_CONST(6.7), FX32_CONST(6.7), FX32_CONST(17.3) };
static const VecFx32 data_ov223_021bc910 = { 0, FX32_CONST(2.6), 0 };

static const BGSysLCDConfig data_ov223_021bc934 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
static const ClActSysSetup data_ov223_021bc944 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// pokebw2's sPselLights
static const Light data_ov223_021bc960[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

static const BGSysVRAMConfig data_ov223_021bc980 = {
    GX_VRAM_BG_128_A, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,      GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_B,  GX_VRAM_TEXPLTT_01_FG,   GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// BG1 to BG7, in the order the game creates them
static const PselBGSetup data_ov223_021bc9b0[7] = {
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 2,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 3,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x18000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 5,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, 0x4000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 7,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x10000, 0x4000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// The 3D system calls this once it is set up
// pokebw2's PselGraphic_G3DInit
static void func_ov223_021bc34c(void) {
    u32 i;

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    G2_SetBG0Priority(0);
    G3X_SetShading(GX_SHADING_HIGHLIGHT);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, 0, 0, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    for (i = 0; i < NELEMS(data_ov223_021bc960); i++) {
        GFL_G3DSysLightSet(i, &data_ov223_021bc960[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

// pokebw2's PselGraphic_Create
PselGraphic *func_ov223_021bc3f0(u32 layout, HeapID heapId) {
    PselGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(PselGraphic), FALSE, "psel_graphic.c", 463);

    MI_CpuFill8(graphic, 0, sizeof(PselGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov223_021bc980);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov223_021bc54c(&graphic->bg, heapId);
    func_ov223_021bc5f0(&graphic->clact, &data_ov223_021bc980, heapId);
    func_ov223_021bc660(&graphic->g3d, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov223_021bc538, graphic, 0);
    return graphic;
}

// pokebw2's PselGraphic_Free
void func_ov223_021bc498(PselGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov223_021bc6cc(&graphic->g3d);
    func_ov223_021bc630(&graphic->clact);
    func_ov223_021bc5ac(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(PselGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's PselGraphic_Update
void func_ov223_021bc4f4(PselGraphic *graphic) {
    func_ov223_021bc64c(&graphic->clact);
    func_ov223_021bc5e4(&graphic->bg);
}

// pokebw2's PselGraphic_Begin3D
void func_ov223_021bc508(PselGraphic *graphic) {
    func_ov223_021bc6dc(&graphic->g3d);
}

// pokebw2's PselGraphic_End3D
void func_ov223_021bc514(PselGraphic *graphic) {
    func_ov223_021bc6f0(&graphic->g3d);
}

// pokebw2's PselGraphic_GetClActUnit
ClActUnit *func_ov223_021bc520(PselGraphic *graphic) {
    return func_ov223_021bc65c(&graphic->clact);
}

// pokebw2's PselGraphic_GetCamera
G3DCamera *func_ov223_021bc52c(PselGraphic *graphic) {
    return func_ov223_021bc6f8(&graphic->g3d);
}

// pokebw2's PselGraphic_VBlank
static void func_ov223_021bc538(TCB *tcb, void *data) {
    PselGraphic *graphic = data;

    func_ov223_021bc5e8(&graphic->bg);
    func_ov223_021bc654(&graphic->clact);
}

// pokebw2's PselBG_Init
static void func_ov223_021bc54c(PselBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(PselBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov223_021bc934);
    for (i = 0; i < NELEMS(data_ov223_021bc9b0); i++) {
        GFL_BGSysCreateBG(data_ov223_021bc9b0[i].bg, &data_ov223_021bc9b0[i].setup, data_ov223_021bc9b0[i].mode);
        GFL_BGSysClearBG(data_ov223_021bc9b0[i].bg);
        GFL_BGSysSetBGEnabled(data_ov223_021bc9b0[i].bg, data_ov223_021bc9b0[i].enabled);
    }
}

// pokebw2's PselBG_Free
static void func_ov223_021bc5ac(PselBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov223_021bc9b0); i++) {
        GFL_BGSysReleaseBG(data_ov223_021bc9b0[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(PselBG));
}

// pokebw2's PselBG_Update
static void func_ov223_021bc5e4(PselBG *bg) {
}

// pokebw2's PselBG_VBlank
static void func_ov223_021bc5e8(PselBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's PselClAct_Init
static void func_ov223_021bc5f0(PselClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(PselClAct));
    ClActSys_Create(&data_ov223_021bc944, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's PselClAct_Free
static void func_ov223_021bc630(PselClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(PselClAct));
}

// pokebw2's PselClAct_Update
static void func_ov223_021bc64c(PselClAct *clact) {
    func_0204a600();
}

// pokebw2's PselClAct_VBlank
static void func_ov223_021bc654(PselClAct *clact) {
    func_0204a648();
}

// pokebw2's PselClAct_GetUnit
static ClActUnit *func_ov223_021bc65c(PselClAct *clact) {
    return clact->unit;
}

// pokebw2's PselG3dSys_Init
static void func_ov223_021bc660(PselG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 1, 0, 1, 0x1000, heapId, func_ov223_021bc34c);
    g3d->camera =
        GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)), FX32_CONST(4.0 / 3.0),
                            0, FX32_ONE, FX32_CONST(1024), 0, &data_ov223_021bc91c, &data_ov223_021bc910,
                            &data_ov223_021bc928, heapId);
}

// pokebw2's PselG3dSys_Free
static void func_ov223_021bc6cc(PselG3dSys *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}

// pokebw2's PselG3dSys_Begin
static void func_ov223_021bc6dc(PselG3dSys *g3d) {
    GFL_G3DSysReset();
    GFL_G3DCameraFlush(g3d->camera);
    GFL_G3DSysMtxViewFlush();
}

// pokebw2's PselG3dSys_End
static void func_ov223_021bc6f0(PselG3dSys *g3d) {
    GFL_G3DSysReqSwapBuffers();
}

// pokebw2's PselG3dSys_GetCamera
static G3DCamera *func_ov223_021bc6f8(PselG3dSys *g3d) {
    return g3d->camera;
}
