// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_bvsave_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's battle video saving screen: it downloads the battle video of the Global Link that the record
// screen showed, lets the player choose one of the three slots for downloaded videos, asks before it overwrites one,
// saves the video and tells the Global Link that it was saved. The name is a guess: the ROM gives none, and br_core.c
// runs it as BR_PROCID_BV_SAVE

#include "types.h"
#include "app/battle_rec_ov.h"
#include "app/battle_recorder/br_btn.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_net.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "constants/sound.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "save/battle_rec.h"
#include "save/gds_profile.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/wordset.h"

// The buttons of the slot page: the three slots for downloaded videos, then the one to go back
enum {
    BTN_SLOT1,
    BTN_SLOT2,
    BTN_SLOT3,
    BTN_RETURN,
    BTN_MAX,
};

// The answers of the question before a slot is overwritten
enum {
    MSGWIN_YES,
    MSGWIN_NO,
    MSGWIN_MAX,
};

#define BR_BVSAVE_PRINT_COLOR 0x3da0

typedef struct BrBvSaveWork BrBvSaveWork;

// What makes and removes a page. Each is called once and then cleared
typedef struct {
    void (*create)(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param);
    void (*delete)(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param);
} BrBvSavePage;

struct BrBvSaveWork {
    BmpOamSys *bmpoam;
    PrintQueue *que;
    BrMsgWin *text;
    BrSeq *seq;
    // The slot button chosen. Its slot is one more, since the first slot is for the player's own video
    u32 select;
    BrBtn *btn[BTN_MAX];
    u32 unk24;
    BrMsgWin *msgWin[MSGWIN_MAX];
    // For the main and the sub screen
    BrBallEffect *ballEff[2];
    u16 heapId;
    BrBvSaveProcParam *p_param;
    u16 saveSeq;
    u16 saveSeq2;
    u32 cnt;
    u32 netCnt;
    BrBvSavePage page;
};

// Where a window is, in tiles, and its message
typedef struct {
    u8 x;
    u8 y;
    u8 w;
    u8 h;
    u32 msgID;
} BrBvSaveMsgWinData;

BOOL func_ov183_021e2578(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021e25e4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021e2640(GameProc *proc, u32 *state, void *param, void *work);
void func_ov183_021e26b8(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021e2910(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021e2ae0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021e2c2c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021e2e30(BrBvSavePage *page, BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param);
static void func_ov183_021e2e48(BrBvSavePage *page, BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param);
BrBvSavePage func_ov183_021e2e60(void);
void func_ov183_021e2e70(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param);
void func_ov183_021e2f54(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param);
BrBvSavePage func_ov183_021e2f98(void);
void func_ov183_021e2fa8(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param);
void func_ov183_021e2fe8(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param);
BrBvSavePage func_ov183_021e300c(void);
void func_ov183_021e301c(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param);
void func_ov183_021e31ac(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param);
BOOL func_ov183_021e31e8(BrBvSaveWork *p_wk, u32 x, u32 y);
BOOL func_ov183_021e3228(BrBvSaveWork *p_wk, u32 x, u32 y);
BrBallEffect *func_ov186_021fd760(ClActUnit *unit, BrRes *res, u32 display, HeapID heapId);
void func_ov094_021fd19c(BrMsgWin *p_wk, BrRes *res, const StrBuf *str);
void func_ov186_021fb964(BrRes *res, u32 bgID, HeapID heapId);

// pokebw2's BrBvSave_ProcExit

BOOL func_ov183_021e2578(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvSaveProcParam *p_param = param;
    BrBvSaveWork *p_wk;
    int i;

    p_wk = GFL_ProcInitSubsystem(proc, sizeof(BrBvSaveWork), func_ov186_021f9b9c(p_param->procSys));
    MI_CpuFill8(p_wk, 0, sizeof(BrBvSaveWork));
    p_wk->p_param = p_param;
    p_wk->heapId = func_ov186_021f9b9c(p_param->procSys);
    p_wk->bmpoam = func_02025538(p_wk->heapId, p_param->unit);
    p_wk->seq = func_ov186_021fd6d8(p_wk, func_ov183_021e26b8, p_wk->heapId);
    p_wk->que = func_0201c440(p_wk->heapId);
    for (i = 0; i < 2; i++) {
        p_wk->ballEff[i] = func_ov186_021fd760(p_param->unit, p_param->res, i, p_wk->heapId);
    }
    return TRUE;
}

static BOOL func_ov183_021e25e4(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvSaveWork *p_wk = work;
    int i;

    func_ov183_021e2e48(&p_wk->page, p_wk, p_wk->p_param);
    for (i = 0; i < 2; i++) {
        func_ov186_021fd820(p_wk->ballEff[i]);
    }
    if (p_wk->text != NULL) {
        func_ov186_021fd15c(p_wk->text, p_wk->p_param->res);
        p_wk->text = NULL;
    }
    func_0201c4c0(p_wk->que);
    func_ov186_021fd714(p_wk->seq);
    func_02025588(p_wk->bmpoam);
    func_ov186_021f9ba0(p_wk->p_param->procSys);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrBvSave_ProcMain
static BOOL func_ov183_021e2640(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvSaveWork *p_wk = work;
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

void func_ov183_021e26b8(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_INIT,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_DOWNLOAD_START,
        SEQ_DOWNLOAD_WAIT,
        SEQ_DOWNLOAD_END,
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_NEXT,
        SEQ_MSG_WAIT,
        SEQ_ERROR_FADEOUT_START,
        SEQ_ERROR_FADEOUT_WAIT,
        SEQ_ERROR_END,
    };
    BrBvSaveWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_INIT:
        p_wk->page = func_ov183_021e2f98();
        func_ov183_021e2e30(&p_wk->page, p_wk, p_wk->p_param);
        *p_seq = SEQ_FADEIN_START;
        break;
    case SEQ_FADEIN_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_DOWNLOAD_START;
        }
        break;
    case SEQ_DOWNLOAD_START: {
        BrNetRequestParam req;

        MI_CpuFill8(&req, 0, sizeof(BrNetRequestParam));
        *(u64 *)&req = p_wk->p_param->videoNumber;
        func_ov186_021ff54c(p_wk->p_param->net, BR_NET_REQUEST_3, &req);
        p_wk->cnt = 0;
        *p_seq = SEQ_DOWNLOAD_WAIT;
        break;
    }
    case SEQ_DOWNLOAD_WAIT:
        // The balls turn for at least 96 frames
        p_wk->cnt++;
        if (func_ov186_021ff648(p_wk->p_param->net) && p_wk->cnt > 96) {
            func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_NONE, NULL);
            *p_seq = SEQ_DOWNLOAD_END;
        }
        break;
    case SEQ_DOWNLOAD_END: {
        u32 msgID;

        if (!func_ov186_021ff72c(p_wk->p_param->net, &msgID)) {
            u32 value;
            BattleRecVideo *video;

            if (func_ov186_021ff698(p_wk->p_param->net, (void **)&video, &value)) {
                BOOL isSaved;
                u64 number;
                int i;

                func_0200b938((GdsProfile *)video->profile, &video->header, &video->body,
                              GameData_GetSaveControl(p_wk->p_param->gameData));
                isSaved = FALSE;
                number = func_0200b98c(func_0200b928(), BATTLE_REC_HEADER_DATA_NUMBER, 0);
                for (i = BR_RECORD_MODE_OTHER1; i <= BR_RECORD_MODE_OTHER3; i++) {
                    if (p_wk->p_param->recordInfo->videoNumber[i] == number) {
                        isSaved = TRUE;
                        break;
                    }
                }
                if (isSaved == TRUE) {
                    func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x13c);
                    *p_seq = SEQ_MSG_WAIT;
                } else {
                    *p_seq = SEQ_FADEOUT_START;
                }
            } else {
                func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x153);
                *p_seq = SEQ_MSG_WAIT;
            }
        } else {
            func_ov186_021fd170(p_wk->text, p_wk->p_param->res, msgID);
            *p_seq = SEQ_MSG_WAIT;
        }
        break;
    }
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
            *p_seq = SEQ_ERROR_FADEOUT_START;
        }
        break;
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_NEXT;
        }
        break;
    case SEQ_NEXT:
        func_ov183_021e2e48(&p_wk->page, p_wk, p_wk->p_param);
        func_ov186_021fd73c(p_seqwk, func_ov183_021e2910);
        break;
    case SEQ_ERROR_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_ERROR_FADEOUT_WAIT;
        break;
    case SEQ_ERROR_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_ERROR_END;
        }
        break;
    case SEQ_ERROR_END:
        func_ov183_021e2e48(&p_wk->page, p_wk, p_wk->p_param);
        func_ov186_021fd744(p_seqwk);
        break;
    }
}


void func_ov183_021e2910(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_INIT,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_INPUT,
        SEQ_SAVE_FADEOUT_START,
        SEQ_SAVE_FADEOUT_WAIT,
        SEQ_SAVE_NEXT,
        SEQ_CONFIRM_FADEOUT_START,
        SEQ_CONFIRM_FADEOUT_WAIT,
        SEQ_CONFIRM_NEXT,
        SEQ_RETURN_FADEOUT_START,
        SEQ_RETURN_FADEOUT_WAIT,
        SEQ_RETURN_END,
    };
    BrBvSaveWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_INIT:
        p_wk->page = func_ov183_021e300c();
        func_ov183_021e2e30(&p_wk->page, p_wk, p_wk->p_param);
        *p_seq = SEQ_FADEIN_START;
        break;
    case SEQ_FADEIN_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_INPUT;
        }
        break;
    case SEQ_INPUT: {
        u32 x;
        u32 y;

        if (func_02035d20(&x, &y)) {
            int i;

            for (i = BTN_SLOT1; i <= BTN_SLOT3; i++) {
                if (func_ov186_021fb4b4(p_wk->btn[i], x, y)) {
                    BrPoint pos;

                    pos.x = x;
                    pos.y = y;
                    func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
                    p_wk->select = i;
                    // A slot that holds a video is asked about first
                    if (p_wk->p_param->recordInfo->isValid[p_wk->select + 1]) {
                        *p_seq = SEQ_CONFIRM_FADEOUT_START;
                    } else {
                        *p_seq = SEQ_SAVE_FADEOUT_START;
                    }
                }
            }
            if (func_ov186_021fb4b4(p_wk->btn[BTN_RETURN], x, y)) {
                BrPoint pos;

                pos.x = x;
                pos.y = y;
                func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
                *p_seq = SEQ_RETURN_FADEOUT_START;
            }
        }
        break;
    }
    case SEQ_SAVE_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_SAVE_FADEOUT_WAIT;
        break;
    case SEQ_SAVE_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_SAVE_NEXT;
        }
        break;
    case SEQ_SAVE_NEXT:
        func_ov183_021e2e48(&p_wk->page, p_wk, p_wk->p_param);
        func_ov186_021fd73c(p_wk->seq, func_ov183_021e2c2c);
        break;
    case SEQ_CONFIRM_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        (*p_seq)++;
        break;
    case SEQ_CONFIRM_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            (*p_seq)++;
        }
        break;
    case SEQ_CONFIRM_NEXT:
        func_ov183_021e2e48(&p_wk->page, p_wk, p_wk->p_param);
        func_ov186_021fd73c(p_wk->seq, func_ov183_021e2ae0);
        break;
    case SEQ_RETURN_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_RETURN_FADEOUT_WAIT;
        break;
    case SEQ_RETURN_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_RETURN_END;
        }
        break;
    case SEQ_RETURN_END:
        func_ov183_021e2e48(&p_wk->page, p_wk, p_wk->p_param);
        func_ov186_021fd744(p_seqwk);
        break;
    }
}


// Asks whether to overwrite the video of the chosen slot
// pokebw2's BrBvSave_SeqConfirm
void func_ov183_021e2ae0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_INIT,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_INPUT,
        SEQ_YES_FADEOUT_START,
        SEQ_YES_FADEOUT_WAIT,
        SEQ_YES_NEXT,
        SEQ_NO_FADEOUT_START,
        SEQ_NO_FADEOUT_WAIT,
        SEQ_NO_NEXT,
    };
    BrBvSaveWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_INIT:
        p_wk->page = func_ov183_021e2e60();
        func_ov183_021e2e30(&p_wk->page, p_wk, p_wk->p_param);
        (*p_seq)++;
        break;
    case SEQ_FADEIN_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        (*p_seq)++;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            (*p_seq)++;
        }
        break;
    case SEQ_INPUT: {
        u32 x;
        u32 y;

        if (func_02035d20(&x, &y)) {
            if (func_ov183_021e31e8(p_wk, x, y)) {
                *p_seq = SEQ_YES_FADEOUT_START;
            }
            if (func_ov183_021e3228(p_wk, x, y)) {
                *p_seq = SEQ_NO_FADEOUT_START;
            }
        }
        break;
    }
    case SEQ_YES_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        (*p_seq)++;
        break;
    case SEQ_YES_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            (*p_seq)++;
        }
        break;
    case SEQ_YES_NEXT:
        func_ov183_021e2e48(&p_wk->page, p_wk, p_wk->p_param);
        func_ov186_021fd73c(p_seqwk, func_ov183_021e2c2c);
        break;
    case SEQ_NO_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        (*p_seq)++;
        break;
    case SEQ_NO_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            (*p_seq)++;
        }
        break;
    case SEQ_NO_NEXT:
        func_ov183_021e2e48(&p_wk->page, p_wk, p_wk->p_param);
        func_ov186_021fd73c(p_seqwk, func_ov183_021e2910);
        break;
    }
}

void func_ov183_021e2c2c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_SAVE_INIT,
        SEQ_SAVE_MAIN,
        SEQ_NOTIFY_START,
        SEQ_NOTIFY_WAIT,
        SEQ_MSG_WAIT,
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_END,
    };
    BrBvSaveWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEIN_START: {
        BrPoint pos;

        if (p_wk->text == NULL) {
            p_wk->text = func_ov186_021fd12c(p_wk->p_param->res, p_wk->que, p_wk->heapId);
        }
        func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x13d);
        pos.x = 0x80;
        pos.y = 0x60;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_ROT, &pos);
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        (*p_seq)++;
        break;
    }
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            (*p_seq)++;
        }
        break;
    case SEQ_SAVE_INIT:
        p_wk->cnt = 0;
        p_wk->saveSeq = 0;
        p_wk->saveSeq2 = 0;
        (*p_seq)++;
        break;
    case SEQ_SAVE_MAIN: {
        u32 ret = func_ov125_021d7540(p_wk->p_param->gameData, p_wk->select + 1, (u8)p_wk->p_param->unk24,
                                      &p_wk->saveSeq, &p_wk->saveSeq2, p_wk->heapId);

        if (ret == 2 || ret == 3) {
            p_wk->p_param->isSave = TRUE;
            p_wk->p_param->saveSlot = p_wk->select + 1;
            (*p_seq)++;
        }
        p_wk->cnt++;
        break;
    }
    case SEQ_NOTIFY_START: {
        BrNetRequestParam req;

        MI_CpuFill8(&req, 0, sizeof(BrNetRequestParam));
        *(u64 *)&req = func_0200b98c(func_0200b928(), BATTLE_REC_HEADER_DATA_NUMBER, 0);
        func_ov186_021ff54c(p_wk->p_param->net, BR_NET_REQUEST_8, &req);
        func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x13f);
        p_wk->netCnt = 0;
        p_wk->cnt++;
        (*p_seq)++;
        break;
    }
    case SEQ_NOTIFY_WAIT:
        // The balls turn for at least 96 frames, and the second message shows for at least 60
        p_wk->cnt++;
        p_wk->netCnt++;
        if (func_ov186_021ff648(p_wk->p_param->net) && p_wk->cnt > 96 && p_wk->netCnt > 60) {
            func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_NONE, NULL);
            func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x140);
            (*p_seq)++;
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
            (*p_seq)++;
        }
        break;
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        (*p_seq)++;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            (*p_seq)++;
        }
        break;
    case SEQ_END:
        if (p_wk->text != NULL) {
            func_ov186_021fd15c(p_wk->text, p_wk->p_param->res);
            p_wk->text = NULL;
        }
        func_ov186_021fd744(p_seqwk);
        break;
    }
}


// pokebw2's BrBvSave_CreatePage
void func_ov183_021e2e30(BrBvSavePage *page, BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param) {
    if (page->create != NULL) {
        page->create(p_wk, p_param);
        page->create = NULL;
    }
}

// pokebw2's BrBvSave_DeletePage
static void func_ov183_021e2e48(BrBvSavePage *page, BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param) {
    if (page->delete != NULL) {
        page->delete(p_wk, p_param);
        page->delete = NULL;
    }
}

// The text box with the balls turning, while the video downloads
// pokebw2's sBrBvSaveDownloadPage
static const BrBvSavePage data_ov183_021e3910 = { func_ov183_021e2fa8, func_ov183_021e2fe8 };

// The question before a slot is overwritten, which names the player of the video there, with its two answers
// pokebw2's sBrBvSaveConfirmPage
static const BrBvSavePage data_ov183_021e3920 = { func_ov183_021e2e70, func_ov183_021e2f54 };

// The buttons of the three slots, which name the players of the videos they hold, and the one to go back
// pokebw2's sBrBvSaveSelectPage
static const BrBvSavePage data_ov183_021e3918 = { func_ov183_021e301c, func_ov183_021e31ac };

// pokebw2's sBrBvSaveConfirmWins
static const BrBvSaveMsgWinData data_ov183_021e3928[MSGWIN_MAX] = {
    [MSGWIN_YES] = { 4, 6, 9, 2, 0x9f },
    [MSGWIN_NO] = { 18, 6, 9, 2, 0xa0 },
};

// pokebw2's BR_BVSAVE_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e3938 = {
    func_ov183_021e2578,
    func_ov183_021e2640,
    func_ov183_021e25e4,
};

// pokebw2's BrBvSave_GetConfirmPage
BrBvSavePage func_ov183_021e2e60(void) {
    return data_ov183_021e3920;
}

void func_ov183_021e2e70(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param) {
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
    // The downloaded videos' slots come after the player's own
    func_0201ee0c(wordSet, 0, (p_param->recordInfo->name + 1)[p_wk->select], (p_param->recordInfo->sex + 1)[p_wk->select], 1,
                  2);
    GFL_WordSetFormatStrbuf(wordSet, str, src);
    func_ov186_021fd19c(p_wk->text, p_param->res, str);
    GFL_StrBufFree(str);
    GFL_StrBufFree(src);

    func_ov186_021fb964(p_param->res, 0x1e, p_wk->heapId);
    font = func_ov186_021fc20c(p_param->res);
    msg = func_ov186_021fc214(p_param->res);
    for (i = 0; i < MSGWIN_MAX; i++) {
        const BrBvSaveMsgWinData *data = &data_ov183_021e3928[i];

        p_wk->msgWin[i] = func_ov186_021fc6a4(4, data->x, data->y, data->w, data->h, 14, p_wk->que, p_wk->heapId);
        func_ov186_021fc754(p_wk->msgWin[i], msg, data->msgID, font, BR_BVSAVE_PRINT_COLOR);
    }
}




void func_ov183_021e2f54(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param) {
    int i;

    for (i = 0; i < MSGWIN_MAX; i++) {
        if (p_wk->msgWin[i] != NULL) {
            func_ov186_021fc734(p_wk->msgWin[i]);
            p_wk->msgWin[i] = NULL;
        }
    }
    GFL_BGSysLoadScr(4);
    func_ov186_021fbda4(p_param->res, 0x1e);
    if (p_wk->text != NULL) {
        func_ov186_021fd15c(p_wk->text, p_param->res);
        p_wk->text = NULL;
    }
}

// pokebw2's BrBvSave_GetDownloadPage
BrBvSavePage func_ov183_021e2f98(void) {
    return data_ov183_021e3910;
}

void func_ov183_021e2fa8(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param) {
    BrPoint pos;

    if (p_wk->text == NULL) {
        p_wk->text = func_ov186_021fd12c(p_wk->p_param->res, p_wk->que, p_wk->heapId);
    }
    func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x14b);
    pos.x = 0x80;
    pos.y = 0x60;
    func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_ROT, &pos);
}

void func_ov183_021e2fe8(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param) {
    func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_NONE, NULL);
    if (p_wk->text != NULL) {
        func_ov186_021fd15c(p_wk->text, p_wk->p_param->res);
        p_wk->text = NULL;
    }
}

// pokebw2's BrBvSave_GetSelectPage
BrBvSavePage func_ov183_021e300c(void) {
    return data_ov183_021e3918;
}

void func_ov183_021e301c(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param) {
    Font *font;
    MsgData *msg;
    WordSet *wordSet;
    StrBuf *str;
    StrBuf *src;
    ClActorSetup setup;
    BrResObjData obj;
    GdsProfile *profile;
    StrBuf *name;
    int i;

    font = func_ov186_021fc20c(p_param->res);
    msg = func_ov186_021fc214(p_param->res);
    wordSet = func_ov186_021fc21c(p_param->res);
    str = GFL_StrBufCreate(0x80, p_wk->heapId);
    src = GFL_StrBufCreate(0x80, p_wk->heapId);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 48;
    setup.y = 40;
    setup.sequence = 11;
    setup.priority = 1;
    func_ov186_021fbe54(p_param->res, BR_RES_OBJ_BROWSE_BTN_S, p_wk->heapId);
    func_ov186_021fc124(p_param->res, BR_RES_OBJ_BROWSE_BTN_S, &obj);
    for (i = BTN_SLOT1; i < BTN_RETURN; i++) {
        // The slot of the button
        int slot = BR_RECORD_MODE_OTHER1 + i;

        if (p_param->recordInfo->isValid[slot]) {
            GFL_MsgDataLoadStrbuf(msg, 0x1a, src);
            func_0201ee0c(wordSet, 0, p_param->recordInfo->name[slot], p_param->recordInfo->sex[slot], 1, 2);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
        } else {
            GFL_MsgDataLoadStrbuf(msg, 9, str);
        }
        p_wk->btn[i] = func_ov186_021fb3bc(&setup, str, 160, CLACT_SURFACE_SUB, p_param->unit, p_wk->bmpoam, font, &obj,
                                           p_wk->heapId);
        setup.y += 40;
    }
    setup.sequence = 1;
    p_wk->btn[BTN_RETURN] = func_ov186_021fb378(&setup, 4, 160, CLACT_SURFACE_SUB, p_param->unit, p_wk->bmpoam, font, msg,
                                                &obj, p_wk->heapId);

    // The text asks where to save the video of its player
    profile = getVSPlayerAllocation();
    name = func_0200d6d0(profile, HEAPID_TAIL(p_wk->heapId));
    if (p_wk->text == NULL) {
        p_wk->text = func_ov186_021fd12c(p_param->res, p_wk->que, p_wk->heapId);
    }
    GFL_MsgDataLoadStrbuf(msg, 0x5c, src);
    func_0201ee0c(wordSet, 0, name, func_0200d6ec(profile), 1, 2);
    GFL_WordSetFormatStrbuf(wordSet, str, src);
    func_ov186_021fd19c(p_wk->text, p_param->res, str);
    GFL_StrBufFree(name);
    GFL_StrBufFree(src);
    GFL_StrBufFree(str);
}


void func_ov183_021e31ac(BrBvSaveWork *p_wk, BrBvSaveProcParam *p_param) {
    int i;

    if (p_wk->text != NULL) {
        func_ov186_021fd15c(p_wk->text, p_param->res);
        p_wk->text = NULL;
    }
    for (i = 0; i < BTN_MAX; i++) {
        if (p_wk->btn[i] != NULL) {
            func_ov186_021fb494(p_wk->btn[i]);
            p_wk->btn[i] = NULL;
        }
    }
    func_ov186_021fc0c8(p_param->res, BR_RES_OBJ_BROWSE_BTN_S);
}

// The answers of the question
// pokebw2's BrBvSave_GetTrgYes
BOOL func_ov183_021e31e8(BrBvSaveWork *p_wk, u32 x, u32 y) {
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

// pokebw2's BrBvSave_GetTrgNo
BOOL func_ov183_021e3228(BrBvSaveWork *p_wk, u32 x, u32 y) {
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
