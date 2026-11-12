// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov305/comm_btl_demo_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/comm_btl_demo_graphic.h"
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

// The BGs, cell actors and 3D system of the link battle's introduction and result screen, named after the ROM's
// "comm_btl_demo_graphic.c". The file starts at the 3D system's set-up callback, as its siblings of the same template
// do (btl_rec_sel_graphic.c, psel_graphic.c)

#define BG_COUNT 6

typedef struct {
    u32 unk0;
} CommBtlDemoBG;

typedef struct {
    ClActUnit *unit;
} CommBtlDemoClAct;

typedef struct {
    G3DCamera *camera;
} CommBtlDemoG3dSys;

struct CommBtlDemoGraphic {
    CommBtlDemoBG bg;
    CommBtlDemoClAct clact;
    CommBtlDemoG3dSys g3d;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} CommBtlDemoBGSetup;

static void func_ov213_021baca0(TCB *tcb, void *data);
static void func_ov213_021bacb4(CommBtlDemoBG *bg, HeapID heapId);
static void func_ov213_021bad14(CommBtlDemoBG *bg);
static void func_ov213_021bad4c(CommBtlDemoBG *bg);
static void func_ov213_021bad50(CommBtlDemoBG *bg);
static void func_ov213_021bad58(CommBtlDemoClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov213_021bad98(CommBtlDemoClAct *clact);
static void func_ov213_021badb4(CommBtlDemoClAct *clact);
static void func_ov213_021badbc(CommBtlDemoClAct *clact);
static ClActUnit *func_ov213_021badc4(CommBtlDemoClAct *clact);
static void func_ov213_021badc8(CommBtlDemoG3dSys *g3d, HeapID heapId);
static void func_ov213_021bae40(CommBtlDemoG3dSys *g3d);
static void func_ov213_021bae50(CommBtlDemoG3dSys *g3d);
static void func_ov213_021bae64(CommBtlDemoG3dSys *g3d);

// pokebw2's sCommBtlDemoCameraTarget
static const VecFx32 data_ov213_021bb164 = { 0, 0, 0 };
// pokebw2's sCommBtlDemoCameraPosition
static const VecFx32 data_ov213_021bb170 = { 0, 0, FX32_CONST(128) };

// pokebw2's sCommBtlDemoVRAMConfig
static const BGSysVRAMConfig data_ov213_021bb1c8 = {
    GX_VRAM_BG_128_A, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_BD, GX_VRAM_TEXPLTT_0_F,    GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_64K,
};

// BG1 to BG3 of the main screen and BG0 to BG2 of the sub screen
// pokebw2's sCommBtlDemoBGSetups
static const CommBtlDemoBGSetup data_ov213_021bb1f8[BG_COUNT] = {
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 2,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 3,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 5,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// pokebw2's sCommBtlDemoLCDConfig
static const BGSysLCDConfig data_ov213_021bb17c = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
// pokebw2's sCommBtlDemoClActSetup
static const ClActSysSetup data_ov213_021bb18c = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// pokebw2's sCommBtlDemoLights
static const Light data_ov213_021bb1a8[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// The 3D system calls this once it is set up
// pokebw2's CommBtlDemoGraphic_G3DInit
static void func_ov213_021baabc(void) {
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
    for (i = 0; i < NELEMS(data_ov213_021bb1a8); i++) {
        GFL_G3DSysLightSet(i, &data_ov213_021bb1a8[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

// pokebw2's CommBtlDemoGraphic_Create
CommBtlDemoGraphic *func_ov213_021bab64(u32 layout, HeapID heapId) {
    CommBtlDemoGraphic *graphic =
        GFL_HeapAllocate(heapId, sizeof(CommBtlDemoGraphic), FALSE, "comm_btl_demo_graphic.c", 436);

    MI_CpuFill8(graphic, 0, sizeof(CommBtlDemoGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov213_021bb1c8);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov213_021bacb4(&graphic->bg, heapId);
    func_ov213_021bad58(&graphic->clact, &data_ov213_021bb1c8, heapId);
    func_ov213_021badc8(&graphic->g3d, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov213_021baca0, graphic, 0);
    return graphic;
}

// pokebw2's CommBtlDemoGraphic_Free
void func_ov213_021bac0c(CommBtlDemoGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov213_021bae40(&graphic->g3d);
    func_ov213_021bad98(&graphic->clact);
    func_ov213_021bad14(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(CommBtlDemoGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's CommBtlDemoGraphic_Update
void func_ov213_021bac68(CommBtlDemoGraphic *graphic) {
    func_ov213_021badb4(&graphic->clact);
    func_ov213_021bad4c(&graphic->bg);
}

// pokebw2's CommBtlDemoGraphic_Begin3D
void func_ov213_021bac7c(CommBtlDemoGraphic *graphic) {
    func_ov213_021bae50(&graphic->g3d);
}

// pokebw2's CommBtlDemoGraphic_End3D
void func_ov213_021bac88(CommBtlDemoGraphic *graphic) {
    func_ov213_021bae64(&graphic->g3d);
}

// pokebw2's CommBtlDemoGraphic_GetClActUnit
ClActUnit *func_ov213_021bac94(CommBtlDemoGraphic *graphic) {
    return func_ov213_021badc4(&graphic->clact);
}

// pokebw2's CommBtlDemoGraphic_VBlank
static void func_ov213_021baca0(TCB *tcb, void *data) {
    CommBtlDemoGraphic *graphic = data;

    func_ov213_021bad50(&graphic->bg);
    func_ov213_021badbc(&graphic->clact);
}

// pokebw2's CommBtlDemoBG_Init
static void func_ov213_021bacb4(CommBtlDemoBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(CommBtlDemoBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov213_021bb17c);
    for (i = 0; i < NELEMS(data_ov213_021bb1f8); i++) {
        GFL_BGSysCreateBG(data_ov213_021bb1f8[i].bg, &data_ov213_021bb1f8[i].setup, data_ov213_021bb1f8[i].mode);
        GFL_BGSysClearBG(data_ov213_021bb1f8[i].bg);
        GFL_BGSysSetBGEnabled(data_ov213_021bb1f8[i].bg, data_ov213_021bb1f8[i].enabled);
    }
}

// pokebw2's CommBtlDemoBG_Free
static void func_ov213_021bad14(CommBtlDemoBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov213_021bb1f8); i++) {
        GFL_BGSysReleaseBG(data_ov213_021bb1f8[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(CommBtlDemoBG));
}

// pokebw2's CommBtlDemoBG_Update
static void func_ov213_021bad4c(CommBtlDemoBG *bg) {
}

// pokebw2's CommBtlDemoBG_VBlank
static void func_ov213_021bad50(CommBtlDemoBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's CommBtlDemoClAct_Init
static void func_ov213_021bad58(CommBtlDemoClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(CommBtlDemoClAct));
    ClActSys_Create(&data_ov213_021bb18c, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's CommBtlDemoClAct_Free
static void func_ov213_021bad98(CommBtlDemoClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(CommBtlDemoClAct));
}

// pokebw2's CommBtlDemoClAct_Update
static void func_ov213_021badb4(CommBtlDemoClAct *clact) {
    func_0204a600();
}

// pokebw2's CommBtlDemoClAct_VBlank
static void func_ov213_021badbc(CommBtlDemoClAct *clact) {
    func_0204a648();
}

// pokebw2's CommBtlDemoClAct_GetUnit
static ClActUnit *func_ov213_021badc4(CommBtlDemoClAct *clact) {
    return clact->unit;
}

// pokebw2's CommBtlDemoG3dSys_Init
static void func_ov213_021badc8(CommBtlDemoG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 1, 0, 1, 0, heapId, func_ov213_021baabc);
    {
        VecFx32 upVector = { 0, FX32_ONE, 0 };

        g3d->camera =
            GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)),
                                FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024), 0, &data_ov213_021bb170,
                                &upVector, &data_ov213_021bb164, heapId);
    }
}

// pokebw2's CommBtlDemoG3dSys_Free
static void func_ov213_021bae40(CommBtlDemoG3dSys *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}

// pokebw2's CommBtlDemoG3dSys_Begin
static void func_ov213_021bae50(CommBtlDemoG3dSys *g3d) {
    GFL_G3DSysReset();
    GFL_G3DCameraFlush(g3d->camera);
    GFL_G3DSysMtxViewFlush();
}

// pokebw2's CommBtlDemoG3dSys_End
static void func_ov213_021bae64(CommBtlDemoG3dSys *g3d) {
    GFL_G3DSysReqSwapBuffers();
}
