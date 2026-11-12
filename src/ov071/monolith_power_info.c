// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov143/monolith_power_info.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Entralink monolith's top screen over the pass power list: the three pass powers that are equipped, and the
// description of the one that the list's cursor is on, or what the player needs to unlock it. The ROM gives no name
// for this file; monolith_power_info.c is a guess from what it shows

#include "types.h"
#include "app/monolith/monolith_tool.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/gx_layers.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/gx.h"
#include "save/high_link.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "gfl/bmp.h"
#include "system/game_system.h"
#include "system/game_data.h"
#include "save/save_control.h"
#include "field/beacon_view.h"

// The BGs of the text and of the background
#define MONOLITH_POWER_INFO_BG_TEXT 0
#define MONOLITH_POWER_INFO_BG 2

// The title, the equipped pass powers and the description
#define MONOLITH_POWER_INFO_WINDOW_COUNT 3

enum {
    MONOLITH_POWER_INFO_SEQ_MAIN,
};

typedef struct {
    BmpWin *windows[MONOLITH_POWER_INFO_WINDOW_COUNT];
    PrintWindow prints[MONOLITH_POWER_INFO_WINDOW_COUNT];
    // The pass power whose description is shown, or HIGH_LINK_POWER_NONE
    u32 shownPower;
} MonolithPowerInfoWork;

static BOOL func_ov071_021ba1fc(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov071_021ba2cc(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov071_021ba384(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov071_021ba3b4(void);
static void func_ov071_021ba400(void);
static void func_ov071_021ba420(MonolithWork *wk);
static void func_ov071_021ba44c(MonolithWork *wk, MonolithPowerInfoWork *piw);
static void func_ov071_021ba4d4(MonolithPowerInfoWork *piw);
void func_ov071_021ba4ec(MonolithWork *wk, MonolithPowerInfoWork *piw);
void func_ov071_021ba528(MonolithScreenParam *screen, MonolithWork *wk, MonolithPowerInfoWork *piw);
void func_ov071_021ba5d4(MonolithScreenParam *screen, MonolithWork *wk, MonolithPowerInfoWork *piw);

// pokebw2's MONOLITH_POWER_INFO_PROC_FUNCTIONS
const GameProcFunctions data_ov071_021bab88 = {
    func_ov071_021ba1fc,
    func_ov071_021ba2cc,
    func_ov071_021ba384,
};

// pokebw2's sMonolithPowerInfoBGSetupText
static const BGSetup data_ov071_021bab94 = {
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

// pokebw2's sMonolithPowerInfoBGSetup
static const BGSetup data_ov071_021babb4 = {
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

// pokebw2's MonolithPowerInfo_Init
static BOOL func_ov071_021ba1fc(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithPowerInfoWork *piw;
    BOOL printing;
    int i;

    switch (*state) {
    case 0:
        piw = GFL_ProcInitSubsystem(proc, sizeof(MonolithPowerInfoWork), HEAPID_MONOLITH);
        MI_CpuFill8(piw, 0, sizeof(MonolithPowerInfoWork));
        piw->shownPower = HIGH_LINK_POWER_NONE;
        func_ov071_021ba3b4();
        func_ov071_021ba420(screen->work);
        func_ov071_021ba44c(screen->work, piw);
        func_ov071_021ba4ec(screen->work, piw);
        func_ov071_021ba528(screen, screen->work, piw);
        (*state)++;
        break;
    case 1:
        piw = work;
        printing = FALSE;
        for (i = 0; i < MONOLITH_POWER_INFO_WINDOW_COUNT; i++) {
            PrintWindow_Flush(&piw->prints[i], screen->work->printQueue);
            if (PrintWindow_IsPrinted(&piw->prints[i]) == FALSE) {
                printing = TRUE;
            }
        }
        if (printing == FALSE) {
            GFL_BGSysSetBGEnabled(MONOLITH_POWER_INFO_BG_TEXT, TRUE);
            GFL_BGSysSetBGEnabled(MONOLITH_POWER_INFO_BG, TRUE);
            GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static inline BOOL PrintWindow_FlushAndIsPrinted(PrintWindow *printWindow, PrintQueue *queue) {
    PrintWindow_Flush(printWindow, queue);
    return PrintWindow_IsPrinted(printWindow);
}

BOOL func_ov071_021ba2cc(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithPowerInfoWork *piw = work;
    u8 printed[MONOLITH_POWER_INFO_WINDOW_COUNT];
    long i;

    if (screen->endTop == TRUE) {
        return TRUE;
    }
    for (i = 0; i < MONOLITH_POWER_INFO_WINDOW_COUNT; i++) {
        PrintWindow_Flush(&piw->prints[i], screen->work->printQueue);
        printed[i] = PrintWindow_IsPrinted(&piw->prints[i]);
    }
    switch (*state) {
    case MONOLITH_POWER_INFO_SEQ_MAIN:
        if (screen->state->unk_03 == TRUE && printed[1] == TRUE) {
            func_ov071_021ba528(screen, screen->work, piw);
            screen->state->unk_03 = FALSE;
        }
        if (screen->state->unk_01 != piw->shownPower && printed[2] == TRUE) {
            func_ov071_021ba5d4(screen, screen->work, piw);
            piw->shownPower = screen->state->unk_01;
        }
        return FALSE;
    }
    return FALSE;
}


// pokebw2's MonolithPowerInfo_Exit
static BOOL func_ov071_021ba384(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithPowerInfoWork *piw = work;

    if (func_0201c6b4(screen->work->printQueue) == FALSE) {
        return FALSE;
    }
    GFL_BGSysSetEnabledBGsA(GX_PLANEMASK_BG3);
    func_ov071_021ba4d4(piw);
    func_ov071_021ba400();
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's MonolithPowerInfo_CreateBGs
static void func_ov071_021ba3b4(void) {
    GFL_BGSysCreateBG(MONOLITH_POWER_INFO_BG_TEXT, &data_ov071_021bab94, 0);
    GFL_BGSysCreateBG(MONOLITH_POWER_INFO_BG, &data_ov071_021babb4, 0);
    GFL_BGSysFillScrArea(MONOLITH_POWER_INFO_BG_TEXT, 0, 0, 0, 32, 32, 17);
    GFL_BGSysFillScrArea(MONOLITH_POWER_INFO_BG, 0, 0, 0, 32, 32, 17);
}

// pokebw2's MonolithPowerInfo_ReleaseBGs
static void func_ov071_021ba400(void) {
    GFL_BGSysSetBGEnabled(MONOLITH_POWER_INFO_BG_TEXT, FALSE);
    GFL_BGSysSetBGEnabled(MONOLITH_POWER_INFO_BG, FALSE);
    GFL_BGSysReleaseBG(MONOLITH_POWER_INFO_BG_TEXT);
    GFL_BGSysReleaseBG(MONOLITH_POWER_INFO_BG);
}

// pokebw2's MonolithPowerInfo_LoadGraphics
static void func_ov071_021ba420(MonolithWork *wk) {
    loadBGScrToVramByFileNoReserveNegAlign(wk->arc, 13, MONOLITH_POWER_INFO_BG, 0, 0, TRUE, HEAPID_MONOLITH);
    GFL_BGSysLoadScr(MONOLITH_POWER_INFO_BG_TEXT);
    GFL_BGSysLoadScr(MONOLITH_POWER_INFO_BG);
}

// pokebw2's MonolithPowerInfo_CreateWindows
static void func_ov071_021ba44c(MonolithWork *wk, MonolithPowerInfoWork *piw) {
    int i;

    piw->windows[0] = BmpWin_CreateDynamic(MONOLITH_POWER_INFO_BG_TEXT, 2, 0, 28, 3, 13, TRUE);
    BmpWin_FlushChar(piw->windows[0]);
    BmpWin_FlushMap(piw->windows[0]);
    piw->windows[1] = BmpWin_CreateDynamic(MONOLITH_POWER_INFO_BG_TEXT, 2, 4, 28, 8, 13, TRUE);
    BmpWin_FlushChar(piw->windows[1]);
    BmpWin_FlushMap(piw->windows[1]);
    piw->windows[2] = BmpWin_CreateDynamic(MONOLITH_POWER_INFO_BG_TEXT, 2, 15, 28, 9, 13, TRUE);
    BmpWin_FlushChar(piw->windows[2]);
    BmpWin_FlushMap(piw->windows[2]);
    for (i = 0; i < MONOLITH_POWER_INFO_WINDOW_COUNT; i++) {
        PrintWindow_Init(&piw->prints[i], piw->windows[i]);
    }
    GFL_BGSysLoadScr(MONOLITH_POWER_INFO_BG_TEXT);
}

// pokebw2's MonolithPowerInfo_FreeWindows
static void func_ov071_021ba4d4(MonolithPowerInfoWork *piw) {
    int i;

    for (i = 0; i < MONOLITH_POWER_INFO_WINDOW_COUNT; i++) {
        BmpWin_Free(piw->windows[i]);
    }
}

void func_ov071_021ba4ec(MonolithWork *wk, MonolithPowerInfoWork *piw) {
    StrBuf *strbuf;

    strbuf = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, 0x26);
    PrintWindow_PrintNoColor(&piw->prints[0], wk->printQueue, 0, 4, strbuf, wk->font);
    GFL_StrBufFree(strbuf);
}

void func_ov071_021ba528(MonolithScreenParam *screen, MonolithWork *wk, MonolithPowerInfoWork *piw) {
    GameData *gameData;
    SaveControl *save;
    StrBuf *strbuf;
    StrBuf *description;
    int index;

    gameData = GSYS_GetGameData(screen->param->gsys);
    save = GameData_GetSaveControl(gameData);
    index = func_0200bf10(getHighLinkBlockAddress(save));
    GFL_BitmapFill(BmpWin_GetBitmap(piw->windows[1]), 0);
    if (index == HIGH_LINK_POWER_NONE) {
        strbuf = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, 0x27);
    } else {
        strbuf = GFL_MsgDataLoadStrbufNew(wk->msgPowerNames, wk->unk_34[index].unk_04);
        description = GFL_MsgDataLoadStrbufNew(wk->msgPowerInfo, wk->unk_34[index].unk_06);
        PrintWindow_PrintNoColor(&piw->prints[1], wk->printQueue, 0, 0x10, description, wk->font);
        GFL_StrBufFree(description);
    }
    PrintWindow_PrintNoColor(&piw->prints[1], wk->printQueue, 0, 0, strbuf, wk->font);
    GFL_StrBufFree(strbuf);
}

void func_ov071_021ba5d4(MonolithScreenParam *screen, MonolithWork *wk, MonolithPowerInfoWork *piw) {
    PassPowerLevel *levels;
    StrBuf *strbuf;
    StrBuf *description;
    StrBuf *dest;
    u32 index;
    int messageId;
    int number;

    index = screen->state->unk_01;
    GFL_BitmapFill(BmpWin_GetBitmap(piw->windows[2]), 0);
    if (index != HIGH_LINK_POWER_NONE) {
        if (screen->state->unk_02 == FALSE) {
            WordSetNumber(wk->wordSet, 0, wk->unk_34[index].unk_02, 4, 0, TRUE);
            strbuf = GFL_MsgDataLoadStrbufNew(wk->msgPowerInfo, wk->unk_34[index].unk_06);
            description = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, 0x28);
            dest = GFL_StrBufCreate(0x40, 0x61);
            GFL_WordSetFormatStrbuf(wk->wordSet, dest, description);
            PrintWindow_PrintNoColor(&piw->prints[2], wk->printQueue, 0, 4, strbuf, wk->font);
            PrintWindow_PrintNoColor(&piw->prints[2], wk->printQueue, 0x70, 0x34, dest, wk->font);
            GFL_StrBufFree(strbuf);
            GFL_StrBufFree(description);
            GFL_StrBufFree(dest);
            return;
        }
        levels = func_ov071_021b6f40(screen);
        if (wk->unk_34[index].unk_00 > levels->level1) {
            number = wk->unk_34[index].unk_00;
            messageId = 0x29;
        } else {
            number = wk->unk_34[index].unk_01;
            messageId = 0x2a;
        }
        WordSetNumber(wk->wordSet, 0, number, 3, 0, TRUE);
        WordSetNumber(wk->wordSet, 1, levels->level1, 3, 0, TRUE);
        WordSetNumber(wk->wordSet, 2, levels->level2, 3, 0, TRUE);
        strbuf = GFL_MsgDataLoadStrbufNew(wk->msgMonolith, messageId);
        dest = GFL_StrBufCreate(0x80, 0x61);
        GFL_WordSetFormatStrbuf(wk->wordSet, dest, strbuf);
        PrintWindow_PrintNoColor(&piw->prints[2], wk->printQueue, 0, 4, dest, wk->font);
        GFL_StrBufFree(dest);
        GFL_StrBufFree(strbuf);
    } else {
        BmpWin_FlushChar(piw->windows[2]);
    }
}

