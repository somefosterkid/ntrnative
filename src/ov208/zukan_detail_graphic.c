// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zukan_detail_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_detail.h"
#include "app/zukan_detail_graphic.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The detail screen's BGs, cell actors and 3D system. The 3D system is only created for the pages that use it

typedef struct {
    u32 unk0;
} ZukanDetailBG;

typedef struct {
    ClActUnit *unit;
} ZukanDetailClAct;

struct ZukanDetailGraphic {
    ZukanDetailBG bg;
    ZukanDetailClAct clact;
    ZukanDetailG3dSys g3d;
    TCB *vblankTask;
    BOOL g3dCreated;
};

static void func_ov208_021bcf84(TCB *tcb, void *data);
static void func_ov208_021bcf98(ZukanDetailBG *bg, HeapID heapId);
static void func_ov208_021bcff8(ZukanDetailBG *bg);
static void func_ov208_021bd030(ZukanDetailBG *bg);
static void func_ov208_021bd034(ZukanDetailBG *bg);
static void func_ov208_021bd03c(ZukanDetailClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov208_021bd07c(ZukanDetailClAct *clact);
static void func_ov208_021bd098(ZukanDetailClAct *clact);
static void func_ov208_021bd0a0(ZukanDetailClAct *clact);
static ClActUnit *func_ov208_021bd0a8(ZukanDetailClAct *clact);
static void func_ov208_021bd0ac(ZukanDetailG3dSys *g3d, HeapID heapId);

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov208_021c8184 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_23_G,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_D,   GX_VRAM_TEXPLTT_0123_E,  GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sCameraPosition
static const VecFx32 data_ov208_021c8120 = { 0, 0, FX32_CONST(70) };
// pokebw2's sCameraTarget
static const VecFx32 data_ov208_021c812c = { 0, 0, 0 };
// pokebw2's sCameraUpVector
static const VecFx32 data_ov208_021c8114 = { 0, FX32_ONE, 0 };

// pokebw2's sBGSetups
static const ZukanDetailBGSetup data_ov208_021c81b4[BGSYS_BG_COUNT] = {
    { 0,
      { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, 0x4000,
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
      { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 5,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, 0x4000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 7,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x18000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// pokebw2's sLights
static const Light data_ov208_021c8164[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov208_021c8138 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
// pokebw2's sClActSetup
static const ClActSysSetup data_ov208_021c8148 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// The 3D system calls this once it is set up
// pokebw2's ZukanDetailGraphic_G3DInit
static void func_ov208_021bcd68(void) {
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
    for (i = 0; i < NELEMS(data_ov208_021c8164); i++) {
        GFL_G3DSysLightSet(i, &data_ov208_021c8164[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

// pokebw2's ZukanDetailGraphic_Create
ZukanDetailGraphic *func_ov208_021bce10(u32 layout, HeapID heapId, BOOL with3D) {
    ZukanDetailGraphic *graphic =
        GFL_HeapAllocate(heapId, sizeof(ZukanDetailGraphic), FALSE, "zukan_detail_graphic.c", 475);

    MI_CpuFill8(graphic, 0, sizeof(ZukanDetailGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov208_021c8184);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    graphic->g3dCreated = FALSE;
    func_ov208_021bcf98(&graphic->bg, heapId);
    func_ov208_021bd03c(&graphic->clact, &data_ov208_021c8184, heapId);
    if (with3D) {
        func_ov208_021bcf58(graphic, heapId);
    }
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov208_021bcf84, graphic, 0);
    return graphic;
}

// pokebw2's ZukanDetailGraphic_Free
void func_ov208_021bcec0(ZukanDetailGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov208_021bcf6c(graphic);
    func_ov208_021bd07c(&graphic->clact);
    func_ov208_021bcff8(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(ZukanDetailGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's ZukanDetailGraphic_Update
void func_ov208_021bcf18(ZukanDetailGraphic *graphic) {
    func_ov208_021bd098(&graphic->clact);
    func_ov208_021bd030(&graphic->bg);
}

// pokebw2's ZukanDetailGraphic_Begin3D
void func_ov208_021bcf2c(ZukanDetailGraphic *graphic) {
    if (graphic->g3dCreated) {
        func_ov208_021bd12c(&graphic->g3d);
    }
}

// pokebw2's ZukanDetailGraphic_End3D
void func_ov208_021bcf3c(ZukanDetailGraphic *graphic) {
    if (graphic->g3dCreated) {
        func_ov208_021bd140(&graphic->g3d);
    }
}

// pokebw2's ZukanDetailGraphic_GetClActUnit
ClActUnit *func_ov208_021bcf4c(ZukanDetailGraphic *graphic) {
    return func_ov208_021bd0a8(&graphic->clact);
}

// pokebw2's ZukanDetailGraphic_Create3D
void func_ov208_021bcf58(ZukanDetailGraphic *graphic, HeapID heapId) {
    if (!graphic->g3dCreated) {
        graphic->g3dCreated = TRUE;
        func_ov208_021bd0ac(&graphic->g3d, heapId);
    }
}

// pokebw2's ZukanDetailGraphic_Free3D
void func_ov208_021bcf6c(ZukanDetailGraphic *graphic) {
    if (graphic->g3dCreated) {
        func_ov208_021bd11c(&graphic->g3d);
        graphic->g3dCreated = FALSE;
    }
}

// pokebw2's ZukanDetailGraphic_VBlank
static void func_ov208_021bcf84(TCB *tcb, void *data) {
    ZukanDetailGraphic *graphic = data;

    func_ov208_021bd034(&graphic->bg);
    func_ov208_021bd0a0(&graphic->clact);
}

// pokebw2's ZukanDetailBG_Init
static void func_ov208_021bcf98(ZukanDetailBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(ZukanDetailBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov208_021c8138);
    for (i = 0; i < NELEMS(data_ov208_021c81b4); i++) {
        GFL_BGSysCreateBG(data_ov208_021c81b4[i].bg, &data_ov208_021c81b4[i].setup, data_ov208_021c81b4[i].mode);
        GFL_BGSysClearBG(data_ov208_021c81b4[i].bg);
        GFL_BGSysSetBGEnabled(data_ov208_021c81b4[i].bg, data_ov208_021c81b4[i].enabled);
    }
}

// pokebw2's ZukanDetailBG_Free
static void func_ov208_021bcff8(ZukanDetailBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov208_021c81b4); i++) {
        GFL_BGSysReleaseBG(data_ov208_021c81b4[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(ZukanDetailBG));
}

// pokebw2's ZukanDetailBG_Update
static void func_ov208_021bd030(ZukanDetailBG *bg) {
}

// pokebw2's ZukanDetailBG_VBlank
static void func_ov208_021bd034(ZukanDetailBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's ZukanDetailClAct_Init
static void func_ov208_021bd03c(ZukanDetailClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(ZukanDetailClAct));
    ClActSys_Create(&data_ov208_021c8148, vramConfig, heapId);
    clact->unit = func_0204b100(80, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's ZukanDetailClAct_Free
static void func_ov208_021bd07c(ZukanDetailClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(ZukanDetailClAct));
}

// pokebw2's ZukanDetailClAct_Update
static void func_ov208_021bd098(ZukanDetailClAct *clact) {
    func_0204a600();
}

// pokebw2's ZukanDetailClAct_VBlank
static void func_ov208_021bd0a0(ZukanDetailClAct *clact) {
    func_0204a648();
}

// pokebw2's ZukanDetailClAct_GetUnit
static ClActUnit *func_ov208_021bd0a8(ZukanDetailClAct *clact) {
    return clact->unit;
}

// pokebw2's ZukanDetailG3dSys_Init
static void func_ov208_021bd0ac(ZukanDetailG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 1, 0, 2, 0, heapId, func_ov208_021bcd68);
    // A 40 degree field of view
    g3d->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)),
                                      FX_CosIdx(DEG_TO_IDX(20)), FX32_CONST(4.0 / 3.0), 0, 0x107c20, FX32_CONST(1024),
                                      0, &data_ov208_021c8120, &data_ov208_021c8114, &data_ov208_021c812c, heapId);
}
