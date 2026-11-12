// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov313/th_award_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/th_award_graphic.h"
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

// The BGs and cell actors of the Trial House's results screen, named after the ROM's "th_award_graphic.c"

typedef struct {
    u32 unk0;
} ThAwardBG;

typedef struct {
    ClActUnit *unit;
} ThAwardClAct;

struct ThAwardGraphic {
    ThAwardBG bg;
    ThAwardClAct clact;
    u32 unk8;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} ThAwardBGSetup;

static void func_ov220_021ba1ec(TCB *tcb, void *data);
static void func_ov220_021ba200(ThAwardBG *bg, HeapID heapId);
static void func_ov220_021ba260(ThAwardBG *bg);
static void func_ov220_021ba298(ThAwardBG *bg);
static void func_ov220_021ba29c(ThAwardBG *bg);
static void func_ov220_021ba2a4(ThAwardClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov220_021ba2e4(ThAwardClAct *clact);
static void func_ov220_021ba300(ThAwardClAct *clact);
static void func_ov220_021ba308(ThAwardClAct *clact);
static ClActUnit *func_ov220_021ba310(ThAwardClAct *clact);

// pokebw2's sThAwardLCDConfig
static const BGSysLCDConfig data_ov220_021ba3fc = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
// pokebw2's sThAwardClActSetup
static const ClActSysSetup data_ov220_021ba40c = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

// pokebw2's sThAwardVRAMConfig
static const BGSysVRAMConfig data_ov220_021ba428 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_D,   GX_VRAM_TEXPLTT_0_F,     GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sThAwardBGSetups
static const ThAwardBGSetup data_ov220_021ba458[3] = {
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 2,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 3,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// pokebw2's ThAwardGraphic_Create
ThAwardGraphic *func_ov220_021ba0d8(u32 layout, HeapID heapId) {
    ThAwardGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(ThAwardGraphic), FALSE, "th_award_graphic.c", 458);

    MI_CpuFill8(graphic, 0, sizeof(ThAwardGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov220_021ba428);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov220_021ba200(&graphic->bg, heapId);
    func_ov220_021ba2a4(&graphic->clact, &data_ov220_021ba428, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov220_021ba1ec, graphic, 0);
    return graphic;
}

// pokebw2's ThAwardGraphic_Free
void func_ov220_021ba178(ThAwardGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov220_021ba2e4(&graphic->clact);
    func_ov220_021ba260(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(ThAwardGraphic));
    GFL_HeapFree(graphic);
}

// pokebw2's ThAwardGraphic_Update
void func_ov220_021ba1cc(ThAwardGraphic *graphic) {
    func_ov220_021ba300(&graphic->clact);
    func_ov220_021ba298(&graphic->bg);
}

// pokebw2's ThAwardGraphic_GetClActUnit
ClActUnit *func_ov220_021ba1e0(ThAwardGraphic *graphic) {
    return func_ov220_021ba310(&graphic->clact);
}

// pokebw2's ThAwardGraphic_VBlank
static void func_ov220_021ba1ec(TCB *tcb, void *data) {
    ThAwardGraphic *graphic = data;

    func_ov220_021ba29c(&graphic->bg);
    func_ov220_021ba308(&graphic->clact);
}

// pokebw2's ThAwardBG_Init
static void func_ov220_021ba200(ThAwardBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(ThAwardBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov220_021ba3fc);
    for (i = 0; i < NELEMS(data_ov220_021ba458); i++) {
        GFL_BGSysCreateBG(data_ov220_021ba458[i].bg, &data_ov220_021ba458[i].setup, data_ov220_021ba458[i].mode);
        GFL_BGSysClearBG(data_ov220_021ba458[i].bg);
        GFL_BGSysSetBGEnabled(data_ov220_021ba458[i].bg, data_ov220_021ba458[i].enabled);
    }
}

// pokebw2's ThAwardBG_Free
static void func_ov220_021ba260(ThAwardBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov220_021ba458); i++) {
        GFL_BGSysReleaseBG(data_ov220_021ba458[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(ThAwardBG));
}

// pokebw2's ThAwardBG_Update
static void func_ov220_021ba298(ThAwardBG *bg) {
}

// pokebw2's ThAwardBG_VBlank
static void func_ov220_021ba29c(ThAwardBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's ThAwardClAct_Init
static void func_ov220_021ba2a4(ThAwardClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    MI_CpuFill8(clact, 0, sizeof(ThAwardClAct));
    ClActSys_Create(&data_ov220_021ba40c, vramConfig, heapId);
    clact->unit = func_0204b100(128, 0, heapId);
    func_0204b270(clact->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's ThAwardClAct_Free
static void func_ov220_021ba2e4(ThAwardClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(ThAwardClAct));
}

// pokebw2's ThAwardClAct_Update
static void func_ov220_021ba300(ThAwardClAct *clact) {
    func_0204a600();
}

// pokebw2's ThAwardClAct_VBlank
static void func_ov220_021ba308(ThAwardClAct *clact) {
    func_0204a648();
}

// pokebw2's ThAwardClAct_GetUnit
static ClActUnit *func_ov220_021ba310(ThAwardClAct *clact) {
    return clact->unit;
}
