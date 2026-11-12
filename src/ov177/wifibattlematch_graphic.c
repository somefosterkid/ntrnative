// Black's code at ov177 0x021e5440-0x021e567c: the BGs and cell actor system that the Wi-Fi and live competition screens
// draw on. The name is the file name its heap allocation passes, as pokebw2's src/ov262/wifibattlematch_graphic.c has it.

#include "types.h"
#include "app/wifibattlematch_graphic.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "struct_decls.h"
#include "system/gf_font.h"

// A BG of the screens: its ID, setup, mode and whether it is shown
typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} WifiBattleMatchGraphicBGSetup;

// The screens' BGs, cell actors and vblank task, as pokebw2's wifibattlematch_graphic.c has them
typedef struct {
    u32 unk0;
} WifiBattleMatchGraphicBG;

typedef struct {
    ClActUnit *unit;
} WifiBattleMatchGraphicClAct;

struct WifiBattleMatchGraphic {
    WifiBattleMatchGraphicBG bg;
    WifiBattleMatchGraphicClAct clact;
    u32 unk8;
    TCB *vblankTask;
};

void func_ov177_021e5568(WifiBattleMatchGraphicBG *bg, HeapID heapId);
void func_ov177_021e560c(WifiBattleMatchGraphicClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
void func_ov177_021e5554(TCB *tcb, void *data);
void func_ov177_021e564c(WifiBattleMatchGraphicClAct *clact);
void func_ov177_021e55c8(WifiBattleMatchGraphicBG *bg);
ClActUnit *func_ov177_021e5678(WifiBattleMatchGraphicClAct *clact);
const ClActSysSetup data_ov177_021e8b10 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };
const BGSysLCDConfig data_ov177_021e8b00 = { 1, 0, 0, 0 };


// clang-format off
const BGSysVRAMConfig data_ov177_021e8b2c = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,      GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};
// clang-format on

// clang-format off
const WifiBattleMatchGraphicBGSetup data_ov177_021e8b5c[8] = {
    { 0,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 1,
      { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 2,
      { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 3,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 5,
      { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 6,
      { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
    { 7,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT, TRUE },
};
// clang-format on

WifiBattleMatchGraphic *func_ov177_021e5440(u32 layout, HeapID heapId) {
    WifiBattleMatchGraphic *graphic;
    TCB *task;

    graphic = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchGraphic), FALSE, "wifibattlematch_graphic.c", 0x1bb);
    MI_CpuFill8(graphic, 0, sizeof(WifiBattleMatchGraphic));
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(0);
    GFL_BGSysSetVRAMBanks(&data_ov177_021e8b2c);
    GFL_BGSysSetDisplayLayout(layout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();
    func_ov177_021e5568(&graphic->bg, heapId);
    func_ov177_021e560c(&graphic->clact, &data_ov177_021e8b2c, heapId);
    task = GFL_VBlankTCBAdd(func_ov177_021e5554, graphic, 0);
    graphic->vblankTask = task;
    return graphic;
}

void func_ov177_021e5604(WifiBattleMatchGraphicBG *bg);
void func_ov177_021e5670(WifiBattleMatchGraphicClAct *clact);

void func_ov177_021e54e0(WifiBattleMatchGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov177_021e564c(&graphic->clact);
    func_ov177_021e55c8(&graphic->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(graphic, 0, sizeof(WifiBattleMatchGraphic));
    GFL_HeapFree(graphic);
}

void func_ov177_021e5668(WifiBattleMatchGraphicClAct *clact);
void func_ov177_021e5600(WifiBattleMatchGraphicBG *bg);


void func_ov177_021e5534(WifiBattleMatchGraphic *graphic) {
    func_ov177_021e5668(&graphic->clact);
    func_ov177_021e5600(&graphic->bg);
}


ClActUnit *func_ov177_021e5548(WifiBattleMatchGraphic *graphic) {
    return func_ov177_021e5678(&graphic->clact);
}

void func_ov177_021e5554(TCB *tcb, void *data) {
    WifiBattleMatchGraphic *graphic = data;

    func_ov177_021e5604(&graphic->bg);
    func_ov177_021e5670(&graphic->clact);
}

void func_ov177_021e5568(WifiBattleMatchGraphicBG *bg, HeapID heapId) {
    u32 i;

    MI_CpuFill8(bg, 0, sizeof(WifiBattleMatchGraphicBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov177_021e8b00);
    for (i = 0; i < NELEMS(data_ov177_021e8b5c); i++) {
        GFL_BGSysCreateBG(data_ov177_021e8b5c[i].bg, &data_ov177_021e8b5c[i].setup, data_ov177_021e8b5c[i].mode);
        GFL_BGSysClearBG(data_ov177_021e8b5c[i].bg);
        GFL_BGSysSetBGEnabled(data_ov177_021e8b5c[i].bg, data_ov177_021e8b5c[i].enabled);
    }
}


void func_ov177_021e55c8(WifiBattleMatchGraphicBG *bg) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov177_021e8b5c); i++) {
        GFL_BGSysReleaseBG(data_ov177_021e8b5c[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(WifiBattleMatchGraphicBG));
}

void func_ov177_021e5600(WifiBattleMatchGraphicBG *bg) {
}

void func_ov177_021e5604(WifiBattleMatchGraphicBG *bg) {
    GFL_BGSysUpdate();
}


void func_ov177_021e560c(WifiBattleMatchGraphicClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    ClActUnit *unit;

    MI_CpuFill8(clact, 0, sizeof(WifiBattleMatchGraphicClAct));
    ClActSys_Create(&data_ov177_021e8b10, vramConfig, heapId);
    unit = func_0204b100(0x80, 0, heapId);
    clact->unit = unit;
    func_0204b270(unit);
    GFL_BGSysSetBGEnabledA(0x10, 1);
    GFL_BGSysSetBGEnabledB(0x10, 1);
}

void func_ov177_021e564c(WifiBattleMatchGraphicClAct *clact) {
    func_0204b1cc(clact->unit);
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(WifiBattleMatchGraphicClAct));
}

void func_ov177_021e5668(WifiBattleMatchGraphicClAct *clact) {
    func_0204a600();
}

void func_ov177_021e5670(WifiBattleMatchGraphicClAct *clact) {
    func_0204a648();
}



ClActUnit *func_ov177_021e5678(WifiBattleMatchGraphicClAct *clact) {
    return clact->unit;
}
