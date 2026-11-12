// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_record_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's battle video screen: it shows a battle video's summary (the player, the battle mode, the
// video's number and the Pokémon) or its player's profile, with the buttons to play it, to go back and, for a video of
// the Global Link, to save it. It downloads a video found by its number first, and downloads the whole video when one
// of the Global Link is played. The name is a guess: the ROM gives none, and br_core.c runs it as BR_PROCID_RECORD

#include "types.h"
#include "app/battle_recorder/br_record_proc.h"
#include "app/battle_rec_ov.h"
#include "app/battle_recorder/br_btn.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_net.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "save/battle_rec.h"
#include "save/gds_profile.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/pms.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "gfl/proc.h"
#include "pml/poke_party.h"

// The buttons on the sub screen
enum {
    BTN_RETURN,
    BTN_SAVE,
    BTN_MAX,
};

// The windows of the summary
enum {
    MSGWIN_NAME,
    MSGWIN_MODE,
    MSGWIN_NUMBER,
    MSGWIN_COUNTER,
    MSGWIN_COUNTER_UNIT,
    MSGWIN_MAX,
};

// The windows on the sub screen: the title, or the two answers of the question before an unwatched video plays
#define BR_RECORD_TITLE_MAX 2

#define BR_RECORD_PRINT_COLOR 0x3da0

typedef struct {
    BmpOamSys *bmpoam;
    PrintQueue *que;
    BrSeq *seq;
    // For the main and the sub screen
    BrBallEffect *ballEff[2];
    u32 unk14;
    BrMsgWin *text;
    BrBtn *btn[BTN_MAX];
    u32 unk24;
    BrMsgWin *title[BR_RECORD_TITLE_MAX];
    BrMsgWin *msgWin[MSGWIN_MAX];
    BrProfile *profileWin;
    u32 iconPlt;
    u32 iconCell;
    u32 iconChr[BATTLE_REC_HEADER_PKM_MAX];
    ClActor *icon[BATTLE_REC_HEADER_PKM_MAX];
    // Whether the main screen shows the player's profile instead of the summary
    BOOL isProfile;
    HeapID heapId;
    u16 saveSeq;
    u16 saveSeq2;
    u32 cnt;
    GdsProfile *profile;
    BattleRecHeader *header;
    // Whether the video was watched to its end, without which a saved video has to be marked before it plays
    BOOL isSecure;
    BOOL canSave;
    BrRecordProcParam *p_param;
    BOOL isInfoCreated;
    BOOL isButtonsCreated;
    BOOL isProfileCreated;
    BOOL isTextCreated;
    BOOL isConfirmCreated;
} BrRecordWork;

// Where a window is, in tiles, and its message
typedef struct {
    u8 x;
    u8 y;
    u8 w;
    u8 h;
    u32 msgID;
} BrRecordMsgWinData;

BOOL func_ov183_021db940(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021dbb3c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021dbbc4(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov183_021dbc68(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov183_021dbca8(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov183_021dbcf4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021dbd40(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov183_021dbd7c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021dbeb0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021dbfb0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021dc228(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021dc4fc(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021dc638(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);

void func_ov186_021fb964(BrRes *p_wk, u32 bgID, HeapID heapId);

static void func_ov183_021dc8ec(BrRecordWork *p_wk, BrRecordProcParam *p_param);
void func_ov183_021dcbd8(BrRecordWork *p_wk, ClActUnit *unit, BattleRecHeader *header, HeapID heapId);
static void func_ov183_021dcdd8(BrRecordWork *p_wk, BrRecordProcParam *p_param);
static void func_ov183_021dce28(BrRecordWork *p_wk, BrRecordProcParam *p_param);
static void func_ov183_021dce48(BrRecordWork *p_wk, BrRecordProcParam *p_param);
void func_ov183_021dcee4(BrRecordWork *p_wk, BrRecordProcParam *p_param);
static void func_ov183_021dd054(BrRecordWork *p_wk, BrRecordProcParam *p_param);
static void func_ov183_021dd0ac(BrRecordWork *p_wk, BrRecordProcParam *p_param);
static void func_ov183_021dd0f4(BrRecordWork *p_wk, BrRecordProcParam *p_param);
void func_ov183_021dd128(BrRecordWork *p_wk, BrRecordProcParam *p_param);
static void func_ov183_021dd1d4(BrRecordWork *p_wk, BrRecordProcParam *p_param);
BOOL func_ov183_021dd228(BattleRecHeader *header);
BOOL func_ov183_021dd25c(BattleRecHeader *header);
static BOOL func_ov183_021dd27c(BrRecordWork *p_wk, u32 x, u32 y);
static BOOL func_ov183_021dd2bc(BrRecordWork *p_wk, u32 x, u32 y);
BOOL func_ov183_021dd2fc(BrRecordWork *p_wk, u32 x, u32 y);
BOOL func_ov183_021dd33c(BrRecordWork *p_wk, u32 x, u32 y);

// pokebw2's BR_RECORD_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e329c = {
    func_ov183_021db940,
    func_ov183_021dbbc4,
    func_ov183_021dbb3c,
};

// The answers of the question before an unwatched video plays. The last two entries are not read: the third is where
// func_ov183_021dcee4 puts the title
// pokebw2's sBrRecordConfirmWins
static const BrRecordMsgWinData data_ov183_021e32a8[] = {
#include "asset/br_record_proc_data_ov183_021e32a8.inc"
};

// pokebw2's sBrRecordInfoWins
static const BrRecordMsgWinData data_ov183_021e32c8[MSGWIN_MAX] = {
#include "asset/br_record_proc_data_ov183_021e32c8.inc"
};

// Where the Pokémon icons go: in two rows of six, or in four groups of three
static const struct {
    s16 x;
    s16 y;
// pokebw2's sBrRecordIconPos
} data_ov183_021e32f0[2][BATTLE_REC_HEADER_PKM_MAX] = {
#include "asset/br_record_proc_data_ov183_021e32f0.inc"
};

// pokebw2's BrRecord_ProcExit

// pokebw2's BrRecord_ProcInit
BOOL func_ov183_021db940(GameProc *proc, u32 *state, void *param, void *work) {
    BrRecordProcParam *p_param = param;
    BrRecordWork *p_wk;
    int i;

    p_wk = GFL_ProcInitSubsystem(proc, sizeof(BrRecordWork), func_ov186_021f9b9c(p_param->procSys));
    MI_CpuFill8(p_wk, 0, sizeof(BrRecordWork));
    p_wk->heapId = func_ov186_021f9b9c(p_param->procSys);
    p_wk->isProfile = FALSE;
    p_wk->p_param = p_param;

    p_wk->bmpoam = func_02025538(p_wk->heapId, p_param->unit);
    p_wk->que = func_0201c440(p_wk->heapId);
    p_wk->seq = func_ov186_021fd6d8(p_wk, NULL, p_wk->heapId);
    for (i = 0; i < 2; i++) {
        p_wk->ballEff[i] = func_ov186_021fd760(p_param->unit, p_param->res, i, p_wk->heapId);
    }

    switch (p_wk->p_param->mode) {
    case BR_RECORD_MODE_BROWSE: {
        BrOutlineData *outline = p_wk->p_param->outline;

        p_wk->header = &outline->data[outline->select].header;
        p_wk->profile = &p_wk->p_param->outline->data[p_wk->p_param->outline->select].profile;
        p_wk->canSave = TRUE;
        if (*p_wk->p_param->unk3C && p_wk->p_param->isRecovery) {
            p_wk->p_param->outline->isSecure[p_wk->p_param->outline->select] = TRUE;
        }
        p_wk->isSecure = p_wk->p_param->outline->isSecure[p_wk->p_param->outline->select];
        break;
    }
    case BR_RECORD_MODE_MINE:
        p_wk->header = func_0200b928();
        p_wk->profile = getVSPlayerAllocation();
        p_wk->isSecure = TRUE;
        p_wk->canSave = FALSE;
        break;
    case BR_RECORD_MODE_OTHER1:
    case BR_RECORD_MODE_OTHER2:
    case BR_RECORD_MODE_OTHER3:
        p_wk->header = func_0200b928();
        p_wk->profile = getVSPlayerAllocation();
        p_wk->isSecure = func_0200b98c(p_wk->header, BATTLE_REC_HEADER_UNK2A, 0);
        p_wk->canSave = FALSE;
        break;
    case BR_RECORD_MODE_CODEIN:
        if (!p_wk->p_param->isRecovery) {
            // The video is downloaded first
            p_wk->header = NULL;
            p_wk->profile = NULL;
        } else {
            p_wk->header = func_0200b928();
            p_wk->profile = getVSPlayerAllocation();
            p_wk->isSecure = *p_wk->p_param->unk3C;
        }
        p_wk->canSave = TRUE;
        break;
    }

    if (p_wk->p_param->mode == BR_RECORD_MODE_CODEIN) {
        if (!p_wk->p_param->isRecovery) {
            func_ov186_021fd73c(p_wk->seq, func_ov183_021dbfb0);
        } else {
            func_ov186_021fd73c(p_wk->seq, func_ov183_021dbc68);
        }
    } else if ((p_wk->p_param->mode == BR_RECORD_MODE_OTHER1 || p_wk->p_param->mode == BR_RECORD_MODE_OTHER2
                || p_wk->p_param->mode == BR_RECORD_MODE_OTHER3)
               && p_wk->p_param->isRecovery && *p_wk->p_param->unk3C && !p_wk->isSecure) {
        // A saved video that has just been watched to its end for the first time is marked
        func_ov186_021fd73c(p_wk->seq, func_ov183_021dc4fc);
    } else {
        func_ov186_021fd73c(p_wk->seq, func_ov183_021dbc68);
    }
    return TRUE;
}

static BOOL func_ov183_021dbb3c(GameProc *proc, u32 *state, void *param, void *work) {
    BrRecordProcParam *p_param = param;
    BrRecordWork *p_wk = work;
    int i;

    func_ov183_021dce28(p_wk, p_param);
    func_ov183_021dce48(p_wk, p_param);
    func_ov183_021dd054(p_wk, p_param);
    func_ov183_021dd0f4(p_wk, p_param);
    func_ov183_021dd1d4(p_wk, p_param);
    p_param->unk4 = p_wk->isSecure;

    GFL_BGSysLoadScr(4);
    GFL_BGSysLoadScr(1);
    if (p_wk->text != NULL) {
        func_ov186_021fd15c(p_wk->text, p_wk->p_param->res);
        p_wk->text = NULL;
    }
    for (i = 0; i < 2; i++) {
        func_ov186_021fd820(p_wk->ballEff[i]);
    }
    func_ov186_021fd714(p_wk->seq);
    func_0201c4c0(p_wk->que);
    func_02025588(p_wk->bmpoam);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrRecord_ProcMain
static BOOL func_ov183_021dbbc4(GameProc *proc, u32 *state, void *param, void *work) {
    BrRecordWork *p_wk = work;
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
    for (i = 0; i < BR_RECORD_TITLE_MAX; i++) {
        if (p_wk->title[i] != NULL) {
            func_ov186_021fc830(p_wk->title[i]);
        }
    }
    for (i = 0; i < MSGWIN_MAX; i++) {
        if (p_wk->msgWin[i] != NULL) {
            func_ov186_021fc830(p_wk->msgWin[i]);
        }
    }
    if (p_wk->profileWin != NULL) {
        func_ov186_021fd6b0(p_wk->profileWin);
    }
    if (p_wk->text != NULL) {
        func_ov186_021fd1bc(p_wk->text);
    }
    return FALSE;
}

// Makes the screen and fades it in
// pokebw2's BrRecord_SeqStart
static void func_ov183_021dbc68(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrRecordWork *p_wk = p_wk_adrs;

    func_ov183_021dcee4(p_wk, p_wk->p_param);
    if (p_wk->isProfile) {
        func_ov183_021dcdd8(p_wk, p_wk->p_param);
    } else {
        func_ov183_021dc8ec(p_wk, p_wk->p_param);
    }
    func_ov186_021fd73c(p_seqwk, func_ov183_021dbca8);
}

// pokebw2's BrRecord_SeqFadeIn
static void func_ov183_021dbca8(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrRecordWork *p_wk = p_wk_adrs;

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
        func_ov186_021fd73c(p_seqwk, func_ov183_021dbd7c);
        break;
    }
}

// pokebw2's BrRecord_SeqFadeOut
static void func_ov183_021dbcf4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_NEXT,
    };
    BrRecordWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
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
        func_ov186_021fd73c(p_seqwk, func_ov183_021dbd40);
        break;
    }
}

void func_ov183_021dbd40(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrRecordWork *p_wk = p_wk_adrs;

    if (p_wk->isProfile) {
        func_ov183_021dce28(p_wk, p_wk->p_param);
    } else {
        func_ov183_021dce48(p_wk, p_wk->p_param);
    }
    func_ov183_021dd054(p_wk, p_wk->p_param);
    func_ov186_021fd744(p_seqwk);
}




// Waits for the player to touch a button
// pokebw2's BrRecord_SeqMain
static void func_ov183_021dbd7c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrRecordWork *p_wk = p_wk_adrs;
    u32 x;
    u32 y;

    if (func_02035d20(&x, &y)) {
        if (func_ov183_021dd27c(p_wk, x, y)) {
            p_wk->isProfile ^= 1;
            func_ov186_021fd73c(p_seqwk, func_ov183_021dbeb0);
        }
        if (func_ov186_021fb4b4(p_wk->btn[BTN_RETURN], x, y)) {
            BrPoint pos;

            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
            p_wk->p_param->result = 0;
            func_ov186_021f9ba0(p_wk->p_param->procSys);
            func_ov186_021fd73c(p_seqwk, func_ov183_021dbcf4);
        }
        if (p_wk->btn[BTN_SAVE] != NULL && func_ov186_021fb4b4(p_wk->btn[BTN_SAVE], x, y)) {
            BrPoint pos;

            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
            func_ov186_021f9bf4(p_wk->p_param->procSys, BR_PROCID_BV_SAVE);
            p_wk->p_param->videoNumber = func_0200b98c(p_wk->header, BATTLE_REC_HEADER_DATA_NUMBER, 0);
            func_ov186_021fd73c(p_seqwk, func_ov183_021dbcf4);
        }
        if (func_ov183_021dd2bc(p_wk, x, y)) {
            if (p_wk->p_param->mode == BR_RECORD_MODE_BROWSE || p_wk->p_param->mode == BR_RECORD_MODE_CODEIN) {
                func_ov186_021fd73c(p_seqwk, func_ov183_021dc228);
            } else if (!p_wk->isSecure) {
                func_ov186_021fd73c(p_seqwk, func_ov183_021dc638);
            } else {
                p_wk->p_param->result = 1;
                func_ov186_021f9c50(p_wk->p_param->procSys);
                func_ov186_021fd73c(p_seqwk, func_ov183_021dbcf4);
            }
        }
    }
}

void func_ov183_021dbeb0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrRecordWork *p_wk = p_wk_adrs;
    void *msgData;
    void *wordSet;

    switch (*p_seq) {
    case 0:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_MAIN, BR_FADE_DIR_OUT);
        (*p_seq)++;
        break;
    case 1:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            (*p_seq)++;
        }
        break;
    case 2:
        msgData = func_ov186_021fc20c(p_wk->p_param->res);
        wordSet = func_ov186_021fc214(p_wk->p_param->res);
        if (p_wk->isProfile) {
            func_ov183_021dce48(p_wk, p_wk->p_param);
            func_ov186_021fc754(p_wk->title[0], wordSet, 0x72, msgData, BR_RECORD_PRINT_COLOR);
            func_ov183_021dcdd8(p_wk, p_wk->p_param);
        } else {
            func_ov183_021dce28(p_wk, p_wk->p_param);
            func_ov186_021fc754(p_wk->title[0], wordSet, 0x71, msgData, BR_RECORD_PRINT_COLOR);
            func_ov183_021dc8ec(p_wk, p_wk->p_param);
        }
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_MAIN, BR_FADE_DIR_IN);
        (*p_seq)++;
        break;
    case 3:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            func_ov186_021fd73c(p_seqwk, func_ov183_021dbd7c);
        }
        break;
    }
}

// Downloads the video found by its number
// pokebw2's BrRecord_SeqDownloadOutline
void func_ov183_021dbfb0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_INIT,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_DOWNLOAD_START,
        SEQ_DOWNLOAD_WAIT,
        SEQ_DOWNLOAD_END,
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_INFO_FADEIN_START,
        SEQ_INFO_FADEIN_WAIT,
        SEQ_NEXT,
        SEQ_MSG_WAIT,
        SEQ_ERROR_FADEOUT_START,
        SEQ_ERROR_FADEOUT_WAIT,
        SEQ_ERROR_END,
    };
    BrRecordWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_INIT:
        func_ov183_021dd0ac(p_wk, p_wk->p_param);
        func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x14b);
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
        req.dataNumber = p_wk->p_param->videoNumber;
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
            BattleRecVideo *video;
            u32 value;

            if (func_ov186_021ff698(p_wk->p_param->net, (void **)&video, &value)) {
                p_wk->profile = (GdsProfile *)video->profile;
                p_wk->header = &video->header;
                *p_seq = SEQ_FADEOUT_START;
            } else {
                func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x114);
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
            func_ov186_021f9ba0(p_wk->p_param->procSys);
            *p_seq = SEQ_ERROR_FADEOUT_START;
        }
        break;
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            func_ov183_021dd0f4(p_wk, p_wk->p_param);
            func_ov183_021dcee4(p_wk, p_wk->p_param);
            func_ov183_021dc8ec(p_wk, p_wk->p_param);
            *p_seq = SEQ_INFO_FADEIN_START;
        }
        break;
    case SEQ_INFO_FADEIN_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_INFO_FADEIN_WAIT;
        break;
    case SEQ_INFO_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_NEXT;
        }
        break;
    case SEQ_NEXT:
        func_ov186_021fd73c(p_seqwk, func_ov183_021dbd7c);
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
        func_ov183_021dd0f4(p_wk, p_wk->p_param);
        func_ov186_021fd744(p_seqwk);
        break;
    }
}

// Downloads the whole video of the Global Link to play it
// pokebw2's BrRecord_SeqDownloadPlay
void func_ov183_021dc228(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_CHANGE,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_DOWNLOAD_START,
        SEQ_DOWNLOAD_WAIT,
        SEQ_DOWNLOAD_END,
        SEQ_PLAY_FADEOUT_START,
        SEQ_PLAY_FADEOUT_WAIT,
        SEQ_PLAY_END,
        SEQ_MSG_WAIT,
        SEQ_ERROR_FADEOUT_START,
        SEQ_ERROR_FADEOUT_WAIT,
        SEQ_ERROR_END,
    };
    BrRecordWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_CHANGE;
        }
        break;
    case SEQ_CHANGE:
        if (p_wk->isProfile) {
            func_ov183_021dce28(p_wk, p_wk->p_param);
        } else {
            func_ov183_021dce48(p_wk, p_wk->p_param);
        }
        func_ov183_021dd054(p_wk, p_wk->p_param);
        func_ov183_021dd0ac(p_wk, p_wk->p_param);
        func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x14b);
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
        req.dataNumber = func_0200b98c(p_wk->header, BATTLE_REC_HEADER_DATA_NUMBER, 0);
        func_ov186_021ff54c(p_wk->p_param->net, BR_NET_REQUEST_3, &req);
        p_wk->cnt = 0;
        *p_seq = SEQ_DOWNLOAD_WAIT;
        break;
    }
    case SEQ_DOWNLOAD_WAIT:
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
                func_0200b938((GdsProfile *)video->profile, &video->header, &video->body,
                              GameData_GetSaveControl(p_wk->p_param->gameData));
                *(u64 *)p_wk->p_param->video = (s32)value;
                p_wk->p_param->result = 1;
                func_ov186_021f9c50(p_wk->p_param->procSys);
                *p_seq = SEQ_PLAY_FADEOUT_START;
            } else {
                func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x114);
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
    case SEQ_PLAY_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_PLAY_FADEOUT_WAIT;
        break;
    case SEQ_PLAY_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_PLAY_END;
        }
        break;
    case SEQ_PLAY_END:
        if (!GFL_SndIsPlaying(SEQ_SE_BREC_04)) {
            func_ov183_021dd0f4(p_wk, p_wk->p_param);
            func_ov186_021fd744(p_wk->seq);
        }
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
        func_ov183_021dd0f4(p_wk, p_wk->p_param);
        func_ov186_021fd73c(p_seqwk, func_ov183_021dbc68);
        break;
    }
}

// Marks a saved video that was not watched to its end, then saves it
// pokebw2's BrRecord_SeqSecureSave
void func_ov183_021dc4fc(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_WAIT,
        SEQ_SAVE_INIT,
        SEQ_SAVE_MAIN,
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_END,
    };
    BrRecordWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEIN_START:
        func_ov183_021dd0ac(p_wk, p_wk->p_param);
        func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x152);
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_WAIT;
        }
        break;
    case SEQ_WAIT:
        *p_seq = SEQ_SAVE_INIT;
        break;
    case SEQ_SAVE_INIT:
        p_wk->saveSeq = 0;
        p_wk->saveSeq2 = 0;
        *p_seq = SEQ_SAVE_MAIN;
        break;
    case SEQ_SAVE_MAIN:
        GFL_ASSERT(p_wk->p_param->mode <= BR_RECODE_PROC_OTHER_02);
        if (func_ov125_021d7630(p_wk->p_param->gameData, p_wk->p_param->mode, &p_wk->saveSeq, &p_wk->saveSeq2,
                                p_wk->heapId)
            == 2) {
            func_0200b490();
            p_wk->isSecure = func_0200b98c(p_wk->header, BATTLE_REC_HEADER_UNK2A, 0);
            *p_seq = SEQ_FADEOUT_START;
        }
        break;
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
        func_ov183_021dd0f4(p_wk, p_wk->p_param);
        func_ov186_021fd73c(p_seqwk, func_ov183_021dbc68);
        break;
    }
}

void func_ov183_021dc638(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_CHANGE,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_WAIT,
        SEQ_INPUT,
        SEQ_YES_FADEOUT_START,
        SEQ_YES_FADEOUT_WAIT,
        SEQ_YES_END,
        SEQ_NO_FADEOUT_START,
        SEQ_NO_FADEOUT_WAIT,
        SEQ_NO_END,
        SEQ_SAVE_INIT,
        SEQ_SAVE_FADEIN_START,
        SEQ_SAVE_FADEIN_WAIT,
        SEQ_SAVE_WAIT,
        SEQ_SAVE_START,
        SEQ_SAVE_MAIN,
        SEQ_PLAY_FADEOUT_START,
        SEQ_PLAY_FADEOUT_WAIT,
        SEQ_PLAY_END,
    };
    BrRecordWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_CHANGE;
        }
        break;
    case SEQ_CHANGE:
        if (p_wk->isProfile) {
            func_ov183_021dce28(p_wk, p_wk->p_param);
        } else {
            func_ov183_021dce48(p_wk, p_wk->p_param);
        }
        func_ov183_021dd054(p_wk, p_wk->p_param);
        func_ov183_021dd128(p_wk, p_wk->p_param);
        *p_seq = SEQ_FADEIN_START;
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
    case SEQ_WAIT:
        (*p_seq)++;
        break;
    case SEQ_INPUT: {
        u32 x;
        u32 y;

        if (func_02035d20(&x, &y)) {
            if (func_ov183_021dd2fc(p_wk, x, y)) {
                *p_seq = SEQ_YES_FADEOUT_START;
            } else if (func_ov183_021dd33c(p_wk, x, y)) {
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
    case SEQ_YES_END:
        func_ov183_021dd1d4(p_wk, p_wk->p_param);
        *p_seq = SEQ_SAVE_INIT;
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
    case SEQ_NO_END:
        func_ov183_021dd1d4(p_wk, p_wk->p_param);
        func_ov186_021fd73c(p_seqwk, func_ov183_021dbc68);
        break;
    case SEQ_SAVE_INIT:
        func_ov183_021dd0ac(p_wk, p_wk->p_param);
        *p_seq = SEQ_SAVE_FADEIN_START;
        break;
    case SEQ_SAVE_FADEIN_START:
        func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x152);
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_SAVE_FADEIN_WAIT;
        break;
    case SEQ_SAVE_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_SAVE_WAIT;
        }
        break;
    case SEQ_SAVE_WAIT:
        *p_seq = SEQ_SAVE_START;
        break;
    case SEQ_SAVE_START:
        func_02012dac(p_wk->p_param->gameData);
        *p_seq = SEQ_SAVE_MAIN;
        break;
    case SEQ_SAVE_MAIN:
        if (func_02012dd0(p_wk->p_param->gameData) == 2) {
            func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_NONE, NULL);
            *p_seq = SEQ_PLAY_FADEOUT_START;
        }
        break;
    case SEQ_PLAY_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_PLAY_FADEOUT_WAIT;
        break;
    case SEQ_PLAY_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_PLAY_END;
        }
        break;
    case SEQ_PLAY_END:
        if (!GFL_SndIsPlaying(SEQ_SE_BREC_04)) {
            func_ov183_021dd0f4(p_wk, p_wk->p_param);
            p_wk->p_param->result = 1;
            func_ov186_021f9c50(p_wk->p_param->procSys);
            func_ov186_021fd744(p_seqwk);
        }
        break;
    }
}

static void func_ov183_021dc8ec(BrRecordWork *p_wk, BrRecordProcParam *p_param) {
    int i;
    Font *font;
    MsgData *msg;
    WordSet *wordSet;
    BOOL isPrint;
    StrBuf *str;

    if (p_wk->isInfoCreated) {
        return;
    }
    p_wk->isInfoCreated = TRUE;

    if (func_ov183_021dd228(p_wk->header)) {
        func_ov186_021fb964(p_param->res, 7, p_wk->heapId);
    } else {
        func_ov186_021fb964(p_param->res, 6, p_wk->heapId);
    }
    font = func_ov186_021fc20c(p_param->res);
    msg = func_ov186_021fc214(p_param->res);
    wordSet = func_ov186_021fc21c(p_param->res);

    for (i = 0; i < MSGWIN_MAX; i++) {
        const BrRecordMsgWinData *data = &data_ov183_021e32c8[i];

        p_wk->msgWin[i] = func_ov186_021fc6a4(1, data->x, data->y, data->w, data->h, 14, p_wk->que, p_wk->heapId);
        switch (i) {
        case MSGWIN_NAME: {
            StrBuf *name;
            StrBuf *src;

            str = GFL_StrBufCreate(0x80, p_wk->heapId);
            name = func_0200d6d0(p_wk->profile, p_wk->heapId);
            src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            func_0201ee0c(wordSet, 0, name, func_0200d6ec(p_wk->profile), 1, 2);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            GFL_StrBufFree(name);
            GFL_StrBufFree(src);
            func_ov186_021fc868(p_wk->msgWin[i], 0, 1, 0);
            isPrint = TRUE;
            break;
        }
        case MSGWIN_MODE: {
            u32 mode = func_0200b98c(p_wk->header, BATTLE_REC_HEADER_MODE, 0);

            str = GFL_MsgDataLoadStrbufNew(msg, mode + data->msgID);
            func_ov186_021fc868(p_wk->msgWin[i], 0, 1, 0);
            isPrint = TRUE;
            break;
        }
        case MSGWIN_NUMBER: {
            u64 number = func_0200b98c(p_wk->header, BATTLE_REC_HEADER_DATA_NUMBER, 0);
            u32 low;
            u32 middle;
            u32 high;
            PMSData pms;
            StrBuf *intro;
            StrBuf *src;

            low = number % 100000;
            number /= 100000;
            middle = number % 100000;
            number /= 100000;
            high = number;

            intro = func_0200d774(p_wk->profile, &pms, p_wk->heapId);
            if (intro != NULL) {
                src = GFL_MsgDataLoadStrbufNew(msg, 0xd);
                GFL_StrBufFree(intro);
            } else {
                src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
            }
            str = GFL_StrBufCreate(0x80, p_wk->heapId);
            WordSetNumber(wordSet, 2, low, 5, 2, TRUE);
            WordSetNumber(wordSet, 1, middle, 5, 2, TRUE);
            WordSetNumber(wordSet, 0, high, 2, 2, TRUE);
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            func_ov186_021fc868(p_wk->msgWin[i], 0, 1, 0);
            GFL_StrBufFree(src);
            isPrint = TRUE;
            break;
        }
        case MSGWIN_COUNTER:
            if (func_ov183_021dd25c(p_wk->header)) {
                u32 counter = func_0200b98c(p_wk->header, BATTLE_REC_HEADER_COUNTER, 0);
                StrBuf *src;

                str = GFL_StrBufCreate(0x80, p_wk->heapId);
                src = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
                WordSetNumber(wordSet, 0, counter, 4, 0, TRUE);
                GFL_WordSetFormatStrbuf(wordSet, str, src);
                GFL_StrBufFree(src);
                func_ov186_021fc868(p_wk->msgWin[i], 0, 1, 1);
                isPrint = TRUE;
            } else {
                isPrint = FALSE;
            }
            break;
        case MSGWIN_COUNTER_UNIT:
            if (func_ov183_021dd25c(p_wk->header)) {
                str = GFL_MsgDataLoadStrbufNew(msg, data->msgID);
                func_ov186_021fc868(p_wk->msgWin[i], 0, 1, 1);
                isPrint = TRUE;
            } else {
                isPrint = FALSE;
            }
            break;
        }
        if (isPrint) {
            func_ov186_021fc7c4(p_wk->msgWin[i], str, font, BR_RECORD_PRINT_COLOR);
            GFL_StrBufFree(str);
        }
    }

    func_ov183_021dcbd8(p_wk, p_param->unit, p_wk->header, p_wk->heapId);
}







// Makes the icons of the video's Pokémon, each side's closed up to its first places
// pokebw2's BrRecord_CreateIcons
void func_ov183_021dcbd8(BrRecordWork *p_wk, ClActUnit *unit, BattleRecHeader *header, HeapID heapId) {
    u32 species[BATTLE_REC_HEADER_PKM_MAX];
    u32 form[BATTLE_REC_HEADER_PKM_MAX];
    u32 gender[BATTLE_REC_HEADER_PKM_MAX];
    int cnt = 0;
    int layout;
    ArcTool *handle;
    int step;
    int start;
    int end;
    int i;
    int idx;

    handle = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, HEAPID_TAIL(heapId));
    p_wk->iconPlt = func_0204acec(handle, func_0201bc3c(), CLACT_SURFACE_MAIN, 0x80, heapId);
    p_wk->iconCell = func_0204af28(handle, func_0201bc40(), getOBJTileMapping_MainEng(), heapId);

    idx = 0;
    step = 6;
    start = 0;
    end = 6;
    layout = 0;
    if (func_ov183_021dd228(header) == TRUE) {
        step = 3;
        end = 3;
        layout = 1;
    }
    do {
        for (i = start; i < end; i++) {
            u32 monsno;
            u32 formno;
            u32 sex;

            species[idx] = 0xff;
            monsno = func_0200b98c(header, BATTLE_REC_HEADER_SPECIES, i);
            formno = func_0200b98c(header, BATTLE_REC_HEADER_FORM, i);
            sex = func_0200b98c(header, BATTLE_REC_HEADER_GENDER, i);
            if (monsno != 0) {
                species[idx] = monsno;
                form[idx] = formno;
                gender[idx] = sex;
                idx++;
            }
        }
        for (i = idx; i < end; i++) {
            species[idx] = 0;
            form[idx] = 0;
            gender[idx] = 0;
            idx++;
        }
        start += step;
        end += step;
    } while (end <= BATTLE_REC_HEADER_PKM_MAX);

    for (i = 0; i < BATTLE_REC_HEADER_PKM_MAX; i++) {
        p_wk->icon[i] = NULL;
        p_wk->iconChr[i] = -1;
        if (species[i] != 0) {
            ClActorSetup setup;

            p_wk->iconChr[i] = func_0204a6c8(handle, PokeParty_GetIconIndex(species[i], form[i], gender[i], FALSE),
                                             FALSE, CLACT_SURFACE_MAIN, heapId);
            MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
            setup.x = data_ov183_021e32f0[layout][i].x + 8;
            setup.y = data_ov183_021e32f0[layout][i].y - 12;
            setup.sequence = 1;
            setup.priority = BATTLE_REC_HEADER_PKM_MAX - cnt;
            p_wk->icon[cnt] = func_0204b294(unit, p_wk->iconChr[i], p_wk->iconPlt, p_wk->iconCell, &setup, 0, heapId);
            func_0204b688(p_wk->icon[cnt], 1);
            func_0204b6f4(p_wk->icon[cnt], func_0201bb5c(species[i], form[i], gender[i], FALSE), 0);
            cnt++;
        }
    }

    GFL_ArcToolFree(handle);
}

// pokebw2's BrRecord_CreateProfile
static void func_ov183_021dcdd8(BrRecordWork *p_wk, BrRecordProcParam *p_param) {
    if (p_wk->profileWin == NULL && !p_wk->isProfileCreated) {
        u32 type;

        p_wk->isProfileCreated = TRUE;
        if (p_wk->p_param->mode == BR_RECORD_MODE_MINE) {
            type = BR_PROFILE_TYPE_TRAINER;
        } else {
            type = BR_PROFILE_TYPE_UNION;
        }
        p_wk->profileWin = func_ov186_021fd1c4(p_wk->profile, p_param->res, p_param->unit, p_wk->que, type, p_wk->heapId);
    }
}

// pokebw2's BrRecord_DeleteProfile
static void func_ov183_021dce28(BrRecordWork *p_wk, BrRecordProcParam *p_param) {
    if (p_wk->isProfileCreated && p_wk->profileWin != NULL) {
        func_ov186_021fd640(p_wk->profileWin);
        p_wk->profileWin = NULL;
        p_wk->isProfileCreated = FALSE;
    }
}

// pokebw2's BrRecord_DeleteInfo
static void func_ov183_021dce48(BrRecordWork *p_wk, BrRecordProcParam *p_param) {
    int i;

    if (p_wk->isInfoCreated) {
        for (i = 0; i < BATTLE_REC_HEADER_PKM_MAX; i++) {
            if (p_wk->icon[i] != NULL) {
                func_0204b3dc(p_wk->icon[i], FALSE);
                func_0204b3b4(p_wk->icon[i]);
                p_wk->icon[i] = NULL;
            }
            if (p_wk->iconChr[i] != -1) {
                func_0204a8d4(p_wk->iconChr[i]);
                p_wk->iconChr[i] = -1;
            }
        }
        if (p_wk->iconPlt != -1) {
            func_0204ada4(p_wk->iconPlt);
            p_wk->iconPlt = -1;
        }
        if (p_wk->iconCell != -1) {
            func_0204afd8(p_wk->iconCell);
            p_wk->iconCell = -1;
        }
        for (i = 0; i < MSGWIN_MAX; i++) {
            if (p_wk->msgWin[i] != NULL) {
                func_ov186_021fc734(p_wk->msgWin[i]);
                p_wk->msgWin[i] = NULL;
            }
        }
        func_ov186_021fbda4(p_param->res, 6);
        GFL_BGSysLoadScr(1);
        p_wk->isInfoCreated = FALSE;
    }
}

// Makes the buttons of the sub screen and the title of the main screen
// pokebw2's BrRecord_CreateButtons
void func_ov183_021dcee4(BrRecordWork *p_wk, BrRecordProcParam *p_param) {
    Font *font;
    MsgData *msg;

    if (p_wk->isButtonsCreated) {
        return;
    }
    p_wk->isButtonsCreated = TRUE;

    func_ov186_021fb964(p_param->res, 5, p_wk->heapId);
    func_ov186_021fbe54(p_param->res, 6, p_wk->heapId);
    font = func_ov186_021fc20c(p_param->res);
    msg = func_ov186_021fc214(p_param->res);

    if (p_wk->canSave) {
        ClActorSetup setup;
        BrResObjData obj;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.x = 24;
        setup.y = 168;
        setup.sequence = 0;
        setup.priority = 1;
        func_ov186_021fc124(p_param->res, 6, &obj);
        p_wk->btn[BTN_RETURN] = func_ov186_021fb378(&setup, 4, 96, CLACT_SURFACE_SUB, p_param->unit, p_wk->bmpoam,
                                                    font, msg, &obj, p_wk->heapId);
        setup.x = 136;
        setup.sequence = 3;
        p_wk->btn[BTN_SAVE] = func_ov186_021fb378(&setup, 0x54, 96, CLACT_SURFACE_SUB, p_param->unit, p_wk->bmpoam,
                                                  font, msg, &obj, p_wk->heapId);
    } else {
        ClActorSetup setup;
        BrResObjData obj;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.x = 80;
        setup.y = 168;
        setup.sequence = 0;
        setup.priority = 1;
        func_ov186_021fc124(p_param->res, 6, &obj);
        p_wk->btn[BTN_RETURN] = func_ov186_021fb378(&setup, 4, 96, CLACT_SURFACE_SUB, p_param->unit, p_wk->bmpoam,
                                                    font, msg, &obj, p_wk->heapId);
    }

    p_wk->title[0] = func_ov186_021fc6a4(4, 8, 3, 16, 2, 14, p_wk->que, p_wk->heapId);
    if (p_wk->isProfile) {
        func_ov186_021fc754(p_wk->title[0], msg, 0x72, font, BR_RECORD_PRINT_COLOR);
    } else {
        func_ov186_021fc754(p_wk->title[0], msg, 0x71, font, BR_RECORD_PRINT_COLOR);
    }
}


// pokebw2's BrRecord_DeleteButtons
static void func_ov183_021dd054(BrRecordWork *p_wk, BrRecordProcParam *p_param) {
    int i;

    if (p_wk->isButtonsCreated) {
        if (p_wk->title[0] != NULL) {
            func_ov186_021fc734(p_wk->title[0]);
            p_wk->title[0] = NULL;
        }
        for (i = 0; i < BTN_MAX; i++) {
            if (p_wk->btn[i] != NULL) {
                func_ov186_021fb494(p_wk->btn[i]);
                p_wk->btn[i] = NULL;
            }
        }
        GFL_BGSysLoadScr(4);
        func_ov186_021fc0c8(p_param->res, 6);
        func_ov186_021fbda4(p_param->res, 5);
        p_wk->isButtonsCreated = FALSE;
    }
}

// Makes the text box and starts the balls turning, for a download or a save
// pokebw2's BrRecord_CreateText
static void func_ov183_021dd0ac(BrRecordWork *p_wk, BrRecordProcParam *p_param) {
    if (!p_wk->isTextCreated) {
        BrPoint pos;

        p_wk->isTextCreated = TRUE;
        if (p_wk->text == NULL) {
            p_wk->text = func_ov186_021fd12c(p_wk->p_param->res, p_wk->que, p_wk->heapId);
        }
        pos.x = 0x80;
        pos.y = 0x60;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_ROT, &pos);
    }
}

// pokebw2's BrRecord_DeleteText
static void func_ov183_021dd0f4(BrRecordWork *p_wk, BrRecordProcParam *p_param) {
    if (p_wk->isTextCreated) {
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_NONE, NULL);
        if (p_wk->text != NULL) {
            func_ov186_021fd15c(p_wk->text, p_wk->p_param->res);
            p_wk->text = NULL;
        }
        p_wk->isTextCreated = FALSE;
    }
}

// Makes the question before an unwatched video plays and its two answers
// pokebw2's BrRecord_CreateConfirm
void func_ov183_021dd128(BrRecordWork *p_wk, BrRecordProcParam *p_param) {
    Font *font;
    MsgData *msg;
    int i;

    if (!p_wk->isConfirmCreated) {
        p_wk->isConfirmCreated = TRUE;
        if (p_wk->text == NULL) {
            p_wk->text = func_ov186_021fd12c(p_param->res, p_wk->que, p_wk->heapId);
        }
        func_ov186_021fd170(p_wk->text, p_param->res, 0x14f);
        func_ov186_021fb964(p_param->res, 0x1e, p_wk->heapId);
        font = func_ov186_021fc20c(p_param->res);
        msg = func_ov186_021fc214(p_param->res);
        for (i = 0; i < BR_RECORD_TITLE_MAX; i++) {
            const BrRecordMsgWinData *data = &data_ov183_021e32a8[i];

            p_wk->title[i] = func_ov186_021fc6a4(4, data->x, data->y, data->w, data->h, 14, p_wk->que, p_wk->heapId);
            func_ov186_021fc754(p_wk->title[i], msg, data->msgID, font, BR_RECORD_PRINT_COLOR);
        }
    }
}


// pokebw2's BrRecord_DeleteConfirm
static void func_ov183_021dd1d4(BrRecordWork *p_wk, BrRecordProcParam *p_param) {
    int i;

    if (p_wk->isConfirmCreated) {
        for (i = 0; i < BR_RECORD_TITLE_MAX; i++) {
            if (p_wk->title[i] != NULL) {
                func_ov186_021fc734(p_wk->title[i]);
                p_wk->title[i] = NULL;
            }
        }
        GFL_BGSysLoadScr(4);
        func_ov186_021fbda4(p_param->res, 0x1e);
        if (p_wk->text != NULL) {
            func_ov186_021fd15c(p_wk->text, p_param->res);
            p_wk->text = NULL;
        }
        p_wk->isConfirmCreated = FALSE;
    }
}

// Whether the video's battle mode has teams of three Pokémon, whose icons are laid out in groups of three
// pokebw2's BrRecord_IsSmallTeamMode
BOOL func_ov183_021dd228(BattleRecHeader *header) {
    u32 mode = func_0200b98c(header, BATTLE_REC_HEADER_MODE, 0);

    switch (mode) {
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x16:
        return TRUE;
    default:
        return FALSE;
    }
}

// Whether the video's battle mode has a counter to show, such as a Battle Subway streak
// pokebw2's BrRecord_IsCounterMode
BOOL func_ov183_021dd25c(BattleRecHeader *header) {
    u32 mode = func_0200b98c(header, BATTLE_REC_HEADER_MODE, 0);

    switch (mode) {
    case 0x14:
    case 0x15:
    case 0x16:
        return TRUE;
    }
    return FALSE;
}

// The title, which swaps the summary and the profile
// pokebw2's BrRecord_GetTrgChange
static BOOL func_ov183_021dd27c(BrRecordWork *p_wk, u32 x, u32 y) {
    BOOL ret = ((u32)(x - 64) <= (u32)(200 - 64)) & ((u32)(y - 16) <= (u32)(48 - 16));

    if (ret) {
        BrPoint pos;

        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        GFL_SndSEPlay(SEQ_SE_BREC_06);
    }
    return ret;
}

// The play button
// pokebw2's BrRecord_GetTrgPlay
static BOOL func_ov183_021dd2bc(BrRecordWork *p_wk, u32 x, u32 y) {
    BOOL ret = ((u32)(x - 80) <= (u32)(176 - 80)) & ((u32)(y - 56) <= (u32)(120 - 56));

    if (ret) {
        BrPoint pos;

        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        GFL_SndSEPlay(SEQ_SE_BREC_06);
    }
    return ret;
}

// The answers of the question before an unwatched video plays
// pokebw2's BrRecord_GetTrgYes
BOOL func_ov183_021dd2fc(BrRecordWork *p_wk, u32 x, u32 y) {
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

// pokebw2's BrRecord_GetTrgNo
BOOL func_ov183_021dd33c(BrRecordWork *p_wk, u32 x, u32 y) {
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
