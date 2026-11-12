// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov260/wifibattlematch_link_random.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// wifibattlematch_link_random.c, in overlay 260 (a descriptive name: the ROM names only the next file,
// wifibattlematch_net.c): the random matchups of the Wi-Fi battle competitions, which the field starts. The player
// picks the free matchups or the ones by rating, and the link finds an opponent through the Global Link server,
// checks the team and the player's data that the other sends, and leaves overlay 290 to pick the teams and battle.
// Each step is a task of overlay 262 (wifibattlematch_util.c) that goes through its states with a message box and
// menus. After the battle the link comes back to send the result

#include "types.h"
#include "app/wifibattlematch_graphic.h"
#include "app/wifibattlematch_link.h"
#include "app/wifibattlematch_net.h"
#include "app/wifibattlematch_util.h"
#include "app/wifibattlematch_view.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "gfl/bg_sys.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/nhttp_rap_evilcheck.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "pml/poke_party.h"
#include "save/dream_world.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/country_region.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/pms.h"
#include "system/pms_data.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/event_fishing.h"
#include "constants/pokemon.h"
#include "gfl/dwc_rapcommon.h"

// The modes of the random matchups (WifiBattleMatchLinkParam.mode); the 2 is overlay 290's own
enum {
    LINK_MODE_RATING,
    LINK_MODE_FREE,
    LINK_MODE_QUIT = 3,
};

// The menus
enum {
    MENU_YES_NO,
    MENU_MODE,
};

// What the check of a B press during the search says
enum {
    CANCEL_NONE,
    CANCEL_BUSY,
    CANCEL_DECLINED,
    CANCEL_CONFIRMED,
};

// What a sub task has left while it runs
#define SUB_TASK_RUNNING 0xffff

// The work of the random matchups
typedef struct {
    WifiBattleMatchGraphic *graphic;
    WifiBattleMatchRes *res;
    Font *font;
    PrintQueue *printQueue;
    MsgData *msgData;
    WordSet *wordSet;
    WifiBattleMatchCard *card;
    WifiBattleMatchPartnerCard *partnerCard;
    WifiBattleMatchLinkIcon *linkIcon;
    WifiBattleMatchMsgBox *msgBox;
    WifiBattleMatchMenu *menu;
    u32 menuKind;
    WifiBattleMatchNet *net;
    WifiBattleMatchTask *task;
    WifiBattleMatchTask *subTask;
    u32 subTaskResult;
    WifiBattleMatchLinkParam *param;
    WifiBattleMatchNetCheck signature;
    u32 timer;
    u32 cancelSeq;
    u32 idleTimer;
    u32 unkEC;
    BOOL recordChanged;
    u32 exchangeResult;
    BOOL hadExchangeResult;
    u32 exchangeDone;
} WifiBattleMatchLinkRandom;

BOOL func_ov175_021d4318(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov175_021d4448(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov175_021d44d4(GameProc *proc, u32 *state, void *param, void *work);
void func_ov175_021d458c(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d4608(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d4730(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d495c(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov175_021d4b3c(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d51bc(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d54e8(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d5560(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d5634(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d56d4(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d57bc(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov175_021d5908(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d5e24(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d6000(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d6048(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d60e0(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d61a4(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d61cc(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d61f0(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d62a4(WifiBattleMatchLinkRandom *link, u32 kind);
void func_ov175_021d639c(WifiBattleMatchLinkRandom *link);
BOOL func_ov175_021d63b0(WifiBattleMatchLinkRandom *link);
BOOL func_ov175_021d63bc(WifiBattleMatchLinkRandom *link);
void func_ov175_021d63c8(WifiBattleMatchLinkRandom *link, u32 kind);
void func_ov175_021d63dc(WifiBattleMatchLinkRandom *link);
void func_ov175_021d63f8(WifiBattleMatchLinkRandom *link, WifiBattleMatchLinkPlayer *partner);
void func_ov175_021d644c(WifiBattleMatchLinkRandom *link);
BOOL func_ov175_021d6460(WifiBattleMatchLinkRandom *link);
BOOL func_ov175_021d646c(void *results, u32 battleType, WifiBattleMatchLinkRandom *link);
void func_ov175_021d6524(void *results, u32 battleType, u32 matchResult);
void func_ov175_021d6550(WifiBattleMatchLinkRandom *link, u32 kind);
void func_ov175_021d65c8(WifiBattleMatchLinkRandom *link);
s32 func_ov175_021d65dc(WifiBattleMatchLinkRandom *link);
void func_ov175_021d65f0(WifiBattleMatchNetKeys *condition, WifiBattleMatchLinkRandom *link);
void func_ov175_021d6698(WifiBattleMatchLinkPlayer *player, WifiBattleMatchLinkRandom *link);
void func_ov175_021d6724(WifiBattleMatchLinkPlayer *player, WifiBattleMatchLinkRandom *link);
void func_ov175_021d6788(WifiBattleMatchLinkRandom *link, WifiBattleMatchTaskFunc func);
u32 func_ov175_021d679c(WifiBattleMatchLinkRandom *link);
BOOL func_ov175_021d67bc(WifiBattleMatchLinkPlayer *player, HeapID heapId);
u32 func_ov175_021d6854(WifiBattleMatchLinkRandom *link, BOOL canCancel, BOOL canDisconnect);
void func_ov175_021d6954(WifiBattleMatchLinkRandom *link);

// pokebw2's WIFIBATTLEMATCH_LINK_RANDOM_PROC_FUNCTIONS
const GameProcFunctions data_ov175_021e2c74 = {
    func_ov175_021d4318,
    func_ov175_021d44d4,
    func_ov175_021d4448,
};

// pokebw2's WifiBattleMatchLink_TaskStart


BOOL func_ov175_021d4318(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchLinkParam *linkParam = param;
    WifiBattleMatchLinkRandom *link;

    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_OvlLoad(OVERLAY_ID(114));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFIBATTLEMATCH_LINK, 0x35000);
    link = GFL_ProcInitSubsystem(proc, sizeof(WifiBattleMatchLinkRandom), HEAPID_WIFIBATTLEMATCH_LINK);
    MI_CpuFill8(link, 0, sizeof(WifiBattleMatchLinkRandom));
    link->param = linkParam;
    link->graphic = func_ov177_021e5440(1, HEAPID_WIFIBATTLEMATCH_LINK);
    link->res = func_ov177_021e567c(func_ov177_021e5548(link->graphic), 2, HEAPID_WIFIBATTLEMATCH_LINK);
    func_ov175_021d6698(linkParam->player, link);
    link->font = GFL_FontCreate(23, 0, 0, 0, HEAPID_WIFIBATTLEMATCH_LINK);
    link->printQueue = func_0201c440(HEAPID_WIFIBATTLEMATCH_LINK);
    link->msgData =
        GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_EVENT_FISHING, HEAPID_WIFIBATTLEMATCH_LINK);
    link->wordSet = GFL_WordSetSystemCreate(8, 0x40, HEAPID_WIFIBATTLEMATCH_LINK);
    link->net = func_ov175_021d9dc8(link->param->unk3C, linkParam->param->gameData, linkParam->unk1C,
                                    HEAPID_WIFIBATTLEMATCH_LINK);
    link->msgBox = func_ov177_021e8568(0, 15, 14, 10, link->printQueue, link->font, HEAPID_WIFIBATTLEMATCH_LINK);
    link->task = func_ov177_021e8a3c(link, func_ov175_021d458c, HEAPID_WIFIBATTLEMATCH_LINK);
    link->subTask = func_ov177_021e8a3c(link, NULL, HEAPID_WIFIBATTLEMATCH_LINK);
    link->linkIcon = func_ov177_021e7d20(func_ov177_021e5548(link->graphic), link->res, HEAPID_WIFIBATTLEMATCH_LINK);
    func_ov177_021e7e0c(link->linkIcon, FALSE);
    if (linkParam->result != 0) {
        if (linkParam->mode == 0) {
            func_ov175_021d63c8(link, 0);
        } else if (linkParam->mode == 1) {
            if (linkParam->result == 1) {
                func_ov175_021d6524(linkParam->record, linkParam->param->unk4, linkParam->battleResult->result);
            }
            func_ov175_021d63c8(link, 1);
        }
    }
    func_ov175_021dd008(link->net, TRUE);
    return TRUE;
}

BOOL func_ov175_021d4448(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchLinkRandom *link = work;

    func_ov175_021dd008(link->net, FALSE);
    func_ov177_021e7df0(link->linkIcon);
    func_ov177_021e8a78(link->subTask);
    func_ov177_021e8a78(link->task);
    func_ov177_021e8628(link->msgBox);
    if (link->net != NULL) {
        func_ov175_021d9ea0(link->net);
    }
    func_ov175_021d644c(link);
    func_ov175_021d639c(link);
    func_ov175_021d65c8(link);
    GFL_WordSetSystemFree(link->wordSet);
    GFL_MsgDataFree(link->msgData);
    func_0201c4c0(link->printQueue);
    GFL_FontFree(link->font);
    func_ov177_021e5868(link->res);
    func_ov177_021e54e0(link->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_WIFIBATTLEMATCH_LINK);
    GFL_OvlUnload(OVERLAY_ID(114));
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

BOOL func_ov175_021d44d4(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchLinkRandom *link = work;

    switch (*state) {
    case 0:
        GFL_FadeSet(3, 16, 0, 0);
        *state = 1;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov177_021e8a80(link->task);
        if (func_ov177_021e8a90(link->task)) {
            *state = 3;
        }
        if (link->net != NULL) {
            func_ov175_021d9ed0(link->net);
        }
        break;
    case 3:
        GFL_FadeSet(3, 0, 16, 0);
        *state = 4;
        break;
    case 4:
        if (!GFL_FadeIsRunning()) {
            return TRUE;
        }
        break;
    }
    func_ov177_021e5534(link->graphic);
    func_0201c4e4(link->printQueue);
    func_ov177_021e867c(link->msgBox);
    func_ov177_021e7e08(link->linkIcon);
    func_ov177_021e5980(link->res);
    if (link->card != NULL) {
        func_ov177_021e64c0(link->card, link->printQueue);
    }
    if (link->partnerCard != NULL) {
        func_ov177_021e7774(link->partnerCard, link->printQueue);
    }
    return FALSE;
}


void func_ov175_021d458c(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkParam *param = link->param;

    switch (param->result) {
    case 0:
        func_ov177_021e8aa0(task, func_ov175_021d4608);
        return;
    case 2:
        func_ov175_021da36c(link->net);
        // fall through
    case 1:
        if (param->mode == 0) {
            func_ov177_021e8aa0(task, func_ov175_021d51bc);
            return;
        } else if (param->mode == 1) {
            func_ov177_021e8aa0(task, func_ov175_021d5e24);
            return;
        }
        break;
    case 3:
        if (param->mode == 0) {
            func_ov177_021e8aa0(task, func_ov175_021d54e8);
            return;
        } else if (param->mode == 1) {
            func_ov177_021e8aa0(task, func_ov175_021d6000);
        }
        break;
    }
}

void func_ov175_021d4608(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    s32 selected;

    switch (*seq) {
    case 0:
        func_ov177_021e7e0c(link->linkIcon, FALSE);
        *seq = 1;
        break;
    case 1:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_025,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 7;
        func_ov177_021e8ab4(task, 2);
        break;
    case 2:
        func_ov175_021d6550(link, MENU_MODE);
        *seq = 3;
        break;
    case 3:
        selected = func_ov175_021d65dc(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d65c8(link);
        switch (selected) {
        case 0:
            param->mode = LINK_MODE_FREE;
            func_ov177_021e8aa0(task, func_ov175_021d57bc);
            break;
        case 1:
            param->mode = LINK_MODE_RATING;
            func_ov177_021e8aa0(task, func_ov175_021d4730);
            break;
        case 2:
            param->mode = LINK_MODE_QUIT;
            *seq = 4;
            break;
        }
        break;
    case 4:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_030,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 7;
        func_ov177_021e8ab4(task, 5);
        break;
    case 5:
        func_ov175_021d6550(link, MENU_YES_NO);
        *seq = 6;
        break;
    case 6:
        selected = func_ov175_021d65dc(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d65c8(link);
        switch (selected) {
        case 0:
            param->action = WIFIBATTLEMATCH_LINK_ACTION_LOGOUT;
            func_ov177_021e8aa0(task, func_ov175_021d61cc);
            break;
        case 1:
            *seq = 1;
            break;
        }
        break;
    case 7:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

void func_ov175_021d4730(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkParam *param = link->param;

    switch (*seq) {
    case 0:
        if (func_020092ec(getDreamWorldStuffAddress(GameData_GetSaveControl(param->param->gameData)))) {
            *seq = 1;
        } else {
            *seq = 6;
        }
        break;
    case 1:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_041,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 14;
        func_ov177_021e8ab4(task, 2);
        break;
    case 2:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_024,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 14;
        func_ov177_021e8ab4(task, 3);
        break;
    case 3:
        MI_CpuFill8(param->account, 0, sizeof(WifiBattleMatchLinkAccount));
        func_ov175_021dc738(link->net, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 4;
        break;
    case 4:
        switch (func_ov175_021dc77c(link->net)) {
        case 0:
            break;
        case 1:
            func_ov175_021dc8a0(link->net, link->param->account);
            *seq = 5;
            break;
        case 3:
            MI_CpuFill8(link->param->account, 0, sizeof(WifiBattleMatchLinkAccount));
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        case 2:
            switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
            case 2:
                func_ov177_021e8aa0(task, func_ov175_021d61a4);
                break;
            case 1:
                func_ov175_021d63dc(link);
                func_ov177_021e8aa0(task, func_ov175_021d4608);
                break;
            }
            break;
        }
        break;
    case 5:
        if (param->account->registered != 0) {
            *seq = 9;
        } else {
            *seq = 6;
        }
        break;
    case 6:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_055,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 14;
        func_ov177_021e8ab4(task, 12);
        break;
    case 7:
    case 8:
        break;
    case 9:
        func_ov175_021d6788(link, func_ov175_021d61f0);
        *seq = 10;
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov175_021d63dc(link);
            func_ov177_021e8aa0(task, func_ov175_021d4608);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
        break;
    case 10: {
        u32 signResult = func_ov175_021d679c(link);

        if (signResult == 0) {
            *seq = 13;
        } else if (signResult == 1) {
            *seq = 11;
        } else if (signResult == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
        }
        if (signResult == SUB_TASK_RUNNING) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov175_021d63dc(link);
            func_ov177_021e8aa0(task, func_ov175_021d4608);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
        break;
    }
    case 11:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_120,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 14;
        func_ov177_021e8ab4(task, 12);
        break;
    case 12:
        func_ov177_021e8aa0(task, func_ov175_021d61cc);
        break;
    case 13:
        func_ov177_021e8aa0(task, func_ov175_021d495c);
        break;
    case 14:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}


void func_ov175_021d495c(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    BOOL ready;

    switch (*seq) {
    case 0:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_024,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        func_ov177_021e8ab4(task, 1);
        *seq = 11;
        break;
    case 1:
        if (*link->param->unk20 != 0) {
            break;
        }
        func_ov175_021db3a4(link->net, -1, 0, param->ratings);
        *seq = 2;
        break;
    case 2:
        ready = func_ov175_021db478(link->net);
        if (ready == 1) {
            *link->param->unk20 = 1800;
            link->recordChanged = func_ov175_021d646c(param->record, param->param->unk4, link);
            func_ov175_021d6724(param->player, link);
            *link->param->unk28 = link->param->ratings->unk54;
            *seq = 3;
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov175_021d63dc(link);
            func_ov177_021e8aa0(task, func_ov175_021d4608);
            *link->param->unk20 = 1800;
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            *link->param->unk20 = 1800;
            break;
        }
        break;
    case 3:
        if (func_ov175_021dc194(link->net) == 0) {
            func_ov175_021dbfa4(link->net);
            *seq = 4;
        } else {
            *seq = 5;
        }
        break;
    case 4: {
        u32 done = func_ov175_021dbfb4(link->net);

        if (done == 1) {
            *seq = 1;
        }
        if (done == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov175_021d63dc(link);
            func_ov177_021e8aa0(task, func_ov175_021d4608);
            *link->param->unk20 = 1800;
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            *link->param->unk20 = 1800;
            break;
        }
        break;
    }
    case 5:
        if (link->card == NULL) {
            *seq = 8;
        } else if (link->recordChanged) {
            *seq = 6;
        } else {
            *seq = 10;
        }
        link->recordChanged = FALSE;
        break;
    case 6:
        *seq = 7;
        break;
    case 7:
        if (func_ov175_021d63bc(link)) {
            func_ov175_021d639c(link);
            *seq = 8;
        }
        break;
    case 8:
        func_ov175_021d62a4(link, 0);
        *seq = 9;
        break;
    case 9:
        if (func_ov175_021d63b0(link)) {
            *seq = 10;
        }
        break;
    case 10:
        func_ov177_021e8aa0(task, func_ov175_021d4b3c);
        break;
    case 11:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

static void func_ov175_021d4b3c(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    WifiBattleMatchLinkPlayer *otherPlayer;
    void *received;
    WifiBattleMatchNetKeys condition;
    BOOL countIdle = FALSE;
    BOOL canCancel = FALSE;
    BOOL canDisconnect;
    BOOL timedOut;
    BOOL noTerminator;
    BOOL matchFound;
    u32 cancel;
    u32 ret;
    u32 fixCount;
    s32 selected;
    int i;
    u32 size;

    if ((s32)*seq >= 2 && (s32)*seq < 8) {
        canCancel = TRUE;
    }
    canDisconnect = (s32)*seq < 6 || (s32)*seq > 7;
    cancel = func_ov175_021d6854(link, canCancel, canDisconnect);
    if (cancel == CANCEL_CONFIRMED) {
        *seq = 22;
        return;
    }
    switch (*seq) {
    case 0:
        link->cancelSeq = 0;
        func_ov177_021e7e0c(link->linkIcon, TRUE);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_033,
                            WIFIBATTLEMATCH_MSG_QUEUE);
        *seq = 1;
        break;
    case 1:
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d4608);
            return;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            return;
        default:
            func_ov175_021d65f0(&condition, link);
            func_ov175_021da0d4(link->net, 2, 1, param->param->unk4, &condition);
            *seq = 2;
            break;
        }
        break;
    case 2:
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d4608);
            func_ov175_021d6954(link);
            return;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            return;
        default:
            ret = func_ov175_021da1e4(link->net);
            if (ret == 1) {
                link->idleTimer = 0;
                *seq = 3;
            } else if (ret == 2) {
                *seq = 1;
            }
            break;
        }
        break;
    case 3:
        if (cancel != CANCEL_NONE) {
            break;
        }
        if (func_ov175_021dc1a0(link->net, param->player)) {
            link->idleTimer = 0;
            *seq = 4;
        }
        countIdle = TRUE;
        break;
    case 4:
        if (func_ov175_021dc1e4(link->net, &received)) {
            otherPlayer = param->otherPlayer;
            size = PokeParty_GetSaveDataSize() + sizeof(WifiBattleMatchLinkPlayer);
            MI_CpuCopy8(received, otherPlayer, size);
            fixCount = 0;
            noTerminator = TRUE;
            for (i = 0; i < 8; i++) {
                if (otherPlayer->info.name[i] == GFL_StrBufGetTerminator()) {
                    noTerminator = FALSE;
                }
            }
            if (noTerminator) {
                MsgData *msgData;
                StrBuf *name;

                fixCount++;
                msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_EVENT_FISHING,
                                             HEAPID_TAIL(HEAPID_WIFIBATTLEMATCH_LINK));
                name = GFL_MsgDataLoadStrbufNew(msgData, HEAPID_TAIL(HEAPID_WIFIBATTLEMATCH_LINK));
                GFL_StrBufStoreString(name, otherPlayer->info.name, 8);
                GFL_StrBufFree(name);
                GFL_MsgDataFree(msgData);
            }
            if (otherPlayer->info.gender != GENDER_MALE && otherPlayer->info.gender != GENDER_FEMALE) {
                otherPlayer->info.gender = GENDER_MALE;
                fixCount++;
            }
            if (otherPlayer->info.unk1C >= 16) {
                otherPlayer->info.unk1C = otherPlayer->info.gender == GENDER_MALE ? 0 : 8;
                fixCount++;
            }
            if (!func_02025bec(otherPlayer->info.country, otherPlayer->info.province, otherPlayer->info.region)) {
                otherPlayer->info.country = 0;
                otherPlayer->info.province = 0;
                fixCount++;
            }
            if (!func_020245d4(&otherPlayer->sentence, HEAPID_TAIL(HEAPID_WIFIBATTLEMATCH_LINK))) {
                func_020242a4(&otherPlayer->sentence, 1);
                fixCount++;
            }
            if (fixCount == 0) {
                getPalPadFriendListAddress(GameData_GetSaveControl(param->param->gameData));
            }
            link->idleTimer = 0;
            *seq = 5;
        }
        countIdle = TRUE;
        break;
    case 5:
        if (func_ov175_021d67bc(param->otherPlayer, HEAPID_WIFIBATTLEMATCH_LINK)) {
            *seq = 6;
        } else {
            func_ov175_021da36c(link->net);
            func_ov177_021e8aa0(task, func_ov175_021d4b3c);
        }
        break;
    case 6:
        func_ov175_021dca28(link->net, &param->otherPlayer->info, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 7;
        break;
    case 7:
        if (func_ov175_021dcaec(link->net, &matchFound)) {
            *link->param->unk40 = matchFound;
            *seq = 8;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d5560);
            func_ov175_021d6954(link);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
        break;
    case 8:
        if (cancel != CANCEL_NONE) {
            break;
        }
        func_ov175_021d9ed4(link->net, 21);
        link->idleTimer = 0;
        *seq = 9;
        break;
    case 9:
        if (!func_ov175_021da45c(link->net) && func_ov175_021d9eec(link->net)) {
            *seq = 10;
        }
        countIdle = TRUE;
        break;
    case 10:
        func_ov175_021d9ed4(link->net, 22);
        link->idleTimer = 0;
        *seq = 11;
        break;
    case 11:
        if (func_ov175_021d9eec(link->net)) {
            *seq = 12;
        }
        countIdle = TRUE;
        break;
    case 12:
        func_ov177_021e7e0c(link->linkIcon, FALSE);
        GFL_SndSEPlay(SEQ_SE_SYS_74);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_034,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 30;
        func_ov177_021e8ab4(task, 13);
        break;
    case 13:
        if (link->timer++ > 120) {
            link->timer = 0;
            func_ov175_021d63f8(link, param->otherPlayer);
            GFL_BGSysSetBGEnabled(0, FALSE);
            *seq = 14;
        }
        break;
    case 14:
        if (func_ov175_021d6460(link)) {
            *seq = 15;
        }
        break;
    case 15:
        if (link->timer++ != 0) {
            link->timer = 0;
            *seq = 16;
        }
        break;
    case 16:
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFIBATTLEMATCH_LINK_DWC, 0x60000);
        func_ov009_02156e30(HEAPID_DEVICE_ALLOC, 0x60000, 1000, HEAPID_WIFIBATTLEMATCH_LINK_DWC);
        func_ov009_02156f50(HEAPID_WIFIBATTLEMATCH_LINK_DWC);
        func_ov175_021da5b0(link->net, 0, 2, 0, 0, 0);
        link->exchangeDone = 0;
        link->exchangeResult = 0;
        *seq = 17;
        break;
    case 17:
        if (link->exchangeDone == 0) {
            link->exchangeDone = func_ov175_021da6b8(link->net, &link->exchangeResult);
        }
        if (link->exchangeDone == 1) {
            *seq = 18;
        }
        if (link->exchangeDone != 0) {
            switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
            case 1:
                func_ov175_021da36c(link->net);
                // fall through
            case 2:
            case 4:
                func_ov009_02156ed0();
                func_ov177_021e8aa0(task, func_ov175_021d61a4);
                break;
            }
        }
        break;
    case 18:
        func_ov009_02156ed0();
        *seq = 19;
        break;
    case 19:
        GFL_BGSysSetBGEnabled(0, TRUE);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_036,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 30;
        func_ov177_021e8ab4(task, 20);
        break;
    case 20:
        if (link->timer++ > 60) {
            link->timer = 0;
            param->action = WIFIBATTLEMATCH_LINK_ACTION_TEAM_LIST;
            func_ov177_021e8aa8(task);
        }
        break;
    case 21:
        func_ov175_021da36c(link->net);
        param->result = 2;
        if (link->exchangeResult != 0) {
            link->hadExchangeResult = TRUE;
            func_ov177_021e8aa0(task, func_ov175_021d51bc);
        } else {
            func_ov177_021e8aa0(task, func_ov175_021d54e8);
        }
        break;
    case 22:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_039,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 30;
        func_ov177_021e8ab4(task, 23);
        break;
    case 23:
        func_ov175_021d6550(link, MENU_YES_NO);
        *seq = 24;
        break;
    case 24:
        selected = func_ov175_021d65dc(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d65c8(link);
        switch (selected) {
        case 0:
            func_ov177_021e8aa0(task, func_ov175_021d56d4);
            break;
        case 1:
            *seq = 25;
            break;
        }
        break;
    case 25:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_038,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 30;
        func_ov177_021e8ab4(task, 26);
        break;
    case 26:
        func_ov175_021d6550(link, MENU_YES_NO);
        *seq = 27;
        break;
    case 27:
        selected = func_ov175_021d65dc(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d65c8(link);
        switch (selected) {
        case 0:
            *seq = 0;
            break;
        case 1:
            *seq = 22;
            break;
        }
        break;
    case 28:
        if (func_ov175_021da434(link->net)) {
            *seq = 29;
        }
        break;
    case 29:
        func_ov175_021da36c(link->net);
        *seq = 0;
        break;
    case 30:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
    if (countIdle) {
        timedOut = FALSE;
        if (link->idleTimer++ > 2400) {
            link->idleTimer = 0;
            timedOut = TRUE;
        }
        if (func_ov175_021da45c(link->net)) {
            timedOut = TRUE;
        }
        if (timedOut) {
            *seq = 28;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d5560);
            func_ov175_021d6954(link);
            return;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
    }
}

void func_ov175_021d51bc(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    WifiBattleMatchLinkReportBuf reportBuf;
    RivalDataSave *rivalData;
    u32 ready;

    switch (*seq) {
    case 0:
        if (param->result == 2 && link->hadExchangeResult == 0) {
            func_ov177_021e8aa0(task, func_ov175_021d54e8);
            return;
        }
        link->hadExchangeResult = 0;
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFIBATTLEMATCH_LINK_DWC, 0x60000);
        func_ov009_02156e30(HEAPID_DEVICE_ALLOC, 0x60000, 1000, HEAPID_WIFIBATTLEMATCH_LINK_DWC);
        func_ov009_02156f50(HEAPID_WIFIBATTLEMATCH_LINK_DWC);
        *seq = 1;
        break;
    case 1:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_037,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 14;
        func_ov177_021e8ab4(task, 2);
        break;
    case 2:
        func_ov175_021da5b0(link->net, 1, 2, param->param->unk4, &param->battleResult->result,
                            param->result == 2);
        link->exchangeDone = 0;
        link->exchangeResult = 0;
        *seq = 3;
        break;
    case 3:
        if (link->exchangeDone == 0) {
            link->exchangeDone = func_ov175_021da6b8(link->net, &link->exchangeResult);
            if (link->exchangeDone != 0 && link->exchangeResult != 0) {
                func_ov177_021e8ac0(param->param->gameData, param->battleResult->result);
            }
        }
        if (link->exchangeDone == 1) {
            param->battleResult->unk14 = func_ov175_021dadac(link->net);
            *seq = 4;
        }
        if (link->exchangeDone == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
            func_ov175_021da36c(link->net);
            // fall through
        case 2:
        case 4:
            func_ov009_02156ed0();
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
        break;
    case 4:
        func_ov009_02156ed0();
        if (param->result == 2) {
            func_ov177_021e8aa0(task, func_ov175_021d54e8);
            return;
        }
        *seq = 5;
        break;
    case 5:
        rivalData = getHollow_RivalData(GameData_GetSaveControl(param->param->gameData));
        func_0200ed54(rivalData, getIDAsUInt(&param->otherPlayer->info));
        *seq = 6;
        func_ov175_021da384(link->net);
        break;
    case 6:
        if (func_ov175_021da390(link->net)) {
            if (*link->param->unk20 == 0) {
                *seq = 8;
            } else {
                func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_042,
                                    WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
                *seq = 14;
                func_ov177_021e8ab4(task, 7);
            }
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            *seq = 7;
            return;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            return;
        }
        break;
    case 7:
        if (*link->param->unk20 == 0) {
            *seq = 8;
        }
        break;
    case 8:
        MI_CpuFill8(&reportBuf, 0, sizeof(reportBuf));
        reportBuf.report = *link->param->report;
        reportBuf.unk50 = *link->param->unk28;
        func_ov175_021dbc04(link->net, 3, &reportBuf);
        *seq = 9;
        break;
    case 9:
        ready = func_ov175_021dbdac(link->net);
        if (ready == 1) {
            *link->param->unk20 = 1800;
            *seq = 10;
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            *link->param->unk20 = 1800;
            return;
        case 1:
            *link->param->unk20 = 1800;
            *seq = 10;
            return;
        }
        break;
    case 10:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_088,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 14;
        func_ov177_021e8ab4(task, 11);
        break;
    case 11:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_040,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 14;
        func_ov177_021e8ab4(task, 12);
        break;
    case 12:
        func_02012dac(param->param->gameData);
        *seq = 13;
        break;
    case 13:
        switch (func_02012dd0(param->param->gameData)) {
        case 0:
        case 1:
            break;
        case 2:
        case 3:
            param->action = WIFIBATTLEMATCH_LINK_ACTION_RESULT;
            func_ov177_021e8aa8(task);
            break;
        }
        break;
    case 14:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}




void func_ov175_021d54e8(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;

    switch (*seq) {
    case 0:
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 0:
            *seq = 1;
            break;
        case 1:
        case 4:
            func_ov177_021e8aa0(task, func_ov175_021d5560);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
        break;
    case 1:
        func_ov177_021e8aa0(task, func_ov175_021d5560);
        break;
    case 2:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

void func_ov175_021d5560(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    s32 selected;

    switch (*seq) {
    case 0:
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 0:
        case 1:
        case 4:
            *seq = 1;
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
        break;
    case 1:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_038,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 4;
        func_ov177_021e8ab4(task, 2);
        break;
    case 2:
        func_ov175_021d6550(link, MENU_YES_NO);
        *seq = 3;
        break;
    case 3:
        selected = func_ov175_021d65dc(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d65c8(link);
        switch (selected) {
        case 0:
            func_ov177_021e8aa0(task, func_ov175_021d495c);
            break;
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d5634);
            break;
        }
        break;
    case 4:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}


void func_ov175_021d5634(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    s32 selected;

    switch (*seq) {
    case 0:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_039,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 3;
        func_ov177_021e8ab4(task, 1);
        break;
    case 1:
        func_ov175_021d6550(link, MENU_YES_NO);
        *seq = 2;
        break;
    case 2:
        selected = func_ov175_021d65dc(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d65c8(link);
        switch (selected) {
        case 0:
            func_ov175_021da36c(link->net);
            func_ov177_021e8aa0(task, func_ov175_021d56d4);
            break;
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d5560);
            break;
        }
        break;
    case 3:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

void func_ov175_021d56d4(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkParam *param = link->param;

    switch (*seq) {
    case 0:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_040,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        func_ov177_021e8ab4(task, 1);
        *seq = 7;
        break;
    case 1:
        func_02012dac(param->param->gameData);
        *seq = 2;
        break;
    case 2:
        if (func_02012dd0(param->param->gameData) == 2) {
            *seq = 3;
        }
        break;
    case 3:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_037,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 7;
        func_ov177_021e8ab4(task, 4);
        break;
    case 4:
        if (*link->param->unk20 == 0) {
            func_ov175_021dbc04(link->net, 2, NULL);
            *seq = 5;
        }
        break;
    case 5:
        if (func_ov175_021dbdac(link->net)) {
            *link->param->unk20 = 1800;
            *seq = 6;
            func_ov175_021d9f04(link->net, TRUE, FALSE);
        }
        break;
    case 6:
        func_ov175_021da36c(link->net);
        param->action = WIFIBATTLEMATCH_LINK_ACTION_LOGOUT;
        func_ov177_021e8aa0(task, func_ov175_021d61cc);
        break;
    case 7:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}




// pokebw2's WifiBattleMatchLink_TaskFreeStart
void func_ov175_021d57bc(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    u32 signResult;

    switch (*seq) {
    case 0:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_026,
                                    WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov175_021d62a4(link, 1);
        *seq = 1;
        break;
    case 1:
        if (func_ov175_021d63b0(link)) {
            *seq = 2;
        }
        break;
    case 2:
        if (func_ov177_021e8844(link->msgBox)) {
            *seq = 3;
        }
        break;
    case 3:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_024,
                                    WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 4;
        break;
    case 4:
        func_ov175_021d6788(link, func_ov175_021d61f0);
        *seq = 5;
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov175_021d63dc(link);
            func_ov177_021e8aa0(task, func_ov175_021d4608);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
        break;
    case 5:
        signResult = func_ov175_021d679c(link);
        if (signResult == 0) {
            func_ov177_021e8aa0(task, func_ov175_021d5908);
        } else if (signResult == 1) {
            *seq = 6;
        } else if (signResult == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
        }
        if (signResult == SUB_TASK_RUNNING) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov175_021d63dc(link);
            func_ov177_021e8aa0(task, func_ov175_021d4608);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
        break;
    case 6:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_120,
                                    WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 7;
        break;
    case 7:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8aa0(task, func_ov175_021d61cc);
        }
        break;
    }
}

static void func_ov175_021d5908(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    WifiBattleMatchLinkPlayer *otherPlayer;
    void *received;
    WifiBattleMatchNetKeys condition;
    BOOL countIdle = FALSE;
    BOOL canCancel = FALSE;
    BOOL canDisconnect;
    BOOL timedOut;
    BOOL noTerminator;
    BOOL matchFound;
    u32 cancel;
    u32 ret;
    int i;
    u32 size;

    if ((s32)*seq >= 3 && (s32)*seq < 13) {
        canCancel = TRUE;
    }
    canDisconnect = (s32)*seq < 11 || (s32)*seq > 12;
    cancel = func_ov175_021d6854(link, canCancel, canDisconnect);
    if (cancel == CANCEL_CONFIRMED) {
        func_ov177_021e8aa0(task, func_ov175_021d60e0);
        return;
    }
    switch (*seq) {
    case 0:
        func_ov175_021d6724(param->player, link);
        *seq = 1;
        break;
    case 1:
        link->cancelSeq = 0;
        func_ov177_021e7e0c(link->linkIcon, TRUE);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_033,
                            WIFIBATTLEMATCH_MSG_QUEUE);
        *seq = 2;
        break;
    case 2:
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d4608);
            return;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            return;
        default:
            func_ov175_021d65f0(&condition, link);
            func_ov175_021da0d4(link->net, 3, 0, param->param->unk4, &condition);
            *seq = 3;
            break;
        }
        break;
    case 3:
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d4608);
            func_ov175_021d6954(link);
            return;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            return;
        default:
            ret = func_ov175_021da1e4(link->net);
            if (ret == 1) {
                link->idleTimer = 0;
                *seq = 6;
            } else if (ret == 2) {
                *seq = 2;
            }
            break;
        }
        break;
    case 6:
        func_ov175_021d9ed4(link->net, 20);
        *seq = 7;
        link->idleTimer = 0;
        break;
    case 7:
        if (func_ov175_021d9eec(link->net)) {
            link->idleTimer = 0;
            *seq = 8;
        }
        countIdle = TRUE;
        break;
    case 8:
        if (cancel != CANCEL_NONE) {
            break;
        }
        if (func_ov175_021dc1a0(link->net, param->player)) {
            link->idleTimer = 0;
            *seq = 9;
        }
        countIdle = TRUE;
        break;
    case 9:
        if (func_ov175_021dc1e4(link->net, &received)) {
            otherPlayer = param->otherPlayer;
            size = PokeParty_GetSaveDataSize() + sizeof(WifiBattleMatchLinkPlayer);
            MI_CpuCopy8(received, otherPlayer, size);
            noTerminator = TRUE;
            for (i = 0; i < 8; i++) {
                if (otherPlayer->info.name[i] == GFL_StrBufGetTerminator()) {
                    noTerminator = FALSE;
                }
            }
            if (noTerminator) {
                MsgData *msgData;
                StrBuf *name;

                msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_EVENT_FISHING,
                                             HEAPID_TAIL(HEAPID_WIFIBATTLEMATCH_LINK));
                name = GFL_MsgDataLoadStrbufNew(msgData, HEAPID_TAIL(HEAPID_WIFIBATTLEMATCH_LINK));
                GFL_StrBufStoreString(name, otherPlayer->info.name, 8);
                GFL_StrBufFree(name);
                GFL_MsgDataFree(msgData);
            }
            if (otherPlayer->info.gender != GENDER_MALE && otherPlayer->info.gender != GENDER_FEMALE) {
                otherPlayer->info.gender = GENDER_MALE;
            }
            if (otherPlayer->info.unk1C >= 16) {
                otherPlayer->info.unk1C = otherPlayer->info.gender == GENDER_MALE ? 0 : 8;
            }
            if (!func_02025bec(otherPlayer->info.country, otherPlayer->info.province, otherPlayer->info.region)) {
                otherPlayer->info.country = 0;
                otherPlayer->info.province = 0;
            }
            if (!func_020245d4(&otherPlayer->sentence, HEAPID_TAIL(HEAPID_WIFIBATTLEMATCH_LINK))) {
                func_020242a4(&otherPlayer->sentence, 1);
            }
            link->idleTimer = 0;
            *seq = 10;
        }
        countIdle = TRUE;
        break;
    case 10:
        if (func_ov175_021d67bc(param->otherPlayer, HEAPID_WIFIBATTLEMATCH_LINK)) {
            *seq = 11;
        } else {
            func_ov175_021da36c(link->net);
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d5908);
        }
        break;
    case 11:
        func_ov175_021dca28(link->net, &param->otherPlayer->info, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 12;
        break;
    case 12:
        if (func_ov175_021dcaec(link->net, &matchFound)) {
            *link->param->unk40 = matchFound;
            *seq = 13;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d6048);
            func_ov175_021d6954(link);
            break;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
        break;
    case 13:
        if (cancel != CANCEL_NONE) {
            break;
        }
        func_ov175_021d9ed4(link->net, 21);
        link->idleTimer = 0;
        *seq = 14;
        break;
    case 14:
        if (!func_ov175_021da45c(link->net) && func_ov175_021d9eec(link->net)) {
            *seq = 15;
        }
        countIdle = TRUE;
        break;
    case 15:
        func_ov175_021d9ed4(link->net, 22);
        link->idleTimer = 0;
        *seq = 16;
        break;
    case 16:
        countIdle = TRUE;
        if (func_ov175_021d9eec(link->net)) {
            *seq = 17;
        }
        break;
    case 17:
        func_ov177_021e7e0c(link->linkIcon, FALSE);
        GFL_SndSEPlay(SEQ_SE_SYS_74);
        func_ov175_021d65c8(link);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_034,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 25;
        func_ov177_021e8ab4(task, 18);
        break;
    case 18:
        if (link->timer++ > 120) {
            link->timer = 0;
            func_ov175_021d63f8(link, param->otherPlayer);
            GFL_BGSysSetBGEnabled(0, FALSE);
            *seq = 19;
        }
        break;
    case 19:
        if (func_ov175_021d6460(link)) {
            *seq = 20;
        }
        break;
    case 20:
        if (link->timer++ > 300) {
            link->timer = 0;
            *seq = 21;
        }
        break;
    case 21:
        GFL_BGSysSetBGEnabled(0, TRUE);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_036,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 25;
        func_ov177_021e8ab4(task, 22);
        break;
    case 22:
        if (link->timer++ > 60) {
            link->timer = 0;
            param->action = WIFIBATTLEMATCH_LINK_ACTION_TEAM_LIST;
            func_ov177_021e8aa8(task);
        }
        break;
    case 23:
        if (func_ov175_021da434(link->net)) {
            *seq = 24;
        }
        break;
    case 24:
        func_ov175_021da36c(link->net);
        *seq = 1;
        break;
    case 25:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
    if (countIdle) {
        timedOut = FALSE;
        if (link->idleTimer++ > 2400) {
            link->idleTimer = 0;
            timedOut = TRUE;
        }
        if (func_ov175_021da45c(link->net)) {
            timedOut = TRUE;
        }
        if (timedOut) {
            *seq = 23;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d6048);
            func_ov175_021d6954(link);
            return;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            break;
        }
    }
}


void func_ov175_021d5e24(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    RivalDataSave *rivalData;

    switch (*seq) {
    case 0:
        if (param->result == 2) {
            func_ov177_021e8aa0(task, func_ov175_021d6000);
            return;
        }
        *seq = 3;
        break;
    case 3:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_043,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 11;
        func_ov177_021e8ab4(task, 1);
        link->timer = 0;
        break;
    case 1:
        if (func_ov175_021dc2c0(link->net, &param->battleResult->unk10)) {
            *seq = 2;
        }
        break;
    case 2:
        if (func_ov175_021dc2d8(link->net, &param->battleResult->unk14)) {
            *seq = 4;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            link->timer = 0;
            *seq = 7;
            return;
        case 2:
            link->timer = 0;
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            return;
        }
        break;
    case 4:
        rivalData = getHollow_RivalData(GameData_GetSaveControl(param->param->gameData));
        func_0200ed54(rivalData, getIDAsUInt(&param->otherPlayer->info));
        func_ov177_021e8ac0(param->param->gameData, param->battleResult->result);
        func_ov175_021da384(link->net);
        *seq = 5;
        break;
    case 5:
        link->timer++;
        if (func_ov175_021da390(link->net)) {
            *seq = 6;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            link->timer = 0;
            *seq = 7;
            return;
        case 2:
            link->timer = 0;
            func_ov177_021e8aa0(task, func_ov175_021d61a4);
            return;
        }
        break;
    case 6:
        if (link->timer++ > 30) {
            link->timer = 0;
            *seq = 7;
        }
        break;
    case 7:
        *seq = 8;
        break;
    case 8:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_040,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 11;
        func_ov177_021e8ab4(task, 9);
        break;
    case 9:
        func_02012dac(param->param->gameData);
        *seq = 10;
        break;
    case 10:
        switch (func_02012dd0(param->param->gameData)) {
        case 0:
        case 1:
            break;
        case 2:
        case 3:
            param->action = WIFIBATTLEMATCH_LINK_ACTION_RESULT;
            func_ov177_021e8aa8(task);
            break;
        }
        break;
    case 11:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

void func_ov175_021d6000(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;

    switch (*seq) {
    case 0:
        *seq = 1;
        break;
    case 1:
        *seq = 2;
        break;
    case 2:
        func_ov177_021e8aa0(task, func_ov175_021d6048);
        break;
    case 3:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

void func_ov175_021d6048(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    s32 selected;

    switch (*seq) {
    case 0:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_038, WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 3;
        func_ov177_021e8ab4(task, 1);
        break;
    case 1:
        func_ov175_021d6550(link, MENU_YES_NO);
        *seq = 2;
        break;
    case 2:
        selected = func_ov175_021d65dc(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d65c8(link);
        switch (selected) {
        case 0:
            func_ov177_021e8aa0(task, func_ov175_021d5908);
            break;
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d60e0);
            break;
        }
        break;
    case 3:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}




void func_ov175_021d60e0(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    s32 selected;

    switch (*seq) {
    case 0:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_039,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 5;
        func_ov177_021e8ab4(task, 1);
        break;
    case 1:
        func_ov175_021d6550(link, MENU_YES_NO);
        *seq = 2;
        break;
    case 2:
        selected = func_ov175_021d65dc(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d65c8(link);
        switch (selected) {
        case 0:
            param->action = WIFIBATTLEMATCH_LINK_ACTION_LOGOUT;
            *seq = 3;
            break;
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d6048);
            break;
        }
        break;
    case 3:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_043,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 5;
        func_ov177_021e8ab4(task, 4);
        break;
    case 4:
        func_ov177_021e8aa0(task, func_ov175_021d61cc);
        break;
    case 5:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

// pokebw2's WifiBattleMatchLink_TaskRelogin
void func_ov175_021d61a4(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;

    link->param->action = WIFIBATTLEMATCH_LINK_ACTION_RELOGIN;
    if (link->net != NULL) {
        func_ov175_021d9ea0(link->net);
        link->net = NULL;
    }
    func_0200fbb0();
    func_ov177_021e8aa8(task);
}

// pokebw2's WifiBattleMatchLink_TaskEnd
void func_ov175_021d61cc(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;

    if (*seq == 0) {
        if (link->net != NULL) {
            func_ov175_021d9ea0(link->net);
            link->net = NULL;
        }
        func_ov177_021e8aa8(task);
    }
}

// pokebw2's WifiBattleMatchLink_TaskSignTeam
void func_ov175_021d61f0(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkRandom *link = work;
    WifiBattleMatchLinkPlayer *player = link->param->player;
    u32 ret;

    switch (*seq) {
    case 0:
        link->timer = 0;
        MI_CpuFill8(&link->signature, 0, sizeof(link->signature));
        (*seq)++;
        break;
    case 1:
        func_ov175_021dcb58(link->net, player->partyData, TRUE);
        (*seq)++;
        break;
    case 2:
        ret = func_ov175_021dcc28(link->net, &link->signature);
        if (ret == 1) {
            *seq = 3;
        } else if (ret == 2) {
            link->subTaskResult = 2;
            *seq = 4;
        } else if (ret == 3) {
            link->subTaskResult = 3;
            *seq = 4;
        }
        break;
    case 3:
        if (link->signature.status == 0) {
            MI_CpuCopy8(link->signature.signature, player->signature, sizeof(player->signature));
            link->subTaskResult = 0;
            *seq = 4;
        } else {
            link->subTaskResult = 1;
            *seq = 4;
        }
        break;
    case 4:
        func_ov177_021e8aa8(task);
        break;
    }
}

void func_ov175_021d62a4(WifiBattleMatchLinkRandom *link, u32 kind) {
    GameData *gameData;
    PlayerInfo *playerInfo;
    ClActUnit *unit;
    WifiBattleMatchRankInfo info;
    u32 titleMsg;

    if (link->card != NULL) {
        return;
    }
    gameData = link->param->param->gameData;
    playerInfo = GetGameDataPlayerInfo(gameData);
    unit = func_ov177_021e5548(link->graphic);
    MI_CpuFill8(&info, 0, sizeof(info));
    if (kind == 0) {
        titleMsg = 1;
        info.titleMsg = link->param->param->unk4;
        info.unk4 = func_0200dfd8(link->param->record, link->param->param->unk4 + 5, 0);
        info.unk10 = func_0200dfd8(link->param->record, link->param->param->unk4 + 5, 1);
        info.unk12 = func_0200dfd8(link->param->record, link->param->param->unk4 + 5, 2);
        info.unkC = info.unk10 + info.unk12;
        if (info.unk4 == 0 && info.unkC == 0) {
            info.unk4 = 1500;
        }
    } else if (kind == 1) {
        titleMsg = 0;
        info.titleMsg = link->param->param->unk4;
        info.unk4 = 0;
        info.unk10 = func_0200dfd8(link->param->record, link->param->param->unk4, 1);
        info.unk12 = func_0200dfd8(link->param->record, link->param->param->unk4, 2);
        info.unkC = info.unk10 + info.unk12;
    }
    info.look = func_02008554(playerInfo);
    link->card = func_ov177_021e5c20(&info, titleMsg, playerInfo, unit, link->res, link->font, link->printQueue,
                                     link->msgData, link->wordSet, HEAPID_WIFIBATTLEMATCH_LINK);
}


// pokebw2's WifiBattleMatchLink_DeleteRankCard
void func_ov175_021d639c(WifiBattleMatchLinkRandom *link) {
    if (link->card != NULL) {
        func_ov177_021e5d40(link->card);
        link->card = NULL;
    }
}

// pokebw2's WifiBattleMatchLink_IsRankCardShown
BOOL func_ov175_021d63b0(WifiBattleMatchLinkRandom *link) {
    return func_ov177_021e64c8(link->card);
}

// pokebw2's WifiBattleMatchLink_IsRankCardHidden
BOOL func_ov175_021d63bc(WifiBattleMatchLinkRandom *link) {
    return func_ov177_021e65fc(link->card);
}

// pokebw2's WifiBattleMatchLink_ShowRankCard
void func_ov175_021d63c8(WifiBattleMatchLinkRandom *link, u32 kind) {
    func_ov175_021d62a4(link, kind);
    while (!func_ov175_021d63b0(link)) {
    }
}

// pokebw2's WifiBattleMatchLink_HideRankCard
void func_ov175_021d63dc(WifiBattleMatchLinkRandom *link) {
    if (link->card != NULL) {
        while (!func_ov175_021d63bc(link)) {
        }
        func_ov175_021d639c(link);
    }
}

void func_ov175_021d63f8(WifiBattleMatchLinkRandom *link, WifiBattleMatchLinkPlayer *partner) {
    ClActUnit *unit;
    u32 kind;
    u32 shown;

    if (link->partnerCard == NULL) {
        unit = func_ov177_021e5548(link->graphic);
        if (link->param->mode == 0) {
            shown = 1;
            kind = 2;
        } else if (link->param->mode == 1) {
            shown = 0;
            kind = 3;
        }
        link->partnerCard = func_ov177_021e7520((const LiveBattleMatchPlayerData *)partner, unit, link->res,
                                                link->font, link->printQueue, link->msgData, link->wordSet, kind,
                                                shown, 0, HEAPID_WIFIBATTLEMATCH_LINK);
    }
}


// pokebw2's WifiBattleMatchLink_DeletePartnerCard
void func_ov175_021d644c(WifiBattleMatchLinkRandom *link) {
    if (link->partnerCard != NULL) {
        func_ov177_021e7710(link->partnerCard);
        link->partnerCard = NULL;
    }
}

// pokebw2's WifiBattleMatchLink_IsPartnerCardDone
BOOL func_ov175_021d6460(WifiBattleMatchLinkRandom *link) {
    return func_ov177_021e777c(link->partnerCard);
}

// pokebw2's WifiBattleMatchLink_UpdateRecord
BOOL func_ov175_021d646c(void *results, u32 battleType, WifiBattleMatchLinkRandom *link) {
    u32 rating;
    u32 first;
    u32 second;
    BOOL changed;

    switch (battleType) {
    case 0:
        rating = link->param->ratings->rating[3];
        first = link->param->ratings->results[3].first;
        second = link->param->ratings->results[3].second;
        break;
    case 1:
        rating = link->param->ratings->rating[0];
        first = link->param->ratings->results[0].first;
        second = link->param->ratings->results[0].second;
        break;
    case 2:
        rating = link->param->ratings->rating[4];
        first = link->param->ratings->results[4].first;
        second = link->param->ratings->results[4].second;
        break;
    case 3:
        rating = link->param->ratings->rating[1];
        first = link->param->ratings->results[1].first;
        second = link->param->ratings->results[1].second;
        break;
    case 4:
        rating = link->param->ratings->rating[2];
        first = link->param->ratings->results[2].first;
        second = link->param->ratings->results[2].second;
        break;
    }
    if (rating != func_0200dfd8(results, battleType + 5, 0) || first != func_0200dfd8(results, battleType + 5, 1) ||
        second != func_0200dfd8(results, battleType + 5, 2)) {
        changed = TRUE;
    } else {
        changed = FALSE;
    }
    func_0200dfe4(results, battleType + 5, 0, rating);
    func_0200dfe4(results, battleType + 5, 1, first);
    func_0200dfe4(results, battleType + 5, 2, second);
    return changed;
}

// pokebw2's WifiBattleMatchLink_AddMatchResult
void func_ov175_021d6524(void *results, u32 battleType, u32 matchResult) {
    switch (matchResult) {
    case 1:
    case 4:
        func_0200e008(results, battleType, 1);
        break;
    case 0:
    case 3:
        func_0200e008(results, battleType, 2);
        break;
    }
}

// pokebw2's WifiBattleMatchLink_CreateMenu
void func_ov175_021d6550(WifiBattleMatchLinkRandom *link, u32 kind) {
    WifiBattleMatchMenuParam menuParam;

    if (link->menu == NULL) {
        MI_CpuFill8(&menuParam, 0, sizeof(menuParam));
        menuParam.msgData = link->msgData;
        menuParam.font = link->font;
        menuParam.printQueue = link->printQueue;
        menuParam.bg = 0;
        menuParam.palette = 15;
        menuParam.framePalette = 13;
        menuParam.frameChar = 1;
        switch (kind) {
        case MENU_YES_NO:
            menuParam.msgIds[0] = EventFishing_Text_031;
            menuParam.msgIds[1] = EventFishing_Text_032;
            menuParam.count = 2;
            menuParam.canCancel = TRUE;
            menuParam.cancelValue = 1;
            break;
        case MENU_MODE:
            menuParam.msgIds[0] = EventFishing_Text_027;
            menuParam.msgIds[1] = EventFishing_Text_028;
            menuParam.msgIds[2] = EventFishing_Text_029;
            menuParam.count = 3;
            menuParam.canCancel = TRUE;
            menuParam.cancelValue = 2;
            break;
        }
        link->menuKind = kind;
        link->menu = func_ov177_021e888c(&menuParam, HEAPID_WIFIBATTLEMATCH_LINK);
    }
}

// pokebw2's WifiBattleMatchLink_DeleteMenu
void func_ov175_021d65c8(WifiBattleMatchLinkRandom *link) {
    if (link->menu != NULL) {
        func_ov177_021e89d0(link->menu);
        link->menu = NULL;
    }
}

// pokebw2's WifiBattleMatchLink_GetMenuSelection
s32 func_ov175_021d65dc(WifiBattleMatchLinkRandom *link) {
    if (link->menu != NULL) {
        return func_ov177_021e8a00(link->menu);
    }
    return -1;
}

// pokebw2's WifiBattleMatchLink_FillCondition
void func_ov175_021d65f0(WifiBattleMatchNetKeys *condition, WifiBattleMatchLinkRandom *link) {
    WifiBattleMatchLinkParam *param;

    MI_CpuFill8(condition, 0, sizeof(WifiBattleMatchNetKeys));
    param = link->param;
    condition->disconnects = param->ratings->unk20;
    condition->cup = 0;
    condition->matches = param->ratings->results[3].first + param->ratings->results[3].second +
                         param->ratings->results[0].first + param->ratings->results[0].second +
                         param->ratings->results[4].first + param->ratings->results[4].second +
                         param->ratings->results[1].first + param->ratings->results[1].second +
                         param->ratings->results[2].first + param->ratings->results[2].second;
    switch (param->param->unk4) {
    case 0:
        condition->rating = param->ratings->rating[3];
        break;
    case 1:
        condition->rating = param->ratings->rating[0];
        break;
    case 2:
        condition->rating = param->ratings->rating[4];
        break;
    case 3:
        condition->rating = param->ratings->rating[1];
        break;
    case 4:
        condition->rating = param->ratings->rating[2];
        break;
    }
}

// pokebw2's WifiBattleMatchLink_FillOwnPlayer
void func_ov175_021d6698(WifiBattleMatchLinkPlayer *player, WifiBattleMatchLinkRandom *link) {
    GameData *gameData = link->param->param->gameData;
    SaveControl *save = GameData_GetSaveControl(gameData);
    PlayerInfo *info;

    player->unk44 = 100;
    info = GetGameDataPlayerInfo(gameData);
    MI_CpuCopy8(info, player, func_02008468());
    func_0200e6fc(func_0200e6e8(save), 0, &player->sentence);
    if (link->param->mode == 0) {
        player->record1 = func_0200dfd8(link->param->record, link->param->param->unk4 + 5, 1);
        player->record2 = func_0200dfd8(link->param->record, link->param->param->unk4 + 5, 2);
    } else {
        player->record1 = func_0200dfd8(link->param->record, link->param->param->unk4, 1);
        player->record2 = func_0200dfd8(link->param->record, link->param->param->unk4, 2);
    }
    player->gsid = func_02009cc0(func_0201283c(gameData));
}

// pokebw2's WifiBattleMatchLink_UpdatePlayerRecord
void func_ov175_021d6724(WifiBattleMatchLinkPlayer *player, WifiBattleMatchLinkRandom *link) {
    if (link->param->mode == 0) {
        player->record1 = func_0200dfd8(link->param->record, link->param->param->unk4 + 5, 1);
        player->record2 = func_0200dfd8(link->param->record, link->param->param->unk4 + 5, 2);
        player->unk34 = func_0200dfd8(link->param->record, link->param->param->unk4 + 5, 0);
    } else {
        player->record1 = func_0200dfd8(link->param->record, link->param->param->unk4, 1);
        player->record2 = func_0200dfd8(link->param->record, link->param->param->unk4, 2);
    }
}

// pokebw2's WifiBattleMatchLink_StartSubTask
void func_ov175_021d6788(WifiBattleMatchLinkRandom *link, WifiBattleMatchTaskFunc func) {
    func_ov177_021e8aa0(link->subTask, func);
    link->subTaskResult = SUB_TASK_RUNNING;
}

// pokebw2's WifiBattleMatchLink_UpdateSubTask
u32 func_ov175_021d679c(WifiBattleMatchLinkRandom *link) {
    func_ov177_021e8a80(link->subTask);
    if (func_ov177_021e8a90(link->subTask)) {
        return link->subTaskResult;
    }
    return SUB_TASK_RUNNING;
}

BOOL func_ov175_021d67bc(WifiBattleMatchLinkPlayer *player, HeapID heapId) {
    int i;
    PokeParty *party = (PokeParty *)player->partyData;
    void *array;
    BOOL result;

    array = func_ov114_021b9a64(PokeParty_GetPkmRawSize(), PokeParty_GetPkmCount(party), HEAPID_TAIL(heapId));
    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        PartyPkm *pkm = PokeParty_GetPkm(party, i);

        func_ov114_021b9a94(array, pkm, PokeParty_GetPkmRawSize(), i);
    }
    result = func_ov114_021b9a1c(array, PokeParty_GetPkmRawSize(), PokeParty_GetPkmCount(party), player->signature,
                                 HEAPID_TAIL(heapId));
    func_ov114_021b9a8c(array);
    return result;
}

u32 func_ov175_021d6854(WifiBattleMatchLinkRandom *link, BOOL canCancel, BOOL canDisconnect) {
    s32 selected;

    switch (link->cancelSeq) {
    case 0:
        if (canCancel && (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
            link->cancelSeq++;
            break;
        }
        return CANCEL_NONE;
    case 1:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_035,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        link->cancelSeq++;
        break;
    case 2:
        if (func_ov177_021e8844(link->msgBox)) {
            link->cancelSeq++;
        }
        break;
    case 3:
        func_ov175_021d6550(link, MENU_YES_NO);
        link->cancelSeq++;
        break;
    case 4:
        selected = func_ov175_021d65dc(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d65c8(link);
        switch (selected) {
        case 0:
            link->cancelSeq = 5;
            break;
        case 1:
            func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_033,
                                WIFIBATTLEMATCH_MSG_QUEUE);
            link->cancelSeq = 0;
            return CANCEL_DECLINED;
        }
        break;
    case 5:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_043,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        link->cancelSeq++;
        break;
    case 6:
        if (canDisconnect) {
            func_ov175_021da434(link->net);
            link->cancelSeq++;
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov175_021da36c(link->net);
            if (!func_0200fb24() && func_0203cd98()) {
                DWC_CloseAllConnectionsHard();
            }
        }
        break;
    case 7:
        link->cancelSeq = 0;
        return CANCEL_CONFIRMED;
    }
    return CANCEL_BUSY;
}



// pokebw2's WifiBattleMatchLink_ResetCancel
void func_ov175_021d6954(WifiBattleMatchLinkRandom *link) {
    link->cancelSeq = 0;
    func_ov175_021d65c8(link);
}
