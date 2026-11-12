// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov269/br_musical_send_proc.c, Black 2/White 2's
// version of this file in overlay 269, adapted to Black's. The names are Black's dsd names and swan's; pokebw2's
// guesses are in the comments.

// The Battle Recorder's musical photo sending screen: it shows the photo of the player's last musical with the buttons
// to go back or to send it to the Global Link, and sends it. The name is pokebw2's guess: the ROM gives none. Unlike
// Black 2's, it loads only the photo's overlay

#include "types.h"
#include "app/battle_recorder/br_musical_send_proc.h"
#include "app/battle_recorder/br_btn.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_graphic.h"
#include "app/battle_recorder/br_net.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_sidebar.h"
#include "app/battle_recorder/br_util.h"
#include "app/musical/mus_shot_photo.h"
#include "constants/sound.h"
#include "gfl/clact.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "save/save_control.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/printsys.h"

// The buttons on the sub screen
enum {
    BTN_RETURN,
    BTN_SEND,
    BTN_MAX,
};

// The button graphics, from br_res.c
#define BR_RES_OBJ_MUSICAL_SEND_BTN 6
#define BR_RES_BG_MUSICAL_SEND 0x17

typedef struct {
    MusShotPhoto *photo;
    BrBtn *btn[BTN_MAX];
    BrSeq *seq;
    BrMsgWin *text;
    // For the main and the sub screen
    BrBallEffect *ballEff[2];
    BmpOamSys *bmpoam;
    PrintQueue *que;
    HeapID heapId;
    MusicalShot *shot;
    u32 cnt;
    BrMusicalSendProcParam *param;
} BrMusicalSendWork;

static BOOL func_ov184_021f8480(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov184_021f857c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov184_021f851c(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov184_021f85dc(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov184_021f8624(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov184_021f8670(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov184_021f86ec(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov184_021f88a4(BrMusicalSendWork *wk, BrMusicalSendProcParam *param);
static void func_ov184_021f88e0(BrMusicalSendWork *wk, BrMusicalSendProcParam *param);
static void func_ov184_021f891c(BrMusicalSendWork *wk, BrMusicalSendProcParam *param);
static void func_ov184_021f89d0(BrMusicalSendWork *wk, BrMusicalSendProcParam *param);

// pokebw2's data_ov184_021f8a04
const GameProcFunctions data_ov184_021f8a04 = {
    func_ov184_021f8480,
    func_ov184_021f857c,
    func_ov184_021f851c,
};

// pokebw2's BrMusicalSend_ProcInit
static BOOL func_ov184_021f8480(GameProc *proc, u32 *state, void *param, void *work) {
    BrMusicalSendProcParam *p = param;
    BrMusicalSendWork *wk;
    ClActUnit *unit;
    int i;

    GFL_OvlLoad(OVERLAY_ID(112));

    wk = GFL_ProcInitSubsystem(proc, sizeof(BrMusicalSendWork), func_ov186_021f9b9c(p->procSys));
    MI_CpuFill8(wk, 0, sizeof(BrMusicalSendWork));
    wk->heapId = func_ov186_021f9b9c(p->procSys);
    wk->param = p;

    wk->que = func_0201c440(wk->heapId);
    unit = func_ov186_021fc4dc(p->graphic);
    wk->bmpoam = func_02025538(wk->heapId, unit);
    for (i = 0; i < 2; i++) {
        wk->ballEff[i] = func_ov186_021fd760(unit, p->res, i, wk->heapId);
    }
    wk->shot = func_0200a640(getAddressOfMusicalDataInfo(GameData_GetSaveControl(p->gameData)));

    func_ov184_021f88a4(wk, p);
    func_ov184_021f891c(wk, p);

    wk->seq = func_ov186_021fd6d8(wk, func_ov184_021f85dc, wk->heapId);
    return TRUE;
}

// pokebw2's BrMusicalSend_ProcExit
static BOOL func_ov184_021f851c(GameProc *proc, u32 *state, void *param, void *work) {
    BrMusicalSendProcParam *p = param;
    BrMusicalSendWork *wk = work;
    int i;

    func_ov186_021fd714(wk->seq);
    func_ov184_021f89d0(wk, p);
    func_ov184_021f88e0(wk, p);
    for (i = 0; i < 2; i++) {
        func_ov186_021fd820(wk->ballEff[i]);
    }
    if (wk->text != NULL) {
        func_ov186_021fd15c(wk->text, p->res);
        wk->text = NULL;
    }
    func_02025588(wk->bmpoam);
    func_0201c4c0(wk->que);

    GFL_ProcReleaseSubsystem(proc);
    GFL_OvlUnload(OVERLAY_ID(112));
    return TRUE;
}

// pokebw2's BrMusicalSend_ProcMain
static BOOL func_ov184_021f857c(GameProc *proc, u32 *state, void *param, void *work) {
    BrMusicalSendWork *wk = work;
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
    if (wk->text != NULL) {
        func_ov186_021fd1bc(wk->text);
    }
    if (wk->photo != NULL) {
        func_ov112_021f3c50(wk->photo);
    }
    return FALSE;
}

// pokebw2's BrMusicalSend_SeqFadeIn: fades in, then waits for a button
static void func_ov184_021f85dc(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_NEXT,
    };
    BrMusicalSendWork *wk = p_wk_adrs;

    switch (*p_seq) {
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
        func_ov186_021fd73c(p_seqwk, func_ov184_021f8670);
        break;
    }
}

// pokebw2's BrMusicalSend_SeqReturn: fades out and goes back to the menu
static void func_ov184_021f8624(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_END,
    };
    BrMusicalSendWork *wk = p_wk_adrs;

    switch (*p_seq) {
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_MASTER_BRIGHT_AND_ALPHA, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
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

// pokebw2's BrMusicalSend_SeqTouch: waits for the player to touch a button
static void func_ov184_021f8670(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrMusicalSendWork *wk = p_wk_adrs;
    u32 x;
    u32 y;

    if (func_02035d20(&x, &y)) {
        if (func_ov186_021fb4b4(wk->btn[BTN_RETURN], x, y)) {
            BrPoint pos;

            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
            wk->param->isSend = FALSE;
            func_ov186_021fd73c(p_seqwk, func_ov184_021f8624);
        }
        if (func_ov186_021fb4b4(wk->btn[BTN_SEND], x, y)) {
            BrPoint pos;

            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
            wk->param->isSend = TRUE;
            func_ov186_021fd73c(p_seqwk, func_ov184_021f86ec);
        }
    }
}

// pokebw2's BrMusicalSend_SeqSend: sends the photo, shows the result and goes back to the menu
static void func_ov184_021f86ec(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    enum {
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_CHANGE_SCREEN,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_SEND_START,
        SEQ_SEND_WAIT,
        SEQ_RESULT,
        SEQ_TOUCH_WAIT,
        SEQ_END_FADEOUT_START,
        SEQ_END_FADEOUT_WAIT,
        SEQ_END,
    };
    BrMusicalSendWork *wk = p_wk_adrs;

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
    case SEQ_CHANGE_SCREEN: {
        BrPoint pos;

        func_ov184_021f89d0(wk, wk->param);
        func_ov184_021f88e0(wk, wk->param);
        wk->text = func_ov186_021fd12c(wk->param->res, wk->que, wk->heapId);
        func_ov186_021fd170(wk->text, wk->param->res, 0x145);
        pos.x = 0x80;
        pos.y = 0x60;
        func_ov186_021fd860(wk->ballEff[CLACT_SURFACE_MAIN], BR_BALL_EFFECT_ROT, &pos);
        *p_seq = SEQ_FADEIN_START;
        break;
    }
    case SEQ_FADEIN_START:
        func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_MASTER_BRIGHT_AND_ALPHA, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *p_seq = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_SEND_START;
        }
        break;
    case SEQ_SEND_START: {
        BrNetRequestParam req;

        MI_CpuFill8(&req, 0, sizeof(BrNetRequestParam));
        req.data = wk->shot;
        func_ov186_021ff54c(wk->param->net, 0, &req);
        wk->cnt = 0;
        *p_seq = SEQ_SEND_WAIT;
        break;
    }
    case SEQ_SEND_WAIT:
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
            *p_seq = SEQ_END_FADEOUT_START;
        } else {
            func_ov186_021fd170(wk->text, wk->param->res, msgID);
            *p_seq = SEQ_TOUCH_WAIT;
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
            *p_seq = SEQ_END_FADEOUT_START;
        }
        break;
    case SEQ_END_FADEOUT_START:
        func_ov186_021fe784(wk->param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *p_seq = SEQ_END_FADEOUT_WAIT;
        break;
    case SEQ_END_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(wk->param->fade)) {
            *p_seq = SEQ_END;
        }
        break;
    case SEQ_END:
        func_ov186_021fd744(p_seqwk);
        func_ov186_021f9ba0(wk->param->procSys);
        break;
    }
}

// pokebw2's BrMusicalSend_CreatePhoto: shows the photo on the main screen in place of its BG and sidebars
static void func_ov184_021f88a4(BrMusicalSendWork *wk, BrMusicalSendProcParam *param) {
    if (wk->photo == NULL) {
        func_ov186_021feec0(param->sidebar, CLACT_SURFACE_MAIN, FALSE);
        func_ov186_021fbda4(param->res, 0);
        func_ov186_021fc1f8(param->res, CLACT_VRAM_MAIN);
        func_ov186_021fc610(param->graphic, wk->heapId);
        wk->photo = func_ov112_021f3af0(wk->shot, wk->heapId);
    }
}

// pokebw2's BrMusicalSend_DeletePhoto
static void func_ov184_021f88e0(BrMusicalSendWork *wk, BrMusicalSendProcParam *param) {
    if (wk->photo != NULL) {
        func_ov112_021f3bf0(wk->photo);
        wk->photo = NULL;
        func_ov186_021fc640(param->graphic, wk->heapId);
        func_ov186_021fc148(param->res, CLACT_VRAM_MAIN, wk->heapId);
        func_ov186_021fb964(param->res, 0, wk->heapId);
        func_ov186_021feec0(param->sidebar, CLACT_SURFACE_MAIN, TRUE);
    }
}

// pokebw2's BrMusicalSend_CreateButtons
static void func_ov184_021f891c(BrMusicalSendWork *wk, BrMusicalSendProcParam *param) {
    ClActUnit *unit;
    Font *font;
    MsgData *msg;
    int i;
    u32 msgID;
    ClActorSetup setup;
    BrResObjData obj;

    if (wk->btn[0] == NULL) {
        func_ov186_021fb964(param->res, BR_RES_BG_MUSICAL_SEND, wk->heapId);
        func_ov186_021fbe54(param->res, BR_RES_OBJ_MUSICAL_SEND_BTN, wk->heapId);
        font = func_ov186_021fc20c(param->res);
        msg = func_ov186_021fc214(param->res);

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.y = 168;
        setup.priority = 1;
        unit = func_ov186_021fc4dc(param->graphic);
        func_ov186_021fc124(param->res, BR_RES_OBJ_MUSICAL_SEND_BTN, &obj);

        for (i = 0; i < BTN_MAX; i++) {
            if (i == BTN_RETURN) {
                msgID = 4;
                setup.x = 32;
                setup.sequence = 0;
            } else {
                msgID = 43;
                setup.x = 128;
                setup.sequence = 4;
            }
            wk->btn[i] = func_ov186_021fb378(&setup, msgID, 96, CLACT_SURFACE_SUB, unit, wk->bmpoam, font, msg, &obj,
                                             wk->heapId);
        }
    }
}

// pokebw2's BrMusicalSend_DeleteButtons
static void func_ov184_021f89d0(BrMusicalSendWork *wk, BrMusicalSendProcParam *param) {
    int i;

    if (wk->btn[0] != NULL) {
        func_ov186_021fc0c8(param->res, BR_RES_OBJ_MUSICAL_SEND_BTN);
        func_ov186_021fbda4(param->res, BR_RES_BG_MUSICAL_SEND);
        for (i = 0; i < BTN_MAX; i++) {
            func_ov186_021fb494(wk->btn[i]);
            wk->btn[i] = NULL;
        }
    }
}
