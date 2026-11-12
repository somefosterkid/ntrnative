// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov028/colosseum.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Union Room colosseum's net commands (from 0x1500) and union events: the handlers that receive the players' info,
// trainer cards, choices and parties into the UnionColosseum, and the functions that send them. pokebw2 has them in
// colosseum.c; in Black they are an object of their own (its "" assert string and command table precede colosseum.c's
// data, whose first allocation is at line 53)
#include "field/colosseum.h"
#include "types.h"
#include "field/event_colosseum_battle.h"
#include "field/field_event.h"
#include "field/union_comm.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "field/comm_player.h"

// The heap that the Union Room uses
// The net commands, from the base 0x1500
#define COLOSSEUM_CMD_BASE 0x1500
#define COLOSSEUM_CMD_MEMBER (COLOSSEUM_CMD_BASE + 1)
#define COLOSSEUM_CMD_CARD (COLOSSEUM_CMD_BASE + 2)
#define COLOSSEUM_CMD_CHOICE (COLOSSEUM_CMD_BASE + 3)
#define COLOSSEUM_CMD_UNK4 (COLOSSEUM_CMD_BASE + 4)
#define COLOSSEUM_CMD_UNK5 (COLOSSEUM_CMD_BASE + 5)
#define COLOSSEUM_CMD_PARTY (COLOSSEUM_CMD_BASE + 6)
#define COLOSSEUM_CMD_RESULT (COLOSSEUM_CMD_BASE + 7)
#define COLOSSEUM_CMD_UNK8 (COLOSSEUM_CMD_BASE + 8)
#define COLOSSEUM_CMD_UNK9 (COLOSSEUM_CMD_BASE + 9)
#define COLOSSEUM_CMD_UNKA (COLOSSEUM_CMD_BASE + 10)
#define COLOSSEUM_CMD_UNKB (COLOSSEUM_CMD_BASE + 11)
#define COLOSSEUM_CMD_UNKD (COLOSSEUM_CMD_BASE + 13)

static void *func_ov012_02174b58(int netId, void *work, int size);
static void func_ov012_02174b7c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174b80(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174bec(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174c60(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174cb8(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174d10(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174d64(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174dc8(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174e08(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174e44(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174e88(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174ec4(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174efc(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02174f18(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov012_02175358(UnionColosseum *colosseum, BattlePlayer *player, u8 index, u32 unused);

// pokebw2's sColosseumCommands
static const NetCommand data_ov012_02175588[] = {
    {func_ov012_02174b7c, NULL},
    {func_ov012_02174b80, func_ov012_02174b58},
    {func_ov012_02174bec, func_ov012_02174b58},
    {func_ov012_02174c60, func_ov012_02174b58},
    {func_ov012_02174cb8, NULL},
    {func_ov012_02174d10, NULL},
    {func_ov012_02174d64, func_ov012_02174b58},
    {func_ov012_02174dc8, NULL},
    {func_ov012_02174e08, NULL},
    {func_ov012_02174e44, NULL},
    {func_ov012_02174e88, NULL},
    {func_ov012_02174ec4, NULL},
    {func_ov012_02174efc, NULL},
    {func_ov012_02174f18, NULL},
};

// pokebw2's Colosseum_RegisterCommands
void func_ov012_02174b40(UnionSystem *unisys) {
    func_0203a574(COLOSSEUM_CMD_BASE, data_ov012_02175588, NELEMS(data_ov012_02175588), unisys);
}

// pokebw2's Colosseum_GetRecvBuffer
static void *func_ov012_02174b58(int netId, void *work, int size) {
    UnionSystem *unisys = work;

    GFL_ASSERT(size <= 0x800);
    return unisys->recvBuf[netId];
}

// pokebw2's Colosseum_RecvNone
static void func_ov012_02174b7c(int netId, int size, const void *data, void *work, NetHandle *handle) {
}

// pokebw2's Colosseum_RecvMember
static void func_ov012_02174b80(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (netId != func_0203d1a4(func_020399a8()) && colosseum != NULL && colosseum->commPlayer != NULL) {
        MI_CpuCopy8(data, &colosseum->members[netId], size);
    }
}

// pokebw2's Colosseum_SendMember
BOOL func_ov012_02174bb8(const void *member, BOOL toParent) {
    u32 sendId = 0;

    if (toParent != TRUE) {
        sendId = GFL_NET_NETID_SERVER;
    }
    return func_0203d3f8(func_020399a8(), (u8)sendId, COLOSSEUM_CMD_MEMBER, 0x2c, member, TRUE, FALSE, TRUE);
}

static void func_ov012_02174bec(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (netId != func_0203d1a4(func_020399a8()) && colosseum != NULL && colosseum->commPlayer != NULL
        && colosseum->card[netId] != NULL) {
        MI_CpuCopy8(data, colosseum->card[netId], size);
        colosseum->cardReady[netId] = TRUE;
    }
}

// pokebw2's Colosseum_SendCardHead
BOOL func_ov012_02174c34(const TrainerCardData *card) {
    return func_0203d3f8(func_020399a8(), GFL_NET_NETID_SERVER, COLOSSEUM_CMD_CARD, 0x678, card, TRUE, FALSE, TRUE);
}

// pokebw2's Colosseum_RecvChoice
static void func_ov012_02174c60(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (netId != func_0203d1a4(func_020399a8()) && colosseum != NULL && colosseum->commPlayer != NULL) {
        func_ov012_021751b4(colosseum, netId, data);
    }
}

// pokebw2's Colosseum_SendChoice
BOOL func_ov012_02174c90(const ColosseumChoice *choice) {
    return func_0203d3f8(func_020399a8(), GFL_NET_NETID_SERVER, COLOSSEUM_CMD_CHOICE, sizeof(ColosseumChoice), choice, FALSE,
                  TRUE, TRUE);
}

// pokebw2's Colosseum_RecvUnk4
static void func_ov012_02174cb8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL && colosseum->commPlayer != NULL && func_0203d378()) {
        func_ov012_021750d8(colosseum, netId, *(const u8 *)data);
    }
}

BOOL func_ov012_02174ce4(UnionColosseum *colosseum) {
    u8 value = func_ov012_021750c0(colosseum);

    return func_0203d3f8(func_020399a8(), 0, COLOSSEUM_CMD_UNK4, 1, &value, TRUE, FALSE, FALSE);
}

// pokebw2's Colosseum_RecvUnk5
static void func_ov012_02174d10(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL && colosseum->commPlayer != NULL) {
        func_ov012_021750c8(colosseum, *(const u32 *)data);
    }
}

// pokebw2's Colosseum_SendUnk5
BOOL func_ov012_02174d2c(UnionColosseum *colosseum, u32 netId, u32 value) {
    func_ov012_021750c0(colosseum);
    return func_0203d3f8(func_020399a8(), (u8)netId, COLOSSEUM_CMD_UNK5, 4, &value, TRUE, FALSE, FALSE);
}

// pokebw2's Colosseum_RecvParty
static void func_ov012_02174d64(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL && colosseum->party[netId] != NULL) {
        MI_CpuCopy8(data, colosseum->party[netId], size);
        colosseum->partyReady[netId] = TRUE;
    }
}

// pokebw2's Colosseum_SendParty
BOOL func_ov012_02174d98(const PokeParty *party) {
    NetHandle *handle = func_020399a8();

    return func_0203d3f8(handle, GFL_NET_NETID_SERVER, COLOSSEUM_CMD_PARTY, PokeParty_GetSaveDataSize(), party, TRUE, FALSE,
                  TRUE);
}

// pokebw2's Colosseum_RecvResult
static void func_ov012_02174dc8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL) {
        MI_CpuCopy8(data, colosseum->unk15C, size);
        colosseum->unk160 = TRUE;
    }
}

// pokebw2's Colosseum_SendResult
BOOL func_ov012_02174df0(const void *result) {
    return func_0203d3a8(func_020399a8(), COLOSSEUM_CMD_RESULT, 4, result);
}

// pokebw2's Colosseum_RecvUnk8
static void func_ov012_02174e08(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL) {
        colosseum->unkD4[netId] = TRUE;
    }
}

BOOL func_ov012_02174e20(void) {
    return func_0203d3f8(func_020399a8(), 0, COLOSSEUM_CMD_UNK8, 0, NULL, TRUE, FALSE, FALSE);
}

// pokebw2's Colosseum_RecvUnk9
static void func_ov012_02174e44(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL) {
        colosseum->unkD8[netId] = TRUE;
        colosseum->unkD4[netId] = FALSE;
    }
}

BOOL func_ov012_02174e64(void) {
    return func_0203d3f8(func_020399a8(), 0, COLOSSEUM_CMD_UNK9, 0, NULL, TRUE, FALSE, FALSE);
}

// pokebw2's Colosseum_RecvUnkA
static void func_ov012_02174e88(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL) {
        colosseum->unkDF = FALSE;
    }
}

// pokebw2's Colosseum_SendUnkA
BOOL func_ov012_02174e9c(u32 netId) {
    return func_0203d3f8(func_020399a8(), (u8)netId, COLOSSEUM_CMD_UNKA, 0, NULL, TRUE, FALSE, FALSE);
}

// pokebw2's Colosseum_RecvUnkB
static void func_ov012_02174ec4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL) {
        colosseum->unkCB = TRUE;
    }
}

// pokebw2's Colosseum_SendUnkB
BOOL func_ov012_02174ed8(void) {
    return func_0203d3f8(func_020399a8(), GFL_NET_NETID_SERVER, COLOSSEUM_CMD_UNKB, 0, NULL, TRUE, FALSE, FALSE);
}

// pokebw2's Colosseum_RecvUnkC
static void func_ov012_02174efc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL) {
        colosseum->unk161++;
    }
}

// pokebw2's Colosseum_RecvUnkD
static void func_ov012_02174f18(int netId, int size, const void *data, void *work, NetHandle *handle) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL) {
        colosseum->unk162[netId] = TRUE;
    }
}

BOOL func_ov012_02174f34(void) {
    return func_0203d3a8(func_020399a8(), COLOSSEUM_CMD_UNKD, 0, NULL);
}
