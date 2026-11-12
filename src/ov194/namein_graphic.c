// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov280/namein_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/namein_graphic.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The BGs and cell actors of the name-entry screen, named after the ROM's "namein_graphic.c". The name-entry screen
// is overlay 280. The code is the same as box_search_graphic.c's, with other tables

#define BG_COUNT 8

typedef struct {
    u32 unk0;
} NameinGraphicBG;

typedef struct {
    ClActUnit *unit;
} NameinGraphicClAct;

struct NameinGraphic {
    NameinGraphicBG bg;
    NameinGraphicClAct clact;
    u32 unk8;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} NameinGraphicBGSetup;

static void func_ov194_021f4bf8(TCB *tcb, void *data);
static void func_ov194_021f4c0c(NameinGraphicBG *bg, HeapID heapId);
static void func_ov194_021f4c6c(NameinGraphicBG *bg);
static void func_ov194_021f4ca4(NameinGraphicBG *bg);
static void func_ov194_021f4ca8(NameinGraphicBG *bg);
static void func_ov194_021f4cb0(NameinGraphicClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov194_021f4cf0(NameinGraphicClAct *clact);
static void func_ov194_021f4d0c(NameinGraphicClAct *clact);
static void func_ov194_021f4d14(NameinGraphicClAct *clact);
static ClActUnit *func_ov194_021f4d1c(NameinGraphicClAct *clact);

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov194_021f55a4 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sClActSysSetup
static const ClActSysSetup data_ov194_021f55b4 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov194_021f55d0 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE, GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,      GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,   GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sBGSetups
static const NameinGraphicBGSetup data_ov194_021f5600[BG_COUNT] = {
    { 0,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 2,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 3,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
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
    { 7,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
};

// pokebw2's NameinGraphic_Create
NameinGraphic *func_ov194_021f4ae4(u32 layout, HeapID heapId) {
    NameinGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(NameinGraphic), FALSE, "namein_graphic.c", 443);

    MI_CpuFill8(graphic, 0, sizeof(NameinGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov194_021f55d0);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov194_021f4c0c(&graphic->bg, heapId);
    func_ov194_021f4cb0(&graphic->clact, &data_ov194_021f55d0, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov194_021f4bf8, graphic, 0);
    return graphic;
}

// pokebw2's NameinGraphic_Delete
void func_ov194_021f4b84(NameinGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov194_021f4cf0(&graphic->clact);
    func_ov194_021f4c6c(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(NameinGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's NameinGraphic_Main
void func_ov194_021f4bd8(NameinGraphic *graphic) {
    func_ov194_021f4d0c(&graphic->clact);
    func_ov194_021f4ca4(&graphic->bg);
}

// pokebw2's NameinGraphic_GetClActUnit
ClActUnit *func_ov194_021f4bec(NameinGraphic *graphic) {
    return func_ov194_021f4d1c(&graphic->clact);
}

// pokebw2's NameinGraphic_VBlank
static void func_ov194_021f4bf8(TCB *tcb, void *data) {
    NameinGraphic *graphic = data;

    func_ov194_021f4ca8(&graphic->bg);
    func_ov194_021f4d14(&graphic->clact);
}

// pokebw2's NameinGraphic_BGInit
static void func_ov194_021f4c0c(NameinGraphicBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(NameinGraphicBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov194_021f55a4);
    for (i = 0; i < NELEMS(data_ov194_021f5600); i++) {
        GFL_BGSysCreateBG(data_ov194_021f5600[i].bg, &data_ov194_021f5600[i].setup, data_ov194_021f5600[i].mode);
        GFL_BGSysClearBG(data_ov194_021f5600[i].bg);
        GFL_BGSysSetBGEnabled(data_ov194_021f5600[i].bg, data_ov194_021f5600[i].enabled);
    }
}

// pokebw2's NameinGraphic_BGExit
static void func_ov194_021f4c6c(NameinGraphicBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov194_021f5600); i++) {
        GFL_BGSysReleaseBG(data_ov194_021f5600[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(NameinGraphicBG));
}

// pokebw2's NameinGraphic_BGMain
static void func_ov194_021f4ca4(NameinGraphicBG *bg) {
}

// pokebw2's NameinGraphic_BGVBlank
static void func_ov194_021f4ca8(NameinGraphicBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's NameinGraphic_ClActInit
static void func_ov194_021f4cb0(NameinGraphicClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(NameinGraphicClAct));
    ClActSys_Create(&data_ov194_021f55b4, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's NameinGraphic_ClActExit
static void func_ov194_021f4cf0(NameinGraphicClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(NameinGraphicClAct));
}

// pokebw2's NameinGraphic_ClActMain
static void func_ov194_021f4d0c(NameinGraphicClAct *clact) {
    func_0204a600();
}

// pokebw2's NameinGraphic_ClActVBlank
static void func_ov194_021f4d14(NameinGraphicClAct *clact) {
    func_0204a648();
}

// pokebw2's NameinGraphic_ClActGetUnit
static ClActUnit *func_ov194_021f4d1c(NameinGraphicClAct *clact) {
    return clact->unit;
}
