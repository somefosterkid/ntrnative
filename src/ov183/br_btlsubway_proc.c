// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_btlsubway_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's Battle Subway records screen: a menu of the Subway's lines on the sub screen, and the streaks
// of the chosen line with the player's Battle Points on the main screen. The name is a guess: the ROM gives none, and
// br_core.c runs it as BR_PROCID_BTLSUBWAY

#include "types.h"
#include "app/battle_recorder/br_btn.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "constants/flags.h"
#include "constants/sound.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "save/bsubway_save.h"
#include "save/event_work.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "gfl/proc.h"

// The lines of the menu, which are what the main screen shows
enum {
    BR_BTLSUBWAY_PAGE_TOP,
    BR_BTLSUBWAY_PAGE_SINGLE,
    BR_BTLSUBWAY_PAGE_DOUBLE,
    BR_BTLSUBWAY_PAGE_MULTI,
    BR_BTLSUBWAY_PAGE_WIFI,
    BR_BTLSUBWAY_PAGE_MAX,
};

#define BR_BTLSUBWAY_MENU_MAX 4
#define BR_BTLSUBWAY_MSGWIN_MAX 16
#define BR_BTLSUBWAY_BP_DIGITS 4
#define BR_BTLSUBWAY_STREAK_MAX 9999
#define BR_BTLSUBWAY_PRINT_COLOR 0x3da0

// The graphics of the Battle Points, from br_res.c
#define BR_RES_OBJ_BTLSUBWAY_BP 13

// The player's Battle Points: the icon and the digits
typedef struct {
    ClActor *icon;
    ClActor *digit[BR_BTLSUBWAY_BP_DIGITS];
} BrBtlSubwayBp;

typedef struct {
    BmpOamSys *bmpoam;
    PrintQueue *que;
    BrBtlSubwayBp bp;
    BrMsgWin *text;
    BrBtn *btn;
    BrMsgWin *menuWin[BR_BTLSUBWAY_MENU_MAX];
    BrMsgWin *msgWin[BR_BTLSUBWAY_MSGWIN_MAX];
    BrBallEffect *ballEff;
    u32 select;
    HeapID heapId;
} BrBtlSubwayWork;

// Where a window is, in tiles, and its message
typedef struct {
    u8 x;
    u8 y;
    u8 w;
    u8 h;
    u32 msgID;
} BrBtlSubwayMsgWinData;

typedef struct {
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
} BrBtlSubwayRect;

static BOOL func_ov183_021dd37c(GameProc *proc, u32 *state, void *param, void *work);
BrBallEffect *func_ov186_021fd760(ClActUnit *unit, BrRes *res, u32 display, HeapID heapId);
void func_ov186_021fbe54(BrRes *res, u32 objId, HeapID heapId);
static BOOL func_ov183_021dd3f8(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021dd440(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov183_021dd5c8(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p);
static void func_ov183_021dd684(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p);
void func_ov183_021dd858(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p);
static void func_ov183_021dda2c(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p);
void func_ov183_021ddccc(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p);
static void func_ov183_021ddde8(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p);

void func_ov186_021fb964(BrRes *res, u32 bgID, HeapID heapId);
void func_ov183_021dde2c(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p);
static void func_ov183_021ddf5c(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p);
static u32 func_ov183_021ddfa0(BrBtlSubwayWork *wk, u32 x, u32 y);
BOOL func_ov183_021de000(BSubwayScoreData *score, GameData *gameData);
BOOL func_ov183_021de02c(BSubwayScoreData *score, GameData *gameData);
BOOL func_ov183_021de058(BSubwayScoreData *score, GameData *gameData);
void func_ov183_021de084(BrBtlSubwayBp *bp, ClActUnit *unit, const BrResObjData *obj, u32 value, s16 x, s16 y,
                               HeapID heapId);
static void func_ov183_021de12c(BrBtlSubwayBp *bp);
void func_ov183_021de15c(BrBtlSubwayBp *bp, u32 value);

// pokebw2's sBrBtlSubwayTopWins
static const BrBtlSubwayMsgWinData data_ov183_021e3350[] = {
    { 4, 6, 24, 2, 0x82 },
};

// pokebw2's sBrBtlSubwayMenuWins
static const BrBtlSubwayMsgWinData data_ov183_021e336c[BR_BTLSUBWAY_MENU_MAX] = {
#include "asset/br_btlsubway_proc_data_ov183_021e336c.inc"
};

// pokebw2's sBrBtlSubwayWifiWins
static const BrBtlSubwayMsgWinData data_ov183_021e338c[] = {
#include "asset/br_btlsubway_proc_data_ov183_021e338c.inc"
};

// pokebw2's BR_BTLSUBWAY_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e33b4 = {
    func_ov183_021dd37c,
    func_ov183_021dd440,
    func_ov183_021dd3f8,
};

// pokebw2's sBrBtlSubwaySingleWins
static const BrBtlSubwayMsgWinData data_ov183_021e33c0[] = {
#include "asset/br_btlsubway_proc_data_ov183_021e33c0.inc"
};

// pokebw2's sBrBtlSubwayDoubleWins
static const BrBtlSubwayMsgWinData data_ov183_021e3400[] = {
#include "asset/br_btlsubway_proc_data_ov183_021e3400.inc"
};

// The lines of the menu on the sub screen. The first, of no line, is never hit
// pokebw2's sBrBtlSubwayMenuRects
static const BrBtlSubwayRect data_ov183_021e3440[BR_BTLSUBWAY_PAGE_MAX] = {
#include "asset/br_btlsubway_proc_data_ov183_021e3440.inc"
};

// pokebw2's sBrBtlSubwayMultiWins
static const BrBtlSubwayMsgWinData data_ov183_021e3490[BR_BTLSUBWAY_MSGWIN_MAX] = {
#include "asset/br_btlsubway_proc_data_ov183_021e3490.inc"
};

// pokebw2's BrBtlSubway_ProcExit

static BOOL func_ov183_021dd37c(GameProc *proc, u32 *state, void *param, void *work) {
    BrBtlSubwayProcParam *p = param;
    BrBtlSubwayWork *wk;

    wk = GFL_ProcInitSubsystem(proc, sizeof(BrBtlSubwayWork), func_ov186_021f9b9c(p->procSys));
    MI_CpuFill8(wk, 0, sizeof(BrBtlSubwayWork));
    wk->heapId = func_ov186_021f9b9c(p->procSys);

    wk->bmpoam = func_02025538(wk->heapId, p->unit);
    wk->que = func_0201c440(wk->heapId);
    wk->ballEff = func_ov186_021fd760(p->unit, p->res, CLACT_SURFACE_SUB, wk->heapId);
    func_ov186_021fbe54(p->res, BR_RES_OBJ_BTLSUBWAY_BP, wk->heapId);

    func_ov183_021dde2c(wk, p);
    func_ov183_021dd5c8(wk, p);
    return TRUE;
}

static BOOL func_ov183_021dd3f8(GameProc *proc, u32 *state, void *param, void *work) {
    BrBtlSubwayProcParam *p = param;
    BrBtlSubwayWork *wk = work;

    func_ov183_021ddf5c(wk, p);
    func_ov183_021ddde8(wk, p);
    GFL_BGSysLoadScr(4);
    GFL_BGSysLoadScr(1);
    func_ov186_021fc0c8(p->res, BR_RES_OBJ_BTLSUBWAY_BP);
    func_ov186_021fd820(wk->ballEff);
    func_0201c4c0(wk->que);
    func_02025588(wk->bmpoam);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrBtlSubway_ProcMain
static BOOL func_ov183_021dd440(GameProc *proc, u32 *state, void *param, void *work) {
    enum {
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_MAIN,
        SEQ_CHANGE_FADEOUT_START,
        SEQ_CHANGE_FADEOUT_WAIT,
        SEQ_CHANGE,
        SEQ_CHANGE_FADEIN_WAIT,
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_END,
    };
    BrBtlSubwayProcParam *p = param;
    BrBtlSubwayWork *wk = work;
    int i;

    switch (*state) {
    case SEQ_FADEIN_START:
        func_ov186_021fe784(p->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *state = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p->fade)) {
            *state = SEQ_MAIN;
        }
        break;
    case SEQ_MAIN: {
        u32 x;
        u32 y;

        if (func_02035d20(&x, &y)) {
            wk->select = func_ov183_021ddfa0(wk, x, y);
            if (wk->select != BR_BTLSUBWAY_PAGE_TOP) {
                *state = SEQ_CHANGE_FADEOUT_START;
            }
            if (func_ov186_021fb4b4(wk->btn, x, y)) {
                BrPoint pos;

                pos.x = x;
                pos.y = y;
                func_ov186_021fd860(wk->ballEff, BR_BALL_EFFECT_SPREAD, &pos);
                *state = SEQ_FADEOUT_START;
            }
        }
        break;
    }
    case SEQ_CHANGE_FADEOUT_START:
        func_ov186_021fe784(p->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_MAIN, BR_FADE_DIR_OUT);
        (*state)++;
        break;
    case SEQ_CHANGE_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p->fade)) {
            (*state)++;
        }
        break;
    case SEQ_CHANGE:
        func_ov183_021ddde8(wk, p);
        switch (wk->select) {
        case BR_BTLSUBWAY_PAGE_SINGLE:
            func_ov183_021dd684(wk, p);
            break;
        case BR_BTLSUBWAY_PAGE_DOUBLE:
            func_ov183_021dd858(wk, p);
            break;
        case BR_BTLSUBWAY_PAGE_MULTI:
            func_ov183_021dda2c(wk, p);
            break;
        case BR_BTLSUBWAY_PAGE_WIFI:
            func_ov183_021ddccc(wk, p);
            break;
        }
        func_ov186_021fe784(p->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_MAIN, BR_FADE_DIR_IN);
        (*state)++;
        break;
    case SEQ_CHANGE_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p->fade)) {
            *state = SEQ_MAIN;
        }
        break;
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *state = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p->fade)) {
            *state = SEQ_END;
        }
        break;
    case SEQ_END:
        func_ov186_021f9ba0(p->procSys);
        return TRUE;
    }

    for (i = 0; i < BR_BTLSUBWAY_MENU_MAX; i++) {
        if (wk->menuWin[i] != NULL) {
            func_ov186_021fc830(wk->menuWin[i]);
        }
    }
    for (i = 0; i < BR_BTLSUBWAY_MSGWIN_MAX; i++) {
        if (wk->msgWin[i] != NULL) {
            func_ov186_021fc830(wk->msgWin[i]);
        }
    }
    func_0201c4e4(wk->que);
    if (wk->text != NULL) {
        func_ov186_021fd1bc(wk->text);
    }
    func_ov186_021fd854(wk->ballEff);
    return FALSE;
}

static void func_ov183_021dd5c8(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p) {
    Font *font;
    MsgData *msg;
    u32 bp;
    int i;
    BrResObjData obj;

    font = func_ov186_021fc20c(p->res);
    msg = func_ov186_021fc214(p->res);
    func_ov186_021fb964(p->res, 0xe, wk->heapId);

    for (i = 0; i < 1; i++) {
        const BrBtlSubwayMsgWinData *data = &data_ov183_021e3350[i];

        wk->msgWin[i] = func_ov186_021fc6a4(1, data->x, data->y, data->w, data->h, 14, wk->que, wk->heapId);
        func_ov186_021fc868(wk->msgWin[i], 0, 1, 0);
        func_ov186_021fc754(wk->msgWin[i], msg, data->msgID, font, BR_BTLSUBWAY_PRINT_COLOR);
    }

    wk->text = func_ov186_021fd12c(p->res, wk->que, wk->heapId);
    func_ov186_021fd170(wk->text, p->res, 0x86);

    bp = func_0200dabc(p->score);
    func_ov186_021fc124(p->res, BR_RES_OBJ_BTLSUBWAY_BP, &obj);
    func_ov183_021de084(&wk->bp, p->unit, &obj, bp, 104, 88, wk->heapId);
}

static void func_ov183_021dd684(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p) {
    Font *font;
    MsgData *msg;
    WordSet *wordSet;
    u32 mode;
    int i;
    StrBuf *str;
    StrBuf *src;
    u32 bp;
    BrResObjData obj;

    font = func_ov186_021fc20c(p->res);
    msg = func_ov186_021fc214(p->res);
    wordSet = func_ov186_021fc21c(p->res);
    func_ov186_021fb964(p->res, 0xa, wk->heapId);

    if (func_ov183_021de000(p->score, p->gameData)) {
        mode = 5;
    } else {
        mode = 0;
    }

    for (i = 0; i < 8; i++) {
        const BrBtlSubwayMsgWinData *data = &data_ov183_021e33c0[i];

        wk->msgWin[i] = func_ov186_021fc6a4(1, data->x, data->y, data->w, data->h, 14, wk->que, wk->heapId);
        func_ov186_021fc868(wk->msgWin[i], 0, 1, 0);

        switch (i) {
        case 1:
            str = GFL_MsgDataLoadStrbufNew(msg, func_ov183_021de000(p->score, p->gameData) ? 0x83 : 0x7e);
            break;
        case 2:
            str = GFL_MsgDataLoadStrbufNew(msg, func_0200db44(p->score, mode) ? 0x91 : 0x87);
            break;
        case 3: {
            s32 streak = func_0200dac4(p->score, mode);

            streak = (streak > BR_BTLSUBWAY_STREAK_MAX) ? BR_BTLSUBWAY_STREAK_MAX : streak;
            src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            str = GFL_StrBufCreate(0x80, wk->heapId);
            WordSetNumber(wordSet, 0, streak, 4, 1, 1);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            GFL_StrBufFree(src);
            break;
        }
        case 6: {
            s32 maxStreak = func_0200dad8(p->score, mode);

            maxStreak = (maxStreak > BR_BTLSUBWAY_STREAK_MAX) ? BR_BTLSUBWAY_STREAK_MAX : maxStreak;
            src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            str = GFL_StrBufCreate(0x80, wk->heapId);
            WordSetNumber(wordSet, 0, maxStreak, 4, 1, 1);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            GFL_StrBufFree(src);
            break;
        }
        default:
            str = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            break;
        }

        func_ov186_021fc7c4(wk->msgWin[i], str, font, BR_BTLSUBWAY_PRINT_COLOR);
        GFL_StrBufFree(str);
    }

    bp = func_0200dabc(p->score);
    func_ov186_021fc124(p->res, BR_RES_OBJ_BTLSUBWAY_BP, &obj);
    func_ov183_021de084(&wk->bp, p->unit, &obj, bp, 0xa0, 0x20, wk->heapId);
}

void func_ov183_021dd858(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p) {
    Font *font;
    MsgData *msg;
    WordSet *wordSet;
    u16 mode;
    int i;
    StrBuf *str;
    u32 bp;
    BrResObjData obj;

    font = func_ov186_021fc20c(p->res);
    msg = func_ov186_021fc214(p->res);
    wordSet = func_ov186_021fc21c(p->res);
    func_ov186_021fb964(p->res, 0xb, wk->heapId);

    if (func_ov183_021de02c(p->score, p->gameData)) {
        mode = 6;
    } else {
        mode = 1;
    }

    for (i = 0; i < 8; i++) {
        const BrBtlSubwayMsgWinData *data = &data_ov183_021e3400[i];

        wk->msgWin[i] = func_ov186_021fc6a4(1, data->x, data->y, data->w, data->h, 14, wk->que, wk->heapId);
        func_ov186_021fc868(wk->msgWin[i], 0, 1, 0);
        switch (i) {
        case 1:
            str = GFL_MsgDataLoadStrbufNew(msg, func_ov183_021de02c(p->score, p->gameData) ? 0x84 : 0x7f);
            break;
        case 2:
            str = GFL_MsgDataLoadStrbufNew(msg, func_0200db44(p->score, mode) ? 0x91 : 0x87);
            break;
        case 3: {
            s32 streak = func_0200dac4(p->score, mode);
            StrBuf *src;

            streak = (streak > BR_BTLSUBWAY_STREAK_MAX) ? BR_BTLSUBWAY_STREAK_MAX : streak;
            src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            str = GFL_StrBufCreate(0x80, wk->heapId);
            WordSetNumber(wordSet, 0, streak, 4, 1, TRUE);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            GFL_StrBufFree(src);
            break;
        }
        case 6: {
            int streak = func_0200dad8(p->score, mode);
            StrBuf *src;

            streak = (streak > BR_BTLSUBWAY_STREAK_MAX) ? BR_BTLSUBWAY_STREAK_MAX : streak;
            src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            str = GFL_StrBufCreate(0x80, wk->heapId);
            WordSetNumber(wordSet, 0, streak, 4, 1, TRUE);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            GFL_StrBufFree(src);
            break;
        }
        default:
            str = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            break;
        }
        func_ov186_021fc7c4(wk->msgWin[i], str, font, BR_BTLSUBWAY_PRINT_COLOR);
        GFL_StrBufFree(str);
    }

    bp = func_0200dabc(p->score);
    func_ov186_021fc124(p->res, BR_RES_OBJ_BTLSUBWAY_BP, &obj);
    func_ov183_021de084(&wk->bp, p->unit, &obj, bp, 160, 32, wk->heapId);
}

static void func_ov183_021dda2c(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p) {
    Font *font;
    MsgData *msg;
    WordSet *wordSet;
    u32 mode;
    u32 commMode;
    int i;
    StrBuf *str;
    u32 bp;
    BrResObjData obj;

    font = func_ov186_021fc20c(p->res);
    msg = func_ov186_021fc214(p->res);
    wordSet = func_ov186_021fc21c(p->res);
    func_ov186_021fb964(p->res, 0xc, wk->heapId);

    if (func_ov183_021de058(p->score, p->gameData)) {
        mode = 7;
        commMode = 8;
    } else {
        mode = 2;
        commMode = 3;
    }

    for (i = 0; i < BR_BTLSUBWAY_MSGWIN_MAX; i++) {
        const BrBtlSubwayMsgWinData *data = &data_ov183_021e3490[i];

        wk->msgWin[i] = func_ov186_021fc6a4(1, data->x, data->y, data->w, data->h, 14, wk->que, wk->heapId);
        func_ov186_021fc868(wk->msgWin[i], 0, 1, 0);
        switch (i) {
        case 1:
            str = GFL_MsgDataLoadStrbufNew(msg, func_ov183_021de058(p->score, p->gameData) ? 0x85 : 0x80);
            break;
        case 3:
            str = GFL_MsgDataLoadStrbufNew(msg, func_0200db44(p->score, mode) ? 0x91 : 0x87);
            break;
        case 4:
            {
                int streak = MATH_IMin(func_0200dac4(p->score, mode), BR_BTLSUBWAY_STREAK_MAX);
                StrBuf *src;
                src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
                str = GFL_StrBufCreate(0x80, wk->heapId);
                WordSetNumber(wordSet, 0, streak, 4, 1, TRUE);
                GFL_WordSetFormatStrbuf(wordSet, str, src);
                GFL_StrBufFree(src);
                break;
            }
        case 7:
            {
                int streak = MATH_IMin(func_0200dad8(p->score, mode), BR_BTLSUBWAY_STREAK_MAX);
                StrBuf *src;
                src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
                str = GFL_StrBufCreate(0x80, wk->heapId);
                WordSetNumber(wordSet, 0, streak, 4, 1, TRUE);
                GFL_WordSetFormatStrbuf(wordSet, str, src);
                GFL_StrBufFree(src);
                break;
            }
        case 10:
            str = GFL_MsgDataLoadStrbufNew(msg, func_0200db44(p->score, commMode) ? 0x91 : 0x87);
            break;
        case 11:
            {
                int streak = MATH_IMin(func_0200dac4(p->score, commMode), BR_BTLSUBWAY_STREAK_MAX);
                StrBuf *src;
                src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
                str = GFL_StrBufCreate(0x80, wk->heapId);
                WordSetNumber(wordSet, 0, streak, 4, 1, TRUE);
                GFL_WordSetFormatStrbuf(wordSet, str, src);
                GFL_StrBufFree(src);
                break;
            }
        case 14:
            {
                int streak = MATH_IMin(func_0200dad8(p->score, commMode), BR_BTLSUBWAY_STREAK_MAX);
                StrBuf *src;
                src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
                str = GFL_StrBufCreate(0x80, wk->heapId);
                WordSetNumber(wordSet, 0, streak, 4, 1, TRUE);
                GFL_WordSetFormatStrbuf(wordSet, str, src);
                GFL_StrBufFree(src);
                break;
            }
        default:
            str = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            break;
        }
        func_ov186_021fc7c4(wk->msgWin[i], str, font, BR_BTLSUBWAY_PRINT_COLOR);
        GFL_StrBufFree(str);
    }

    bp = func_0200dabc(p->score);
    func_ov186_021fc124(p->res, BR_RES_OBJ_BTLSUBWAY_BP, &obj);
    func_ov183_021de084(&wk->bp, p->unit, &obj, bp, 160, 32, wk->heapId);
}



void func_ov183_021ddccc(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p) {
    Font *font;
    MsgData *msg;
    WordSet *wordSet;
    StrBuf *str;
    StrBuf *src;
    u32 rank;
    int i;
    u32 bp;
    BrResObjData obj;

    font = func_ov186_021fc20c(p->res);
    msg = func_ov186_021fc214(p->res);
    wordSet = func_ov186_021fc21c(p->res);
    func_ov186_021fb964(p->res, 0xd, wk->heapId);

    for (i = 0; i < 5; i++) {
        const BrBtlSubwayMsgWinData *data = &data_ov183_021e338c[i];

        wk->msgWin[i] = func_ov186_021fc6a4(1, data->x, data->y, data->w, data->h, 14, wk->que, wk->heapId);
        if (i == 4) {
            rank = func_0200dc08(p->score);
            src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            str = GFL_StrBufCreate(0x80, wk->heapId);
            WordSetNumber(wordSet, 0, rank, 2, 1, 1);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            GFL_StrBufFree(src);
        } else {
            str = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
        }
        func_ov186_021fc7c4(wk->msgWin[i], str, font, BR_BTLSUBWAY_PRINT_COLOR);
        GFL_StrBufFree(str);
    }

    bp = func_0200dabc(p->score);
    func_ov186_021fc124(p->res, BR_RES_OBJ_BTLSUBWAY_BP, &obj);
    func_ov183_021de084(&wk->bp, p->unit, &obj, bp, 160, 32, wk->heapId);
}




// pokebw2's BrBtlSubway_DeletePage
static void func_ov183_021ddde8(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p) {
    int i;

    func_ov183_021de12c(&wk->bp);
    for (i = 0; i < BR_BTLSUBWAY_MSGWIN_MAX; i++) {
        if (wk->msgWin[i] != NULL) {
            func_ov186_021fc734(wk->msgWin[i]);
            wk->msgWin[i] = NULL;
        }
    }
    func_ov186_021fbda4(p->res, 0xa);
    if (wk->text != NULL) {
        func_ov186_021fd15c(wk->text, p->res);
        wk->text = NULL;
    }
}

void func_ov183_021dde2c(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p) {
    Font *font;
    MsgData *msg;
    ClActorSetup setup;
    BrResObjData obj;
    int i;
    u32 msgID;

    func_ov186_021fbe54(p->res, 6, wk->heapId);
    func_ov186_021fb964(p->res, 9, wk->heapId);
    font = func_ov186_021fc20c(p->res);
    msg = func_ov186_021fc214(p->res);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 80;
    setup.y = 168;
    setup.sequence = 0;
    setup.priority = 1;
    func_ov186_021fc124(p->res, 6, &obj);
    wk->btn = func_ov186_021fb378(&setup, 4, 96, CLACT_SURFACE_SUB, p->unit, wk->bmpoam, font, msg, &obj, wk->heapId);

    for (i = 0; i < BR_BTLSUBWAY_MENU_MAX; i++) {
        const BrBtlSubwayMsgWinData *data = &data_ov183_021e336c[i];

        wk->menuWin[i] = func_ov186_021fc6a4(4, data->x, data->y, data->w, data->h, 14, wk->que, wk->heapId);
        switch (i) {
        case 0:
            if (func_ov183_021de000(p->score, p->gameData)) {
                msgID = 0x83;
            } else {
                msgID = 0x7e;
            }
            break;
        case 1:
            if (func_ov183_021de02c(p->score, p->gameData)) {
                msgID = 0x84;
            } else {
                msgID = 0x7f;
            }
            break;
        case 2:
            if (func_ov183_021de058(p->score, p->gameData)) {
                msgID = 0x85;
            } else {
                msgID = 0x80;
            }
            break;
        case 3:
            msgID = 0x81;
            break;
        }
        func_ov186_021fc754(wk->menuWin[i], msg, msgID, font, BR_BTLSUBWAY_PRINT_COLOR);
    }
}


// pokebw2's BrBtlSubway_DeleteMenu
static void func_ov183_021ddf5c(BrBtlSubwayWork *wk, BrBtlSubwayProcParam *p) {
    int i;

    for (i = 0; i < BR_BTLSUBWAY_MENU_MAX; i++) {
        if (wk->menuWin[i] != NULL) {
            func_ov186_021fc734(wk->menuWin[i]);
            wk->menuWin[i] = NULL;
        }
    }
    if (wk->btn != NULL) {
        func_ov186_021fb494(wk->btn);
        wk->btn = NULL;
    }
    func_ov186_021fc0c8(p->res, 6);
    func_ov186_021fbda4(p->res, 9);
}

// The line of the menu that the touch is on, other than the one shown, or BR_BTLSUBWAY_PAGE_TOP
// pokebw2's BrBtlSubway_GetTrgMenu
static u32 func_ov183_021ddfa0(BrBtlSubwayWork *wk, u32 x, u32 y) {
    u32 select = wk->select;
    int i;

    for (i = 0; i < BR_BTLSUBWAY_PAGE_MAX; i++) {
        if (i != select) {
            const BrBtlSubwayRect *rect = &data_ov183_021e3440[i];

            if ((u32)(x - rect->left) <= (u32)(rect->right - rect->left)
                && (u32)(y - rect->top) <= (u32)(rect->bottom - rect->top)) {
                BrPoint pos;

                pos.x = x;
                pos.y = y;
                func_ov186_021fd860(wk->ballEff, BR_BALL_EFFECT_SPREAD, &pos);
                GFL_SndSEPlay(SEQ_SE_BREC_06);
                return i;
            }
        }
    }
    return BR_BTLSUBWAY_PAGE_TOP;
}

// Whether the Super lines are open and the player has taken the line
// pokebw2's BrBtlSubway_IsSuperSingle
BOOL func_ov183_021de000(BSubwayScoreData *score, GameData *gameData) {
    BOOL flag = func_0200dba0(score, 4, BSUBWAY_FLAG_GET);
    BOOL isOpen = EventWork_FlagGet(GameData_GetEventWork(gameData), EVENT_FLAG_0x0960);

    if (flag && isOpen) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BrBtlSubway_IsSuperDouble
BOOL func_ov183_021de02c(BSubwayScoreData *score, GameData *gameData) {
    BOOL flag = func_0200dba0(score, 5, BSUBWAY_FLAG_GET);
    BOOL isOpen = EventWork_FlagGet(GameData_GetEventWork(gameData), EVENT_FLAG_0x0960);

    if (flag && isOpen) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BrBtlSubway_IsSuperMulti
BOOL func_ov183_021de058(BSubwayScoreData *score, GameData *gameData) {
    BOOL flag = func_0200dba0(score, 6, BSUBWAY_FLAG_GET);
    BOOL isOpen = EventWork_FlagGet(GameData_GetEventWork(gameData), EVENT_FLAG_0x0960);

    if (flag && isOpen) {
        return TRUE;
    }
    return FALSE;
}

// Makes the Battle Points: the icon at the position and four digits to its right
void func_ov183_021de084(BrBtlSubwayBp *bp, ClActUnit *unit, const BrResObjData *obj, u32 value, s16 x, s16 y,
                               HeapID heapId) {
    ClActorSetup setup;
    int i;

    MI_CpuFill8(bp, 0, sizeof(BrBtlSubwayBp));
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = x;
    setup.y = y;
    setup.sequence = 10;
    bp->icon = func_0204b294(unit, obj->chr, obj->plt, obj->cell, &setup, 0, heapId);
    func_0204b688(bp->icon, 1);

    setup.sequence = 0;
    for (i = 0; i < BR_BTLSUBWAY_BP_DIGITS; i++) {
        setup.x = x + 20 + i * 16;
        setup.y = y;
        bp->digit[i] = func_0204b294(unit, obj->chr, obj->plt, obj->cell, &setup, 0, heapId);
        func_0204b688(bp->digit[i], 1);
    }
    func_ov183_021de15c(bp, value);
}

// pokebw2's BrBtlSubwayBp_Exit
static void func_ov183_021de12c(BrBtlSubwayBp *bp) {
    int i;

    for (i = 0; i < BR_BTLSUBWAY_BP_DIGITS; i++) {
        if (bp->digit[i] != NULL) {
            func_0204b3b4(bp->digit[i]);
            bp->digit[i] = NULL;
        }
    }
    func_0204b3b4(bp->icon);
    MI_CpuFill8(bp, 0, sizeof(BrBtlSubwayBp));
}

// pokebw2's BrBtlSubwayBp_SetValue
void func_ov183_021de15c(BrBtlSubwayBp *bp, u32 value) {
    static const u32 data_ov183_021e3358[] = { 1, 10, 100, 1000, 10000 };
    u8 rest = value;
    int keta = 0;
    int i;

    // The number of digits, which nothing uses
    if (rest != 0) {
        for (;;) {
            if (rest == 0) {
                break;
            }
            rest /= 10;
            keta++;
        }
    }

    for (i = 0; i < BR_BTLSUBWAY_BP_DIGITS; i++) {
        func_0204b878(bp->digit[i], 0);
    }
    for (i = 0; i < BR_BTLSUBWAY_BP_DIGITS; i++) {
        u32 number = value % data_ov183_021e3358[i + 1] / data_ov183_021e3358[i];

        func_0204b878(bp->digit[BR_BTLSUBWAY_BP_DIGITS - 1 - i], number);
    }
}
