// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov190/wifilogout.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// wifilogout.c, in overlay 190: the process that logs out of the Nintendo Wi-Fi Connection. The ROM has no file name for
// it, so it is named after its process (our name)

#include "app/wifi_login.h"
#include "types.h"
#include "app/wifilogin_disp.h"
#include "app/wifilogin_message.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_state.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "system/app_taskmenu.h"
#include "system/bmp_menu.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/wifi_error.h"

typedef struct WifiLogoutWork WifiLogoutWork;
typedef void (*WifiLogoutStateFunc)(WifiLogoutWork *work);

struct WifiLogoutWork {
    // The yes/no menu, which the logout never opens
    WifiLoginMessageMenu *menu;
    WifiLoginDisp *disp;
    WifiLoginMessage *msg;
    WifiLogoutStateFunc state;
    WifiLogoutParam *param;
    HeapID heapId;
    // Set by the network library's callback once the connection is closed
    BOOL disconnected;
    u8 unk1C[4];
};

void func_ov115_021d6130(WifiLogoutWork *work, WifiLogoutStateFunc state);
void func_ov115_021d6134(WifiLogoutWork *work, WifiLogoutStateFunc state, int line);
static BOOL func_ov115_021d613c(WifiLogoutWork *work);
void func_ov115_021d6150(WifiLogoutWork *work);
void func_ov115_021d6198(WifiLogoutWork *work);
void func_ov115_021d61b8(WifiLogoutWork *work);
void func_ov115_021d61d8(WifiLogoutWork *work);
static void func_ov115_021d61fc(WifiLogoutWork *work);
void func_ov115_021d625c(WifiLogoutWork *work);
void func_ov115_021d6274(WifiLogoutWork *work);
void func_ov115_021d6294(WifiLogoutWork *work);
void func_ov115_021d62b4(WifiLogoutWork *work);
void func_ov115_021d630c(WifiLogoutWork *work);
void func_ov115_021d6328(void *work);
BOOL func_ov115_021d603c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov115_021d6104(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov115_021d60d4(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's WIFILOGOUT_PROC_FUNCTIONS
const GameProcFunctions data_ov115_021d654c = {
    func_ov115_021d603c,
    func_ov115_021d6104,
    func_ov115_021d60d4,
};

// pokebw2's WifiLogoutProc_Main


BOOL func_ov115_021d603c(GameProc *proc, u32 *state, void *param, void *work) {
    WifiLogoutParam *logoutParam = param;
    WifiLogoutWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, 0x79, 6 << 14);
    wk = GFL_ProcInitSubsystem(proc, sizeof(WifiLogoutWork), 0x79);
    MI_CpuFill8(wk, 0, sizeof(WifiLogoutWork));
    wk->param = logoutParam;
    wk->heapId = 0x79;
    if (logoutParam->unk10 == 0) {
        logoutParam->unk10 = 6;
    }
    wk->disp = func_ov115_021d4d38(wk->heapId, logoutParam->unk4, logoutParam->unk8);
    wk->msg = func_ov115_021d5984(wk->heapId, 0xe8, logoutParam->unk8);
    func_ov115_021d6134(wk, func_ov115_021d6150, 173);
    if (logoutParam->unk4 == 0) {
        func_ov115_021d5ccc(wk->msg, 36);
        if (logoutParam->unkC == 0) {
            GFL_SndBGMPlay(0x481, 0xffff);
        }
    }
    if (logoutParam->unk8 == 0) {
        func_0203d34c(FALSE, wk->heapId);
    } else {
        func_0203d34c(TRUE, wk->heapId);
    }
    return TRUE;
}

BOOL func_ov115_021d60d4(GameProc *proc, u32 *state, void *param, void *work) {
    WifiLogoutParam *logoutParam = param;
    WifiLogoutWork *wk = work;

    if (logoutParam->unk4 == 0) {
        func_ov115_021d5d3c(wk->msg);
    }
    func_ov115_021d5a9c(wk->msg);
    func_ov115_021d4e04(wk->disp);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x79);
    return TRUE;
}

static BOOL func_ov115_021d6104(GameProc *proc, u32 *state, void *param, void *work) {
    WifiLogoutWork *wk = work;
    BOOL done;

    if (wk->menu != NULL) {
        func_ov115_021d5e48(wk->menu);
    }
    func_ov115_021d4df4(wk->disp);
    func_ov115_021d5a88(wk->msg);
    done = FALSE;
    if (func_ov115_021d613c(wk)) {
        done = TRUE;
    }
    return done;
}

// pokebw2's WifiLogout_SetState
void func_ov115_021d6130(WifiLogoutWork *work, WifiLogoutStateFunc state) {
    work->state = state;
}

// Every state change gives the line it is on
// pokebw2's WifiLogout_ChangeState
void func_ov115_021d6134(WifiLogoutWork *work, WifiLogoutStateFunc state, int line) {
    func_ov115_021d6130(work, state);
}

// Runs the state, returning whether it has ended
// pokebw2's WifiLogout_RunState
static BOOL func_ov115_021d613c(WifiLogoutWork *work) {
    if (work->state != NULL) {
        work->state(work);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WifiLogout_StateStartFadeIn
void func_ov115_021d6150(WifiLogoutWork *work) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN,
                (work->param->unk10 & 8) ? WIPE_COLOR_WHITE : WIPE_COLOR_BLACK, 6, 1, work->heapId);
    func_ov115_021d6134(work, func_ov115_021d6198, 347);
}

// pokebw2's WifiLogout_StateWaitFadeIn
void func_ov115_021d6198(WifiLogoutWork *work) {
    if (GFL_WipeIsFinished()) {
        func_ov115_021d6134(work, func_ov115_021d61b8, 361);
    }
}

// pokebw2's WifiLogout_StatePrintDisconnecting
void func_ov115_021d61b8(WifiLogoutWork *work) {
    func_ov115_021d5c24(work->msg, WifiError_Text_026);
    func_ov115_021d6134(work, func_ov115_021d61d8, 0x65 << 2);
}

// pokebw2's WifiLogout_StateWaitDisconnecting
void func_ov115_021d61d8(WifiLogoutWork *work) {
    if (func_ov115_021d5c70(work->msg)) {
        func_ov115_021d6134(work, func_ov115_021d61fc, 418);
    }
}

static void func_ov115_021d61fc(WifiLogoutWork *work) {
    if (func_0200fb24()) {
        func_0203cf04();
        func_0203d0d0();
        func_0203c968();
        func_0203ca04();
        func_0200fba0();
    }
    if (func_0203cdbc()) {
        func_ov115_021d6134(work, func_ov115_021d6274, 447);
    } else if (func_0203ceac(func_ov115_021d6328)) {
        func_ov115_021d6134(work, func_ov115_021d625c, 455);
    }
}


// pokebw2's WifiLogout_StateWaitDisconnected
void func_ov115_021d625c(WifiLogoutWork *work) {
    if (work->disconnected) {
        func_ov115_021d6134(work, func_ov115_021d6274, 0x76 << 2);
    }
}

// pokebw2's WifiLogout_StatePrintDisconnected
void func_ov115_021d6274(WifiLogoutWork *work) {
    func_ov115_021d5c24(work->msg, WifiError_Text_027);
    func_ov115_021d6134(work, func_ov115_021d6294, 486);
}

// pokebw2's WifiLogout_StateWaitDisconnectedMessage
void func_ov115_021d6294(WifiLogoutWork *work) {
    if (func_ov115_021d5c70(work->msg)) {
        func_ov115_021d6134(work, func_ov115_021d62b4, 0x7d << 2);
    }
}

// pokebw2's WifiLogout_StateStartFadeOut
void func_ov115_021d62b4(WifiLogoutWork *work) {
    u32 color = (work->param->unk10 & 0x10) ? WIPE_COLOR_WHITE : WIPE_COLOR_BLACK;

    if (work->param->unkC == 0) {
        GFL_SndBGMFadeOut(6);
    }
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, color, 6, 1, work->heapId);
    func_ov115_021d6134(work, func_ov115_021d630c, 531);
}

// pokebw2's WifiLogout_StateWaitFadeOut
void func_ov115_021d630c(WifiLogoutWork *work) {
    if (GFL_WipeIsFinished()) {
        func_ov115_021d6134(work, NULL, 545);
    }
}

// pokebw2's WifiLogout_OnDisconnected
void func_ov115_021d6328(void *work) {
    WifiLogoutWork *wk = work;

    wk->disconnected = TRUE;
}
