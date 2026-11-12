// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov297/zukan_toroku_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_toroku_graphic.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The BGs and the cell actor system of the Pokédex registration

#define BG_COUNT 4

typedef struct {
    u32 unk0;
} ZukanTorokuBG;

typedef struct {
    ClActUnit *unit;
} ZukanTorokuClAct;

struct ZukanTorokuGraphic {
    ZukanTorokuBG bg;
    ZukanTorokuClAct clact;
    // The 3D system of the template that this is copied from, which this does not use
    u32 unused8;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} ZukanTorokuBGSetup;

static void func_ov207_0220b434(TCB *tcb, void *data);
static void func_ov207_0220b448(ZukanTorokuBG *bg, HeapID heapId);
static void func_ov207_0220b4a8(ZukanTorokuBG *bg);
static void func_ov207_0220b4e0(ZukanTorokuBG *bg);
static void func_ov207_0220b4e4(ZukanTorokuBG *bg);
static void func_ov207_0220b4ec(ZukanTorokuClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov207_0220b52c(ZukanTorokuClAct *clact);
static void func_ov207_0220b548(ZukanTorokuClAct *clact);
static void func_ov207_0220b550(ZukanTorokuClAct *clact);
static ClActUnit *func_ov207_0220b558(ZukanTorokuClAct *clact);

static const BGSysLCDConfig data_ov207_0220babc = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
static const ClActSysSetup data_ov207_0220bacc = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

static const BGSysVRAMConfig data_ov207_0220bae8 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_D,   GX_VRAM_TEXPLTT_0_F,     GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

static const ZukanTorokuBGSetup data_ov207_0220bb18[BG_COUNT] = {
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
};

// pokebw2's ZukanTorokuGraphic_Create
ZukanTorokuGraphic *func_ov207_0220b318(u32 layout, HeapID heapId) {
    ZukanTorokuGraphic *graphic =
        GFL_HeapAllocate(heapId, sizeof(ZukanTorokuGraphic), FALSE, "zukan_toroku_graphic.c", 455);

    MI_CpuFill8(graphic, 0, sizeof(ZukanTorokuGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov207_0220bae8);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov207_0220b448(&graphic->bg, heapId);
    func_ov207_0220b4ec(&graphic->clact, &data_ov207_0220bae8, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov207_0220b434, graphic, 0);
    return graphic;
}

// pokebw2's ZukanTorokuGraphic_Free
void func_ov207_0220b3b8(ZukanTorokuGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov207_0220b52c(&graphic->clact);
    func_ov207_0220b4a8(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(ZukanTorokuGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's ZukanTorokuGraphic_Update
void func_ov207_0220b40c(ZukanTorokuGraphic *graphic) {
    func_ov207_0220b548(&graphic->clact);
    func_ov207_0220b4e0(&graphic->bg);
}

// pokebw2's ZukanTorokuGraphic_Begin3D
void func_ov207_0220b420(ZukanTorokuGraphic *graphic) {
}

// pokebw2's ZukanTorokuGraphic_End3D
void func_ov207_0220b424(ZukanTorokuGraphic *graphic) {
}

// pokebw2's ZukanTorokuGraphic_GetClActUnit
ClActUnit *func_ov207_0220b428(ZukanTorokuGraphic *graphic) {
    return func_ov207_0220b558(&graphic->clact);
}

// pokebw2's ZukanTorokuGraphic_VBlank
static void func_ov207_0220b434(TCB *tcb, void *data) {
    ZukanTorokuGraphic *graphic = data;

    func_ov207_0220b4e4(&graphic->bg);
    func_ov207_0220b550(&graphic->clact);
}

// pokebw2's ZukanTorokuBG_Init
static void func_ov207_0220b448(ZukanTorokuBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(ZukanTorokuBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov207_0220babc);
    for (i = 0; i < NELEMS(data_ov207_0220bb18); i++) {
        GFL_BGSysCreateBG(data_ov207_0220bb18[i].bg, &data_ov207_0220bb18[i].setup, data_ov207_0220bb18[i].mode);
        GFL_BGSysClearBG(data_ov207_0220bb18[i].bg);
        GFL_BGSysSetBGEnabled(data_ov207_0220bb18[i].bg, data_ov207_0220bb18[i].enabled);
    }
}

// pokebw2's ZukanTorokuBG_Free
static void func_ov207_0220b4a8(ZukanTorokuBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov207_0220bb18); i++) {
        GFL_BGSysReleaseBG(data_ov207_0220bb18[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(ZukanTorokuBG));
}

// pokebw2's ZukanTorokuBG_Update
static void func_ov207_0220b4e0(ZukanTorokuBG *bg) {
}

// pokebw2's ZukanTorokuBG_VBlank
static void func_ov207_0220b4e4(ZukanTorokuBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's ZukanTorokuClAct_Init
static void func_ov207_0220b4ec(ZukanTorokuClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(ZukanTorokuClAct));
    ClActSys_Create(&data_ov207_0220bacc, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's ZukanTorokuClAct_Free
static void func_ov207_0220b52c(ZukanTorokuClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(ZukanTorokuClAct));
}

// pokebw2's ZukanTorokuClAct_Update
static void func_ov207_0220b548(ZukanTorokuClAct *clact) {
    func_0204a600();
}

// pokebw2's ZukanTorokuClAct_VBlank
static void func_ov207_0220b550(ZukanTorokuClAct *clact) {
    func_0204a648();
}

// pokebw2's ZukanTorokuClAct_GetUnit
static ClActUnit *func_ov207_0220b558(ZukanTorokuClAct *clact) {
    return clact->unit;
}
