// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov199/pdwacc_state.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Dream World account screens, which the start menu's Game Sync settings start: they create the Game Sync account
// and show its ID, or only show the ID. The ROM doesn't name this file; the name is pdwacc_message.c's and
// pdwacc_disp.c's counterpart of gsync_state.c. Function names are ours.

#include "types.h"
#include "app/gsync.h"
#include "app/gsync/pdwacc_disp.h"
#include "app/gsync/pdwacc_message.h"
#include "constants/sound.h"
#include "gfl/nhttp_rap.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "save/dream_world.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/app_taskmenu.h"
#include "system/game_data.h"
#include "system/wipe.h"
#include "dwc/nhttp.h"

// Errors of the account screens' own, beside the server's
#define PDWACC_ERROR_SERVICE_UNAVAILABLE 0xfff1
#define PDWACC_ERROR_BAD_GATEWAY 0xfff2
#define PDWACC_ERROR_HTTP 0xfff3

// The request that creates the account, sending the save data
#define PDWACC_REQUEST_CREATE 9

typedef struct PdwAccWork PdwAccWork;
typedef void (*PdwAccStateFunc)(PdwAccWork *wk);

struct PdwAccWork {
    HeapID heapId;
    void *unk4;
    NHttpRap *http;
    PdwAccDisp *disp;
    PdwAccMessage *msg;
    AppTaskMenu *yesNo;
    u32 unk18;
    SaveControl *save;
    GameData *gameData;
    void *unk24;
    s32 profileId;
    u32 unk2C;
    PdwAccStateFunc state;
    u8 unk34[0x38];
    BOOL saving;
    u32 error;
};

BOOL func_ov123_021d9fd4(PdwAccWork *wk, int status);
static void func_ov123_021da030(PdwAccWork *wk, PdwAccStateFunc state);
static void func_ov123_021da034(PdwAccWork *wk, PdwAccStateFunc state, int line);
static void func_ov123_021da03c(PdwAccWork *wk);
static void func_ov123_021da054(PdwAccWork *wk);
void func_ov123_021da090(PdwAccWork *wk);
void func_ov123_021da0b8(PdwAccWork *wk);
void func_ov123_021da0e4(PdwAccWork *wk);
void func_ov123_021da110(PdwAccWork *wk);
void func_ov123_021da138(PdwAccWork *wk);
void func_ov123_021da16c(PdwAccWork *wk);
void func_ov123_021da1a0(PdwAccWork *wk);
static void func_ov123_021da1c8(PdwAccWork *wk);
static void func_ov123_021da1ec(PdwAccWork *wk);
void func_ov123_021da244(PdwAccWork *wk);
void func_ov123_021da268(PdwAccWork *wk);
void func_ov123_021da290(PdwAccWork *wk);
void func_ov123_021da2dc(PdwAccWork *wk);
void func_ov123_021da308(PdwAccWork *wk);
void func_ov123_021da328(PdwAccWork *wk);
void func_ov123_021da3a0(PdwAccWork *wk);
void func_ov123_021da408(PdwAccWork *wk);
void func_ov123_021da434(PdwAccWork *wk);
void func_ov123_021da45c(void *work, int a1, int code, int error);
BOOL func_ov123_021da478(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov123_021da54c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov123_021da5d0(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's PDWACC_PROC_FUNCTIONS
const GameProcFunctions data_ov123_021da9a8 = {
    func_ov123_021da478,
    func_ov123_021da54c,
    func_ov123_021da5d0,
};

// Returns whether the HTTP status is an error, and shows it
// pokebw2's PdwAcc_CheckHttpStatus
BOOL func_ov123_021d9fd4(PdwAccWork *wk, int status) {
    switch (status) {
    case 503:
        wk->error = PDWACC_ERROR_SERVICE_UNAVAILABLE;
        func_ov123_021da034(wk, func_ov123_021da1ec, 153);
        return TRUE;
    case 502:
        wk->error = PDWACC_ERROR_BAD_GATEWAY;
        func_ov123_021da034(wk, func_ov123_021da1ec, 157);
        return TRUE;
    default:
        if (status >= 400) {
            wk->error = PDWACC_ERROR_HTTP;
            func_ov123_021da034(wk, func_ov123_021da1ec, 163);
            return TRUE;
        }
        return FALSE;
    }
}

// pokebw2's PdwAcc_SetState
static void func_ov123_021da030(PdwAccWork *wk, PdwAccStateFunc state) {
    wk->state = state;
}

// Changes the state, from a line that the debug build reported
// pokebw2's PdwAcc_ChangeState
static void func_ov123_021da034(PdwAccWork *wk, PdwAccStateFunc state, int line) {
    func_ov123_021da030(wk, state);
}

// pokebw2's PdwAcc_StateWaitFadeOut
static void func_ov123_021da03c(PdwAccWork *wk) {
    if (GFL_WipeIsFinished()) {
        func_ov123_021da034(wk, NULL, 213);
    }
}

// pokebw2's PdwAcc_StateFadeOut
static void func_ov123_021da054(PdwAccWork *wk) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_WHITE, 6, 1, wk->heapId);
    GFL_SndBGMFadeOut(6);
    func_ov123_021da034(wk, func_ov123_021da03c, 231);
}

// pokebw2's PdwAcc_StateWaitSavedMessage
void func_ov123_021da090(PdwAccWork *wk) {
    if (func_ov123_021d95f0(wk->msg) && GCTX_HIDGetPressedKeys()) {
        func_ov123_021da034(wk, func_ov123_021da054, 241);
    }
}

// pokebw2's PdwAcc_StateWaitSave
void func_ov123_021da0b8(PdwAccWork *wk) {
    int result = func_02012dd0(wk->gameData);

    if (result == 2 || result == 3) {
        wk->saving = FALSE;
        func_ov123_021d9514(wk->msg, 11);
        func_ov123_021da034(wk, func_ov123_021da090, 254);
    }
}

// pokebw2's PdwAcc_StateStartSave
void func_ov123_021da0e4(PdwAccWork *wk) {
    if (func_ov123_021d95f0(wk->msg)) {
        wk->saving = TRUE;
        func_02012dac(wk->gameData);
        func_ov123_021da034(wk, func_ov123_021da0b8, 265);
    }
}

// pokebw2's PdwAcc_StateSave
void func_ov123_021da110(PdwAccWork *wk) {
    func_ov123_021d9514(wk->msg, 6);
    func_ov123_021d95b4(wk->msg);
    func_ov123_021da034(wk, func_ov123_021da0e4, 274);
}

// pokebw2's PdwAcc_StateIdNext
void func_ov123_021da138(PdwAccWork *wk) {
    if (GCTX_HIDGetPressedKeys()) {
        func_ov123_021d97b0(wk->msg);
        func_020092e0(getDreamWorldStuffAddress(wk->save), TRUE);
        func_ov123_021da034(wk, func_ov123_021da110, 282);
    }
}

// pokebw2's PdwAcc_StateIdWait
void func_ov123_021da16c(PdwAccWork *wk) {
    if (GCTX_HIDGetPressedKeys()) {
        func_ov123_021d9844(wk->msg, wk->profileId, wk->profileId);
        func_ov123_021d9730(wk->msg, 9);
        func_ov123_021da034(wk, func_ov123_021da138, 298);
    }
}

// pokebw2's PdwAcc_StateShowId
void func_ov123_021da1a0(PdwAccWork *wk) {
    func_ov123_021d9844(wk->msg, wk->profileId, wk->profileId);
    func_ov123_021d9730(wk->msg, 8);
    func_ov123_021da034(wk, func_ov123_021da16c, 316);
}

// pokebw2's PdwAcc_StateErrorWait
static void func_ov123_021da1c8(PdwAccWork *wk) {
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        func_ov123_021da034(wk, func_ov123_021da054, 331);
    }
}

// pokebw2's PdwAcc_StateError
static void func_ov123_021da1ec(PdwAccWork *wk) {
    u32 msgId = wk->error + 11;

    if (wk->http != NULL) {
        func_ov114_021b9774(wk->http);
    }
    if (wk->error == PDWACC_ERROR_SERVICE_UNAVAILABLE) {
        msgId = 22;
    } else if (wk->error == PDWACC_ERROR_BAD_GATEWAY) {
        msgId = 23;
    } else if (wk->error == PDWACC_ERROR_HTTP) {
        msgId = 20;
    } else if (wk->error >= 10) {
        msgId = 20;
    }
    func_ov123_021d9730(wk->msg, msgId);
    func_ov123_021da034(wk, func_ov123_021da1c8, 359);
}

// pokebw2's PdwAcc_StateWaitCreateMessage
void func_ov123_021da244(PdwAccWork *wk) {
    if (func_ov123_021d95f0(wk->msg)) {
        func_ov123_021da034(wk, func_ov123_021da3a0, 370);
    }
}

// pokebw2's PdwAcc_StateCreateStart
void func_ov123_021da268(PdwAccWork *wk) {
    func_ov123_021d9514(wk->msg, 3);
    func_ov123_021d95b4(wk->msg);
    func_ov123_021da034(wk, func_ov123_021da244, 379);
}

// pokebw2's PdwAcc_StateCreateAnswer
void func_ov123_021da290(PdwAccWork *wk) {
    if (func_020279e8(wk->yesNo)) {
        if (func_02027a04(wk->yesNo) == 0) {
            func_ov123_021da034(wk, func_ov123_021da268, 397);
        } else {
            func_ov123_021da034(wk, func_ov123_021da054, 400);
        }
        func_ov123_021d9634(wk->msg);
        func_02027858(wk->yesNo);
        wk->yesNo = NULL;
    }
}

// pokebw2's PdwAcc_StateCreateAsk
void func_ov123_021da2dc(PdwAccWork *wk) {
    if (func_ov123_021d95f0(wk->msg)) {
        wk->yesNo = func_ov123_021d9670(wk->msg, 0);
        func_ov123_021da034(wk, func_ov123_021da290, 422);
    }
}

// pokebw2's PdwAcc_StateStart
void func_ov123_021da308(PdwAccWork *wk) {
    func_ov123_021d9514(wk->msg, 2);
    func_ov123_021da034(wk, func_ov123_021da2dc, 439);
}

void func_ov123_021da328(PdwAccWork *wk) {
    if (func_0203cd98()) {
        if (!func_ov123_021d9fd4(wk, func_ov114_021b99dc(wk->http)) && func_ov114_021b9790(wk->http) == 0) {
            u32 *response = func_ov114_021b97f4(wk->http);

            func_ov123_021d9634(wk->msg);
            if (*response == 2) {
                func_ov123_021da034(wk, func_ov123_021da1a0, 468);
            } else if (*response == 0) {
                func_ov123_021da034(wk, func_ov123_021da1a0, 471);
            } else {
                wk->error = *response;
                func_ov123_021da034(wk, func_ov123_021da1ec, 475);
            }
        }
    }
}

BOOL func_ov114_021b95d4(u32 a0, NHttpRap *a1);
void func_ov123_021da3a0(PdwAccWork *wk) {
    u32 size;

    if (func_0203cd98()) {
        if (func_ov114_021b95d4(PDWACC_REQUEST_CREATE, wk->http)) {
            void *data = func_020071d8(wk->save, &size);

            func_ov114_021be79c(func_ov114_021b973c(wk->http), data, 0x80000);
            if (func_ov114_021b9748(wk->http) == 0) {
                func_ov123_021da034(wk, func_ov123_021da328, 502);
            }
        }
    } else if (GCTX_HIDGetPressedKeys()) {
        func_ov123_021da034(wk, func_ov123_021da328, 508);
    }
}



// pokebw2's PdwAcc_StateIdOnlyWait
void func_ov123_021da408(PdwAccWork *wk) {
    if (GFL_WipeIsFinished() && GCTX_HIDGetPressedKeys()) {
        func_ov123_021d9918(wk->msg);
        func_ov123_021da034(wk, func_ov123_021da054, 556);
    }
}

// pokebw2's PdwAcc_StateShowIdOnly
void func_ov123_021da434(PdwAccWork *wk) {
    func_ov123_021d9844(wk->msg, wk->profileId, wk->profileId);
    func_ov123_021d9730(wk->msg, 7);
    func_ov123_021da034(wk, func_ov123_021da408, 572);
}

// pokebw2's PdwAcc_OnDisconnect
void func_ov123_021da45c(void *work, int a1, int code, int error) {
    PdwAccWork *wk = work;

    if (wk->http != NULL) {
        func_ov114_021b9774(wk->http);
        func_ov114_021b9838(wk->http);
        wk->http = NULL;
    }
}

BOOL func_ov123_021da478(GameProc *proc, u32 *state, void *param, void *work) {
    PdwAccParam *pdwParam = param;
    PdwAccWork *wk;

    GFL_OvlLoad(OVERLAY_ID(114));
    wk = GFL_ProcInitSubsystem(proc, sizeof(PdwAccWork), pdwParam->heapId);
    MI_CpuFill8(wk, 0, sizeof(PdwAccWork));
    wk->heapId = pdwParam->heapId;
    wk->save = GameData_GetSaveControl(pdwParam->gameData);
    wk->gameData = pdwParam->gameData;
    wk->profileId = func_0200853c(GetGameDataPlayerInfo(pdwParam->gameData));
    if (func_0203cd98()) {
        wk->http = func_ov114_021b9800(pdwParam->heapId, wk->profileId, pdwParam->loginBuffer);
        func_ov009_02156dc4(func_ov123_021da45c, wk);
    }
    wk->disp = func_ov123_021d9958(wk->heapId);
    wk->msg = func_ov123_021d92cc(wk->heapId, 0x57);
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, wk->heapId);
    GFL_SndBGMPlay(SEQ_BGM_GAME_SYNC, SND_CHANNEL_MASK_ALL);
    switch (pdwParam->mode) {
    case PDWACC_MODE_CREATE:
        func_ov123_021da034(wk, func_ov123_021da308, 630);
        break;
    case PDWACC_MODE_SHOW_ID:
        func_ov123_021da034(wk, func_ov123_021da434, 633);
        break;
    }
    pdwParam->disconnected = FALSE;
    return TRUE;
}


// pokebw2's PdwAcc_ProcMain
static BOOL func_ov123_021da54c(GameProc *proc, u32 *state, void *param, void *work) {
    PdwAccParam *pdwParam = param;
    PdwAccWork *wk = work;
    PdwAccStateFunc func = wk->state;
    BOOL done = TRUE;

    if (func != NULL) {
        if (!func_0200fb24()) {
            func(wk);
        }
        done = FALSE;
    }
    if (wk->yesNo != NULL) {
        func_02027974(wk->yesNo);
    }
    func_ov123_021d9a14(wk->disp);
    func_ov123_021d9404(wk->msg);
    if (GFL_WipeIsFinished()) {
        if (func_0200fb24()) {
            func_ov114_021b9774(wk->http);
            if (wk->saving) {
                func_02012e08(wk->gameData);
                wk->saving = FALSE;
            }
            func_0202223c(0, WIPE_COLOR_BLACK);
            func_0202223c(1, WIPE_COLOR_BLACK);
            func_ov009_02157170(1, 0);
            pdwParam->disconnected = TRUE;
            done = TRUE;
        }
    } else if (func_0200fb24()) {
        done = TRUE;
    }
    return done;
}

// pokebw2's PdwAcc_ProcExit
static BOOL func_ov123_021da5d0(GameProc *proc, u32 *state, void *param, void *work) {
    PdwAccWork *wk = work;

    if (wk->yesNo != NULL) {
        func_02027858(wk->yesNo);
    }
    func_ov123_021d941c(wk->msg);
    func_ov123_021d9a1c(wk->disp);
    if (wk->unk4 != NULL) {
        GFL_HeapFree(wk->unk4);
    }
    if (wk->unk24 != NULL) {
        GFL_HeapFree(wk->unk24);
    }
    if (wk->http != NULL) {
        func_ov114_021b9838(wk->http);
        wk->http = NULL;
    }
    if (func_0203cd98()) {
        func_ov009_02156dc4(NULL, NULL);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_OvlUnload(OVERLAY_ID(114));
    return TRUE;
}
