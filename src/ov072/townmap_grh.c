// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov144/townmap_grh.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/townmap.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

// The BGs and cell actors of the town map. It starts at func_ov072_021bb6e8, as its sibling graphic files start at
// theirs and as the first use of its name shows; the empty VBlank task before it is townmap.c's, which adds it with
// its own work

// The resources of the map's actors
enum {
    TOWNMAP_RES_CHARS,
    TOWNMAP_RES_CELL_ANIMS,
    TOWNMAP_RES_PLTT,
    TOWNMAP_RES_COUNT
};

typedef struct {
    u32 unk0;
} TownMapBG;

typedef struct {
    ClActUnit *units[TOWNMAP_UNIT_COUNT];
    ClActor *actors[TOWNMAP_ACTOR_COUNT];
    u32 unk14;
    u32 res[TOWNMAP_RES_COUNT];
} TownMapClAct;

struct TownMapGraphic {
    TownMapBG bg;
    TownMapClAct clact;
    TCB *vblankTask;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
} TownMapBGSetup;

static void func_ov072_021bb7bc(void);
static void func_ov072_021bb80c(TCB *tcb, void *data);
static void func_ov072_021bb820(TownMapBG *bg, HeapID heapId);
static void func_ov072_021bb968(TownMapBG *bg);
static void func_ov072_021bb9b0(TownMapBG *bg);
static void func_ov072_021bb9b8(TownMapClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId);
static void func_ov072_021bbab4(TownMapClAct *clact);
static void func_ov072_021bbafc(TownMapClAct *clact);
static void func_ov072_021bbb04(TownMapClAct *clact);
static ClActor *func_ov072_021bbb0c(TownMapClAct *clact, u32 index);
static ClActUnit *func_ov072_021bbb14(TownMapClAct *clact, u32 index);
static u32 func_ov072_021bbb1c(TownMapClAct *clact, u32 index);

// pokebw2's sTownMapLCDConfig
static const BGSysLCDConfig data_ov072_021bbbcc = { GX_DISPMODE_GRAPHICS, GX_BGMODE_5, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sTownMapVRAMConfig
static const BGSysVRAMConfig data_ov072_021bbbdc = {
    GX_VRAM_A | GX_VRAM_D, GX_VRAM_E,               GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B,     GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,      GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// In the order of TOWNMAP_BG_*
// pokebw2's sTownMapBGSetups
static const TownMapBGSetup data_ov072_021bbc28[TOWNMAP_BG_COUNT] = {
    { 0,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 1,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 2,
      { 0, 0, 0x1000, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_EXTENDED },
    { 3,
      { 0, 0, 0x1000, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x18000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_EXTENDED },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
    { 5,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT },
};

// pokebw2's TownMapGraphic_Create
TownMapGraphic *func_ov072_021bb6e8(HeapID heapId) {
    TownMapGraphic *graphic = GFL_HeapAllocate(heapId, sizeof(TownMapGraphic), FALSE, "townmap_grh.c", 236);

    MI_CpuFill8(graphic, 0, sizeof(TownMapGraphic));
    func_ov072_021bb7bc();
    func_ov072_021bb820(&graphic->bg, heapId);
    func_ov072_021bb9b8(&graphic->clact, &data_ov072_021bbbdc, heapId);
    graphic->vblankTask = GFL_VBlankTCBAdd(func_ov072_021bb80c, graphic, 1);
    return graphic;
}

// pokebw2's TownMapGraphic_Free
void func_ov072_021bb738(TownMapGraphic *graphic) {
    GFL_TCBRemove(graphic->vblankTask);
    func_ov072_021bbab4(&graphic->clact);
    func_ov072_021bb968(&graphic->bg);
    GFL_HeapFree(graphic);
}

// pokebw2's TownMapGraphic_Update
void func_ov072_021bb758(TownMapGraphic *graphic) {
    func_ov072_021bbafc(&graphic->clact);
}

// pokebw2's TownMapGraphic_GetBG
u8 func_ov072_021bb764(TownMapGraphic *graphic, u32 index) {
    return data_ov072_021bbc28[index].bg;
}

// pokebw2's TownMapGraphic_GetUnit
ClActUnit *func_ov072_021bb778(TownMapGraphic *graphic, u32 index) {
    return func_ov072_021bbb14(&graphic->clact, index);
}

// pokebw2's TownMapGraphic_GetActor
ClActor *func_ov072_021bb784(TownMapGraphic *graphic, u32 index) {
    return func_ov072_021bbb0c(&graphic->clact, index);
}

// pokebw2's TownMapGraphic_GetRes
void func_ov072_021bb790(TownMapGraphic *graphic, u32 *chars, u32 *cellAnims, u32 *pltt) {
    *chars = func_ov072_021bbb1c(&graphic->clact, TOWNMAP_RES_CHARS);
    *cellAnims = func_ov072_021bbb1c(&graphic->clact, TOWNMAP_RES_CELL_ANIMS);
    *pltt = func_ov072_021bbb1c(&graphic->clact, TOWNMAP_RES_PLTT);
}

// pokebw2's TownMapGraphic_InitDisplay
static void func_ov072_021bb7bc(void) {
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov072_021bbbdc);
    GFL_BGSysSetDisplayLayout(0);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    func_0201dd60();
}

// pokebw2's TownMapGraphic_VBlank
static void func_ov072_021bb80c(TCB *tcb, void *data) {
    TownMapGraphic *graphic = data;

    func_ov072_021bbb04(&graphic->clact);
    func_ov072_021bb9b0(&graphic->bg);
}

// pokebw2's TownMapBG_Init
static void func_ov072_021bb820(TownMapBG *bg, HeapID heapId) {
    int i;
    ArcTool *arc;

    MI_CpuFill8(bg, 0, sizeof(TownMapBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov072_021bbbcc);
    for (i = 0; i < TOWNMAP_BG_COUNT; i++) {
        GFL_BGSysCreateBG(data_ov072_021bbc28[i].bg, &data_ov072_021bbc28[i].setup, data_ov072_021bbc28[i].mode);
        GFL_BGSysSetBGEnabled(data_ov072_021bbc28[i].bg, TRUE);
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_TOWNMAP_GRA, heapId);
    // The map's 256-color palettes, in the slots of BG3 and BG2
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 2, 0x6000, 0x2000, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 2, 0x4000, 0x2000, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 5, 2, 0, 0x10000, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 7, 3, 0, 0x10000, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 17, 2, 0, 0x800, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 16, 3, 0, 0x800, FALSE, heapId);
    // The upper screen's background
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 4, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 6, 5, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 15, 5, 0, 0, FALSE, heapId);
    // The font's palette, as palette 14 of both screens
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 14 * 0x20, 0x20, heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 14 * 0x20, 0x20, heapId);
    GFL_BGSysFillChar(1, 0, 1, 0);
    GFL_ArcToolFree(arc);
}

// pokebw2's TownMapBG_Free
static void func_ov072_021bb968(TownMapBG *bg) {
    int i;

    GFL_BGSysFreeFilledChar(1, 1, 0);
    for (i = 0; i < TOWNMAP_BG_COUNT; i++) {
        GFL_BGSysClearBG(data_ov072_021bbc28[i].bg);
        GFL_BGSysReleaseBG(data_ov072_021bbc28[i].bg);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(bg, 0, sizeof(TownMapBG));
}

// pokebw2's TownMapBG_VBlank
static void func_ov072_021bb9b0(TownMapBG *bg) {
    GFL_BGSysUpdate();
}

// pokebw2's TownMapClAct_Init
static void func_ov072_021bb9b8(TownMapClAct *clact, const BGSysVRAMConfig *vramConfig, HeapID heapId) {
    int i;
    ArcTool *arc;

    MI_CpuFill8(clact, 0, sizeof(TownMapClAct));
    {
        ClActSysSetup setup = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 32, 32, 32, 32, 16, 16 };

        ClActSys_Create(&setup, vramConfig, heapId);
    }
    clact->units[TOWNMAP_UNIT_MAIN] = func_0204b100(128, 0, heapId);
    func_0204b270(clact->units[TOWNMAP_UNIT_MAIN]);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    arc = GFL_ArcSysCreateFileHandle(ARCID_TOWNMAP_GRA, heapId);
    clact->res[TOWNMAP_RES_PLTT] = func_0204abf0(arc, 2, CLACT_VRAM_MAIN, 0, heapId);
    clact->res[TOWNMAP_RES_CHARS] = func_0204a6c8(arc, 8, FALSE, CLACT_VRAM_MAIN, heapId);
    clact->res[TOWNMAP_RES_CELL_ANIMS] = func_0204af28(arc, 12, 19, heapId);
    GFL_ArcToolFree(arc);
    {
        ClActorSetup setup;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        for (i = 0; i < TOWNMAP_ACTOR_COUNT; i++) {
            clact->actors[i] =
                func_0204b294(clact->units[TOWNMAP_UNIT_MAIN], clact->res[TOWNMAP_RES_CHARS],
                              clact->res[TOWNMAP_RES_PLTT], clact->res[TOWNMAP_RES_CELL_ANIMS], &setup, 1, heapId);
        }
    }
    func_0204b878(clact->actors[TOWNMAP_ACTOR_NAME_PLATE], 0);
    func_0204b878(clact->actors[TOWNMAP_ACTOR_CURSOR], 4);
    func_0204b878(clact->actors[TOWNMAP_ACTOR_RING], 5);
    func_0204b878(clact->actors[TOWNMAP_ACTOR_PLAYER], 6);
    for (i = 0; i < TOWNMAP_ACTOR_COUNT; i++) {
        func_0204b3dc(clact->actors[i], FALSE);
    }
}

// pokebw2's TownMapClAct_Free
static void func_ov072_021bbab4(TownMapClAct *clact) {
    int i;

    for (i = 0; i < TOWNMAP_ACTOR_COUNT; i++) {
        if (clact->actors[i] != NULL) {
            func_0204b3b4(clact->actors[i]);
        }
    }
    func_0204afd8(clact->res[TOWNMAP_RES_CELL_ANIMS]);
    func_0204a8d4(clact->res[TOWNMAP_RES_CHARS]);
    func_0204ada4(clact->res[TOWNMAP_RES_PLTT]);
    if (clact->units[TOWNMAP_UNIT_MAIN] != NULL) {
        func_0204b1cc(clact->units[TOWNMAP_UNIT_MAIN]);
    }
    func_0204a5ac();
    MI_CpuFill8(clact, 0, sizeof(TownMapClAct));
}

// pokebw2's TownMapClAct_Update
static void func_ov072_021bbafc(TownMapClAct *clact) {
    func_0204a600();
}

// pokebw2's TownMapClAct_VBlank
static void func_ov072_021bbb04(TownMapClAct *clact) {
    func_0204a648();
}

// pokebw2's TownMapClAct_GetActor
static ClActor *func_ov072_021bbb0c(TownMapClAct *clact, u32 index) {
    return clact->actors[index];
}

// pokebw2's TownMapClAct_GetUnit
static ClActUnit *func_ov072_021bbb14(TownMapClAct *clact, u32 index) {
    return clact->units[index];
}

// pokebw2's TownMapClAct_GetRes
static u32 func_ov072_021bbb1c(TownMapClAct *clact, u32 index) {
    return clact->res[index];
}
