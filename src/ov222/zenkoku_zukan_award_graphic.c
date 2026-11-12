// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov315/zenkoku_zukan_award_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zenkoku_zukan_award_graphic.h"
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

// The BGs, cell actors and 3D system of the national Pokédex diploma, named after the ROM's
// "zenkoku_zukan_award_graphic.c"

typedef struct {
    u32 unk0;
} ZenkokuZukanAwardBG;

typedef struct {
    ClActUnit *unit;
} ZenkokuZukanAwardClAct;

typedef struct {
    G3DCamera *camera;
} ZenkokuZukanAwardG3dSys;

struct ZenkokuZukanAwardGraphic {
    ZenkokuZukanAwardBG bg;
    ZenkokuZukanAwardClAct clact;
    ZenkokuZukanAwardG3dSys g3d;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} ZenkokuZukanAwardBGSetup;

static void func_ov222_021ba034(TCB *tcb, void *data);
static void func_ov222_021ba048(ZenkokuZukanAwardBG *bg, HeapID heapId);
static void func_ov222_021ba0a8(ZenkokuZukanAwardBG *bg);
static void func_ov222_021ba0e0(ZenkokuZukanAwardBG *bg);
static void func_ov222_021ba0e4(ZenkokuZukanAwardBG *bg);
static void func_ov222_021ba0ec(ZenkokuZukanAwardClAct *clact, const BGSysVRAMConfig *vramConfig,
                                        HeapID heapId);
static void func_ov222_021ba12c(ZenkokuZukanAwardClAct *clact);
static void func_ov222_021ba148(ZenkokuZukanAwardClAct *clact);
static void func_ov222_021ba150(ZenkokuZukanAwardClAct *clact);
static ClActUnit *func_ov222_021ba158(ZenkokuZukanAwardClAct *clact);
static void func_ov222_021ba15c(ZenkokuZukanAwardG3dSys *g3d, HeapID heapId);
static void func_ov222_021ba1c8(ZenkokuZukanAwardG3dSys *g3d);

// pokebw2's sZenkokuZukanAwardCameraUpVector
static const VecFx32 data_ov222_021ba25c = { 0, FX32_CONST(2.6), 0 };
// pokebw2's sZenkokuZukanAwardCameraPosition
static const VecFx32 data_ov222_021ba268 = { FX32_CONST(6.7), FX32_CONST(6.7), FX32_CONST(17.3) };
// pokebw2's sZenkokuZukanAwardCameraTarget
static const VecFx32 data_ov222_021ba274 = { 0, FX32_ONE, 0 };

// pokebw2's sZenkokuZukanAwardClActSetup
static const ClActSysSetup data_ov222_021ba290 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };
// pokebw2's sZenkokuZukanAwardLCDConfig
static const BGSysLCDConfig data_ov222_021ba280 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0,
                                                            GX_BG0_AS_3D };

// pokebw2's sZenkokuZukanAwardLights
static const Light data_ov222_021ba2ac[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sZenkokuZukanAwardVRAMConfig
static const BGSysVRAMConfig data_ov222_021ba2cc = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_D,   GX_VRAM_TEXPLTT_0_F,     GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sZenkokuZukanAwardBGSetups
static const ZenkokuZukanAwardBGSetup data_ov222_021ba2fc[2] = {
    { 1,
      { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x04000, 0x10000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 2,
      { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// The 3D system calls this once it is set up
// pokebw2's ZenkokuZukanAwardGraphic_G3DInit
static void func_ov222_021b9e6c(void) {
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
    for (i = 0; i < NELEMS(data_ov222_021ba2ac); i++) {
        GFL_G3DSysLightSet(i, &data_ov222_021ba2ac[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

// pokebw2's ZenkokuZukanAwardGraphic_Create
ZenkokuZukanAwardGraphic *func_ov222_021b9f10(u32 layout, HeapID heapId) {
    ZenkokuZukanAwardGraphic *graphic =
        GFL_HeapAllocate(heapId, sizeof(ZenkokuZukanAwardGraphic), FALSE, "zenkoku_zukan_award_graphic.c", 460);

    MI_CpuFill8(graphic, 0, sizeof(ZenkokuZukanAwardGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov222_021ba2cc);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov222_021ba048(&graphic->bg, heapId);
    func_ov222_021ba0ec(&graphic->clact, &data_ov222_021ba2cc, heapId);
    func_ov222_021ba15c(&graphic->g3d, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov222_021ba034, graphic, 0);
    return graphic;
}

// pokebw2's ZenkokuZukanAwardGraphic_Free
void func_ov222_021b9fb8(ZenkokuZukanAwardGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov222_021ba1c8(&graphic->g3d);
    func_ov222_021ba12c(&graphic->clact);
    func_ov222_021ba0a8(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(ZenkokuZukanAwardGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's ZenkokuZukanAwardGraphic_Update
void func_ov222_021ba014(ZenkokuZukanAwardGraphic *graphic) {
    func_ov222_021ba148(&graphic->clact);
    func_ov222_021ba0e0(&graphic->bg);
}

// pokebw2's ZenkokuZukanAwardGraphic_GetClActUnit
ClActUnit *func_ov222_021ba028(ZenkokuZukanAwardGraphic *graphic) {
    return func_ov222_021ba158(&graphic->clact);
}

// pokebw2's ZenkokuZukanAwardGraphic_VBlank
static void func_ov222_021ba034(TCB *tcb, void *data) {
    ZenkokuZukanAwardGraphic *graphic = data;

    func_ov222_021ba0e4(&graphic->bg);
    func_ov222_021ba150(&graphic->clact);
}

// pokebw2's ZenkokuZukanAwardBG_Init
static void func_ov222_021ba048(ZenkokuZukanAwardBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(ZenkokuZukanAwardBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov222_021ba280);
    for (i = 0; i < NELEMS(data_ov222_021ba2fc); i++) {
        GFL_BGSysCreateBG(data_ov222_021ba2fc[i].bg, &data_ov222_021ba2fc[i].setup,
                          data_ov222_021ba2fc[i].mode);
        GFL_BGSysClearBG(data_ov222_021ba2fc[i].bg);
        GFL_BGSysSetBGEnabled(data_ov222_021ba2fc[i].bg, data_ov222_021ba2fc[i].enabled);
    }
}

// pokebw2's ZenkokuZukanAwardBG_Free
static void func_ov222_021ba0a8(ZenkokuZukanAwardBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov222_021ba2fc); i++) {
        GFL_BGSysReleaseBG(data_ov222_021ba2fc[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(ZenkokuZukanAwardBG));
}

// pokebw2's ZenkokuZukanAwardBG_Update
static void func_ov222_021ba0e0(ZenkokuZukanAwardBG *bg) {
}

// pokebw2's ZenkokuZukanAwardBG_VBlank
static void func_ov222_021ba0e4(ZenkokuZukanAwardBG *bg) {
    GFL_BGSysUpdate();
}

static void func_ov222_021ba0ec(ZenkokuZukanAwardClAct *clact, const BGSysVRAMConfig *vramConfig,
                                        HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(ZenkokuZukanAwardClAct));
    ClActSys_Create(&data_ov222_021ba290, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's ZenkokuZukanAwardClAct_Free
static void func_ov222_021ba12c(ZenkokuZukanAwardClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(ZenkokuZukanAwardClAct));
}

// pokebw2's ZenkokuZukanAwardClAct_Update
static void func_ov222_021ba148(ZenkokuZukanAwardClAct *clact) {
    func_0204a600();
}

// pokebw2's ZenkokuZukanAwardClAct_VBlank
static void func_ov222_021ba150(ZenkokuZukanAwardClAct *clact) {
    func_0204a648();
}

// pokebw2's ZenkokuZukanAwardClAct_GetUnit
static ClActUnit *func_ov222_021ba158(ZenkokuZukanAwardClAct *clact) {
    return clact->unit;
}

// pokebw2's ZenkokuZukanAwardG3dSys_Init
static void func_ov222_021ba15c(ZenkokuZukanAwardG3dSys *g3d, HeapID heapId) {
    GFL_G3DSysCreate(FALSE, 1, 0, 1, 0, heapId, func_ov222_021b9e6c);
    // A 40 degree field of view
    g3d->camera =
        GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)),
                            FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024), 0, &data_ov222_021ba268,
                            &data_ov222_021ba25c, &data_ov222_021ba274, heapId);
}

// pokebw2's ZenkokuZukanAwardG3dSys_Free
static void func_ov222_021ba1c8(ZenkokuZukanAwardG3dSys *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}
