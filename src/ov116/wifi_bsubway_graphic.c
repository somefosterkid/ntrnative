// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov191/wifi_bsubway_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wifi_bsubway_graphic.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The BGs of the Wi-Fi Battle Subway's screen, named after the ROM's "wifi_bsubway_graphic.c". It is the graphic
// template of pkwd_selectscreen_graphic.c without the cell actors

#define BG_COUNT 8

typedef struct {
    u32 unk0;
} WifiBSubwayBG;

struct WifiBSubwayGraphic {
    WifiBSubwayBG bg;
    u32 unk4;
    u32 unk8;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} WifiBSubwayBGSetup;

static void func_ov116_021d82f8(TCB *tcb, void *data);
static void func_ov116_021d8304(WifiBSubwayBG *bg, HeapID heapId);
static void func_ov116_021d8364(WifiBSubwayBG *bg);
static void func_ov116_021d839c(WifiBSubwayBG *bg);

// pokebw2's sWifiBSubwayLCDConfig
static const BGSysLCDConfig data_ov116_021d83b0 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sWifiBSubwayVRAMConfig
static const BGSysVRAMConfig data_ov116_021d83c0 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sWifiBSubwayBGSetups
static const WifiBSubwayBGSetup data_ov116_021d83f0[BG_COUNT] = {
    { 0,
      { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 2,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 3,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x18000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 5,
      { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 6,
      { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 7,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE }
};

// pokebw2's WifiBSubwayGraphic_Create
WifiBSubwayGraphic *func_ov116_021d821c(u32 layout, HeapID heapId) {
    WifiBSubwayGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(WifiBSubwayGraphic), FALSE, "wifi_bsubway_graphic.c", 444);

    MI_CpuFill8(graphic, 0, sizeof(WifiBSubwayGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov116_021d83c0);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov116_021d8304(&graphic->bg, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov116_021d82f8, graphic, 0);
    return graphic;
}

// pokebw2's WifiBSubwayGraphic_Free
void func_ov116_021d82ac(WifiBSubwayGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov116_021d8364(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(WifiBSubwayGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's WifiBSubwayGraphic_VBlank
static void func_ov116_021d82f8(TCB *tcb, void *data) {
    WifiBSubwayGraphic *graphic = data;

    func_ov116_021d839c(&graphic->bg);
}

// pokebw2's WifiBSubwayBG_Init
static void func_ov116_021d8304(WifiBSubwayBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(WifiBSubwayBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov116_021d83b0);
    for (i = 0; i < NELEMS(data_ov116_021d83f0); i++) {
        GFL_BGSysCreateBG(data_ov116_021d83f0[i].bg, &data_ov116_021d83f0[i].setup, data_ov116_021d83f0[i].mode);
        GFL_BGSysClearBG(data_ov116_021d83f0[i].bg);
        GFL_BGSysSetBGEnabled(data_ov116_021d83f0[i].bg, data_ov116_021d83f0[i].enabled);
    }
}

// pokebw2's WifiBSubwayBG_Free
static void func_ov116_021d8364(WifiBSubwayBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov116_021d83f0); i++) {
        GFL_BGSysReleaseBG(data_ov116_021d83f0[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(WifiBSubwayBG));
}

// pokebw2's WifiBSubwayBG_VBlank
static void func_ov116_021d839c(WifiBSubwayBG *bg) {
    GFL_BGSysUpdate();
}
