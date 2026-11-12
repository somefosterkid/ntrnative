// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_bvdelete_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's battle video deletion screen: it asks twice whether to delete the saved battle video of a
// slot, then deletes it and says so. The name is a guess: the ROM gives none, and br_core.c runs it as
// BR_PROCID_BV_DELETE

#include "types.h"
#include "app/battle_recorder/br_bvdelete_proc.h"
#include "app/battle_rec_ov.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "constants/sound.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "system/printsys.h"
#include "system/wordset.h"

// The answers of the question
enum {
    MSGWIN_YES,
    MSGWIN_NO,
    MSGWIN_MAX,
};

#define BR_BVDELETE_PRINT_COLOR 0x3da0

typedef struct {
    PrintQueue *que;
    BrMsgWin *text;
    BrSeq *seq;
    BrMsgWin *msgWin[MSGWIN_MAX];
    // For the main and the sub screen
    BrBallEffect *ballEff[2];
    u16 heapId;
    BrBvDeleteProcParam *p_param;
    u32 cnt;
} BrBvDeleteWork;

// Where a window is, in tiles, and its message
typedef struct {
    u8 x;
    u8 y;
    u8 w;
    u8 h;
    u32 msgID;
} BrBvDeleteMsgWinData;

static BOOL func_ov183_021e2028(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021e20a4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021e20f8(GameProc *proc, u32 *state, void *param, void *work);
void func_ov183_021e214c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov183_021e2194(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021e21e0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov183_021e23c8(BrBvDeleteWork *p_wk, BrBvDeleteProcParam *p_param);
static void func_ov183_021e24ac(BrBvDeleteWork *p_wk, BrBvDeleteProcParam *p_param);
static void func_ov183_021e24c4(BrBvDeleteWork *p_wk, BrBvDeleteProcParam *p_param);
BOOL func_ov183_021e24f8(BrBvDeleteWork *p_wk, u32 x, u32 y);
BOOL func_ov183_021e2538(BrBvDeleteWork *p_wk, u32 x, u32 y);

// pokebw2's BR_BVDELETE_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e38f4 = {
    func_ov183_021e2028,
    func_ov183_021e20f8,
    func_ov183_021e20a4,
};

// pokebw2's sBrBvDeleteConfirmWins
static const BrBvDeleteMsgWinData data_ov183_021e3900[MSGWIN_MAX] = {
    [MSGWIN_YES] = { 4, 6, 9, 2, 0x9f },
    [MSGWIN_NO] = { 18, 6, 9, 2, 0xa0 },
};

// pokebw2's BrBvDelete_ProcInit
static BOOL func_ov183_021e2028(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvDeleteProcParam *p_param = param;
    BrBvDeleteWork *p_wk;
    int i;

    GFL_ASSERT(p_param->mode < BR_BVDELETE_MODE_MAX);

    p_wk = GFL_ProcInitSubsystem(proc, sizeof(BrBvDeleteWork), func_ov186_021f9b9c(p_param->procSys));
    MI_CpuFill8(p_wk, 0, sizeof(BrBvDeleteWork));
    p_wk->p_param = p_param;
    p_wk->heapId = func_ov186_021f9b9c(p_param->procSys);

    p_wk->seq = func_ov186_021fd6d8(p_wk, func_ov183_021e214c, p_wk->heapId);
    p_wk->que = func_0201c440(p_wk->heapId);
    func_ov183_021e23c8(p_wk, p_param);
    for (i = 0; i < 2; i++) {
        p_wk->ballEff[i] = func_ov186_021fd760(p_param->unit, p_param->res, i, p_wk->heapId);
    }
    return TRUE;
}

// pokebw2's BrBvDelete_ProcExit
static BOOL func_ov183_021e20a4(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvDeleteProcParam *p_param = param;
    BrBvDeleteWork *p_wk = work;
    int i;

    for (i = 0; i < 2; i++) {
        func_ov186_021fd820(p_wk->ballEff[i]);
    }
    if (p_wk->p_param->isDelete) {
        func_ov183_021e24ac(p_wk, p_param);
    } else {
        func_ov183_021e24ac(p_wk, p_param);
        func_ov183_021e24c4(p_wk, p_param);
    }
    func_0201c4c0(p_wk->que);
    func_ov186_021fd714(p_wk->seq);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrBvDelete_ProcMain
static BOOL func_ov183_021e20f8(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvDeleteWork *p_wk = work;
    int i;

    func_ov186_021fd71c(p_wk->seq);
    if (func_ov186_021fd72c(p_wk->seq)) {
        return TRUE;
    }

    for (i = 0; i < 2; i++) {
        func_ov186_021fd854(p_wk->ballEff[i]);
    }
    func_0201c4e4(p_wk->que);
    if (p_wk->text != NULL) {
        func_ov186_021fd1bc(p_wk->text);
    }
    for (i = 0; i < MSGWIN_MAX; i++) {
        if (p_wk->msgWin[i] != NULL) {
            func_ov186_021fc830(p_wk->msgWin[i]);
        }
    }
    return FALSE;
}

// pokebw2's BrBvDelete_SeqFadeIn
void func_ov183_021e214c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrBvDeleteWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEIN_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_NEXT;
        }
        break;
    case SEQ_NEXT:
        func_ov186_021fd73c(p_seqwk, func_ov183_021e21e0);
        break;
    }
}

// pokebw2's BrBvDelete_SeqFadeOut
static void func_ov183_021e2194(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_END,
    };
    BrBvDeleteWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_END;
        }
        break;
    case SEQ_END:
        func_ov186_021f9ba0(p_wk->p_param->procSys);
        func_ov186_021fd744(p_seqwk);
        break;
    }
}

// Asks twice, then deletes the video and shows the result until the screen is touched
// pokebw2's BrBvDelete_SeqMain
void func_ov183_021e21e0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_INPUT,
        SEQ_INPUT_AGAIN,
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_DELETE_START,
        SEQ_DELETE_MAIN,
        SEQ_DELETE_WAIT,
        SEQ_MSG_WAIT,
        SEQ_END,
    };
    BrBvDeleteWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_INPUT: {
        u32 x;
        u32 y;

        if (func_02035d20(&x, &y)) {
            if (func_ov183_021e24f8(p_wk, x, y)) {
                func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x142);
                *p_seq = SEQ_INPUT_AGAIN;
            } else if (func_ov183_021e2538(p_wk, x, y)) {
                *p_seq = SEQ_END;
            }
        }
        break;
    }
    case SEQ_INPUT_AGAIN: {
        u32 x;
        u32 y;

        if (func_02035d20(&x, &y)) {
            if (func_ov183_021e24f8(p_wk, x, y)) {
                *p_seq = SEQ_FADEOUT_START;
            } else if (func_ov183_021e2538(p_wk, x, y)) {
                *p_seq = SEQ_END;
            }
        }
        break;
    }
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_SUB, BR_FADE_DIR_OUT);
        (*p_seq)++;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            BrPoint pos;

            func_ov183_021e24c4(p_wk, p_wk->p_param);
            func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x143);
            pos.x = 0x80;
            pos.y = 0x60;
            func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_ROT, &pos);
            p_wk->cnt = 0;
            (*p_seq)++;
        }
        break;
    case SEQ_FADEIN_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_SUB, BR_FADE_DIR_IN);
        (*p_seq)++;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            (*p_seq)++;
        }
        break;
    case SEQ_DELETE_START:
        func_ov125_021d76a8(p_wk->p_param->gameData, p_wk->heapId, p_wk->p_param->mode);
        p_wk->cnt++;
        (*p_seq)++;
        break;
    case SEQ_DELETE_MAIN: {
        u32 ret = func_ov125_021d76d8(p_wk->p_param->gameData, p_wk->p_param->mode);

        p_wk->cnt++;
        if (ret == 2 || ret == 3) {
            p_wk->p_param->isDelete = TRUE;
            *p_seq = SEQ_DELETE_WAIT;
        }
        break;
    }
    case SEQ_DELETE_WAIT:
        // The balls turn for at least 96 frames
        if (p_wk->cnt++ > 96) {
            func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_NONE, NULL);
            func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x144);
            *p_seq = SEQ_MSG_WAIT;
        }
        break;
    case SEQ_MSG_WAIT:
        if (func_02035c60()) {
            u32 x;
            u32 y;
            BrPoint pos;

            func_02035d20(&x, &y);
            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
            GFL_SndSEPlay(SEQ_SE_BREC_06);
            *p_seq = SEQ_END;
        }
        break;
    case SEQ_END:
        func_ov186_021fd73c(p_seqwk, func_ov183_021e2194);
        break;
    }
}

// pokebw2's BrBvDelete_CreateConfirm
static void func_ov183_021e23c8(BrBvDeleteWork *p_wk, BrBvDeleteProcParam *p_param) {
    WordSet *wordSet;
    StrBuf *src;
    StrBuf *str;
    Font *font;
    MsgData *msg;
    int i;

    if (p_wk->text == NULL) {
        p_wk->text = func_ov186_021fd12c(p_param->res, p_wk->que, p_wk->heapId);
    }
    wordSet = func_ov186_021fc21c(p_param->res);
    src = GFL_MsgDataLoadStrbufNew(func_ov186_021fc214(p_param->res), 0x141);
    str = GFL_StrBufCreate(0x80, p_wk->heapId);
    func_0201ee0c(wordSet, 0, p_param->recordInfo->name[p_param->mode], p_param->recordInfo->sex[p_param->mode], 1,
                  2);
    GFL_WordSetFormatStrbuf(wordSet, str, src);
    func_ov186_021fd19c(p_wk->text, p_param->res, str);
    GFL_StrBufFree(str);
    GFL_StrBufFree(src);

    func_ov186_021fb964(p_param->res, 0x1e, p_wk->heapId);
    font = func_ov186_021fc20c(p_param->res);
    msg = func_ov186_021fc214(p_param->res);
    for (i = 0; i < MSGWIN_MAX; i++) {
        const BrBvDeleteMsgWinData *data = &data_ov183_021e3900[i];

        p_wk->msgWin[i] = func_ov186_021fc6a4(4, data->x, data->y, data->w, data->h, 14, p_wk->que, p_wk->heapId);
        func_ov186_021fc754(p_wk->msgWin[i], msg, data->msgID, font, BR_BVDELETE_PRINT_COLOR);
    }
}

// pokebw2's BrBvDelete_DeleteText
static void func_ov183_021e24ac(BrBvDeleteWork *p_wk, BrBvDeleteProcParam *p_param) {
    if (p_wk->text != NULL) {
        func_ov186_021fd15c(p_wk->text, p_param->res);
        p_wk->text = NULL;
    }
}

// pokebw2's BrBvDelete_DeleteConfirm
static void func_ov183_021e24c4(BrBvDeleteWork *p_wk, BrBvDeleteProcParam *p_param) {
    int i;

    for (i = 0; i < MSGWIN_MAX; i++) {
        if (p_wk->msgWin[i] != NULL) {
            func_ov186_021fc734(p_wk->msgWin[i]);
            p_wk->msgWin[i] = NULL;
        }
    }
    GFL_BGSysLoadScr(4);
    func_ov186_021fbda4(p_param->res, 0x1e);
}

// The answers of the question
// pokebw2's BrBvDelete_GetTrgYes
BOOL func_ov183_021e24f8(BrBvDeleteWork *p_wk, u32 x, u32 y) {
    BOOL ret = ((u32)(x - 32) <= (u32)(104 - 32)) & ((u32)(y - 48) <= (u32)(64 - 48));

    if (ret) {
        BrPoint pos;

        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        GFL_SndSEPlay(SEQ_SE_BREC_06);
    }
    return ret;
}

// pokebw2's BrBvDelete_GetTrgNo
BOOL func_ov183_021e2538(BrBvDeleteWork *p_wk, u32 x, u32 y) {
    BOOL ret = ((u32)(x - 144) <= (u32)(216 - 144)) & ((u32)(y - 48) <= (u32)(64 - 48));

    if (ret) {
        BrPoint pos;

        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        GFL_SndSEPlay(SEQ_SE_BREC_06);
    }
    return ret;
}
