// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov028/union_receive.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Union Room's net commands from 0x1400 that the machines send each other, with the senders for them. The file's
// name is a guess: the ROM only shows that it is a file of its own, between union_main.c and union_subproc.c, by the
// assert file name ("") that its asserts share
#include "field/union_main.h"
#include "types.h"
#include "app/p_status.h"
#include "app/pokelist.h"
#include "app/pokemon_trade.h"
#include "constants/zones.h"
#include "demo/shinka_demo.h"
#include "field/colosseum.h"
#include "field/event_colosseum_battle.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/field_controller.h"
#include "field/field_event.h"
#include "field/field_sound.h"
#include "field/union_app.h"
#include "field/union_comm.h"
#include "field/union_msg.h"
#include "field/union_oneself.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_sync.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "nitro/os.h"

// The net commands, from UNION_NET_CMD_BASE
#define UNION_NET_CMD_BASE 0x1400
enum {
    UNION_NET_CMD_UNK0 = UNION_NET_CMD_BASE,
    UNION_NET_CMD_UNK1,
    UNION_NET_CMD_UNK2,
    UNION_NET_CMD_MAC,
    UNION_NET_CMD_UNK4,
    UNION_NET_CMD_PLAYER_INFO,
    UNION_NET_CMD_CARD_A,
    UNION_NET_CMD_UNK7,
    UNION_NET_CMD_UNK8,
    UNION_NET_CMD_UNK9,
    UNION_NET_CMD_ENTRY_REQUEST,
    UNION_NET_CMD_ENTRY_REPLY_2,
    UNION_NET_CMD_ENTRY_REPLY_1,
    UNION_NET_CMD_ENQUEUE,
    UNION_NET_CMD_UNKE,
    UNION_NET_CMD_REQUEST_STATUS,
    UNION_NET_CMD_STATUS,
    UNION_NET_CMD_APP_PROFILE,
    UNION_NET_CMD_PROFILE,
    UNION_NET_CMD_CONFIRM_PROFILE,
    UNION_NET_CMD_LEAVE,
    UNION_NET_CMD_JOIN_AVENUE,
};

// pokebw2's UnionNet_GetRecvBuffer
void *func_ov012_02173984(int netId, void *work, int size) {
    UnionSystem *unisys = work;

    GFL_ASSERT(size <= 0x800);
    return unisys->recvBuf[netId];
}

void func_ov012_021739a8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    func_ov012_02172edc(unisys);
    unisys->situ.mycomm.doneMask |= (u8)(1 << netId);
}

BOOL func_ov012_021739cc(void) {
    return func_0203d3a8(func_020399a8(), UNION_NET_CMD_UNK0, 0, NULL);
}

void func_ov012_021739e0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionSelf *self = &unisys->situ;
    const u32 *value = data;

    if (netId != func_0203d1a4(func_020399a8())) {
        self->mycomm.unk19 = *value;
    }
}

BOOL func_ov012_02173a04(u32 value) {
    return func_0203d3a8(func_020399a8(), UNION_NET_CMD_UNK1, sizeof(u32), &value);
}

void func_ov012_02173a24(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionSelf *self = &unisys->situ;
    const u32 *value = data;

    if (netId != func_0203d1a4(func_020399a8())) {
        self->mycomm.unk1A = *value;
    }
}

BOOL func_ov012_02173a48(u32 value) {
    return func_0203d3a8(func_020399a8(), UNION_NET_CMD_UNK2, sizeof(u32), &value);
}

void func_ov012_02173a68(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionSelf *self = &unisys->situ;

    if (netId != func_0203d1a4(func_020399a8())) {
        MI_CpuCopy8(data, self->mycomm.unk10, 8);
    }
}

BOOL func_ov012_02173a90(void) {
    u8 mac[8];

    MI_CpuFill8(mac, 0, sizeof(mac));
    OS_GetMacAddress(mac);
    mac[6] = 1;
    return func_0203d3a8(func_020399a8(), UNION_NET_CMD_MAC, sizeof(mac), mac);
}

void func_ov012_02173ac4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionSelf *self = &unisys->situ;
    const u32 *value = data;

    if (netId != func_0203d1a4(func_020399a8())) {
        self->mycomm.unk46_2 = *value;
    }
}

BOOL func_ov012_02173afc(u32 value) {
    return func_0203d3a8(func_020399a8(), UNION_NET_CMD_UNK4, sizeof(u32), &value);
}

void func_ov012_02173b1c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    func_02008494(data, func_02012950(unisys->param->gameData, netId));
    unisys->situ.mycomm.unk47 |= (u8)(1 << netId);
}

BOOL func_ov012_02173b4c(UnionSystem *unisys) {
    return func_0203d3a8(func_020399a8(), UNION_NET_CMD_PLAYER_INFO, func_02008468(),
                         GetGameDataPlayerInfo(unisys->param->gameData));
}

void func_ov012_02173b78(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionSelf *self = &unisys->situ;

    if (netId != func_0203d1a4(func_020399a8())) {
        self->mycomm.recvFlag = 1;
        MI_CpuCopy8(data, unisys->alloc.target_card, size);
    }
}

BOOL func_ov012_02173bb0(UnionSystem *unisys) {
    GFL_ASSERT(unisys->alloc.my_card != NULL);
    return func_0203d3f8(func_020399a8(), 0xff, UNION_NET_CMD_CARD_A, 0x678, unisys->alloc.my_card, TRUE, FALSE, TRUE);
}

void func_ov012_02173bfc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;
    const ColosseumMember *member = data;
    GameData *gameData = GSYS_GetGameData(unisys->param->gsys);
    u8 mac[6];

    if (colosseum == NULL || colosseum->commPlayer == NULL || colosseum->unk168 == NULL) {
        GFL_ASSERT(0);
        return;
    }
    OS_GetMacAddress(mac);
    func_ov021_021cd624(colosseum->unk168, netId, GetGameDataPlayerInfo(gameData), member->unk27, mac);
}


void func_ov012_02173c64(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;
    const u32 *value = data;

    if (colosseum == NULL) {
        GFL_ASSERT(0);
        return;
    }
    colosseum->unkDE = *value;
}

void func_ov012_02173c8c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum == NULL) {
        GFL_ASSERT(0);
        return;
    }
    colosseum->unkCA = 1;
}

BOOL func_ov012_02173cb4(void) {
    return func_0203d3a8(func_020399a8(), UNION_NET_CMD_UNK9, 0, NULL);
}

void func_ov012_02173ccc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    if (unisys->alloc.uniapp == NULL) {
        GFL_ASSERT(0);
        unisys->entryRepliedTo |= (u8)(1 << netId);
        return;
    }
    if (func_ov022_02183be8(unisys->alloc.uniapp, netId) == TRUE) {
        unisys->entryAcceptedTo |= (u8)(1 << netId);
    } else {
        unisys->entryRepliedTo |= (u8)(1 << netId);
    }
}

BOOL func_ov012_02173d38(UnionSystem *unisys) {
    NetHandle *handle;
    u32 size;

    unisys->entryReply = 0;
    handle = func_020399a8();
    size = func_02008468();
    return func_0203d3f8(handle, 0, UNION_NET_CMD_ENTRY_REQUEST, size, GetGameDataPlayerInfo(unisys->param->gameData),
                         TRUE, FALSE, FALSE);
}

void func_ov012_02173d7c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    unisys->entryReply = 2;
}

BOOL func_ov012_02173d88(u8 sendTo) {
    return func_0203d4d0(func_020399a8(), sendTo, UNION_NET_CMD_ENTRY_REPLY_2, 0, NULL, TRUE, FALSE, FALSE);
}

void func_ov012_02173db0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    unisys->entryReply = 1;
}

BOOL func_ov012_02173dbc(u8 sendTo) {
    return func_0203d4d0(func_020399a8(), sendTo, UNION_NET_CMD_ENTRY_REPLY_1, 0, NULL, TRUE, FALSE, FALSE);
}

void func_ov012_02173de4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    if (func_0203d378()) {
        if (unisys->alloc.uniapp == NULL) {
            GFL_ASSERT(unisys->alloc.uniapp != NULL);
            return;
        }
        func_ov022_02183f6c(unisys->alloc.uniapp, netId);
    }
}

// pokebw2's UnionMain_SendEnqueue
BOOL func_ov012_02173e1c(void) {
    return func_0203d3f8(func_020399a8(), 0, UNION_NET_CMD_ENQUEUE, 0, NULL, TRUE, FALSE, FALSE);
}

void func_ov012_02173e40(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    if (unisys->alloc.uniapp == NULL) {
        GFL_ASSERT(unisys->alloc.uniapp != NULL);
        return;
    }
    func_ov022_02183f7c(unisys->alloc.uniapp);
}


BOOL func_ov012_02173e68(u8 netId) {
    return func_0203d3f8(func_020399a8(), netId, UNION_NET_CMD_UNKE, 0, NULL, TRUE, FALSE, FALSE);
}

void func_ov012_02173e90(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    if (func_0203d378()) {
        if (unisys->alloc.uniapp == NULL) {
            GFL_ASSERT(unisys->alloc.uniapp != NULL);
            return;
        }
        func_ov022_02183f88(unisys->alloc.uniapp, netId);
    }
}

// pokebw2's UnionMain_SendRequestStatus
BOOL func_ov012_02173ec8(void) {
    return func_0203d3f8(func_020399a8(), 0, UNION_NET_CMD_REQUEST_STATUS, 0, NULL, TRUE, FALSE, FALSE);
}

void func_ov012_02173eec(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    if (func_0203d378() != TRUE) {
        if (unisys->alloc.uniapp == NULL) {
            GFL_ASSERT(0);
            return;
        }
        func_ov022_02183f98(unisys->alloc.uniapp, data);
    }
}

BOOL func_ov012_02173f20(const UnionAppStatus *status, u8 sendTo) {
    NetHandle *handle = func_020399a8();

    return func_0203d4d0(handle, sendTo, UNION_NET_CMD_STATUS, func_ov022_021840a8(), status, TRUE, FALSE, FALSE);
}

void func_ov012_02173f54(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    if (unisys->alloc.uniapp == NULL) {
        GFL_ASSERT(0);
        return;
    }
    func_ov022_02183fb8(unisys->alloc.uniapp, unisys, netId, (UnionAppMember *)data);
    func_ov022_02183fa8(unisys->alloc.uniapp, netId);
}

// pokebw2's UnionMain_SendAppProfile
BOOL func_ov012_02173f90(u8 sendTo, const PlayerInfo *info) {
    UnionAppMember member;

    MI_CpuFill8(&member, 0, sizeof(UnionAppMember));
    member.info = *info;
    OS_GetMacAddress(member.mac);
    return func_0203d4d0(func_020399a8(), sendTo, UNION_NET_CMD_APP_PROFILE, sizeof(UnionAppMember), &member, TRUE,
                         FALSE, FALSE);
}

void func_ov012_02173fe0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    if (unisys->alloc.uniapp == NULL) {
        GFL_ASSERT(0);
        return;
    }
    func_ov022_02183fb8(unisys->alloc.uniapp, unisys, netId, (UnionAppMember *)data);
}

BOOL func_ov012_02174010(u8 sendTo, const UnionAppMember *member) {
    return func_0203d4d0(func_020399a8(), sendTo, UNION_NET_CMD_PROFILE, sizeof(UnionAppMember), member, TRUE, FALSE,
                         FALSE);
}

void func_ov012_0217403c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;

    if (netId != func_0203d1a4(func_020399a8())) {
        if (unisys->alloc.uniapp == NULL) {
            GFL_ASSERT(0);
            return;
        }
        func_ov022_02183fb8(unisys->alloc.uniapp, unisys, netId, (UnionAppMember *)data);
        func_ov022_02184060(unisys->alloc.uniapp, netId);
    }
}

// pokebw2's UnionMain_SendConfirmProfile
BOOL func_ov012_02174084(u8 sendTo, const PlayerInfo *info) {
    UnionAppMember member;

    MI_CpuFill8(&member, 0, sizeof(UnionAppMember));
    member.info = *info;
    OS_GetMacAddress(member.mac);
    return func_0203d4d0(func_020399a8(), sendTo, UNION_NET_CMD_CONFIRM_PROFILE, sizeof(UnionAppMember), &member, TRUE,
                         FALSE, FALSE);
}

void func_ov012_021740d4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    const u8 *leftNetId = data;

    if (netId != func_0203d1a4(func_020399a8())) {
        if (unisys->alloc.uniapp == NULL) {
            GFL_ASSERT(0);
            return;
        }
        func_ov022_021840a4(unisys->alloc.uniapp, *leftNetId);
    }
}


BOOL func_ov012_0217410c(u8 sendTo, u8 leftNetId) {
    return func_0203d4d0(func_020399a8(), sendTo, UNION_NET_CMD_LEAVE, 1, &leftNetId, TRUE, FALSE, FALSE);
}

