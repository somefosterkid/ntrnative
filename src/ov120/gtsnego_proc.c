// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov195/gtsnego_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// gtsnego_proc.c, in overlay 195: the process of GTS Negotiations. The player picks a trade with anyone, with the
// search's settings, or a rendezvous with someone of the Pal Pad; it searches for a partner with matching keys, and
// the two machines exchange their settings, player info, greeting and Join Avenue person before the trade's process
// takes over. The ROM has no file name for it, so it is named after its process, as pokemontrade_proc.c is (a guess).
// It draws with gtsnego_disp.c and prints with gtsnego_message.c. Our names

#include "types.h"
#include "app/gtsnego_local.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "dwc/dwc.h"
#include "gfl/bg_sys.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_state.h"
#include "gfl/net_system.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/game_data.h"
#include "system/pms.h"
#include "system/pms_data.h"
#include "system/wipe.h"
#include "text/system/0045.h"
#include "text/system/ability_handlers_btl_main.h"
#include "app/gtsnego.h"
#include "system/app_taskmenu.h"

// The network commands, and the timings the machines synchronize on
#define GTSNEGO_NET_CMD_BASE 0x2200
enum {
    GTSNEGO_NET_CMD_REQUEST = GTSNEGO_NET_CMD_BASE,
    GTSNEGO_NET_CMD_PLAYER_INFO,
    GTSNEGO_NET_CMD_PHRASE,
};
#define GTSNEGO_TIMING_NO 0x22

// The name that replaces a partner's name with bad words
#define GTSNEGO_DEFAULT_NAME_MSG AbilityHandlersBtlMain_Text_VersionName

// The search's kinds
#define GTSNEGO_MODE_ANYONE 0
#define GTSNEGO_MODE_RENDEZVOUS 1

// The level range a rendezvous with anyone of the Pal Pad searches with
#define GTSNEGO_LEVEL_RENDEZVOUS 4

// The icon of the touch bar that goes back
#define GTSNEGO_TOUCHBAR_RETURN 1

// The search's matching keys
enum {
    GTSNEGO_KEY_MY_ID,
    GTSNEGO_KEY_MODE,
    GTSNEGO_KEY_LEVEL,
    GTSNEGO_KEY_MY_KIND,
    GTSNEGO_KEY_WANT_KIND,
    GTSNEGO_KEY_ROM,
    GTSNEGO_KEY_DEBUG,
    GTSNEGO_KEY_FRIEND_ID,
    GTSNEGO_KEY_COUNT,
};

typedef struct GtsNegoWork GtsNegoWork;
typedef void (*GtsNegoStateFunc)(GtsNegoWork *wk);

typedef struct {
    int value;
    u8 keyId;
    u8 unk5;
    char key[6];
} GtsNegoMatchKey;

// What the machines show of each other: the greeting and how many trades the player has made
typedef struct {
    PMSData phrase;
    u32 tradeCount;
} GtsNegoGreeting;

// The partner's name, for the server's check for bad words
typedef struct {
    u16 name[12];
    const u16 *words;
    char result;
    int badCount;
    u16 unk24;
} GtsNegoNameCheck;

struct GtsNegoWork {
    GtsNegoStateFunc state;
    // What a touched button of the lower screen does
    BOOL (*buttonCallback)(int button, GtsNegoWork *wk);
    u32 unk8;
    HeapID heapId;
    u32 unk10;
    int timer;
    // The person to meet
    u32 friendId;
    u8 unk1C[0x10];
    GameData *gameData;
    SaveControl *save;
    AppTaskMenuWin *button;
    AppTaskMenu *menu;
    GtsNegoDisp *disp;
    GtsNegoMessage *msg;
    WifiList *wifiList;
    GtsNegoParam *param;
    u8 unk4C[0xc];
    GtsNegoMatchKey keys[GTSNEGO_KEY_COUNT];
    char query[0x80];
    int mode;
    // The search's settings: 5 kinds of Pokémon each, and 4 level ranges
    int myKind;
    int wantKind;
    int level;
    // The person of the Pal Pad to meet, or -1 for any of them
    int friendIndex;
    u32 myId;
    GtsNegoScroll scroll;
    // The GTSNEGO_CURSOR_* of each menu
    int mainCursor;
    int searchCursor;
    int friendCursor;
    GtsNegoGreeting send;
    GtsNegoGreeting recv;
    u32 unk180;
    // With fewer than two Pokémon there is no trading
    BOOL notEnoughPokemon;
    GtsNegoNameCheck nameCheck;
    // The error screen was shown, and the frames since
    BOOL errorShown;
    u32 errorFrames;
};

static void func_ov120_021d4240(GtsNegoWork *wk, GtsNegoStateFunc state);
static void func_ov120_021d4244(GtsNegoWork *wk, GtsNegoStateFunc state, int line);
void func_ov120_021d4774(GtsNegoWork *wk);
static void func_ov120_021d46a4(GtsNegoWork *wk);
static void func_ov120_021d4988(GtsNegoWork *wk);
void func_ov120_021d44ec(GtsNegoWork *wk);
void func_ov120_021d4864(GtsNegoWork *wk);
void func_ov120_021d44ac(GtsNegoWork *wk);
static void func_ov120_021d48f0(GtsNegoWork *wk);
static void func_ov120_021d5ad4(GtsNegoWork *wk);
void func_ov120_021d424c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov120_021d4278(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov120_021d42a8(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov120_021d435c(int delta, GtsNegoWork *wk);
void func_ov120_021d437c(int delta, GtsNegoWork *wk);
void func_ov120_021d439c(int delta, GtsNegoWork *wk);
static int func_ov120_021d43bc(int arrow, GtsNegoWork *wk);
BOOL func_ov120_021d444c(int button, GtsNegoWork *wk);
void func_ov120_021d4474(int dir, GtsNegoWork *wk);
int func_ov120_021d4a20(int index, void *work);
void func_ov120_021d7020(GtsNegoDisp *disp);
static void func_ov120_021d4b48(GtsNegoWork *wk);
void func_ov120_021d4c98(u32 button, u32 event, void *work);
void func_ov120_021d4cc4(GtsNegoWork *wk);
static void func_ov120_021d4d28(GtsNegoWork *wk);
void func_ov120_021d4ec0(GtsNegoWork *wk);
PlayerInfo *func_ov120_021d4f98(GameData *gameData, int index);
void func_ov120_021d4fc0(GtsNegoWork *wk);
void func_ov120_021d4fd0(GtsNegoWork *wk);
void func_ov120_021d4ff4(GtsNegoWork *wk);
void func_ov120_021d505c(u32 button, u32 event, void *work);
void func_ov120_021d50a0(GtsNegoWork *wk);
static void func_ov120_021d50c4(GtsNegoWork *wk);
void func_ov120_021d5188(GtsNegoWork *wk);
void func_ov120_021d51d4(GtsNegoWork *wk);
void func_ov120_021d5208(GtsNegoWork *wk);
int func_ov120_021d5230(GtsNegoWork *wk);
int func_ov120_021d5284(GtsNegoWork *wk);
void func_ov120_021d52d4(GtsNegoWork *wk);
void func_ov120_021d5318(GtsNegoWork *wk);
static void func_ov120_021d5334(GtsNegoWork *wk);
void func_ov120_021d5758(GtsNegoWork *wk);
static void func_ov120_021d57b0(GtsNegoWork *wk);
static void func_ov120_021d588c(GtsNegoWork *wk);
int func_ov120_021d593c(GtsNegoWork *wk, int cursor);
static int func_ov120_021d596c(GtsNegoWork *wk, int cursor);
static void func_ov120_021d5980(GtsNegoWork *wk);
static void func_ov120_021d59e0(GtsNegoWork *wk);
void func_ov120_021d5a84(GtsNegoWork *wk);
void func_ov120_021d5bec(GtsNegoWork *wk);
BOOL func_ov120_021d5c60(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov120_021d5d74(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov120_021d5e30(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov120_021d4584(GtsNegoWork *wk);
void func_ov120_021d47d4(GtsNegoWork *wk);

static char data_ov120_021d8b50[] = "ty=%d And db=%d";

static const NetCommand data_ov120_021d88ec[] = {
    { func_ov120_021d424c, NULL },
    { func_ov120_021d4278, NULL },
    { func_ov120_021d42a8, NULL },
};

// The names of the matching keys, two letters each
static char data_ov120_021d8b40[GTSNEGO_KEY_COUNT][2] = {
    { 'p', '1' }, { 't', 'y' }, { 'l', 'v' }, { 'm', 'y' }, { 'f', 'r' }, { 'r', 'm' }, { 'd', 'b' }, { 'p', '2' },
};


// pokebw2's GtsNego_SetState
static void func_ov120_021d4240(GtsNegoWork *wk, GtsNegoStateFunc state) {
    wk->state = state;
}

// Every state change gives the line it is on
// pokebw2's GtsNego_ChangeState
static void func_ov120_021d4244(GtsNegoWork *wk, GtsNegoStateFunc state, int line) {
    func_ov120_021d4240(wk, state);
}

// The partner's settings
// pokebw2's GtsNego_OnRecvRequest
void func_ov120_021d424c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    GtsNegoWork *wk = work;
    GtsNegoParam *param = wk->param;

    if (handle != func_020399a8()) {
        return;
    }
    if (netId == func_0203d1a4(func_020399a8())) {
        return;
    }
    MI_CpuCopy8(data, &param->unk4[1], sizeof(GtsNegoUnk4));
}

// pokebw2's GtsNego_OnRecvPlayerInfo
void func_ov120_021d4278(int netId, int size, const void *data, void *work, NetHandle *handle) {
    GtsNegoWork *wk = work;
    GtsNegoParam *param = wk->param;

    if (handle != func_020399a8()) {
        return;
    }
    if (netId == func_0203d1a4(func_020399a8())) {
        return;
    }
    MI_CpuCopy8(data, param->playerInfo2, func_02008468());
}

// pokebw2's GtsNego_OnRecvPhrase
void func_ov120_021d42a8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    GtsNegoWork *wk = work;

    if (handle != func_020399a8()) {
        return;
    }
    if (netId == func_0203d1a4(func_020399a8())) {
        return;
    }
    MI_CpuCopy8(data, &wk->recv, sizeof(GtsNegoGreeting));
}

BOOL func_ov120_021d42d8(int mode, GtsNegoWork *wk) {
    wk->mode = mode;
    func_020355d8(TRUE);
    if (wk->notEnoughPokemon == TRUE) {
        GFL_SndSEPlay(0x551);
        return TRUE;
    }
    if (mode == 0) {
        wk->mainCursor = 0;
        func_ov120_021d69f4(wk->disp, wk->button, 0);
        func_ov120_021d593c(wk, wk->mainCursor);
        func_ov120_021d4244(wk, func_ov120_021d5980, 375);
    } else {
        wk->mainCursor = 1;
        func_ov120_021d69f4(wk->disp, wk->button, 1);
        func_ov120_021d593c(wk, wk->mainCursor);
        func_ov120_021d4244(wk, func_ov120_021d5980, 382);
    }
    return TRUE;
}

void func_ov120_021d435c(int delta, GtsNegoWork *wk) {
    wk->myKind += delta;
    if (wk->myKind >= 5) {
        wk->myKind = 0;
    } else if (wk->myKind < 0) {
        wk->myKind = 4;
    }
}

// pokebw2's GtsNego_AddWantKind
void func_ov120_021d437c(int delta, GtsNegoWork *wk) {
    wk->wantKind += delta;
    if (wk->wantKind >= 5) {
        wk->wantKind = 0;
    } else if (wk->wantKind < 0) {
        wk->wantKind = 4;
    }
}

// pokebw2's GtsNego_AddLevel
void func_ov120_021d439c(int delta, GtsNegoWork *wk) {
    wk->level += delta;
    if (wk->level >= 4) {
        wk->level = 0;
    } else if (wk->level < 0) {
        wk->level = 3;
    }
}

// Changes the setting of one of the six arrows, left then right of each, and returns the setting's cursor
// pokebw2's GtsNego_PressArrow
static int func_ov120_021d43bc(int arrow, GtsNegoWork *wk) {
    int cursor = 0;

    switch (arrow) {
    case 0:
        func_ov120_021d439c(-1, wk);
        cursor = GTSNEGO_CURSOR_LEVEL;
        func_ov120_021d7e88(wk->msg, wk->level);
        break;
    case 1:
        func_ov120_021d439c(1, wk);
        cursor = GTSNEGO_CURSOR_LEVEL;
        func_ov120_021d7e88(wk->msg, wk->level);
        break;
    case 2:
        func_ov120_021d435c(-1, wk);
        cursor = GTSNEGO_CURSOR_MY_KIND;
        func_ov120_021d7e9c(wk->msg, wk->myKind);
        break;
    case 3:
        func_ov120_021d435c(1, wk);
        cursor = GTSNEGO_CURSOR_MY_KIND;
        func_ov120_021d7e9c(wk->msg, wk->myKind);
        break;
    case 4:
        func_ov120_021d437c(-1, wk);
        cursor = GTSNEGO_CURSOR_WANT_KIND;
        func_ov120_021d7eb0(wk->msg, wk->wantKind);
        break;
    case 5:
        func_ov120_021d437c(1, wk);
        cursor = GTSNEGO_CURSOR_WANT_KIND;
        func_ov120_021d7eb0(wk->msg, wk->wantKind);
        break;
    }
    if (arrow >= 0 && arrow <= 5) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov120_021d6b84(wk->disp, arrow);
    }
    return cursor;
}

// An arrow of the search's settings was touched
// pokebw2's GtsNego_OnSearchButton
BOOL func_ov120_021d444c(int button, GtsNegoWork *wk) {
    int cursor;

    if (wk->button != NULL) {
        func_02028218(wk->button, FALSE);
    }
    cursor = func_ov120_021d43bc(button, wk);
    if (cursor != 0) {
        wk->searchCursor = cursor;
    }
    return TRUE;
}

// Left or right on the setting the cursor is on
// pokebw2's GtsNego_PressArrowOfCursor
void func_ov120_021d4474(int dir, GtsNegoWork *wk) {
    int cursor = wk->searchCursor;
    int setting = cursor - GTSNEGO_CURSOR_LEVEL;

    // BUG: the check is always true, so the search button's cursor gives an arrow past the last, which does nothing
    if (cursor >= GTSNEGO_CURSOR_LEVEL || cursor <= GTSNEGO_CURSOR_WANT_KIND) {
        func_ov120_021d43bc(dir + setting * 2, wk);
    }
}

void func_ov120_021d4490(GtsNegoWork *wk) {
    if (GFL_WipeIsFinished()) {
        func_ov120_021d4244(wk, NULL, 531);
    }
}

void func_ov120_021d44ac(GtsNegoWork *wk) {
    wk->timer--;
    if (wk->timer <= 0) {
        GFL_WipeSet(0, 0, 0, 0, 6, 1, wk->heapId);
        func_ov120_021d4244(wk, func_ov120_021d4490, 542);
    }
}


void func_ov120_021d44ec(GtsNegoWork *wk) {
    PMSData local = {1, 1, {1, 1}};
    PMSData *phrase = &local;
    if (func_0203cd98()) {
        phrase = &wk->recv.phrase;
    }
    wk->timer = 180;
    func_ov120_021d8430(wk->msg, wk->disp);
    func_ov120_021d8484(wk->msg, phrase);
    func_ov120_021d4244(wk, func_ov120_021d44ac, 562);
}


void func_ov120_021d4544(GtsNegoWork *wk) {
    wk->timer--;
    if (wk->timer == 20) {
        func_ov120_021d7a74(wk->msg, 39);
    }
    if (wk->timer == 0) {
        func_ov120_021d7f5c(wk->msg, 35);
        func_ov120_021d80d4(wk->msg);
        func_ov120_021d4244(wk, func_ov120_021d44ec, 577);
    }
}

static void func_ov120_021d4584(GtsNegoWork *wk) {
    GtsNegoParam *param = wk->param;
    PlayerInfo *info;
    int count = 2;

    if (func_0203cd98() == 0) {
        info = GetGameDataPlayerInfo(param->gameData);
    } else {
        info = param->playerInfo2;
        count = wk->recv.tradeCount;
        param->unk1C[param->unk1AC] = getIDAsUInt(info);
        param->unk1AC++;
        if (param->unk1AC >= 100) {
            param->unk1AC = 0;
        }
    }
    func_ov120_021d880c(wk->msg, info, count);
    func_ov120_021d74b0(wk->disp, func_02008554(info));
    wk->timer = 180;
    func_ov120_021d4244(wk, func_ov120_021d4544, 613);
}

void func_ov120_021d4608(GtsNegoWork *wk) {
    PlayerInfo *info = wk->param->playerInfo2;
    BOOL named = FALSE;
    BOOL connected;
    MsgData *msgData;
    StrBuf *name;
    PlayerInfo *dest;
    u32 id;
    RivalDataSave *rivalData;

    connected = DWC_CheckProfanityProcess() == 2;
    if (connected) {
        named = wk->nameCheck.badCount > 0;
    }
    if (connected) {
        if (named) {
            msgData = GFL_MsgSysLoadData(0, 2, 0x51, 0x1f);
            name = GFL_MsgDataLoadStrbufNew(msgData, GTSNEGO_DEFAULT_NAME_MSG);
            GFL_MsgDataFree(msgData);
            copyTrainerNameFromStrbuf(info, name);
            GFL_StrBufFree(name);
        }
        dest = func_02012950(wk->gameData, 1 - func_02039338());
        func_02008494(info, dest);
        id = getIDAsUInt(info);
        rivalData = getHollow_RivalData(wk->save);
        func_0200ed54(rivalData, id);
        func_ov120_021d4244(wk, func_ov120_021d4584, 643);
    }
}

static void func_ov120_021d46a4(GtsNegoWork *wk) {
    PlayerInfo *info = wk->param->playerInfo2;
    NetHandle *handle = func_020399a8();
    if (func_02039cb4(handle, 3, 0x22)) {
        StrBuf *name = copyTrainerNameToNewStrbuf(info->name, 0x1f);
        const u16 *str;
        u32 i = 0;

        MI_CpuFill8(&wk->nameCheck, 0, sizeof(GtsNegoNameCheck));
        wk->nameCheck.unk24 = 0x1f;
        str = GFL_StrBufGetStringPtr(name);
        if (GFL_StrBufGetCharCount(name) + 1 != 0) {
            do {
                u16 c = str[i];
                if (c == GFL_StrBufGetTerminator()) {
                    wk->nameCheck.name[i] = 0;
                } else {
                    wk->nameCheck.name[i] = c;
                }
                i++;
            } while (i < GFL_StrBufGetCharCount(name) + 1);
        }
        wk->nameCheck.words = wk->nameCheck.name;
        wk->nameCheck.badCount = 0;
        func_ov009_0217bc44(&wk->nameCheck.words, 1, 0, 0, &wk->nameCheck.result, &wk->nameCheck.badCount, 0x80);
        GFL_StrBufFree(name);
        func_ov120_021d4244(wk, func_ov120_021d4608, 0x29c);
    }
}




void func_ov120_021d4774(GtsNegoWork *wk) {
    func_02039c68(func_020399a8(), 3, 0x22);
    func_ov120_021d4244(wk, func_ov120_021d46a4, 684);
}



void func_ov120_021d4798(GtsNegoWork *wk) {
    NetHandle *handle = func_020399a8();
    u32 size = func_02008468();
    if (func_0203d3a8(handle, GTSNEGO_NET_CMD_PLAYER_INFO, (u16)size, wk->param->playerInfo)) {
        func_ov120_021d4244(wk, func_ov120_021d4774, 701);
    }
}

void func_ov120_021d47d4(GtsNegoWork *wk) {
    if (func_0203d3a8(func_020399a8(), 0x2202, 0xc, &wk->send)) {
        func_ov120_021d4244(wk, func_ov120_021d4798, 719);
    }
}


void func_ov120_021d4808(GtsNegoWork *wk) {
    NetHandle *handle;
    if (!func_0203cd98()) {
        func_ov120_021d4244(wk, func_ov120_021d4584, 735);
        return;
    }
    handle = func_020399a8();
    if (func_02039cb4(handle, 2, GTSNEGO_TIMING_NO)) {
        handle = func_020399a8();
        if (func_0203d3a8(handle, GTSNEGO_NET_CMD_REQUEST, 8, wk->param->unk4)) {
            func_ov120_021d4244(wk, func_ov120_021d47d4, 743);
        }
    }
}

void func_ov120_021d4864(GtsNegoWork *wk) {
    GtsNegoParam *param;
    NetHandle *handle;
    void *cgear;
    if (func_ov120_021d7af4(wk->msg)) {
        if (!func_0203cd98()) {
            func_ov120_021d4244(wk, func_ov120_021d4808, 766);
            return;
        }
        if (wk->timer > 0) {
            wk->timer--;
            return;
        }
        if (func_020399ec() == 2) {
            param = wk->param;
            cgear = func_0200e6e8(wk->save);
            param->unk4[0].unk0 = wk->level;
            param->unk4[0].unk4 = wk->myKind;
            func_0200e6fc(cgear, 0, &wk->send.phrase);
            wk->send.tradeCount = func_02009ea8(func_02012f14(wk->gameData));
            handle = func_020399a8();
            func_02039c68(handle, 2, GTSNEGO_TIMING_NO);
            func_ov120_021d4244(wk, func_ov120_021d4808, 788);
        }
    }
}



void func_ov120_021d48f0(GtsNegoWork *wk) {
    if (func_0203cd98() == 0) {
        func_ov120_021d4244(wk, func_ov120_021d4864, 797);
    } else if (func_020399ec() != 0) {
        if (func_02039acc() != 0) {
            func_ov120_021d4244(wk, func_ov120_021d4864, 802);
        }
    }
}

void func_ov120_021d4930(GtsNegoWork *wk) {
    if (func_ov120_021d7af4(wk->msg)) {
        if (wk->timer > 0) {
            wk->timer--;
            return;
        }
        if (func_02039338()) {
            if (func_02039acc()) {
                wk->timer = 180;
                func_ov120_021d4244(wk, func_ov120_021d4864, 831);
            }
        } else {
            wk->timer = 180;
            func_ov120_021d4244(wk, func_ov120_021d48f0, 836);
        }
    }
}

static void func_ov120_021d4988(GtsNegoWork *wk) {
    func_ov120_021d7c10(wk->msg);
    if (GCTX_HIDGetPressedKeys() == 2) {
        func_ov120_021d8638(wk->msg);
        if (func_0203cd98()) {
            func_0203c554(TRUE);
        }
        func_020355d8(FALSE);
        GFL_SndSEPlay(0x551);
        func_ov120_021d4244(wk, func_ov120_021d4ff4, 864);
    }
    if (!func_0203cd98()) {
        if (GCTX_HIDGetPressedKeys() != 1) {
            return;
        }
    } else {
        if (func_0203c3ec() != 1) {
            return;
        }
        func_0203d7b0(1);
        func_0203d7bc(TRUE);
    }
    func_ov120_021d7b38(wk->msg);
    func_ov120_021d7a74(wk->msg, 16);
    wk->timer = 180;
    func_ov120_021d8648(wk->msg);
    func_ov120_021d4244(wk, func_ov120_021d4930, 887);
}

int func_ov120_021d4a20(int index, void *work) {
    GtsNegoWork *wk = work;
    GtsNegoParam *param = wk->param;
    int score = -1;
    int level = DWC_GetMatchIntValue(index, wk->keys[2].key, -1);
    int partnerWantKind = DWC_GetMatchIntValue(index, wk->keys[4].key, -1);
    int partnerMyKind = DWC_GetMatchIntValue(index, wk->keys[3].key, -1);
    u32 id = DWC_GetMatchIntValue(index, wk->keys[0].key, 0);
    u32 friendId = DWC_GetMatchIntValue(index, wk->keys[7].key, 0);
    int i;

    if (wk->mode == GTSNEGO_MODE_RENDEZVOUS) {
        if (level == GTSNEGO_LEVEL_RENDEZVOUS) {
            if (wk->friendIndex == -1) {
                if (func_02009e74(getPalPadFriendListAddress(wk->save), id)) {
                    score = 100;
                }
            } else if (wk->friendId == id) {
                score = 100;
            }
        } else if (friendId == getIDAsUInt(GetGameDataPlayerInfo(param->gameData))) {
            if (wk->friendIndex == -1) {
                if (func_02009e74(getPalPadFriendListAddress(wk->save), id)) {
                    score = 100;
                }
            } else if (wk->friendId == id) {
                score = 100;
            }
        }
    } else if (wk->level == level) {
        if (wk->myKind == partnerWantKind) {
            score += 20;
        } else if (wk->myKind == 0 || partnerWantKind == 0) {
            score += 10;
        } else {
            score -= 40;
        }
        if (wk->wantKind == partnerMyKind) {
            score += 20;
        } else if (partnerMyKind == 0 || wk->wantKind == 0) {
            score += 10;
        } else {
            score -= 40;
        }
        for (i = 0; i < GTSNEGO_MET_COUNT; i++) {
            if (id == param->unk1C[i] && id != 0) {
                score = 0;
                break;
            }
        }
    }
    return score;
}

static void func_ov120_021d4b48(GtsNegoWork *wk) {
    int values[GTSNEGO_KEY_COUNT];
    PlayerInfo *friend;
    int i;

    if (!func_ov120_021d7af4(wk->msg)) {
        return;
    }
    if (!func_0203cd98()) {
        func_ov120_021d8590(wk->msg, func_ov120_021d505c, wk);
        func_ov120_021d4244(wk, func_ov120_021d4988, 1002);
        return;
    }
    func_0203d644(FALSE);
    func_02009ca8(&wk->wifiList->myUserData, (DWCFriendData *)values);
    values[GTSNEGO_KEY_MY_ID] = wk->myId;
    values[GTSNEGO_KEY_MODE] = wk->mode;
    values[GTSNEGO_KEY_LEVEL] = wk->level;
    values[GTSNEGO_KEY_MY_KIND] = wk->myKind;
    values[GTSNEGO_KEY_WANT_KIND] = wk->wantKind;
    values[GTSNEGO_KEY_ROM] = 0x20000 | GAME_VERSION;
    values[GTSNEGO_KEY_DEBUG] = 1;
    values[GTSNEGO_KEY_FRIEND_ID] = 0;
    if (wk->mode == GTSNEGO_MODE_RENDEZVOUS) {
        if (wk->friendIndex == -1) {
            values[GTSNEGO_KEY_LEVEL] = GTSNEGO_LEVEL_RENDEZVOUS;
        } else {
            friend = func_ov120_021d4f70(wk->gameData, wk->friendIndex);
            values[GTSNEGO_KEY_FRIEND_ID] = getIDAsUInt(friend);
            wk->friendId = getIDAsUInt(friend);
        }
    }
    for (i = 0; i < GTSNEGO_KEY_COUNT; i++) {
        GtsNegoMatchKey *key = &wk->keys[i];

        MI_CpuCopy8(data_ov120_021d8b40[i], key->key, 2);
        key->value = values[i];
        key->keyId = DWC_AddMatchKeyInt(key->keyId, key->key, &key->value);
    }
    STD_TSPrintf(wk->query, data_ov120_021d8b50, wk->mode, 1);
    if (func_ov009_0215580c(wk->query, 2, func_ov120_021d4a20, wk)) {
        func_0203c35c();
        func_ov009_02156bc4(0);
        func_ov120_021d8590(wk->msg, func_ov120_021d505c, wk);
        func_ov120_021d4244(wk, func_ov120_021d4988, 1053);
    } else {
        func_0203c9ac(0, 0, 0, 1009);
    }
}








// A button of the lower screen's menu was touched
// pokebw2's GtsNego_OnButton
void func_ov120_021d4c98(u32 button, u32 event, void *work) {
    GtsNegoWork *wk = work;

    if (event == 0) {
        if (wk->buttonCallback != NULL) {
            func_ov120_021d69f4(wk->disp, wk->button, GTSNEGO_CURSOR_NONE);
            func_020355d8(TRUE);
            if (wk->buttonCallback(button, wk)) {
                return;
            }
        }
    }
}

// Once the search button has flashed, shows the search
// pokebw2's GtsNego_StateWaitSearchPressed
void func_ov120_021d4cc4(GtsNegoWork *wk) {
    if (func_02028230(wk->button)) {
        func_02028148(wk->button);
        wk->button = NULL;
        GFL_BGSysQueueScrLoad(5);
        func_ov120_021d79f4(wk->msg, Bank0045_Text_014);
        func_ov120_021d7550(wk->disp);
        func_ov120_021d8520(wk->msg, Bank0045_Text_013);
        func_ov120_021d63f4(wk->disp, TRUE, GTSNEGO_DIM_SEARCH);
        wk->param->result = GTSNEGO_RESULT_ANYONE;
        func_ov120_021d69f4(wk->disp, NULL, GTSNEGO_CURSOR_NONE);
        func_ov120_021d4244(wk, func_ov120_021d4b48, 1120);
    }
}

static void func_ov120_021d4d28(GtsNegoWork *wk) {
    static const TouchRect data_ov120_021d88d0[] = {
        { 0xa0, 0xc0, 0x50, 0xb0 },
        { TOUCH_RECT_END },
    };
    BOOL moved = FALSE;
    BOOL back = FALSE;
    TouchBar *bar = func_ov120_021d6988(wk->disp);

    if (!func_ov120_021d7af4(wk->msg)) {
        return;
    }
    if (!func_ov067_021b6498(bar)) {
        func_ov120_021d7c10(wk->msg);
        if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == 1) {
            func_ov120_021d69f4(wk->disp, wk->button, wk->searchCursor);
            func_020355d8(FALSE);
            return;
        }
        if (GCTX_HIDGetPressedKeys() == PAD_KEY_LEFT) {
            func_ov120_021d4474(0, wk);
        }
        if (GCTX_HIDGetPressedKeys() == PAD_KEY_RIGHT) {
            func_ov120_021d4474(1, wk);
        }
        if (GCTX_HIDGetPressedKeys() == PAD_KEY_UP) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            moved = TRUE;
            if (wk->searchCursor != GTSNEGO_CURSOR_LEVEL) {
                wk->searchCursor--;
            }
        }
        if (GCTX_HIDGetPressedKeys() == PAD_KEY_DOWN) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            moved = TRUE;
            if (wk->searchCursor != GTSNEGO_CURSOR_SEARCH) {
                wk->searchCursor++;
            }
        }
        if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A && wk->searchCursor == GTSNEGO_CURSOR_SEARCH) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov120_021d6a9c(wk->disp, wk->searchCursor);
            func_0202822c(wk->button, TRUE);
            func_ov120_021d4244(wk, func_ov120_021d4cc4, 1226);
            return;
        }
        if (moved) {
            func_ov120_021d69f4(wk->disp, wk->button, wk->searchCursor);
        }
        if (func_02035c08(data_ov120_021d88d0) == 0) {
            func_020355d8(TRUE);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_0202822c(wk->button, TRUE);
            func_ov120_021d4244(wk, func_ov120_021d4cc4, 1266);
            return;
        }
    }
    func_ov067_021b63d0(func_ov120_021d6988(wk->disp));
    if (func_ov067_021b6448(func_ov120_021d6988(wk->disp)) == 1) {
        back = TRUE;
    }
    if (back) {
        GFL_BGSysSetBGEnabled(4, FALSE);
        func_ov120_021d69f4(wk->disp, wk->button, GTSNEGO_CURSOR_NONE);
        func_ov120_021d7ec4(wk->msg);
        func_ov120_021d68c4(wk->disp);
        func_02028148(wk->button);
        wk->button = NULL;
        GFL_BGSysQueueScrLoad(5);
        func_ov120_021d4244(wk, func_ov120_021d588c, 1291);
    }
}


void func_ov120_021d4ec0(GtsNegoWork *wk) {
    wk->mode = GTSNEGO_MODE_ANYONE;
    func_ov067_021b64d0(func_ov120_021d6988(wk->disp), TRUE);
    func_ov120_021d7d3c(wk->msg);
    func_ov120_021d6814(wk->disp);
    wk->buttonCallback = func_ov120_021d444c;
    func_ov120_021d7dc8(wk->msg, func_ov120_021d4c98, wk, wk->level, wk->myKind, wk->wantKind);
    func_ov120_021d79f4(wk->msg, 20);
    wk->button = func_ov120_021d7ecc(wk->msg, 18);
    if (!func_020355c4()) {
        func_ov120_021d69f4(wk->disp, NULL, wk->searchCursor);
        if (wk->searchCursor == GTSNEGO_CURSOR_SEARCH) {
            func_02028218(wk->button, TRUE);
        }
    } else {
        func_ov120_021d69f4(wk->disp, NULL, GTSNEGO_CURSOR_NONE);
    }
    GFL_BGSysQueueScrLoad(6);
    func_ov120_021d4244(wk, func_ov120_021d4d28, 1335);
}


// pokebw2's GtsNego_GetFriend
PlayerInfo *func_ov120_021d4f70(GameData *gameData, int index) {
    int count = func_02009e8c(func_02012f14(gameData));

    if (index >= count || index < 0) {
        return NULL;
    }
    return func_02009e54(func_02012f14(gameData), index);
}

// The person at index, or the first or last when it is past the list's ends
// pokebw2's GtsNego_GetFriendClamped
PlayerInfo *func_ov120_021d4f98(GameData *gameData, int index) {
    int count = func_02009e8c(func_02012f14(gameData));

    if (index < 0) {
        index = 0;
    }
    if (index >= count) {
        index = count - 1;
    }
    return func_02009e54(func_02012f14(gameData), index);
}

// pokebw2's GtsNego_StateReturnToMainMenu
void func_ov120_021d4fc0(GtsNegoWork *wk) {
    func_ov120_021d588c(wk);
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_ALL);
}

// pokebw2's GtsNego_StateResetScreens
void func_ov120_021d761c(GtsNegoDisp *disp);
void func_ov120_021d883c(GtsNegoMessage *msg);

void func_ov120_021d4fd0(GtsNegoWork *wk) {
    func_ov120_021d761c(wk->disp);
    func_ov120_021d883c(wk->msg);
    func_ov120_021d4244(wk, func_ov120_021d4fc0, 1477);
}

// Once the cancel button has flashed, leaves the search
// pokebw2's GtsNego_StateWaitCancelPressed
void func_ov120_021d4ff4(GtsNegoWork *wk) {
    if (func_ov120_021d8664(wk->msg)) {
        GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_BG3);
        func_ov120_021d7ec4(wk->msg);
        func_ov120_021d68c4(wk->disp);
        func_ov120_021d63f4(wk->disp, FALSE, GTSNEGO_DIM_ALL);
        func_ov120_021d69f4(wk->disp, NULL, GTSNEGO_CURSOR_NONE);
        GFL_BGSysQueueScrLoad(5);
        if (wk->mode == GTSNEGO_MODE_RENDEZVOUS) {
            GFL_BGSysSetBGEnabled(4, FALSE);
            func_ov120_021d6d94(wk->disp);
        }
        func_ov120_021d4244(wk, func_ov120_021d4fd0, 1501);
    }
}

// The cancel button was touched
// pokebw2's GtsNego_OnCancelButton
void func_ov120_021d505c(u32 button, u32 event, void *work) {
    GtsNegoWork *wk = work;

    if (event == 0) {
        if (wk->state == func_ov120_021d4988) {
            func_ov120_021d8638(wk->msg);
            func_0203c554(TRUE);
            func_020355d8(TRUE);
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov120_021d4244(wk, func_ov120_021d4ff4, 1525);
        }
    }
}

// pokebw2's GtsNego_StateRecreateFriendButton

void func_ov120_021d50a0(GtsNegoWork *wk) {
    wk->button = func_ov120_021d7ecc(wk->msg, Bank0045_Text_044);
    func_ov120_021d4244(wk, func_ov120_021d5334, 1535);
}

// The answer to meeting the person picked: search, or back to the list
// pokebw2's GtsNego_StateWaitFriendConfirm
static void func_ov120_021d50c4(GtsNegoWork *wk) {
    if (func_020279e8(wk->menu)) {
        int pos = func_02027a04(wk->menu);

        func_ov120_021d8418(wk->menu);
        wk->menu = NULL;
        func_ov067_021b6558(func_ov120_021d6988(wk->disp), GTSNEGO_TOUCHBAR_RETURN, TRUE);
        switch (pos) {
        case 0:
            func_ov120_021d79f4(wk->msg, Bank0045_Text_014);
            func_ov120_021d7550(wk->disp);
            func_ov120_021d8520(wk->msg, Bank0045_Text_013);
            wk->level = 0;
            wk->myKind = 0;
            wk->wantKind = 0;
            wk->param->result = GTSNEGO_RESULT_RENDEZVOUS;
            func_ov120_021d4244(wk, func_ov120_021d4b48, 1556);
            break;
        case 1:
            func_ov120_021d63f4(wk->disp, FALSE, GTSNEGO_DIM_ALL);
            func_ov120_021d79f4(wk->msg, Bank0045_Text_019);
            if (!func_020355c4()) {
                func_ov120_021d69f4(wk->disp, NULL, wk->friendCursor);
            } else {
                func_ov120_021d69f4(wk->disp, NULL, GTSNEGO_CURSOR_NONE);
            }
            func_ov120_021d6fd8(wk->disp, TRUE);
            func_ov120_021d4244(wk, func_ov120_021d50a0, 1568);
            break;
        }
    }
}

// pokebw2's GtsNego_StateOpenFriendConfirm

void func_ov120_021d5188(GtsNegoWork *wk) {
    if (func_ov120_021d7af4(wk->msg)) {
        func_ov067_021b6558(func_ov120_021d6988(wk->disp), GTSNEGO_TOUCHBAR_RETURN, FALSE);
        wk->menu = func_ov120_021d8358(wk->msg, 1);
        func_ov120_021d63f4(wk->disp, TRUE, GTSNEGO_DIM_LIST_MENU);
        func_ov120_021d6fd8(wk->disp, FALSE);
        func_ov120_021d4244(wk, func_ov120_021d50c4, 1588);
    }
}

// pokebw2's GtsNego_StatePrintFriendChosen

void func_ov120_021d51d4(GtsNegoWork *wk) {
    if (wk->friendIndex == -1) {
        func_ov120_021d79f4(wk->msg, Bank0045_Text_045);
    } else {
        func_ov120_021d79f4(wk->msg, Bank0045_Text_031);
    }
    func_ov120_021d4244(wk, func_ov120_021d5188, 1606);
}

// pokebw2's GtsNego_StateLeaveFriendList
void func_ov120_021d6d94(GtsNegoDisp *disp);



void func_ov120_021d5208(GtsNegoWork *wk) {
    GFL_BGSysSetBGEnabled(4, FALSE);
    func_ov120_021d6d94(wk->disp);
    func_ov120_021d4244(wk, func_ov120_021d588c, 1614);
}

// Up in the list of people: moves the cursor, or scrolls the list, the person above coming in
// pokebw2's GtsNego_FriendListUp
int func_ov120_021d5230(GtsNegoWork *wk) {
    int moved = func_ov120_021d6e50(wk->disp, &wk->scroll);

    switch (moved) {
    case GTSNEGO_SCROLL_NONE:
        break;
    case GTSNEGO_SCROLL_CURSOR:
        if (wk->friendCursor != GTSNEGO_CURSOR_FRIEND_1) {
            wk->friendCursor--;
        }
        break;
    case GTSNEGO_SCROLL_LIST:
        func_ov120_021d6ff8(wk->disp, GTSNEGO_LIST_DOWN);
        if (wk->scroll.top - 1 >= 0) {
            func_ov120_021d82d8(wk->msg, wk->gameData, wk->scroll.top - 1);
        }
        break;
    }
    return moved;
}

// pokebw2's GtsNego_FriendListDown
int func_ov120_021d5284(GtsNegoWork *wk) {
    int moved = func_ov120_021d6e1c(wk->disp, &wk->scroll);

    switch (moved) {
    case GTSNEGO_SCROLL_NONE:
        break;
    case GTSNEGO_SCROLL_CURSOR:
        if (wk->friendCursor != GTSNEGO_CURSOR_FRIEND_3) {
            wk->friendCursor++;
        }
        break;
    case GTSNEGO_SCROLL_LIST:
        func_ov120_021d6ff8(wk->disp, GTSNEGO_LIST_UP);
        func_ov120_021d822c(wk->msg, wk->gameData, wk->scroll.top + 2);
        break;
    }
    return moved;
}

// Once the button for anyone of the list has flashed, asks to confirm
// pokebw2's GtsNego_StateWaitAnyFriendPressed

void func_ov120_021d52d4(GtsNegoWork *wk) {
    if (func_02028230(wk->button)) {
        func_02028148(wk->button);
        wk->button = NULL;
        wk->friendIndex = -1;
        func_ov120_021d69f4(wk->disp, NULL, GTSNEGO_CURSOR_NONE);
        GFL_BGSysQueueScrLoad(5);
        func_ov120_021d4244(wk, func_ov120_021d51d4, 1722);
    }
}

// pokebw2's GtsNego_FreeButton
void func_ov120_021d5318(GtsNegoWork *wk) {
    if (wk->button != NULL) {
        func_02028148(wk->button);
        GFL_BGSysQueueScrLoad(5);
        wk->button = NULL;
    }
}

static void func_ov120_021d5334(GtsNegoWork *wk) {
    static const TouchRect data_ov120_021d88c8[] = {
        { 0x1a, 0x9e, 0xec, 0x00 },
        { TOUCH_RECT_END },
    };
    static const TouchRect data_ov120_021d88d8[] = {
        { 0x18, 0x48, 0x00, 0xec },
        { 0x48, 0x78, 0x00, 0xec },
        { 0x78, 0xa8, 0x00, 0xec },
        { 0xa8, 0xc0, 0x40, 0xc0 },
        { TOUCH_RECT_END },
    };
    int done = 0;
    int moved = 0;
    int scrolling;
    int i;
    int result;
    int touched;
    u32 x;
    u32 y;
    TouchBar *bar = func_ov120_021d6988(wk->disp);

    if (!func_ov120_021d7af4(wk->msg)) {
        return;
    }
    if (func_ov067_021b6498(bar)) {
        goto touchbar;
    }
    scrolling = func_ov120_021d702c(wk->disp, &done);
    if (done == GTSNEGO_LIST_DOWN) {
        func_ov120_021d6d10(wk->disp, wk->gameData, wk->scroll.top - 2);
        func_ov120_021d82f8(wk->msg);
        func_ov120_021d7260(wk->disp, func_ov120_021d4f98(wk->gameData, wk->scroll.top - 2));
    } else if (done == GTSNEGO_LIST_UP) {
        func_ov120_021d6d10(wk->disp, wk->gameData, wk->scroll.top - 2);
        func_ov120_021d824c(wk->msg);
        func_ov120_021d7220(wk->disp, func_ov120_021d4f98(wk->gameData, wk->scroll.top + 3));
    }
    if (scrolling != GTSNEGO_LIST_STILL) {
        return;
    }
    if (func_020355c4() == 1 && GCTX_HIDGetPressedKeys() != 0) {
        func_ov120_021d69f4(wk->disp, NULL, wk->friendCursor);
        func_020355d8(FALSE);
        return;
    }
    if (GCTX_HIDGetTypedKeys() == PAD_BUTTON_R) {
        moved = 0;
        for (i = 0; i < GTSNEGO_LIST_SHOWN; i++) {
            result = func_ov120_021d5284(wk);
            moved |= result;
            func_ov120_021d7020(wk->disp);
            if (result == GTSNEGO_SCROLL_NONE) {
                break;
            }
            if (result == GTSNEGO_SCROLL_CURSOR) {
                i--;
            } else {
                func_ov120_021d70c8(wk->disp, TRUE);
                func_ov120_021d6d10(wk->disp, wk->gameData, wk->scroll.top - 2);
                func_ov120_021d824c(wk->msg);
                func_ov120_021d7220(wk->disp, func_ov120_021d4f98(wk->gameData, wk->scroll.top + 3));
                func_ov120_021d69f4(wk->disp, NULL, wk->friendCursor);
            }
        }
    }
    if (GCTX_HIDGetTypedKeys() == PAD_BUTTON_L) {
        moved = 0;
        for (i = 0; i < GTSNEGO_LIST_SHOWN; i++) {
            result = func_ov120_021d5230(wk);
            moved |= result;
            func_ov120_021d7020(wk->disp);
            if (result == GTSNEGO_SCROLL_NONE) {
                break;
            }
            if (result == GTSNEGO_SCROLL_CURSOR) {
                i--;
            } else {
                func_ov120_021d69f4(wk->disp, NULL, wk->friendCursor);
                func_ov120_021d70c8(wk->disp, FALSE);
                func_ov120_021d6d10(wk->disp, wk->gameData, wk->scroll.top - 2);
                func_ov120_021d82f8(wk->msg);
                func_ov120_021d7260(wk->disp, func_ov120_021d4f98(wk->gameData, wk->scroll.top - 2));
            }
        }
    }
    if (GCTX_HIDGetTypedKeys() == PAD_KEY_UP) {
        moved = func_ov120_021d5230(wk);
    }
    if (GCTX_HIDGetTypedKeys() == PAD_KEY_DOWN) {
        moved = func_ov120_021d5284(wk);
    }
    if (moved) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov120_021d733c(wk->disp, wk->scroll.top + wk->friendCursor - GTSNEGO_CURSOR_FRIEND_1, wk->scroll.count);
        func_ov120_021d69f4(wk->disp, NULL, wk->friendCursor);
        return;
    }
    if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov120_021d6a9c(wk->disp, wk->friendCursor);
        wk->friendIndex = wk->scroll.top + wk->friendCursor - GTSNEGO_CURSOR_FRIEND_1;
        func_ov120_021d5318(wk);
        func_ov120_021d4244(wk, func_ov120_021d51d4, 1888);
        return;
    }
    if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_START) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_0202822c(wk->button, TRUE);
        func_ov120_021d4244(wk, func_ov120_021d52d4, 1894);
        return;
    }
    touched = func_02035c08(data_ov120_021d88d8);
    switch (touched) {
    case 3:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_0202822c(wk->button, TRUE);
        func_ov120_021d4244(wk, func_ov120_021d52d4, 1904);
        return;
    case 2:
        if (wk->scroll.count < 3) {
            break;
        }
        /* fall through */
    case 1:
        if (wk->scroll.count < 2) {
            break;
        }
        /* fall through */
    case 0:
        func_020355d8(TRUE);
        wk->friendCursor = touched + GTSNEGO_CURSOR_FRIEND_1;
        func_ov120_021d69f4(wk->disp, NULL, wk->friendCursor);
        func_ov120_021d6a9c(wk->disp, wk->friendCursor);
        wk->friendIndex = wk->scroll.top + wk->friendCursor - GTSNEGO_CURSOR_FRIEND_1;
        func_ov120_021d6e70(wk->disp, &wk->scroll, touched);
        func_ov120_021d733c(wk->disp, wk->scroll.top + wk->friendCursor - GTSNEGO_CURSOR_FRIEND_1, wk->scroll.count);
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov120_021d5318(wk);
        func_ov120_021d4244(wk, func_ov120_021d51d4, 1931);
        return;
    }
    if (func_02035ba4(data_ov120_021d88c8) == 0) {
        if (func_02035cb8(&x, &y) == 1) {
            int newTop = func_ov120_021d7374(wk->disp, y, wk->scroll.count);

            if (wk->scroll.top != newTop) {
                wk->scroll.top = newTop;
                func_ov120_021d82a8(wk->msg, wk->gameData, wk->scroll.top - 2);
                func_ov120_021d72a0(wk->disp, wk->gameData, wk->scroll.top - 2);
                func_ov120_021d6d10(wk->disp, wk->gameData, wk->scroll.top - 2);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        }
        return;
    }
touchbar:
    func_ov067_021b63d0(func_ov120_021d6988(wk->disp));
    if (func_ov067_021b6448(func_ov120_021d6988(wk->disp)) == 1) {
        GFL_BGSysSetBGEnabled(4, FALSE);
        GFL_BGSysSetBGEnabled(6, FALSE);
        func_ov120_021d5318(wk);
        func_ov120_021d69f4(wk->disp, wk->button, GTSNEGO_CURSOR_NONE);
        func_ov120_021d6d48(wk->disp);
        func_ov120_021d7d3c(wk->msg);
        func_ov120_021d4244(wk, func_ov120_021d5208, 1965);
    }
}


void func_ov120_021d5758(GtsNegoWork *wk) {
    if (func_ov120_021d7af4(wk->msg)) {
        wk->timer--;
        if (wk->timer <= 0) {
            func_ov120_021d79f4(wk->msg, 32);
            func_ov067_021b64d0(func_ov120_021d6988(wk->disp), TRUE);
            func_ov120_021d4244(wk, func_ov120_021d5ad4, 1991);
            if (!func_020355c4()) {
                func_ov120_021d69f4(wk->disp, NULL, wk->mainCursor);
            }
        }
    }
}


// pokebw2's GtsNego_StateOpenFriendList
static void func_ov120_021d57b0(GtsNegoWork *wk) {
    BOOL scrollable = TRUE;
    int count = func_02009e8c(func_02012f14(wk->gameData));

    wk->mode = GTSNEGO_MODE_RENDEZVOUS;
    if (count == 0) {
        func_ov120_021d79f4(wk->msg, Bank0045_Text_029);
        wk->timer = 120;
        func_ov120_021d4244(wk, func_ov120_021d5758, 2023);
        return;
    }
    wk->scroll.count = count;
    func_ov120_021d79f4(wk->msg, Bank0045_Text_019);
    if (!func_020355c4()) {
        func_ov120_021d69f4(wk->disp, NULL, wk->friendCursor);
    } else {
        func_ov120_021d69f4(wk->disp, NULL, GTSNEGO_CURSOR_NONE);
    }
    func_ov120_021d7d3c(wk->msg);
    if (count <= GTSNEGO_LIST_SHOWN) {
        scrollable = FALSE;
    }
    func_ov120_021d6bb0(wk->disp, scrollable);
    func_ov120_021d81e0(wk->msg, wk->gameData);
    func_ov120_021d82a8(wk->msg, wk->gameData, wk->scroll.top - 2);
    func_ov120_021d72a0(wk->disp, wk->gameData, wk->scroll.top - 2);
    func_ov120_021d6d10(wk->disp, wk->gameData, wk->scroll.top - 2);
    wk->button = func_ov120_021d7ecc(wk->msg, Bank0045_Text_044);
    func_ov120_021d733c(wk->disp, wk->scroll.top + wk->friendCursor - GTSNEGO_CURSOR_FRIEND_1, wk->scroll.count);
    func_ov120_021d4244(wk, func_ov120_021d5334, 2069);
}

// Shows the main menu, a few frames after the error screen when it was shown
// pokebw2's GtsNego_StateOpenMainMenu
static void func_ov120_021d588c(GtsNegoWork *wk) {
    func_ov120_021d70f0(wk->disp);
    if (wk->errorShown) {
        if (wk->errorFrames++ < 3) {
            return;
        }
        func_0200f800();
        wk->errorShown = FALSE;
    }
    if (!func_020355c4()) {
        func_ov120_021d69f4(wk->disp, NULL, wk->mainCursor);
    } else {
        func_ov120_021d69f4(wk->disp, NULL, GTSNEGO_CURSOR_NONE);
    }
    func_ov120_021d7d6c(wk->msg, func_ov120_021d4c98, wk);
    wk->buttonCallback = func_ov120_021d42d8;
    if (howManyPartyPokesAreNotEggs(GameData_GetParty(wk->gameData)) < 2) {
        wk->notEnoughPokemon = TRUE;
    }
    if (!wk->notEnoughPokemon) {
        func_ov120_021d79f4(wk->msg, Bank0045_Text_032);
    } else {
        func_ov120_021d79f4(wk->msg, Bank0045_Text_043);
    }
    GFL_BGSysSetBGEnabled(4, TRUE);
    GFL_BGSysSetBGEnabled(6, TRUE);
    func_ov120_021d4244(wk, func_ov120_021d5ad4, 2156);
}

int func_ov120_021d593c(GtsNegoWork *wk, int cursor) {
    if (func_ov120_021d6a9c(wk->disp, cursor)) {
        func_ov067_021b64d0(func_ov120_021d6988(wk->disp), FALSE);
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return cursor;
    }
    return GTSNEGO_CURSOR_NONE;
}


// The cursor once its press has played, none until then
// pokebw2's GtsNego_GetPressedCursor
static int func_ov120_021d596c(GtsNegoWork *wk, int cursor) {
    if (!func_ov120_021d6ac0(wk->disp)) {
        cursor = GTSNEGO_CURSOR_NONE;
    }
    return cursor;
}

// pokebw2's GtsNego_StateWaitMainPressed
static void func_ov120_021d5980(GtsNegoWork *wk) {
    int cursor = func_ov120_021d596c(wk, wk->mainCursor);

    if (cursor == GTSNEGO_CURSOR_ANYONE) {
        func_ov120_021d69f4(wk->disp, wk->button, GTSNEGO_CURSOR_NONE);
        func_ov120_021d7d3c(wk->msg);
        GFL_BGSysQueueScrLoad(6);
        func_ov120_021d4244(wk, func_ov120_021d4ec0, 2211);
    } else if (cursor == GTSNEGO_CURSOR_RENDEZVOUS) {
        func_ov120_021d69f4(wk->disp, wk->button, GTSNEGO_CURSOR_NONE);
        func_ov120_021d4244(wk, func_ov120_021d57b0, 2215);
    }
}

// The answer to leaving: fade out, or back to the main menu
// pokebw2's GtsNego_StateWaitExitConfirm
static void func_ov120_021d59e0(GtsNegoWork *wk) {
    if (func_020279e8(wk->menu)) {
        int pos = func_02027a04(wk->menu);

        func_ov120_021d8418(wk->menu);
        wk->menu = NULL;
        switch (pos) {
        case 0:
            wk->param->result = GTSNEGO_RESULT_EXIT;
            wk->timer = 0;
            func_ov120_021d4244(wk, func_ov120_021d44ac, 2229);
            break;
        case 1:
            if (!wk->notEnoughPokemon) {
                func_ov120_021d79f4(wk->msg, Bank0045_Text_032);
            } else {
                func_ov120_021d79f4(wk->msg, Bank0045_Text_043);
            }
            if (!func_020355c4()) {
                func_ov120_021d69f4(wk->disp, NULL, wk->mainCursor);
            }
            func_ov067_021b6558(func_ov120_021d6988(wk->disp), GTSNEGO_TOUCHBAR_RETURN, TRUE);
            func_ov120_021d63f4(wk->disp, FALSE, GTSNEGO_DIM_ALL);
            func_ov120_021d4244(wk, func_ov120_021d5ad4, 2243);
            break;
        }
    }
}

// pokebw2's GtsNego_StateOpenExitConfirm
void func_ov120_021d5a84(GtsNegoWork *wk) {
    if (func_ov120_021d7af4(wk->msg)) {
        func_ov067_021b6558(func_ov120_021d6988(wk->disp), GTSNEGO_TOUCHBAR_RETURN, FALSE);
        wk->menu = func_ov120_021d7b60(wk->msg, 1);
        func_ov120_021d63f4(wk->disp, TRUE, GTSNEGO_DIM_ALL);
        func_ov120_021d69f4(wk->disp, NULL, GTSNEGO_CURSOR_NONE);
        func_ov120_021d4244(wk, func_ov120_021d59e0, 2260);
    }
}

static void func_ov120_021d5ad4(GtsNegoWork *wk) {
    BOOL moved = FALSE;
    TouchBar *bar = func_ov120_021d6988(wk->disp);

    if (!func_ov120_021d7af4(wk->msg)) {
        return;
    }
    if (!func_ov067_021b6498(bar)) {
        if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == 1) {
            func_ov120_021d69f4(wk->disp, NULL, wk->mainCursor);
            func_020355d8(FALSE);
            return;
        }
        if (GCTX_HIDGetPressedKeys() == 1) {
            if (!wk->notEnoughPokemon) {
                func_ov120_021d593c(wk, wk->mainCursor);
                func_ov120_021d4244(wk, func_ov120_021d5980, 2292);
                return;
            }
            GFL_SndSEPlay(0x551);
        }
        if (GCTX_HIDGetPressedKeys() == 0x40) {
            moved = TRUE;
            GFL_SndSEPlay(0x548);
            wk->mainCursor = GTSNEGO_CURSOR_ANYONE;
        }
        if (GCTX_HIDGetPressedKeys() == 0x80) {
            moved = TRUE;
            GFL_SndSEPlay(0x548);
            wk->mainCursor = GTSNEGO_CURSOR_RENDEZVOUS;
        }
        if (moved) {
            func_ov120_021d69f4(wk->disp, NULL, wk->mainCursor);
        }
        func_ov120_021d7c10(wk->msg);
    }
    func_ov067_021b63d0(func_ov120_021d6988(wk->disp));
    if (func_ov067_021b6448(func_ov120_021d6988(wk->disp)) == 1) {
        wk->timer = 1;
        func_ov120_021d79f4(wk->msg, 42);
        if (func_020355c4() == 1) {
            func_ov120_021d69f4(wk->disp, NULL, GTSNEGO_CURSOR_NONE);
        }
        func_ov120_021d4244(wk, func_ov120_021d5a84, 2326);
    }
}


// The partner was lost: back to the main menu, behind the error screen
// pokebw2's GtsNego_HandleDisconnect
void func_ov120_021d5bec(GtsNegoWork *wk) {
    if (wk->param->result != GTSNEGO_RESULT_EXIT) {
        func_0203d7b0(0);
        func_0203d7bc(FALSE);
        func_ov120_021d846c(wk->msg);
        func_ov120_021d63f4(wk->disp, FALSE, GTSNEGO_DIM_ALL);
        GFL_BGSysSetBGEnabled(4, FALSE);
        func_ov120_021d761c(wk->disp);
        func_ov120_021d7b38(wk->msg);
        func_ov120_021d883c(wk->msg);
        func_ov120_021d69f4(wk->disp, NULL, GTSNEGO_CURSOR_NONE);
        wk->errorShown = TRUE;
        wk->errorFrames = 0;
        wk->param->result = GTSNEGO_RESULT_EXIT;
        func_ov120_021d4244(wk, func_ov120_021d588c, 2359);
    }
}

BOOL func_ov120_021d5c60(GameProc *proc, u32 *state, void *param, void *work) {
    GtsNegoParam *negoParam = param;
    GtsNegoWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_GTSNEGO, 0x38000);
    G2_BlendNone();
    G2S_BlendNone();
    func_0202223c(0, WIPE_COLOR_BLACK);
    func_0202223c(1, WIPE_COLOR_BLACK);
    wk = GFL_ProcInitSubsystem(proc, sizeof(GtsNegoWork), HEAPID_GTSNEGO);
    MI_CpuFill8(wk, 0, sizeof(GtsNegoWork));
    wk->heapId = HEAPID_GTSNEGO;
    wk->param = negoParam;
    wk->disp = func_ov120_021d5e88(wk->heapId, negoParam->gameData);
    wk->msg = func_ov120_021d7724(wk->heapId, TEXT_BANK_0045);
    wk->gameData = negoParam->gameData;
    wk->save = GameData_GetSaveControl(negoParam->gameData);
    wk->wifiList = func_0201283c(negoParam->gameData);
    wk->param->result = GTSNEGO_RESULT_EXIT;
    wk->mainCursor = GTSNEGO_CURSOR_ANYONE;
    wk->searchCursor = GTSNEGO_CURSOR_LEVEL;
    wk->friendCursor = GTSNEGO_CURSOR_FRIEND_1;
    wk->myId = getIDAsUInt(GetGameDataPlayerInfo(negoParam->gameData));
    if (func_0203cd98()) {
        func_0203a574(0x2200, data_ov120_021d88ec, 3, wk);
    }
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, wk->heapId);
    GFL_SndBGMPlay(SEQ_BGM_GTS, 0xffff);
    func_0203d34c(TRUE, wk->heapId);
    func_0200f7e8(TRUE);
    func_ov120_021d4244(wk, func_ov120_021d588c, 2410);
    return TRUE;
}


BOOL func_ov120_021d5d74(GameProc *proc, u32 *state, void *param, void *work) {
    GtsNegoWork *wk = work;
    GtsNegoStateFunc func = wk->state;
    BOOL done = TRUE;

    if (func != NULL) {
        if (!func_0200fb24()) {
            func(wk);
        }
        done = FALSE;
    }
    if (wk->button != NULL) {
        TouchBar *bar = func_ov120_021d6988(wk->disp);

        if (bar == NULL || !func_ov067_021b6498(bar)) {
            func_02028178(wk->button);
        }
    }
    if (wk->menu != NULL) {
        func_02027974(wk->menu);
    }
    func_ov120_021d5f98(wk->disp);
    func_ov120_021d7858(wk->msg);
    if (func_0203cd98()) {
        if (func_0200fb24()) {
            switch (func_ov009_02157170(1, 1)) {
            case 1:
            case 4:
                func_ov120_021d5bec(wk);
                break;
            case 2:
                wk->param->result = GTSNEGO_RESULT_RETRY_LOGIN;
                done = TRUE;
                func_0202223c(0, WIPE_COLOR_BLACK);
                func_0202223c(1, WIPE_COLOR_BLACK);
                break;
            case 3:
                func_0200f7dc();
                break;
            }
        }
    } else if (func_0200fb24()) {
        done = TRUE;
    }
    return done;
}


BOOL func_ov120_021d5e30(GameProc *proc, u32 *state, void *param, void *work) {
    GtsNegoWork *wk = work;
    func_0200f7f4();
    if (!GFL_WipeIsFinished()) {
        return FALSE;
    }
    if (func_0203cd98()) {
        func_0203a5e4(GTSNEGO_NET_CMD_BASE);
    }
    if (wk->button != NULL) {
        func_02028148(wk->button);
    }
    if (wk->menu != NULL) {
        func_02027858(wk->menu);
    }
    func_ov120_021d7898(wk->msg);
    func_ov120_021d5fc4(wk->disp);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_GTSNEGO);
    return TRUE;
}


const GameProcFunctions data_ov120_021d890c = {
    func_ov120_021d5c60,
    func_ov120_021d5d74,
    func_ov120_021d5e30,
};
