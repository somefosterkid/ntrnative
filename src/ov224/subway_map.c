// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov317/subway_map.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/subway_map.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/fade.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "save/player_info.h"
#include "system/gf_font.h"
#include "system/printsys.h"

// The process that shows the subway map

#define BG_MAIN_MAP 1
#define BG_MAIN_STATIONS 2
#define BG_SUB_MAP 4

// The steps of the process
enum {
    STEP_START,
    STEP_WAIT_FADE_IN,
    STEP_WAIT_INPUT,
    STEP_WAIT_FADE_OUT,
    STEP_END,
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} SubwayMapBGSetup;

typedef struct {
    u16 heapId;
    SubwayMapParam *param;
    SubwayMapGraphic *graphic;
    Font *font;
    PrintQueue *printQueue;
    TCB *vblankTask;
} SubwayMapWork;

static BOOL func_ov224_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov224_021b9718(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov224_021b9788(GameProc *proc, u32 *state, void *param, void *work);
void func_ov224_021b9820(TCB *tcb, void *data);
void func_ov224_021b9824(SubwayMapWork *work);
void func_ov224_021b9898(SubwayMapWork *work);

// pokebw2's SUBWAY_MAP_PROC_FUNCTIONS
// pokebw2's SubwayMap_Exit
// pokebw2's SubwayMap_Init
// pokebw2's SubwayMap_Main
const GameProcFunctions data_ov224_021b9bf8 = { func_ov224_021b95a0, func_ov224_021b9788, func_ov224_021b9718 };

// pokebw2's sSubBGSetups
static const SubwayMapBGSetup data_ov224_021b9c04[1] = {
    { BG_SUB_MAP,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x8000, GX_BG_CHARBASE_0x00000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// The arc file IDs of each palette's NCLR, NCGR and screen data
static u32 data_ov224_021b9d40[2][3] = {
    {0xd, 0xe, 0x3},
    {0x6, 0x7, 0x3},
};



static BOOL func_ov224_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    SubwayMapWork *wk;
    ArcTool *arc;
    HeapID heapId;
    u32 palette;
    int i;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_SUBWAY_MAP, 0x30000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(SubwayMapWork), HEAPID_SUBWAY_MAP);
    MI_CpuFill8(wk, 0, sizeof(SubwayMapWork));
    wk->heapId = HEAPID_SUBWAY_MAP;
    wk->param = param;
    wk->graphic = func_ov224_021b9940(1, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, 0, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    GFL_BGSysSetBGPriority(1, 1);
    GFL_BGSysSetBGPriority(2, 0);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov224_021b9820, wk, 1);
    func_ov224_021b9824(wk);
    G2S_BlendNone();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_DispOn();
    GFL_BGSysDisableAllB();
    GXS_SetGraphicsMode(0);
    GFL_BGSysDisableBGsB();
    for (i = 0; i < NELEMS(data_ov224_021b9c04); i++) {
        GFL_BGSysCreateBG(data_ov224_021b9c04[i].bg, &data_ov224_021b9c04[i].setup, data_ov224_021b9c04[i].mode);
        GFL_BGSysClearBG(data_ov224_021b9c04[i].bg);
        GFL_BGSysSetBGEnabled(data_ov224_021b9c04[i].bg, data_ov224_021b9c04[i].enabled);
    }
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    heapId = wk->heapId;
    palette = TRUE;
    if (wk->param->playerInfo->gender != GENDER_FEMALE) {
        palette = FALSE;
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_C_GEAR, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, data_ov224_021b9d40[palette][0], PALTYPE_SUB_BG, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, data_ov224_021b9d40[palette][1], BG_SUB_MAP, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, data_ov224_021b9d40[palette][2], BG_SUB_MAP, 0, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(BG_SUB_MAP);
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
    func_0203d34c(FALSE, wk->heapId);
    return TRUE;
}

static BOOL func_ov224_021b9718(GameProc *proc, u32 *state, void *param, void *work) {
    SubwayMapWork *wk = work;
    int i;

    for (i = 0; i < NELEMS(data_ov224_021b9c04); i++) {
        GFL_BGSysReleaseBG(data_ov224_021b9c04[i].bg);
    }
    G2S_BlendNone();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    func_ov224_021b9898(wk);
    GFL_TCBRemove(wk->vblankTask);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
    func_ov224_021b99e8(wk->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_SUBWAY_MAP);
    return TRUE;
}

static BOOL func_ov224_021b9788(GameProc *proc, u32 *state, void *param, void *work) {
    SubwayMapWork *wk = work;
    BOOL done;
    u32 x;
    u32 y;

    switch (*state) {
    case STEP_START:
        *state = STEP_WAIT_FADE_IN;
        break;
    case STEP_WAIT_FADE_IN:
        if (!GFL_FadeIsRunning()) {
            *state = STEP_WAIT_INPUT;
        }
        break;
    case STEP_WAIT_INPUT:
        done = FALSE;
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            func_020355d8(FALSE);
            done = TRUE;
        } else if (func_02035d20(&x, &y)) {
            done = TRUE;
            func_020355d8(TRUE);
        }
        if (done) {
            *state = STEP_WAIT_FADE_OUT;
            GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
        }
        break;
    case STEP_WAIT_FADE_OUT:
        if (!GFL_FadeIsRunning()) {
            *state = STEP_END;
        }
        break;
    case STEP_END:
        return TRUE;
    }
    func_0201c4e4(wk->printQueue);
    func_ov224_021b9a44(wk->graphic);
    return FALSE;
}

// pokebw2's SubwayMap_VBlank
void func_ov224_021b9820(TCB *tcb, void *data) {
}

// pokebw2's SubwayMap_LoadGraphics
void func_ov224_021b9824(SubwayMapWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_SUBWAY_MAP, work->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 0, PALTYPE_MAIN_BG, 0, 0, work->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, BG_MAIN_MAP, 0, 0, FALSE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 2, BG_MAIN_MAP, 0, 0, FALSE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 3, BG_MAIN_STATIONS, 0, 0, FALSE, work->heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(BG_MAIN_MAP);
    GFL_BGSysLoadScr(BG_MAIN_STATIONS);
}

// pokebw2's SubwayMap_Unused
void func_ov224_021b9898(SubwayMapWork *work) {
}
