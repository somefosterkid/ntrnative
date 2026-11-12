// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/game_sync_settings_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/gsync.h"
#include "app/wifi_login.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "save/dream_world.h"
#include "save/save_control.h"
#include "system/game_data.h"

// the procs it queues, defined in wifilogin.c, wifilogout.c and pdwacc_state.c
extern const GameProcFunctions data_ov123_021da9a8;
extern const GameProcFunctions data_ov115_021d6338;
extern const GameProcFunctions data_ov115_021d654c;

// The proc of the start menu's Game Sync settings: logs in, runs the Dream World account screens of overlay 199,
// logs out and restarts the game. The file's name is a guess, from what it does: the ROM has no string for it

enum {
    GAME_SYNC_SETTINGS_LOGIN,
    GAME_SYNC_SETTINGS_ACCOUNT,
    GAME_SYNC_SETTINGS_CHECK_ERROR,
    GAME_SYNC_SETTINGS_LOGOUT,
    GAME_SYNC_SETTINGS_END,
};

// Black 2's; twinned code confirms the offsets of gameData, account, heapId and state in Black
typedef struct {
    WifiLoginParam login;
    WifiLogoutParam logout;
    GameData *gameData;
    PdwAccParam account;
    u8 loginBuffer[0x174];
    HeapID heapId;
    u32 state;
} GameSyncSettingsWork;

BOOL func_02010a5c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_02010ac4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL freeMemResetSystem(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's GAME_SYNC_SETTINGS_PROC_FUNCTIONS
const GameProcFunctions data_0209def0 = {
    func_02010a5c,
    func_02010ac4,
    freeMemResetSystem,
};

// pokebw2's GameSyncSettings_Main

BOOL func_02010a5c(GameProc *proc, u32 *state, void *param, void *work) {
    GameSyncSettingsWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_GAMESYNC, 0x78000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(GameSyncSettingsWork), HEAPID_GAMESYNC);
    MI_CpuFill8(wk, 0, sizeof(GameSyncSettingsWork));
    wk->heapId = HEAPID_GAMESYNC;
    wk->gameData = GameData_Create(HEAPID_GAMESYNC);
    if (func_020092ec(getDreamWorldStuffAddress(GameData_GetSaveControl(wk->gameData)))) {
        wk->state = GAME_SYNC_SETTINGS_ACCOUNT;
        wk->account.mode = PDWACC_MODE_SHOW_ID;
    } else {
        wk->state = GAME_SYNC_SETTINGS_LOGIN;
        wk->account.mode = PDWACC_MODE_CREATE;
    }
    return TRUE;
}

static BOOL func_02010ac4(GameProc *proc, u32 *state, void *param, void *work) {
    GameSyncSettingsWork *wk = work;

    switch (wk->state) {
    case GAME_SYNC_SETTINGS_LOGIN:
        wk->login.gameData = wk->gameData;
        wk->login.unk4 = 0;
        wk->login.unk8 = 1;
        wk->login.unkC = 0x32;
        wk->login.buffer = wk->loginBuffer;
        GCTX_ProcMgrQueueProc(OVERLAY_WIFILOGIN, &data_ov115_021d6338, &wk->login);
        wk->state++;
        break;
    case GAME_SYNC_SETTINGS_ACCOUNT:
        if (wk->login.result == 1) {
            wk->state = GAME_SYNC_SETTINGS_END;
        } else {
            wk->account.gameData = wk->gameData;
            wk->account.heapId = wk->heapId;
            wk->account.loginBuffer = wk->loginBuffer;
            GCTX_ProcMgrQueueProc(OVERLAY_GSYNC, &data_ov123_021da9a8, &wk->account);
            wk->state++;
        }
        break;
    case GAME_SYNC_SETTINGS_CHECK_ERROR:
        if (wk->account.mode == PDWACC_MODE_SHOW_ID) {
            wk->state = GAME_SYNC_SETTINGS_END;
        } else {
            GFL_NetErrShow(0);
            wk->state++;
        }
        break;
    case GAME_SYNC_SETTINGS_LOGOUT:
        // Without a connection left, it logs in again
        if (!func_0203cd98() || wk->account.disconnected == TRUE) {
            wk->login.unk14 = 1;
            wk->state = GAME_SYNC_SETTINGS_LOGIN;
        } else {
            wk->logout.gameData = wk->gameData;
            wk->logout.unk4 = 0;
            wk->logout.unk8 = 1;
            GCTX_ProcMgrQueueProc(OVERLAY_WIFILOGIN, &data_ov115_021d654c, &wk->logout);
            wk->state++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

static BOOL freeMemResetSystem(GameProc *proc, u32 *state, void *param, void *work) {
    GameSyncSettingsWork *wk = work;

    GameData_Free(wk->gameData);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_GAMESYNC);
    OS_ResetSystem(0);
    return TRUE;
}


