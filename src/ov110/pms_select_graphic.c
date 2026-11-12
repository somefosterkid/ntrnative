// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov185/pms_select_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/pms_select_graphic.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The BGs and cell actors of the phrase select, named after the ROM's "pms_select_graphic.c". The names are ours

typedef struct {
    u32 unused;
} PMSSelectBG;

typedef struct {
    ClActUnit *unit;
} PMSSelectClAct;

struct PMSSelectGraphic {
    PMSSelectBG bg;
    PMSSelectClAct clact;
    u32 unused;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} PMSSelectBGSetup;

static void func_ov110_021c1724(TCB *tcb, void *data);
static void func_ov110_021c1738(PMSSelectBG *bg, HeapID heapId);
static void func_ov110_021c1798(PMSSelectBG *bg);
static void func_ov110_021c17d0(PMSSelectBG *bg);
static void func_ov110_021c17d4(PMSSelectBG *bg);
static void func_ov110_021c17dc(PMSSelectClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov110_021c181c(PMSSelectClAct *clact);
static void func_ov110_021c1838(PMSSelectClAct *clact);
static void func_ov110_021c1840(PMSSelectClAct *clact);
static ClActUnit *func_ov110_021c1848(PMSSelectClAct *clact);

// pokebw2's sPMSSelectLCDConfig
static const BGSysLCDConfig data_ov110_021c3a18 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sPMSSelectClActSetup
static const ClActSysSetup data_ov110_021c3a28 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// pokebw2's sPMSSelectVRAMConfig
static const BGSysVRAMConfig data_ov110_021c3a44 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,      GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sPMSSelectBGSetups
static const PMSSelectBGSetup data_ov110_021c3a74[] = {
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
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 5,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// pokebw2's PMSSelectGraphic_Create
PMSSelectGraphic *func_ov110_021c1610(u32 layout, HeapID heapId) {
    PMSSelectGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(PMSSelectGraphic), FALSE, "pms_select_graphic.c", 337);

    MI_CpuFill8(graphic, 0, sizeof(PMSSelectGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov110_021c3a44);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov110_021c1738(&graphic->bg, heapId);
    func_ov110_021c17dc(&graphic->clact, &data_ov110_021c3a44, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov110_021c1724, graphic, 0);
    return graphic;
}

// pokebw2's PMSSelectGraphic_Delete
void func_ov110_021c16b0(PMSSelectGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov110_021c181c(&graphic->clact);
    func_ov110_021c1798(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(PMSSelectGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's PMSSelectGraphic_Main
void func_ov110_021c1704(PMSSelectGraphic *graphic) {
    func_ov110_021c1838(&graphic->clact);
    func_ov110_021c17d0(&graphic->bg);
}

// pokebw2's PMSSelectGraphic_GetClActUnit
ClActUnit *func_ov110_021c1718(PMSSelectGraphic *graphic) {
    return func_ov110_021c1848(&graphic->clact);
}

// pokebw2's PMSSelectGraphic_VBlank
static void func_ov110_021c1724(TCB *tcb, void *data) {
    PMSSelectGraphic *graphic = data;

    func_ov110_021c17d4(&graphic->bg);
    func_ov110_021c1840(&graphic->clact);
}

// pokebw2's PMSSelectGraphic_BGInit
static void func_ov110_021c1738(PMSSelectBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(PMSSelectBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov110_021c3a18);
    for (i = 0; i < NELEMS(data_ov110_021c3a74); i++) {
        GFL_BGSysCreateBG(data_ov110_021c3a74[i].bg, &data_ov110_021c3a74[i].setup, data_ov110_021c3a74[i].mode);
        GFL_BGSysClearBG(data_ov110_021c3a74[i].bg);
        GFL_BGSysSetBGEnabled(data_ov110_021c3a74[i].bg, data_ov110_021c3a74[i].enabled);
    }
}

// pokebw2's PMSSelectGraphic_BGExit
static void func_ov110_021c1798(PMSSelectBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov110_021c3a74); i++) {
        GFL_BGSysReleaseBG(data_ov110_021c3a74[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(PMSSelectBG));
}

// pokebw2's PMSSelectGraphic_BGMain
static void func_ov110_021c17d0(PMSSelectBG *bg) {
}

// pokebw2's PMSSelectGraphic_BGVBlank
static void func_ov110_021c17d4(PMSSelectBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's PMSSelectGraphic_ClActInit
static void func_ov110_021c17dc(PMSSelectClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(PMSSelectClAct));
    ClActSys_Create(&data_ov110_021c3a28, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's PMSSelectGraphic_ClActExit
static void func_ov110_021c181c(PMSSelectClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(PMSSelectClAct));
}

// pokebw2's PMSSelectGraphic_ClActMain
static void func_ov110_021c1838(PMSSelectClAct *clact) {
    func_0204a600();
}

// pokebw2's PMSSelectGraphic_ClActVBlank
static void func_ov110_021c1840(PMSSelectClAct *clact) {
    func_0204a648();
}

// pokebw2's PMSSelectGraphic_ClActGetUnit
static ClActUnit *func_ov110_021c1848(PMSSelectClAct *clact) {
    return clact->unit;
}
