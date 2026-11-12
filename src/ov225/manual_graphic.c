// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov319/manual_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/manual_graphic.h"
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

// The BGs and cell actors of the manual, named after the ROM's "manual_graphic.c". Only the sub engine's BGs are
// created here: the main engine shows a picture on a bitmap BG, which manual_common.c sets up

#define BG_COUNT 4

typedef struct {
    u32 unk0;
} ManualBG;

typedef struct {
    ClActUnit *unit;
} ManualClAct;

struct ManualGraphic {
    ManualBG bg;
    ManualClAct clact;
    u32 unk8;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} ManualBGSetup;

static void func_ov225_021b9a14(TCB *tcb, void *data);
static void func_ov225_021b9a28(ManualBG *bg, HeapID heapId);
static void func_ov225_021b9a88(ManualBG *bg);
static void func_ov225_021b9ac0(ManualBG *bg);
static void func_ov225_021b9ac4(ManualBG *bg);
static void func_ov225_021b9acc(ManualClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov225_021b9b0c(ManualClAct *clact);
static void func_ov225_021b9b28(ManualClAct *clact);
static void func_ov225_021b9b30(ManualClAct *clact);
static ClActUnit *func_ov225_021b9b38(ManualClAct *clact);

static const BGSysLCDConfig data_ov225_021bbdf0 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_5, GX_BGMODE_0, GX_BG0_AS_2D };
static const ClActSysSetup data_ov225_021bbe00 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

static const BGSysVRAMConfig data_ov225_021bbe1c = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_D,   GX_VRAM_TEXPLTT_0_F,     GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

static const ManualBGSetup data_ov225_021bbe4c[BG_COUNT] = {
    { 4,
      { 0, 0, 0x1000, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x4000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 5,
      { 0, 0, 0x1000, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 6,
      { 0, 0, 0x1000, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 7,
      { 0, 0, 0x1000, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x18000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE }
};

// pokebw2's ManualGraphic_Create
ManualGraphic *func_ov225_021b9900(u32 layout, HeapID heapId) {
    ManualGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(ManualGraphic), FALSE, "manual_graphic.c", 454);

    MI_CpuFill8(graphic, 0, sizeof(ManualGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov225_021bbe1c);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov225_021b9a28(&graphic->bg, heapId);
    func_ov225_021b9acc(&graphic->clact, &data_ov225_021bbe1c, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov225_021b9a14, graphic, 0);
    return graphic;
}

// pokebw2's ManualGraphic_Free
void func_ov225_021b99a0(ManualGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov225_021b9b0c(&graphic->clact);
    func_ov225_021b9a88(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(ManualGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's ManualGraphic_Update
void func_ov225_021b99f4(ManualGraphic *graphic) {
    func_ov225_021b9b28(&graphic->clact);
    func_ov225_021b9ac0(&graphic->bg);
}

// pokebw2's ManualGraphic_GetClActUnit
ClActUnit *func_ov225_021b9a08(ManualGraphic *graphic) {
    return func_ov225_021b9b38(&graphic->clact);
}

// pokebw2's ManualGraphic_VBlank
static void func_ov225_021b9a14(TCB *tcb, void *data) {
    ManualGraphic *graphic = data;

    func_ov225_021b9ac4(&graphic->bg);
    func_ov225_021b9b30(&graphic->clact);
}

// pokebw2's ManualBG_Init
static void func_ov225_021b9a28(ManualBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(ManualBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov225_021bbdf0);
    for (i = 0; i < NELEMS(data_ov225_021bbe4c); i++) {
        GFL_BGSysCreateBG(data_ov225_021bbe4c[i].bg, &data_ov225_021bbe4c[i].setup, data_ov225_021bbe4c[i].mode);
        GFL_BGSysClearBG(data_ov225_021bbe4c[i].bg);
        GFL_BGSysSetBGEnabled(data_ov225_021bbe4c[i].bg, data_ov225_021bbe4c[i].enabled);
    }
}

// pokebw2's ManualBG_Free
static void func_ov225_021b9a88(ManualBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov225_021bbe4c); i++) {
        GFL_BGSysReleaseBG(data_ov225_021bbe4c[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(ManualBG));
}

// pokebw2's ManualBG_Update
static void func_ov225_021b9ac0(ManualBG *bg) {
}

// pokebw2's ManualBG_VBlank
static void func_ov225_021b9ac4(ManualBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's ManualClAct_Init
static void func_ov225_021b9acc(ManualClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(ManualClAct));
    ClActSys_Create(&data_ov225_021bbe00, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's ManualClAct_Free
static void func_ov225_021b9b0c(ManualClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(ManualClAct));
}

// pokebw2's ManualClAct_Update
static void func_ov225_021b9b28(ManualClAct *clact) {
    func_0204a600();
}

// pokebw2's ManualClAct_VBlank
static void func_ov225_021b9b30(ManualClAct *clact) {
    func_0204a648();
}

// pokebw2's ManualClAct_GetUnit
static ClActUnit *func_ov225_021b9b38(ManualClAct *clact) {
    return clact->unit;
}
