// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_enter.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wifi_login.h"
#include "app/wifilogin_message.h"
#include "constants/sound.h"
#include "dpw/dpw_tr.h"
#include "dwc/dwc.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/dpw_profile.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/gx.h"
#include "save/player_info.h"
#include "save/wifi_list.h"
#include "system/bmp_winframe.h"
#include "system/game_system.h"
#include "system/wordset.h"
#include "worldtrade_local.h"
#include "app/worldtrade.h"

typedef struct WorldTradePrint WorldTradePrint;

void func_ov135_021f4fc4(WorldTradeWork *wk);
void func_ov135_021f513c(WorldTradeWork *wk);

// The message printing of worldtrade_adapter.c (pokebw2's WorldTrade_Print and WorldTrade_StreamPrint)
void func_ov135_02202858(BmpWin *win, u8 font, StrBuf *str, int x, int y, WorldTradePrint *print);
void func_ov135_02202878(BmpWin *win, u8 font, StrBuf *str, int x, int y, WorldTradePrint *print);

// The Global Trade Station's connection screen: logging in to Nintendo Wi-Fi Connection and the trade server, and
// logging out, through the Wi-Fi login proc; and the text printing the other screens share. The names are ours,
// guessed

// The modes that the connection screen is entered with
#define ENTER_MODE_LOGIN 0x13
#define ENTER_MODE_LOGOUT 0x14
// Logging in again after an error
#define ENTER_MODE_RELOGIN 0x15

// The connection screen's steps
enum {
    ENTER_START,
    ENTER_END,
    ENTER_WIFI_LOGIN,
    ENTER_WIFI_LOGIN_WAIT,
    ENTER_WIFI_LOGOUT,
    ENTER_WIFI_LOGOUT_WAIT,
};

// The steps of the login that the Wi-Fi login proc calls back for
enum {
    LOGIN_INTERNET_CONNECT,
    LOGIN_INTERNET_CONNECT_WAIT,
    LOGIN_DPW_TR_INIT,
    LOGIN_SERVER_START,
    LOGIN_SERVER_RESULT,
    LOGIN_PROFILE_START,
    LOGIN_PROFILE_RESULT,
    LOGIN_SERVER_ERROR_PRINT,
    LOGIN_SERVER_ERROR_WAIT,
    LOGIN_END,
};

// The library's results are 0 to 2 for the server's state and negative for its errors. -5000 to -5005 are this
// file's, for the profile's errors

// The frames to wait for the server before giving up
#define SERVER_TIMEOUT (30 * 60 * 2)

static void func_ov135_021f8a3c(WorldTradeWork *wk);
static void func_ov135_021f8a40(WorldTradeWork *wk);
int func_ov135_021f8a44(WorldTradeWork *wk);
int func_ov135_021f8a74(WorldTradeWork *wk);
int func_ov135_021f8a80(WorldTradeWork *wk);
static int func_ov135_021f8ab0(WorldTradeWork *wk);
int func_ov135_021f8af4(WorldTradeWork *wk);
int func_ov135_021f8b0c(WorldTradeWork *wk);
static BOOL func_ov135_021f8bbc(WorldTradeWork *wk);
static int func_ov135_021f8be8(WorldTradeWork *wk);
int func_ov135_021f8d10(WorldTradeWork *wk);
int func_ov135_021f8da4(WorldTradeWork *wk);
int func_ov135_021f8de4(WorldTradeWork *wk);
int func_ov135_021f8e58(WorldTradeWork *wk);
u32 func_ov135_021f8e88(WifiLoginMessage *msgWork, void *data);
static u32 func_ov135_021f8f5c(WifiLoginMessage *msgWork, void *data);
int func_ov135_021f8f68(WorldTradeWork *wk);
int func_ov135_021f8f84(WorldTradeWork *wk, void *msgWork);
int func_ov135_021f9024(WorldTradeWork *wk, void *msgWork);
void func_ov135_021f90b8(WorldTradeWork *wk, MsgData *msgManager, int msgNo, int wait, u16 dat, BOOL stream);
int func_ov135_021f914c(BmpWin *win, StrBuf *str, int x, int flag, u16 color, int font, WorldTradePrint *print);

// pokebw2's sEnterFuncTable
static int (*data_ov135_02203c1c[])(WorldTradeWork *wk) = {
    func_ov135_021f8a44,
    func_ov135_021f8f68,
    func_ov135_021f8d10,
    func_ov135_021f8da4,
    func_ov135_021f8de4,
    func_ov135_021f8e58,
};

// pokebw2's WorldTrade_Enter_End

int func_ov135_021f89b4(WorldTradeWork *wk, int seq) {
    func_ov135_021f50ec(wk);
    func_ov135_021f8a3c(wk);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    switch (wk->subProcessMode) {
    case ENTER_MODE_LOGIN:
    case ENTER_MODE_RELOGIN:
        wk->subprocessSeq = ENTER_WIFI_LOGIN;
        break;
    case ENTER_MODE_LOGOUT:
        wk->subprocessSeq = ENTER_WIFI_LOGOUT;
        break;
    }
    return WT_SEQ_FADEIN;
}

int func_ov135_021f89f0(WorldTradeWork *wk, int seq) {
    int oldSeq = wk->subprocessSeq;
    int ret = data_ov135_02203c1c[oldSeq](wk);

    if (oldSeq != wk->subprocessSeq) {
        wk->localSeq = 0;
        wk->localWait = 0;
    }
    return ret;
}


int func_ov135_021f8a18(WorldTradeWork *wk, int seq) {
    func_ov135_021f8a40(wk);
    func_ov135_021f5074(wk);
    func_ov135_021f4ec8(wk);
    if (wk->subProcess == WORLDTRADE_ENTER) {
        return WT_SEQ_OUT;
    }
    return WT_SEQ_INIT;
}

// pokebw2's Enter_InitWork
static void func_ov135_021f8a3c(WorldTradeWork *wk) {
}

// pokebw2's Enter_FreeWork
static void func_ov135_021f8a40(WorldTradeWork *wk) {
}

int func_ov135_021f8a44(WorldTradeWork *wk) {
    GFL_SndBGMPlay(SEQ_BGM_GTS, SND_CHANNEL_MASK_ALL);
    wk->subprocessSeq = ENTER_END;
    wk->boxSearchFlag = 1;
    wk->openingFlag = 0;
    func_ov135_021f4ec0(wk, WORLDTRADE_TITLE, 0);
    return WT_SEQ_MAIN;
}


// pokebw2's Enter_InternetConnect
int func_ov135_021f8a74(WorldTradeWork *wk) {
    DWC_NASLoginAsync();
    return TRUE;
}

// pokebw2's Enter_InternetConnectWait
int func_ov135_021f8a80(WorldTradeWork *wk) {
    switch (func_ov009_02168124()) {
    case 3:
        return 1;
    case 0:
    case 4:
    case 5:
        return 2;
    }
    return 0;
}

// pokebw2's Enter_DpwTrInit
static int func_ov135_021f8ab0(WorldTradeWork *wk) {
    void *userData = func_02009860(wk->param->wifilist);
    s32 pid;

    func_02008540(wk->param->mystatus, func_02009cc0(wk->param->wifilist));
    pid = func_0200853c(wk->param->mystatus);
    func_ov114_021c7ba0(pid, func_0205994c(userData), 0);
    return TRUE;
}

int func_ov135_021f8af4(WorldTradeWork *wk) {
    func_ov114_021c8da0();
    wk->timeoutCount = 0;
    return TRUE;
}

int func_ov135_021f8b0c(WorldTradeWork *wk) {
    s32 result;

    func_ov114_021c7c1c();
    if (func_ov114_021c866c()) {
        result = func_ov114_021c86a8();
        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            return 1;
        case 1:
            func_ov135_021f4fc4(wk);
            wk->connectErrorNo = result;
            return 2;
        case 2:
        case -1:
            func_ov135_021f4fc4(wk);
            wk->connectErrorNo = result;
            return 2;
        case -12:
        case -15:
            func_ov135_021f4fc4(wk);
            wk->connectErrorNo = result;
            return 2;
        case -2:
        case -14:
            func_ov135_021f4fc4(wk);
            wk->connectErrorNo = result;
            return 2;
        case -3:
        case -4:
        case -5:
        case -6:
        case -7:
        case -8:
        case -9:
        case -10:
        case -11:
        case -13:
        default:
            func_ov135_021f4fc4(wk);
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == SERVER_TIMEOUT) {
        func_ov135_021f513c(wk);
    }
    return 0;
}

static BOOL func_ov135_021f8bbc(WorldTradeWork *wk) {
    func_ov114_021b9b00(&wk->dcProfile, wk->param->mystatus);
    func_ov114_021c8e18(&wk->dcProfile, &wk->dcProfileResult);
    wk->timeoutCount = 0;
    return TRUE;
}

static int func_ov135_021f8be8(WorldTradeWork *wk) {
    s32 result;

    func_ov114_021c7c1c();
    if (func_ov114_021c866c()) {
        result = func_ov114_021c86a8();
        wk->timeoutCount = 0;
        switch (result) {
        case 0:
            switch (wk->dcProfileResult.code) {
            case 0:
                switch (wk->dcProfileResult.mailAddrAuthResult) {
                case 0:
                    func_ov135_021f4ec0(wk, 1, 0);
                    return 1;
                case 3:
                    wk->connectErrorNo = -5003;
                    return 2;
                case 1:
                    wk->connectErrorNo = -5000;
                    return 2;
                case 2:
                    wk->connectErrorNo = -5001;
                    return 2;
                default:
                    func_ov135_021f513c(wk);
                    break;
                }
                break;
            case 1:
                wk->connectErrorNo = -5004;
                return 2;
            case 2:
                wk->connectErrorNo = -5005;
                return 2;
            default:
                func_ov135_021f4fc4(wk);
                func_ov135_021f513c(wk);
                break;
            }
            break;
        case 1:
            func_ov135_021f4fc4(wk);
            wk->connectErrorNo = result;
            return 2;
        case 2:
        case -1:
            func_ov135_021f4fc4(wk);
            wk->connectErrorNo = result;
            return 2;
        case -12:
        case -15:
            func_ov135_021f4fc4(wk);
            wk->connectErrorNo = result;
            return 2;
        case -2:
        case -14:
            func_ov135_021f4fc4(wk);
            wk->connectErrorNo = result;
            return 2;
        default:
            func_ov135_021f4fc4(wk);
            func_ov135_021f513c(wk);
            break;
        }
    } else if (++wk->timeoutCount == 0xe10) {
        func_ov135_021f513c(wk);
    }
    return 0;
}



extern const GameProcFunctions data_ov115_021d6338;

/* Corrected struct WorldTradeWork: added wifiLoginBuffer at 0x116c (was part of unk10E0) */
int func_ov135_021f8d10(WorldTradeWork *wk) {
    WifiLoginParam *param;

    wk->subProcParam = GFL_HeapAllocate(HEAPID_WORLDTRADE, sizeof(WifiLoginParam), FALSE, "worldtrade_enter.c", 611);
    MI_CpuFill8(wk->subProcParam, 0, sizeof(WifiLoginParam));
    param = wk->subProcParam;
    param->gameData = GSYS_GetGameData(wk->param->gsys);
    param->unk4 = 0;
    param->unk8 = 1;
    param->buffer = &wk->wifiLoginBuffer;
    param->unkC = 30;
    param->unk20 = func_ov135_021f8e88;
    param->unk24 = wk;
    wk->loginSeq = LOGIN_INTERNET_CONNECT;
    if (wk->subProcessMode == ENTER_MODE_RELOGIN) {
        param->unk14 = 1;
    } else {
        param->unk14 = 0;
    }
    QueueGameProc(wk->procManager, OVERLAY_ID(115), &data_ov115_021d6338, wk->subProcParam);
    GFL_NetErrShow(0);
    wk->subprocessSeq = ENTER_WIFI_LOGIN_WAIT;
    return 2;
}



int func_ov135_021f8da4(WorldTradeWork *wk) {
    WifiLoginParam *param = wk->subProcParam;

    if (!wk->procResult) {
        switch (param->result) {
        case 0:
            wk->subprocessSeq = ENTER_START;
            break;
        case 1:
            func_ov135_021f4ec0(wk, 0, 0);
            wk->subprocessSeq = ENTER_END;
            break;
        }
        GFL_HeapFree(wk->subProcParam);
    }
    return WT_SEQ_MAIN;
}

int func_ov135_021f8de4(WorldTradeWork *wk) {
    WifiLogoutParam *param;

    wk->subProcParam = GFL_HeapAllocate(HEAPID_WORLDTRADE, sizeof(WifiLogoutParam), FALSE, "worldtrade_enter.c", 689);
    MI_CpuFill8(wk->subProcParam, 0, sizeof(WifiLogoutParam));
    param = wk->subProcParam;
    param->gameData = GSYS_GetGameData(wk->param->gsys);
    param->unk4 = 0;
    param->unk8 = 1;
    param->unk14 = func_ov135_021f8f5c;
    param->unk18 = wk;
    QueueGameProc(wk->procManager, OVERLAY_ID(115), &data_ov115_021d654c, wk->subProcParam);
    wk->subprocessSeq = ENTER_WIFI_LOGOUT_WAIT;
    GFL_NetErrShow(0);
    return WT_SEQ_MAIN;
}

int func_ov135_021f8e58(WorldTradeWork *wk) {
    if (!wk->procResult) {
        GFL_HeapFree(wk->subProcParam);
        func_ov135_021f4ec0(wk, 0, 0);
        wk->subprocessSeq = ENTER_END;
    }
    return WT_SEQ_MAIN;
}


// pokebw2's Enter_WifiLoginCallback
u32 func_ov135_021f8e88(WifiLoginMessage *msgWork, void *data) {
    WorldTradeWork *wk = data;

    switch (wk->loginSeq) {
    case LOGIN_INTERNET_CONNECT:
        if (func_ov135_021f8a74(wk)) {
            wk->loginSeq++;
        }
        break;
    case LOGIN_INTERNET_CONNECT_WAIT:
        switch (func_ov135_021f8a80(wk)) {
        case 1:
            wk->loginSeq++;
            break;
        case 2:
            wk->loginSeq = LOGIN_END;
            break;
        }
        break;
    case LOGIN_DPW_TR_INIT:
        if (func_ov135_021f8ab0(wk)) {
            wk->loginSeq++;
        }
        break;
    case LOGIN_SERVER_START:
        if (func_ov135_021f8af4(wk)) {
            wk->loginSeq++;
        }
        break;
    case LOGIN_SERVER_RESULT:
        switch (func_ov135_021f8b0c(wk)) {
        case 1:
            wk->loginSeq++;
            break;
        case 2:
            wk->loginSeq = LOGIN_SERVER_ERROR_PRINT;
            break;
        }
        break;
    case LOGIN_PROFILE_START:
        if (func_ov135_021f8bbc(wk)) {
            wk->loginSeq++;
        }
        break;
    case LOGIN_PROFILE_RESULT:
        switch (func_ov135_021f8be8(wk)) {
        case 1:
            wk->loginSeq = LOGIN_INTERNET_CONNECT;
            return 0;
        case 2:
            wk->loginSeq = LOGIN_SERVER_ERROR_PRINT;
            break;
        }
        break;
    case LOGIN_SERVER_ERROR_PRINT:
        if (func_ov135_021f8f84(wk, msgWork)) {
            wk->loginSeq++;
        }
        break;
    case LOGIN_SERVER_ERROR_WAIT:
        if (func_ov135_021f9024(wk, msgWork)) {
            wk->loginSeq++;
        }
        break;
    case LOGIN_END:
        wk->loginSeq = LOGIN_INTERNET_CONNECT;
        return 1;
    }
    return 2;
}






// pokebw2's Enter_LogoutCallback
static u32 func_ov135_021f8f5c(WifiLoginMessage *msgWork, void *data) {
    func_ov114_021c8658();
    return 0;
}

/* Corrected struct WorldTradeWork: added subOutFlag at 0xeb4 (was part of unkEB4) */
int func_ov135_021f8f68(WorldTradeWork *wk) {
    func_ov135_021f4fc4(wk);
    wk->subprocessSeq = ENTER_START;
    wk->subOutFlag = 1;
    return WT_SEQ_FADEOUT;
}

/* Corrected struct WorldTradeWork: added connectErrorNo at 0x38 and msgManager at 0xb6c */
int func_ov135_021f8f84(WorldTradeWork *wk, void *msgWork) {
    int msgNo;

    switch (wk->connectErrorNo) {
    case 1:
        msgNo = 0xa6;
        break;
    case 2:
        msgNo = 0xa7;
        break;
    case -1:
        msgNo = 0x9d;
        break;
    case -2:
        msgNo = 0xa9;
        break;
    case -3:
        msgNo = 0xa2;
        break;
    case -4:
        msgNo = 0xa4;
        break;
    case -5:
        msgNo = 0xa3;
        break;
    case -6:
        msgNo = 0x1b;
        break;
    case -7:
        msgNo = 0x1c;
        break;
    case -8:
        msgNo = 0x1d;
        break;
    case -9:
        msgNo = 0x1e;
        break;
    case -10:
        msgNo = 0x1f;
        break;
    case -11:
        msgNo = 0x20;
        break;
    case -12:
        msgNo = 0xa0;
        break;
    case -13:
        msgNo = 0xa5;
        break;
    case -14:
        msgNo = 0xaa;
        break;
    case -15:
        msgNo = 0xa1;
        break;
    default:
        msgNo = 0x9f;
        break;
    }
    func_ov115_021d5b5c(msgWork, wk->msgManager, msgNo);
    wk->localSeq = 0;
    return TRUE;
}

int func_ov135_021f9024(WorldTradeWork *wk, void *msgWork) {
    switch (wk->localSeq) {
    case 0:
        wk->localSeq++;
        break;
    case 1:
        if (func_ov115_021d5c70(msgWork)) {
            wk->localSeq++;
        }
        break;
    case 2:
        wk->localSeq++;
        break;
    case 3:
        func_ov114_021c8658();
        DWC_ShutdownFriendsMatch();
        func_0205cc20();
        wk->localSeq++;
        break;
    default:
        wk->localWait++;
        if (wk->localWait > 60) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}




// pokebw2's Enter_MessagePrint
void func_ov135_021f9088(WorldTradeWork *wk, MsgData *msgManager, int msgNo, int wait, u16 dat) {
    func_ov135_021f90b8(wk, msgManager, msgNo, wait, dat, TRUE);
}

// pokebw2's Enter_MessagePrintNoStream
void func_ov135_021f90a0(WorldTradeWork *wk, MsgData *msgManager, int msgNo, int wait, u16 dat) {
    func_ov135_021f90b8(wk, msgManager, msgNo, wait, dat, FALSE);
}

void func_ov135_021f90b8(WorldTradeWork *wk, MsgData *msgManager, int msgNo, int wait, u16 dat, BOOL stream) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgManager, msgNo);

    GFL_WordSetFormatStrbuf(wk->wordSet, wk->talkString, str);
    GFL_StrBufFree(str);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWin), 15);
    BmpWin_FlushChar(wk->msgWin);
    BmpWin_DrawFrame(wk->msgWin, 0, 1, 14);
    if (stream) {
        func_ov135_02202878(wk->msgWin, 0, wk->talkString, 0, 0, &wk->print);
    } else {
        func_ov135_02202858(wk->msgWin, 0, wk->talkString, 0, 0, &wk->print);
    }
    wk->wait = 0;
}


int func_ov135_022026c4(WorldTradePrint *print, u8 font, StrBuf *str, int unused);
int func_ov135_021f914c(BmpWin *win, StrBuf *str, int x, int flag, u16 color, int font,
                               WorldTradePrint *print) {
    int length;

    switch (flag) {
    case 1:
        length = func_ov135_022026c4(print, font, str, 0);
        x = (func_02045738(win) * 8 - length) / 2;
        break;
    case 2:
        length = func_ov135_022026c4(print, font, str, 0);
        x = func_02045738(win) * 8 - length;
        break;
    }
    return x;
}

void func_ov135_021f91a0(BmpWin *win, StrBuf *str, int x, int y, int flag, u16 color, WorldTradePrint *print) {
    x = func_ov135_021f914c(win, str, x, flag, color, 0, print);
    func_ov135_022028c8(win, 0, str, x, y, 0, color, print);
}

void func_ov135_021f91dc(BmpWin *win, StrBuf *str, int x, int y, int flag, u16 color, WorldTradePrint *print) {
    x = func_ov135_021f914c(win, str, x, flag, color, 0, print);
    func_ov135_022028c8(win, 0, str, x, y, 0, color, print);
}

// The message IDs that the explanation screens print
static const u32 data_ov135_022031ec[] = { 0xf7, 0xf8, 0xf9, 0xfa, 0xfb };
void func_ov135_021f9218(BmpWin *win, MsgData *msgManager, int no, WorldTradePrint *print) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgManager, data_ov135_022031ec[no]);

    func_ov135_021f91a0(win, str, 0, 0, 0, 0x440, print);
    GFL_StrBufFree(str);
}





// pokebw2's sExplainMsgTable

// pokebw2's WorldTrade_WifiIconAdd
void func_ov135_021f9254(WorldTradeWork *wk) {
    func_0203d34c(FALSE, HEAPID_WORLDTRADE);
}
