// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov195/gtsnego_disp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// GTS Negotiations' display: the BGs and actors of its screens. The lower screen has the main menu, the search's
// settings with their arrows, or the list of the people of the Pal Pad, which scrolls, with their icons and a scroll
// bar; the top screen shows the search and the partner found. The name is the ROM's string, from GFL_HeapAllocate's
// call. Function names are ours.

#include "types.h"
#include "app/gtsnego_local.h"
#include "app/ui/touchbar.h"
#include "app/ui/ui_scene.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"
#include "save/player_info.h"
#include "system/app_taskmenu.h"
#include "system/blink_palanm.h"
#include "system/bmp_winframe.h"
#include "system/palanm.h"

// The archives of the screens' graphics and of the people's icons, which have no names yet
#define GTSNEGO_ARCID 0x90
#define GTSNEGO_ARCID_AVATAR 0x20

#define GTSNEGO_AVATAR_COUNT 16
#define GTSNEGO_ARROW_COUNT 12

// The tiles of a row of the list of people, and what the list scrolls by a frame
#define GTSNEGO_LIST_ROW_HEIGHT 6
#define GTSNEGO_LIST_SCROLL_SPEED 8

// The scroll bar: where its knob starts, and how far it moves
#define GTSNEGO_KNOB_TOP 26
#define GTSNEGO_KNOB_RANGE 132

// The actors' resources
enum {
    GTSNEGO_RES_PLTT_SUB,
    GTSNEGO_RES_PLTT_MAIN,
    GTSNEGO_RES_PLTT_AVATAR,
    GTSNEGO_RES_PLTT_FOOTER,
    GTSNEGO_RES_CHAR_SUB,
    GTSNEGO_RES_CHAR_MAIN,
    GTSNEGO_RES_CHAR_AVATAR,
    GTSNEGO_RES_CHAR_FOOTER = GTSNEGO_RES_CHAR_AVATAR + GTSNEGO_AVATAR_COUNT,
    GTSNEGO_RES_ANIM_SUB,
    GTSNEGO_RES_ANIM_MAIN,
    GTSNEGO_RES_ANIM_AVATAR,
    GTSNEGO_RES_ANIM_FOOTER = GTSNEGO_RES_ANIM_AVATAR + GTSNEGO_AVATAR_COUNT,
    GTSNEGO_RES_UNUSED,
    GTSNEGO_RES_COUNT,
};

// The actors of the list of people: the scroll bar's knob and the two that are always there
enum {
    GTSNEGO_LIST_ACTOR_KNOB,
    GTSNEGO_LIST_ACTOR_BAR,
    GTSNEGO_LIST_ACTOR_HEADER,
    GTSNEGO_LIST_ACTOR_COUNT,
};

// Black 2's; twinned code confirms the offsets of subChars, mainChars, touchBar, clUnit, res, arrows, heapId,
// blinkMain, blinkSub, scrollPending, listScroll, listScrollDir, listScrollTarget, topScroll, topScrolling, listScreen,
// listScreenFile, searchActors, cursor, listFooter, listActors and friendIcons in Black
struct GtsNegoDisp {
    // The characters of BG 4, BG 0 and BG 7
    u32 subChars;
    u32 mainChars;
    u32 barChars;
    TouchBar *touchBar;
    ClActUnit *clUnit;
    TCB *vblankTask;
    u32 res[GTSNEGO_RES_COUNT];
    ClActor *arrows[GTSNEGO_ARROW_COUNT];
    HeapID heapId;
    BlinkPalAnm *blinkMain;
    BlinkPalAnm *blinkSub;
    // The list of people is scrolled in the V-blank: where it is, the way it goes and where it stops
    BOOL scrollPending;
    int listScroll;
    int listScrollDir;
    int listScrollTarget;
    // The top screen's BG 2 scrolls while it searches
    int topScroll;
    BOOL topScrolling;
    GameData *gameData;
    // The screen of a row of the list, in its file
    NNSG2dScreenData *listScreen;
    void *listScreenFile;
    // The lower screen's palettes, kept while they are dimmed
    void *objPalette;
    void *bgPalette;
    ClActor *searchActors[2];
    ClActor *cursor;
    ClActor *listFooter;
    u32 unk13C;
    ClActor *listActors[GTSNEGO_LIST_ACTOR_COUNT];
    ClActor *friendIcons[GTSNEGO_LIST_ROWS];
};

// Where the cursor goes for a GTSNEGO_CURSOR_*, with its animation there and that of a press
// Black 2's, not checked against Black
typedef struct {
    u8 anim;
    u8 x;
    u8 y;
    u8 pressAnim;
} GtsNegoCursorPos;

static void func_ov120_021d6100(GtsNegoDisp *disp);
void func_ov120_021d6328(GtsNegoDisp *disp, u32 vram, int *starts, int *counts, int count);
static void GtsNegoDisp_DimPalettesVBlank(GtsNegoDisp *disp, u32 vram, int *starts, int *counts, int count);
void func_ov120_021d6530(TCB *tcb, void *work);
static void func_ov120_021d6568(GtsNegoDisp *disp);
static void func_ov120_021d69a0(GtsNegoDisp *disp);
static void func_ov120_021d6af0(GtsNegoDisp *disp, int x, int y, BOOL right);
static void func_ov120_021d6c7c(GtsNegoDisp *disp, int row);
static void func_ov120_021d6ccc(GtsNegoDisp *disp, int row);
void func_ov120_021d6e84(GtsNegoDisp *disp);
static void func_ov120_021d6ef4(GtsNegoDisp *disp, BOOL scrollable);
static int func_ov120_021d72c8(GtsNegoDisp *disp, int pos, int count);
static int func_ov120_021d72e8(GtsNegoDisp *disp, int y, int count);
static void func_ov120_021d73b0(uintptr_t param, fx32 frame);
static void func_ov120_021d73d8(GtsNegoDisp *disp);
static void func_ov120_021d7484(GtsNegoDisp *disp);
static void func_ov120_021d75f4(GtsNegoDisp *disp);

// The sub screen's OBJ extended palettes are in bank I (the SDK's GX_VRAM_SUB_OBJEXTPLTT_0_I)
// pokebw2's sGtsNegoDispVRAMConfig
static BGSysVRAMConfig data_ov120_021d8b98 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_I,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

static const ClActSysSetup data_ov120_021d8974 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 100, 100, 100, 100, 16, 16 };

// pokebw2's sGtsNegoDispLCDConfig
static BGSysLCDConfig data_ov120_021d8b60 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's GtsNegoDisp_Create
GtsNegoDisp *func_ov120_021d5e88(HeapID heapId, GameData *gameData) {
    GtsNegoDisp *disp = GFL_HeapAllocate(heapId, sizeof(GtsNegoDisp), TRUE, "gtsnego_disp.c", 258);

    disp->heapId = heapId;
    disp->gameData = gameData;
    GFL_OvlLoad(OVERLAY_APP_UI);
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    GFL_BGSysSetDisplayLayout(1);
    GXS_DispOn();
    GX_DispOn();
    GFL_BGSysCreate(disp->heapId);
    ClActSys_Create(&data_ov120_021d8974, &data_ov120_021d8b98, disp->heapId);
    disp->clUnit = func_0204b100(64, 0, disp->heapId);
    GFL_BGSysSetVRAMBanks(&data_ov120_021d8b98);
    GFL_BGSysSetLCDConfig(&data_ov120_021d8b60);
    func_ov120_021d6100(disp);
    func_ov120_021d6568(disp);
    disp->vblankTask = GFL_VBlankTCBAdd(func_ov120_021d6530, disp, 0);
    disp->objPalette = GFL_HeapAllocate(disp->heapId, 0x200, FALSE, "gtsnego_disp.c", 280);
    disp->bgPalette = GFL_HeapAllocate(disp->heapId, 0x200, FALSE, "gtsnego_disp.c", 281);
    func_ov120_021d69a0(disp);
    func_ov120_021d692c(disp, 0);
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_ALL);
    GFL_BGSysSetEnabledBGsA(GX_PLANEMASK_ALL);
    return disp;
}

// pokebw2's GtsNegoDisp_Main
void func_ov120_021d5f98(GtsNegoDisp *disp) {
    func_0204a600();
    if (disp->blinkMain != NULL) {
        func_0202ea40(disp->blinkMain);
    }
    if (disp->blinkSub != NULL) {
        func_0202ea40(disp->blinkSub);
    }
    func_ov120_021d75f4(disp);
}

// pokebw2's GtsNegoDisp_Free
void func_ov120_021d5fc4(GtsNegoDisp *disp) {
    int i;

    func_0204b3b4(disp->cursor);
    func_ov120_021d698c(disp);
    func_ov120_021d6ed8(disp);
    func_ov120_021d6c64(disp);
    GFL_HeapFree(disp->objPalette);
    GFL_HeapFree(disp->bgPalette);
    func_ov120_021d6b5c(disp);
    func_0204ada4(disp->res[GTSNEGO_RES_PLTT_SUB]);
    func_0204ada4(disp->res[GTSNEGO_RES_PLTT_MAIN]);
    func_0204a8d4(disp->res[GTSNEGO_RES_CHAR_SUB]);
    func_0204a8d4(disp->res[GTSNEGO_RES_CHAR_MAIN]);
    func_0204afd8(disp->res[GTSNEGO_RES_ANIM_SUB]);
    func_0204afd8(disp->res[GTSNEGO_RES_ANIM_MAIN]);
    func_0204ada4(disp->res[GTSNEGO_RES_PLTT_FOOTER]);
    func_0204a8d4(disp->res[GTSNEGO_RES_CHAR_FOOTER]);
    func_0204afd8(disp->res[GTSNEGO_RES_ANIM_FOOTER]);
    func_0204ada4(disp->res[GTSNEGO_RES_PLTT_AVATAR]);
    for (i = 0; i < GTSNEGO_AVATAR_COUNT; i++) {
        func_0204a8d4(disp->res[GTSNEGO_RES_CHAR_AVATAR + i]);
        func_0204afd8(disp->res[GTSNEGO_RES_ANIM_AVATAR + i]);
    }
    GFL_TCBRemove(disp->vblankTask);
    func_0204b1cc(disp->clUnit);
    func_0204a5ac();
    func_0202ea20(disp->blinkMain);
    func_0202ea20(disp->blinkSub);
    GFL_BGSysFreeCharMemory(7, CHAR_POS(disp->barChars), CHAR_SIZE(disp->barChars));
    GFL_BGSysFreeCharMemory(4, CHAR_POS(disp->subChars), CHAR_SIZE(disp->subChars));
    GFL_BGSysFreeCharMemory(0, CHAR_POS(disp->mainChars), CHAR_SIZE(disp->mainChars));
    GFL_BGSysFreeFilledChar(1, 1, 0);
    GFL_BGSysFreeFilledChar(5, 1, 0);
    GFL_BGSysFreeFilledChar(6, 1, 0);
    GFL_BGSysSetBGEnabled(5, FALSE);
    for (i = 0; i <= 7; i++) {
        GFL_BGSysReleaseBG(i);
    }
    GFL_BGSysFree();
    GFL_HeapFree(disp);
    GFL_OvlUnload(OVERLAY_APP_UI);
}

// pokebw2's GtsNegoDisp_GetClActUnit
ClActUnit *func_ov120_021d60fc(GtsNegoDisp *disp) {
    return disp->clUnit;
}

// pokebw2's GtsNegoDisp_CreateBGs
static void func_ov120_021d6100(GtsNegoDisp *disp) {
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(0);
        GFL_BGSysLoadScr(0);
        GFL_BGSysSetBGEnabled(0, TRUE);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(1, TRUE);
        GFL_BGSysFillChar(1, 0, 1, 0);
        GFL_BGSysFillScrArea(1, 0, 0, 0, 32, 32, 17);
        GFL_BGSysLoadScr(1);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(2, TRUE);
        GFL_BGSysFillScrArea(2, 0, 0, 0, 32, 32, 17);
        GFL_BGSysLoadScr(2);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(3, TRUE);
        GFL_BGSysFillScrArea(3, 0, 0, 0, 32, 32, 17);
        GFL_BGSysLoadScr(3);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x1000,
            0,
            BGRES_256x512,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd000,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(4, TRUE);
        GFL_BGSysLoadScr(4);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(5, TRUE);
        GFL_BGSysFillChar(5, 0, 1, 0);
        GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 32, 17);
        GFL_BGSysLoadScr(5);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x1000,
            0,
            BGRES_256x512,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(6, TRUE);
        GFL_BGSysFillChar(6, 0, 1, 0);
        GFL_BGSysFillScrArea(6, 0, 0, 0, 32, 32, 17);
        GFL_BGSysLoadScr(6);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(7, TRUE);
        GFL_BGSysLoadScr(7);
    }
}

// Dims count runs of palettes of the lower screen's OBJ or BG palettes (PALFADE_VRAM_*), each from a palette of
// starts for as many as in counts
// pokebw2's GtsNegoDisp_DimPalettes
void func_ov120_021d6328(GtsNegoDisp *disp, u32 vram, int *starts, int *counts, int count) {
    int i;
    PaletteFade *fade = func_02021518(disp->heapId);
    u8 *colors;

    func_0202155c(fade, vram, 0x200, disp->heapId);
    func_020216d4(fade, vram, 0, 0x200);
    func_02021fd8(fade, vram, 0, 0x100, 6, 0);
    colors = (u8 *)func_02021730(fade, vram);
    for (i = 0; i < count; i++) {
        switch (vram) {
        case PALFADE_VRAM_SUB_OBJ:
            DC_FlushRange(colors + starts[i] * 32, counts[i] * 32);
            GXS_LoadOBJPltt(colors + starts[i] * 32, starts[i] * 32, counts[i] * 32);
            break;
        case PALFADE_VRAM_SUB_BG:
            DC_FlushRange(colors + starts[i] * 32, counts[i] * 32);
            GXS_LoadBGPltt(colors + starts[i] * 32, starts[i] * 32, counts[i] * 32);
            break;
        }
    }
    func_020215a0(fade, vram);
    func_02021540(fade);
}

void func_ov120_021d63f4(GtsNegoDisp *disp, BOOL dim, u32 type) {
    if (dim) {
        MI_CpuCopy8((void *)HW_DB_OBJ_PLTT, disp->objPalette, 0x200);
        MI_CpuCopy8((void *)HW_DB_BG_PLTT, disp->bgPalette, 0x200);
        switch (type) {
        case GTSNEGO_DIM_SEARCH: {
            int starts[] = { 0 };
            int counts[] = { 16 };

            func_ov120_021d6328(disp, PALFADE_VRAM_SUB_OBJ, starts, counts, NELEMS(starts));
            break;
        }
        case GTSNEGO_DIM_ALL: {
            int starts[] = { 1 };
            int counts[] = { 15 };

            func_ov120_021d6328(disp, PALFADE_VRAM_SUB_OBJ, starts, counts, NELEMS(starts));
            break;
        }
        case GTSNEGO_DIM_LIST_MENU: {
            int starts[] = { 0, 11 };
            int counts[] = { 10, 5 };

            func_ov120_021d6328(disp, PALFADE_VRAM_SUB_OBJ, starts, counts, NELEMS(starts));
            break;
        }
        }
        switch (type) {
        case GTSNEGO_DIM_ALL:
        case GTSNEGO_DIM_SEARCH: {
            int starts[] = { 0, 11 };
            int counts[] = { 9, 4 };

            func_ov120_021d6328(disp, PALFADE_VRAM_SUB_BG, starts, counts, NELEMS(starts));
            break;
        }
        case GTSNEGO_DIM_LIST_MENU: {
            int starts[] = { 0, 11 };
            int counts[] = { 9, 5 };

            func_ov120_021d6328(disp, PALFADE_VRAM_SUB_BG, starts, counts, NELEMS(starts));
            break;
        }
        }
    } else {
        DC_FlushRange(disp->objPalette, 0x200);
        GXS_LoadOBJPltt(disp->objPalette, 0, 0x200);
        DC_FlushRange(disp->bgPalette, 0x200);
        GXS_LoadBGPltt(disp->bgPalette, 0, 0x200);
    }
}


// pokebw2's GtsNegoDisp_VBlank
void func_ov120_021d6530(TCB *tcb, void *work) {
    GtsNegoDisp *disp = work;

    func_0204a648();
    if (disp->scrollPending) {
        GFL_BGSysMoveBG(4, BG_MOVE_SET_Y, disp->listScroll + 72);
        GFL_BGSysMoveBG(6, BG_MOVE_SET_Y, disp->listScroll + 72);
        disp->scrollPending = FALSE;
    }
}

// pokebw2's GtsNegoDisp_LoadGraphics
static void func_ov120_021d6568(GtsNegoDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID, disp->heapId);
    int i = 0;

    GFL_G2DIOLoadArcNCLRDefault(arc, 3, PALTYPE_SUB_BG, 0, 0xc0, disp->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_SUB_BG, 0, 0x40, disp->heapId);
    disp->barChars = GFL_BGSysLoadArcNCGRDynamic(arc, 6, 7, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 17, 7, 0, CHAR_POS(disp->barChars), 0, FALSE, disp->heapId);
    disp->subChars = GFL_BGSysLoadArcNCGRDynamic(arc, 8, 4, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 18, 4, 0, CHAR_POS(disp->subChars), 0, FALSE, disp->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_MAIN_BG, 0, 0, disp->heapId);
    disp->mainChars = GFL_BGSysLoadArcNCGRDynamic(arc, 6, 0, 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 12, 0, 0, CHAR_POS(disp->mainChars), 0, FALSE, disp->heapId);
    disp->res[GTSNEGO_RES_CHAR_FOOTER] = func_0204a6c8(arc, 5, FALSE, CLACT_VRAM_SUB, disp->heapId);
    disp->res[GTSNEGO_RES_PLTT_FOOTER] = func_0204ac18(arc, 0, CLACT_VRAM_SUB, 0x140, 0, 1, disp->heapId);
    disp->res[GTSNEGO_RES_ANIM_FOOTER] = func_0204af28(arc, 10, 22, disp->heapId);
    disp->res[GTSNEGO_RES_CHAR_SUB] = func_0204a6c8(arc, 7, FALSE, CLACT_VRAM_SUB, disp->heapId);
    disp->res[GTSNEGO_RES_PLTT_SUB] = func_0204ac18(arc, 2, CLACT_VRAM_SUB, 0x100, 0, 8, disp->heapId);
    disp->res[GTSNEGO_RES_ANIM_SUB] = func_0204af28(arc, 11, 23, disp->heapId);
    disp->res[GTSNEGO_RES_CHAR_MAIN] = func_0204a6c8(arc, 7, FALSE, CLACT_VRAM_MAIN, disp->heapId);
    disp->res[GTSNEGO_RES_PLTT_MAIN] = func_0204ac18(arc, 2, CLACT_VRAM_MAIN, 0, 0, 8, disp->heapId);
    disp->res[GTSNEGO_RES_ANIM_MAIN] = func_0204af28(arc, 11, 23, disp->heapId);
    disp->blinkMain = func_0202e940(0, 16, 0, disp->heapId);
    func_0202e9ac(disp->blinkMain, arc, 1, 0, 16);
    disp->blinkSub = func_0202e940(0, 16, 7, disp->heapId);
    func_0202e9ac(disp->blinkSub, arc, 1, 0, 16);
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID_AVATAR, disp->heapId);
    for (; i < GTSNEGO_AVATAR_COUNT; i++) {
        disp->res[GTSNEGO_RES_CHAR_AVATAR + i] = func_0204a6c8(arc, 1 + i, FALSE, CLACT_VRAM_SUB, disp->heapId);
        disp->res[GTSNEGO_RES_ANIM_AVATAR + i] = func_0204af28(arc, 17 + i * 2, 18 + i * 2, disp->heapId);
    }
    disp->res[GTSNEGO_RES_PLTT_AVATAR] = func_0204ac18(arc, 0, CLACT_VRAM_SUB, 0, 0, 8, disp->heapId);
    GFL_ArcToolFree(arc);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ,
                      15, 9);
}

// pokebw2's GtsNegoDisp_Dummy
void func_ov120_021d6810(GtsNegoDisp *disp, BOOL list) {
}

// pokebw2's GtsNegoDisp_ShowSearchMenu
void func_ov120_021d6814(GtsNegoDisp *disp) {
    ArcTool *arc;

    func_ov120_021d6810(disp, FALSE);
    arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 19, 4, 0, CHAR_POS(disp->subChars), 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 20, 4, 0, CHAR_POS(disp->subChars), 0xc0, FALSE, disp->heapId);
    GFL_ArcToolFree(arc);
    func_ov120_021d6af0(disp, 96, 56, FALSE);
    func_ov120_021d6af0(disp, 240, 56, TRUE);
    func_ov120_021d6af0(disp, 120, 96, FALSE);
    func_ov120_021d6af0(disp, 240, 96, TRUE);
    func_ov120_021d6af0(disp, 120, 136, FALSE);
    func_ov120_021d6af0(disp, 240, 136, TRUE);
}

// pokebw2's GtsNegoDisp_HideSearchMenu
void func_ov120_021d68c4(GtsNegoDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID, disp->heapId);

    func_ov120_021d6b5c(disp);
    GFL_BGSysFillScrArea(6, 0, 0, 0, 32, 32, 17);
    GFL_BGSysQueueScrLoad(6);
    GFL_G2DIOLoadNSCRSync(arc, 18, 4, 0, CHAR_POS(disp->subChars), 0, FALSE, disp->heapId);
    GFL_BGSysQueueScrLoad(4);
    GFL_ArcToolFree(arc);
}

// The touch bar with its return button, its icons in an OBJ palette
// pokebw2's GtsNegoDisp_CreateTouchBar
void func_ov120_021d692c(GtsNegoDisp *disp, u32 objPalette) {
    TouchBarItem items[] = {
        { 1, { 232, 168 } },
    };
    TouchBarSetup setup;

    MI_CpuFill8(&setup, 0, sizeof(TouchBarSetup));
    setup.items = items;
    setup.count = NELEMS(items);
    setup.unit = disp->clUnit;
    setup.bg = 7;
    setup.bgPalette = 15;
    setup.vramType = CLACT_VRAM_BOTH;
    setup.unk1C = FALSE;
    setup.objPalette = objPalette;
    disp->touchBar = func_ov067_021b62e0(&setup, disp->heapId);
}

// pokebw2's GtsNegoDisp_GetTouchBar
TouchBar *func_ov120_021d6988(GtsNegoDisp *disp) {
    return disp->touchBar;
}

// pokebw2's GtsNegoDisp_FreeTouchBar
void func_ov120_021d698c(GtsNegoDisp *disp) {
    if (disp->touchBar != NULL) {
        func_ov067_021b639c(disp->touchBar);
        disp->touchBar = NULL;
    }
}

// pokebw2's GtsNegoDisp_CreateCursor
static void func_ov120_021d69a0(GtsNegoDisp *disp) {
    ClActorSetup setup;

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 2;
    disp->cursor = func_0204b294(disp->clUnit, disp->res[GTSNEGO_RES_CHAR_SUB], disp->res[GTSNEGO_RES_PLTT_SUB],
                                 disp->res[GTSNEGO_RES_ANIM_SUB], &setup, 1, disp->heapId);
    func_0204b92c(disp->cursor, TRUE);
    func_0204b3dc(disp->cursor, FALSE);
    func_0204b688(disp->cursor, GX_OAM_MODE_XLU);
}

// pokebw2's sGtsNegoDispCursors
static GtsNegoCursorPos data_ov120_021d8b70[] = {
#include "asset/gtsnego_disp_data_ov120_021d8b70.inc"
};

// pokebw2's GtsNegoDisp_SetCursor
void func_ov120_021d69f4(GtsNegoDisp *disp, AppTaskMenuWin *button, int cursor) {
    ClActorPos pos;

    switch (cursor) {
    case GTSNEGO_CURSOR_NONE:
        func_0204b3dc(disp->cursor, FALSE);
        return;
    case GTSNEGO_CURSOR_SEARCH:
        func_0204b3dc(disp->cursor, FALSE);
        if (button != NULL) {
            func_02028218(button, TRUE);
        }
        return;
    case GTSNEGO_CURSOR_LEVEL:
    case GTSNEGO_CURSOR_MY_KIND:
    case GTSNEGO_CURSOR_WANT_KIND:
    case GTSNEGO_CURSOR_FRIEND_1:
    case GTSNEGO_CURSOR_FRIEND_2:
    case GTSNEGO_CURSOR_FRIEND_3:
        if (button != NULL) {
            func_02028218(button, FALSE);
        }
        break;
    }
    pos.x = data_ov120_021d8b70[cursor].x;
    pos.y = data_ov120_021d8b70[cursor].y;
    func_0204b404(disp->cursor, &pos, 1);
    func_0204b878(disp->cursor, data_ov120_021d8b70[cursor].anim);
    func_0204b3dc(disp->cursor, TRUE);
}

// pokebw2's GtsNegoDisp_PressCursor
BOOL func_ov120_021d6a9c(GtsNegoDisp *disp, int cursor) {
    if (disp->cursor == NULL) {
        return FALSE;
    }
    func_0204b878(disp->cursor, data_ov120_021d8b70[cursor].pressAnim);
    return TRUE;
}

// pokebw2's GtsNegoDisp_IsCursorPressDone
BOOL func_ov120_021d6ac0(GtsNegoDisp *disp) {
    if (disp->cursor == NULL) {
        return FALSE;
    }
    if (func_0204b898(disp->cursor) >= 29) {
        if (!func_0204b98c(disp->cursor)) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

// Adds an arrow of the search's settings, pointing left or right
// pokebw2's GtsNegoDisp_AddArrow
static void func_ov120_021d6af0(GtsNegoDisp *disp, int x, int y, BOOL right) {
    int i;

    for (i = 0; i < GTSNEGO_ARROW_COUNT; i++) {
        if (disp->arrows[i] == NULL) {
            ClActorSetup setup;

            setup.x = x;
            setup.y = y;
            setup.sequence = right ? 2 : 0;
            setup.priority = 0;
            setup.bgPriority = 1;
            disp->arrows[i] =
                func_0204b294(disp->clUnit, disp->res[GTSNEGO_RES_CHAR_SUB], disp->res[GTSNEGO_RES_PLTT_SUB],
                              disp->res[GTSNEGO_RES_ANIM_SUB], &setup, 1, disp->heapId);
            func_0204b92c(disp->arrows[i], TRUE);
            func_0204b3dc(disp->arrows[i], TRUE);
            return;
        }
    }
}

// pokebw2's GtsNegoDisp_FreeArrows
void func_ov120_021d6b5c(GtsNegoDisp *disp) {
    int i;

    for (i = 0; i < GTSNEGO_ARROW_COUNT; i++) {
        if (disp->arrows[i] != NULL) {
            func_0204b3b4(disp->arrows[i]);
            disp->arrows[i] = NULL;
        }
    }
}

// Plays an arrow's animation of a press: the one after its own
// pokebw2's GtsNegoDisp_PressArrow
void func_ov120_021d6b84(GtsNegoDisp *disp, int arrow) {
    int sequence = func_0204b898(disp->arrows[arrow]);
    int pressed = 1;

    if (sequence >= 2) {
        pressed = 3;
    }
    func_0204b878(disp->arrows[arrow], pressed);
    func_0204b92c(disp->arrows[arrow], TRUE);
}

// pokebw2's GtsNegoDisp_ShowFriendList
void func_ov120_021d6bb0(GtsNegoDisp *disp, BOOL scrollable) {
    ArcTool *arc;

    func_ov120_021d698c(disp);
    func_ov120_021d692c(disp, 8);
    func_ov120_021d6810(disp, TRUE);
    arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID, disp->heapId);
    disp->listScreenFile = GFL_G2DIOReadNSCRArc(arc, 21, FALSE, &disp->listScreen, disp->heapId);
    GFL_G2DIOLoadArcNCLR(arc, 0, PALTYPE_SUB_OBJ, 0, 0x140, 0x20, disp->heapId);
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID_AVATAR, disp->heapId);
    GFL_G2DIOLoadArcNCLR(arc, 0, PALTYPE_SUB_OBJ, 0, 0, 0x100, disp->heapId);
    GFL_ArcToolFree(arc);
    func_ov120_021d6e84(disp);
    func_ov120_021d6ef4(disp, scrollable);
    disp->scrollPending = TRUE;
}

// pokebw2's GtsNegoDisp_FreeListScreen
void func_ov120_021d6c64(GtsNegoDisp *disp) {
    if (disp->listScreenFile != NULL) {
        GFL_HeapFree(disp->listScreenFile);
        disp->listScreenFile = NULL;
    }
}

// Draws a row of the list of people on BG 4
// pokebw2's GtsNegoDisp_DrawListRow
static void func_ov120_021d6c7c(GtsNegoDisp *disp, int row) {
    u16 *tiles = (u16 *)disp->listScreen->rawData;
    u16 base = CHAR_POS(disp->subChars);
    int i = 0;
    int x;
    int y;

    for (y = 0; y < GTSNEGO_LIST_ROW_HEIGHT; y++) {
        for (x = 0; x < 32; x++) {
            func_02042fb0(4, x, y + row * GTSNEGO_LIST_ROW_HEIGHT, base + tiles[i]);
            i++;
        }
    }
}

// Clears a row of the list, with the tile at the row's top right corner
// pokebw2's GtsNegoDisp_ClearListRow
static void func_ov120_021d6ccc(GtsNegoDisp *disp, int row) {
    u16 *tiles = (u16 *)disp->listScreen->rawData;
    u16 base = CHAR_POS(disp->subChars);
    int x;
    int y;

    for (y = 0; y < GTSNEGO_LIST_ROW_HEIGHT; y++) {
        for (x = 0; x < 32; x++) {
            func_02042fb0(4, x, y + row * GTSNEGO_LIST_ROW_HEIGHT, tiles[31] + base);
        }
    }
}

// Draws the rows of the list that have a person, from the person at top
// pokebw2's GtsNegoDisp_DrawListRows
void func_ov120_021d6d10(GtsNegoDisp *disp, GameData *gameData, int top) {
    int i;

    for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
        if (func_ov120_021d4f70(gameData, top + i) != NULL) {
            func_ov120_021d6c7c(disp, i);
        } else {
            func_ov120_021d6ccc(disp, i);
        }
    }
    GFL_BGSysQueueScrLoad(4);
}

// pokebw2's GtsNegoDisp_LeaveFriendList
void func_ov120_021d6d48(GtsNegoDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID, disp->heapId);
    int i;

    for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
        if (disp->friendIcons[i] != NULL) {
            func_0204b3b4(disp->friendIcons[i]);
            disp->friendIcons[i] = NULL;
        }
    }
    func_ov120_021d698c(disp);
    func_ov120_021d692c(disp, 0);
    func_ov120_021d6ed8(disp);
    GFL_ArcToolFree(arc);
}

// pokebw2's GtsNegoDisp_HideFriendList
void func_ov120_021d6d94(GtsNegoDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID, disp->heapId);

    func_ov120_021d6c64(disp);
    func_ov120_021d6fb4(disp);
    GFL_BGSysFillScrArea(6, 0, 0, 0, 32, 32, 17);
    GFL_BGSysQueueScrLoad(6);
    GFL_G2DIOLoadArcNCLR(arc, 2, PALTYPE_SUB_OBJ, 0, 0x100, 0x100, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 18, 4, 0, CHAR_POS(disp->subChars), 0, FALSE, disp->heapId);
    GFL_BGSysQueueScrLoad(4);
    GFL_ArcToolFree(arc);
}

// pokebw2's GtsNegoDisp_ScrollDown
int func_ov120_021d6e1c(GtsNegoDisp *disp, GtsNegoScroll *scroll) {
    int result = GTSNEGO_SCROLL_NONE;

    if (scroll->cursor < GTSNEGO_LIST_SHOWN - 1 && scroll->cursor < scroll->count - 1) {
        scroll->cursor++;
        return GTSNEGO_SCROLL_CURSOR;
    }
    if (scroll->cursor == GTSNEGO_LIST_SHOWN - 1 && scroll->top + GTSNEGO_LIST_SHOWN < scroll->count) {
        scroll->top++;
        result = GTSNEGO_SCROLL_LIST;
    }
    return result;
}

// pokebw2's GtsNegoDisp_ScrollUp
int func_ov120_021d6e50(GtsNegoDisp *disp, GtsNegoScroll *scroll) {
    int result = GTSNEGO_SCROLL_NONE;

    if (scroll->cursor > 0) {
        scroll->cursor--;
        return GTSNEGO_SCROLL_CURSOR;
    }
    if (scroll->cursor == 0 && scroll->top != 0) {
        scroll->top--;
        result = GTSNEGO_SCROLL_LIST;
    }
    return result;
}

// Puts the cursor on a row of the list that has a person
// pokebw2's GtsNegoDisp_SetScrollCursor
void func_ov120_021d6e70(GtsNegoDisp *disp, GtsNegoScroll *scroll, u32 row) {
    if (row < scroll->count) {
        scroll->cursor = row;
    }
    if (scroll->cursor > GTSNEGO_LIST_SHOWN - 1) {
        scroll->cursor = GTSNEGO_LIST_SHOWN - 1;
    }
}

// pokebw2's GtsNegoDisp_CreateListFooter
void func_ov120_021d6e84(GtsNegoDisp *disp) {
    ClActorSetup setup;

    setup.x = 128;
    setup.y = 200;
    setup.sequence = 0;
    setup.priority = 1;
    setup.bgPriority = 1;
    disp->listFooter = func_0204b294(disp->clUnit, disp->res[GTSNEGO_RES_CHAR_FOOTER],
                                     disp->res[GTSNEGO_RES_PLTT_FOOTER], disp->res[GTSNEGO_RES_ANIM_FOOTER], &setup, 1,
                                     disp->heapId);
    func_0204b92c(disp->listFooter, TRUE);
    func_0204b3dc(disp->listFooter, TRUE);
}

// pokebw2's GtsNegoDisp_FreeListFooter
void func_ov120_021d6ed8(GtsNegoDisp *disp) {
    if (disp->listFooter != NULL) {
        func_0204b3b4(disp->listFooter);
    }
    disp->listFooter = NULL;
}

// The list's actors, without the knob when the list does not scroll
// pokebw2's GtsNegoDisp_CreateListActors
static void func_ov120_021d6ef4(GtsNegoDisp *disp, BOOL scrollable) {
    ClActorSetup setup;
    u16 xs[] = { 250, 250, 128 };
    u16 ys[] = { 96, 96, 16 };
    u16 sequences[] = { 4, 10, 9 };
    u16 bgPriorities[] = { 1, 1, 1 };
    int i;

    for (i = 0; i < GTSNEGO_LIST_ACTOR_COUNT; i++) {
        if (!scrollable && i == GTSNEGO_LIST_ACTOR_KNOB) {
            continue;
        }
        setup.x = xs[i];
        setup.y = ys[i];
        setup.sequence = sequences[i];
        setup.priority = i;
        setup.bgPriority = bgPriorities[i];
        disp->listActors[i] =
            func_0204b294(disp->clUnit, disp->res[GTSNEGO_RES_CHAR_SUB], disp->res[GTSNEGO_RES_PLTT_SUB],
                          disp->res[GTSNEGO_RES_ANIM_SUB], &setup, 1, disp->heapId);
        func_0204b92c(disp->listActors[i], TRUE);
        func_0204b3dc(disp->listActors[i], TRUE);
    }
}

// pokebw2's GtsNegoDisp_FreeListActors
void func_ov120_021d6fb4(GtsNegoDisp *disp) {
    int i;

    for (i = 0; i < GTSNEGO_LIST_ACTOR_COUNT; i++) {
        if (disp->listActors[i] != NULL) {
            func_0204b3b4(disp->listActors[i]);
            disp->listActors[i] = NULL;
        }
    }
}

// pokebw2's GtsNegoDisp_SetKnobAnimating
void func_ov120_021d6fd8(GtsNegoDisp *disp, BOOL animating) {
    if (disp->listActors[GTSNEGO_LIST_ACTOR_KNOB] != NULL) {
        func_0204b9a0(disp->listActors[GTSNEGO_LIST_ACTOR_KNOB]);
        func_0204b92c(disp->listActors[GTSNEGO_LIST_ACTOR_KNOB], animating);
    }
}

// pokebw2's GtsNegoDisp_StartListScroll
void func_ov120_021d6ff8(GtsNegoDisp *disp, int dir) {
    disp->listScrollDir = dir;
    if (dir == GTSNEGO_LIST_UP) {
        disp->listScrollTarget = disp->listScroll + GTSNEGO_LIST_ROW_HEIGHT * 8;
    } else if (dir == GTSNEGO_LIST_DOWN) {
        disp->listScrollTarget = disp->listScroll - GTSNEGO_LIST_ROW_HEIGHT * 8;
    }
}

// pokebw2's GtsNegoDisp_StopListScroll
void func_ov120_021d7020(GtsNegoDisp *disp) {
    disp->listScrollDir = GTSNEGO_LIST_STILL;
}

// Once the list has scrolled a row, the scroll goes back, for the caller to move the rows instead
// pokebw2's GtsNegoDisp_UpdateListScroll
int func_ov120_021d702c(GtsNegoDisp *disp, int *done) {
    int i;

    if (disp->listScrollDir == GTSNEGO_LIST_UP) {
        disp->listScroll += GTSNEGO_LIST_SCROLL_SPEED;
        if (disp->listScrollTarget == disp->listScroll) {
            *done = disp->listScrollDir;
            disp->listScrollDir = GTSNEGO_LIST_STILL;
            disp->listScroll -= GTSNEGO_LIST_ROW_HEIGHT * 8;
        }
        for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
            func_ov120_021d71e8(disp, i, -GTSNEGO_LIST_SCROLL_SPEED);
        }
        disp->scrollPending = TRUE;
    }
    if (disp->listScrollDir == GTSNEGO_LIST_DOWN) {
        disp->listScroll -= GTSNEGO_LIST_SCROLL_SPEED;
        if (disp->listScrollTarget == disp->listScroll) {
            *done = disp->listScrollDir;
            disp->listScrollDir = GTSNEGO_LIST_STILL;
            disp->listScroll += GTSNEGO_LIST_ROW_HEIGHT * 8;
        }
        for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
            func_ov120_021d71e8(disp, i, GTSNEGO_LIST_SCROLL_SPEED);
        }
        disp->scrollPending = TRUE;
    }
    return disp->listScrollDir;
}

// Moves the people's icons a row up or down
// pokebw2's GtsNegoDisp_MoveFriendIcons
void func_ov120_021d70c8(GtsNegoDisp *disp, BOOL up) {
    int dir = 1;
    int i;

    if (up) {
        dir = -1;
    }
    for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
        func_ov120_021d71e8(disp, i, dir * (GTSNEGO_LIST_ROW_HEIGHT * 8));
    }
}

// pokebw2's GtsNegoDisp_ResetList
void func_ov120_021d70f0(GtsNegoDisp *disp) {
    GFL_BGSysMoveBG(4, BG_MOVE_SET_Y, 0);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_Y, 0);
    GFL_BGSysFillScrArea(1, 0, 0, 0, 32, 32, 17);
    GFL_BGSysLoadScr(1);
}

// Creates the icon of a row of the list, hidden, with a trainer's look
// pokebw2's GtsNegoDisp_CreateFriendIcon
void func_ov120_021d7128(GtsNegoDisp *disp, int row, int avatar) {
    ClActorSetup setup;

    if (disp->friendIcons[row] != NULL) {
        func_0204b3b4(disp->friendIcons[row]);
    }
    setup.x = 0;
    setup.y = 0;
    setup.sequence = 1;
    setup.priority = 10;
    setup.bgPriority = 2;
    disp->friendIcons[row] =
        func_0204b294(disp->clUnit, disp->res[GTSNEGO_RES_CHAR_AVATAR + avatar], disp->res[GTSNEGO_RES_PLTT_AVATAR],
                      disp->res[GTSNEGO_RES_ANIM_AVATAR + avatar], &setup, 1, disp->heapId);
    func_0204b92c(disp->friendIcons[row], TRUE);
    func_0204b3dc(disp->friendIcons[row], FALSE);
}

// pokebw2's GtsNegoDisp_ShowFriendIcon
void func_ov120_021d7190(GtsNegoDisp *disp, int row, int x, int y) {
    if (row < GTSNEGO_LIST_ROWS) {
        ClActorPos pos;

        pos.x = x;
        pos.y = y;
        func_0204b404(disp->friendIcons[row], &pos, 1);
        func_0204b3dc(disp->friendIcons[row], TRUE);
    }
}

// The icon of a person in a row of the list, the second row being the first on the screen
// pokebw2's GtsNegoDisp_SetFriendIcon
void func_ov120_021d71b8(GtsNegoDisp *disp, PlayerInfo *friend, int row) {
    if (friend != NULL) {
        func_ov120_021d7128(disp, row, func_02008554(friend));
        func_ov120_021d7190(disp, row, 16, -(GTSNEGO_LIST_ROW_HEIGHT * 8) * (1 - row));
    }
}

// pokebw2's GtsNegoDisp_MoveFriendIcon
void func_ov120_021d71e8(GtsNegoDisp *disp, int row, int y) {
    if (disp->friendIcons[row] != NULL) {
        ClActorPos pos;

        func_0204b454(disp->friendIcons[row], &pos, 1);
        pos.y += y;
        func_0204b404(disp->friendIcons[row], &pos, 1);
    }
}

// pokebw2's GtsNegoDisp_ShiftFriendIconsUp
void func_ov120_021d7220(GtsNegoDisp *disp, PlayerInfo *friend) {
    if (friend != NULL) {
        int i;

        if (disp->friendIcons[0] != NULL) {
            func_0204b3b4(disp->friendIcons[0]);
        }
        for (i = 0; i < GTSNEGO_LIST_ROWS - 1; i++) {
            disp->friendIcons[i] = disp->friendIcons[i + 1];
        }
        disp->friendIcons[GTSNEGO_LIST_ROWS - 1] = NULL;
        func_ov120_021d71b8(disp, friend, GTSNEGO_LIST_ROWS - 1);
    }
}

// pokebw2's GtsNegoDisp_ShiftFriendIconsDown
void func_ov120_021d7260(GtsNegoDisp *disp, PlayerInfo *friend) {
    if (friend != NULL) {
        int i;

        if (disp->friendIcons[GTSNEGO_LIST_ROWS - 1] != NULL) {
            func_0204b3b4(disp->friendIcons[GTSNEGO_LIST_ROWS - 1]);
        }
        for (i = GTSNEGO_LIST_ROWS - 1; i > 0; i--) {
            disp->friendIcons[i] = disp->friendIcons[i - 1];
        }
        disp->friendIcons[0] = NULL;
        func_ov120_021d71b8(disp, friend, 0);
    }
}

// pokebw2's GtsNegoDisp_SetFriendIcons
void func_ov120_021d72a0(GtsNegoDisp *disp, GameData *gameData, int top) {
    int i;

    for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
        if (i + top >= 0) {
            PlayerInfo *friend = func_ov120_021d4f70(gameData, i + top);

            if (friend != NULL) {
                func_ov120_021d71b8(disp, friend, i);
            }
        }
    }
}

// How far down the scroll bar the knob is for a position of count
// pokebw2's GtsNegoDisp_GetKnobY
static int func_ov120_021d72c8(GtsNegoDisp *disp, int pos, int count) {
    if (pos == 0) {
        return 0;
    }
    if (pos == count - 1) {
        return GTSNEGO_KNOB_RANGE;
    }
    return pos * GTSNEGO_KNOB_RANGE / (count - 1);
}

// The first row of the list for the knob at y
// pokebw2's GtsNegoDisp_GetKnobPos
static int func_ov120_021d72e8(GtsNegoDisp *disp, int y, int count) {
    float pos = y - GTSNEGO_KNOB_TOP;
    float range = GTSNEGO_KNOB_RANGE;

    if (count <= GTSNEGO_LIST_SHOWN) {
        return 0;
    }
    pos = pos / (range / (count - 2));
    if (pos >= count - 2) {
        pos = count - 3;
    }
    return pos;
}

// pokebw2's GtsNegoDisp_SetKnob
void func_ov120_021d733c(GtsNegoDisp *disp, int pos, int count) {
    if (disp->listActors[GTSNEGO_LIST_ACTOR_KNOB] != NULL) {
        ClActorPos knob;

        func_0204b454(disp->listActors[GTSNEGO_LIST_ACTOR_KNOB], &knob, 1);
        knob.y = func_ov120_021d72c8(disp, pos, count) + GTSNEGO_KNOB_TOP;
        func_0204b404(disp->listActors[GTSNEGO_LIST_ACTOR_KNOB], &knob, 1);
    }
}

// pokebw2's GtsNegoDisp_DragKnob
int func_ov120_021d7374(GtsNegoDisp *disp, int y, int count) {
    int pos = 0;

    if (disp->listActors[GTSNEGO_LIST_ACTOR_KNOB] != NULL) {
        ClActorPos knob;

        func_0204b454(disp->listActors[GTSNEGO_LIST_ACTOR_KNOB], &knob, 1);
        knob.y = y;
        func_0204b404(disp->listActors[GTSNEGO_LIST_ACTOR_KNOB], &knob, 1);
        pos = func_ov120_021d72e8(disp, y, count);
    }
    return pos;
}

// The search's trainer cycles through the looks, a sequence each
// pokebw2's GtsNegoDisp_OnSearchAnimEnd
static void func_ov120_021d73b0(uintptr_t param, fx32 frame) {
    GtsNegoDisp *disp = (GtsNegoDisp *)param;
    u16 sequence = func_0204b898(disp->searchActors[1]) + 1;

    if (sequence > 26) {
        sequence = 11;
    }
    func_0204b878(disp->searchActors[1], sequence);
}

// pokebw2's GtsNegoDisp_CreateSearchActors
static void func_ov120_021d73d8(GtsNegoDisp *disp) {
    ClActorSetup setup;
    ClActorCallback callback;

    setup.x = 128;
    setup.y = 52;
    setup.sequence = 27;
    setup.priority = 2;
    setup.bgPriority = 1;
    disp->searchActors[0] =
        func_0204b294(disp->clUnit, disp->res[GTSNEGO_RES_CHAR_MAIN], disp->res[GTSNEGO_RES_PLTT_MAIN],
                      disp->res[GTSNEGO_RES_ANIM_MAIN], &setup, 0, disp->heapId);
    func_0204b92c(disp->searchActors[0], TRUE);
    func_0204b3dc(disp->searchActors[0], TRUE);
    setup.sequence = 11;
    setup.priority = 1;
    disp->searchActors[1] =
        func_0204b294(disp->clUnit, disp->res[GTSNEGO_RES_CHAR_MAIN], disp->res[GTSNEGO_RES_PLTT_MAIN],
                      disp->res[GTSNEGO_RES_ANIM_MAIN], &setup, 0, disp->heapId);
    func_0204b92c(disp->searchActors[1], TRUE);
    func_0204b3dc(disp->searchActors[1], TRUE);
    callback.type = CLACT_CALLBACK_LAST_FRAME;
    callback.param = (uintptr_t)disp;
    callback.func = func_ov120_021d73b0;
    func_0204ba00(disp->searchActors[1], &callback);
    GFL_BGSysSetEnabledBGsA(GX_PLANEMASK_ALL);
}

// pokebw2's GtsNegoDisp_FreeSearchActors
static void func_ov120_021d7484(GtsNegoDisp *disp) {
    func_0204b3dc(disp->searchActors[0], FALSE);
    func_0204b3b4(disp->searchActors[0]);
    func_0204b3dc(disp->searchActors[1], FALSE);
    func_0204b3b4(disp->searchActors[1]);
}

// Stops the search's trainer on the partner's look
// pokebw2's GtsNegoDisp_ShowPartner
void func_ov120_021d74b0(GtsNegoDisp *disp, int avatar) {
    int sequences[GTSNEGO_AVATAR_COUNT] = {
#include "asset/gtsnego_disp_sequences_021d74b0.inc"
    };
    ArcTool *arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID, disp->heapId);

    GFL_G2DIOLoadNSCRSync(arc, 15, 0, 0, CHAR_POS(disp->mainChars), 0, FALSE, disp->heapId);
    GFL_ArcToolFree(arc);
    func_0204b92c(disp->searchActors[1], FALSE);
    func_0204b878(disp->searchActors[1], sequences[avatar] + 11);
    func_0204b908(disp->searchActors[1], 4);
    func_0204b878(disp->searchActors[0], 28);
    GFL_BGSysQueueScrLoad(0);
    disp->topScroll = 0;
    disp->topScrolling = FALSE;
}

// pokebw2's GtsNegoDisp_ShowSearching
void func_ov120_021d7550(GtsNegoDisp *disp) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID, disp->heapId);

    GFL_G2DIOLoadNSCRSync(arc, 14, 2, 0, CHAR_POS(disp->mainChars), 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 13, 0, 0, CHAR_POS(disp->mainChars), 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 16, 3, 0, CHAR_POS(disp->mainChars), 0, FALSE, disp->heapId);
    GFL_ArcToolFree(arc);
    func_ov120_021d73d8(disp);
    GFL_BGSysQueueScrLoad(2);
    GFL_BGSysQueueScrLoad(0);
    GFL_BGSysQueueScrLoad(3);
    disp->topScroll = 0;
    disp->topScrolling = TRUE;
}

// pokebw2's GtsNegoDisp_UpdateTopScroll
static void func_ov120_021d75f4(GtsNegoDisp *disp) {
    if (disp->topScrolling) {
        GFL_BGSysMoveBG(2, BG_MOVE_SET_Y, disp->topScroll);
        disp->topScroll++;
    }
}

// pokebw2's GtsNegoDisp_Reset
void func_ov120_021d761c(GtsNegoDisp *disp) {
    ArcTool *arc;
    int i;

    for (i = 0; i < GTSNEGO_LIST_ROWS; i++) {
        if (disp->friendIcons[i] != NULL) {
            func_0204b3dc(disp->friendIcons[i], FALSE);
            func_0204b3b4(disp->friendIcons[i]);
            disp->friendIcons[i] = NULL;
        }
    }
    func_ov120_021d6ed8(disp);
    func_ov120_021d6c64(disp);
    func_ov120_021d7484(disp);
    func_ov120_021d6fb4(disp);
    func_ov120_021d6b5c(disp);
    arc = GFL_ArcSysCreateFileHandle(GTSNEGO_ARCID, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 12, 0, 0, CHAR_POS(disp->mainChars), 0, FALSE, disp->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 18, 4, 0, CHAR_POS(disp->subChars), 0, FALSE, disp->heapId);
    GFL_G2DIOLoadArcNCLR(arc, 2, PALTYPE_SUB_OBJ, 0, 0x100, 0x100, disp->heapId);
    GFL_ArcToolFree(arc);
    func_ov120_021d698c(disp);
    func_ov120_021d692c(disp, 0);
    GFL_BGSysSetBGEnabled(2, FALSE);
    GFL_BGSysQueueScrLoad(0);
    GFL_BGSysSetBGEnabled(3, FALSE);
    disp->topScroll = 0;
    disp->topScrolling = FALSE;
}
