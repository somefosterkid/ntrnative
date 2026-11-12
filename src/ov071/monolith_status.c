// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov143/monolith_status.c, Black 2/White 2's
// version of this file in overlay 143, adapted to Black's. The names are Black's dsd names and swan's; pokebw2's
// guesses are in the comments.
#include "types.h"
#include "app/monolith/monolith_main.h"
#include "app/monolith/monolith_tool.h"
#include "field/intrude_comm.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/gx.h"
#include "system/game_system.h"
#include "system/printsys.h"
#include "system/wordset.h"

// The Entralink monolith's top screen: the mission that the bottom screen's cursor is on, its title and its text, which
// names the player who gave it. Black 2's shows the Entree's levels instead. The ROM gives no name for this file;
// monolith_status.c is pokebw2's guess for Black 2's

// The BGs of the text and of the background
#define MONOLITH_STATUS_BG_TEXT 0
#define MONOLITH_STATUS_BG 2

// The title and the text
#define MONOLITH_STATUS_WINDOW_COUNT 2

// The value of MonolithState's mission for the mission in progress
#define MONOLITH_STATUS_MISSION_IN_PROGRESS 6

enum {
    MONOLITH_STATUS_SEQ_MAIN,
};

typedef struct {
    BmpWin *windows[MONOLITH_STATUS_WINDOW_COUNT];
    PrintWindow prints[MONOLITH_STATUS_WINDOW_COUNT];
    MonolithOamText text;
    // The mission shown, or 0xff before the first is
    u8 mission;
} MonolithStatusWork;

static BOOL func_ov071_021b8494(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov071_021b855c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov071_021b8618(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov071_021b8650(void);
static void func_ov071_021b869c(void);
static void func_ov071_021b86bc(MonolithWork *wk);
static void func_ov071_021b86e8(MonolithWork *wk, MonolithStatusWork *sw);
static void func_ov071_021b8750(MonolithStatusWork *sw);
static void func_ov071_021b8764(MonolithWork *wk, MonolithStatusWork *sw);
static void func_ov071_021b878c(MonolithStatusWork *sw);
static void func_ov071_021b8798(MonolithScreenParam *screen, MonolithWork *wk, MonolithStatusWork *sw,
                                IntrudeComm *intrude, u8 mission);

// pokebw2's MONOLITH_STATUS_PROC_FUNCTIONS
const GameProcFunctions data_ov071_021baa40 = { func_ov071_021b8494, func_ov071_021b855c, func_ov071_021b8618 };

const u32 data_ov071_021baa4c[MONOLITH_MISSION_COUNT] = { 2, 1, 0, 5, 4, 3 };

// pokebw2's sMonolithStatusBGSetupText
static const BGSetup data_ov071_021baa64 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x2000,
    GX_BG_CHARBASE_0x0c000,
    0x8000,
    GX_BG_EXTPLTT_01,
    2,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// pokebw2's sMonolithStatusBGSetup
static const BGSetup data_ov071_021baa84 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x1000,
    GX_BG_CHARBASE_0x04000,
    0x8000,
    GX_BG_EXTPLTT_01,
    3,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// pokebw2's MonolithStatus_Init
static BOOL func_ov071_021b8494(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithStatusWork *sw;
    BOOL printing;
    int i;

    switch (*state) {
    case 0:
        sw = GFL_ProcInitSubsystem(proc, sizeof(MonolithStatusWork), HEAPID_MONOLITH);
        MI_CpuFill8(sw, 0, sizeof(MonolithStatusWork));
        func_ov071_021b8650();
        func_ov071_021b86bc(screen->work);
        func_ov071_021b86e8(screen->work, sw);
        func_ov071_021b8764(screen->work, sw);
        sw->mission = 0xff;
        (*state)++;
        break;
    case 1:
        sw = work;
        printing = FALSE;
        for (i = 0; i < MONOLITH_STATUS_WINDOW_COUNT; i++) {
            PrintWindow_Flush(&sw->prints[i], screen->work->printQueue);
            if (PrintWindow_IsPrinted(&sw->prints[i]) == FALSE) {
                printing = TRUE;
            }
        }
        if (printing == FALSE) {
            GFL_BGSysSetBGEnabled(MONOLITH_STATUS_BG_TEXT, TRUE);
            GFL_BGSysSetBGEnabled(MONOLITH_STATUS_BG, TRUE);
            GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's MonolithStatus_Main
static BOOL func_ov071_021b855c(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithStatusWork *sw = work;
    IntrudeComm *intrude = func_ov010_02158344(GSYS_GetGameCommSystem(screen->param->gsys));
    int printed = 0;
    int i;

    if (screen->endTop == TRUE) {
        return TRUE;
    }
    func_ov071_021b6f04(screen->work, &sw->text);
    for (i = 0; i < MONOLITH_STATUS_WINDOW_COUNT; i++) {
        PrintWindow_Flush(&sw->prints[i], screen->work->printQueue);
        if (PrintWindow_IsPrinted(&sw->prints[i]) == TRUE) {
            printed++;
        }
    }
    if (printed == MONOLITH_STATUS_WINDOW_COUNT && sw->mission != screen->state->mission) {
        func_ov071_021b8798(screen, screen->work, sw, intrude, screen->state->mission);
        sw->mission = screen->state->mission;
    }
    switch (*state) {
    case MONOLITH_STATUS_SEQ_MAIN:
        return FALSE;
    }
    return FALSE;
}

// pokebw2's MonolithStatus_Exit
static BOOL func_ov071_021b8618(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithStatusWork *sw = work;

    if (func_0201c6b4(screen->work->printQueue) == FALSE) {
        return FALSE;
    }
    GFL_BGSysSetEnabledBGsA(GX_PLANEMASK_BG3);
    func_ov071_021b8750(sw);
    func_ov071_021b869c();
    func_ov071_021b878c(sw);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's MonolithStatus_CreateBGs
static void func_ov071_021b8650(void) {
    GFL_BGSysCreateBG(MONOLITH_STATUS_BG_TEXT, &data_ov071_021baa64, BGMODE_TEXT);
    GFL_BGSysCreateBG(MONOLITH_STATUS_BG, &data_ov071_021baa84, BGMODE_TEXT);
    GFL_BGSysFillScrArea(MONOLITH_STATUS_BG_TEXT, 0, 0, 0, 32, 32, 17);
    GFL_BGSysFillScrArea(MONOLITH_STATUS_BG, 0, 0, 0, 32, 32, 17);
}

// pokebw2's MonolithStatus_ReleaseBGs
static void func_ov071_021b869c(void) {
    GFL_BGSysSetBGEnabled(MONOLITH_STATUS_BG_TEXT, FALSE);
    GFL_BGSysSetBGEnabled(MONOLITH_STATUS_BG, FALSE);
    GFL_BGSysReleaseBG(MONOLITH_STATUS_BG_TEXT);
    GFL_BGSysReleaseBG(MONOLITH_STATUS_BG);
}

// pokebw2's MonolithStatus_LoadScreen
static void func_ov071_021b86bc(MonolithWork *wk) {
    loadBGScrToVramByFileNoReserveNegAlign(wk->arc, 12, MONOLITH_STATUS_BG, 0, 0, TRUE, HEAPID_MONOLITH);
    GFL_BGSysLoadScr(MONOLITH_STATUS_BG_TEXT);
    GFL_BGSysLoadScr(MONOLITH_STATUS_BG);
}

// pokebw2's MonolithStatus_CreateWindows
static void func_ov071_021b86e8(MonolithWork *wk, MonolithStatusWork *sw) {
    int i;

    sw->windows[0] = BmpWin_CreateDynamic(MONOLITH_STATUS_BG_TEXT, 2, 0, 28, 3, 13, TRUE);
    BmpWin_FlushChar(sw->windows[0]);
    BmpWin_FlushMap(sw->windows[0]);
    sw->windows[1] = BmpWin_CreateDynamic(MONOLITH_STATUS_BG_TEXT, 2, 5, 28, 16, 13, TRUE);
    BmpWin_FlushChar(sw->windows[1]);
    BmpWin_FlushMap(sw->windows[1]);
    for (i = 0; i < MONOLITH_STATUS_WINDOW_COUNT; i++) {
        PrintWindow_Init(&sw->prints[i], sw->windows[i]);
    }
    GFL_BGSysLoadScr(MONOLITH_STATUS_BG_TEXT);
}

// pokebw2's MonolithStatus_FreeWindows
static void func_ov071_021b8750(MonolithStatusWork *sw) {
    BmpWin_Free(sw->windows[0]);
    BmpWin_Free(sw->windows[1]);
}

// Shows message 19 at the bottom of the screen
static void func_ov071_021b8764(MonolithWork *wk, MonolithStatusWork *sw) {
    func_ov071_021b6e1c(wk, &sw->text, 1, 128, 180, 28, 2, 19, NULL);
}

static void func_ov071_021b878c(MonolithStatusWork *sw) {
    func_ov071_021b6eec(&sw->text);
}

// pokebw2's MonolithStatus_Draw
static void func_ov071_021b8798(MonolithScreenParam *screen, MonolithWork *wk, MonolithStatusWork *sw,
                                IntrudeComm *intrude, u8 mission) {
    StrBuf *expanded = GFL_StrBufCreate(256, HEAPID_MONOLITH);
    IntrudePlayer *player;
    IntrudeMission *data;
    u32 msgId;
    StrBuf *title;
    StrBuf *text;

    GFL_BitmapFill(BmpWin_GetBitmap(sw->windows[0]), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(sw->windows[1]), 0);
    if (mission == MONOLITH_STATUS_MISSION_IN_PROGRESS && intrude != NULL &&
        func_ov011_02175b14(&intrude->unk_1AC) == TRUE) {
        IntrudeMissionEntry *entry = func_ov011_02175b34(&intrude->unk_1AC);

        data = &entry->mission;
        player = &entry->player;
    } else {
        // BUG: The mission in progress, once the communication is gone, reads past the end of data_ov071_021baa4c
#ifdef BUGFIX
        if (mission >= MONOLITH_STATUS_MISSION_IN_PROGRESS) {
#else
        if (mission > MONOLITH_STATUS_MISSION_IN_PROGRESS) {
#endif
            mission = 0;
        }
        data = &wk->missions[data_ov071_021baa4c[mission]];
        player = &screen->param->commPlayer.player;
    }
    msgId = data->msgId;
    title = GFL_MsgDataLoadStrbufNew(wk->msgMission, data->kind);
    text = GFL_MsgDataLoadStrbufNew(wk->msgMission, msgId);
    func_ov011_021762d8(data, player, wk->wordSet, HEAPID_MONOLITH);
    GFL_WordSetFormatStrbuf(wk->wordSet, expanded, text);
    PrintWindow_PrintNoColor(&sw->prints[0], wk->printQueue, 0, 4, title, wk->font);
    PrintWindow_PrintNoColor(&sw->prints[1], wk->printQueue, 0, 0, expanded, wk->font);
    GFL_StrBufFree(title);
    GFL_StrBufFree(text);
    GFL_StrBufFree(expanded);
}
