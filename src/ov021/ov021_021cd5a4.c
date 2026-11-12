// Black's code at ov021 0x021cd5a4-0x021cf45c. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/union_oneself.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/std.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/wordset.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/key.h"
#include "nitro/os.h"
#include "gfl/net_system.h"
#include "field/field_msgbg.h"
#include "nitro/math.h"


// The beacon of the activity's network, which its own setup data fill in (pokebw2's CommEntryBeacon)
typedef struct CommEntryBeacon {
    PlayerInfo info;
    u8 mac[6];
    u16 gameId;
} CommEntryBeacon;

// The game ID of the beacons of a group being gathered
#define COMM_ENTRY_BEACON_GAME_ID 0x3a0b

// What a player sends to ask to join (pokebw2's CommEntryPacket)
typedef struct CommEntryPacket {
    PlayerInfo info;
    u8 mac[6];
    u8 autoAccept;
} CommEntryPacket;

// A player who asked to join the leader, by network ID (pokebw2's CommEntryMember)
typedef struct CommEntryMember {
    PlayerInfo info;
    u8 state;
    // Whether to accept it without asking the leader
    u8 autoAccept;
    u8 unk22;
    u8 mac[6];
} CommEntryMember;

// A leader whose beacon was heard (pokebw2's CommEntryLeader)
typedef struct CommEntryLeader {
    PlayerInfo info;
    u8 mac[6];
    u8 valid;
    // The frames until it leaves the list
    s16 timer;
} CommEntryLeader;

// The yes/no menu and the questions that use it (pokebw2's CommEntryYesNo)
typedef struct CommEntryYesNo {
    ListMenuUI *window;
    u32 result;
    u8 seq;
} CommEntryYesNo;

// The list of the leaders heard, to pick one to ask (pokebw2's CommEntryLeaderList)
typedef struct CommEntryLeaderList {
    ListMenuOption *options;
    ListMenuUI *menu;
    CommEntryLeader leaders[16];
    CommEntryLeader *selected;
    u32 result;
    u8 seq;
    // Whether a leader came or went since the options were built
    u8 changed;
    u8 optionsBuilt;
    // The step to go back to when the player doesn't leave after all
    u8 prevSeq;
    u16 timer;
} CommEntryLeaderList;

// The members as the leader sends them, by network ID (pokebw2's CommEntryMemberList)
typedef struct CommEntryMemberList {
    PlayerInfo players[4];
    u8 mask;
} CommEntryMemberList;

// pokebw2's CommEntryMenu, the same layout in Black (0x580 bytes)
struct CommEntryMenu {
    Field *field;
    WordSet *wordSet;
    MsgData *msgData;
    // The members' names and the title above them
    MsgBGSysWin *membersWin;
    MsgBGSysWin *titleWin;
    MsgBGTalkWin *msgWin;
    StrBuf *msgBuf;
    StrBuf *memberFmt;
    StrBuf *memberBuf;
    StrBuf *countFmt;
    StrBuf *countBuf;
    u32 result;
    PlayerInfo self;
    CommEntryMember members[4];
    // The player the leader is asked about
    BOOL asking;
    CommEntryMember asked;
    CommEntryYesNo yesNo;
    CommEntryLeaderList list;
    // The network's setup, kept to start it again after a leader refuses
    GFLNetInitData netInit;
    void *netWork;
    UnionSystem *callbackWork;
    CommEntryMenuCallback onJoin;
    CommEntryMenuCallback onLeave;
    CommEntryMemberList memberList;
    u8 memberListChanged;
    // The leader started the activity, or disbanded the group
    u8 started;
    u8 cancelled;
    // Whether the commands go to every machine, for the Musical
    u8 sendToAll;
    u8 minCount;
    u8 maxCount;
    u8 seq;
    u8 kind;
    u8 mode;
    // The members to send an answer to, as bits
    u8 acceptMask;
    u8 refuseMask;
    // Whether to send the member list, whether it changed again while it was being sent, and whether it was sent
    u8 sendMemberList;
    u8 memberListPending;
    u8 memberListSent;
    // The members the leader checks on before the activity starts, as bits
    u8 checkMask;
    HeapID heapId;
    // The leader's answer, and its info
    u32 reply;
    PlayerInfo replyInfo;
    u8 savedStates[4];
    // A MAC address that the creator gives, which nothing reads
    u8 mac[6];
    // The MAC address of the member each row shows
    u8 shownMacs[4][6];
    s8 shownCount;
    u8 readyCount;
    u16 nextMsg;
    // Whether the leader checked on the members, and whether to answer it
    u8 checked;
    u8 sendCheckReply;
    u8 readyMask;
    u8 showWaitMsg;
    u16 lastMsg;
    u8 unk57C[4];
};

// The beacon this machine sends (pokebw2's CommEntryMenu_GetBeacon)
static inline CommEntryBeacon *CommEntryMenu_GetBeacon(void) {
    return func_0203d798()->unk18(func_0203d650());
}

void func_ov021_02190740(MsgBGSysWin *param_1, u16 param_2, u16 param_3, u32 param_4);
// The messages that depend on the activity (pokebw2's CommEntryMessages)
typedef struct {
    // the heading while looking for others
    u16 title;
    // the question about cancelling
    u16 cancelAsk;
    // the notice that it was cancelled
    u16 cancelled;
    // the notice that nobody has joined
    u16 noMembers;
    // the notice about a failure
    u16 error;
    // the instruction to pick a group to join
    u16 chooseLeader;
} CommEntryMessages;

// The messages of each activity: multi battle, musical, Battle Subway
static const CommEntryMessages data_ov021_021db498[3] ATTRIBUTE_ALIGN(4) = {
    { 4, 12, 16, 20, 23, 28 },
    { 5, 13, 17, 20, 24, 28 },
    { 6, 14, 18, 20, 25, 29 },
};
void func_ov021_02190490(FieldMsgBG *msgBGSys);
void func_ov021_02190794(MsgBGSysWin *window, int x, int y, int width, int height);
static const NetCommand data_ov021_021db4bc[8];
static const u8 data_ov021_021db470[6] = { 0 };
// The list of the leaders heard, five rows at a time
static const ListMenuRequest data_ov021_021db478 = {
    .count = 16,
    .unk02 = 5,
    .unk05 = 13,
    .unk07_4 = 1,
    .unk08_0 = 15,
    .unk08_4 = 2,
    .unk10 = 12,
    .rowHeight = 16,
    .x = 1,
    .y = 1,
    .width = 22,
    .height = 10,
};
void func_ov021_021ce6f0(CommEntryMenu *menu, u32 param_2);
void func_ov021_021ce934(CommEntryMenu *menu, PlayerInfo *param_2, s16 param_3);
void func_ov021_021cee78(CommEntryMenu *menu, const u8 *param_2, PlayerInfo *param_3);
void func_ov021_021cef08(CommEntryMenu *menu, s32 param_2);
u32 func_ov021_021ce3e8(CommEntryMenu *menu, CommEntryMember *member);
void func_ov021_021cea84(CommEntryMenu *menu, u32 param_2);
void func_ov021_021ceab0(CommEntryMenu *menu);
u32 func_ov021_021ce760(CommEntryMenu *menu);
void func_ov021_021ce81c(CommEntryMenu *menu);
void func_ov021_021cf17c(void);
u32 func_ov021_021cd6d0(CommEntryMenu *menu);
u32 func_ov021_021cdab8(CommEntryMenu *menu);
u32 func_ov021_021cdd5c(CommEntryMenu *menu);
u32 func_ov021_021cdf18(CommEntryMenu *menu);
void func_ov021_021ce3d8(CommEntryMenu *menu, s32 param_2);
void func_ov021_021ce9e4(CommEntryMenu *menu, u32 param_2);
u32 func_ov021_021cea50(CommEntryMenu *menu);
void func_ov021_021ce8b0(CommEntryMenu *menu, CommEntryMember *member, s32 param_3);
void func_ov021_021ce8f4(CommEntryMenu *menu, CommEntryMember *member, u32 param_3);
u32 func_ov021_021ce84c(CommEntryMenu *menu, s32 param_2);
u32 func_ov021_021ce78c(CommEntryMenu *menu);
BOOL func_ov021_021cf220(u8 param_1, s32 param_2, PlayerInfo *param_3);
BOOL func_ov021_021cf288(u8 param_1, s32 param_2, PlayerInfo *param_3);
u8 func_ov021_021ce1f4(CommEntryMenu *menu);
BOOL func_ov021_021cf3fc(CommEntryMemberList *param_1, u8 param_2, s32 param_3);
s32 func_ov021_021ce73c(CommEntryMenu *menu);
void func_ov021_021cee00(CommEntryMenu *menu);
void func_ov021_021cee48(CommEntryMenu *menu);
u32 func_ov021_021ceda0(CommEntryMenu *menu);
void func_ov021_021cef1c(CommEntryMenu *menu);
BOOL func_ov021_021cf1ac(PlayerInfo *param_1, u8 param_2, s32 param_3);
void func_ov021_021ceb64(CommEntryMenu *menu);
void func_ov021_021ceb20(CommEntryMenu *menu);
u32 func_ov021_021ceb84(CommEntryMenu *menu);
void func_ov021_021ce7ac(CommEntryMenu *menu);
u32 func_ov021_021cf068(CommEntryMenu *menu);
BOOL func_ov021_021cf32c(s32 param_1);
void func_ov021_021ce9a0(CommEntryMenu *menu);
void func_ov021_021cf0f4(CommEntryMenu *menu);
void func_ov021_021cf164(CommEntryMenu *menu);
void func_ov021_021ce984(CommEntryMenu *menu, s32 param_2);
void func_ov021_021ce24c(CommEntryMenu *menu);
void func_ov021_021ce2bc(CommEntryMenu *menu);
void func_ov021_021ce12c(CommEntryMenu *menu);
void func_ov021_021ce36c(CommEntryMenu *menu);
void func_ov021_021ce41c(CommEntryMenu *menu);
void func_ov021_021cea28(CommEntryMenu *menu);
u32 func_ov021_021ce52c(CommEntryMenu *menu);
u32 func_ov021_021ce5ac(CommEntryMenu *menu);
BOOL func_ov021_021cf3b0(s32 param_1);
BOOL func_ov021_021cf2f0(s32 param_1);
BOOL func_ov021_021cf378(s32 param_1);
u32 func_ov021_021cf12c(u32 param_1);




CommEntryMenu *func_ov021_021cd48c(PlayerInfo *info, Field *field, u32 minCount, u32 maxCount, HeapID heapId, u32 kind,
                                   u32 mode, const u8 *mac) {
    FieldMsgBG *msgBGSys = Field_GetMsgBGSys(field);
    CommEntryMenu *menu = GFL_HeapAllocate(heapId, sizeof(CommEntryMenu), TRUE, "comm_entry_menu.c", 377);

    menu->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    menu->msgData = func_ov021_021904b0(msgBGSys, 0x1e);
    menu->msgWin = func_ov021_02190f64(msgBGSys, menu->msgData);
    menu->msgBuf = GFL_StrBufCreate(100, heapId);
    menu->memberFmt = GFL_MsgDataLoadStrbufNew(menu->msgData, 7);
    menu->memberBuf = GFL_StrBufCreate(64, heapId);
    if (mode == 1) {
        menu->countFmt = GFL_MsgDataLoadStrbufNew(menu->msgData, 3);
    } else {
        menu->countFmt = GFL_MsgDataLoadStrbufNew(menu->msgData, 2);
    }
    menu->countBuf = GFL_StrBufCreate(32, heapId);
    menu->minCount = minCount;
    menu->maxCount = maxCount;
    menu->field = field;
    menu->kind = kind;
    menu->mode = mode;
    menu->heapId = heapId;
    menu->shownCount = -1;
    menu->nextMsg = 0xffff;
    func_02008494(info, &menu->self);
    if (mac != NULL) {
        MI_CpuCopy8(mac, menu->mac, 6);
    }
    func_0203d7bc(FALSE);
    func_ov021_021cf164(menu);
    if (mode == 1) {
        menu->sendToAll = 1;
    }
    CommEntryMenu_GetBeacon()->gameId = COMM_ENTRY_BEACON_GAME_ID;
    return menu;
}

u32 func_ov021_021cd5a4(CommEntryMenu *menu) {
    u32 mask = func_ov021_021ce760(menu);
    func_ov021_021ce81c(menu);
    if (menu->yesNo.window != NULL) {
        func_ov021_021909bc(menu->yesNo.window);
        menu->yesNo.window = NULL;
    }
    GFL_StrBufFree(menu->countBuf);
    GFL_StrBufFree(menu->countFmt);
    GFL_StrBufFree(menu->memberBuf);
    GFL_StrBufFree(menu->memberFmt);
    GFL_StrBufFree(menu->msgBuf);
    func_ov021_02190e40(menu->msgWin);
    func_ov021_021904c8(menu->msgData);
    GFL_WordSetSystemFree(menu->wordSet);
    if (func_0203cd98() == 1) {
        func_ov021_021cf17c();
    }
    GFL_HeapFree(menu);
    return mask;
}

void func_ov021_021cd60c(CommEntryMenu *menu, UnionSystem *unisys, CommEntryMenuCallback onJoin,
                         CommEntryMenuCallback onLeave) {
    menu->callbackWork = unisys;
    menu->onJoin = onJoin;
    menu->onLeave = onLeave;
}

void func_ov021_021cd624(CommEntryMenu *menu, u32 netId, PlayerInfo *info, u8 a3, const u8 *mac) {
    CommEntryMember *member = &menu->members[netId];
    CommEntryMenuCallback onJoin;
    func_02008494(info, &member->info);
    member->state = 1;
    member->autoAccept = a3;
    MI_CpuCopy8(mac, member->mac, 6);
    func_ov021_021cea84(menu, (u8)netId);
    func_ov021_021ceab0(menu);
    onJoin = menu->onJoin;
    if (onJoin != NULL) {
        onJoin(menu->callbackWork, info, mac);
    }
}

u32 func_ov021_021cd688(CommEntryMenu *menu) {
    u32 result;
    switch (menu->kind) {
    case 0:
        result = func_ov021_021cd6d0(menu);
        break;
    case 1:
        result = func_ov021_021cdab8(menu);
        break;
    case 2:
        result = func_ov021_021cdd5c(menu);
        break;
    case 3:
        result = func_ov021_021cdf18(menu);
        break;
    }
    if (result == 1) {
        return menu->result;
    }
    return 0;
}

u32 func_ov021_021cd6d0(CommEntryMenu *menu) {
    u8 idx;
    u8 mask;
    u32 r;
    s32 sv;
    u16 old;
    if (menu->seq != 0) {
        func_ov021_021ce24c(menu);
        func_ov021_021ce2bc(menu);
        func_ov021_021ce12c(menu);
    }
    switch (menu->seq) {
    case 0:
        func_ov021_021ce7ac(menu);
        func_ov021_021ce9e4(menu, 8);
        menu->seq++;
        break;
    case 1:
        if (menu->mode == 0) {
            menu->seq = 4;
        } else {
            func_0203d01c();
            menu->seq++;
        }
        break;
    case 2:
        if (func_020392b8() > 1) {
            if (func_02039acc() == 1) {
                menu->seq++;
            }
        } else if (GCTX_HIDGetPressedKeys() & 2) {
            menu->seq = 7;
            menu->result = 2;
            menu->seq = 0x13;
        }
        break;
    case 3:
        if (func_020399b8(func_020399a8()) == 1) {
            menu->seq = 4;
        }
        break;
    case 4:
        if (func_ov021_021cf1ac(&menu->self, 1, menu->sendToAll) == 1) {
            menu->seq++;
        }
        break;
    case 5:
        func_ov021_021ce36c(menu);
        func_ov021_021ce41c(menu);
        if (func_ov021_021ce78c(menu) != 1) {
            break;
        }
        sv = func_ov021_021ce73c(menu);
        old = menu->nextMsg;
        menu->nextMsg = 0xffff;
        if (sv >= menu->maxCount || (sv >= menu->minCount && (GCTX_HIDGetPressedKeys() & 1))) {
            func_ov021_021cea28(menu);
            menu->seq = 6;
        } else if (GCTX_HIDGetPressedKeys() & 2) {
            menu->seq = 7;
        } else if (menu->mode == 0 && func_0203d1b4() <= 1) {
            menu->seq = 0x11;
        } else if (old != 0xffff) {
            func_ov021_021ce9e4(menu, old);
        }
        break;
    case 6:
        r = func_ov021_021ce52c(menu);
        if (r == 1) {
            func_0203d1e8(func_020399a8(), 0);
            menu->seq = 9;
        } else if (r == 2) {
            if (func_ov021_021ce73c(menu) >= menu->maxCount) {
                menu->seq = 7;
            } else {
                menu->seq = 5;
                if (func_ov021_021ce73c(menu) > 1 && menu->mode == 1) {
                    menu->nextMsg = 9;
                } else {
                    menu->nextMsg = 8;
                }
            }
        }
        break;
    case 7:
        r = func_ov021_021ce5ac(menu);
        if (r == 1) {
            func_0203d1e8(func_020399a8(), 0);
            menu->seq = 13;
        } else if (r == 2) {
            menu->seq = 5;
        }
        break;
    case 9:
        if (func_ov021_021cf2f0(menu->sendToAll) != 1) {
            break;
        }
        func_ov021_021ce6f0(menu, func_0203d1a4(func_020399a8()));
        menu->checkMask = menu->memberList.mask;
        func_0203d7bc(1);
        menu->seq = 10;
        break;
    case 10:
        mask = 1 << func_0203d1a4(func_020399a8());
        for (idx = 0; idx < 4; idx++) {
            if (idx != func_0203d1a4(func_020399a8()) && (menu->checkMask & 1 << idx)) {
                if (func_0203d1c0(idx) == TRUE) {
                    if (menu->readyMask & 1 << idx) {
                        mask |= 1 << idx;
                    }
                } else {
                    menu->seq = 11;
                    break;
                }
            }
        }
        if (menu->checkMask == mask) {
            if (MATH_CountPopulation(menu->readyMask) < menu->minCount) {
                menu->seq = 11;
            } else {
                menu->seq = 12;
            }
        }
        break;
    case 11:
        if (func_ov021_021cf3b0(menu->sendToAll) != 1) {
            break;
        }
        menu->checkMask = menu->memberList.mask;
        menu->result = 2;
        menu->seq = 14;
        break;
    case 12:
        if (func_ov021_021cf378(menu->sendToAll) == 1) {
            menu->result = 1;
            menu->seq = 8;
            menu->readyCount = MATH_CountPopulation(menu->readyMask);
            func_02039c68(func_020399a8(), 1, 0x27);
        }
        break;
    case 8:
        if (menu->readyCount != func_020392b8() || menu->minCount > func_020392b8()) {
            if (func_ov021_021cf3b0(menu->sendToAll) != 1) {
                break;
            }
            menu->checkMask = menu->memberList.mask;
            menu->result = 2;
            menu->seq = 14;
        } else if (func_02039cb4(func_020399a8(), 1, 0x27) == 1) {
            menu->result = 1;
            menu->seq = 0x13;
            {
                GFLNetInitData *net = func_0203d798();
                void *w = func_0203d650();
                CommEntryBeacon *beacon = (*net->unk18)(w);
                beacon->gameId = 0;
            }
        }
        break;
    case 13:
        if (func_ov021_021cf3b0(menu->sendToAll) != 1) {
            break;
        }
        menu->checkMask = menu->memberList.mask;
        menu->result = 2;
        menu->seq = 0x10;
        break;
    case 14:
        func_ov021_021ce9e4(menu, data_ov021_021db498[menu->mode].error);
        menu->seq = 15;
        break;
    case 15:
        if (func_ov021_02190ef0(menu->msgWin) != 1) {
            break;
        }
        menu->seq = 0x10;
        break;
    case 16:
        if (func_ov021_021cf12c(menu->checkMask) != 0) {
            break;
        }
        menu->seq = 0x13;
        break;
    case 17:
        func_ov021_021ce9e4(menu, 0x14);
        menu->seq = 0x12;
        break;
    case 18:
        if (func_ov021_02190ef0(menu->msgWin) == 1) {
            menu->result = 2;
            menu->seq = 0x13;
        }
        break;
    case 19:
        func_ov021_021ce81c(menu);
        return 1;
    default:
        break;
    }
    if (func_0200fb24()) {
        menu->result = 4;
        return 1;
    }
    return 0;
}


u32 func_ov021_021cdab8(CommEntryMenu *menu) {
    u32 r;
    switch (menu->seq) {
    case 0:
        func_ov021_021ce9e4(menu, data_ov021_021db498[menu->mode].chooseLeader);
        menu->seq++;
        break;
    case 1:
        if (func_ov021_02190ef0(menu->msgWin) == 1) {
            menu->seq++;
        }
        break;
    case 2:
        func_ov021_021ceb64(menu);
        func_ov021_021ce81c(menu);
        menu->list.selected = NULL;
        func_ov021_021ceb20(menu);
        func_0203d010();
        menu->seq++;
        break;
    case 3:
        r = func_ov021_021ceb84(menu);
        switch (r) {
        case 1:
            func_ov021_021ceb64(menu);
            if (menu->started == 1) {
                menu->seq = 6;
            } else if (menu->cancelled == 1) {
                menu->seq = 8;
            } else {
                copyVarForText(menu->wordSet, 0, &menu->list.selected->info);
                func_ov021_021ce9e4(menu, 0x21);
                menu->seq = 4;
            }
            break;
        case 2:
            func_ov021_021ceb64(menu);
            menu->seq = 10;
            break;
        case 3:
            func_ov021_021ceb64(menu);
            menu->seq = 14;
            break;
        }
        break;
    case 4:
        if (func_ov021_02190ef0(menu->msgWin) == 1) {
            func_ov021_021ce9e4(menu, 0x22);
            func_ov021_021ce7ac(menu);
            menu->seq++;
        }
        break;
    case 5:
        if (func_ov021_021cf068(menu) == 2) {
            func_ov021_021ce81c(menu);
            func_ov021_021cf0f4(menu);
            menu->seq = 14;
        } else {
            if (menu->sendCheckReply == 1 && func_ov021_021cf32c(menu->sendToAll) == 1) {
                menu->sendCheckReply = 0;
            }
            func_ov021_021ce9a0(menu);
            if (menu->started == 1) {
                func_ov021_021ce81c(menu);
                func_ov021_021cf0f4(menu);
                menu->seq = 6;
            } else if (menu->cancelled == 1) {
                func_ov021_021ce81c(menu);
                func_ov021_021cf0f4(menu);
                menu->seq = 8;
            }
        }
        break;
    case 6:
        if (menu->mode == 0) {
            func_ov021_021ce9e4(menu, 0x1e);
        }
        menu->seq = 7;
        func_02039c68(func_020399a8(), 1, 0x27);
        break;
    case 7:
        if (menu->mode == 0 && func_ov021_02190ef0(menu->msgWin) != 1) {
            break;
        }
        if (func_02039cb4(func_020399a8(), 1, 0x27) == 1) {
            menu->result = 1;
            menu->seq = 15;
        } else if (menu->cancelled == 1) {
            menu->seq = 8;
        }
        break;
    case 8:
        func_ov021_021ce9e4(menu, data_ov021_021db498[menu->mode].error);
        menu->seq = 9;
        break;
    case 9:
        if (func_ov021_02190ef0(menu->msgWin) == 1) {
            menu->seq = 14;
        }
        break;
    case 10:
        menu->netWork = func_0203d650();
        MI_CpuCopy8(func_0203d798(), &menu->netInit, sizeof(GFLNetInitData));
        func_ov021_021cf17c();
        func_0203ceac(NULL);
        copyVarForText(menu->wordSet, 0, &menu->replyInfo);
        func_ov021_021ce9e4(menu, 0x1f);
        menu->seq = 11;
        break;
    case 11:
        if (func_0203cdbc() == 1 && func_ov021_02190ef0(menu->msgWin) == 1) {
            menu->seq = 12;
        }
        break;
    case 12:
        func_0203cbac(&menu->netInit, NULL, menu->netWork);
        menu->seq = 13;
        break;
    case 13:
        if (func_0203cd98() == 1) {
            func_ov021_021cf164(menu);
            menu->seq = 0;
        }
        break;
    case 14:
        menu->result = 2;
        menu->seq = 15;
        break;
    case 15:
        func_ov021_021ceb64(menu);
        func_ov021_021ce81c(menu);
        return 1;
    default:
        break;
    }
    if (func_0200fb24()) {
        func_ov021_021cf0f4(menu);
        func_ov021_021ceb64(menu);
        menu->result = 4;
        return 1;
    }
    return 0;
}

u32 func_ov021_021cdd5c(CommEntryMenu *menu) {
    switch (menu->seq) {
    case 0:
        func_ov021_021ce9e4(menu, 0x15);
        menu->seq++;
        break;
    case 1:
        if (func_ov021_021cf1ac(&menu->self, 1, menu->sendToAll) == 1) {
            menu->seq++;
        }
        break;
    case 2:
        func_ov021_021ce7ac(menu);
        menu->seq++;
        break;
    case 3:
        if (func_ov021_021cf068(menu) == 2) {
            func_ov021_021ce81c(menu);
            func_ov021_021cf0f4(menu);
            menu->seq = 10;
        } else {
            if (menu->sendCheckReply == 1 && func_ov021_021cf32c(menu->sendToAll) == 1) {
                menu->sendCheckReply = 0;
            }
            func_ov021_021ce9a0(menu);
            if (menu->started == 1) {
                func_ov021_021ce81c(menu);
                func_ov021_021cf0f4(menu);
                menu->seq = 4;
            } else if (menu->cancelled == 1) {
                func_ov021_021ce81c(menu);
                func_ov021_021cf0f4(menu);
                menu->seq = 6;
            }
        }
        break;
    case 4:
        if (menu->mode == 0) {
            func_ov021_021ce9e4(menu, 0x1e);
        }
        menu->seq = 5;
        func_02039c68(func_020399a8(), 1, 0x27);
        break;
    case 5:
        if (menu->mode == 0 && func_ov021_02190ef0(menu->msgWin) != 1) {
            break;
        }
        if (func_02039cb4(func_020399a8(), 1, 0x27) == 1) {
            menu->result = 1;
            menu->seq = 11;
        } else if (menu->cancelled == 1) {
            menu->seq = 6;
        }
        break;
    case 6:
        func_ov021_021ce9e4(menu, data_ov021_021db498[menu->mode].error);
        menu->seq = 7;
        break;
    case 7:
        if (func_ov021_02190ef0(menu->msgWin) == 1) {
            menu->result = 2;
            menu->seq = 11;
        }
        break;
    case 8:
        func_0203ceac(NULL);
        copyVarForText(menu->wordSet, 0, &menu->list.selected->info);
        func_ov021_021ce9e4(menu, 0x1f);
        menu->seq = 9;
        break;
    case 9:
        if (func_0203cdbc() == 1 && func_ov021_02190ef0(menu->msgWin) == 1) {
            menu->result = 3;
            menu->seq = 11;
        }
        break;
    case 10:
        menu->result = 2;
        menu->seq = 11;
        break;
    case 11:
        func_ov021_021ce81c(menu);
        return 1;
    }
    if (func_0200fb24()) {
        func_ov021_021cf0f4(menu);
        menu->result = 4;
        return 1;
    }
    return 0;
}

u32 func_ov021_021cdf18(CommEntryMenu *menu) {
    switch (menu->seq) {
    case 0:
        func_ov021_021ce9e4(menu, 0x15);
        menu->seq++;
        break;
    case 1:
        menu->reply = 0;
        if (func_ov021_021cf1ac(&menu->self, 0, menu->sendToAll) == 1) {
            menu->seq = 2;
        }
        break;
    case 2:
        if (func_ov021_021cf068(menu) == 2) {
            func_ov021_021ce81c(menu);
            func_ov021_021cf0f4(menu);
            func_ov021_021cf0f4(menu);
            menu->seq = 11;
        } else if (menu->reply == 1) {
            menu->seq = 3;
            func_ov021_021cf0f4(menu);
        } else if (menu->reply == 2) {
            menu->seq = 9;
            func_ov021_021cf0f4(menu);
        } else if (menu->started == 1) {
            menu->seq = 5;
            func_ov021_021cf0f4(menu);
        } else if (menu->cancelled == 1) {
            menu->seq = 11;
            func_ov021_021cf0f4(menu);
        }
        break;
    case 3:
        func_ov021_021ce7ac(menu);
        menu->seq++;
        break;
    case 4:
        if (func_ov021_021cf068(menu) == 2) {
            func_ov021_021ce81c(menu);
            func_ov021_021cf0f4(menu);
            menu->seq = 11;
        } else {
            if (menu->sendCheckReply == 1 && func_ov021_021cf32c(menu->sendToAll) == 1) {
                menu->sendCheckReply = 0;
            }
            func_ov021_021ce9a0(menu);
            if (menu->started == 1) {
                func_ov021_021ce81c(menu);
                func_ov021_021cf0f4(menu);
                menu->seq = 5;
            } else if (menu->cancelled == 1) {
                func_ov021_021ce81c(menu);
                func_ov021_021cf0f4(menu);
                menu->seq = 7;
            }
        }
        break;
    case 5:
        if (menu->mode == 0) {
            func_ov021_021ce9e4(menu, 0x1e);
        }
        menu->seq = 6;
        func_02039c68(func_020399a8(), 1, 0x27);
        break;
    case 6:
        if (menu->mode == 0 && func_ov021_02190ef0(menu->msgWin) != 1) {
            break;
        }
        if (func_02039cb4(func_020399a8(), 1, 0x27) == 1) {
            menu->result = 1;
            menu->seq = 12;
        } else if (menu->cancelled == 1) {
            menu->seq = 7;
        }
        break;
    case 7:
        func_ov021_021ce9e4(menu, data_ov021_021db498[menu->mode].error);
        menu->seq = 8;
        break;
    case 8:
        if (func_ov021_02190ef0(menu->msgWin) == 1) {
            menu->result = 2;
            menu->seq = 12;
        }
        break;
    case 9:
        func_0203ceac(NULL);
        copyVarForText(menu->wordSet, 0, &menu->replyInfo);
        func_ov021_021ce9e4(menu, 0x1f);
        menu->seq = 10;
        break;
    case 10:
        if (func_0203cdbc() == 1 && func_ov021_02190ef0(menu->msgWin) == 1) {
            menu->result = 3;
            menu->seq = 12;
        }
        break;
    case 11:
        menu->result = 2;
        menu->seq = 12;
        break;
    case 12:
        func_ov021_021ce81c(menu);
        return 1;
    }
    if (func_0200fb24()) {
        func_ov021_021cf0f4(menu);
        menu->result = 4;
        return 1;
    }
    return 0;
}




void func_ov021_021ce12c(CommEntryMenu *menu) {
    u8 i;
    for (i = 0; i < 4; i++) {
        u32 bit = 1 << i;
        if (menu->acceptMask & bit) {
            if (func_ov021_021cf220(i, menu->sendToAll, &menu->self) == 1) {
                menu->acceptMask ^= (u8)bit;
            }
        } else if (menu->refuseMask & bit) {
            if (func_ov021_021cf288(i, menu->sendToAll, &menu->self) == 1) {
                menu->refuseMask ^= (u8)bit;
            }
        }
    }
    if (menu->sendMemberList == 1 && menu->memberListSent == 0) {
        u8 mask = func_ov021_021ce1f4(menu);
        if (mask > 1) {
            if (func_ov021_021cf3fc(&menu->memberList, mask, menu->sendToAll) == 1) {
                menu->sendMemberList = 0;
                menu->memberListSent = 1;
            }
        }
    }
}




u8 func_ov021_021ce1f4(CommEntryMenu *menu) {
    u8 i;
    u8 mask = 0;
    for (i = 0; i < 4; i++) {
        if (menu->members[i].state != 0) {
            func_02008494(&menu->members[i].info, &menu->memberList.players[i]);
            mask |= 1 << i;
        }
    }
    menu->memberList.mask = mask;
    return (u8)(mask | 1);
}

void func_ov021_021ce24c(CommEntryMenu *menu) {
    u8 i;

    for (i = 0; i < 4; i++) {
        CommEntryMember *member = &menu->members[i];
        if (member->state != 0 && func_0203d1c0(i) == 0) {
            if (menu->onLeave != NULL) {
                menu->onLeave(menu->callbackWork, &menu->members[i].info, menu->members[i].mac);
            }
            func_ov021_021ce3d8(menu, i);
            func_ov021_021cea84(menu, i);
            func_ov021_021ceab0(menu);
        }
    }
}

void func_ov021_021ce2bc(CommEntryMenu *menu) {
    s32 i;
    s32 active = 0;

    for (i = 0; i < 4; i++) {
        CommEntryMember *member = &menu->members[i];
        if (member->state != 0) {
            func_ov021_021ce8b0(menu, &menu->members[i], i);
            active++;
        } else {
            func_ov021_021ce8f4(menu, &menu->members[i], i);
        }
    }
    if (active == 0) {
        func_ov021_021ce934(menu, &menu->self, 0);
        active++;
    }
    if (menu->shownCount != active && func_ov021_021ce84c(menu, active) == 1) {
        menu->shownCount = active;
        if (menu->mode == 1 && active == 1) {
            menu->showWaitMsg = 1;
        }
    }
    if (menu->showWaitMsg == 1 && func_ov021_021ce78c(menu) == 1 && menu->yesNo.window == NULL) {
        if (active == 1) {
            func_ov021_021ce9e4(menu, 8);
        }
        menu->showWaitMsg = 0;
    }
}




void func_ov021_021ce36c(CommEntryMenu *menu) {
    CommEntryMember *member = menu->members;
    u8 i;
    for (i = 0; i < 4; i++) {
        switch (member->state) {
        case 1:
            if (member->autoAccept == 1) {
                member->state = 4;
                menu->acceptMask |= (u8)(1 << i);
            } else {
                member->state = 2;
            }
            break;
        case 2:
            if (func_ov021_021ce3e8(menu, member) == 1) {
                member->state = 3;
            }
            break;
        }
        member++;
    }
}


void func_ov021_021ce3d8(CommEntryMenu *menu, s32 param_2) {
    menu->members[param_2].state = 0;
}

u32 func_ov021_021ce3e8(CommEntryMenu *menu, CommEntryMember *member) {
    if (menu->asking == 0) {
        menu->asked = *member;
        menu->asking = 1;
        return 1;
    }
    return 0;
}

void func_ov021_021ce41c(CommEntryMenu *menu) {
    CommEntryMember *asked = &menu->asked;
    CommEntryYesNo *yesNo = &menu->yesNo;
    u8 i;
    u32 r;
    u8 state;
    if (menu->asking != 0 && func_ov021_02190ef0(menu->msgWin)) {
        switch (yesNo->seq) {
        case 0:
            copyVarForText(menu->wordSet, 0, &asked->info);
            func_ov021_021ce9e4(menu, 10);
            yesNo->seq++;
            return;
        case 1:
            yesNo->window = func_ov021_02190be4(Field_GetMsgBGSys(menu->field), 0);
            yesNo->seq++;
            return;
        case 2:
            r = func_ov021_02190c78(yesNo->window);
            if (r != 2) {
                state = 4;
                if (r != 0) {
                    state = 5;
                }
                for (i = 0; i < 4; i++) {
                    if (menu->members[i].state == 3) {
                        menu->members[i].state = state;
                        if (state == 4) {
                            menu->acceptMask |= (u8)(1 << i);
                        } else {
                            menu->refuseMask |= (u8)(1 << i);
                        }
                        break;
                    }
                }
                func_ov021_021909bc(yesNo->window);
                yesNo->window = NULL;
                yesNo->seq++;
                return;
            }
            break;
        case 3:
            menu->asking = 0;
            yesNo->seq = 0;
            func_ov021_021ce36c(menu);
            if (menu->asking == 0) {
                if (func_ov021_021ce73c(menu) > 1 && menu->mode == 1) {
                    menu->nextMsg = 9;
                    return;
                }
                menu->nextMsg = 8;
            }
            break;
        }
    }
}


u32 func_ov021_021ce52c(CommEntryMenu *menu) {
    CommEntryYesNo *yesNo = &menu->yesNo;
    u32 r;
    if (func_ov021_02190ef0(menu->msgWin) == 0) {
        return 0;
    }
    switch (yesNo->seq) {
    case 0:
        func_ov021_021ce9e4(menu, 0xb);
        yesNo->seq++;
        break;
    case 1:
        yesNo->window = func_ov021_02190be4(Field_GetMsgBGSys(menu->field), 0);
        yesNo->seq++;
        break;
    case 2:
        if (func_ov021_021cea50(menu) == 0) {
            r = 1;
        } else {
            r = func_ov021_02190c78(yesNo->window);
            if (r == 2) {
                break;
            }
        }
        func_ov021_021909bc(yesNo->window);
        yesNo->window = NULL;
        yesNo->seq = 0;
        if (r == 0) {
            return 1;
        }
        return 2;
    }
    return 0;
}

u32 func_ov021_021ce5ac(CommEntryMenu *menu) {
    CommEntryYesNo *yesNo = &menu->yesNo;
    u32 r;
    if (func_ov021_02190ef0(menu->msgWin) == 0) {
        return 0;
    }
    switch (yesNo->seq) {
    case 0:
        func_ov021_021ce9e4(menu, data_ov021_021db498[menu->mode].cancelAsk);
        yesNo->seq++;
        break;
    case 1:
        yesNo->window = func_ov021_02190be4(Field_GetMsgBGSys(menu->field), 0);
        yesNo->seq++;
        break;
    case 2:
        r = func_ov021_02190c78(yesNo->window);
        if (r != 2) {
            func_ov021_021909bc(yesNo->window);
            yesNo->window = NULL;
            if (r == 1) {
                yesNo->result = 2;
                yesNo->seq = 0x64;
            } else if (func_ov021_021ce73c(menu) <= 1) {
                func_ov021_021ce9e4(menu, data_ov021_021db498[menu->mode].cancelled);
                yesNo->seq = 5;
            } else {
                func_ov021_021ce9e4(menu, 0xf);
                yesNo->seq++;
            }
        }
        break;
    case 3:
        yesNo->window = func_ov021_02190be4(Field_GetMsgBGSys(menu->field), 0);
        yesNo->seq++;
        break;
    case 4:
        r = func_ov021_02190c78(yesNo->window);
        if (r != 2) {
            func_ov021_021909bc(yesNo->window);
            yesNo->window = NULL;
            if (r == 0) {
                func_ov021_021ce9e4(menu, data_ov021_021db498[menu->mode].cancelled);
                yesNo->seq++;
            } else {
                yesNo->result = 2;
                yesNo->seq = 0x64;
            }
        }
        break;
    case 5:
        yesNo->seq = 0xff;
        yesNo->result = 1;
        break;
    case 100:
        if (func_ov021_021ce73c(menu) > 1 && menu->mode == 1) {
            menu->nextMsg = 9;
        } else {
            menu->nextMsg = 8;
        }
        yesNo->seq = 0xff;
        break;
    case 0xff:
        if (yesNo->window) {
            func_ov021_021909bc(yesNo->window);
            yesNo->window = NULL;
        }
        yesNo->seq = 0;
        return yesNo->result;
    }
    return 0;
}




void func_ov021_021ce6f0(CommEntryMenu *menu, u32 param_2) {
    menu->readyMask |= (u8)(1 << param_2);
}


void func_ov021_021ce70c(CommEntryMenu *menu) {
    menu->started = 1;
}

void func_ov021_021ce718(CommEntryMenu *menu) {
    menu->cancelled = 1;
}

void func_ov021_021ce724(CommEntryMenu *menu) {
    menu->checked = 1;
    menu->sendCheckReply = 1;
    func_0203d7bc(1);
}

s32 func_ov021_021ce73c(CommEntryMenu *menu) {
    s32 i;
    s32 j;

    j = 0;
    i = 0;
    do {
        if (menu->members[i].state == 4) {
            j = j + 1;
        }
        i = i + 1;
    } while (i < 4);
    return j;
}

u32 func_ov021_021ce760(CommEntryMenu *menu) {
    u32 mask = 0;
    s32 i;
    for (i = 0; i < 4; i++) {
        if (menu->members[i].state == 4) {
            mask |= 1 << i;
        }
    }
    return mask;
}


u32 func_ov021_021ce78c(CommEntryMenu *menu) {
    BOOL BVar1;

    if ((menu->asking == 0) &&
          (BVar1 = func_ov021_02190ef0(menu->msgWin), BVar1 == 1)) {
        return 1;
    }
    return 0;
}

void func_ov021_021ce7ac(CommEntryMenu *menu) {
    FieldMsgBG *msgBGSys;
    MsgBGSysWin *ptr;
    s16 val;

    msgBGSys = Field_GetMsgBGSys(menu->field);
    val = 0x10;
    if (menu->kind != 0) {
        val = 0xe;
    }
    menu->membersWin = FieldMsgBG_CreateMoneyWin(msgBGSys, menu->msgData, 2, 6, 0x12, val - 6);
    ptr = FieldMsgBG_CreateMoneyWin(msgBGSys, menu->msgData, 2, 1, 0x1c, 2);
    menu->titleWin = ptr;
    func_ov021_02190740(ptr, 0, 0, data_ov021_021db498[menu->mode].title);
}

void func_ov021_021ce81c(CommEntryMenu *menu) {
    FieldMsgBG *msgBGSys;

    msgBGSys = Field_GetMsgBGSys(menu->field);
    if (menu->titleWin != NULL) {
        func_ov021_02190728(menu->titleWin);
        menu->titleWin = NULL;
    }
    if (menu->membersWin != NULL) {
        func_ov021_02190728(menu->membersWin);
        menu->membersWin = NULL;
    }
    func_ov021_02190490(msgBGSys);
}

u32 func_ov021_021ce84c(CommEntryMenu *menu, s32 param_2) {
    BOOL BVar1;

    if ((menu->membersWin != NULL) &&
          (BVar1 = func_ov021_0219077c(menu->membersWin), BVar1 == 1)) {
        func_ov021_02190794(menu->membersWin, 0, 0x40, 0x90, 0x50);
        WordSetNumber(menu->wordSet, 0, (u32)menu->maxCount - param_2, 1, 0, 1);
        GFL_WordSetFormatStrbuf
                            (menu->wordSet, menu->countBuf, menu->countFmt)
        ;
        func_ov021_02190758(menu->membersWin, 0, 0x40, menu->countBuf);
        return 1;
    }
    return 0;
}

// The MAC shown in a row of the member list (pokebw2's CommEntryMenu_GetShownMac)
static inline u8 *CommEntryMenu_GetShownMac(CommEntryMenu *menu, int netId) {
    return menu->shownMacs[netId];
}

void func_ov021_021ce8b0(CommEntryMenu *menu, CommEntryMember *member, s32 param_3) {
    u8 (*shownMacs)[6] = menu->shownMacs;
    if (GFL_STD_MemCmp(member->mac, shownMacs[param_3], 6) != 0) {
        func_ov021_021ce934(menu, &member->info, (u8)param_3);
        MI_CpuCopy8(member->mac, shownMacs[param_3], 6);
    }
}

void func_ov021_021ce8f4(CommEntryMenu *menu, CommEntryMember *member, u32 param_3) {
    if (GFL_STD_MemCmp(CommEntryMenu_GetShownMac(menu, param_3), data_ov021_021db470, 6) != 0) {
        func_ov021_021ce984(menu, param_3);
        MI_CpuCopy8(data_ov021_021db470, menu->shownMacs[param_3], 6);
    }
}



void func_ov021_021ce934(CommEntryMenu *menu, PlayerInfo *param_2, s16 param_3) {
    if (menu->membersWin != NULL) {
        copyVarForText(menu->wordSet, 0, param_2);
        WordSetNumber(menu->wordSet, 1, (u32)getTrainerID(param_2), 5, 2, 1);
        GFL_WordSetFormatStrbuf
                            (menu->wordSet, menu->memberBuf, menu->memberFmt)
        ;
        func_ov021_02190758(menu->membersWin, 0, param_3 << 4, menu->memberBuf);
    }
}

void func_ov021_021ce984(CommEntryMenu *menu, s32 param_2) {
    MsgBGSysWin *window = menu->membersWin;
    if (window != NULL) {
        func_ov021_02190794(window, 0, param_2 << 4, 0x90, 0x10);
    }
}

void func_ov021_021ce9a0(CommEntryMenu *menu) {
    int flag;
    s32 i;
    if (menu->memberListChanged != 0) {
        menu->memberListChanged = 0;
        for (i = 0; i < 4; i++) {
            func_ov021_021ce984(menu, i);
            flag = 1;
            if ((flag << i) & menu->memberList.mask) {
                func_ov021_021ce934(menu, &menu->memberList.players[i], (u8)i);
            }
        }
    }
}



void func_ov021_021ce9e4(CommEntryMenu *menu, u32 param_2) {
    StrBuf *src;

    if ((param_2 != 8) || (menu->lastMsg != 8)) {
        menu->lastMsg = (u16)param_2;
        src = GFL_MsgDataLoadStrbufNew(menu->msgData, param_2);
        GFL_WordSetFormatStrbuf(menu->wordSet, menu->msgBuf, src);
        GFL_StrBufFree(src);
        func_ov021_02190f7c(menu->msgWin);
        func_ov021_02190eb8(menu->msgWin, 0, 0, menu->msgBuf);
    }
}

void func_ov021_021cea28(CommEntryMenu *menu) {
    u32 i;

    i = 0;
    do {
        menu->savedStates[i] = menu->members[i].state;
        i = i + 1 & 0xff;
    } while (i < 4);
}

u32 func_ov021_021cea50(CommEntryMenu *menu) {
    u32 i;

    i = 0;
    do {
        if (menu->savedStates[i] != menu->members[i].state) {
            return 0;
        }
        i = i + 1 & 0xff;
    } while (i < 4);
    return 1;
}

void func_ov021_021cea84(CommEntryMenu *menu, u32 param_2) {
    u16 mask = (u8)(0xff ^ (1 << param_2));
    menu->acceptMask &= (u8)(0xff ^ (1 << param_2));
    menu->refuseMask = menu->refuseMask & mask;
}

void func_ov021_021ceab0(CommEntryMenu *menu) {
    if (menu->sendMemberList == 1 || menu->memberListSent == 1) {
        menu->memberListPending = 1;
        return;
    }
    menu->sendMemberList = 1;
    menu->memberListPending = 0;
}




void func_ov021_021ceadc(CommEntryMenu *menu) {
    menu->memberListSent = 0;
    menu->sendMemberList = menu->memberListPending;
    menu->memberListPending = 0;
}

CommEntryMemberList *func_ov021_021ceaf8(CommEntryMenu *menu) {
    return &menu->memberList;
}

void func_ov021_021ceb04(CommEntryMenu *menu, const CommEntryMemberList *list) {
    MI_CpuCopy8(list, &menu->memberList, sizeof(CommEntryMemberList));
    menu->memberListChanged = 1;
}

void func_ov021_021ceb20(CommEntryMenu *menu) {
    FieldMsgBG *msgbg = Field_GetMsgBGSys(menu->field);
    menu->list.options = InitListMenuOptionHeap(0x10, menu->heapId);
    func_ov021_021cef1c(menu);
    menu->list.menu = func_ov021_0219097c(msgbg, (ListMenuRequest *)&data_ov021_021db478, menu->list.options);
    menu->list.seq = 0;
}


void func_ov021_021ceb64(CommEntryMenu *menu) {
    if (menu->list.menu != NULL) {
        func_ov021_021909bc(menu->list.menu);
        menu->list.menu = NULL;
        menu->list.optionsBuilt = 0;
    }
}

u32 func_ov021_021ceb84(CommEntryMenu *menu) {
    CommEntryYesNo *yesNo = &menu->yesNo;
    CommEntryLeaderList *list = &menu->list;
    u32 r;
    switch (list->seq) {
    case 0:
        if (list->selected == NULL) {
            func_ov021_021cee00(menu);
            func_ov021_021cee48(menu);
            if (func_ov021_021ceda0(menu) == 0) {
                list->result = 3;
                list->seq = 3;
                break;
            }
            if (menu->list.changed == 1) {
                func_ov021_021cef1c(menu);
                func_ov021_02190a14(menu->list.menu);
                menu->list.changed = 0;
            }
            break;
        }
        if (func_020392b8() > 1) {
            if (func_02039acc() == 1) {
                list->seq++;
            }
            break;
        }
        if (GCTX_HIDGetPressedKeys() & 2) {
            list->prevSeq = list->seq;
            list->seq = 4;
        }
        list->timer++;
        if (list->timer >= 300) {
            list->seq = 6;
            func_ov021_021ce9e4(menu, 0x24);
        }
        break;
    case 1:
        if (func_020399b8(func_020399a8()) == 1) {
            menu->reply = 0;
            if (func_ov021_021cf1ac(&menu->self, 0, menu->sendToAll) == 1) {
                list->seq = 2;
            }
        } else if (GCTX_HIDGetPressedKeys() & 2) {
            list->prevSeq = list->seq;
            list->seq = 4;
        }
        if (menu->cancelled == 1) {
            list->result = 1;
            list->seq = 3;
        }
        break;
    case 2:
        if (menu->reply == 1) {
            list->result = 1;
            list->seq = 3;
        } else if (menu->reply == 2) {
            list->result = 2;
            list->seq = 3;
        } else if (GCTX_HIDGetPressedKeys() & 2) {
            list->prevSeq = list->seq;
            list->seq = 4;
        }
        if (menu->cancelled == 1) {
            list->result = 1;
            list->seq = 3;
        }
        break;
    case 3:
        return list->result;
    case 4:
        func_ov021_021ce9e4(menu, 0x23);
        yesNo->window = func_ov021_02190be4(Field_GetMsgBGSys(menu->field), 0);
        list->seq = 5;
        break;
    case 5:
        r = func_ov021_02190c78(yesNo->window);
        if (func_ov021_0219049c(Field_GetMsgBGSys(menu->field)) == 1 && menu->checked == 1) {
            r = 1;
        }
        if (r == 2 && func_ov021_0219049c(Field_GetMsgBGSys(menu->field)) == 1) {
            if (menu->started == 1 || menu->cancelled == 1) {
                func_ov021_021909bc(yesNo->window);
                yesNo->window = NULL;
                list->result = 1;
                list->seq = 3;
            }
            break;
        }
        func_ov021_021909bc(yesNo->window);
        yesNo->window = NULL;
        if (r == 1) {
            list->seq = list->prevSeq;
            func_ov021_021ce9e4(menu, 0x20);
            if (menu->list.menu != NULL) {
                func_ov021_02190b1c(menu->list.menu);
            }
            break;
        }
        list->result = 3;
        list->seq = 3;
        break;
    case 6:
        if (func_ov021_02190ef0(menu->msgWin) == 1) {
            list->result = 3;
            list->seq = 3;
        }
        break;
    }
    return 0;
}


u32 func_ov021_021ceda0(CommEntryMenu *menu) {
    CommEntryLeaderList *list = &menu->list;
    intptr_t sel;
    sel = ListMenuUI_Update(list->menu);
    switch (sel) {
    case -2:
        return 0;
    case -1:
        break;
    default:
        if (list->leaders[sel].valid == 1) {
            CommEntryLeader *entry = &list->leaders[sel];
            list->selected = entry;
            func_0203d000(entry->mac);
            copyVarForText(menu->wordSet, 0, &list->selected->info);
            func_ov021_021ce9e4(menu, 0x20);
            list->timer = 0;
        }
        break;
    }
    return 1;
}


void func_ov021_021cee00(CommEntryMenu *menu) {
    CommEntryLeaderList *list = &menu->list;
    s32 i;
    for (i = 0; i < 0x10; i++) {
        if (list->leaders[i].valid == TRUE && --list->leaders[i].timer <= 0) {
            func_ov021_021cef08(menu, i);
            list->changed = 1;
        }
    }
}


void func_ov021_021cee48(CommEntryMenu *menu) {
    s32 i;
    CommEntryBeacon *beacon;
    for (i = 0; i < 0x10; i++) {
        beacon = func_0203cf10(i);
        if (beacon != NULL && beacon->gameId == COMM_ENTRY_BEACON_GAME_ID) {
            func_ov021_021cee78(menu, beacon->mac, &beacon->info);
        }
    }
}


void func_ov021_021cee78(CommEntryMenu *menu, const u8 *param_2, PlayerInfo *param_3) {
    CommEntryLeaderList *list = &menu->list;
    s32 i;
    for (i = 0; i < 0x10; i++) {
        if (list->leaders[i].valid == 1 && GFL_STD_MemCmp(list->leaders[i].mac, param_2, 6) == 0) {
            list->leaders[i].timer = 0x96;
            return;
        }
    }
    for (i = 0; i < 0x10; i++) {
        if (list->leaders[i].valid == 0) {
            MI_CpuCopy8(param_2, list->leaders[i].mac, 6);
            func_02008494(param_3, &list->leaders[i].info);
            list->leaders[i].timer = 0x96;
            list->leaders[i].valid = 1;
            list->changed = 1;
            return;
        }
    }
}


void func_ov021_021cef08(CommEntryMenu *menu, s32 param_2) {
    menu->list.leaders[param_2].valid = 0;
}

void func_ov021_021cef1c(CommEntryMenu *menu) {
    u16 eos;
    StrBuf *dest;
    StrBuf *strbuf;
    StrBuf *strbuf2;
    StrBuf *small;
    s32 i;
    eos = GFL_StrBufGetTerminator();
    if (menu->list.optionsBuilt == 1) {
        func_ov021_02190a6c(menu->list.options);
    }
    strbuf = GFL_StrBufCreate(0x40, menu->heapId);
    strbuf2 = GFL_StrBufCreate(0x40, menu->heapId);
    dest = GFL_StrBufCreate(0x40, menu->heapId);
    small = GFL_StrBufCreate(4, menu->heapId);
    GFL_MsgDataLoadStrbuf(menu->msgData, 0x1a, strbuf);
    GFL_MsgDataLoadStrbuf(menu->msgData, 0x1b, strbuf2);
    for (i = 0; i < 0x10; i++) {
        WordSetNumber(menu->wordSet, 0, i + 1, 2, 2, 1);
        if (menu->list.leaders[i].valid == 1) {
            copyVarForText(menu->wordSet, 1, &menu->list.leaders[i].info);
            WordSetNumber(menu->wordSet, 2, getTrainerID(&menu->list.leaders[i].info), 5, 2, 1);
            GFL_WordSetFormatStrbuf(menu->wordSet, dest, strbuf2);
        } else {
            GFL_StrBufLoadString(small, &eos);
            func_0201ee0c(menu->wordSet, 1, small, 0, 1, 2);
            func_0201ee0c(menu->wordSet, 2, small, 0, 1, 2);
            GFL_WordSetFormatStrbuf(menu->wordSet, dest, strbuf);
        }
        AppendListMenuOption(menu->list.options, dest, i, menu->heapId);
    }
    GFL_StrBufFree(strbuf2);
    GFL_StrBufFree(strbuf);
    GFL_StrBufFree(dest);
    GFL_StrBufFree(small);
    menu->list.optionsBuilt = 1;
}


u32 func_ov021_021cf068(CommEntryMenu *menu) {
    CommEntryYesNo *yesNo = &menu->yesNo;
    u32 r;
    switch (yesNo->seq) {
    case 0:
        if (GCTX_HIDGetPressedKeys() & 2) {
            yesNo->window = func_ov021_02190be4(Field_GetMsgBGSys(menu->field), 0);
            func_ov021_021ce9e4(menu, 0x23);
            yesNo->seq = 1;
        }
        break;
    case 1:
        r = func_ov021_02190c78(yesNo->window);
        if (func_ov021_0219049c(Field_GetMsgBGSys(menu->field)) == 1 && menu->checked == 1) {
            r = 1;
        }
        if (r != 2) {
            func_ov021_021909bc(yesNo->window);
            yesNo->window = NULL;
            if (r == 1) {
                yesNo->seq = 0;
                func_ov021_021ce9e4(menu, 0x22);
            } else {
                return 2;
            }
        }
        return 1;
    }
    return 0;
}


void func_ov021_021cf0f4(CommEntryMenu *menu) {
    CommEntryYesNo *yesNo = &menu->yesNo;
    ListMenuUI *window = yesNo->window;
    if (window != NULL) {
        func_ov021_021909bc(window);
        yesNo->window = NULL;
    }
    yesNo->seq = 0;
}


void func_ov021_021cf110(CommEntryMenu *menu, u32 param_2, PlayerInfo *param_3) {
    menu->reply = param_2;
    func_02008494(param_3, &menu->replyInfo);
}

u32 func_ov021_021cf128(void) {
    return sizeof(CommEntryMemberList);
}

u32 func_ov021_021cf12c(u32 param_1) {
    u8 i;
    for (i = 0; i < 4; i++) {
        NetHandle *handle = func_020399a8();
        if (i != func_0203d1a4(handle) && (param_1 & (1 << i)) && func_0203d1c0(i) == 1) {
            return 1;
        }
    }
    return 0;
}


void func_ov021_021cf164(CommEntryMenu *menu) {
    func_0203a574(0x2700, data_ov021_021db4bc, NELEMS(data_ov021_021db4bc), menu);
}

void func_ov021_021cf17c(void) {
    func_0203a5e4(0x2700);
}

void *func_ov021_021cf188(int netId, void *work, int size) {
    return func_ov021_021ceaf8(work);
}

void func_ov021_021cf194(int netId, int size, const void *data, void *work, NetHandle *handle) {
    CommEntryPacket *info = (CommEntryPacket *)data;

    func_ov021_021cd624(work, netId, &info->info, info->autoAccept, info->mac);
}

BOOL func_ov021_021cf1ac(PlayerInfo *param_1, u8 param_2, s32 param_3) {
    CommEntryPacket info;
    MI_CpuFill8(&info, 0, sizeof(CommEntryPacket));
    func_02008494(param_1, &info.info);
    OS_GetMacAddress(info.mac);
    info.autoAccept = param_2;
    if (param_3 == 1) {
        return func_0203d3f8(func_020399a8(), 0xff, 0x2700, sizeof(CommEntryPacket), &info, 0, 0, 0);
    }
    return func_0203d3f8(func_020399a8(), 0, 0x2700, sizeof(CommEntryPacket), &info, 0, 0, 0);
}



void func_ov021_021cf214(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_ov021_021cf110(work, 1, (PlayerInfo *)data);
}

BOOL func_ov021_021cf220(u8 param_1, s32 param_2, PlayerInfo *param_3) {
    NetHandle *pNVar1;
    u32 size;

    if (param_2 == 1) {
        pNVar1 = func_0203996c(0xff);
        size = func_02008468();
        return func_0203d3f8(pNVar1, param_1, 0x2701, size, param_3, 0, 0, 0);
    }
    pNVar1 = func_020399a8();
    size = func_02008468();
    return func_0203d3f8(pNVar1, param_1, 0x2701, size, param_3, 0, 0, 0);
}

void func_ov021_021cf27c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_ov021_021cf110(work, 2, (PlayerInfo *)data);
}

BOOL func_ov021_021cf288(u8 param_1, s32 param_2, PlayerInfo *param_3) {
    NetHandle *pNVar1;
    u32 size;

    if (param_2 == 1) {
        pNVar1 = func_0203996c(0xff);
        size = func_02008468();
        return func_0203d3f8(pNVar1, param_1, 0x2702, size, param_3, 0, 0, 0);
    }
    pNVar1 = func_020399a8();
    size = func_02008468();
    return func_0203d3f8(pNVar1, param_1, 0x2702, size, param_3, 0, 0, 0);
}

void func_ov021_021cf2e4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_ov021_021ce724(work);
}


BOOL func_ov021_021cf2f0(s32 param_1) {
    NetHandle *pNVar1;

    if (param_1 == 1) {
        pNVar1 = func_0203996c(0xff);
        return func_0203d3a8(pNVar1, 0x2703, 0, (void *)0x0);
    }
    pNVar1 = func_020399a8();
    return func_0203d3a8(pNVar1, 0x2703, 0, (void *)0x0);
}

void func_ov021_021cf31c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_ov021_021ce6f0(work, (u8)netId);
}

BOOL func_ov021_021cf32c(s32 param_1) {
    NetHandle *pNVar1;

    if (param_1 == 1) {
        pNVar1 = func_020399a8();
        return func_0203d3f8(pNVar1, 0xff, 0x2704, 0, (void *)0x0, 0, 0, 0);
    }
    pNVar1 = func_020399a8();
    return func_0203d3f8(pNVar1, 0, 0x2704, 0, (void *)0x0, 0, 0, 0);
}

void func_ov021_021cf36c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_ov021_021ce70c(work);
}


BOOL func_ov021_021cf378(s32 param_1) {
    NetHandle *pNVar1;

    if (param_1 == 1) {
        pNVar1 = func_0203996c(0xff);
        return func_0203d3a8(pNVar1, 0x2705, 0, (void *)0x0);
    }
    pNVar1 = func_020399a8();
    return func_0203d3a8(pNVar1, 0x2705, 0, (void *)0x0);
}

void func_ov021_021cf3a4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_ov021_021ce718(work);
}


BOOL func_ov021_021cf3b0(s32 param_1) {
    NetHandle *pNVar1;

    if (param_1 == 1) {
        pNVar1 = func_0203996c(0xff);
        return func_0203d3a8(pNVar1, 0x2706, 0, (void *)0x0);
    }
    pNVar1 = func_020399a8();
    return func_0203d3a8(pNVar1, 0x2706, 0, (void *)0x0);
}

void func_ov021_021cf3dc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    BOOL BVar1;

    BVar1 = func_0203d378();
    if (BVar1 == 1) {
        func_ov021_021ceadc(work);
        return;
    }
    func_ov021_021ceb04(work, data);
}

BOOL func_ov021_021cf3fc(CommEntryMemberList *param_1, u8 param_2, s32 param_3) {
    NetHandle *pNVar1;
    u32 size;

    if (param_3 == 1) {
        pNVar1 = func_0203996c(0xff);
        size = func_ov021_021cf128();
        return func_0203d4d0(pNVar1, param_2, 0x2707, size, param_1, 0, 0, 1);
    }
    pNVar1 = func_020399a8();
    size = func_ov021_021cf128();
    return func_0203d4d0(pNVar1, param_2, 0x2707, size, param_1, 0, 0, 1);
}

// The commands of the entry (0x2700 on)
static const NetCommand data_ov021_021db4bc[8] = {
    { func_ov021_021cf194, NULL },
    { func_ov021_021cf214, NULL },
    { func_ov021_021cf27c, NULL },
    { func_ov021_021cf2e4, NULL },
    { func_ov021_021cf31c, NULL },
    { func_ov021_021cf36c, NULL },
    { func_ov021_021cf3a4, NULL },
    { func_ov021_021cf3dc, func_ov021_021cf188 },
};
