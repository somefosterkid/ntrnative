// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_menu_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's menu screen: it runs one of br_btn.c's button menus until the player chooses a screen or goes
// back, and changes the Battle Recorder's color when the player touches a sidebar. The name is a guess: the ROM gives
// none, and br_core.c runs it as BR_PROCID_MENU

#include "types.h"
#include "app/battle_recorder/br_btn.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "constants/sound.h"
#include "gfl/clact.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/proc.h"

#define BR_MENU_COLOR_NUM 7

typedef struct {
    BrBtnSys *btnSys;
    BrBallEffect *ballEff;
    TCB *vblankTcb;
    BrMenuProcParam *param;
    // Set by the VBlank task when it has loaded the new color
    BOOL isColorLoaded;
    HeapID heapId;
} BrMenuWork;

static BOOL func_ov183_021db698(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021db740(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021db770(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov183_021db924(TCB *tcb, void *data);

// pokebw2's BR_MENU_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e3274 = {
    func_ov183_021db698,
    func_ov183_021db770,
    func_ov183_021db740,
};

// pokebw2's BrMenu_ProcInit
static BOOL func_ov183_021db698(GameProc *proc, u32 *state, void *param, void *work) {
    BrMenuProcParam *p = param;
    BrMenuWork *wk;

    wk = GFL_ProcInitSubsystem(proc, sizeof(BrMenuWork), func_ov186_021f9b9c(p->procSys));
    MI_CpuFill8(wk, 0, sizeof(BrMenuWork));
    wk->param = p;
    wk->heapId = func_ov186_021f9b9c(p->procSys);

    func_ov186_021fc1f8(p->res, CLACT_SURFACE_MAIN);
    func_ov186_021fc1f8(p->res, CLACT_SURFACE_SUB);
    func_ov186_021fc148(p->res, CLACT_SURFACE_MAIN, wk->heapId);
    func_ov186_021fc148(p->res, CLACT_SURFACE_SUB, wk->heapId);
    if (p->menuID < 5) {
        func_ov186_021fb964(p->res, 3, wk->heapId);
    } else {
        func_ov186_021fb964(p->res, 4, wk->heapId);
    }

    wk->ballEff = func_ov186_021fd760(p->unit, p->res, CLACT_SURFACE_SUB, wk->heapId);
    wk->btnSys = func_ov186_021f9cb8(p->menuID, p->unit, p->res, p->recordInfo, p->btnRecovery, wk->ballEff, wk->heapId);

    if (p->fadeType == BR_FADE_TYPE_MASTER_BRIGHT_AND_ALPHA) {
        func_ov186_021fe840(p->fade, BR_FADE_DISPLAY_SUB, 0);
    } else {
        func_ov186_021fe840(p->fade, BR_FADE_DISPLAY_BOTH, 0);
    }
    return TRUE;
}

// pokebw2's BrMenu_ProcExit
static BOOL func_ov183_021db740(GameProc *proc, u32 *state, void *param, void *work) {
    BrMenuProcParam *p = param;
    BrMenuWork *wk = work;

    if (p->menuID < 5) {
        func_ov186_021fbda4(p->res, 3);
    } else {
        func_ov186_021fbda4(p->res, 4);
    }
    func_ov186_021f9f48(wk->btnSys);
    func_ov186_021fd820(wk->ballEff);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrMenu_ProcMain
static BOOL func_ov183_021db770(GameProc *proc, u32 *state, void *param, void *work) {
    enum {
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_MAIN,
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_END,
        SEQ_COLOR_FADEOUT_START,
        SEQ_COLOR_FADEOUT_WAIT,
        SEQ_COLOR_LOAD_START,
        SEQ_COLOR_LOAD_WAIT,
        SEQ_COLOR_FADEIN_START,
        SEQ_COLOR_FADEIN_WAIT,
    };
    // The colors, in the order that touching a sidebar goes through them
    static const u32 data_ov183_021e3280[BR_MENU_COLOR_NUM] = { 0, 2, 3, 4, 5, 6, 7 };
    BrMenuProcParam *p = param;
    BrMenuWork *wk = work;

    switch (*state) {
    case SEQ_FADEIN_START:
        func_ov186_021fe784(p->fade, p->fadeType, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *state = SEQ_FADEIN_WAIT;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p->fade)) {
            *state = SEQ_MAIN;
        }
        break;
    case SEQ_MAIN:
        // Only the top menus change the color, when a sidebar is touched
        if (p->menuID <= 4 && !func_ov186_021fa018(wk->btnSys)) {
            u32 x;
            u32 y;

            if (func_02035d20(&x, &y) && (x < 8 || x > 248)) {
                GFL_SndSEPlay(SEQ_SE_BREC_06);
                *state = SEQ_COLOR_FADEOUT_START;
                break;
            }
        }
        func_ov186_021f9fe8(wk->btnSys);
        if (func_ov186_021fa004(wk->btnSys, &p->unk4, &p->nextMode) != 0) {
            *state = SEQ_FADEOUT_START;
        }
        break;
    case SEQ_FADEOUT_START:
        if (p->unk4 == BR_PROCID_MUSICAL_SEND) {
            func_ov186_021fe784(p->fade, BR_FADE_TYPE_MASTER_BRIGHT_AND_ALPHA, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        } else {
            func_ov186_021fe784(p->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        }
        *state = SEQ_FADEOUT_WAIT;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p->fade)) {
            *state = SEQ_END;
        }
        break;
    case SEQ_END:
        switch (func_ov186_021fa004(wk->btnSys, &p->unk4, &p->nextMode)) {
        case 1:
            func_ov186_021f9bf4(p->procSys, p->unk4);
            return TRUE;
        case 2:
            func_ov186_021f9ba0(p->procSys);
            *p->result = func_ov186_021fa01c(wk->btnSys);
            return TRUE;
        }
        break;
    case SEQ_COLOR_FADEOUT_START: {
        u32 color;
        u32 i;

        color = func_ov186_021fc238(p->res);
        for (i = 0; i < BR_MENU_COLOR_NUM; i++) {
            if (color == data_ov183_021e3280[i]) {
                color = data_ov183_021e3280[(i + 1) % BR_MENU_COLOR_NUM];
                break;
            }
        }
        func_ov186_021fc230(p->res, color);
        func_ov186_021fe7f4(p->fade, func_ov186_021fc224(p->res));
        func_ov186_021fe784(p->fade, BR_FADE_TYPE_PLTT, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        *state = SEQ_COLOR_FADEOUT_WAIT;
        break;
    }
    case SEQ_COLOR_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p->fade)) {
            *state = SEQ_COLOR_LOAD_START;
        }
        break;
    case SEQ_COLOR_LOAD_START:
        wk->vblankTcb = GFL_VBlankTCBAdd(func_ov183_021db924, wk, 0);
        wk->isColorLoaded = FALSE;
        *state = SEQ_COLOR_LOAD_WAIT;
        break;
    case SEQ_COLOR_LOAD_WAIT:
        if (wk->isColorLoaded) {
            GFL_TCBRemove(wk->vblankTcb);
            *state = SEQ_COLOR_FADEIN_START;
        }
        break;
    case SEQ_COLOR_FADEIN_START:
        func_ov186_021fe784(p->fade, BR_FADE_TYPE_PLTT, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        *state = SEQ_COLOR_FADEIN_WAIT;
        break;
    case SEQ_COLOR_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p->fade)) {
            *state = SEQ_MAIN;
        }
        break;
    }

    func_ov186_021fd854(wk->ballEff);
    return FALSE;
}

// Loads the palettes of the new color, once
// pokebw2's BrMenu_VBlankLoadColor
static void func_ov183_021db924(TCB *tcb, void *data) {
    BrMenuWork *wk = data;
    BrMenuProcParam *p = wk->param;

    if (!wk->isColorLoaded) {
        func_ov186_021fc240(p->res, p->fade, wk->heapId);
    }
    wk->isColorLoaded = TRUE;
}
