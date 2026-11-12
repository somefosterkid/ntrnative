// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_rndmatch_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's Random Matchup records screen: a menu of the records on the sub screen, and on the main screen
// the player's ratings or the wins and losses of the Rating and Free modes, by battle type. The name is a guess: the
// ROM gives none, and br_core.c runs it as BR_PROCID_RNDMATCH

#include "types.h"
#include "app/battle_recorder/br_btn.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "constants/sound.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "save/records.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/wordset.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
BrBallEffect *func_ov186_021fd760(ClActUnit *unit, BrRes *res, u32 display, HeapID heapId);
void func_ov186_021fbe54(BrRes *p_wk, u32 objID, HeapID heapId);

// The records of the menu, which are what the main screen shows
enum {
    BR_RNDMATCH_PAGE_TOP,
    BR_RNDMATCH_PAGE_RATING,
    BR_RNDMATCH_PAGE_RATE,
    BR_RNDMATCH_PAGE_FREE,
    BR_RNDMATCH_PAGE_MAX,
};

#define BR_RNDMATCH_MENU_MAX 3
#define BR_RNDMATCH_MSGWIN_MAX 18
#define BR_RNDMATCH_PRINT_COLOR 0x3da0

// The battle types of a mode, and where the Rating mode's results are in the Random Matchup records, after the Free
// mode's
#define BR_RNDMATCH_BTL_TYPE_MAX 5
#define BR_RNDMATCH_RECORD_RATE BR_RNDMATCH_BTL_TYPE_MAX

// The fields of a result of the Random Matchup records
enum {
    BR_RNDMATCH_FIELD_RATING,
    BR_RNDMATCH_FIELD_WIN,
    BR_RNDMATCH_FIELD_LOSE,
};

typedef struct {
    BmpOamSys *bmpoam;
    PrintQueue *que;
    BrMsgWin *text;
    BrBtn *btn;
    BrMsgWin *menuWin[BR_RNDMATCH_MENU_MAX];
    BrMsgWin *msgWin[BR_RNDMATCH_MSGWIN_MAX];
    BrBallEffect *ballEff;
    u32 select;
    HeapID heapId;
} BrRndMatchWork;

// Where a window is, in tiles, and its message
typedef struct {
    u8 x;
    u8 y;
    u8 w;
    u8 h;
    u32 msgID;
} BrRndMatchMsgWinData;

typedef struct {
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
} BrRndMatchRect;

static BOOL func_ov183_021de1c8(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021de234(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021de274(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov183_021de3e4(BrRndMatchWork *wk, BrRndMatchProcParam *p);
static void func_ov183_021de404(BrRndMatchWork *wk, BrRndMatchProcParam *p);
static void func_ov183_021de55c(BrRndMatchWork *wk, BrRndMatchProcParam *p);
static void func_ov183_021de66c(BrRndMatchWork *wk, BrRndMatchProcParam *p);
static void func_ov183_021de780(BrRndMatchWork *wk, BrRndMatchProcParam *p);
static void func_ov183_021de7c0(BrRndMatchWork *wk, BrRndMatchProcParam *p);
static void func_ov183_021de8fc(BrRndMatchWork *wk, BrRndMatchProcParam *p);
static u32 func_ov183_021de940(BrRndMatchWork *wk, u32 x, u32 y);

// pokebw2's BR_RNDMATCH_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e3510 = {
    func_ov183_021de1c8,
    func_ov183_021de274,
    func_ov183_021de234,
};

// pokebw2's sBrRndMatchMenuWins
static const BrRndMatchMsgWinData data_ov183_021e351c[BR_RNDMATCH_MENU_MAX] = {
#include "asset/br_rndmatch_proc_data_ov183_021e351c.inc"
};

// The records of the menu on the sub screen. The first, of no record, is never hit
// pokebw2's sBrRndMatchMenuRects
static const BrRndMatchRect data_ov183_021e3534[BR_RNDMATCH_PAGE_MAX] = {
#include "asset/br_rndmatch_proc_data_ov183_021e3534.inc"
};

// pokebw2's sBrRndMatchRatingWins
static const BrRndMatchMsgWinData data_ov183_021e3574[] = {
#include "asset/br_rndmatch_proc_data_ov183_021e3574.inc"
};

// pokebw2's sBrRndMatchFreeWins
static const BrRndMatchMsgWinData data_ov183_021e35d4[BR_RNDMATCH_MSGWIN_MAX] = {
#include "asset/br_rndmatch_proc_data_ov183_021e35d4.inc"
};

// pokebw2's sBrRndMatchRateWins
static const BrRndMatchMsgWinData data_ov183_021e3664[BR_RNDMATCH_MSGWIN_MAX] = {
#include "asset/br_rndmatch_proc_data_ov183_021e3664.inc"
};

// pokebw2's BrRndMatch_ProcInit
static BOOL func_ov183_021de1c8(GameProc *proc, u32 *state, void *param, void *work) {
    BrRndMatchProcParam *p = param;
    BrRndMatchWork *wk;

    wk = GFL_ProcInitSubsystem(proc, sizeof(BrRndMatchWork), func_ov186_021f9b9c(p->procSys));
    MI_CpuFill8(wk, 0, sizeof(BrRndMatchWork));
    wk->heapId = func_ov186_021f9b9c(p->procSys);

    wk->bmpoam = func_02025538(wk->heapId, p->unit);
    wk->que = func_0201c440(wk->heapId);
    wk->ballEff = func_ov186_021fd760(p->unit, p->res, CLACT_SURFACE_SUB, wk->heapId);

    func_ov183_021de3e4(wk, p);
    func_ov183_021de7c0(wk, p);
    return TRUE;
}

// pokebw2's BrRndMatch_ProcExit
static BOOL func_ov183_021de234(GameProc *proc, u32 *state, void *param, void *work) {
    BrRndMatchProcParam *p = param;
    BrRndMatchWork *wk = work;

    func_ov183_021de780(wk, p);
    func_ov183_021de8fc(wk, p);
    GFL_BGSysLoadScr(4);
    GFL_BGSysLoadScr(1);
    func_ov186_021fd820(wk->ballEff);
    func_0201c4c0(wk->que);
    func_02025588(wk->bmpoam);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrRndMatch_ProcMain
static BOOL func_ov183_021de274(GameProc *proc, u32 *state, void *param, void *work) {
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
    BrRndMatchProcParam *p = param;
    BrRndMatchWork *wk = work;
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
            wk->select = func_ov183_021de940(wk, x, y);
            if (wk->select != BR_RNDMATCH_PAGE_TOP) {
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
        func_ov183_021de780(wk, p);
        switch (wk->select) {
        case BR_RNDMATCH_PAGE_RATING:
            func_ov183_021de404(wk, p);
            break;
        case BR_RNDMATCH_PAGE_RATE:
            func_ov183_021de55c(wk, p);
            break;
        case BR_RNDMATCH_PAGE_FREE:
            func_ov183_021de66c(wk, p);
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

    for (i = 0; i < BR_RNDMATCH_MENU_MAX; i++) {
        if (wk->menuWin[i] != NULL) {
            func_ov186_021fc830(wk->menuWin[i]);
        }
    }
    for (i = 0; i < BR_RNDMATCH_MSGWIN_MAX; i++) {
        if (wk->msgWin[i] != NULL) {
            func_ov186_021fc830(wk->msgWin[i]);
        }
    }
    func_0201c4e4(wk->que);
    func_ov186_021fd854(wk->ballEff);
    if (wk->text != NULL) {
        func_ov186_021fd1bc(wk->text);
    }
    return FALSE;
}

// The main screen before a record is chosen: the explanation
// pokebw2's BrRndMatch_CreateTop
static void func_ov183_021de3e4(BrRndMatchWork *wk, BrRndMatchProcParam *p) {
    wk->text = func_ov186_021fd12c(p->res, wk->que, wk->heapId);
    func_ov186_021fd170(wk->text, p->res, 0x86);
}

// The player's rating in each battle type
// pokebw2's BrRndMatch_CreateRating
static void func_ov183_021de404(BrRndMatchWork *wk, BrRndMatchProcParam *p) {
    Font *font;
    MsgData *msg;
    WordSet *wordSet;
    StrBuf *str;
    int i;

    font = func_ov186_021fc20c(p->res);
    msg = func_ov186_021fc214(p->res);
    wordSet = func_ov186_021fc21c(p->res);
    func_ov186_021fb964(p->res, 0x10, wk->heapId);

    for (i = 0; i < 12; i++) {
        BOOL isFormat = TRUE;
        const BrRndMatchMsgWinData *data = &data_ov183_021e3574[i];

        wk->msgWin[i] = func_ov186_021fc6a4(1, data->x, data->y, data->w, data->h, 14, wk->que, wk->heapId);
        func_ov186_021fc868(wk->msgWin[i], 0, 1, BR_MSGWIN_POS_ABSOLUTE);
        switch (i) {
        case 0:
            copyVarForText(wordSet, 0, GetGameDataPlayerInfo(p->gameData));
            break;
        case 7:
            WordSetNumber(wordSet, 0,
                          func_0200dfd8(p->record, BR_RNDMATCH_RECORD_RATE + 0, BR_RNDMATCH_FIELD_RATING), 4, 1,
                          TRUE);
            break;
        case 8:
            WordSetNumber(wordSet, 0,
                          func_0200dfd8(p->record, BR_RNDMATCH_RECORD_RATE + 1, BR_RNDMATCH_FIELD_RATING), 4, 1,
                          TRUE);
            break;
        case 9:
            WordSetNumber(wordSet, 0,
                          func_0200dfd8(p->record, BR_RNDMATCH_RECORD_RATE + 2, BR_RNDMATCH_FIELD_RATING), 4, 1,
                          TRUE);
            break;
        case 10:
            WordSetNumber(wordSet, 0,
                          func_0200dfd8(p->record, BR_RNDMATCH_RECORD_RATE + 3, BR_RNDMATCH_FIELD_RATING), 4, 1,
                          TRUE);
            break;
        case 11:
            WordSetNumber(wordSet, 0,
                          func_0200dfd8(p->record, BR_RNDMATCH_RECORD_RATE + 4, BR_RNDMATCH_FIELD_RATING), 4, 1,
                          TRUE);
            break;
        default:
            str = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            isFormat = FALSE;
            break;
        }
        if (isFormat) {
            StrBuf *src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);

            str = GFL_StrBufCreate(0x80, wk->heapId);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            GFL_StrBufFree(src);
        }
        func_ov186_021fc7c4(wk->msgWin[i], str, font, BR_RNDMATCH_PRINT_COLOR);
        GFL_StrBufFree(str);
    }
}

// The wins and losses of the Rating mode in each battle type
// pokebw2's BrRndMatch_CreateRate
static void func_ov183_021de55c(BrRndMatchWork *wk, BrRndMatchProcParam *p) {
    Font *font;
    MsgData *msg;
    WordSet *wordSet;
    StrBuf *str;
    int i;
    u32 value;
    BOOL isFormat;

    font = func_ov186_021fc20c(p->res);
    msg = func_ov186_021fc214(p->res);
    wordSet = func_ov186_021fc21c(p->res);
    func_ov186_021fb964(p->res, 0x11, wk->heapId);

    for (i = 0; i < BR_RNDMATCH_MSGWIN_MAX; i++) {
        const BrRndMatchMsgWinData *data = &data_ov183_021e3664[i];

        wk->msgWin[i] = func_ov186_021fc6a4(1, data->x, data->y, data->w, data->h, 14, wk->que, wk->heapId);
        func_ov186_021fc868(wk->msgWin[i], 0, 1, BR_MSGWIN_POS_ABSOLUTE);
        if (8 <= i && i <= 12) {
            value = func_0200dfd8(p->record, i - 8 + BR_RNDMATCH_RECORD_RATE, BR_RNDMATCH_FIELD_WIN);
            isFormat = TRUE;
        } else if (13 <= i && i <= 17) {
            value = func_0200dfd8(p->record, i - 13 + BR_RNDMATCH_RECORD_RATE, BR_RNDMATCH_FIELD_LOSE);
            isFormat = TRUE;
        } else {
            str = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            isFormat = FALSE;
        }
        if (isFormat) {
            StrBuf *src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);

            str = GFL_StrBufCreate(0x80, wk->heapId);
            WordSetNumber(wordSet, 0, value, 5, 1, TRUE);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            GFL_StrBufFree(src);
        }
        func_ov186_021fc7c4(wk->msgWin[i], str, font, BR_RNDMATCH_PRINT_COLOR);
        GFL_StrBufFree(str);
    }
}

// The wins and losses of the Free mode in each battle type
// pokebw2's BrRndMatch_CreateFree
static void func_ov183_021de66c(BrRndMatchWork *wk, BrRndMatchProcParam *p) {
    Font *font;
    MsgData *msg;
    WordSet *wordSet;
    StrBuf *str;
    int i;
    u32 value;
    BOOL isFormat;

    font = func_ov186_021fc20c(p->res);
    msg = func_ov186_021fc214(p->res);
    wordSet = func_ov186_021fc21c(p->res);
    func_ov186_021fb964(p->res, 0x11, wk->heapId);

    for (i = 0; i < BR_RNDMATCH_MSGWIN_MAX; i++) {
        const BrRndMatchMsgWinData *data = &data_ov183_021e35d4[i];

        wk->msgWin[i] = func_ov186_021fc6a4(1, data->x, data->y, data->w, data->h, 14, wk->que, wk->heapId);
        func_ov186_021fc868(wk->msgWin[i], 0, 1, BR_MSGWIN_POS_ABSOLUTE);
        if (8 <= i && i <= 12) {
            value = func_0200dfd8(p->record, i - 8, BR_RNDMATCH_FIELD_WIN);
            isFormat = TRUE;
        } else if (13 <= i && i <= 17) {
            value = func_0200dfd8(p->record, i - 13, BR_RNDMATCH_FIELD_LOSE);
            isFormat = TRUE;
        } else {
            str = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            isFormat = FALSE;
        }
        if (isFormat) {
            StrBuf *src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);

            str = GFL_StrBufCreate(0x80, wk->heapId);
            WordSetNumber(wordSet, 0, value, 5, 1, TRUE);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            GFL_StrBufFree(src);
        }
        func_ov186_021fc7c4(wk->msgWin[i], str, font, BR_RNDMATCH_PRINT_COLOR);
        GFL_StrBufFree(str);
    }
}

// pokebw2's BrRndMatch_DeletePage
static void func_ov183_021de780(BrRndMatchWork *wk, BrRndMatchProcParam *p) {
    int i;

    for (i = 0; i < BR_RNDMATCH_MSGWIN_MAX; i++) {
        if (wk->msgWin[i] != NULL) {
            func_ov186_021fc734(wk->msgWin[i]);
            wk->msgWin[i] = NULL;
        }
    }
    func_ov186_021fbda4(p->res, 0x11);
    if (wk->text != NULL) {
        func_ov186_021fd15c(wk->text, p->res);
        wk->text = NULL;
    }
}

// Makes the menu of the records and the return button on the sub screen
// pokebw2's BrRndMatch_CreateMenu
static void func_ov183_021de7c0(BrRndMatchWork *wk, BrRndMatchProcParam *p) {
    Font *font;
    WordSet *wordSet;
    MsgData *msg;
    ClActorSetup setup;
    BrResObjData obj;
    StrBuf *src;
    StrBuf *str;
    PlayerInfo *player;
    int i;

    func_ov186_021fbe54(p->res, 6, wk->heapId);
    func_ov186_021fb964(p->res, 0xf, wk->heapId);
    font = func_ov186_021fc20c(p->res);
    msg = func_ov186_021fc214(p->res);
    wordSet = func_ov186_021fc21c(p->res);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 80;
    setup.y = 168;
    setup.sequence = 0;
    setup.priority = 1;
    func_ov186_021fc124(p->res, 6, &obj);
    wk->btn = func_ov186_021fb378(&setup, 4, 96, CLACT_SURFACE_SUB, p->unit, wk->bmpoam, font, msg, &obj, wk->heapId);

    src = GFL_StrBufCreate(0x80, wk->heapId);
    str = GFL_StrBufCreate(0x80, wk->heapId);
    player = GetGameDataPlayerInfo(p->gameData);

    for (i = 0; i < BR_RNDMATCH_MENU_MAX; i++) {
        const BrRndMatchMsgWinData *data = &data_ov183_021e351c[i];

        wk->menuWin[i] = func_ov186_021fc6a4(4, data->x, data->y, data->w, data->h, 14, wk->que, wk->heapId);
        if (i == 0) {
            // The player's name
            GFL_MsgDataLoadStrbuf(msg, data->msgID, src);
            copyVarForText(wordSet, 0, player);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
        } else {
            GFL_MsgDataLoadStrbuf(msg, data->msgID, str);
        }
        func_ov186_021fc7c4(wk->menuWin[i], str, font, BR_RNDMATCH_PRINT_COLOR);
    }

    GFL_StrBufFree(src);
    GFL_StrBufFree(str);
}

// pokebw2's BrRndMatch_DeleteMenu
static void func_ov183_021de8fc(BrRndMatchWork *wk, BrRndMatchProcParam *p) {
    int i;

    for (i = 0; i < BR_RNDMATCH_MENU_MAX; i++) {
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
    func_ov186_021fbda4(p->res, 0xf);
}

// The record of the menu that the touch is on, other than the one shown, or BR_RNDMATCH_PAGE_TOP
// pokebw2's BrRndMatch_GetTrgMenu
static u32 func_ov183_021de940(BrRndMatchWork *wk, u32 x, u32 y) {
    u32 select = wk->select;
    int i;

    for (i = 0; i < BR_RNDMATCH_PAGE_MAX; i++) {
        if (i != select) {
            const BrRndMatchRect *rect = &data_ov183_021e3534[i];

            if ((u32)(x - rect->left) <= (u32)(rect->right - rect->left) &&
                (u32)(y - rect->top) <= (u32)(rect->bottom - rect->top)) {
                BrPoint pos;

                pos.x = x;
                pos.y = y;
                func_ov186_021fd860(wk->ballEff, BR_BALL_EFFECT_SPREAD, &pos);
                GFL_SndSEPlay(SEQ_SE_BREC_06);
                return i;
            }
        }
    }
    return BR_RNDMATCH_PAGE_TOP;
}
