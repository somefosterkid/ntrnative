// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_codein_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's screen for entering a battle video's number: it runs br_codein.c's code input until the
// player confirms a number, which the record screen then looks up, or goes back. The name is a guess: the ROM gives
// none, and br_core.c runs it as BR_PROCID_CODEIN

#include "types.h"
#include "app/battle_recorder/br_codein.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "gfl/clact.h"
#include "gfl/std.h"
#include "system/printsys.h"
#include "text/system/0008.h"
#include "gfl/proc.h"

// The digits of a battle video's number
#define BR_CODEIN_VIDEO_NUMBER_LENGTH 12

typedef struct {
    BrCodeInParam *codeInParam;
    BrCodeInWork *codeIn;
    BrMsgWin *text;
    PrintQueue *que;
    HeapID heapId;
    BrBallEffect *ballEff;
} BrCodeInProcWork;

static BOOL func_ov183_021e050c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021e05c4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021e0618(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's BR_CODEIN_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e3844 = {
    func_ov183_021e050c,
    func_ov183_021e0618,
    func_ov183_021e05c4,
};

// pokebw2's BrCodeInProc_Init
static BOOL func_ov183_021e050c(GameProc *proc, u32 *state, void *param, void *work) {
    BrCodeInProcParam *p = param;
    BrCodeInProcWork *wk;
    int blocks[BR_CODEIN_BLOCK_COUNT];

    func_ov186_021fc20c(p->res);
    func_ov186_021fc214(p->res);
    wk = GFL_ProcInitSubsystem(proc, sizeof(BrCodeInProcWork), func_ov186_021f9b9c(p->procSys));
    MI_CpuFill8(wk, 0, sizeof(BrCodeInProcWork));
    wk->heapId = func_ov186_021f9b9c(p->procSys);

    func_ov186_021fb964(p->res, 29, wk->heapId);
    func_ov186_021fbe54(p->res, 11, wk->heapId);
    func_ov186_021fbe54(p->res, 12, wk->heapId);
    func_ov186_021fbe54(p->res, 6, wk->heapId);

    wk->ballEff = func_ov186_021fd760(p->unit, p->res, CLACT_SURFACE_SUB, wk->heapId);
    blocks[0] = 2;
    blocks[1] = 5;
    blocks[2] = 5;
    wk->codeInParam =
        func_ov183_021e07a4(wk->heapId, BR_CODEIN_VIDEO_NUMBER_LENGTH, p->unit, p->res, wk->ballEff, blocks);
    wk->codeIn = func_ov183_021e06d0(wk->codeInParam, wk->heapId);

    wk->que = func_0201c440(wk->heapId);
    wk->text = func_ov186_021fd12c(p->res, wk->que, wk->heapId);
    func_ov186_021fd170(wk->text, p->res, Bank0008_Text_104);
    return TRUE;
}

// pokebw2's BrCodeInProc_Exit
static BOOL func_ov183_021e05c4(GameProc *proc, u32 *state, void *param, void *work) {
    BrCodeInProcParam *p = param;
    BrCodeInProcWork *wk = work;

    func_ov186_021fd15c(wk->text, p->res);
    func_ov183_021e0774(wk->codeIn);
    func_ov183_021e0808(wk->codeInParam);
    func_0201c4c0(wk->que);
    func_ov186_021fd820(wk->ballEff);
    func_ov186_021fbda4(p->res, 29);
    func_ov186_021fc0c8(p->res, 6);
    func_ov186_021fc0c8(p->res, 12);
    func_ov186_021fc0c8(p->res, 11);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrCodeInProc_Main
static BOOL func_ov183_021e0618(GameProc *proc, u32 *state, void *param, void *work) {
    enum {
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_MAIN,
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_END,
    };
    BrCodeInProcParam *p = param;
    BrCodeInProcWork *wk = work;

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
    case SEQ_MAIN:
        func_ov183_021e0754(wk->codeIn);
        switch (func_ov183_021e075c(wk->codeIn)) {
        case BR_CODEIN_RESULT_CANCEL:
            func_ov186_021f9ba0(p->procSys);
            *state = SEQ_FADEOUT_START;
            break;
        case BR_CODEIN_RESULT_DECIDE:
            p->videoNumber = func_ov183_021e0764(wk->codeIn);
            func_ov186_021f9bf4(p->procSys, BR_PROCID_RECORD);
            *state = SEQ_FADEOUT_START;
            break;
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
        return TRUE;
    }

    if (wk->text != NULL) {
        func_ov186_021fd1bc(wk->text);
    }
    func_0201c4e4(wk->que);
    func_ov186_021fd854(wk->ballEff);
    return FALSE;
}
