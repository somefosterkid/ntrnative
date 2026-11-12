// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_bvrank_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's battle video list screen: downloads a list of battle videos from the Global Link and shows
// their players' names in a list on the sub screen, with the Pokémon of the five videos in view on the main screen.
// The record screen shows the chosen video. The name is a guess: the ROM gives none, and br_core.c runs it as
// BR_PROCID_BV_RANK

#include "types.h"
#include "app/battle_recorder/br_btn.h"
#include "app/battle_recorder/br_bvrank_proc.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_net.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "save/battle_rec.h"
#include "system/bmp_menuwork.h"
#include "system/bmp_oam.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "gfl/proc.h"
#include "pml/poke_party.h"

#define BR_BVRANK_PRINT_COLOR 0x3da0

// The videos of the list that the main screen shows the Pokémon of, and the Pokémon of a video
#define BR_BVRANK_ICON_ROW_MAX 5
#define BR_BVRANK_ICON_COL_MAX 6

// The species past the last one that has an icon
#define BR_BVRANK_SPECIES_END 651

// The frames the download takes at least
#define BR_BVRANK_DOWNLOAD_WAIT 96

// The Pokémon of a video, without the empty slots
typedef struct {
    u32 species[BR_BVRANK_ICON_COL_MAX];
    u32 form[BR_BVRANK_ICON_COL_MAX];
    u32 gender[BR_BVRANK_ICON_COL_MAX];
} BrBvRankPokeData;

// The list of the videos and the icons of their Pokémon
typedef struct {
    ListMenuOption *p_list_data;
    BrList *p_list;
    WordSet *p_word;
    BrBallEffect *ballMain;
    BrBallEffect *ballSub;
    u16 plt;
    u16 cel;
    // The icons' graphics, a ring of rows that starts at chrTop
    u16 chr[BR_BVRANK_ICON_ROW_MAX][BR_BVRANK_ICON_COL_MAX];
    int chrTop;
    ClActor *icon[BR_BVRANK_ICON_ROW_MAX][BR_BVRANK_ICON_COL_MAX];
    BrOutlineData *p_outline;
} BrBvRankList;

typedef struct {
    BmpOamSys *bmpoam;
    BrBtn *btn;
    BrMsgWin *captionWin;
    BrMsgWin *titleWin;
    BrSeq *seq;
    PrintQueue *que;
    HeapID heapId;
    BrBallEffect *ballEff[2];
    BrMsgWin *text;
    BrBvRankList list;
    u32 cnt;
    BOOL isListCreated;
    BOOL isResLoaded;
    BrBvRankProcParam *p_param;
} BrBvRankWork;

BOOL func_ov183_021de9a0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021dea30(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021deadc(GameProc *proc, u32 *state, void *param, void *work);
void func_ov183_021deb4c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021decb0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021ded14(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021ded64(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021dedb4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021dee7c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021df0dc(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021df18c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov183_021df1dc(BrBvRankList *p_wk, BrOutlineData *p_outline, BrRes *res, ClActUnit *unit, BrBallEffect *ballMain, BrBallEffect *ballSub, BrListPos *pos, HeapID heapId);
static void func_ov183_021df388(BrBvRankList *p_list);
void func_ov183_021df3dc(BrBvRankList *p_list, HeapID heapId);
u32 func_ov183_021df41c(const BrBvRankList *cp_wk);
void func_ov183_021df428(BrBvRankList *p_wk, ClActUnit *unit, HeapID heapId);
static void func_ov183_021df5b8(BrBvRankList *p_wk);
void func_ov183_021df620(BrBvRankList *p_wk, u32 list, s8 slide, HeapID heapId);
void func_ov183_021df838(BrOutline *outline, BrBvRankPokeData *poke);
BOOL func_ov183_021df8f0(u32 x, u32 y);

// pokebw2's BR_BVRANK_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e36f4 = {
    func_ov183_021de9a0,
    func_ov183_021deadc,
    func_ov183_021dea30,
};

// The list of the videos, whose items are bitmaps of three characters each
// pokebw2's sBrBvRankListParam
static const BrListParam data_ov183_021e3700 = {
    NULL, 0, 2, 8, 10, 15, 14, 1, 3, BR_LIST_TYPE_CURSOR, NULL, NULL, NULL, NULL, NULL,
};

// pokebw2's BrBvRank_ProcExit

BrBallEffect *func_ov186_021fd760(ClActUnit *unit, BrRes *res, u32 display, HeapID heapId);



BOOL func_ov183_021de9a0(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvRankProcParam *p_param = param;
    BrBvRankWork *p_wk;
    int i;

    p_wk = GFL_ProcInitSubsystem(proc, sizeof(BrBvRankWork), func_ov186_021f9b9c(p_param->procSys));
    MI_CpuFill8(p_wk, 0, sizeof(BrBvRankWork));
    p_wk->p_param = p_param;
    p_wk->heapId = func_ov186_021f9b9c(p_param->procSys);

    p_wk->bmpoam = func_02025538(p_wk->heapId, p_param->unit);
    p_wk->que = func_0201c440(p_wk->heapId);
    for (i = 0; i < 2; i++) {
        p_wk->ballEff[i] = func_ov186_021fd760(p_param->unit, p_param->res, i, p_wk->heapId);
    }

    p_wk->seq = func_ov186_021fd6d8(p_wk, func_ov183_021df0dc, p_wk->heapId);
    if (p_wk->p_param->isReturn) {
        func_ov186_021fd73c(p_wk->seq, func_ov183_021deb4c);
    } else {
        func_ov186_021fd73c(p_wk->seq, func_ov183_021df0dc);
    }
    return TRUE;
}

static BOOL func_ov183_021dea30(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvRankProcParam *p_param = param;
    BrBvRankWork *p_wk = work;
    int i;

    if (p_wk->isListCreated) {
        func_ov183_021df388(&p_wk->list);
        p_wk->isListCreated = FALSE;
    }
    if (p_wk->titleWin != NULL) {
        func_ov186_021fc734(p_wk->titleWin);
        p_wk->titleWin = NULL;
    }
    if (p_wk->captionWin != NULL) {
        func_ov186_021fc734(p_wk->captionWin);
        p_wk->captionWin = NULL;
    }
    if (p_wk->btn != NULL) {
        func_ov186_021fb494(p_wk->btn);
        p_wk->btn = NULL;
    }
    if (p_wk->isResLoaded) {
        func_ov186_021fc0c8(p_wk->p_param->res, 6);
        func_ov186_021fbda4(p_wk->p_param->res, 0x18);
        p_wk->isResLoaded = FALSE;
    }
    func_ov186_021fd714(p_wk->seq);
    if (p_wk->text != NULL) {
        func_ov186_021fd15c(p_wk->text, p_param->res);
        p_wk->text = NULL;
    }
    for (i = 0; i < 2; i++) {
        func_ov186_021fd820(p_wk->ballEff[i]);
    }
    func_02025588(p_wk->bmpoam);
    func_0201c4c0(p_wk->que);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrBvRank_ProcMain
static BOOL func_ov183_021deadc(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvRankWork *p_wk = work;
    int i;

    if (func_ov186_021ff88c(p_wk->p_param->net) == 2) {
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
    if (p_wk->captionWin != NULL) {
        func_ov186_021fc830(p_wk->captionWin);
    }
    if (p_wk->titleWin != NULL) {
        func_ov186_021fc830(p_wk->titleWin);
    }
    if (p_wk->text != NULL) {
        func_ov186_021fd1bc(p_wk->text);
    }
    return FALSE;
}

void func_ov183_021deb4c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBvRankWork *p_wk = p_wk_adrs;
    Font *font;
    MsgData *msg;
    ClActorSetup setup;
    BrResObjData obj;
    BOOL ret;

    font = func_ov186_021fc20c(p_wk->p_param->res);
    msg = func_ov186_021fc214(p_wk->p_param->res);
    if (p_wk->text != NULL) {
        func_ov186_021fd15c(p_wk->text, p_wk->p_param->res);
        p_wk->text = NULL;
    }
    func_ov186_021fb964(p_wk->p_param->res, 0x18, p_wk->heapId);
    func_ov186_021fbe54(p_wk->p_param->res, 6, p_wk->heapId);
    p_wk->isResLoaded = TRUE;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 80;
    setup.y = 168;
    setup.sequence = 0;
    setup.priority = 1;
    ret = func_ov186_021fc124(p_wk->p_param->res, 6, &obj);
    GFL_ASSERT(ret);
    p_wk->btn = func_ov186_021fb378(&setup, 4, 96, CLACT_SURFACE_SUB, p_wk->p_param->unit, p_wk->bmpoam, font, msg,
                                    &obj, p_wk->heapId);

    p_wk->captionWin = func_ov186_021fc6a4(4, 11, 6, 10, 4, 14, p_wk->que, p_wk->heapId);
    func_ov186_021fc754(p_wk->captionWin, msg, 0x55, font, BR_BVRANK_PRINT_COLOR);

    p_wk->titleWin = func_ov186_021fc6a4(1, 3, 3, 26, 4, 14, p_wk->que, p_wk->heapId);
    func_ov186_021fc868(p_wk->titleWin, 0, 1, BR_MSGWIN_POS_ABSOLUTE);
    func_ov186_021fc754(p_wk->titleWin, msg, 0x57 + p_wk->p_param->mode, font, BR_BVRANK_PRINT_COLOR);

    func_ov183_021df1dc(&p_wk->list, p_wk->p_param->p_outline, p_wk->p_param->res, p_wk->p_param->unit,
                        p_wk->ballEff[0], p_wk->ballEff[1], p_wk->p_param->listPos, p_wk->heapId);
    p_wk->isListCreated = TRUE;

    func_ov186_021fd73c(p_seqwk, func_ov183_021ded14);
}


void func_ov186_021fd744(BrSeq *p_wk);
void func_ov183_021df388(BrBvRankList *p_list);
void func_ov186_021fc734(BrMsgWin *p_wk);
void func_ov186_021fb494(BrBtn *p_wk);
void func_ov186_021fc0c8(BrRes *p_wk, u32 objID);
void func_ov186_021fbda4(BrRes *p_wk, u32 bgID);


void func_ov183_021decb0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBvRankWork *p_wk = p_wk_adrs;

    func_ov183_021df388(&p_wk->list);
    p_wk->isListCreated = FALSE;

    func_ov186_021fc734(p_wk->titleWin);
    p_wk->titleWin = NULL;
    func_ov186_021fc734(p_wk->captionWin);
    p_wk->captionWin = NULL;
    GFL_BGSysLoadScr(4);
    GFL_BGSysLoadScr(1);

    func_ov186_021fb494(p_wk->btn);
    p_wk->btn = NULL;
    func_ov186_021fc0c8(p_wk->p_param->res, 6);
    func_ov186_021fbda4(p_wk->p_param->res, 0x18);
    p_wk->isResLoaded = FALSE;

    func_ov186_021fd744(p_seqwk);
}


// pokebw2's BrBvRank_SeqFadeIn
void func_ov183_021ded14(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_FADEIN_END,
    };
    BrBvRankWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEIN_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_FADEIN_END;
        }
        break;
    case SEQ_FADEIN_END:
        func_ov186_021fd73c(p_seqwk, func_ov183_021dedb4);
        break;
    }
}

// pokebw2's BrBvRank_SeqFadeOut
void func_ov183_021ded64(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_FADEOUT_END,
    };
    BrBvRankWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_FADEOUT_END;
        }
        break;
    case SEQ_FADEOUT_END:
        func_ov186_021fd73c(p_seqwk, func_ov183_021decb0);
        break;
    }
}




void func_ov183_021dedb4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBvRankWork *p_wk = p_wk_adrs;
    u32 x;
    u32 y;

    func_ov183_021df3dc(&p_wk->list, p_wk->heapId);
    if (func_02035d20(&x, &y)) {
        if (func_ov186_021fb4b4(p_wk->btn, x, y)) {
            BrPoint pos;

            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(p_wk->ballEff[1], 1, &pos);
            func_ov186_021f9ba0(p_wk->p_param->procSys);
            func_ov186_021fd73c(p_seqwk, func_ov183_021ded64);
        }
        if (func_ov183_021df8f0(x, y)) {
            u32 index;
            BrPoint pos;

            index = func_ov183_021df41c(&p_wk->list);
            GFL_ASSERT(index < p_wk->p_param->p_outline->data_num);
            p_wk->p_param->p_outline->select = index;
            func_ov186_021f9bf4(p_wk->p_param->procSys, 2);
            func_ov186_021fd73c(p_seqwk, func_ov183_021ded64);
            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(p_wk->ballEff[1], 1, &pos);
        }
    }
}




void func_ov183_021dee7c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBvRankWork *p_wk = p_wk_adrs;
    BrNetRequestParam req;
    u32 msgID;
    u32 type;
    BOOL ok;
    int i;

    switch (*p_seq) {
    case 0:
        MI_CpuFill8(&req, 0, sizeof(BrNetRequestParam));
        switch (p_wk->p_param->mode) {
        case 0:
            type = 5;
            break;
        case 1:
            type = 6;
            break;
        case 2:
            type = 7;
            break;
        case 3:
            *(BrNetVideoSearchData *)&req = p_wk->p_param->search;
            type = 4;
            break;
        default:
            GFL_ASSERT(FALSE);
        }
        MI_CpuFill8(p_wk->p_param->p_outline, 0, sizeof(BrOutlineData));
        p_wk->cnt = 0;
        func_ov186_021ff54c(p_wk->p_param->net, type, &req);
        *p_seq = 1;
        return;
    case 1:
        p_wk->cnt++;
        if (func_ov186_021ff648(p_wk->p_param->net) && p_wk->cnt > 0x60) {
            func_ov186_021fd860(p_wk->ballEff[0], 0, NULL);
            *p_seq = 2;
        }
        break;
    case 2: {
        BrOutlineData *p_outline = p_wk->p_param->p_outline;
        int *p_num = &p_outline->data_num;

        if (!func_ov186_021ff72c(p_wk->p_param->net, &msgID)) {
            switch (p_wk->p_param->mode) {
            case 0:
                ok = func_ov186_021ff6dc(p_wk->p_param->net, p_outline, BR_OUTLINE_RECV_MAX, p_num);
                break;
            case 1:
                ok = func_ov186_021ff6dc(p_wk->p_param->net, p_outline, BR_OUTLINE_RECV_MAX, p_num);
                break;
            case 2:
                ok = func_ov186_021ff704(p_wk->p_param->net, p_outline, BR_OUTLINE_RECV_MAX, p_num);
                break;
            case 3:
                ok = func_ov186_021ff6b4(p_wk->p_param->net, p_outline, BR_OUTLINE_RECV_MAX, p_num);
                break;
            default:
                GFL_ASSERT(FALSE);
            }
            if (ok) {
                for (i = 0; i < p_wk->p_param->p_outline->data_num; i++) {
                    p_wk->p_param->p_outline->isSecure[i] =
                        func_0200b98c(&p_wk->p_param->p_outline->data[i].header, 5, 0);
                }
                func_ov186_021fd73c(p_seqwk, func_ov183_021df18c);
            } else {
                if (p_wk->p_param->mode == 3) {
                    func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x114);
                } else {
                    func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x153);
                }
                *p_seq = 3;
            }
        } else {
            func_ov186_021fd170(p_wk->text, p_wk->p_param->res, msgID);
            *p_seq = 3;
        }
        return;
    }
    case 3:
        if (func_02035c60()) {
            u32 x;
            u32 y;
            BrPoint pos;

            func_02035d20(&x, &y);
            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(p_wk->ballEff[1], 1, &pos);
            GFL_SndSEPlay(0x703);
            func_ov186_021fe784(p_wk->p_param->fade, 2, 3, 1);
            *p_seq = 4;
        }
        break;
    case 4:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = 5;
        }
        break;
    case 5:
        func_ov186_021f9ba0(p_wk->p_param->procSys);
        func_ov186_021fd744(p_seqwk);
        break;
    }
}



// Shows the message of the download
// pokebw2's BrBvRank_SeqConnect
void func_ov183_021df0dc(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_INIT,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_END,
    };
    BrBvRankWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_INIT: {
        BrPoint pos;

        if (p_wk->text == NULL) {
            p_wk->text = func_ov186_021fd12c(p_wk->p_param->res, p_wk->que, p_wk->heapId);
        }
        if (p_wk->p_param->mode == BR_BVRANK_MODE_SEARCH) {
            func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x14b);
        } else {
            func_ov186_021fd170(p_wk->text, p_wk->p_param->res, 0x13e);
        }
        pos.x = 128;
        pos.y = 96;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_ROT, &pos);
        *p_seq = SEQ_FADEIN_START;
        break;
    }
    case SEQ_FADEIN_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_END;
        }
        break;
    case SEQ_END:
        func_ov186_021fd73c(p_seqwk, func_ov183_021dee7c);
        break;
    }
}

// Fades the message of the download out to show the list
// pokebw2's BrBvRank_SeqDownloadEnd
void func_ov183_021df18c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_FADEOUT_END,
    };
    BrBvRankWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_wk->p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->p_param->fade)) {
            *p_seq = SEQ_FADEOUT_END;
        }
        break;
    case SEQ_FADEOUT_END:
        func_ov186_021fd73c(p_seqwk, func_ov183_021deb4c);
        break;
    }
}

static const BrListParam sBrBvRankListParam = { NULL, 0, 2, 8, 10, 15, 14, 1, 3, 1, NULL, NULL, NULL, NULL, NULL };

void func_ov183_021df1dc(BrBvRankList *p_wk, BrOutlineData *p_outline, BrRes *res, ClActUnit *unit,
                         BrBallEffect *ballMain, BrBallEffect *ballSub, BrListPos *pos, HeapID heapId) {
    Font *font;
    MsgData *msg;
    BrListParam param;
    StrBuf *rank;
    StrBuf *src;
    StrBuf *name;
    int i;

    font = func_ov186_021fc20c(res);
    msg = func_ov186_021fc214(res);

    MI_CpuFill8(p_wk, 0, sizeof(BrBvRankList));
    p_wk->p_outline = p_outline;
    p_wk->p_word = GFL_WordSetSystemCreateDefault(heapId);
    p_wk->ballMain = ballMain;
    p_wk->ballSub = ballSub;

    p_wk->p_list_data = ListMenuCore_CreateOptionList(p_outline->data_num, heapId);
    for (i = 0; i < p_outline->data_num; i++) {
        p_wk->p_list_data[i].value = i;
    }

    param = sBrBvRankListParam;
    param.cp_list = (const BrListItem *)p_wk->p_list_data;
    param.list_max = p_outline->data_num;
    param.res = res;
    param.unit = unit;
    param.ballCursor = p_wk->ballMain;
    param.ballDecide = p_wk->ballSub;
    param.pos = pos;
    p_wk->p_list = func_ov186_021fc8c8(&param, heapId);

    rank = GFL_StrBufCreate(0xff, heapId);
    src = GFL_MsgDataLoadStrbufNew(msg, 0x7a);
    name = GFL_StrBufCreate(0xff, heapId);
    for (i = 0; i < p_outline->data_num; i++) {
        GFLBitmap *bmp = GFL_BitmapCreate(param.w, param.str_line, 0x20, heapId);

        GFL_BitmapFill(bmp, 0);
        WordSetNumber(p_wk->p_word, 0, i + 1, 2, 0, TRUE);
        GFL_WordSetFormatStrbuf(p_wk->p_word, rank, src);
        GFL_StrBufLoadString(name, p_outline->data[i].profile.name);
        GFL_TextRendererDrawToBitmapEx(bmp, 16 - GFL_FontGetBlockWidth(rank, font, 0), 4, rank, font, BR_BVRANK_PRINT_COLOR);
        GFL_TextRendererDrawToBitmapEx(bmp, 20, 4, name, font, BR_BVRANK_PRINT_COLOR);
        func_ov186_021fcdac(p_wk->p_list, (u16)i, bmp);
    }
    GFL_StrBufFree(name);
    GFL_StrBufFree(rank);
    GFL_StrBufFree(src);
    func_ov186_021fce50(p_wk->p_list);

    func_ov183_021df428(p_wk, unit, heapId);
    for (i = 0; i < pos->list; i++) {
        func_ov183_021df620(p_wk, i + 1, 1, heapId);
    }
}


// pokebw2's BrBvRankList_Exit
static void func_ov183_021df388(BrBvRankList *p_wk) {
    int i;

    func_ov183_021df5b8(p_wk);
    for (i = 0; i < func_ov186_021fce0c(p_wk->p_list, BR_LIST_PARAM_LISTMAX); i++) {
        GFL_BitmapFree(func_ov186_021fcde8(p_wk->p_list, (u16)i));
    }
    GFL_WordSetSystemFree(p_wk->p_word);
    func_ov186_021fcc80(p_wk->p_list);
    ListMenuCore_FreeOptionList(p_wk->p_list_data);
    MI_CpuFill8(p_wk, 0, sizeof(BrBvRankList));
}

// Runs the list, and scrolls the icons when it has scrolled
// pokebw2's BrBvRankList_Main
void func_ov183_021df3dc(BrBvRankList *p_wk, HeapID heapId) {
    u32 pre_list;
    u32 now_list;
    s8 slide;

    pre_list = func_ov186_021fce0c(p_wk->p_list, BR_LIST_PARAM_LIST);
    func_ov186_021fcd0c(p_wk->p_list);
    now_list = func_ov186_021fce0c(p_wk->p_list, BR_LIST_PARAM_LIST);
    slide = func_ov186_021fce0c(p_wk->p_list, BR_LIST_PARAM_SLIDE);
    if (slide != 0 && pre_list != now_list) {
        func_ov183_021df620(p_wk, now_list, slide, heapId);
    }
}

// pokebw2's BrBvRankList_GetSelect
u32 func_ov183_021df41c(const BrBvRankList *cp_wk) {
    return func_ov186_021fcda8(cp_wk->p_list);
}

// Makes the icons of the Pokémon of the first videos
// pokebw2's BrBvRankList_CreateIcons
void func_ov183_021df428(BrBvRankList *p_wk, ClActUnit *unit, HeapID heapId) {
    int i;
    ArcTool *handle;
    ClActorSetup setup;
    int j;
    int num;

    if (p_wk->p_outline->data_num < BR_BVRANK_ICON_ROW_MAX) {
        num = p_wk->p_outline->data_num;
    } else {
        num = BR_BVRANK_ICON_ROW_MAX;
    }

    handle = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, HEAPID_TAIL(heapId));
    p_wk->plt = func_0204acec(handle, func_0201bc3c(), CLACT_VRAM_MAIN, 0x80, heapId);
    p_wk->cel = func_0204af28(handle, func_0201bc40(), getOBJTileMapping_MainEng(), heapId);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.sequence = 1;
    setup.priority = 0;
    setup.bgPriority = 0;

    for (i = 0; i < num; i++) {
        BrBvRankPokeData poke;

        func_ov183_021df838(&p_wk->p_outline->data[i], &poke);
        for (j = 0; j < BR_BVRANK_ICON_COL_MAX; j++) {
            u32 species = poke.species[j];

            // An empty slot gets a hidden icon
            if (species == 0) {
                species = 1;
            }
            p_wk->chr[i][j] =
                func_0204a6c8(handle, PokeParty_GetIconIndex(species, poke.form[j], poke.gender[j], FALSE), FALSE,
                              CLACT_VRAM_MAIN, heapId);
            setup.x = j * 24 + 110;
            setup.y = i * 24 + 72;
            setup.priority = BR_BVRANK_ICON_COL_MAX - j;
            p_wk->icon[i][j] =
                func_0204b294(unit, p_wk->chr[i][j], p_wk->plt, p_wk->cel, &setup, CLACT_SURFACE_MAIN, heapId);
            func_0204b6f4(p_wk->icon[i][j], func_0201bb5c(species, poke.form[j], poke.gender[j], FALSE), 0);
            func_0204b688(p_wk->icon[i][j], 1);
            if (poke.species[j] == 0) {
                func_0204b3dc(p_wk->icon[i][j], FALSE);
            } else {
                func_0204b3dc(p_wk->icon[i][j], TRUE);
            }
        }
    }

    GFL_ArcToolFree(handle);
}

// pokebw2's BrBvRankList_DeleteIcons
static void func_ov183_021df5b8(BrBvRankList *p_wk) {
    int i;
    int j;

    func_0204afd8(p_wk->cel);
    func_0204ada4(p_wk->plt);
    for (i = 0; i < BR_BVRANK_ICON_ROW_MAX; i++) {
        for (j = 0; j < BR_BVRANK_ICON_COL_MAX; j++) {
            if (p_wk->icon[i][j] != NULL) {
                func_0204a8d4(p_wk->chr[i][j]);
                func_0204b3b4(p_wk->icon[i][j]);
                p_wk->icon[i][j] = NULL;
            }
        }
    }
}

// Scrolls the icons by a video, down if slide is 1 and up if -1, to show the five videos from list on: the icons
// take the graphics of the row after them, and the row that comes into view gets the graphics that went out of view
// pokebw2's BrBvRankList_ScrollIcons
void func_ov183_021df620(BrBvRankList *p_wk, u32 list, s8 slide, HeapID heapId) {
    ArcTool *handle;
    int row;
    int col;

    for (row = 0; row < BR_BVRANK_ICON_ROW_MAX; row++) {
        for (col = 0; col < BR_BVRANK_ICON_COL_MAX; col++) {
            if (slide == 1 && row != BR_BVRANK_ICON_ROW_MAX - 1) {
                ClActor *src = p_wk->icon[row + 1][col];
                ClActor *dst = p_wk->icon[row][col];

                if (func_0204b3f4(src)) {
                    NNSG2dImageProxy proxy;
                    u8 plt;

                    func_0204b7c8(src, &proxy);
                    plt = func_0204b714(src);
                    func_0204b784(dst, &proxy);
                    func_0204b6f4(dst, plt, 0);
                    func_0204b3dc(dst, TRUE);
                } else {
                    func_0204b3dc(dst, FALSE);
                }
            } else if (slide == -1 && row != BR_BVRANK_ICON_ROW_MAX - 1) {
                ClActor *src = p_wk->icon[BR_BVRANK_ICON_ROW_MAX - 2 - row][col];
                ClActor *dst = p_wk->icon[BR_BVRANK_ICON_ROW_MAX - 1 - row][col];

                if (func_0204b3f4(src)) {
                    NNSG2dImageProxy proxy;
                    u8 plt;

                    func_0204b7c8(src, &proxy);
                    plt = func_0204b714(src);
                    func_0204b784(dst, &proxy);
                    func_0204b6f4(dst, plt, 0);
                    func_0204b3dc(dst, TRUE);
                } else {
                    func_0204b3dc(dst, FALSE);
                }
            }
        }
    }

    handle = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, HEAPID_TAIL(heapId));

    if (slide == -1) {
        p_wk->chrTop--;
        if (p_wk->chrTop < 0) {
            p_wk->chrTop = BR_BVRANK_ICON_ROW_MAX - 1;
        }
    }

    for (col = 0; col < BR_BVRANK_ICON_COL_MAX; col++) {
        BrBvRankPokeData poke;

        if (slide == 1) {
            row = BR_BVRANK_ICON_ROW_MAX - 1;
        } else if (slide == -1) {
            row = 0;
        }
        func_ov183_021df838(&p_wk->p_outline->data[row + list], &poke);
        if (poke.species[col] != 0) {
            NNSG2dCharacterData *chr;
            NNSG2dImageProxy proxy;
            void *buf;

            buf = GFL_G2DIOReadOBJNCGRArc(handle,
                                          PokeParty_GetIconIndex(poke.species[col], poke.form[col], poke.gender[col], FALSE),
                                          FALSE, &chr, HEAPID_TAIL(heapId));
            func_0204a9d0(p_wk->chr[p_wk->chrTop][col], chr);
            func_0204ab84(p_wk->chr[p_wk->chrTop][col], &proxy);
            func_0204b784(p_wk->icon[row][col], &proxy);
            func_0204b6f4(p_wk->icon[row][col], func_0201bb5c(poke.species[col], poke.form[col], poke.gender[col], FALSE),
                          0);
            GFL_HeapFree(buf);
            func_0204b3dc(p_wk->icon[row][col], TRUE);
        } else {
            func_0204b3dc(p_wk->icon[row][col], FALSE);
        }
    }

    if (slide == 1) {
        p_wk->chrTop++;
        if (p_wk->chrTop > BR_BVRANK_ICON_ROW_MAX - 1) {
            p_wk->chrTop = 0;
        }
    }

    GFL_ArcToolFree(handle);
}

// The Pokémon of a video. A battle mode with teams of three has them in the header's first and third groups of three
// pokebw2's BrBvRank_GetPokeData
void func_ov183_021df838(BrOutline *outline, BrBvRankPokeData *poke) {
    BattleRecHeader *header;
    u32 form;
    BOOL isSmallTeam;
    u32 max;
    u32 mode;
    int idx = 0;
    int i;

    MI_CpuFill8(poke, 0, sizeof(BrBvRankPokeData));
    header = &outline->header;
    mode = func_0200b98c(header, BATTLE_REC_HEADER_MODE, 0);
    switch (mode) {
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x16:
        isSmallTeam = TRUE;
        max = 9;
        break;
    default:
        isSmallTeam = FALSE;
        max = 6;
        break;
    }

    for (i = 0; i < max; i++) {
        int species;
        u32 gender;

        if (isSmallTeam && 3 <= i && i <= 5) {
            continue;
        }
        species = func_0200b98c(header, BATTLE_REC_HEADER_SPECIES, i);
        form = func_0200b98c(header, BATTLE_REC_HEADER_FORM, i);
        gender = func_0200b98c(header, BATTLE_REC_HEADER_GENDER, i);
        if (species >= BR_BVRANK_SPECIES_END) {
            species = 0;
        }
        if (species != 0) {
            poke->species[idx] = species;
            poke->form[idx] = form;
            poke->gender[idx] = gender;
            idx++;
        }
    }
}

// Whether the touch is on the list's cursor, which chooses the video under it
// pokebw2's BrBvRank_GetTrgList
BOOL func_ov183_021df8f0(u32 x, u32 y) {
    BOOL ret = ((u32)(x - 80) <= 88) & ((u32)(y - 32) <= 56);

    if (ret) {
        GFL_SndSEPlay(SEQ_SE_BREC_06);
    }
    return ret;
}
