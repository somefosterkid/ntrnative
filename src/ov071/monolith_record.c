// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov143/monolith_record.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Entralink monolith's mission records on the bottom screen: the Funfest missions hosted, joined and completed,
// the most participants and the best score, with the return button. The ROM gives no name for this file;
// monolith_record.c is a guess from its screen's title

#include "types.h"
#include "app/monolith/monolith_tool.h"
#include "constants/sound.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/gx_layers.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "field/pass_power_check.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/printsys.h"
#include "system/wordset.h"

// The BGs of the records' text and of their background
#define MONOLITH_RECORD_BG_TEXT 4
#define MONOLITH_RECORD_BG 6

// The title and the five records
#define MONOLITH_RECORD_WINDOW_COUNT 6

// Where the values end, from the left of their windows
#define MONOLITH_RECORD_VALUE_RIGHT 224

enum {
    MONOLITH_RECORD_SEQ_INPUT,
    MONOLITH_RECORD_SEQ_RETURN,
};

typedef struct {
    BmpWin *windows[MONOLITH_RECORD_WINDOW_COUNT];
    PrintWindow prints[MONOLITH_RECORD_WINDOW_COUNT];
    MonolithReturnButton returnButton;
} MonolithRecordWork;

static BOOL func_ov071_021b889c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov071_021b8968(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov071_021b8a38(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov071_021b8a70(void);
static void func_ov071_021b8abc(void);
static void func_ov071_021b8adc(MonolithWork *wk);
static void func_ov071_021b8b08(MonolithWork *wk, MonolithRecordWork *rw);
static void func_ov071_021b8bf4(MonolithRecordWork *rw);
void func_ov071_021b8c0c(MonolithScreenParam *screen, MonolithWork *wk, MonolithRecordWork *rw);
static void func_ov071_021b8ec4(MonolithScreenParam *screen, MonolithRecordWork *rw);
void func_ov071_021b8ed0(MonolithRecordWork *rw);
void func_ov071_021b8edc(MonolithRecordWork *rw);

// pokebw2's sMonolithRecordTouchRects
static const TouchRect data_ov071_021baaa4[] = {
    { 172, 188, 232, 248 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's MONOLITH_RECORD_PROC_FUNCTIONS
const GameProcFunctions data_ov071_021baaac = {
    func_ov071_021b889c,
    func_ov071_021b8968,
    func_ov071_021b8a38,
};

// The setups of MONOLITH_RECORD_BG_TEXT and MONOLITH_RECORD_BG. One array: two setups would be laid out ahead of the
// proc table, where the ROM has them after it
static const BGSetup data_ov071_021baab8[2] = {
#include "asset/monolith_record_data_ov071_021baab8.inc"
};

// pokebw2's MonolithRecord_Init
static BOOL func_ov071_021b889c(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithRecordWork *rw;
    BOOL printing;
    int i;

    switch (*state) {
    case 0:
        rw = GFL_ProcInitSubsystem(proc, sizeof(MonolithRecordWork), HEAPID_MONOLITH);
        MI_CpuFill8(rw, 0, sizeof(MonolithRecordWork));
        func_ov071_021b8a70();
        func_ov071_021b8adc(screen->work);
        func_ov071_021b8b08(screen->work, rw);
        func_ov071_021b8ec4(screen, rw);
        func_ov071_021b8c0c(screen, screen->work, rw);
        (*state)++;
        break;
    case 1:
        rw = work;
        printing = FALSE;
        for (i = 0; i < MONOLITH_RECORD_WINDOW_COUNT; i++) {
            PrintWindow_Flush(&rw->prints[i], screen->work->printQueue);
            if (PrintWindow_IsPrinted(&rw->prints[i]) == FALSE) {
                printing = TRUE;
            }
        }
        if (printing == FALSE) {
            GFL_BGSysSetBGEnabled(MONOLITH_RECORD_BG_TEXT, TRUE);
            GFL_BGSysSetBGEnabled(MONOLITH_RECORD_BG, TRUE);
            GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL func_ov071_021b8968(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithRecordWork *rw = work;
    int i;
    u32 pressed;

    if (screen->exiting == TRUE) {
        return TRUE;
    }
    for (i = 0; i < MONOLITH_RECORD_WINDOW_COUNT; i++) {
        PrintWindow_Flush(&rw->prints[i], screen->work->printQueue);
    }
    func_ov071_021b8edc(rw);
    switch (*state) {
    case MONOLITH_RECORD_SEQ_INPUT:
        pressed = GCTX_HIDGetPressedKeys();
        if (func_02035c08(data_ov071_021baaa4) != TOUCH_RECT_NONE || (pressed & PAD_BUTTON_B)) {
            func_ov071_021b7108(&rw->returnButton);
            if (pressed & PAD_BUTTON_B) {
                func_020355d8(FALSE);
            } else {
                func_020355d8(TRUE);
            }
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            (*state)++;
        }
        break;
    case MONOLITH_RECORD_SEQ_RETURN:
        if (func_ov071_021b7114(&rw->returnButton) == FALSE) {
            screen->next = MONOLITH_SCREEN_MENU;
            return TRUE;
        }
        break;
    }
    return FALSE;
}


// pokebw2's MonolithRecord_Exit
static BOOL func_ov071_021b8a38(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithRecordWork *rw = work;

    if (func_0201c6b4(screen->work->printQueue) == FALSE) {
        return FALSE;
    }
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_BG3);
    func_ov071_021b8ed0(rw);
    func_ov071_021b8bf4(rw);
    func_ov071_021b8abc();
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's MonolithRecord_CreateBGs
static void func_ov071_021b8a70(void) {
    GFL_BGSysCreateBG(MONOLITH_RECORD_BG_TEXT, &data_ov071_021baab8[0], 0);
    GFL_BGSysCreateBG(MONOLITH_RECORD_BG, &data_ov071_021baab8[1], 0);
    GFL_BGSysFillScrArea(MONOLITH_RECORD_BG_TEXT, 0, 0, 0, 32, 32, 17);
    GFL_BGSysFillScrArea(MONOLITH_RECORD_BG, 0, 0, 0, 32, 32, 17);
}

// pokebw2's MonolithRecord_ReleaseBGs
static void func_ov071_021b8abc(void) {
    GFL_BGSysSetBGEnabled(MONOLITH_RECORD_BG_TEXT, FALSE);
    GFL_BGSysSetBGEnabled(MONOLITH_RECORD_BG, FALSE);
    GFL_BGSysReleaseBG(MONOLITH_RECORD_BG_TEXT);
    GFL_BGSysReleaseBG(MONOLITH_RECORD_BG);
}

// pokebw2's MonolithRecord_LoadScreen
static void func_ov071_021b8adc(MonolithWork *wk) {
    loadBGScrToVramByFileNoReserveNegAlign(wk->arc, 5, MONOLITH_RECORD_BG, 0, 0, TRUE, HEAPID_MONOLITH);
    GFL_BGSysLoadScr(MONOLITH_RECORD_BG);
    GFL_BGSysLoadScr(MONOLITH_RECORD_BG_TEXT);
}

// pokebw2's MonolithRecord_CreateWindows
static void func_ov071_021b8b08(MonolithWork *wk, MonolithRecordWork *rw) {
    int i;

    rw->windows[0] = BmpWin_CreateDynamic(MONOLITH_RECORD_BG_TEXT, 2, 0, 30, 3, 13, TRUE);
    BmpWin_FlushChar(rw->windows[0]);
    BmpWin_FlushMap(rw->windows[0]);
    rw->windows[1] = BmpWin_CreateDynamic(MONOLITH_RECORD_BG_TEXT, 2, 5, 30, 2, 13, TRUE);
    BmpWin_FlushChar(rw->windows[1]);
    BmpWin_FlushMap(rw->windows[1]);
    rw->windows[2] = BmpWin_CreateDynamic(MONOLITH_RECORD_BG_TEXT, 2, 8, 30, 2, 13, TRUE);
    BmpWin_FlushChar(rw->windows[2]);
    BmpWin_FlushMap(rw->windows[2]);
    rw->windows[3] = BmpWin_CreateDynamic(MONOLITH_RECORD_BG_TEXT, 2, 14, 30, 4, 13, TRUE);
    BmpWin_FlushChar(rw->windows[3]);
    BmpWin_FlushMap(rw->windows[3]);
    rw->windows[4] = BmpWin_CreateDynamic(MONOLITH_RECORD_BG_TEXT, 2, 16, 30, 2, 13, TRUE);
    BmpWin_FlushChar(rw->windows[4]);
    BmpWin_FlushMap(rw->windows[4]);
    rw->windows[5] = BmpWin_CreateDynamic(MONOLITH_RECORD_BG_TEXT, 2, 18, 30, 2, 13, TRUE);
    BmpWin_FlushChar(rw->windows[5]);
    BmpWin_FlushMap(rw->windows[5]);
    for (i = 0; i < MONOLITH_RECORD_WINDOW_COUNT; i++) {
        PrintWindow_Init(&rw->prints[i], rw->windows[i]);
    }
    GFL_BGSysLoadScr(MONOLITH_RECORD_BG_TEXT);
}

// pokebw2's MonolithRecord_FreeWindows
static void func_ov071_021b8bf4(MonolithRecordWork *rw) {
    int i;

    for (i = 0; i < MONOLITH_RECORD_WINDOW_COUNT; i++) {
        BmpWin_Free(rw->windows[i]);
    }
}

void func_ov071_021b8c0c(MonolithScreenParam *screen, MonolithWork *wk, MonolithRecordWork *rw) {
    u16 *levels;
    StrBuf *str;
    StrBuf *expanded;
    s64 playTime;
    u32 hours;
    u32 minutes;

    func_ov071_021b6f2c(screen);
    levels = func_ov071_021b6f40(screen);
    str = GFL_StrBufCreate(256, HEAPID_MONOLITH);
    expanded = GFL_StrBufCreate(256, HEAPID_MONOLITH);

    GFL_MsgDataLoadStrbuf(wk->msgMonolith, 13, str);
    PrintWindow_PrintNoColor(&rw->prints[0], wk->printQueue, 0, 4, str, wk->font);

    GFL_MsgDataLoadStrbuf(wk->msgMonolith, 5, str);
    WordSetNumber(wk->wordSet, 0, levels[7] + levels[6], 5, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, expanded, str);
    PrintWindow_PrintNoColor(&rw->prints[1], wk->printQueue, 0, 0, expanded, wk->font);

    GFL_MsgDataLoadStrbuf(wk->msgMonolith, 9, str);
    WordSetNumber(wk->wordSet, 0, levels[7], 4, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, expanded, str);
    PrintWindow_PrintNoColor(&rw->prints[2], wk->printQueue, 0, 0, expanded, wk->font);

    GFL_MsgDataLoadStrbuf(wk->msgMonolith, 8, str);
    WordSetNumber(wk->wordSet, 0, levels[6], 4, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, expanded, str);
    PrintWindow_PrintNoColor(&rw->prints[2], wk->printQueue, 120, 0, expanded, wk->font);

    // The seconds played: unk_04 and unk_08 are the u64's low and high words, read as one
    playTime = *(s64 *)&screen->param->highLink.unk_04;
    hours = playTime / 60 / 60;
    if (hours > 999) {
        hours = 999;
        minutes = 59;
    } else {
        minutes = (playTime - hours * 3600) / 60;
    }
    GFL_MsgDataLoadStrbuf(wk->msgMonolith, 11, str);
    WordSetNumber(wk->wordSet, 0, hours, 3, 1, TRUE);
    WordSetNumber(wk->wordSet, 1, minutes, 2, 2, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, expanded, str);
    PrintWindow_PrintNoColor(&rw->prints[3], wk->printQueue, 0, 0, expanded, wk->font);

    GFL_MsgDataLoadStrbuf(wk->msgMonolith, 12, str);
    WordSetNumber(wk->wordSet, 0, screen->param->highLink.unk_00, 5, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, expanded, str);
    PrintWindow_PrintNoColor(&rw->prints[4], wk->printQueue, 0, 0, expanded, wk->font);

    GFL_MsgDataLoadStrbuf(wk->msgMonolith, 10, str);
    WordSetNumber(wk->wordSet, 0, func_ov010_0216feac((const PassPowerData *)wk->unk_34, func_ov071_021b6f40(screen), screen->param->highLink.unk_0C), 4,
                  0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, expanded, str);
    PrintWindow_PrintNoColor(&rw->prints[5], wk->printQueue, 0, 0, expanded, wk->font);

    GFL_StrBufFree(str);
    GFL_StrBufFree(expanded);
}


// pokebw2's MonolithRecord_CreateReturnButton
static void func_ov071_021b8ec4(MonolithScreenParam *screen, MonolithRecordWork *rw) {
    func_ov071_021b7070(screen->work, &rw->returnButton);
}

void func_ov071_021b8ed0(MonolithRecordWork *rw) {
    func_ov071_021b70b8(&rw->returnButton);
}


void func_ov071_021b8edc(MonolithRecordWork *rw) {
    func_ov071_021b70c4(&rw->returnButton);
}
