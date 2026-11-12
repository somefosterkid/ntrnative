// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov190/wifilogin.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// wifilogin.c, in overlay 190: the process that logs in to the Nintendo Wi-Fi Connection. It starts the network
// library for the connection, asks the player what to do when the settings need fixing, connects, saves the new user
// data and calls the caller's callback while it waits. The ROM has no file name for it, so it is named after its
// process (our name). It draws with wifilogin_disp.c and prints with wifilogin_message.c

#include "app/wifi_login.h"
#include "types.h"
#include "app/wifilogin_disp.h"
#include "app/wifilogin_message.h"
#include "app/error_window_screen.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "dwc/dwc.h"
#include "gfl/dwc_rap.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_state.h"
#include "gfl/net_system.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "nitro/os.h"
#include "save/box.h"
#include "save/player_info.h"
#include "save/tournament_block.h"
#include "save/wifi_list.h"
#include "system/dsi.h"
#include "system/game_data.h"
#include "system/wipe.h"
#include "text/system/wifi_error.h"

typedef struct WifiLoginWork WifiLoginWork;
typedef void (*WifiLoginStateFunc)(WifiLoginWork *work);

// Black 2's; twinned code confirms the offsets of state, heapId, userDataInvalid, saved, menu, disp, msg, wifiList,
// param and gameData in Black
struct WifiLoginWork {
    WifiLoginStateFunc state;
    u8 unk4[8];
    HeapID heapId;
    u8 unkE[14];
    // Set by the network command when another machine sends it
    u32 unk1C;
    // Whether the DWC user data is not valid
    BOOL userDataInvalid;
    // Whether the user data was saved
    BOOL saved;
    u8 unk28[4];
    SaveControl *save;
    WifiLoginMessageMenu *menu;
    WifiLoginDisp *disp;
    WifiLoginMessage *msg;
    WifiList *wifiList;
    WifiLoginParam *param;
    GameData *gameData;
};

// The title of the top screen
#define WIFILOGIN_TITLE_MSG WifiError_Text_035

BOOL func_ov115_021d4240(WifiLoginWork *work);
void func_ov115_021d426c(WifiLoginWork *work, WifiLoginStateFunc state);
void func_ov115_021d4270(int netId, int size, const void *data, void *work, NetHandle *handle);
void *func_ov115_021d4290(void *work);
int func_ov115_021d4298(void *work);
int func_ov115_021d429c(int gameCommandBase, int beaconCommandBase);
DWCUserData *func_ov115_021d42a8(void *work);
DWCFriendData *func_ov115_021d42b8(void *work);
void func_ov115_021d42c8(int code, int param, void *work);
void func_ov115_021d42e0(WifiLoginWork *work);
void func_ov115_021d4310(WifiLoginWork *work);
void func_ov115_021d4360(WifiLoginWork *work);
void func_ov115_021d43cc(WifiLoginWork *work);
void func_ov115_021d43f4(WifiLoginWork *work);
void func_ov115_021d4430(WifiLoginWork *work);
void func_ov115_021d4474(WifiLoginWork *work);
void func_ov115_021d4510(WifiLoginWork *work);
void func_ov115_021d456c(WifiLoginWork *work);
void func_ov115_021d45d0(WifiLoginWork *work);
void func_ov115_021d460c(WifiLoginWork *work);
BOOL func_ov115_021d4644(WifiLoginWork *work);
void func_ov115_021d46cc(WifiLoginWork *work);
void func_ov115_021d4708(WifiLoginWork *work);
void func_ov115_021d4798(WifiLoginWork *work);
void func_ov115_021d47c4(WifiLoginWork *work);
void func_ov115_021d4878(WifiLoginWork *work);
void func_ov115_021d48a4(WifiLoginWork *work);
void func_ov115_021d4910(WifiLoginWork *work);
void func_ov115_021d493c(WifiLoginWork *work);
void func_ov115_021d497c(WifiLoginWork *work);
void func_ov115_021d4a98(WifiLoginWork *work);
static void func_ov115_021d4ab0(WifiLoginWork *work);
void func_ov115_021d4b3c(WifiLoginWork *work);
void func_ov115_021d4b80(WifiLoginWork *work);
void func_ov115_021d4bb4(WifiLoginWork *work);
void func_ov115_021d4bc4(WifiLoginWork *work);
BOOL func_ov115_021d4bfc(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov115_021d4ce4(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov115_021d4d10(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's sNetCommands
static const NetCommand data_ov115_021d6330[] = {
    {func_ov115_021d4270, NULL},
};

// pokebw2's WIFILOGIN_PROC_FUNCTIONS
const GameProcFunctions data_ov115_021d6338 = {
    func_ov115_021d4bfc,
    func_ov115_021d4ce4,
    func_ov115_021d4d10,
};

// pokebw2's sNetInit
static GFLNetInitData data_ov115_021d6560 = {
    data_ov115_021d6330,
    NELEMS(data_ov115_021d6330),
    NULL,
    NULL,
    NULL,
    NULL,
    func_ov115_021d4290,
    func_ov115_021d4298,
    func_ov115_021d429c,
    NULL,
    (void (*)(NetHandle *, int, void *))func_ov109_021bb8e0,
    NULL,
    NULL,
    {.wifiError = {{0}, func_ov115_021d42c8}},
    func_ov115_021d42b8,
    func_ov115_021d42a8,
    0x5e400,
    {1, 0, 0, 0, 0x80, 0x13, 0, 0},
    HEAPID_USER,
    0xd,
    0xf,
    0x10,
    0xf0,
    0,
    2,
    0x64,
    0x10,
    1,
    0,
    GFL_NET_TYPE_WIFI,
    1,
    0,
    {0xfe, 0xff, 0, 0},
    0,
    0,
};

// The game service ID that the network library shares with the other machine, which the login gets from its parameter
// pokebw2's sGameType
static u32 data_ov115_021d6640;

// Whether the player presses A or B, or touches the screen when the login lets them
// pokebw2's WifiLogin_IsInputPressed
BOOL func_ov115_021d4240(WifiLoginWork *work) {
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return TRUE;
    }
    if (work->param->unk4 != 0 && func_02035c60()) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiLogin_SetState
void func_ov115_021d426c(WifiLoginWork *work, WifiLoginStateFunc state) {
    work->state = state;
}

// The login's one network command
// pokebw2's WifiLogin_OnNetCommand
void func_ov115_021d4270(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WifiLoginWork *wk = work;

    if (handle == func_020399a8()) {
        if (netId != func_02039338()) {
            wk->unk1C = 1;
        }
    }
}

// pokebw2's WifiLogin_GetInfo
void *func_ov115_021d4290(void *work) {
    return &data_ov115_021d6640;
}

// pokebw2's WifiLogin_GetInfoSize
int func_ov115_021d4298(void *work) {
    return sizeof(data_ov115_021d6640);
}

// pokebw2's WifiLogin_IsSameGame
int func_ov115_021d429c(int gameCommandBase, int beaconCommandBase) {
    if (gameCommandBase == beaconCommandBase) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiLogin_GetUserData
DWCUserData *func_ov115_021d42a8(void *work) {
    WifiLoginWork *wk = work;

    return func_02009860(func_0201283c(wk->gameData));
}

// pokebw2's WifiLogin_GetFriendData
DWCFriendData *func_ov115_021d42b8(void *work) {
    WifiLoginWork *wk = work;

    return func_020099c8(func_0201283c(wk->gameData), 0);
}

// pokebw2's WifiLogin_OnFriendCode
void func_ov115_021d42c8(int code, int param, void *work) {
    WifiLoginWork *wk = work;

    func_02009c24(func_0201283c(wk->gameData), code, param);
}

// pokebw2's WifiLogin_StateWaitMessageEnd
void func_ov115_021d42e0(WifiLoginWork *work) {
    if (GFL_WipeIsFinished() && func_ov115_021d5c70(work->msg) && func_ov115_021d4240(work)) {
        func_ov115_021d426c(work, func_ov115_021d4ab0);
    }
}

// pokebw2's WifiLogin_StateWaitInputThenExit
void func_ov115_021d4310(WifiLoginWork *work) {
    if (func_ov115_021d4240(work)) {
        func_ov115_021d5f7c(work->msg);
        if (work->param->unk4 == 0) {
            func_ov115_021d5ccc(work->msg, WIFILOGIN_TITLE_MSG);
        }
        if (work->param->buffer != NULL) {
            func_ov115_021d426c(work, func_ov115_021d4bb4);
        } else {
            func_ov115_021d426c(work, func_ov115_021d4ab0);
        }
    } else {
        func_ov115_021d4644(work);
    }
}

// Goes on once the network is up: tells the player the settings were fixed, or goes to the connection
// pokebw2's WifiLogin_CheckLoggedIn
void func_ov115_021d4360(WifiLoginWork *work) {
    if (func_0203c600()) {
        if (work->userDataInvalid) {
            func_ov115_021d5d3c(work->msg);
            func_ov115_021d5c94(work->msg);
            if (func_0202eb0c()) {
                func_ov115_021d5ed8(work->msg, WifiError_Text_044);
            } else {
                func_ov115_021d5ed8(work->msg, WifiError_Text_019);
            }
            func_ov115_021d426c(work, func_ov115_021d4310);
        } else if (work->param->buffer != NULL) {
            func_ov115_021d426c(work, func_ov115_021d4bb4);
        } else {
            func_ov115_021d426c(work, func_ov115_021d4ab0);
        }
    } else {
        func_ov115_021d4644(work);
    }
}

// Waits for the user data to be saved
// pokebw2's WifiLogin_StateWaitSave
void func_ov115_021d43cc(WifiLoginWork *work) {
    if (func_ov009_02156c84()) {
        switch (func_02012dd0(work->gameData)) {
        case 0:
        case 1:
            return;
        }
        func_ov009_02156c98();
    }
    func_ov115_021d4360(work);
}

// pokebw2's WifiLogin_StateStartSave
void func_ov115_021d43f4(WifiLoginWork *work) {
    if (func_ov115_021d5c70(work->msg)) {
        func_02008540(GetGameDataPlayerInfo(work->gameData), func_02009cc0(work->wifiList));
        func_02012dac(work->gameData);
        work->saved = TRUE;
        func_ov115_021d426c(work, func_ov115_021d43cc);
    }
}

// pokebw2's WifiLogin_StateWaitMessageThenSave
void func_ov115_021d4430(WifiLoginWork *work) {
    if (func_ov115_021d5c70(work->msg)) {
        if (func_ov009_02156c84() && !work->saved && work->param->unk14 != 2) {
            func_ov115_021d5c24(work->msg, WifiError_Text_030);
            func_ov115_021d426c(work, func_ov115_021d43f4);
        } else {
            func_ov115_021d4360(work);
        }
    }
}

// Starts the network library for the Wi-Fi connection
// pokebw2's WifiLogin_StateStartNet
void func_ov115_021d4474(WifiLoginWork *work) {
    if (OS_IsRunOnTwl()) {
        data_ov115_021d6560.wifiHeapSize = 0x79000;
    } else {
        data_ov115_021d6560.wifiHeapSize = 0x5e400;
    }
    if (work->param->unkC == 0x2c) {
        data_ov115_021d6560.wifiHeapSize = 0x50000;
    } else if (work->param->unkC == 0x24) {
        data_ov115_021d6560.wifiHeapSize = 0x6e000;
    }
    if (work->param->unkC != 0xa) {
        data_ov115_021d6560.getFriendData = NULL;
    }
    data_ov115_021d6560.gameCommandBase = work->param->unkC;
    data_ov115_021d6640 = work->param->unkC;
    func_0203cbac(&data_ov115_021d6560, NULL, work);
    func_0203c8a8();
    if (work->param->unk8 == 0) {
        func_0203d34c(FALSE, work->heapId);
    } else {
        func_0203d34c(TRUE, work->heapId);
    }
    func_ov115_021d5c24(work->msg, WifiError_Text_023);
    func_ov115_021d592c(work->disp);
    func_ov115_021d426c(work, func_ov115_021d4430);
}

// pokebw2's WifiLogin_StateWaitConfirmStart
void func_ov115_021d4510(WifiLoginWork *work) {
    if (func_ov115_021d5e6c(work->menu)) {
        if (func_ov115_021d5ea8(work->menu) == 0) {
            work->param->result = 0;
            func_ov115_021d426c(work, func_ov115_021d4474);
        } else {
            work->param->result = 1;
            func_ov115_021d426c(work, func_ov115_021d4ab0);
        }
        func_ov115_021d5f7c(work->msg);
        if (work->param->unk4 == 0) {
            func_ov115_021d5ccc(work->msg, WIFILOGIN_TITLE_MSG);
        }
        func_ov115_021d5e34(work->menu);
        work->menu = NULL;
    }
}

// pokebw2's WifiLogin_StateWaitConfirmRetry
void func_ov115_021d456c(WifiLoginWork *work) {
    if (func_ov115_021d5e6c(work->menu)) {
        if (func_ov115_021d5ea8(work->menu) == 0) {
            work->param->result = 0;
            func_ov115_021d426c(work, func_ov115_021d4474);
        } else {
            work->param->result = 1;
            func_ov115_021d426c(work, func_ov115_021d4ab0);
        }
        func_ov115_021d5f7c(work->msg);
        func_ov115_021d5c94(work->msg);
        func_ov115_021d5e34(work->menu);
        work->menu = NULL;
        if (work->param->unk4 == 0) {
            func_ov115_021d5ccc(work->msg, WIFILOGIN_TITLE_MSG);
        }
    }
}

// pokebw2's WifiLogin_StateOpenConfirmStart
void func_ov115_021d45d0(WifiLoginWork *work) {
    if (GFL_WipeIsFinished() && func_ov115_021d5c70(work->msg)) {
        work->menu = func_ov115_021d5d5c(work->msg, 1, work->param->unk4 == 0, 0);
        func_ov115_021d426c(work, func_ov115_021d4510);
    }
}

// pokebw2's WifiLogin_StateOpenConfirmError
void func_ov115_021d460c(WifiLoginWork *work) {
    if (GFL_WipeIsFinished()) {
        func_ov115_021d5d3c(work->msg);
        work->menu = func_ov115_021d5d5c(work->msg, 1, work->param->unk4 == 0, 0);
        func_ov115_021d426c(work, func_ov115_021d456c);
    }
}

// Stops at a network error and tells the player, returning whether there was one
// pokebw2's WifiLogin_CheckNetError
BOOL func_ov115_021d4644(WifiLoginWork *work) {
    if (func_0200fb24()) {
        switch (func_0203ca98()->unk4) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            func_ov115_021d5f7c(work->msg);
            func_ov115_021d5d3c(work->msg);
            func_ov115_021d5c94(work->msg);
            func_0200f83c();
            func_ov115_021d5938(work->disp);
            if (func_0203cd98()) {
                func_0200fbb0();
            }
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            func_ov115_021d5ed8(work->msg, WifiError_Text_037);
            func_ov115_021d426c(work, func_ov115_021d460c);
            return TRUE;
        case 7:
            GFL_NetErrAbort();
            break;
        }
    }
    return FALSE;
}

// pokebw2's WifiLogin_StateOpenYesNo
void func_ov115_021d46cc(WifiLoginWork *work) {
    if (GFL_WipeIsFinished() && func_ov115_021d5c70(work->msg)) {
        work->menu = func_ov115_021d5d5c(work->msg, 0, work->param->unk4 == 0, 0);
        func_ov115_021d426c(work, func_ov115_021d456c);
    }
}

// pokebw2's WifiLogin_StateWaitResetConfirm
void func_ov115_021d4708(WifiLoginWork *work) {
    if (func_ov115_021d5e6c(work->menu)) {
        if (func_ov115_021d5ea8(work->menu) == 0) {
            void *entry = func_0200b1d4(getTournamentBlockAddress(GameData_GetSaveControl(work->gameData)), 0);

            func_0200bbd8(getBattleBox(GameData_GetSaveControl(work->gameData)), 1);
            func_0200b110(entry, 10, 5);
            work->param->result = 0;
            initWifi(work->wifiList);
            func_ov115_021d426c(work, func_ov115_021d4474);
        } else {
            work->param->result = 1;
            func_ov115_021d426c(work, func_ov115_021d497c);
        }
        func_ov115_021d5f7c(work->msg);
        if (work->param->unk4 == 0) {
            func_ov115_021d5ccc(work->msg, WIFILOGIN_TITLE_MSG);
        }
        func_ov115_021d5e34(work->menu);
        work->menu = NULL;
    }
}

// pokebw2's WifiLogin_StateOpenResetConfirm
void func_ov115_021d4798(WifiLoginWork *work) {
    work->menu = func_ov115_021d5d5c(work->msg, 1, work->param->unk4 == 0, 1);
    func_ov115_021d426c(work, func_ov115_021d4708);
}

// pokebw2's WifiLogin_StateWaitProfileConfirm
void func_ov115_021d47c4(WifiLoginWork *work) {
    if (func_ov115_021d5e6c(work->menu)) {
        u8 answer = func_ov115_021d5ea8(work->menu);

        func_ov115_021d5f7c(work->msg);
        if (work->param->unk4 == 0) {
            func_ov115_021d5ccc(work->msg, WIFILOGIN_TITLE_MSG);
        }
        func_ov115_021d5e34(work->menu);
        work->menu = NULL;
        if (answer == 0) {
            void *entry = func_0200b1d4(getTournamentBlockAddress(GameData_GetSaveControl(work->gameData)), 0);
            u32 field2 = func_0200b078(entry, 2);
            u32 field10 = func_0200b078(entry, 10);

            if (field2 != 0 && field10 >= 1 && field10 <= 4) {
                func_ov115_021d5d3c(work->msg);
                func_ov115_021d5ed8(work->msg, WifiError_Text_038);
                func_ov115_021d426c(work, func_ov115_021d4798);
                return;
            }
            work->param->result = 0;
            initWifi(work->wifiList);
            func_ov115_021d426c(work, func_ov115_021d4474);
        } else {
            work->param->result = 1;
            func_ov115_021d426c(work, func_ov115_021d497c);
        }
    }
}

// pokebw2's WifiLogin_StateOpenProfileConfirm
void func_ov115_021d4878(WifiLoginWork *work) {
    work->menu = func_ov115_021d5d5c(work->msg, 1, work->param->unk4 == 0, 1);
    func_ov115_021d426c(work, func_ov115_021d47c4);
}

// pokebw2's WifiLogin_StateWaitFixConfirm
void func_ov115_021d48a4(WifiLoginWork *work) {
    if (func_ov115_021d5e6c(work->menu)) {
        if (func_ov115_021d5ea8(work->menu) == 0) {
            work->param->result = 0;
            func_ov115_021d5d3c(work->msg);
            func_ov115_021d5ed8(work->msg, WifiError_Text_022);
            func_ov115_021d426c(work, func_ov115_021d4878);
        } else {
            work->param->result = 1;
            func_ov115_021d5f7c(work->msg);
            if (work->param->unk4 == 0) {
                func_ov115_021d5ccc(work->msg, WIFILOGIN_TITLE_MSG);
            }
            func_ov115_021d426c(work, func_ov115_021d4ab0);
        }
        func_ov115_021d5e34(work->menu);
        work->menu = NULL;
    }
}

// pokebw2's WifiLogin_StateOpenFixConfirm
void func_ov115_021d4910(WifiLoginWork *work) {
    work->menu = func_ov115_021d5d5c(work->msg, 1, work->param->unk4 == 0, 1);
    func_ov115_021d426c(work, func_ov115_021d48a4);
}

// pokebw2's WifiLogin_StateWaitInputThenFix
void func_ov115_021d493c(WifiLoginWork *work) {
    if (GFL_WipeIsFinished() && func_ov115_021d4240(work)) {
        func_ov115_021d5d3c(work->msg);
        if (func_0202eb0c()) {
            func_ov115_021d5ed8(work->msg, WifiError_Text_046);
        } else {
            func_ov115_021d5ed8(work->msg, WifiError_Text_021);
        }
        func_ov115_021d426c(work, func_ov115_021d4910);
    }
}

// Checks that the console can connect, and what state its settings are in
// pokebw2's WifiLogin_StateCheckStart
void func_ov115_021d497c(WifiLoginWork *work) {
    if (!func_0202eb44()) {
        func_ov115_021d5d3c(work->msg);
        func_ov115_021d5ed8(work->msg, WifiError_Text_039);
        work->param->result = 1;
        func_ov115_021d426c(work, func_ov115_021d42e0);
        return;
    }
    if (!hasLicenseBeenAccepted()) {
        func_ov115_021d5d3c(work->msg);
        func_ov115_021d5ed8(work->msg, WifiError_Text_034);
        work->param->result = 1;
        func_ov115_021d426c(work, func_ov115_021d42e0);
        return;
    }
    if (work->param->unk14 == 2) {
        func_ov115_021d5b50(work->msg, WifiError_Text_017);
        func_ov115_021d426c(work, func_ov115_021d46cc);
        return;
    }
    if (!DWC_CheckHasProfile(func_02009860(work->wifiList))) {
        work->userDataInvalid = TRUE;
        func_ov115_021d5d3c(work->msg);
        if (!func_0202eb0c()) {
            func_ov115_021d5ed8(work->msg, WifiError_Text_018);
        } else {
            func_ov115_021d5ed8(work->msg, WifiError_Text_043);
        }
        func_ov115_021d426c(work, func_ov115_021d45d0);
        return;
    }
    if (!DWC_CheckValidConsole(func_02009860(work->wifiList))) {
        work->userDataInvalid = TRUE;
        func_ov115_021d5d3c(work->msg);
        if (!func_0202eb0c()) {
            func_ov115_021d5ed8(work->msg, WifiError_Text_020);
        } else {
            func_ov115_021d5ed8(work->msg, WifiError_Text_045);
        }
        func_ov115_021d426c(work, func_ov115_021d493c);
        return;
    }
    if (work->param->unk14 == 1) {
        func_ov115_021d5d3c(work->msg);
        func_ov115_021d5ed8(work->msg, WifiError_Text_037);
        func_ov115_021d426c(work, func_ov115_021d460c);
    } else {
        func_ov115_021d5b50(work->msg, WifiError_Text_017);
        func_ov115_021d426c(work, func_ov115_021d46cc);
    }
}

// pokebw2's WifiLogin_StateWaitFadeOut
void func_ov115_021d4a98(WifiLoginWork *work) {
    if (GFL_WipeIsFinished()) {
        func_ov115_021d426c(work, NULL);
    }
}

// pokebw2's WifiLogin_StateRunCallback
// pokebw2's WifiLogin_StateRunCallback
// pokebw2's WifiLogin_StateRunCallback
// pokebw2's WifiLogin_StateRunCallback
// pokebw2's WifiLogin_StateRunCallback
static void func_ov115_021d4ab0(WifiLoginWork *work) {
    u32 status;

    if (work->param->unk20 != NULL && work->param->result == 0) {
        status = work->param->unk20(work->msg, work->param->unk24);
        if (status == 2) {
            return;
        }
        switch (status) {
        case 0:
            work->param->result = 0;
            func_ov115_021d426c(work, func_ov115_021d4b3c);
            break;
        case 1:
            work->param->result = 1;
            func_ov115_021d5f7c(work->msg);
            func_ov115_021d5d3c(work->msg);
            func_ov115_021d5c94(work->msg);
            if (func_0203cd98()) {
                func_0200fbb0();
            }
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            func_ov115_021d5ed8(work->msg, 0x25);
            func_ov115_021d426c(work, func_ov115_021d460c);
            break;
        }
        return;
    }
    func_ov115_021d426c(work, func_ov115_021d4b3c);
}


// pokebw2's WifiLogin_StateStartFadeOut
void func_ov115_021d4b3c(WifiLoginWork *work) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, work->heapId);
    if (work->param->unk4 == 0 && work->param->unk18 == 0) {
        GFL_SndBGMFadeOut(8);
    }
    func_ov115_021d426c(work, func_ov115_021d4a98);
}

// Connects to the Nintendo Wi-Fi Connection
// pokebw2's WifiLogin_StateConnect
void func_ov115_021d4b80(WifiLoginWork *work) {
    if (!func_ov115_021d4644(work)) {
        if (DWC_SVLGetTokenAsync("", work->param->buffer)) {
            func_ov115_021d426c(work, func_ov115_021d4bc4);
        } else {
            func_ov115_021d4644(work);
        }
    }
}

// pokebw2's WifiLogin_StateStartConnect
void func_ov115_021d4bb4(WifiLoginWork *work) {
    func_ov115_021d426c(work, func_ov115_021d4b80);
}

// pokebw2's WifiLogin_StateWaitConnect
void func_ov115_021d4bc4(WifiLoginWork *work) {
    if (!func_ov115_021d4644(work)) {
        int connection = DWC_SVLProcess();

        if (connection == 3) {
            func_ov115_021d426c(work, func_ov115_021d4ab0);
        } else if (connection == 4 || connection == 0 || connection == 5) {
            func_ov115_021d4644(work);
        }
    }
}

// pokebw2's WifiLoginProc_Init
BOOL func_ov115_021d4bfc(GameProc *proc, u32 *state, void *param, void *work) {
    WifiLoginParam *loginParam = param;
    WifiLoginWork *wk;

    GFL_ASSERT(loginParam->unkC != 0);
    GFL_HeapCreateChild(HEAPID_USER, 0x78, 6 << 14);
    wk = GFL_ProcInitSubsystem(proc, sizeof(WifiLoginWork), 0x78);
    MI_CpuFill8(wk, 0, sizeof(WifiLoginWork));
    wk->heapId = 0x78;
    wk->gameData = loginParam->gameData;
    wk->disp = func_ov115_021d4d38(wk->heapId, loginParam->unk4, loginParam->unk8);
    wk->msg = func_ov115_021d5984(wk->heapId, TEXT_BANK_WIFI_ERROR, loginParam->unk8);
    wk->save = GameData_GetSaveControl(loginParam->gameData);
    wk->wifiList = func_0201283c(loginParam->gameData);
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, wk->heapId);
    wk->param = loginParam;
    if (func_0203cd98()) {
        func_0203ca04();
        func_0203c968();
        func_0200fbb0();
        func_0200fba0();
    }
    func_ov115_021d426c(wk, func_ov115_021d497c);
    if (wk->param->unk4 == 0) {
        func_ov115_021d5ccc(wk->msg, 0x23);
        if (wk->param->unk18 == 0) {
            GFL_SndBGMPlay(SEQ_BGM_WIFI_ACCESS, SND_CHANNEL_MASK_ALL);
        }
    } else if (wk->param->unk18 == 0) {
        GFL_SndBGMPlay(SEQ_BGM_GAME_SYNC, SND_CHANNEL_MASK_ALL);
    }
    return TRUE;
}


// pokebw2's WifiLoginProc_Main
static BOOL func_ov115_021d4ce4(GameProc *proc, u32 *state, void *param, void *work) {
    WifiLoginWork *wk = work;
    BOOL done = TRUE;

    func_ov115_021d4df4(wk->disp);
    func_ov115_021d5a88(wk->msg);
    if (wk->state != NULL) {
        wk->state(wk);
        done = FALSE;
    }
    if (wk->menu != NULL) {
        func_ov115_021d5e48(wk->menu);
    }
    return done;
}

// pokebw2's WifiLoginProc_Exit
BOOL func_ov115_021d4d10(GameProc *proc, u32 *state, void *param, void *work) {
    WifiLoginWork *wk = work;

    GFL_BGSysReleaseBG(7);
    func_ov115_021d5a9c(wk->msg);
    func_ov115_021d4e04(wk->disp);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x78);
    return TRUE;
}


