// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov271/br_graphic.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's graphics system: the BG system and its seven BGs, the cell actor system and the VBlank task
// that transfers both. The name is the ROM's string, from GFL_HeapAllocate's asserts

#include "types.h"
#include "app/battle_recorder/br_graphic.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "system/gf_font.h"

#define BR_GRAPHIC_BG_NUM 7

// A BG the Battle Recorder sets up
typedef struct {
    u32 frame;
    BGSetup setup;
    u32 mode;
    BOOL isVisible;
} BrGraphicBGData;

typedef struct {
    u32 dummy;
} BrGraphicBG;

typedef struct {
    ClActUnit *unit;
} BrGraphicOBJ;

struct BrGraphic {
    BrGraphicBG bg;
    BrGraphicOBJ obj;
    // Unused here; the slot of a 3D system in the other apps' graphics works
    u32 unk8;
    TCB *vblankTask;
};

static void func_ov186_021fc4e8(TCB *tcb, void *p_wk_adrs);
static void func_ov186_021fc4fc(BrGraphicBG *p_wk, HeapID heapId);
static void func_ov186_021fc55c(BrGraphicBG *p_wk);
static void func_ov186_021fc594(BrGraphicBG *p_wk);
static void func_ov186_021fc598(BrGraphicBG *p_wk);
static void func_ov186_021fc5a0(BrGraphicOBJ *p_wk, const BGSysVRAMConfig *cp_vram, HeapID heapId);
static void func_ov186_021fc5e0(BrGraphicOBJ *p_wk);
static void func_ov186_021fc5fc(BrGraphicOBJ *p_wk);
static void func_ov186_021fc604(BrGraphicOBJ *p_wk);
static ClActUnit *func_ov186_021fc60c(const BrGraphicOBJ *p_wk);

// pokebw2's sc_bgsys_header
static const BGSysLCDConfig data_ov186_02200900 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
const BGSysLCDConfig data_ov186_02200910 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

// pokebw2's sc_clsys_init
static const ClActSysSetup data_ov186_02200920 = {
    0, 0, 0, 512, 4, 124, 4, 124, 0, 64, 32, 64, 64, 16, 16,
};

// The VRAM banks of the Battle Recorder, and of its musical photos, which use 3D
// pokebw2's sc_vram_bank
static const BGSysVRAMConfig data_ov186_0220093c = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,      GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};
// pokebw2's sc_vram_bank_musical
static const BGSysVRAMConfig data_ov186_0220096c = {
    GX_VRAM_BG_128_D,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_32_FG, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_AB, GX_VRAM_TEXPLTT_0123_E,  GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// The BGs: the main engine's four and the sub engine's first three
// pokebw2's sc_bgcnt_data
static const BrGraphicBGData data_ov186_0220099c[BR_GRAPHIC_BG_NUM] = {
    { 0,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
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
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x14000, 0x8000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 5,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, 0x8000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { 6,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, 0x8000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// pokebw2's BrGraphic_Init
BrGraphic *func_ov186_021fc3bc(u32 type, u32 displayLayout, HeapID heapId) {
    BrGraphic *p_wk = GFL_HeapAllocate(heapId, sizeof(BrGraphic), FALSE, "br_graphic.c", 469);
    const BGSysVRAMConfig *cp_vram;

    MI_CpuFill8(p_wk, 0, sizeof(BrGraphic));
    switch (type) {
    case BR_GRAPHIC_TYPE_NORMAL:
        cp_vram = &data_ov186_0220093c;
        break;
    case BR_GRAPHIC_TYPE_MUSICAL:
        cp_vram = &data_ov186_0220096c;
        break;
    }

    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GFL_BGSysInitVRAM(0);
    GFL_BGSysSetVRAMBanks(cp_vram);
    GFL_BGSysSetDisplayLayout(displayLayout);
    GFL_BGSysEnableEngines();
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    func_0201dd60();

    func_ov186_021fc4fc(&p_wk->bg, heapId);
    func_ov186_021fc5a0(&p_wk->obj, cp_vram, heapId);
    p_wk->vblankTask = GFL_VBlankTCBAdd(func_ov186_021fc4e8, p_wk, 0);
    return p_wk;
}

// pokebw2's BrGraphic_Exit
void func_ov186_021fc474(BrGraphic *p_wk) {
    GFL_TCBRemove(p_wk->vblankTask);
    func_ov186_021fc5e0(&p_wk->obj);
    func_ov186_021fc55c(&p_wk->bg);
    func_0201dd68();
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    MI_CpuFill8(p_wk, 0, sizeof(BrGraphic));
    GFL_HeapFree(p_wk);
}

// pokebw2's BrGraphic_Main
void func_ov186_021fc4c8(BrGraphic *p_wk) {
    func_ov186_021fc5fc(&p_wk->obj);
    func_ov186_021fc594(&p_wk->bg);
}

// pokebw2's BrGraphic_GetClunit
ClActUnit *func_ov186_021fc4dc(const BrGraphic *p_wk) {
    return func_ov186_021fc60c(&p_wk->obj);
}

// pokebw2's BrGraphic_VBlankTask
static void func_ov186_021fc4e8(TCB *tcb, void *p_wk_adrs) {
    BrGraphic *p_wk = p_wk_adrs;

    func_ov186_021fc598(&p_wk->bg);
    func_ov186_021fc604(&p_wk->obj);
}

// pokebw2's BrGraphic_BG_Init
static void func_ov186_021fc4fc(BrGraphicBG *p_wk, HeapID heapId) {
    u32 i;

    MI_CpuFill8(p_wk, 0, sizeof(BrGraphicBG));
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysSetLCDConfig(&data_ov186_02200900);
    for (i = 0; i < BR_GRAPHIC_BG_NUM; i++) {
        GFL_BGSysCreateBG(data_ov186_0220099c[i].frame, &data_ov186_0220099c[i].setup, data_ov186_0220099c[i].mode);
        GFL_BGSysClearBG(data_ov186_0220099c[i].frame);
        GFL_BGSysSetBGEnabled(data_ov186_0220099c[i].frame, data_ov186_0220099c[i].isVisible);
    }
}

// Not called in the Battle Recorder: the 3D variant of the BG setup, whose config stays in .rodata
static void BrGraphic_BG_SetLCDConfig3D(void) {
    GFL_BGSysSetLCDConfig(&data_ov186_02200910);
}

// pokebw2's BrGraphic_BG_Exit
static void func_ov186_021fc55c(BrGraphicBG *p_wk) {
    u32 i;

    for (i = 0; i < BR_GRAPHIC_BG_NUM; i++) {
        GFL_BGSysReleaseBG(data_ov186_0220099c[i].frame);
    }
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    MI_CpuFill8(p_wk, 0, sizeof(BrGraphicBG));
}

// pokebw2's BrGraphic_BG_Main
static void func_ov186_021fc594(BrGraphicBG *p_wk) {
}

// pokebw2's BrGraphic_BG_VBlank
static void func_ov186_021fc598(BrGraphicBG *p_wk) {
    GFL_BGSysUpdate();
}

// pokebw2's BrGraphic_OBJ_Init
static void func_ov186_021fc5a0(BrGraphicOBJ *p_wk, const BGSysVRAMConfig *cp_vram, HeapID heapId) {
    MI_CpuFill8(p_wk, 0, sizeof(BrGraphicOBJ));
    ClActSys_Create(&data_ov186_02200920, cp_vram, heapId);
    p_wk->unit = func_0204b100(128, 0, heapId);
    func_0204b270(p_wk->unit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's BrGraphic_OBJ_Exit
static void func_ov186_021fc5e0(BrGraphicOBJ *p_wk) {
    func_0204b1cc(p_wk->unit);
    func_0204a5ac();
    MI_CpuFill8(p_wk, 0, sizeof(BrGraphicOBJ));
}

// pokebw2's BrGraphic_OBJ_Main
static void func_ov186_021fc5fc(BrGraphicOBJ *p_wk) {
    func_0204a600();
}

// pokebw2's BrGraphic_OBJ_VBlank
static void func_ov186_021fc604(BrGraphicOBJ *p_wk) {
    func_0204a648();
}

// pokebw2's BrGraphic_OBJ_GetClunit
static ClActUnit *func_ov186_021fc60c(const BrGraphicOBJ *p_wk) {
    return p_wk->unit;
}

// Releases the main engine's BGs and makes BG0 3D, for the musical photos
// pokebw2's BrGraphic_StartMain3D
void func_ov186_021fc610(BrGraphic *p_wk, HeapID heapId) {
    u32 i;

    for (i = 0; i < BR_GRAPHIC_BG_NUM; i++) {
        if (data_ov186_0220099c[i].frame < BGSYS_BG_SUB) {
            GFL_BGSysReleaseBG(data_ov186_0220099c[i].frame);
        }
    }
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
}

// pokebw2's BrGraphic_EndMain3D
void func_ov186_021fc640(BrGraphic *p_wk, HeapID heapId) {
    u32 i;

    G2_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    for (i = 0; i < BR_GRAPHIC_BG_NUM; i++) {
        if (data_ov186_0220099c[i].frame < BGSYS_BG_SUB) {
            GFL_BGSysCreateBG(data_ov186_0220099c[i].frame, &data_ov186_0220099c[i].setup, data_ov186_0220099c[i].mode);
            GFL_BGSysClearBG(data_ov186_0220099c[i].frame);
        }
        GFL_BGSysSetBGEnabled(data_ov186_0220099c[i].frame, data_ov186_0220099c[i].isVisible);
    }
}
