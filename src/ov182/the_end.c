// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov267/the_end.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/version.h"
#include "demo/the_end.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/version.h"

// The process that shows the picture. It releases the BG 4 of its table, which it does not create

#define BG_PICTURE 1
#define BG_UNUSED 2

// Frames before the picture is shown again, which the steps that use it never reach
#define WAIT_FRAMES 900

// The steps of the process
enum {
    STEP_START,
    STEP_WAIT_FADE_IN_A,
    STEP_WAIT_TIME,
    STEP_WAIT_FADE_OUT,
    STEP_WAIT_FADE_IN,
    STEP_WAIT_INPUT,
    STEP_WAIT_FADE_OUT_END,
    STEP_END,
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} TheEndBGSetup;

typedef struct {
    u16 heapId;
    TheEndGraphic *graphic;
    Font *font;
    PrintQueue *printQueue;
    void *param;
    TCB *vblankTask;
    s32 counter;
} TheEndWork;

BOOL func_ov182_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov182_021b964c(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov182_021b96bc(GameProc *proc, u32 *state, void *param, void *work);
void func_ov182_021b9784(TCB *tcb, void *data);
void func_ov182_021b9788(TheEndWork *work);
void func_ov182_021b9810(TheEndWork *work);
void func_ov182_021b9814(int start, int end, int slowness);

// pokebw2's THE_END_PROC_FUNCTIONS
// pokebw2's TheEnd_Exit
// pokebw2's TheEnd_Init
// pokebw2's TheEnd_Main
const GameProcFunctions data_ov182_021b9ba0 = { func_ov182_021b95a0, func_ov182_021b96bc, func_ov182_021b964c };

// pokebw2's sUnusedBGSetups
static const TheEndBGSetup data_ov182_021b9bac[1] = {
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x8000, GX_BG_CHARBASE_0x00000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// Counts a frame, and whether the time to wait is over
static inline BOOL TheEnd_CountFrame(TheEndWork *work) {
    BOOL over = FALSE;

    if (++work->counter > WAIT_FRAMES) {
        over = TRUE;
    }
    return over;
}


BOOL func_ov182_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    u16 color[1] = { 0xffff };
    TheEndWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_THE_END, 0x30000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(TheEndWork), HEAPID_THE_END);
    MI_CpuFill8(wk, 0, sizeof(TheEndWork));
    wk->heapId = HEAPID_THE_END;
    wk->param = param;
    wk->graphic = func_ov182_021b98e4(1, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, 0, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    GFL_BGSysSetBGPriority(BG_PICTURE, 1);
    GFL_BGSysSetBGPriority(BG_UNUSED, 0);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov182_021b9784, wk, 1);
    func_ov182_021b9788(wk);
    if (getGameVersion() == VERSION_WHITE) {
        GXS_LoadBGPltt(color, 0, 2);
    }
    func_ov182_021b9814(16, 0, 0);
    return TRUE;
}

BOOL func_ov182_021b964c(GameProc *proc, u32 *state, void *param, void *work) {
    TheEndWork *wk = work;
    int i;

    for (i = 0; i < NELEMS(data_ov182_021b9bac); i++) {
        GFL_BGSysReleaseBG(data_ov182_021b9bac[i].bg);
    }
    G2S_BlendNone();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    func_ov182_021b9810(wk);
    GFL_TCBRemove(wk->vblankTask);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
    func_ov182_021b998c(wk->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_THE_END);
    return TRUE;
}

BOOL func_ov182_021b96bc(GameProc *proc, u32 *state, void *param, void *work) {
    TheEndWork *wk = work;

    switch (*state) {
    case STEP_START:
        *state = STEP_WAIT_FADE_IN;
        break;
    case STEP_WAIT_FADE_IN_A:
        if (!GFL_FadeIsRunning()) {
            *state = STEP_WAIT_TIME;
        }
        break;
    case STEP_WAIT_TIME:
        if (TheEnd_CountFrame(wk)) {
            *state = STEP_WAIT_FADE_OUT;
            func_ov182_021b9814(0, 16, 0);
        }
        break;
    case STEP_WAIT_FADE_OUT:
        if (!GFL_FadeIsRunning()) {
            func_ov182_021b9788(wk);
            func_ov182_021b9814(16, 0, 0);
            *state = STEP_WAIT_FADE_IN;
        }
        break;
    case STEP_WAIT_FADE_IN:
        if (!GFL_FadeIsRunning()) {
            wk->counter = 0;
            *state = STEP_WAIT_INPUT;
        }
        break;
    case STEP_WAIT_INPUT:
        if (!(GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START))) {
            if (++wk->counter <= WAIT_FRAMES) {
                break;
            }
        }
        *state = STEP_WAIT_FADE_OUT_END;
        func_ov182_021b9814(0, 16, 0);
        break;
    case STEP_WAIT_FADE_OUT_END:
        if (!GFL_FadeIsRunning()) {
            *state = STEP_END;
        }
        break;
    case STEP_END:
        return TRUE;
    }
    func_0201c4e4(wk->printQueue);
    func_ov182_021b99e8(wk->graphic);
    return FALSE;
}


// pokebw2's TheEnd_VBlank
void func_ov182_021b9784(TCB *tcb, void *data) {
}

// pokebw2's TheEnd_LoadGraphics
void func_ov182_021b9788(TheEndWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_THE_END, work->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 2, PALTYPE_MAIN_BG, 0, 0, work->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 3, BG_PICTURE, 0, 0, FALSE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 5, BG_PICTURE, 0, 0, FALSE, work->heapId);
    if (getGameVersion() == VERSION_WHITE) {
        GFL_BGSysSetScrPaletteNo(BG_PICTURE, 0, 0, 32, 24, 1);
    }
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(BG_PICTURE);
    GFL_BGSysSetBGEnabled(BG_PICTURE, TRUE);
    GFL_BGSysSetBGEnabled(BG_UNUSED, FALSE);
}

// pokebw2's TheEnd_Unused
void func_ov182_021b9810(TheEndWork *work) {
}

// Fades both screens, to black in Black and to white in White
// pokebw2's TheEnd_Fade
void func_ov182_021b9814(int start, int end, int slowness) {
    if (getGameVersion() == VERSION_BLACK) {
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, start, end, slowness);
    } else {
        GFL_FadeSet(FADE_ENGINE_A_WHITE | FADE_ENGINE_B_WHITE, start, end, slowness);
    }
}
