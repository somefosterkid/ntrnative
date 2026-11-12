// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_start_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's opening and closing screen: it shows the start-up message and opens the sidebars, closes them
// when the player leaves, or opens the menu at once after a battle video. The name is a guess: the ROM gives none, and
// br_core.c runs it as BR_PROCID_START

#include "types.h"
#include "app/battle_recorder/br_start_proc.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_sidebar.h"
#include "app/battle_recorder/br_util.h"
#include "constants/sound.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "system/printsys.h"
#include "gfl/proc.h"

typedef struct {
    BrSeq *seq;
    HeapID heapId;
    BrMsgWin *msgWin;
    PrintQueue *que;
    u32 cnt;
    // For the main and the sub screen
    BrBallEffect *ballEff[2];
    BrStartProcParam *param;
} BrStartWork;

BOOL func_ov183_021db2e0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021db36c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021db394(GameProc *proc, u32 *state, void *param, void *work);
void func_ov183_021db3c4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021db594(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021db61c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov183_021db690(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);

// pokebw2's BR_START_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e3268 = {
    func_ov183_021db2e0,
    func_ov183_021db394,
    func_ov183_021db36c,
};

// pokebw2's BrStart_ProcExit

BOOL func_ov183_021db2e0(GameProc *proc, u32 *state, void *param, void *work) {
    BrStartProcParam *p = param;
    BrStartWork *wk;
    BrSeqFunc seqFunc;

    wk = GFL_ProcInitSubsystem(proc, sizeof(BrStartWork), func_ov186_021f9b9c(p->procSys));
    MI_CpuFill8(wk, 0, sizeof(BrStartWork));
    wk->param = p;
    wk->heapId = func_ov186_021f9b9c(p->procSys);
    wk->que = func_0201c440(wk->heapId);
    wk->ballEff[0] = func_ov186_021fd760(p->unit, p->res, 0, wk->heapId);
    wk->ballEff[1] = func_ov186_021fd760(p->unit, p->res, 1, wk->heapId);
    func_ov186_021fd940(wk->ballEff[0], 1);
    func_ov186_021fd940(wk->ballEff[1], 1);

    if (p->mode == BR_START_MODE_OPEN) {
        seqFunc = func_ov183_021db3c4;
    } else if (p->mode == BR_START_MODE_CLOSE) {
        seqFunc = func_ov183_021db594;
    } else {
        seqFunc = func_ov183_021db61c;
    }
    wk->seq = func_ov186_021fd6d8(wk, seqFunc, wk->heapId);
    return TRUE;
}

static BOOL func_ov183_021db36c(GameProc *proc, u32 *state, void *param, void *work) {
    BrStartWork *wk = work;

    func_ov186_021fd820(wk->ballEff[CLACT_SURFACE_MAIN]);
    func_ov186_021fd820(wk->ballEff[CLACT_SURFACE_SUB]);
    func_0201c4c0(wk->que);
    func_ov186_021fd714(wk->seq);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrStart_ProcMain
static BOOL func_ov183_021db394(GameProc *proc, u32 *state, void *param, void *work) {
    BrStartWork *wk = work;

    func_ov186_021fd71c(wk->seq);
    func_0201c4e4(wk->que);
    func_ov186_021fd854(wk->ballEff[CLACT_SURFACE_MAIN]);
    func_ov186_021fd854(wk->ballEff[CLACT_SURFACE_SUB]);
    if (func_ov186_021fd72c(wk->seq)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov183_021db3c4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrStartWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case 0: {
        BrPoint pos;

        pos.x = 0x80;
        pos.y = 0x60;
        func_ov186_021fd860(wk->ballEff[0], BR_BALL_EFFECT_LINE, &pos);
        GFL_SndSEPlay(SEQ_SE_BREC_03);
        *p_seq = 1;
        break;
    }
    case 1:
        if (func_ov186_021fd938(wk->ballEff[0])) {
            *p_seq = 2;
        }
        break;
    case 2: {
        BrPoint pos;

        pos.x = 0x80;
        pos.y = -8;
        func_ov186_021fd860(wk->ballEff[1], BR_BALL_EFFECT_LINE_ROT, &pos);
        *p_seq = 3;
        break;
    }
    case 3:
        if (func_ov186_021fd938(wk->ballEff[1])) {
            *p_seq = 4;
        }
        break;
    case 4: {
        Font *font;
        MsgData *msg;

        wk->msgWin = func_ov186_021fc6a4(4, 8, 16, 16, 2, 14, wk->que, wk->heapId);
        font = func_ov186_021fc20c(wk->param->res);
        msg = func_ov186_021fc214(wk->param->res);
        func_ov186_021fc754(wk->msgWin, msg, 0xe4, font, 0x440);
        func_ov186_021fe794(wk->param->fade, BR_FADE_TYPE_PLTT, 0xf, BR_FADE_DIR_OUT, 4);
        *p_seq = 5;
        break;
    }
    case 5:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = 6;
        }
        break;
    case 6:
        func_ov186_021fe794(wk->param->fade, BR_FADE_TYPE_PLTT, 0xf, BR_FADE_DIR_IN, 4);
        *p_seq = 7;
        break;
    case 7:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = 8;
        }
        break;
    case 8:
        if (func_02035c60()) {
            GFL_SndSEPlay(SEQ_SE_BREC_02);
            func_ov186_021fc734(wk->msgWin);
            wk->msgWin = NULL;
            GFL_BGSysLoadScr(4);
            *p_seq = 9;
        }
        break;
    case 9:
        func_ov186_021fe794(wk->param->fade, BR_FADE_TYPE_PLTT, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN, 48);
        func_ov186_021fee24(wk->param->sidebar);
        *p_seq = 10;
        break;
    case 10: {
        BOOL isEnd = TRUE;

        isEnd &= func_ov186_021fe7b0(wk->param->fade);
        if (isEnd) {
            func_ov186_021fd860(wk->ballEff[1], BR_BALL_EFFECT_RING, NULL);
            *p_seq = 11;
        }
        break;
    }
    case 11:
        func_ov186_021fe794(wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT, 1);
        *p_seq = 12;
        break;
    case 12:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = 13;
        }
        break;
    case 13:
        if (func_ov186_021fd938(wk->ballEff[1])) {
            func_ov186_021f9bf4(wk->param->procSys, BR_PROCID_MENU);
            func_ov186_021fd73c(p_seqwk, func_ov183_021db690);
        }
        break;
    }

    if (wk->msgWin != NULL) {
        func_ov186_021fc830(wk->msgWin);
    }
}


// Closes the sidebars and leaves the Battle Recorder
// pokebw2's BrStart_SeqClose
void func_ov183_021db594(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_INIT,
        SEQ_CLOSE_START,
        SEQ_WAIT,
        SEQ_FADEOUT_WAIT,
        SEQ_END,
    };
    BrStartWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_INIT:
        *p_seq = SEQ_CLOSE_START;
        break;
    case SEQ_CLOSE_START:
        func_ov186_021fee74(wk->param->sidebar);
        *p_seq = SEQ_WAIT;
        break;
    case SEQ_WAIT:
        if (wk->cnt++ > 0) {
            wk->cnt = 0;
            func_ov186_021fe794(wk->param->fade, BR_FADE_TYPE_PLTT, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT, 26);
            *p_seq = SEQ_FADEOUT_WAIT;
        }
        break;
    case SEQ_FADEOUT_WAIT: {
        BOOL isEnd = TRUE;

        isEnd &= func_ov186_021fe7b0(wk->param->fade);
        if (isEnd) {
            *p_seq = SEQ_END;
        }
        break;
    }
    case SEQ_END:
        func_ov186_021f9ba0(wk->param->procSys);
        func_ov186_021fd73c(p_seqwk, func_ov183_021db690);
        break;
    }
}

void func_ov183_021db61c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrStartWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case 0:
        func_ov186_021fe794(wk->param->fade, 2, 3, 1, 1);
        *p_seq = 1;
        break;
    case 1:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = 2;
        }
        break;
    case 2:
        func_ov186_021fee9c(wk->param->sidebar);
        func_ov186_021fee4c(wk->param->sidebar);
        func_ov186_021f9bf4(wk->param->procSys, 1);
        func_ov186_021fd73c(p_seqwk, func_ov183_021db690);
        break;
    }
}




static void func_ov183_021db690(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    func_ov186_021fd744(p_seqwk);
}


