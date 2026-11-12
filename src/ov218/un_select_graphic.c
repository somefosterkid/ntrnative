// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov311/un_select_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/un_select_graphic.h"
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

// The BGs, cell actors and 3D system of the Unity Tower's floor select, named after the ROM's "un_select_graphic.c".
// The names are ours

typedef struct {
    u32 unk0;
} UnSelectBG;

typedef struct {
    ClActUnit *unit;
} UnSelectClAct;

typedef struct {
    G3DCamera *camera;
} UnSelectG3dSys;

struct UnSelectGraphic {
    UnSelectBG bg;
    UnSelectClAct clact;
    UnSelectG3dSys g3d;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} UnSelectBGSetup;

static void func_ov218_021ba96c(TCB *tcb, void *data);
static void func_ov218_021ba980(UnSelectBG *bg, HeapID heapId);
static void func_ov218_021ba9e0(UnSelectBG *bg);
static void func_ov218_021baa18(UnSelectBG *bg);
static void func_ov218_021baa1c(UnSelectBG *bg);
static void func_ov218_021baa24(UnSelectClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov218_021baa64(UnSelectClAct *clact);
static void func_ov218_021baa80(UnSelectClAct *clact);
static void func_ov218_021baa88(UnSelectClAct *clact);
static ClActUnit *func_ov218_021baa90(UnSelectClAct *clact);
static void func_ov218_021baa94(UnSelectG3dSys *g3d, HeapID heapId);
static void func_ov218_021bab00(UnSelectG3dSys *g3d);

// pokebw2's sUnSelectCameraUpVector
static const VecFx32 data_ov218_021bae88 = { 0, FX32_CONST(2.6), 0 };
// pokebw2's sUnSelectCameraPosition
static const VecFx32 data_ov218_021bae94 = { FX32_CONST(6.7), FX32_CONST(6.7), FX32_CONST(17.3) };
// pokebw2's sUnSelectCameraTarget
static const VecFx32 data_ov218_021baea0 = { 0, FX32_ONE, 0 };

// pokebw2's sUnSelectClActSetup
static const ClActSysSetup data_ov218_021baebc = { 0, 192, 0, 0, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };
// pokebw2's sUnSelectLCDConfig
static const BGSysLCDConfig data_ov218_021baeac = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sUnSelectLights
static const Light data_ov218_021baed8[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sUnSelectVRAMConfig
static const BGSysVRAMConfig data_ov218_021baef8 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_D,   GX_VRAM_TEXPLTT_0_F,     GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sUnSelectBGSetups
static const UnSelectBGSetup data_ov218_021baf28[] = {
    { 0,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x4000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
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
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 7,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// The 3D system calls this once it is set up
// pokebw2's UnSelectGraphic_G3DInit
static void func_ov218_021ba7a4(void) {
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
    for (i = 0; i < NELEMS(data_ov218_021baed8); i++) {
        GFL_G3DSysLightSet(i, &data_ov218_021baed8[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

// pokebw2's UnSelectGraphic_Create
UnSelectGraphic *func_ov218_021ba848(u32 layout, HeapID heapId) {
    UnSelectGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(UnSelectGraphic), FALSE, "un_select_graphic.c", 436);

    MI_CpuFill8(graphic, 0, sizeof(UnSelectGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov218_021baef8);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov218_021ba980(&graphic->bg, heapId);
    func_ov218_021baa24(&graphic->clact, &data_ov218_021baef8, heapId);
    func_ov218_021baa94(&graphic->g3d, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov218_021ba96c, graphic, 0);
    return graphic;
}

// pokebw2's UnSelectGraphic_Free
void func_ov218_021ba8f0(UnSelectGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov218_021bab00(&graphic->g3d);
    func_ov218_021baa64(&graphic->clact);
    func_ov218_021ba9e0(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(UnSelectGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's UnSelectGraphic_Update
void func_ov218_021ba94c(UnSelectGraphic *graphic) {
    func_ov218_021baa80(&graphic->clact);
    func_ov218_021baa18(&graphic->bg);
}

// pokebw2's UnSelectGraphic_GetClActUnit
ClActUnit *func_ov218_021ba960(UnSelectGraphic *graphic) {
    return func_ov218_021baa90(&graphic->clact);
}

// pokebw2's UnSelectGraphic_VBlank
static void func_ov218_021ba96c(TCB *tcb, void *data) {
    UnSelectGraphic *graphic = data;

    func_ov218_021baa1c(&graphic->bg);
    func_ov218_021baa88(&graphic->clact);
}

// pokebw2's UnSelectBG_Init
static void func_ov218_021ba980(UnSelectBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(UnSelectBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov218_021baeac);
    for (i = 0; i < NELEMS(data_ov218_021baf28); i++) {
        GFL_BGSysCreateBG(data_ov218_021baf28[i].bg, &data_ov218_021baf28[i].setup, data_ov218_021baf28[i].mode);
        GFL_BGSysClearBG(data_ov218_021baf28[i].bg);
        GFL_BGSysSetBGEnabled(data_ov218_021baf28[i].bg, data_ov218_021baf28[i].enabled);
    }
}

// pokebw2's UnSelectBG_Free
static void func_ov218_021ba9e0(UnSelectBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov218_021baf28); i++) {
        GFL_BGSysReleaseBG(data_ov218_021baf28[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(UnSelectBG));
}

// pokebw2's UnSelectBG_Update
static void func_ov218_021baa18(UnSelectBG *bg) {
}

// pokebw2's UnSelectBG_VBlank
static void func_ov218_021baa1c(UnSelectBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's UnSelectClAct_Init
static void func_ov218_021baa24(UnSelectClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(UnSelectClAct));
    ClActSys_Create(&data_ov218_021baebc, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's UnSelectClAct_Free
static void func_ov218_021baa64(UnSelectClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(UnSelectClAct));
}

// pokebw2's UnSelectClAct_Update
static void func_ov218_021baa80(UnSelectClAct *clact) {
    func_0204a600();
}

// pokebw2's UnSelectClAct_VBlank
static void func_ov218_021baa88(UnSelectClAct *clact) {
    func_0204a648();
}

// pokebw2's UnSelectClAct_GetUnit
static ClActUnit *func_ov218_021baa90(UnSelectClAct *clact) {
    return clact->unit;
}

// pokebw2's UnSelectG3dSys_Init
static void func_ov218_021baa94(UnSelectG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 1, 0, 1, 0, heapId, func_ov218_021ba7a4);
    // A 40 degree field of view
    g3d->camera =
        GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)),
                            FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024), 0, &data_ov218_021bae94,
                            &data_ov218_021bae88, &data_ov218_021baea0, heapId);
}

// pokebw2's UnSelectG3dSys_Free
static void func_ov218_021bab00(UnSelectG3dSys *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}
