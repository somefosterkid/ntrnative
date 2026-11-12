// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov270/br_musical_look_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's musical photo viewing screen: the player picks a Pokémon, downloads the musical photos with it
// from the Global Link, and looks through them, each with the profile of the player who sent it. The name is a guess:
// the ROM gives none, and br_core.c runs it as BR_PROCID_MUSICAL_LOOK

#include "types.h"
#include "app/battle_recorder/br_musical_look_proc.h"
#include "app/battle_recorder/br_btn.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_graphic.h"
#include "app/battle_recorder/br_net.h"
#include "app/battle_recorder/br_pokesearch.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_sidebar.h"
#include "app/battle_recorder/br_util.h"
#include "app/musical/mus_shot_photo.h"
#include "constants/sound.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/printsys.h"

// The most photos downloaded at once
#define RECV_MAX 5

// The button graphics and the sub screen's BG, from br_res.c
#define BR_RES_OBJ_MUSICAL_LOOK_BTN 6
#define BR_RES_BG_MUSICAL_LOOK 0x16

typedef struct {
    BrPokeSearch *search;
    BrMsgWin *text;
    // The species picked in the search
    u16 species;
    u16 cnt;
    // The photo shown, in recv
    u16 idx;
    BrProfile *profile;
    MusShotPhoto *photo;
    BrBtn *btn;
    // The sub screen's text, which says whether the button shows the photo or the profile
    BrMsgWin *msgWin;
    // TRUE while the photo is shown, FALSE while its sender's profile is
    BOOL isPhoto;
    BrSeq *seq;
    BmpOamSys *bmpoam;
    PrintQueue *que;
    HeapID heapId;
    BrMusicalLookProcParam *param;
    // For the main and the sub screen
    BrBallEffect *ballEff[2];
    BrMusicalShotRecv *recv[RECV_MAX];
    int recvNum;
} BrMusicalLookWork;

BOOL func_ov185_021f8480(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov185_021f8518(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov185_021f8590(GameProc *proc, u32 *state, void *param, void *work);
void func_ov185_021f8604(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov185_021f8658(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov185_021f86a4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov185_021f86e4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov185_021f8790(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov185_021f88b0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov185_021f8950(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov185_021f8a0c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov185_021f8aa8(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov185_021f8b5c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov185_021f8c30(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov185_021f8d18(BrMusicalLookWork *wk, BrMusicalLookProcParam *param);
static void func_ov185_021f8d5c(BrMusicalLookWork *wk, BrMusicalLookProcParam *param);
void func_ov185_021f8d98(BrMusicalLookWork *wk, BrMusicalLookProcParam *param);
static void func_ov185_021f8e60(BrMusicalLookWork *wk, BrMusicalLookProcParam *param);
void func_ov185_021f8e90(BrMusicalLookWork *wk, BrMusicalLookProcParam *param);
static void func_ov185_021f8ee8(BrMusicalLookWork *wk, BrMusicalLookProcParam *param);
static void func_ov185_021f8efc(BrMusicalLookWork *wk, BrMusicalLookProcParam *param);
static void func_ov185_021f8f58(BrMusicalLookWork *wk, BrMusicalLookProcParam *param);
static void func_ov185_021f8f88(BrMusicalLookWork *wk, BrMusicalLookProcParam *param);
static void func_ov185_021f8fb0(BrMusicalLookWork *wk, BrMusicalLookProcParam *param);
static BOOL func_ov185_021f8fc8(BrMusicalLookWork *wk, u32 x, u32 y);
static BOOL func_ov185_021f9008(BrMusicalLookWork *wk, u32 x, u32 y);
static BOOL func_ov185_021f9048(BrMusicalLookWork *wk, u32 x, u32 y);

// pokebw2's BR_MUSICAL_LOOK_PROC_FUNCTIONS
const GameProcFunctions data_ov185_021f9088 = {
    func_ov185_021f8480,
    func_ov185_021f8590,
    func_ov185_021f8518,
};

// pokebw2's BrMusicalLook_ProcMain

void func_ov186_021fbe54(BrRes *res, u32 id, HeapID heapId);
BOOL func_ov185_021f8480(GameProc *proc, u32 *state, void *param, void *work) {
    BrMusicalLookProcParam *p = param;
    BrMusicalLookWork *wk;
    ClActUnit *unit;
    int i;

    GFL_OvlLoad(OVERLAY_ID(112));

    wk = GFL_ProcInitSubsystem(proc, sizeof(BrMusicalLookWork), func_ov186_021f9b9c(p->procSys));
    MI_CpuFill8(wk, 0, sizeof(BrMusicalLookWork));
    wk->param = p;
    wk->heapId = func_ov186_021f9b9c(p->procSys);

    wk->que = func_0201c440(wk->heapId);
    unit = func_ov186_021fc4dc(p->graphic);
    wk->bmpoam = func_02025538(wk->heapId, unit);
    for (i = 0; i < 2; i++) {
        wk->ballEff[i] = func_ov186_021fd760(unit, p->res, i, wk->heapId);
    }
    func_ov186_021fbe54(p->res, 6, wk->heapId);
    GFL_BGSysSetBGEnabled(0, TRUE);

    func_ov185_021f8efc(wk, p);

    wk->seq = func_ov186_021fd6d8(wk, func_ov185_021f8604, wk->heapId);
    return TRUE;
}

BOOL func_ov185_021f8518(GameProc *proc, u32 *state, void *param, void *work) {
    BrMusicalLookProcParam *p = param;
    BrMusicalLookWork *wk = work;
    int i;

    func_ov185_021f8d5c(wk, p);
    func_ov185_021f8e60(wk, p);
    func_ov185_021f8ee8(wk, p);
    func_ov185_021f8f58(wk, p);
    func_ov185_021f8fb0(wk, p);
    func_ov186_021fd714(wk->seq);
    func_ov185_021f8f58(wk, p);
    func_ov186_021fc0c8(p->res, BR_RES_OBJ_MUSICAL_LOOK_BTN);
    for (i = 0; i < 2; i++) {
        func_ov186_021fd820(wk->ballEff[i]);
    }
    func_02025588(wk->bmpoam);
    func_0201c4c0(wk->que);
    GFL_ProcReleaseSubsystem(proc);
    GFL_OvlUnload(OVERLAY_ID(112));
    return TRUE;
}


static BOOL func_ov185_021f8590(GameProc *proc, u32 *state, void *param, void *work) {
    BrMusicalLookWork *wk = work;
    int i;

    if (func_ov186_021ff88c(wk->param->net) == 2) {
        func_ov186_021f9c6c(wk->param->procSys);
        return TRUE;
    }

    func_ov186_021fd71c(wk->seq);
    if (func_ov186_021fd72c(wk->seq)) {
        return TRUE;
    }

    for (i = 0; i < 2; i++) {
        func_ov186_021fd854(wk->ballEff[i]);
    }
    func_0201c4e4(wk->que);
    if (wk->msgWin != NULL) {
        func_ov186_021fc830(wk->msgWin);
    }
    if (wk->profile != NULL) {
        func_ov186_021fd6b0(wk->profile);
    }
    if (wk->text != NULL) {
        func_ov186_021fd1bc(wk->text);
    }
    if (wk->photo != NULL) {
        func_ov112_021f3c50(wk->photo);
    }
    return FALSE;
}

void func_ov185_021f8604(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_PRINT_WAIT,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrMusicalLookWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_PRINT_WAIT:
        if (func_ov186_021ffe34(wk->search)) {
            func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
            *p_seq = SEQ_FADEIN_WAIT;
        }
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_NEXT;
        }
        break;
    case SEQ_NEXT:
        func_ov186_021fd73c(p_seqwk, func_ov185_021f86a4);
        break;
    }
}

void func_ov185_021f8658(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_END,
    };
    BrMusicalLookWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_END;
        }
        break;
    case SEQ_END:
        func_ov186_021fd744(wk->seq);
        func_ov186_021f9ba0(wk->param->procSys);
        break;
    }
}

void func_ov185_021f86a4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrMusicalLookWork *wk = p_wk_adrs;
    u16 species;
    u32 select;

    func_ov186_021ffc9c(wk->search);
    select = func_ov186_021ffe90(wk->search, &species);
    switch (select) {
    case BR_POKESEARCH_SELECT_NONE:
        break;
    case BR_POKESEARCH_SELECT_DECIDE:
        wk->species = species;
        func_ov186_021fd73c(p_seqwk, func_ov185_021f8aa8);
        break;
    default:
        func_ov186_021fd73c(p_seqwk, func_ov185_021f8658);
        break;
    }
}




// Waits for the player to touch the return button, the button that swaps the photo and the profile, or the arrows
// pokebw2's BrMusicalLook_SeqTouch
void func_ov185_021f86e4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrMusicalLookWork *wk = p_wk_adrs;
    u32 x;
    u32 y;

    if (func_02035d20(&x, &y)) {
        if (func_ov186_021fb4b4(wk->btn, x, y)) {
            BrPoint pos;

            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
            func_ov186_021fd73c(p_seqwk, func_ov185_021f8950);
        }
        if (func_ov185_021f8fc8(wk, x, y)) {
            func_ov186_021fd73c(p_seqwk, func_ov185_021f8c30);
        }
        if (func_ov185_021f9008(wk, x, y)) {
            if (wk->idx - 1 < 0) {
                wk->idx = wk->recvNum - 1;
            } else {
                wk->idx--;
            }
            func_ov186_021fd73c(p_seqwk, func_ov185_021f8b5c);
        }
        if (func_ov185_021f9048(wk, x, y)) {
            wk->idx++;
            wk->idx %= wk->recvNum;
            func_ov186_021fd73c(p_seqwk, func_ov185_021f8b5c);
        }
    }
}

// Downloads the photos with the Pokémon, then shows them or the result
// pokebw2's BrMusicalLook_SeqDownload
static void func_ov185_021f8790(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_DOWNLOAD_START,
        SEQ_DOWNLOAD_WAIT,
        SEQ_RESULT,
        SEQ_TOUCH_WAIT,
    };
    BrMusicalLookWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_DOWNLOAD_START: {
        BrNetRequestParam req;

        MI_CpuFill8(&req, 0, sizeof(BrNetRequestParam));
        req.species = wk->species;
        wk->cnt = 0;
        func_ov186_021ff54c(wk->param->net, 1, &req);
        *p_seq = SEQ_DOWNLOAD_WAIT;
        break;
    }
    case SEQ_DOWNLOAD_WAIT:
        // The balls turn for at least 96 frames
        wk->cnt++;
        if (func_ov186_021ff648(wk->param->net) && wk->cnt > 96) {
            func_ov186_021fd860(wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_NONE, NULL);
            *p_seq = SEQ_RESULT;
        }
        break;
    case SEQ_RESULT: {
        u32 msgID;

        if (!func_ov186_021ff72c(wk->param->net, &msgID)) {
            if (func_ov186_021ff660(wk->param->net, wk->recv, RECV_MAX, &wk->recvNum) && wk->recvNum != 0) {
                wk->idx = 0;
                func_ov186_021fd73c(p_seqwk, func_ov185_021f88b0);
            } else {
                // No photo has the Pokémon
                *p_seq = SEQ_TOUCH_WAIT;
                func_ov186_021fd170(wk->text, wk->param->res, 0x14d);
            }
        } else {
            *p_seq = SEQ_TOUCH_WAIT;
            func_ov186_021fd170(wk->text, wk->param->res, msgID);
        }
        break;
    }
    case SEQ_TOUCH_WAIT:
        if (func_02035c60()) {
            u32 x;
            u32 y;
            BrPoint pos;

            func_02035d20(&x, &y);
            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
            GFL_SndSEPlay(SEQ_SE_BREC_06);
            func_ov186_021fd73c(p_seqwk, func_ov185_021f8a0c);
        }
        break;
    }
}

void func_ov185_021f88b0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_CHANGE_SCREEN,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrMusicalLookWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_MASTER_BRIGHT_AND_ALPHA, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_CHANGE_SCREEN;
        }
        break;
    case SEQ_CHANGE_SCREEN:
        func_ov185_021f8fb0(wk, wk->param);
        func_ov185_021f8d18(wk, wk->param);
        func_ov185_021f8d98(wk, wk->param);
        *p_seq = SEQ_FADEIN_START;
        break;
    case SEQ_FADEIN_START:
        func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_MASTER_BRIGHT_AND_ALPHA, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_NEXT;
        }
        break;
    case SEQ_NEXT:
        wk->isPhoto = TRUE;
        func_ov186_021fd73c(p_seqwk, func_ov185_021f86e4);
        break;
    }
}

void func_ov185_021f8950(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_CHANGE_SCREEN,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrMusicalLookWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        if (wk->isPhoto) {
            func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_MASTER_BRIGHT_AND_ALPHA, BR_FADE_DISPLAY_BOTH,
                             BR_FADE_DIR_OUT);
        } else {
            func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        }
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_CHANGE_SCREEN;
        }
        break;
    case SEQ_CHANGE_SCREEN:
        func_ov185_021f8ee8(wk, wk->param);
        func_ov185_021f8d5c(wk, wk->param);
        func_ov185_021f8e60(wk, wk->param);
        func_ov185_021f8efc(wk, wk->param);
        *p_seq = SEQ_FADEIN_START;
        break;
    case SEQ_FADEIN_START:
        if (wk->isPhoto) {
            func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_MASTER_BRIGHT_AND_ALPHA, BR_FADE_DISPLAY_BOTH,
                             BR_FADE_DIR_IN);
        } else {
            func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        }
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_NEXT;
        }
        break;
    case SEQ_NEXT:
        func_ov186_021fd73c(p_seqwk, func_ov185_021f86a4);
        break;
    }
}

void func_ov185_021f8a0c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_CHANGE_SCREEN,
        SEQ_PRINT_WAIT,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrMusicalLookWork *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_wk->param->fade)) {
            *p_seq = SEQ_CHANGE_SCREEN;
        }
        break;
    case SEQ_CHANGE_SCREEN:
        func_ov185_021f8fb0(p_wk, p_wk->param);
        func_ov185_021f8efc(p_wk, p_wk->param);
        *p_seq = SEQ_PRINT_WAIT;
        break;
    case SEQ_PRINT_WAIT:
        if (func_ov186_021ffe34(p_wk->search)) {
            func_ov186_021fe784(p_wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
            *p_seq = SEQ_FADEIN_WAIT;
        }
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_wk->param->fade)) {
            *p_seq = SEQ_NEXT;
        }
        break;
    case SEQ_NEXT:
        func_ov186_021fd73c(p_seqwk, func_ov185_021f86a4);
        break;
    }
}

void func_ov185_021f8aa8(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_CHANGE_SCREEN,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrMusicalLookWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_CHANGE_SCREEN;
        }
        break;
    case SEQ_CHANGE_SCREEN: {
        BrPoint pos;

        func_ov185_021f8f58(wk, wk->param);
        func_ov185_021f8f88(wk, wk->param);
        pos.x = 0x80;
        pos.y = 0x60;
        func_ov186_021fd860(wk->ballEff[0], BR_BALL_EFFECT_ROT, &pos);
        *p_seq = SEQ_FADEIN_START;
        break;
    }
    case SEQ_FADEIN_START:
        func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_NEXT;
        }
        break;
    case SEQ_NEXT:
        func_ov186_021fd73c(p_seqwk, func_ov185_021f8790);
        break;
    }
}

void func_ov185_021f8b5c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_CHANGE_SCREEN,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrMusicalLookWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        if (wk->isPhoto) {
            func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_MASTER_BRIGHT_WHITE, BR_FADE_DISPLAY_MAIN, BR_FADE_DIR_OUT);
        } else {
            func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_MAIN, BR_FADE_DIR_OUT);
        }
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_CHANGE_SCREEN;
        }
        break;
    case SEQ_CHANGE_SCREEN:
        if (wk->isPhoto) {
            func_ov185_021f8ee8(wk, wk->param);
            func_ov185_021f8d5c(wk, wk->param);
            func_ov185_021f8d18(wk, wk->param);
        } else {
            func_ov185_021f8ee8(wk, wk->param);
            func_ov185_021f8d5c(wk, wk->param);
            func_ov185_021f8e90(wk, wk->param);
        }
        *p_seq = SEQ_FADEIN_START;
        break;
    case SEQ_FADEIN_START:
        if (wk->isPhoto) {
            func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_MASTER_BRIGHT_WHITE, BR_FADE_DISPLAY_MAIN, BR_FADE_DIR_IN);
        } else {
            func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_MAIN, BR_FADE_DIR_IN);
        }
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_NEXT;
        }
        break;
    case SEQ_NEXT:
        func_ov186_021fd73c(p_seqwk, func_ov185_021f86e4);
        break;
    }
}

void func_ov185_021f8c30(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_CHANGE_SCREEN,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrMusicalLookWork *wk = p_wk_adrs;
    void *msgData;
    void *wordSet;
    u32 msgID;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_MASTER_BRIGHT_WHITE, BR_FADE_DISPLAY_MAIN, BR_FADE_DIR_OUT);
        *p_seq = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_CHANGE_SCREEN;
        }
        break;
    case SEQ_CHANGE_SCREEN: {
        BOOL isPhoto = wk->photo != NULL;

        if (isPhoto) {
            func_ov185_021f8d5c(wk, wk->param);
            func_ov185_021f8e90(wk, wk->param);
        } else {
            func_ov185_021f8ee8(wk, wk->param);
            func_ov185_021f8d18(wk, wk->param);
        }

        msgData = func_ov186_021fc20c(wk->param->res);
        wordSet = func_ov186_021fc214(wk->param->res);
        if (isPhoto) {
            wk->isPhoto = FALSE;
            msgID = 0x72;
        } else {
            wk->isPhoto = TRUE;
            msgID = 0x71;
        }
        func_ov186_021fc754(wk->msgWin, wordSet, msgID, msgData, 0x3da0);
        *p_seq = SEQ_FADEIN_START;
        break;
    }
    case SEQ_FADEIN_START:
        func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_MASTER_BRIGHT_WHITE, BR_FADE_DISPLAY_MAIN, BR_FADE_DIR_IN);
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_NEXT;
        }
        break;
    case SEQ_NEXT:
        func_ov186_021fd73c(p_seqwk, func_ov185_021f86e4);
        break;
    }
}







// Shows the photo on the main screen in place of its BG and sidebars
// pokebw2's BrMusicalLook_CreatePhoto
static void func_ov185_021f8d18(BrMusicalLookWork *wk, BrMusicalLookProcParam *param) {
    if (wk->photo == NULL) {
        func_ov186_021feec0(param->sidebar, CLACT_SURFACE_MAIN, FALSE);
        func_ov186_021fbda4(param->res, 0);
        func_ov186_021fc1f8(param->res, CLACT_VRAM_MAIN);
        func_ov186_021fc610(param->graphic, wk->heapId);
        wk->photo = func_ov112_021f3af0(&wk->recv[wk->idx]->shot, wk->heapId);
    }
}

// pokebw2's BrMusicalLook_DeletePhoto
static void func_ov185_021f8d5c(BrMusicalLookWork *wk, BrMusicalLookProcParam *param) {
    if (wk->photo != NULL) {
        func_ov112_021f3bf0(wk->photo);
        wk->photo = NULL;
        func_ov186_021fc640(param->graphic, wk->heapId);
        func_ov186_021fc148(param->res, CLACT_VRAM_MAIN, wk->heapId);
        func_ov186_021fb964(param->res, 0, wk->heapId);
        func_ov186_021feec0(param->sidebar, CLACT_SURFACE_MAIN, TRUE);
    }
}

void func_ov186_021fb964(BrRes *res, u32 id, HeapID heapId);
void func_ov185_021f8d98(BrMusicalLookWork *wk, BrMusicalLookProcParam *param) {
    ClActUnit *unit;
    Font *font;
    MsgData *msg;
    BOOL ret;
    ClActorSetup setup;
    BrResObjData obj;

    if (wk->msgWin == NULL) {
        font = func_ov186_021fc20c(param->res);
        msg = func_ov186_021fc214(param->res);
        unit = func_ov186_021fc4dc(param->graphic);

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.x = 80;
        setup.y = 168;
        setup.sequence = 0;
        setup.priority = 1;
        unit = func_ov186_021fc4dc(param->graphic);
        ret = func_ov186_021fc124(param->res, BR_RES_OBJ_MUSICAL_LOOK_BTN, &obj);
        GFL_ASSERT(ret);
        wk->btn = func_ov186_021fb378(&setup, 4, 96, CLACT_SURFACE_SUB, unit, wk->bmpoam, font, msg, &obj, wk->heapId);

        wk->msgWin = func_ov186_021fc6a4(4, 8, 3, 16, 2, 14, wk->que, wk->heapId);
        func_ov186_021fc754(wk->msgWin, msg, 0x71, font, 0x3da0);
        func_ov186_021fb964(param->res, BR_RES_BG_MUSICAL_LOOK, wk->heapId);
    }
}


// pokebw2's BrMusicalLook_DeleteButton
static void func_ov185_021f8e60(BrMusicalLookWork *wk, BrMusicalLookProcParam *param) {
    if (wk->msgWin != NULL) {
        func_ov186_021fbda4(param->res, BR_RES_BG_MUSICAL_LOOK);
        func_ov186_021fb494(wk->btn);
        wk->btn = NULL;
        func_ov186_021fc734(wk->msgWin);
        wk->msgWin = NULL;
        // The BG is unloaded a second time
        func_ov186_021fbda4(param->res, BR_RES_BG_MUSICAL_LOOK);
    }
}

void func_ov185_021f8e90(BrMusicalLookWork *wk, BrMusicalLookProcParam *param) {
    Font *font;
    MsgData *msg;

    if (wk->profile == NULL) {
        font = func_ov186_021fc20c(param->res);
        msg = func_ov186_021fc214(param->res);
        wk->profile = func_ov186_021fd1c4((GdsProfile *)wk->recv[wk->idx]->profile, param->res,
                             func_ov186_021fc4dc(param->graphic), wk->que, 1, wk->heapId);
        func_ov186_021fc754(wk->msgWin, msg, 0x72, font, 0x3da0);
    }
}


// pokebw2's BrMusicalLook_DeleteProfile
static void func_ov185_021f8ee8(BrMusicalLookWork *wk, BrMusicalLookProcParam *param) {
    if (wk->profile != NULL) {
        func_ov186_021fd640(wk->profile);
        wk->profile = NULL;
    }
}

// pokebw2's BrMusicalLook_CreateSearch
static void func_ov185_021f8efc(BrMusicalLookWork *wk, BrMusicalLookProcParam *param) {
    ClActUnit *unit;

    if (wk->search == NULL) {
        unit = func_ov186_021fc4dc(param->graphic);
        if (wk->text == NULL) {
            wk->text = func_ov186_021fd12c(param->res, wk->que, wk->heapId);
        }
        func_ov186_021fd170(wk->text, param->res, 0x2c);
        wk->search = func_ov186_021ffbd4(GameData_GetPokedex(param->gameData), param->res, unit, wk->bmpoam, param->fade,
                                       wk->ballEff[CLACT_SURFACE_MAIN], wk->ballEff[CLACT_SURFACE_SUB], wk->heapId);
        func_ov186_021ffe5c(wk->search);
    }
}

// pokebw2's BrMusicalLook_DeleteSearch
static void func_ov185_021f8f58(BrMusicalLookWork *wk, BrMusicalLookProcParam *param) {
    if (wk->search != NULL) {
        func_ov186_021ffe70(wk->search);
        func_ov186_021ffc6c(wk->search);
        wk->search = NULL;
        GFL_BGSysLoadScr(4);
        if (wk->text != NULL) {
            func_ov186_021fd15c(wk->text, param->res);
            wk->text = NULL;
        }
    }
}

// The text that the photos are being downloaded
// pokebw2's BrMusicalLook_CreateText
static void func_ov185_021f8f88(BrMusicalLookWork *wk, BrMusicalLookProcParam *param) {
    if (wk->text == NULL) {
        wk->text = func_ov186_021fd12c(param->res, wk->que, wk->heapId);
    }
    func_ov186_021fd170(wk->text, param->res, 0x146);
}

// pokebw2's BrMusicalLook_DeleteText
static void func_ov185_021f8fb0(BrMusicalLookWork *wk, BrMusicalLookProcParam *param) {
    if (wk->text != NULL) {
        func_ov186_021fd15c(wk->text, param->res);
        wk->text = NULL;
    }
}

// The button over the photo that swaps it and the profile
// pokebw2's BrMusicalLook_GetTrgChange
static BOOL func_ov185_021f8fc8(BrMusicalLookWork *wk, u32 x, u32 y) {
    BOOL ret = ((u32)(x - 64) <= (u32)(184 - 64)) & ((u32)(y - 16) <= (u32)(48 - 16));

    if (ret) {
        BrPoint pos;

        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        GFL_SndSEPlay(SEQ_SE_BREC_06);
    }
    return ret;
}

// pokebw2's BrMusicalLook_GetTrgLeft
static BOOL func_ov185_021f9008(BrMusicalLookWork *wk, u32 x, u32 y) {
    BOOL ret = ((u32)(x - 24) <= (u32)(64 - 24)) & ((u32)(y - 64) <= (u32)(104 - 64));

    if (ret) {
        BrPoint pos;

        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        GFL_SndSEPlay(SEQ_SE_BREC_06);
    }
    return ret;
}

// pokebw2's BrMusicalLook_GetTrgRight
static BOOL func_ov185_021f9048(BrMusicalLookWork *wk, u32 x, u32 y) {
    BOOL ret = ((u32)(x - 184) <= (u32)(224 - 184)) & ((u32)(y - 64) <= (u32)(104 - 64));

    if (ret) {
        BrPoint pos;

        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        GFL_SndSEPlay(SEQ_SE_BREC_06);
    }
    return ret;
}
