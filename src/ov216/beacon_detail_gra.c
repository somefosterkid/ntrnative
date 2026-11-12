// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov308/beacon_detail_gra.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/beacon_detail_gra.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The BGs and cell actors of the screen of a beacon's details, named after the ROM's "beacon_detail_gra.c"

#define BG_COUNT 8

typedef struct {
    u32 unk0;
} BeaconDetailBG;

typedef struct {
    ClActUnit *unit;
    ClActRenderer *renderer;
} BeaconDetailClAct;

struct BeaconDetailGraphic {
    BeaconDetailBG bg;
    BeaconDetailClAct clact;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} BeaconDetailBGSetup;

static void func_ov216_021bca44(TCB *tcb, void *data);
static void func_ov216_021bca58(BeaconDetailBG *bg, HeapID heapId);
static void func_ov216_021bcab8(BeaconDetailBG *bg);
static void func_ov216_021bcaf0(BeaconDetailBG *bg);
static void func_ov216_021bcaf4(BeaconDetailBG *bg);
static void func_ov216_021bcafc(BeaconDetailClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov216_021bcb70(BeaconDetailClAct *clact);
static void func_ov216_021bcb90(BeaconDetailClAct *clact);
static void func_ov216_021bcb98(BeaconDetailClAct *clact);
static ClActUnit *func_ov216_021bcba0(BeaconDetailClAct *clact);

// pokebw2's sBeaconDetailLCDConfig
static const BGSysLCDConfig data_ov216_021bdbac = { GX_DISPMODE_GRAPHICS, GX_BGMODE_5, GX_BGMODE_0, GX_BG0_AS_2D };
// pokebw2's sBeaconDetailClActSetup
static const ClActSysSetup data_ov216_021bdbbc = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// pokebw2's sBeaconDetailVRAMConfig
static const BGSysVRAMConfig data_ov216_021bdbd8 = {
    GX_VRAM_A | GX_VRAM_D, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B,     GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,      GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// The upper screen's panels scroll in from above and below, so their BGs are two screens tall
// pokebw2's sBeaconDetailBGSetups
static const BeaconDetailBGSetup data_ov216_021bdc28[BG_COUNT] = {
    { 0,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x4000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 2,
      { 0, 0, 0x1000, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x20000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_EXTENDED,
      TRUE },
    { 3,
      { 0, 0, 0x1000, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x30000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_EXTENDED,
      TRUE },
    { 4,
      { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 5,
      { 0, 0, 0x1000, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x18000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE },
    { 7,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, 0 },
      BGMODE_TEXT,
      TRUE }
};

// pokebw2's BeaconDetailGraphic_Create
BeaconDetailGraphic *func_ov216_021bc930(u32 layout, HeapID heapId) {
    BeaconDetailGraphic *graphic =
        GFL_HeapAllocate(heapId, sizeof(BeaconDetailGraphic), FALSE, "beacon_detail_gra.c", 317);

    MI_CpuFill8(graphic, 0, sizeof(BeaconDetailGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov216_021bdbd8);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov216_021bca58(&graphic->bg, heapId);
    func_ov216_021bcafc(&graphic->clact, &data_ov216_021bdbd8, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov216_021bca44, graphic, 0);
    return graphic;
}

// pokebw2's BeaconDetailGraphic_Free
void func_ov216_021bc9d0(BeaconDetailGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov216_021bcb70(&graphic->clact);
    func_ov216_021bcab8(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(BeaconDetailGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's BeaconDetailGraphic_Update
void func_ov216_021bca24(BeaconDetailGraphic *graphic) {
    func_ov216_021bcb90(&graphic->clact);
    func_ov216_021bcaf0(&graphic->bg);
}

// pokebw2's BeaconDetailGraphic_GetClActUnit
ClActUnit *func_ov216_021bca38(BeaconDetailGraphic *graphic) {
    return func_ov216_021bcba0(&graphic->clact);
}

// pokebw2's BeaconDetailGraphic_VBlank
static void func_ov216_021bca44(TCB *tcb, void *data) {
    BeaconDetailGraphic *graphic = data;

    func_ov216_021bcaf4(&graphic->bg);
    func_ov216_021bcb98(&graphic->clact);
}

// pokebw2's BeaconDetailBG_Init
static void func_ov216_021bca58(BeaconDetailBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(BeaconDetailBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov216_021bdbac);
    for (i = 0; i < NELEMS(data_ov216_021bdc28); i++) {
        GFL_BGSysCreateBG(data_ov216_021bdc28[i].bg, &data_ov216_021bdc28[i].setup, data_ov216_021bdc28[i].mode);
        GFL_BGSysClearBG(data_ov216_021bdc28[i].bg);
        GFL_BGSysSetBGEnabled(data_ov216_021bdc28[i].bg, data_ov216_021bdc28[i].enabled);
    }
}

// pokebw2's BeaconDetailBG_Free
static void func_ov216_021bcab8(BeaconDetailBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov216_021bdc28); i++) {
        GFL_BGSysReleaseBG(data_ov216_021bdc28[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(BeaconDetailBG));
}

// pokebw2's BeaconDetailBG_Update
static void func_ov216_021bcaf0(BeaconDetailBG *bg) {
}

// pokebw2's BeaconDetailBG_VBlank
static void func_ov216_021bcaf4(BeaconDetailBG *bg) {
    GFL_BGSysUpdate();
}

// The unit draws through a renderer of its own, whose sub screen's surface is the default one's: the screens'
// actors are positioned on a surface
// pokebw2's BeaconDetailClAct_Init
static void func_ov216_021bcafc(BeaconDetailClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(BeaconDetailClAct));
    ClActSys_Create(&data_ov216_021bdbbc, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    {
        ClActSurfaceSetup surfaces[] = {
            { 0, 0, 256, 192, 0, 0 },
            { 0, 512, 256, 192, 1, 0 },
        };

        clact->renderer = func_0204b034(surfaces, NELEMS(surfaces), heapId);
    }
    func_0204b0f4(clact->renderer, TRUE);
    func_0204b258(clact->unit, clact->renderer);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's BeaconDetailClAct_Free
static void func_ov216_021bcb70(BeaconDetailClAct *clact) {
    func_0204b084(clact->renderer);
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(BeaconDetailClAct));
}

// pokebw2's BeaconDetailClAct_Update
static void func_ov216_021bcb90(BeaconDetailClAct *clact) {
    func_0204a600();
}

// pokebw2's BeaconDetailClAct_VBlank
static void func_ov216_021bcb98(BeaconDetailClAct *clact) {
    func_0204a648();
}

// pokebw2's BeaconDetailClAct_GetUnit
static ClActUnit *func_ov216_021bcba0(BeaconDetailClAct *clact) {
    return clact->unit;
}
