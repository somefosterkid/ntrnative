// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov306/btl_rec_sel_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/btl_rec_sel_graphic.h"
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

// The BGs, cell actors and 3D system of the battle video save question screen, named after the ROM's
// "btl_rec_sel_graphic.c". The file starts at the 3D system's set-up callback, as its siblings of the same template do
// (psel_graphic.c, pkwd_selectscreen_graphic.c); the three functions before it use btl_rec_sel.c's work

#define BG_COUNT 3

typedef struct {
    u32 unk0;
} BtlRecSelBG;

typedef struct {
    ClActUnit *unit;
} BtlRecSelClAct;

typedef struct {
    G3DCamera *camera;
} BtlRecSelG3dSys;

struct BtlRecSelGraphic {
    BtlRecSelBG bg;
    BtlRecSelClAct clact;
    BtlRecSelG3dSys g3d;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} BtlRecSelBGSetup;

static void func_ov214_021bb2b4(TCB *tcb, void *data);
static void func_ov214_021bb2c8(BtlRecSelBG *bg, HeapID heapId);
static void func_ov214_021bb328(BtlRecSelBG *bg);
static void func_ov214_021bb360(BtlRecSelBG *bg);
static void func_ov214_021bb364(BtlRecSelBG *bg);
static void func_ov214_021bb36c(BtlRecSelClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov214_021bb3ac(BtlRecSelClAct *clact);
static void func_ov214_021bb3c8(BtlRecSelClAct *clact);
static void func_ov214_021bb3d0(BtlRecSelClAct *clact);
static ClActUnit *func_ov214_021bb3d8(BtlRecSelClAct *clact);
static void func_ov214_021bb3dc(BtlRecSelG3dSys *g3d, HeapID heapId);
static void func_ov214_021bb448(BtlRecSelG3dSys *g3d);
static void func_ov214_021bb458(BtlRecSelG3dSys *g3d);
static void func_ov214_021bb46c(BtlRecSelG3dSys *g3d);

// pokebw2's sBtlRecSelCameraTarget
static const VecFx32 data_ov214_021bb530 = { 0, FX32_ONE, 0 };
// pokebw2's sBtlRecSelCameraPosition
static const VecFx32 data_ov214_021bb524 = { FX32_CONST(6.7), FX32_CONST(6.7), FX32_CONST(17.3) };
// pokebw2's sBtlRecSelCameraUpVector
static const VecFx32 data_ov214_021bb518 = { 0, FX32_CONST(2.6), 0 };

// pokebw2's sBtlRecSelLCDConfig
static const BGSysLCDConfig data_ov214_021bb53c = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
// pokebw2's sBtlRecSelClActSetup
static const ClActSysSetup data_ov214_021bb54c = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// pokebw2's sBtlRecSelLights
static const Light data_ov214_021bb568[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sBtlRecSelVRAMConfig
static const BGSysVRAMConfig data_ov214_021bb588 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_D,   GX_VRAM_TEXPLTT_0_F,     GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// BG1 to BG3 of the main screen
// pokebw2's sBtlRecSelBGSetups
static const BtlRecSelBGSetup data_ov214_021bb5b8[BG_COUNT] = {
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
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// The 3D system calls this once it is set up
// pokebw2's BtlRecSelGraphic_G3DInit
static void func_ov214_021bb0d4(void) {
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
    for (i = 0; i < NELEMS(data_ov214_021bb568); i++) {
        GFL_G3DSysLightSet(i, &data_ov214_021bb568[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

// pokebw2's BtlRecSelGraphic_Create
BtlRecSelGraphic *func_ov214_021bb178(u32 layout, HeapID heapId) {
    BtlRecSelGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(BtlRecSelGraphic), FALSE, "btl_rec_sel_graphic.c", 458);

    MI_CpuFill8(graphic, 0, sizeof(BtlRecSelGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov214_021bb588);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov214_021bb2c8(&graphic->bg, heapId);
    func_ov214_021bb36c(&graphic->clact, &data_ov214_021bb588, heapId);
    func_ov214_021bb3dc(&graphic->g3d, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov214_021bb2b4, graphic, 0);
    return graphic;
}

// pokebw2's BtlRecSelGraphic_Free
void func_ov214_021bb220(BtlRecSelGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov214_021bb448(&graphic->g3d);
    func_ov214_021bb3ac(&graphic->clact);
    func_ov214_021bb328(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(BtlRecSelGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's BtlRecSelGraphic_Update
void func_ov214_021bb27c(BtlRecSelGraphic *graphic) {
    func_ov214_021bb3c8(&graphic->clact);
    func_ov214_021bb360(&graphic->bg);
}

// pokebw2's BtlRecSelGraphic_Begin3D
void func_ov214_021bb290(BtlRecSelGraphic *graphic) {
    func_ov214_021bb458(&graphic->g3d);
}

// pokebw2's BtlRecSelGraphic_End3D
void func_ov214_021bb29c(BtlRecSelGraphic *graphic) {
    func_ov214_021bb46c(&graphic->g3d);
}

// pokebw2's BtlRecSelGraphic_GetClActUnit
ClActUnit *func_ov214_021bb2a8(BtlRecSelGraphic *graphic) {
    return func_ov214_021bb3d8(&graphic->clact);
}

// pokebw2's BtlRecSelGraphic_VBlank
static void func_ov214_021bb2b4(TCB *tcb, void *data) {
    BtlRecSelGraphic *graphic = data;

    func_ov214_021bb364(&graphic->bg);
    func_ov214_021bb3d0(&graphic->clact);
}

// pokebw2's BtlRecSelBG_Init
static void func_ov214_021bb2c8(BtlRecSelBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(BtlRecSelBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov214_021bb53c);
    for (i = 0; i < NELEMS(data_ov214_021bb5b8); i++) {
        GFL_BGSysCreateBG(data_ov214_021bb5b8[i].bg, &data_ov214_021bb5b8[i].setup, data_ov214_021bb5b8[i].mode);
        GFL_BGSysClearBG(data_ov214_021bb5b8[i].bg);
        GFL_BGSysSetBGEnabled(data_ov214_021bb5b8[i].bg, data_ov214_021bb5b8[i].enabled);
    }
}

// pokebw2's BtlRecSelBG_Free
static void func_ov214_021bb328(BtlRecSelBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov214_021bb5b8); i++) {
        GFL_BGSysReleaseBG(data_ov214_021bb5b8[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(BtlRecSelBG));
}

// pokebw2's BtlRecSelBG_Update
static void func_ov214_021bb360(BtlRecSelBG *bg) {
}

// pokebw2's BtlRecSelBG_VBlank
static void func_ov214_021bb364(BtlRecSelBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's BtlRecSelClAct_Init
static void func_ov214_021bb36c(BtlRecSelClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(BtlRecSelClAct));
    ClActSys_Create(&data_ov214_021bb54c, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's BtlRecSelClAct_Free
static void func_ov214_021bb3ac(BtlRecSelClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(BtlRecSelClAct));
}

// pokebw2's BtlRecSelClAct_Update
static void func_ov214_021bb3c8(BtlRecSelClAct *clact) {
    func_0204a600();
}

// pokebw2's BtlRecSelClAct_VBlank
static void func_ov214_021bb3d0(BtlRecSelClAct *clact) {
    func_0204a648();
}

// pokebw2's BtlRecSelClAct_GetUnit
static ClActUnit *func_ov214_021bb3d8(BtlRecSelClAct *clact) {
    return clact->unit;
}

// pokebw2's BtlRecSelG3dSys_Init
static void func_ov214_021bb3dc(BtlRecSelG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 1, 0, 1, 0, heapId, func_ov214_021bb0d4);
    g3d->camera =
        GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)),
                            FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024), 0, &data_ov214_021bb524,
                            &data_ov214_021bb518, &data_ov214_021bb530, heapId);
}

// pokebw2's BtlRecSelG3dSys_Free
static void func_ov214_021bb448(BtlRecSelG3dSys *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}

// pokebw2's BtlRecSelG3dSys_Begin
static void func_ov214_021bb458(BtlRecSelG3dSys *g3d) {
    GFL_G3DSysReset();
    GFL_G3DCameraFlush(g3d->camera);
    GFL_G3DSysMtxViewFlush();
}

// pokebw2's BtlRecSelG3dSys_End
static void func_ov214_021bb46c(BtlRecSelG3dSys *g3d) {
    GFL_G3DSysReqSwapBuffers();
}
