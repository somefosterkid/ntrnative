// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov260/wifibattlematch_link_competition.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// wifibattlematch_link_competition.c, in overlay 260 (a descriptive name: the ROM names only the next file,
// wifibattlematch_net.c): the Wi-Fi competition of the title screen's menu. The link checks the account on the
// Global Link server and the competition's period, updates the Digital Player ID, registers the Battle Box, finds an
// opponent, checks the team and the player's data that the other sends, and leaves overlay 290 to pick the teams
// and battle. Each step is a task of overlay 262 (wifibattlematch_util.c) that goes through its states with a
// message box and menus

#include "types.h"
#include "app/livebattlematch_flow.h"
#include "app/wifibattlematch_graphic.h"
#include "app/wifibattlematch_link.h"
#include "app/wifibattlematch_net.h"
#include "app/wifibattlematch_util.h"
#include "app/wifibattlematch_view.h"
#include "battle/regulation.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/bg_sys.h"
#include "gfl/dwc_rapcommon.h"
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
#include "gfl/str_sjis.h"
#include "nitro/rtc.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/dream_world.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/tournament_block.h"
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

#define HEAPID_WIFIBATTLEMATCH 0x56

// The menus
enum {
    COMPETITION_MENU_YES_NO,
    COMPETITION_MENU_JOIN_INFO_CANCEL,
    COMPETITION_MENU_REGISTER,
    COMPETITION_MENU_BATTLE,
    COMPETITION_MENU_BATTLE_AGAIN,
    COMPETITION_MENU_YES_NO_NO_FIRST,
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

// What a request to the server's account takes
typedef struct {
    u8 unk0;
    u8 unk1;
    u32 unk4;
} WifiBattleMatchLinkRequest;

// The work of the Wi-Fi competition
typedef struct {
    WifiBattleMatchGraphic *graphic;
    WifiBattleMatchRes *res;
    Font *font;
    Font *smallFont;
    PrintQueue *printQueue;
    MsgData *msgData;
    WordSet *wordSet;
    WifiBattleMatchCard *card;
    WifiBattleMatchPartnerCard *partnerCard;
    WifiBattleMatchTeamList *teamList;
    WifiBattleMatchLinkIcon *linkIcon;
    WifiBattleMatchMsgBox *msgBox;
    WifiBattleMatchMenu *menu;
    WifiBattleMatchNet *net;
    WifiBattleMatchTask *task;
    WifiBattleMatchTask *subTask;
    u32 subTaskResult;
    WifiBattleMatchNetCheck signature;
    u32 timer;
    u32 unkE4;
    u32 unkE8;
    u32 cancelSeq;
    u32 idleTimer;
    u32 exchangeResult;
    u32 exchangeDone;
    PokeParty *party;
    BOOL hadExchangeResult;
    WifiBattleMatchLinkParam *param;
    void *entry;
    u8 serverEntry[0x160];
    BOOL hadBattleBox;
} WifiBattleMatchLinkCompetition;

// The three bytes of a date in the tournament block, the first in the high byte
static inline u32 WifiBattleMatchLink_PackBytes(u32 high, u32 middle, u32 low) {
    return (high << 24) | ((middle & 0xff) << 16) | ((low & 0xff) << 8);
}

BOOL func_ov175_021d6964(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov175_021d6a98(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov175_021d6b38(GameProc *proc, u32 *state, void *param, void *work);
void func_ov175_021d6bf4(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d6c54(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d6db8(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d76d4(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d7be4(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d7e18(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d8904(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d8bc4(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d8cf0(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d8e30(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d8ec8(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d8fa8(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d905c(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d9064(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d9090(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d9344(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d95b0(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov175_021d965c(WifiBattleMatchLinkCompetition *link);
void func_ov175_021d97a4(WifiBattleMatchLinkCompetition *link);
BOOL func_ov175_021d97b8(WifiBattleMatchLinkCompetition *link);
BOOL func_ov175_021d97c4(WifiBattleMatchLinkCompetition *link);
void func_ov175_021d97d0(WifiBattleMatchLinkCompetition *link);
void func_ov175_021d97e4(WifiBattleMatchLinkCompetition *link, u32 page);
void func_ov175_021d9804(WifiBattleMatchLinkCompetition *link, WifiBattleMatchLinkPlayer *partner);
void func_ov175_021d9854(WifiBattleMatchLinkCompetition *link);
BOOL func_ov175_021d9868(WifiBattleMatchLinkCompetition *link);
void func_ov175_021d9874(WifiBattleMatchLinkCompetition *link);
void func_ov175_021d98b8(WifiBattleMatchLinkCompetition *link);
BOOL func_ov175_021d98cc(WifiBattleMatchLinkCompetition *link);
BOOL func_ov175_021d98d8(WifiBattleMatchLinkCompetition *link);
void func_ov175_021d98e4(WifiBattleMatchLinkCompetition *link, u32 kind);
void func_ov175_021d99f4(WifiBattleMatchLinkCompetition *link);
s32 func_ov175_021d9a08(WifiBattleMatchLinkCompetition *link);
void func_ov175_021d9a1c(WifiBattleMatchLinkCompetition *link,
                                                        WifiBattleMatchTaskFunc func);
u32 func_ov175_021d9a30(WifiBattleMatchLinkCompetition *link);
void func_ov175_021d9a50(WifiBattleMatchLinkPlayer *player,
                                                              WifiBattleMatchLinkCompetition *link);
void func_ov175_021d9a9c(WifiBattleMatchLinkPlayer *player,
                                                      WifiBattleMatchLinkCompetition *link);
void func_ov175_021d9b18(WifiBattleMatchLinkPlayer *player,
                                                         WifiBattleMatchNetCheck *signature);
BOOL func_ov175_021d9b2c(WifiBattleMatchLinkPlayer *player, PokeParty *party, HeapID heapId);
BOOL func_ov175_021d9bc4(PokeParty *party, PokeParty *otherParty);
u32 func_ov175_021d9c34(WifiBattleMatchLinkCompetition *link, BOOL canCancel, BOOL canDisconnect);
void func_ov175_021d9d40(WifiBattleMatchLinkCompetition *link);
BOOL func_ov175_021d9d50(WifiBattleMatchLinkCompetition *link,
                                                        WifiBattleMatchLinkRanking *ranking);

// pokebw2's WIFIBATTLEMATCH_LINK_COMPETITION_PROC_FUNCTIONS
const GameProcFunctions data_ov175_021e2c80 = {
    func_ov175_021d6964,
    func_ov175_021d6b38,
    func_ov175_021d6a98,
};

// pokebw2's WifiBattleMatchLink_CompetitionTaskStart


BOOL func_ov175_021d6964(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchLinkParam *linkParam = param;
    WifiBattleMatchLinkCompetition *link;

    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_OvlLoad(OVERLAY_ID(114));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFIBATTLEMATCH_LINK, 0x30000);
    link = GFL_ProcInitSubsystem(proc, sizeof(WifiBattleMatchLinkCompetition), HEAPID_WIFIBATTLEMATCH_LINK);
    MI_CpuFill8(link, 0, sizeof(WifiBattleMatchLinkCompetition));
    link->param = linkParam;
    link->entry = func_0200b1d4(getTournamentBlockAddress(GameData_GetSaveControl(linkParam->param->gameData)), 0);
    link->graphic = func_ov177_021e5440(1, HEAPID_WIFIBATTLEMATCH_LINK);
    link->res = func_ov177_021e567c(func_ov177_021e5548(link->graphic), 0, HEAPID_WIFIBATTLEMATCH_LINK);
    link->font = GFL_FontCreate(23, 0, 0, 0, HEAPID_WIFIBATTLEMATCH_LINK);
    link->smallFont = GFL_FontCreate(23, 3, 0, 0, HEAPID_WIFIBATTLEMATCH_LINK);
    link->printQueue = func_0201c440(HEAPID_WIFIBATTLEMATCH_LINK);
    link->msgData =
        GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_EVENT_FISHING, HEAPID_WIFIBATTLEMATCH_LINK);
    link->wordSet = GFL_WordSetSystemCreate(8, 0x40, HEAPID_WIFIBATTLEMATCH_LINK);
    link->net = func_ov175_021d9dc8(link->param->unk3C, linkParam->param->gameData, linkParam->unk1C,
                                    HEAPID_WIFIBATTLEMATCH_LINK);
    link->msgBox = func_ov177_021e8568(0, 15, 14, 10, link->printQueue, link->font, HEAPID_WIFIBATTLEMATCH_LINK);
    link->task = func_ov177_021e8a3c(link, func_ov175_021d6bf4, HEAPID_WIFIBATTLEMATCH_LINK);
    link->subTask = func_ov177_021e8a3c(link, NULL, HEAPID_WIFIBATTLEMATCH_LINK);
    link->linkIcon = func_ov177_021e7d20(func_ov177_021e5548(link->graphic), link->res, HEAPID_WIFIBATTLEMATCH_LINK);
    func_ov177_021e7e0c(link->linkIcon, FALSE);
    link->party = PokeParty_Create(HEAPID_WIFIBATTLEMATCH_LINK);
    func_ov175_021dd008(link->net, TRUE);
    return TRUE;
}

BOOL func_ov175_021d6a98(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchLinkCompetition *link = work;

    GFL_HeapFree(link->party);
    func_ov175_021dd008(link->net, FALSE);
    func_ov177_021e7df0(link->linkIcon);
    func_ov177_021e8a78(link->subTask);
    func_ov177_021e8a78(link->task);
    func_ov177_021e8628(link->msgBox);
    if (link->net != NULL) {
        func_ov175_021d9ea0(link->net);
    }
    func_ov175_021d98b8(link);
    func_ov175_021d9854(link);
    func_ov175_021d97a4(link);
    func_ov175_021d99f4(link);
    GFL_WordSetSystemFree(link->wordSet);
    GFL_MsgDataFree(link->msgData);
    func_0201c4c0(link->printQueue);
    GFL_FontFree(link->smallFont);
    GFL_FontFree(link->font);
    func_ov177_021e5868(link->res);
    func_ov177_021e54e0(link->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_WIFIBATTLEMATCH_LINK);
    GFL_OvlUnload(OVERLAY_ID(114));
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

BOOL func_ov175_021d6b38(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchLinkCompetition *link = work;

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
    func_ov175_021d9ed0(link->net);
    if (link->card != NULL) {
        func_ov177_021e64c0(link->card, link->printQueue);
    }
    if (link->partnerCard != NULL) {
        func_ov177_021e7774(link->partnerCard, link->printQueue);
    }
    if (link->teamList != NULL) {
        func_ov177_021e84f8(link->teamList, link->printQueue);
    }
    return FALSE;
}


void func_ov175_021d6bf4(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;

    switch (link->param->result) {
    case 0:
        func_ov177_021e8aa0(task, func_ov175_021d6c54);
        return;
    case 2:
        func_ov175_021da36c(link->net);
        // fall through
    case 1:
        func_ov175_021d97d0(link);
        func_ov177_021e8aa0(task, func_ov175_021d8904);
        return;
    case 3:
        func_ov175_021d97d0(link);
        func_ov177_021e8aa0(task, func_ov175_021d8bc4);
        break;
    }
}

// pokebw2's WifiBattleMatchLink_CompetitionTaskAccount
void func_ov175_021d6c54(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    u32 ready;

    switch (*seq) {
    case 0:
        if (func_020092ec(getDreamWorldStuffAddress(GameData_GetSaveControl(param->param->gameData)))) {
            *seq = 1;
        } else {
            *seq = 5;
        }
        break;
    case 1:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_054,
                                    WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 2;
        break;
    case 2:
        MI_CpuFill8(param->account, 0, sizeof(WifiBattleMatchLinkAccount));
        func_ov175_021dc738(link->net, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 3;
        break;
    case 3:
        ready = func_ov175_021dc77c(link->net);
        if (ready == 1) {
            func_ov175_021dc8a0(link->net, link->param->account);
            *seq = 4;
            break;
        }
        if (ready == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 4:
        if (param->account->registered != 0) {
            *seq = 7;
        } else {
            *seq = 5;
        }
        break;
    case 5:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_055,
                                    WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 6;
        break;
    case 6:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8aa0(task, func_ov175_021d905c);
        }
        break;
    case 7:
        if (*link->param->unk20 == 0) {
            func_ov175_021da044(link->net);
            *seq = 8;
        }
        break;
    case 8:
        ready = func_ov175_021da054(link->net);
        if (ready == 1) {
            *link->param->unk20 = 1800;
            *seq = 9;
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 9:
        func_ov177_021e8aa0(task, func_ov175_021d6db8);
        break;
    }
}

void func_ov175_021d6db8(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    void *entry = link->entry;
    u32 ready;
    s32 selected;
    u32 state;
    u8 accountState;

    switch (*seq) {
    case 0:
        if (param->account->unk4 == 1) {
            *seq = 1;
        } else {
            *seq = 3;
        }
        break;
    case 1: {
        WifiBattleMatchLinkRequest request;

        MI_CpuFill8(&request, 0, sizeof(request));
        request.unk1 = 0;
        request.unk0 = link->param->account->unk4;
        request.unk4 = func_ov175_021dd020(link->net);
        link->param->account->unk5 = 0;
        func_ov175_021dc8b4(link->net, &request, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 2;
        break;
    }
    case 2:
        ready = func_ov175_021dc908(link->net);
        if (ready == 1) {
            *seq = 11;
        } else if (ready == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 3:
        if (link->param->account->unk5 == func_0200b078(entry, 10)) {
            *seq = 4;
        } else if (func_0200b078(entry, 10) == 4 || func_0200b078(entry, 10) == 5) {
            *seq = 7;
        } else {
            *seq = 4;
        }
        break;
    case 4:
        accountState = param->account->unk5;
        if (accountState == 3) {
            *seq = 5;
        } else if (accountState == 4 || accountState == 5) {
            *seq = 6;
        } else {
            *seq = 11;
        }
        break;
    case 5:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_057,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 46);
        *seq = 56;
        break;
    case 6:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_057,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 46);
        *seq = 56;
        break;
    case 7: {
        WifiBattleMatchLinkRequest request;

        MI_CpuFill8(&request, 0, sizeof(request));
        request.unk1 = func_0200b078(entry, 10);
        request.unk4 = func_ov175_021dd020(link->net);
        link->param->account->unk5 = func_0200b078(entry, 10);
        func_ov175_021dc8b4(link->net, &request, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 8;
        break;
    }
    case 8:
        ready = func_ov175_021dc908(link->net);
        if (ready == 1) {
            *seq = 9;
        } else if (ready == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 9:
        *seq = 6;
        break;
    case 10:
        ready = func_ov175_021dbdac(link->net);
        if (ready == 1) {
            *link->param->unk20 = 1800;
            *seq = 6;
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 11:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_091,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        func_ov177_021e8ab4(task, 12);
        *seq = 56;
        break;
    case 12:
        func_ov175_021dc2f4(link->net, param->account->profileId);
        *seq = 13;
        break;
    case 13:
        ready = func_ov175_021dc338(link->net);
        if (ready == 1) {
            func_ov175_021dc66c(link->net, &link->serverEntry);
            if (func_0200b1a8(&link->serverEntry)) {
                *seq = 14;
            } else {
                *seq = 15;
            }
        } else if (ready == 2) {
            *seq = 15;
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 15:
        if (func_0200b078(entry, 2) == 0) {
            *seq = 18;
        } else {
            *seq = 16;
        }
        break;
    case 16:
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        case 0:
            func_ov175_021d9a1c(link, func_ov175_021d9090);
            *seq = 17;
            break;
        }
        break;
    case 17:
        ready = func_ov175_021d9a30(link);
        if (ready != SUB_TASK_RUNNING) {
            *seq = 46;
        }
        if (ready == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 18:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_059,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 55);
        *seq = 56;
        break;
    case 14:
        if (param->account->unk5 == 0) {
            *seq = 19;
        } else if (param->account->unk5 == 1) {
            *seq = 53;
        } else if (param->account->unk5 == 2) {
            *seq = 54;
        } else {
            *seq = 55;
        }
        break;
    case 19:
        if (func_0200b078(entry, 2) == 0) {
            *seq = 42;
        } else {
            *seq = 20;
        }
        break;
    case 20:
        func_ov175_021d965c(link);
        if (func_0200bbfc(getBattleBox(GameData_GetSaveControl(param->param->gameData)), 1)) {
            func_ov175_021d97e4(link, 0);
        } else {
            func_ov175_021d97e4(link, 1);
        }
        *seq = 21;
        break;
    case 21:
        if (func_ov175_021d97b8(link)) {
            *seq = 22;
        }
        break;
    case 22:
        if (func_0200bbfc(getBattleBox(GameData_GetSaveControl(param->param->gameData)), 1)) {
            *seq = 23;
        } else {
            *seq = 29;
        }
        break;
    case 23:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_061,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 24);
        *seq = 56;
        break;
    case 24:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO);
        *seq = 25;
        break;
    case 25:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 35;
            break;
        case 1:
            *seq = 26;
            break;
        }
        break;
    case 26:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_062,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 27);
        *seq = 56;
        break;
    case 27:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO);
        *seq = 28;
        break;
    case 28:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 55;
            break;
        case 1:
            *seq = 23;
            break;
        }
        break;
    case 29:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_060,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 30);
        *seq = 56;
        break;
    case 30:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO);
        *seq = 31;
        break;
    case 31:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 35;
            break;
        case 1:
            *seq = 32;
            break;
        }
        break;
    case 32:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_062,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 33);
        *seq = 56;
        break;
    case 33:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO);
        *seq = 34;
        break;
    case 34:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 55;
            break;
        case 1:
            *seq = 29;
            break;
        }
        break;
    case 35:
        if (func_ov175_021d97c4(link)) {
            *seq = 36;
        }
        break;
    case 36:
        func_ov175_021d97a4(link);
        *seq = 42;
        break;
    case 37:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_070,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        func_ov177_021e8ab4(task, 38);
        *seq = 56;
        break;
    case 38:
        if (*link->param->unk20 == 0) {
            MI_CpuFill8(&param->unk34->body, 0, sizeof(param->unk34->body));
            func_ov175_021dbc04(link->net, 0, &link->param->unk34->body);
            *seq = 39;
        }
        break;
    case 39:
        ready = func_ov175_021dbdac(link->net);
        if (ready == 1) {
            *link->param->unk20 = 1800;
            *seq = 40;
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 40: {
        WifiBattleMatchLinkRequest request;

        MI_CpuFill8(&request, 0, sizeof(request));
        request.unk0 = 0;
        request.unk1 = 1;
        request.unk4 = func_ov175_021dd020(link->net);
        link->param->account->unk5 = 1;
        func_ov175_021dc8b4(link->net, &request, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 41;
        break;
    }
    case 41:
        ready = func_ov175_021dc908(link->net);
        if (ready == 1) {
            *seq = 53;
        } else if (ready == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 42:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_058,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        func_ov177_021e8ab4(task, 43);
        *seq = 56;
        break;
    case 43: {
        SaveControl *save = GameData_GetSaveControl(param->param->gameData);
        BattleBoxSave *battleBox = getBattleBox(save);
        void *previous = func_0200b204(getTournamentBlockAddress(save), 0);

        MI_CpuCopy8(&link->serverEntry, link->entry, sizeof(link->serverEntry));
        func_0200bbd8(battleBox, 1);
        func_0200b110(link->entry, 10, 1);
        func_0200dfe4(param->record, 10, 0, 0);
        func_0200dfe4(param->record, 10, 1, 0);
        func_0200dfe4(param->record, 10, 2, 0);
        func_0200b1f8(previous);
        func_02012dac(param->param->gameData);
        *seq = 44;
        break;
    }
    case 44:
        state = func_02012dd0(param->param->gameData);
        if (state == 2) {
            *seq = 37;
        } else if (state == 3) {
            *seq = 45;
        }
        break;
    case 45:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_064,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 55);
        *seq = 56;
        break;
    case 46:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_090,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        func_ov177_021e8ab4(task, 47);
        *seq = 56;
        break;
    case 47:
        if (*link->param->unk20 == 0) {
            func_ov175_021db3a4(link->net, -1, 1, param->unk34);
            *seq = 48;
        }
        break;
    case 48:
        ready = func_ov175_021db478(link->net);
        if (ready == 1) {
            *link->param->unk20 = 1800;
            *seq = 49;
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 49:
        if (link->param->unk34->unk10 != func_0200dfd8(param->record, 10, 0)) {
            *seq = 50;
        } else {
            *seq = 55;
        }
        break;
    case 50:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_058,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        func_ov177_021e8ab4(task, 51);
        *seq = 56;
        break;
    case 51:
        func_ov175_021d9d50(link, param->unk34);
        func_02012dac(param->param->gameData);
        *seq = 52;
        break;
    case 52:
        if (func_02012dd0(param->param->gameData) == 2) {
            *seq = 55;
        }
        break;
    case 53:
        func_ov177_021e8aa0(task, func_ov175_021d76d4);
        break;
    case 54:
        func_ov177_021e8aa0(task, func_ov175_021d7be4);
        break;
    case 55:
        param->action = WIFIBATTLEMATCH_LINK_ACTION_LOGOUT;
        func_ov177_021e8aa0(task, func_ov175_021d905c);
        break;
    case 56:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

void func_ov175_021d76d4(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    WifiBattleMatchLinkPlayer *player;
    Regulation *regulation;
    PokeParty *party;
    SaveControl *save;
    BattleBoxSave *battleBox;
    void *previous;
    BOOL ready;
    u32 invalid;
    u32 invalidFlags;
    s32 selected;
    u32 ret;
    int count;
    int i;
    int zeroCount;

    switch (*seq) {
    case 0:
        func_ov175_021d965c(link);
        func_ov175_021d97e4(link, 2);
        func_ov175_021d9874(link);
        *seq = 1;
        break;
    case 1:
        ready = TRUE;
        ready &= func_ov175_021d97b8(link);
        if (func_ov175_021d98cc(link) & ready) {
            *seq = 2;
        }
        break;
    case 2:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_065,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 3);
        *seq = 30;
        break;
    case 3:
        func_ov175_021d98e4(link, COMPETITION_MENU_REGISTER);
        *seq = 4;
        break;
    case 4:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 12;
            break;
        case 1:
            *seq = 5;
            break;
        }
        break;
    case 5:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_068,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 6);
        *seq = 30;
        break;
    case 6:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO);
        *seq = 7;
        break;
    case 7:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 8;
            break;
        case 1:
            *seq = 2;
            break;
        }
        break;
    case 8:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_069,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 9);
        *seq = 30;
        break;
    case 9:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO);
        *seq = 10;
        break;
    case 10:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 11;
            break;
        case 1:
            *seq = 2;
            break;
        }
        break;
    case 11:
        if (func_ov175_021d98d8(link)) {
            func_ov175_021d98b8(link);
            *seq = 29;
        }
        break;
    case 12:
        player = param->player;
        func_ov175_021d9a50(player, link);
        if (howManyPokesAreAbleToFight((PokeParty *)player->partyData) == 0) {
            *seq = 14;
            break;
        }
        invalidFlags = 0;
        regulation = func_0200ae14(link->entry);
        invalid = func_0201a090(regulation, (PokeParty *)player->partyData, &invalidFlags);
        if (invalid == 0) {
            *seq = 15;
        } else {
            *seq = 13;
        }
        break;
    case 13:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_073,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 29);
        *seq = 30;
        break;
    case 14:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_097,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 29);
        *seq = 30;
        break;
    case 15:
        if (func_ov175_021d98d8(link)) {
            func_ov175_021d98b8(link);
            *seq = 16;
        }
        break;
    case 16:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_070,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        func_ov177_021e8ab4(task, 17);
        *seq = 30;
        break;
    case 17:
        func_ov175_021dcb58(link->net, param->player->partyData, TRUE);
        MI_CpuFill8(&link->signature, 0, sizeof(link->signature));
        *seq = 18;
        break;
    case 18:
        ret = func_ov175_021dcc28(link->net, &link->signature);
        if (ret == 1) {
            count = PokeParty_GetPkmCount((PokeParty *)param->player->partyData);
            zeroCount = 0;
            for (i = 0; i < count; i++) {
                if (link->signature.result[i] == 0) {
                    zeroCount++;
                }
            }
            if (link->signature.status == 0 && zeroCount == count) {
                *seq = 20;
            } else {
                *seq = 19;
            }
        } else if (ret == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            *seq = 0;
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 19:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_071,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 29);
        *seq = 30;
        break;
    case 20:
        if (*link->param->unk20 == 0) {
            func_ov175_021dbc04(link->net, 0, param->player->partyData);
            *seq = 21;
        }
        break;
    case 21:
        ready = func_ov175_021dbdac(link->net);
        if (ready == 1) {
            *link->param->unk20 = 1800;
            *seq = 22;
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            *seq = 0;
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 22:
        save = GameData_GetSaveControl(param->param->gameData);
        battleBox = getBattleBox(save);
        previous = func_0200b204(getTournamentBlockAddress(save), 0);
        party = convertBoxedPokeSetToParty(battleBox, HEAPID_TAIL(HEAPID_WIFIBATTLEMATCH));
        func_0200bbc8(battleBox, 1);
        func_0200b110(link->entry, 10, 2);
        func_0200b2a4(previous, party);
        GFL_HeapFree(party);
        *seq = 23;
        break;
    case 23:
        func_02012dac(param->param->gameData);
        *seq = 24;
        break;
    case 24:
        ret = func_02012dd0(param->param->gameData);
        if (ret == 2) {
            *seq = 25;
        } else if (ret == 3) {
            *seq = 29;
        }
        break;
    case 25: {
        WifiBattleMatchLinkRequest request;

        MI_CpuFill8(&request, 0, sizeof(request));
        request.unk1 = 2;
        request.unk4 = func_ov175_021dd020(link->net);
        link->param->account->unk5 = 2;
        func_ov175_021dc8b4(link->net, &request, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 26;
        break;
    }
    case 26:
        ret = func_ov175_021dc908(link->net);
        if (ret == 1) {
            *seq = 27;
        } else if (ret == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            *seq = 0;
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 27:
        func_ov175_021d97e4(link, 0);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_072,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 28);
        *seq = 30;
        break;
    case 28:
        func_ov177_021e8aa0(task, func_ov175_021d7be4);
        break;
    case 29:
        param->action = WIFIBATTLEMATCH_LINK_ACTION_LOGOUT;
        func_ov177_021e8aa0(task, func_ov175_021d8ec8);
        break;
    case 30:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

void func_ov175_021d7be4(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    u32 ret;
    s32 selected;

    switch (*seq) {
    case 0:
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 0:
            *seq = 1;
            break;
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 1:
        if (link->card == NULL) {
            func_ov175_021d965c(link);
            func_ov175_021d97e4(link, 0);
            *seq = 2;
        } else {
            *seq = 3;
        }
        break;
    case 2:
        if (func_ov175_021d97b8(link)) {
            *seq = 3;
        }
        break;
    case 3:
        GFL_BGSysSetBGEnabled(0, TRUE);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_074,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        func_ov177_021e8ab4(task, 4);
        *seq = 16;
        break;
    case 4:
        func_ov175_021d98e4(link, COMPETITION_MENU_BATTLE);
        *seq = 5;
        break;
    case 5:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 10;
            break;
        case 1:
            *seq = 7;
            break;
        case 2:
            *seq = 6;
            break;
        }
        break;
    case 6:
        func_ov177_021e8aa0(task, func_ov175_021d8e30);
        break;
    case 7:
        func_ov175_021d9a1c(link, func_ov175_021d9344);
        *seq = 8;
        break;
    case 8:
        ret = func_ov175_021d9a30(link);
        if (ret == 1 || ret == 2) {
            *seq = 1;
        } else if (ret == 0) {
            *seq = 9;
        } else if (ret == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 9:
        func_ov177_021e8aa0(task, func_ov175_021d8ec8);
        break;
    case 10:
        func_ov175_021d9a50(param->player, link);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_024,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        func_ov177_021e8ab4(task, 11);
        *seq = 16;
        break;
    case 11:
        func_ov175_021d9a1c(link, func_ov175_021d95b0);
        *seq = 12;
        break;
    case 12:
        ret = func_ov175_021d9a30(link);
        if (ret == 0) {
            *seq = 13;
        } else if (ret == 1) {
            *seq = 13;
        } else if (ret == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d7be4);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 13:
        func_ov177_021e8aa0(task, func_ov175_021d7e18);
        break;
    case 14:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_120,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 16;
        func_ov177_021e8ab4(task, 15);
        break;
    case 15:
        func_ov177_021e8aa0(task, func_ov175_021d8ec8);
        break;
    case 16:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

void func_ov175_021d7e18(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    WifiBattleMatchNetKeys condition;
    void *received;
    u32 flags;
    BOOL matchFound;
    char ownName[8];
    char otherName[8];
    WifiBattleMatchLinkPlayer *player;
    WifiBattleMatchLinkPlayer *otherPlayer;
    PokeParty *party;
    void *entry;
    Regulation *regulation;
    BOOL countIdle = FALSE;
    BOOL canCancel = FALSE;
    BOOL canDisconnect;
    BOOL timedOut;
    BOOL noTerminator;
    u32 cancel;
    u32 fixCount;
    u32 ret;
    s32 selected;
    int i;

    if ((s32)*seq >= 12 && (s32)*seq < 27) {
        canCancel = TRUE;
    }
    canDisconnect = ((s32)*seq < 25 || (s32)*seq > 26) && ((s32)*seq < 17 || (s32)*seq > 18);
    cancel = func_ov175_021d9c34(link, canCancel, canDisconnect);
    if (cancel == CANCEL_CONFIRMED) {
        *seq = 40;
        return;
    }
    switch (*seq) {
    case 0:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_024,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        func_ov177_021e8ab4(task, 1);
        *seq = 53;
        break;
    case 1:
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d7be4);
            func_ov175_021d9d40(link);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        case 0:
            func_ov175_021d9a1c(link, func_ov175_021d9090);
            *seq = 2;
            break;
        }
        break;
    case 2:
        ret = func_ov175_021d9a30(link);
        if (ret == 0) {
            *seq = 4;
        } else if (ret == 1 || ret == 2) {
            *seq = 3;
        } else if (ret == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d7be4);
            func_ov175_021d9d40(link);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 3:
        func_ov177_021e8aa0(task, func_ov175_021d8ec8);
        break;
    case 4:
        if (*link->param->unk20 != 0) {
            break;
        }
        func_ov175_021db3a4(link->net, -1, 1, link->param->unk34);
        *seq = 5;
        break;
    case 5:
        ret = func_ov175_021db478(link->net);
        if (ret == 1) {
            player = param->player;
            *link->param->unk20 = 0;
            func_ov175_021d9a9c(player, link);
            *link->param->unk28 = link->param->unk34->unk14;
            if (func_ov175_021d9d50(link, link->param->unk34)) {
                *seq = 6;
            } else {
                *seq = 10;
            }
        }
        if (ret == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d7be4);
            func_ov175_021d9d40(link);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 6:
        *seq = 7;
        break;
    case 7:
        if (func_ov175_021d97c4(link)) {
            func_ov175_021d97a4(link);
            *seq = 8;
        }
        break;
    case 8:
        func_ov175_021d965c(link);
        *seq = 9;
        break;
    case 9:
        if (func_ov175_021d97b8(link)) {
            *seq = 10;
        }
        break;
    case 10:
        link->cancelSeq = 0;
        func_ov177_021e7e0c(link->linkIcon, TRUE);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_085,
                            WIFIBATTLEMATCH_MSG_QUEUE);
        *seq = 53;
        func_ov177_021e8ab4(task, 11);
        break;
    case 11:
        entry = link->entry;
        MI_CpuFill8(&condition, 0, sizeof(condition));
        condition.rating = link->param->unk34->unk10;
        condition.disconnects = link->param->unk34->unk4;
        condition.cup = func_0200b078(entry, 2);
        condition.matches = link->param->unk34->unk8 + link->param->unk34->unkC;
        func_ov175_021da0d4(link->net, 0, 0, param->param->unk4, &condition);
        link->unkE4 = 0;
        link->unkE8 = 0;
        *seq = 12;
        break;
    case 12:
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
            func_ov175_021d9d40(link);
            break;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        ret = func_ov175_021da1e4(link->net);
        if (ret == 1) {
            *seq = 13;
        } else if (ret == 2) {
            *seq = 11;
        }
        break;
    case 13:
        if (cancel == CANCEL_NONE) {
            if (func_ov175_021dc1a0(link->net, param->player)) {
                *seq = 14;
                link->idleTimer = 0;
            }
        } else {
            switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
            case 1:
            case 4:
                link->cancelSeq = 0;
                func_ov175_021d99f4(link);
                func_ov177_021e7e0c(link->linkIcon, FALSE);
                func_ov177_021e8aa0(task, func_ov175_021d8cf0);
                func_ov175_021d9d40(link);
                break;
            case 2:
                func_ov175_021d99f4(link);
                func_ov177_021e7e0c(link->linkIcon, FALSE);
                func_ov177_021e8aa0(task, func_ov175_021d9064);
                break;
            }
        }
        break;
    case 14:
        if (func_ov175_021dc1e4(link->net, &received)) {
            MI_CpuCopy8(received, param->otherPlayer, PokeParty_GetSaveDataSize() + sizeof(WifiBattleMatchLinkPlayer));
            otherPlayer = param->otherPlayer;
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
            *seq = 15;
        }
        countIdle = TRUE;
        break;
    case 15:
        player = param->player;
        func_02052274(player->info.name, ownName, 8);
        party = (PokeParty *)player->partyData;
        for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
            PokeParty_GetPkm(party, i);
        }
        otherPlayer = param->otherPlayer;
        func_02052274(otherPlayer->info.name, otherName, 8);
        party = (PokeParty *)otherPlayer->partyData;
        for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
            PokeParty_GetPkm(party, i);
        }
        if (param->player->entryField2 == param->otherPlayer->entryField2 &&
            param->player->checksum == param->otherPlayer->checksum) {
            *seq = 16;
        } else {
            *seq = 51;
        }
        break;
    case 16:
        flags = 0;
        regulation = func_0200ae14(link->entry);
        if (func_0201a090(regulation, (PokeParty *)param->otherPlayer->partyData, &flags) == 0) {
            *seq = 17;
        } else {
            link->unkE4++;
            *seq = 22;
        }
        break;
    case 17:
        if (*link->param->unk20 == 0) {
            func_ov175_021db3a4(link->net, link->param->otherPlayer->unk4C, 3, link->party);
            *seq = 18;
        }
        break;
    case 18:
        ret = func_ov175_021db478(link->net);
        if (ret == 1) {
            *link->param->unk20 = 3600;
            *seq = 19;
        }
        if (ret == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
            func_ov175_021d9d40(link);
            break;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 19:
        func_ov175_021d9ed4(link->net, 23);
        *seq = 20;
        break;
    case 20:
        if (func_ov175_021d9eec(link->net)) {
            *seq = 21;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
            func_ov175_021d9d40(link);
            break;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 21:
        if (!func_ov175_021d9bc4(link->party, (PokeParty *)link->param->otherPlayer->partyData)) {
            link->unkE4++;
        }
        if (!func_ov175_021d9b2c(link->param->otherPlayer, (PokeParty *)link->param->otherPlayer->partyData,
                                 HEAPID_WIFIBATTLEMATCH_LINK)) {
            link->unkE4++;
        }
        *seq = 22;
        break;
    case 22:
        if (cancel == CANCEL_NONE) {
            if (func_ov175_021dc268(link->net, &link->unkE4)) {
                *seq = 23;
                link->idleTimer = 0;
            }
        } else {
            switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
            case 1:
            case 4:
                link->cancelSeq = 0;
                func_ov175_021d99f4(link);
                func_ov177_021e7e0c(link->linkIcon, FALSE);
                func_ov177_021e8aa0(task, func_ov175_021d8cf0);
                func_ov175_021d9d40(link);
                break;
            case 2:
                func_ov175_021d99f4(link);
                func_ov177_021e7e0c(link->linkIcon, FALSE);
                func_ov177_021e8aa0(task, func_ov175_021d9064);
                break;
            }
        }
        break;
    case 23:
        if (func_ov175_021dc2a0(link->net, &link->unkE8)) {
            *seq = 24;
        } else {
            countIdle = TRUE;
        }
        break;
    case 24:
        if (link->unkE8 == 0 && link->unkE4 == 0) {
            *seq = 25;
        } else {
            *seq = 51;
        }
        break;
    case 25:
        func_ov175_021dca28(link->net, &param->otherPlayer->info, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 26;
        break;
    case 26:
        if (func_ov175_021dcaec(link->net, &matchFound)) {
            *link->param->unk40 = matchFound;
            *seq = 27;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
            func_ov175_021d9d40(link);
            break;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 27:
        if (cancel == CANCEL_NONE) {
            func_ov175_021d9ed4(link->net, 21);
            link->idleTimer = 0;
            *seq = 28;
        }
        break;
    case 28:
        if (!func_ov175_021da45c(link->net) && func_ov175_021d9eec(link->net)) {
            *seq = 29;
        }
        countIdle = TRUE;
        break;
    case 29:
        func_ov175_021d9ed4(link->net, 22);
        *seq = 30;
        link->idleTimer = 0;
        break;
    case 30:
        if (func_ov175_021d9eec(link->net)) {
            *seq = 31;
        }
        countIdle = TRUE;
        break;
    case 31:
        func_ov177_021e7e0c(link->linkIcon, FALSE);
        GFL_SndSEPlay(SEQ_SE_SYS_74);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_087,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 53;
        func_ov177_021e8ab4(task, 32);
        link->timer = 0;
        break;
    case 32:
        if (link->timer++ > 120) {
            link->timer = 0;
            GFL_BGSysSetBGEnabled(0, FALSE);
            func_ov175_021d9804(link, param->otherPlayer);
            *seq = 33;
        }
        break;
    case 33:
        if (func_ov175_021d9868(link)) {
            *seq = 34;
        }
        break;
    case 34:
        if (link->timer++ != 0) {
            link->timer = 0;
            *seq = 35;
        }
        break;
    case 35:
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFIBATTLEMATCH_LINK_DWC, 0x60000);
        func_ov009_02156e30(HEAPID_DEVICE_ALLOC, 0x60000, 1000, HEAPID_WIFIBATTLEMATCH_LINK_DWC);
        func_ov009_02156f50(HEAPID_WIFIBATTLEMATCH_LINK_DWC);
        func_ov175_021da5b0(link->net, 0, 0, 0, 0, 0);
        link->exchangeResult = 0;
        *seq = 36;
        link->exchangeDone = 0;
        break;
    case 36:
        if (link->exchangeDone == 0) {
            link->exchangeDone = func_ov175_021da6b8(link->net, &link->exchangeResult);
        }
        if (link->exchangeDone == 1) {
            *seq = 37;
        }
        if (link->exchangeDone != 0) {
            switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
            case 1:
                func_ov175_021da36c(link->net);
                // fall through
            case 2:
            case 4:
                func_ov009_02156ed0();
                func_ov177_021e7e0c(link->linkIcon, FALSE);
                func_ov177_021e8aa0(task, func_ov175_021d9064);
                break;
            }
        }
        break;
    case 37:
        func_ov009_02156ed0();
        GFL_BGSysSetBGEnabled(0, TRUE);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_036,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 53;
        func_ov177_021e8ab4(task, 38);
        break;
    case 38:
        if (link->timer++ > 60) {
            link->timer = 0;
            param->action = WIFIBATTLEMATCH_LINK_ACTION_TEAM_LIST;
            func_ov177_021e8aa8(task);
        }
        break;
    case 39:
        func_ov175_021da36c(link->net);
        func_ov177_021e7e0c(link->linkIcon, FALSE);
        param->result = 2;
        link->hadExchangeResult = TRUE;
        if (link->exchangeResult != 0) {
            func_ov177_021e8aa0(task, func_ov175_021d8904);
        } else {
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
        }
        break;
    case 40:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_078,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 53;
        func_ov177_021e8ab4(task, 41);
        break;
    case 41:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO);
        *seq = 42;
        break;
    case 42:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            func_ov177_021e8aa0(task, func_ov175_021d8fa8);
            break;
        case 1:
            *seq = 43;
            break;
        }
        break;
    case 43:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_038,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 53;
        func_ov177_021e8ab4(task, 44);
        break;
    case 44:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO);
        *seq = 45;
        break;
    case 45:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 46;
            break;
        case 1:
            *seq = 40;
            break;
        }
        break;
    case 46:
        *seq = 47;
        break;
    case 47:
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d7be4);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        case 0:
            func_ov175_021d9a1c(link, func_ov175_021d9090);
            *seq = 48;
            break;
        }
        break;
    case 48:
        ret = func_ov175_021d9a30(link);
        if (ret == 0) {
            *seq = 10;
        } else if (ret == 1 || ret == 2) {
            *seq = 3;
        } else if (ret == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d7be4);
            func_ov175_021d9d40(link);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 49:
        if (func_ov175_021da434(link->net)) {
            *seq = 50;
        }
        break;
    case 50:
        func_ov175_021da36c(link->net);
        *seq = 10;
        break;
    case 51:
        func_ov175_021da384(link->net);
        *seq = 52;
        link->idleTimer = 0;
        break;
    case 52:
        countIdle = TRUE;
        if (func_ov175_021da390(link->net)) {
            *seq = 11;
        }
        break;
    case 53:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
    if (countIdle) {
        timedOut = FALSE;
        if (link->idleTimer++ > 1200) {
            link->idleTimer = 0;
            timedOut = TRUE;
        }
        if (func_ov175_021da45c(link->net)) {
            timedOut = TRUE;
        }
        if (timedOut) {
            func_ov175_021da36c(link->net);
            if (!func_0200fb24() && func_0203cd98()) {
                DWC_CloseAllConnectionsHard();
            }
            *seq = 10;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
            func_ov175_021d9d40(link);
            return;
        case 2:
            func_ov177_021e7e0c(link->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
    }
}

void func_ov175_021d8904(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    WifiBattleMatchLinkReportBuf reportBuf;
    RivalDataSave *rivalData;
    u32 ready;

    switch (*seq) {
    case 0:
        if (param->result == 2 && link->hadExchangeResult == 0) {
            func_ov177_021e8aa0(task, func_ov175_021d8bc4);
            return;
        }
        link->hadExchangeResult = 0;
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFIBATTLEMATCH_LINK_DWC, 0x60000);
        func_ov009_02156e30(HEAPID_DEVICE_ALLOC, 0x60000, 1000, HEAPID_WIFIBATTLEMATCH_LINK_DWC);
        func_ov009_02156f50(HEAPID_WIFIBATTLEMATCH_LINK_DWC);
        *seq = 1;
        break;
    case 1:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_070,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 13;
        func_ov177_021e8ab4(task, 2);
        break;
    case 2:
        func_ov175_021da5b0(link->net, 1, 0, param->param->unk4, &param->battleResult->result,
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
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            func_ov009_02156ed0();
            return;
        }
        break;
    case 4:
        func_ov009_02156ed0();
        if (param->result == 2) {
            func_ov177_021e8aa0(task, func_ov175_021d8bc4);
            return;
        }
        *seq = 5;
        break;
    case 5:
        rivalData = getHollow_RivalData(GameData_GetSaveControl(param->param->gameData));
        func_0200ed54(rivalData, getIDAsUInt(&param->otherPlayer->info));
        func_ov175_021da384(link->net);
        *seq = 6;
        break;
    case 6:
        if (func_ov175_021da390(link->net)) {
            *seq = 7;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, TRUE)) {
        case 1:
        case 4:
            *seq = 7;
            return;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            return;
        }
        break;
    case 7:
        if (*link->param->unk20 != 0) {
            break;
        }
        MI_CpuFill8(&reportBuf, 0, sizeof(reportBuf));
        reportBuf.report = *link->param->report;
        reportBuf.unk50 = *link->param->unk28;
        func_ov175_021dbc04(link->net, 3, &reportBuf);
        *seq = 8;
        break;
    case 8:
        ready = func_ov175_021dbdac(link->net);
        if (ready == 1) {
            *link->param->unk20 = 1800;
            *seq = 9;
        }
        if (ready == 0) {
            break;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
            return;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            return;
        }
        break;
    case 9:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_088,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 13;
        func_ov177_021e8ab4(task, 10);
        break;
    case 10:
        param->action = WIFIBATTLEMATCH_LINK_ACTION_RESULT;
        func_ov177_021e8aa8(task);
        break;
    case 11:
    case 12:
        break;
    case 13:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}



void func_ov175_021d8bc4(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    u32 ret;

    switch (*seq) {
    case 0:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_040,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 5;
        func_ov177_021e8ab4(task, 1);
        break;
    case 1:
        func_ov175_021d9d50(link, param->unk34);
        func_02012dac(param->param->gameData);
        *seq = 2;
        break;
    case 2:
        ret = func_02012dd0(param->param->gameData);
        if (ret == 2) {
            *seq = 3;
        } else if (ret == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d8ec8);
        }
        break;
    case 3:
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        case 0:
            func_ov175_021d9a1c(link, func_ov175_021d9090);
            *seq = 4;
            break;
        }
        break;
    case 4:
        ret = func_ov175_021d9a30(link);
        if (ret == 0) {
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
        } else if (ret == 1 || ret == 2) {
            func_ov177_021e8aa0(task, func_ov175_021d8ec8);
        } else if (ret == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 5:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

void func_ov175_021d8cf0(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    u32 ret;
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
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 1:
        GFL_BGSysSetBGEnabled(0, TRUE);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_092,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 7;
        func_ov177_021e8ab4(task, 2);
        break;
    case 2:
        func_ov175_021d98e4(link, COMPETITION_MENU_BATTLE_AGAIN);
        *seq = 3;
        break;
    case 3:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            func_ov177_021e8aa0(task, func_ov175_021d7e18);
            break;
        case 1:
            *seq = 4;
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d8e30);
            break;
        }
        break;
    case 4:
        func_ov175_021d9a1c(link, func_ov175_021d9344);
        *seq = 5;
        break;
    case 5:
        ret = func_ov175_021d9a30(link);
        if (ret == 1 || ret == 2) {
            *seq = 1;
        } else if (ret == 0) {
            *seq = 6;
        }
        switch (func_ov175_021d9f04(link->net, FALSE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 6:
        func_ov177_021e8aa0(task, func_ov175_021d8ec8);
        break;
    case 7:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}



void func_ov175_021d8e30(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    s32 selected;

    switch (*seq) {
    case 0:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_078,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 3;
        func_ov177_021e8ab4(task, 1);
        break;
    case 1:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO);
        *seq = 2;
        break;
    case 2:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            func_ov177_021e8aa0(task, func_ov175_021d8fa8);
            break;
        case 1:
            func_ov177_021e8aa0(task, func_ov175_021d8cf0);
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

void func_ov175_021d8ec8(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
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
        func_ov177_021e8ab4(task, 4);
        *seq = 7;
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
        func_ov177_021e8aa0(task, func_ov175_021d905c);
        break;
    case 7:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}


void func_ov175_021d8fa8(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    u32 ret;

    switch (*seq) {
    case 0:
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        case 0:
            func_ov175_021d9a1c(link, func_ov175_021d9090);
            *seq = 1;
            break;
        }
        break;
    case 1:
        ret = func_ov175_021d9a30(link);
        if (ret != SUB_TASK_RUNNING) {
            *seq = 2;
        }
        if (ret == 3) {
            func_ov177_021e8aa0(task, func_ov175_021d9064);
        }
        switch (func_ov175_021d9f04(link->net, TRUE, FALSE)) {
        case 1:
        case 2:
            func_ov177_021e8aa0(task, func_ov175_021d9064);
            break;
        }
        break;
    case 2:
        func_ov177_021e8aa0(task, func_ov175_021d8ec8);
        break;
    case 3:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}



// pokebw2's WifiBattleMatchLink_CompetitionTaskEnd
void func_ov175_021d905c(WifiBattleMatchTask *task, u32 *seq, void *work) {
    func_ov177_021e8aa8(task);
}

// pokebw2's WifiBattleMatchLink_CompetitionTaskRelogin
void func_ov175_021d9064(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;

    link->param->action = WIFIBATTLEMATCH_LINK_ACTION_RELOGIN;
    if (link->net != NULL) {
        func_ov175_021d9ea0(link->net);
        link->net = NULL;
    }
    func_0200fbb0();
    func_ov177_021e8aa8(task);
}

void func_ov175_021d9090(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    void *entry;
    RTCDate startDate;
    RTCDate endDate;
    RTCDate date;
    RTCTime time;
    BOOL valid;
    s32 startDay;
    s32 endDay;
    s32 today;
    u8 accountState;
    u32 ret;
    WifiBattleMatchLinkRequest request;

    switch (*seq) {
    case 0:
        *seq = 1;
        break;
    case 1:
        entry = link->entry;
        if (!func_0203cd98()) {
            GFL_NetErrAbort();
        }
        valid = DWC_GetDateTime(&date, &time);
        if (valid) {
            RTC_ConvertSecondToDateTime(&date, &time, RTC_ConvertDateTimeToSecond(&date, &time));
        }
        startDate.year = func_0200b078(entry, 4);
        startDate.month = func_0200b078(entry, 5);
        startDate.day = func_0200b078(entry, 6);
        startDate.week = RTC_WEEK_SUNDAY;
        endDate.year = func_0200b078(entry, 7);
        endDate.month = func_0200b078(entry, 8);
        endDate.day = func_0200b078(entry, 9);
        endDate.week = RTC_WEEK_SUNDAY;
        startDay = RTC_ConvertDateToDay(&startDate);
        endDay = RTC_ConvertDateToDay(&endDate);
        today = RTC_ConvertDateToDay(&date);
        if (valid && today < startDay) {
            link->subTaskResult = 2;
            *seq = 2;
            return;
        }
        if (valid && startDay <= today && today <= endDay) {
            link->subTaskResult = 0;
            *seq = 13;
            return;
        }
        link->subTaskResult = 1;
        *seq = 3;
        break;
    case 2:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_096,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 14;
        func_ov177_021e8ab4(task, 13);
        break;
    case 3:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_080,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 14;
        func_ov177_021e8ab4(task, 4);
        break;
    case 4:
        accountState = param->account->unk5;
        if (accountState == 1 || accountState == 2) {
            *seq = 5;
        } else {
            *seq = 13;
        }
        break;
    case 5:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_040,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 14;
        func_ov177_021e8ab4(task, 6);
        break;
    case 6: {
        BattleBoxSave *battleBox = getBattleBox(GameData_GetSaveControl(param->param->gameData));

        link->hadBattleBox = func_0200bbfc(battleBox, 1);
        func_0200bbd8(battleBox, 1);
        func_0200b110(link->entry, 10, 3);
        func_02012dac(param->param->gameData);
        *seq = 7;
        break;
    }
    case 7:
        if (func_02012dd0(param->param->gameData) == 2) {
            *seq = 8;
        }
        break;
    case 8:
        getBattleBox(GameData_GetSaveControl(param->param->gameData));
        if (link->hadBattleBox != 0) {
            *seq = 9;
        } else {
            *seq = 10;
        }
        break;
    case 9:
        func_ov175_021d97e4(link, 1);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_081,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 14;
        func_ov177_021e8ab4(task, 10);
        break;
    case 10:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_070,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 14;
        func_ov177_021e8ab4(task, 11);
        break;
    case 11:
        MI_CpuFill8(&request, 0, sizeof(request));
        request.unk1 = 3;
        request.unk4 = func_ov175_021dd020(link->net);
        link->param->account->unk5 = 3;
        func_ov175_021dc8b4(link->net, &request, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 12;
        break;
    case 12:
        ret = func_ov175_021dc908(link->net);
        if (ret == 1) {
            *seq = 13;
        } else if (ret == 3) {
            link->subTaskResult = 3;
            *seq = 13;
        }
        break;
    case 13:
        func_ov177_021e8aa8(task);
        break;
    case 14:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}


void func_ov175_021d9344(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    WifiBattleMatchLinkParam *param = link->param;
    WifiBattleMatchLinkRequest request;
    u8 accountState;
    u32 ret;
    s32 selected;

    switch (*seq) {
    case 0:
        accountState = param->account->unk5;
        if (accountState == 1 || accountState == 2) {
            link->subTaskResult = 1;
            *seq = 2;
        } else {
            link->subTaskResult = 2;
            *seq = 1;
        }
        break;
    case 1:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_082,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 16;
        func_ov177_021e8ab4(task, 15);
        break;
    case 2:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_083,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 16;
        func_ov177_021e8ab4(task, 3);
        break;
    case 3:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO_NO_FIRST);
        *seq = 4;
        break;
    case 4:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 5;
            break;
        case 1:
            *seq = 15;
            break;
        }
        break;
    case 5:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_084,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 16;
        func_ov177_021e8ab4(task, 6);
        break;
    case 6:
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO_NO_FIRST);
        *seq = 7;
        break;
    case 7:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
        switch (selected) {
        case 0:
            *seq = 8;
            break;
        case 1:
            *seq = 15;
            break;
        }
        break;
    case 8:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_040,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 16;
        func_ov177_021e8ab4(task, 9);
        break;
    case 9:
        func_0200bbd8(getBattleBox(GameData_GetSaveControl(param->param->gameData)), 1);
        func_0200b110(link->entry, 10, 4);
        func_02012dac(param->param->gameData);
        *seq = 10;
        break;
    case 10:
        ret = func_02012dd0(param->param->gameData);
        if (ret == 2) {
            link->subTaskResult = 0;
            *seq = 11;
        } else if (ret == 3) {
            *seq = 15;
        }
        break;
    case 11:
        func_ov175_021d97e4(link, 1);
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_081,
                            WIFIBATTLEMATCH_MSG_STREAM_KEY);
        *seq = 16;
        func_ov177_021e8ab4(task, 12);
        break;
    case 12:
        func_ov177_021e8738(link->msgBox, link->msgData, EventFishing_Text_070,
                            WIFIBATTLEMATCH_MSG_QUEUE_WAIT_ICON);
        *seq = 16;
        func_ov177_021e8ab4(task, 13);
        break;
    case 13:
        MI_CpuFill8(&request, 0, sizeof(request));
        request.unk1 = 4;
        request.unk4 = func_ov175_021dd020(link->net);
        link->param->account->unk5 = 4;
        func_ov175_021dc8b4(link->net, &request, HEAPID_WIFIBATTLEMATCH_LINK);
        *seq = 14;
        break;
    case 14:
        ret = func_ov175_021dc908(link->net);
        if (ret == 1) {
            *seq = 15;
        } else if (ret == 3) {
            link->subTaskResult = 3;
            *seq = 15;
        }
        break;
    case 15:
        func_ov177_021e8aa8(task);
        break;
    case 16:
        if (func_ov177_021e8844(link->msgBox)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}


// pokebw2's WifiBattleMatchLink_CompetitionTaskSignTeam
void func_ov175_021d95b0(WifiBattleMatchTask *task, u32 *seq, void *work) {
    WifiBattleMatchLinkCompetition *link = work;
    WifiBattleMatchLinkPlayer *player = link->param->player;
    u32 ret;

    switch (*seq) {
    case 0:
        *seq = 1;
        break;
    case 1:
        MI_CpuFill8(&link->signature, 0, sizeof(link->signature));
        func_ov175_021dcb58(link->net, player->partyData, TRUE);
        *seq = 2;
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
            func_ov175_021d9b18(player, &link->signature);
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

void func_ov175_021d965c(WifiBattleMatchLinkCompetition *link) {
    WifiBattleMatchWifiCardInfo info;
    PlayerInfo *playerInfo;
    ClActUnit *unit;
    void *previous;
    void *entry;
    u32 first;

    if (link->card != NULL) {
        return;
    }
    entry = link->entry;
    playerInfo = GetGameDataPlayerInfo(link->param->param->gameData);
    unit = func_ov177_021e5548(link->graphic);
    previous = func_0200b204(getTournamentBlockAddress(GameData_GetSaveControl(link->param->param->gameData)), 0);
    MI_CpuFill8(&info, 0, sizeof(info));
    MI_CpuCopy8(func_0200b074(entry), &info, 0x92);
    info.startDate = WifiBattleMatchLink_PackBytes(func_0200b078(entry, 4), func_0200b078(entry, 5),
                                                   func_0200b078(entry, 6));
    info.endDate = WifiBattleMatchLink_PackBytes(func_0200b078(entry, 7), func_0200b078(entry, 8),
                                                 func_0200b078(entry, 9));
    info.look = func_02008554(playerInfo);
    info.unkA0 = func_0200dfd8(link->param->record, 10, 0);
    first = func_0200dfd8(link->param->record, 10, 1);
    info.unkA4 = first + func_0200dfd8(link->param->record, 10, 2);
    info.variant = func_0200b078(entry, 11);
    if (info.unkA4 == 0 && info.unkA0 == 0) {
        info.unkA0 = 1500;
    }
    link->card = func_ov177_021e5d68(&info, 0, playerInfo, unit, link->res, link->font, link->printQueue,
                                     link->msgData, link->wordSet, previous, FALSE, HEAPID_WIFIBATTLEMATCH_LINK);
}


void func_ov175_021d97a4(WifiBattleMatchLinkCompetition *link) {
    if (link->card != NULL) {
        func_ov177_021e5ef8(link->card);
        link->card = NULL;
    }
}


// pokebw2's WifiBattleMatchLink_CompetitionIsCardShown
BOOL func_ov175_021d97b8(WifiBattleMatchLinkCompetition *link) {
    return func_ov177_021e64c8(link->card);
}

// pokebw2's WifiBattleMatchLink_CompetitionIsCardHidden
BOOL func_ov175_021d97c4(WifiBattleMatchLinkCompetition *link) {
    return func_ov177_021e65fc(link->card);
}

// pokebw2's WifiBattleMatchLink_CompetitionShowCard
void func_ov175_021d97d0(WifiBattleMatchLinkCompetition *link) {
    func_ov175_021d965c(link);
    while (!func_ov175_021d97b8(link)) {
    }
}

// pokebw2's WifiBattleMatchLink_CompetitionSetCardPage
void func_ov175_021d97e4(WifiBattleMatchLinkCompetition *link, u32 page) {
    if (link->card != NULL) {
        func_ov177_021e5f24(link->card, page, link->msgData, link->printQueue, link->font,
                                         HEAPID_WIFIBATTLEMATCH_LINK);
    }
}

void func_ov175_021d9804(WifiBattleMatchLinkCompetition *link, WifiBattleMatchLinkPlayer *partner) {
    ClActUnit *unit;

    if (link->partnerCard == NULL) {
        unit = func_ov177_021e5548(link->graphic);
        link->partnerCard = func_ov177_021e7520((const LiveBattleMatchPlayerData *)partner, unit, link->res,
                                                link->font, link->printQueue, link->msgData, link->wordSet, 0, 1,
                                                func_0200b078(link->entry, 11), HEAPID_WIFIBATTLEMATCH_LINK);
    }
}


// pokebw2's WifiBattleMatchLink_CompetitionDeletePartnerCard
void func_ov175_021d9854(WifiBattleMatchLinkCompetition *link) {
    if (link->partnerCard != NULL) {
        func_ov177_021e7710(link->partnerCard);
        link->partnerCard = NULL;
    }
}

// pokebw2's WifiBattleMatchLink_CompetitionIsPartnerCardDone
BOOL func_ov175_021d9868(WifiBattleMatchLinkCompetition *link) {
    return func_ov177_021e777c(link->partnerCard);
}

void func_ov175_021d9874(WifiBattleMatchLinkCompetition *link) {
    ClActUnit *unit;

    if (link->teamList == NULL) {
        unit = func_ov177_021e5548(link->graphic);
        link->teamList = func_ov177_021e7e3c(getBattleBox(GameData_GetSaveControl(link->param->param->gameData)), unit,
                                             link->res, link->smallFont, link->printQueue, link->msgData,
                                             HEAPID_WIFIBATTLEMATCH_LINK);
    }
}


// pokebw2's WifiBattleMatchLink_CompetitionDeleteTeamList
void func_ov175_021d98b8(WifiBattleMatchLinkCompetition *link) {
    if (link->teamList != NULL) {
        func_ov177_021e82bc(link->teamList);
        link->teamList = NULL;
    }
}

// pokebw2's WifiBattleMatchLink_CompetitionIsTeamListShown
BOOL func_ov175_021d98cc(WifiBattleMatchLinkCompetition *link) {
    return func_ov177_021e8338(link->teamList);
}

// pokebw2's WifiBattleMatchLink_CompetitionIsTeamListHidden
BOOL func_ov175_021d98d8(WifiBattleMatchLinkCompetition *link) {
    return func_ov177_021e8420(link->teamList);
}

// pokebw2's WifiBattleMatchLink_CompetitionCreateMenu
void func_ov175_021d98e4(WifiBattleMatchLinkCompetition *link, u32 kind) {
    WifiBattleMatchMenuParam menuParam;

    if (link->menu != NULL) {
        return;
    }
    MI_CpuFill8(&menuParam, 0, sizeof(menuParam));
    menuParam.msgData = link->msgData;
    menuParam.font = link->font;
    menuParam.printQueue = link->printQueue;
    menuParam.bg = 0;
    menuParam.palette = 15;
    menuParam.framePalette = 13;
    menuParam.frameChar = 1;
    switch (kind) {
    case 5:
        menuParam.cursorRow = 1;
        // fall through
    case 0:
        menuParam.msgIds[0] = EventFishing_Text_052;
        menuParam.msgIds[1] = EventFishing_Text_053;
        menuParam.count = 2;
        menuParam.canCancel = TRUE;
        menuParam.cancelValue = 1;
        link->menu = func_ov177_021e888c(&menuParam, HEAPID_WIFIBATTLEMATCH_LINK);
        break;
    case 1:
        menuParam.msgIds[0] = EventFishing_Text_047;
        menuParam.msgIds[1] = EventFishing_Text_048;
        menuParam.msgIds[2] = EventFishing_Text_049;
        menuParam.count = 3;
        menuParam.canCancel = TRUE;
        menuParam.cancelValue = 2;
        link->menu = func_ov177_021e888c(&menuParam, HEAPID_WIFIBATTLEMATCH_LINK);
        break;
    case 2:
        menuParam.msgIds[0] = EventFishing_Text_066;
        menuParam.msgIds[1] = EventFishing_Text_067;
        menuParam.count = 2;
        menuParam.canCancel = TRUE;
        menuParam.cancelValue = 1;
        link->menu = func_ov177_021e888c(&menuParam, HEAPID_WIFIBATTLEMATCH_LINK);
        break;
    case 3:
        menuParam.msgIds[0] = EventFishing_Text_075;
        menuParam.msgIds[1] = EventFishing_Text_076;
        menuParam.msgIds[2] = EventFishing_Text_077;
        menuParam.count = 3;
        menuParam.canCancel = TRUE;
        menuParam.cancelValue = 2;
        link->menu = func_ov177_021e88b0(&menuParam, 3, 6, 26, 6, HEAPID_WIFIBATTLEMATCH_LINK);
        break;
    case 4:
        menuParam.msgIds[0] = EventFishing_Text_093;
        menuParam.msgIds[1] = EventFishing_Text_094;
        menuParam.msgIds[2] = EventFishing_Text_095;
        menuParam.count = 3;
        menuParam.canCancel = TRUE;
        menuParam.cancelValue = 2;
        link->menu = func_ov177_021e88b0(&menuParam, 3, 6, 26, 6, HEAPID_WIFIBATTLEMATCH_LINK);
        break;
    }
}

// pokebw2's WifiBattleMatchLink_CompetitionDeleteMenu
void func_ov175_021d99f4(WifiBattleMatchLinkCompetition *link) {
    if (link->menu != NULL) {
        func_ov177_021e89d0(link->menu);
        link->menu = NULL;
    }
}

// pokebw2's WifiBattleMatchLink_CompetitionGetMenuSelection
s32 func_ov175_021d9a08(WifiBattleMatchLinkCompetition *link) {
    if (link->menu != NULL) {
        return func_ov177_021e8a00(link->menu);
    }
    return -1;
}

void func_ov175_021d9a1c(WifiBattleMatchLinkCompetition *link,
                                                        WifiBattleMatchTaskFunc func) {
    func_ov177_021e8aa0(link->subTask, func);
    link->subTaskResult = SUB_TASK_RUNNING;
}

// pokebw2's WifiBattleMatchLink_CompetitionUpdateSubTask
u32 func_ov175_021d9a30(WifiBattleMatchLinkCompetition *link) {
    func_ov177_021e8a80(link->subTask);
    if (func_ov177_021e8a90(link->subTask)) {
        return link->subTaskResult;
    }
    return SUB_TASK_RUNNING;
}

void func_ov175_021d9a50(WifiBattleMatchLinkPlayer *player,
                                                              WifiBattleMatchLinkCompetition *link) {
    PokeParty *party;

    MI_CpuFill8(player, 0, sizeof(WifiBattleMatchLinkPlayer));
    party = convertBoxedPokeSetToParty(getBattleBox(GameData_GetSaveControl(link->param->param->gameData)),
                                       HEAPID_WIFIBATTLEMATCH_LINK);
    PokeParty_Init((PokeParty *)player->partyData);
    MI_CpuCopy8(party, player->partyData, PokeParty_GetSaveDataSize());
    GFL_HeapFree(party);
    player->unk44 = 100;
}

void func_ov175_021d9a9c(WifiBattleMatchLinkPlayer *player,
                                                      WifiBattleMatchLinkCompetition *link) {
    SaveControl *save = GameData_GetSaveControl(link->param->param->gameData);
    WifiBattleMatchLinkParam *param;
    void *entry;

    func_02008494(GetGameDataPlayerInfo(link->param->param->gameData), &player->info);
    func_0200e6fc(func_0200e6e8(save), 0, &player->sentence);
    param = link->param;
    player->record1 = param->unk34->unk8;
    player->record2 = param->unk34->unkC;
    player->unk34 = param->unk34->unk10;
    player->unk4C = func_ov175_021dc194(link->net);
    player->gsid = func_02009cc0(func_0201283c(link->param->param->gameData));
    entry = link->entry;
    player->entryField2 = func_0200b078(entry, 2);
    player->checksum = CRC16_350bytes(entry);
}

void func_ov175_021d9b18(WifiBattleMatchLinkPlayer *player,
                                                         WifiBattleMatchNetCheck *signature) {
    MI_CpuCopy8(signature->signature, player->signature, sizeof(player->signature));
}

BOOL func_ov175_021d9b2c(WifiBattleMatchLinkPlayer *player, PokeParty *party, HeapID heapId) {
    void *array;
    int i;
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


// pokebw2's WifiBattleMatchLink_CompetitionCompareParties
BOOL func_ov175_021d9bc4(PokeParty *party, PokeParty *otherParty) {
    u32 count = PokeParty_GetPkmCount(party);
    u32 i;

    if (count == PokeParty_GetPkmCount(otherParty)) {
        for (i = 0; i < count; i++) {
            PartyPkm *pkm = PokeParty_GetPkm(party, i);
            PartyPkm *otherPkm = PokeParty_GetPkm(otherParty, i);

            if (GFL_STD_MemCmp(func_020185f0(pkm), func_020185f0(otherPkm), PML_GetPkmRawSize()) != 0) {
                return FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

u32 func_ov175_021d9c34(WifiBattleMatchLinkCompetition *link, BOOL canCancel, BOOL canDisconnect) {
    s32 selected;

    switch (link->cancelSeq) {
    case 0:
        if (canCancel && (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
            link->cancelSeq++;
        }
        break;
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
        func_ov175_021d98e4(link, COMPETITION_MENU_YES_NO);
        link->cancelSeq++;
        break;
    case 4:
        selected = func_ov175_021d9a08(link);
        if (selected == -1) {
            break;
        }
        func_ov175_021d99f4(link);
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
    return link->cancelSeq == 0 ? CANCEL_NONE : CANCEL_BUSY;
}


// pokebw2's WifiBattleMatchLink_CompetitionResetCancel
void func_ov175_021d9d40(WifiBattleMatchLinkCompetition *link) {
    link->cancelSeq = 0;
    func_ov175_021d99f4(link);
}

BOOL func_ov175_021d9d50(WifiBattleMatchLinkCompetition *link,
                                                        WifiBattleMatchLinkRanking *ranking) {
    BOOL changed = FALSE;

    if (ranking->unk10 != func_0200dfd8(link->param->record, 10, 0) ||
        ranking->unk8 != func_0200dfd8(link->param->record, 10, 1) ||
        ranking->unkC != func_0200dfd8(link->param->record, 10, 2)) {
        changed = TRUE;
    }
    func_0200dfe4(link->param->record, 10, 0, ranking->unk10);
    func_0200dfe4(link->param->record, 10, 1, ranking->unk8);
    func_0200dfe4(link->param->record, 10, 2, ranking->unkC);
    return changed;
}
