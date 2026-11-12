// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_bvsend_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's battle video upload screen: it loads the player's own battle video, sends it to the Global
// Link and shows the number the video was given, or the error. The name is a guess: the ROM gives none, and br_core.c
// runs it as BR_PROCID_BV_SEND

#include "types.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_net.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "constants/sound.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "save/battle_rec.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/wordset.h"

#define HEAPID_BATTLE_RECORDER_SYS 0x59

typedef struct {
    PrintQueue *que;
    BrSeq *seq;
    u16 heapId;
    BrMsgWin *text;
    // For the main and the sub screen
    BrBallEffect *ballEff[2];
    BrBvSendProcParam *p_param;
    u32 cnt;
} BrBvSendWork;

static BOOL func_ov183_021e1c80(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021e1d10(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021e1d48(GameProc *proc, u32 *state, void *param, void *work);
void func_ov183_021e1da8(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov183_021e1df0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov183_021e1e3c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);

// pokebw2's BR_BVSEND_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e38e8 = {
    func_ov183_021e1c80,
    func_ov183_021e1d48,
    func_ov183_021e1d10,
};

// pokebw2's BrBvSend_ProcInit
static BOOL func_ov183_021e1c80(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvSendProcParam *p_param = param;
    BrBvSendWork *p_wk;
    int i;

    p_wk = GFL_ProcInitSubsystem(proc, sizeof(BrBvSendWork), func_ov186_021f9b9c(p_param->procSys));
    MI_CpuFill8(p_wk, 0, sizeof(BrBvSendWork));
    p_wk->p_param = p_param;
    p_wk->heapId = func_ov186_021f9b9c(p_param->procSys);

    p_wk->seq = func_ov186_021fd6d8(p_wk, func_ov183_021e1da8, p_wk->heapId);
    p_wk->que = func_0201c440(p_wk->heapId);
    p_wk->text = func_ov186_021fd12c(p_param->res, p_wk->que, p_wk->heapId);
    func_ov186_021fd170(p_wk->text, p_param->res, 0x6f);
    for (i = 0; i < 2; i++) {
        p_wk->ballEff[i] = func_ov186_021fd760(p_param->unit, p_param->res, i, p_wk->heapId);
    }
    {
        BrPoint pos = { 0x80, 0x60 };

        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_ROT, &pos);
    }
    return TRUE;
}

// pokebw2's BrBvSend_ProcExit
static BOOL func_ov183_021e1d10(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvSendProcParam *p_param = param;
    BrBvSendWork *p_wk = work;
    int i;

    for (i = 0; i < 2; i++) {
        func_ov186_021fd820(p_wk->ballEff[i]);
    }
    func_ov186_021fd15c(p_wk->text, p_param->res);
    func_0201c4c0(p_wk->que);
    func_ov186_021fd714(p_wk->seq);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrBvSend_ProcMain
static BOOL func_ov183_021e1d48(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvSendWork *p_wk = work;
    int i;

    if (!GameData_CheckEventsPaused(p_wk->p_param->gameData) && func_ov186_021ff88c(p_wk->p_param->net) == 2) {
        func_ov186_021f9c6c(p_wk->p_param->procSys);
        return TRUE;
    }

    func_ov186_021fd71c(p_wk->seq);
    if (func_ov186_021fd72c(p_wk->seq)) {
        return TRUE;
    }

    for (i = 0; i < 2; i++) {
        func_ov186_021fd854(p_wk->ballEff[i]);
    }
    func_0201c4e4(p_wk->que);
    func_ov186_021fd1bc(p_wk->text);
    return FALSE;
}

// pokebw2's BrBvSend_SeqFadeIn
void func_ov183_021e1da8(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrBvSendWork *p_wk = p_wk_adrs;

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
        func_ov186_021fd73c(p_seqwk, func_ov183_021e1e3c);
        break;
    }
}

// pokebw2's BrBvSend_SeqFadeOut
static void func_ov183_021e1df0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_END,
    };
    BrBvSendWork *p_wk = p_wk_adrs;

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

// pokebw2's BrBvSend_SeqMain
static void func_ov183_021e1e3c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_LOAD,
        SEQ_UPLOAD_START,
        SEQ_UPLOAD_WAIT,
        SEQ_END,
        SEQ_MSG_WAIT,
    };
    BrBvSendWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_LOAD: {
        u32 result;

        func_0200b570(GameData_GetSaveControl(p_wk->p_param->gameData), HEAPID_BATTLE_RECORDER_SYS, &result, 0);
        if (result == 1) {
            *p_seq = SEQ_UPLOAD_START;
        } else {
            func_ov186_021fd73c(p_seqwk, func_ov183_021e1df0);
        }
        break;
    }
    case SEQ_UPLOAD_START:
        p_wk->cnt = 0;
        func_ov186_021ff54c(p_wk->p_param->net, BR_NET_REQUEST_2, NULL);
        *p_seq = SEQ_UPLOAD_WAIT;
        break;
    case SEQ_UPLOAD_WAIT:
        // The balls turn for at least 96 frames
        p_wk->cnt++;
        if (func_ov186_021ff648(p_wk->p_param->net)) {
            u64 number = 0;

            if (p_wk->cnt >= 96) {
                u32 msgID;
                u32 ret = func_ov186_021ff72c(p_wk->p_param->net, &msgID);

                if (ret == 0) {
                    if (func_ov186_021ff67c(p_wk->p_param->net, &number)) {
                        MsgData *msg = func_ov186_021fc214(p_wk->p_param->res);
                        WordSet *wordSet = func_ov186_021fc21c(p_wk->p_param->res);
                        StrBuf *src = GFL_MsgDataLoadStrbufNew(msg, 0x14e);
                        StrBuf *str = GFL_StrBufCreate(0x80, HEAPID_TAIL(HEAPID_BATTLE_RECORDER_SYS));
                        // The video's number, in blocks of five digits
                        u64 high = number / 100000;

                        WordSetNumber(wordSet, 2, number % 100000, 5, 2, TRUE);
                        WordSetNumber(wordSet, 1, high % 100000, 5, 2, TRUE);
                        WordSetNumber(wordSet, 0, high / 100000, 2, 2, TRUE);
                        GFL_WordSetFormatStrbuf(wordSet, str, src);
                        func_ov186_021fd19c(p_wk->text, p_wk->p_param->res, str);
                        GFL_StrBufFree(str);
                        GFL_StrBufFree(src);
                    }
                } else if (ret == 3) {
                    func_ov186_021f9c6c(p_wk->p_param->procSys);
                    func_ov186_021fd744(p_seqwk);
                    return;
                } else {
                    func_ov186_021fd170(p_wk->text, p_wk->p_param->res, msgID);
                }
                func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_NONE, NULL);
                *p_seq = SEQ_MSG_WAIT;
            }
        }
        break;
    case SEQ_END:
        func_ov186_021fd73c(p_seqwk, func_ov183_021e1df0);
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
    }
}

