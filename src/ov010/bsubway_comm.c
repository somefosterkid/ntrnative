// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/bsubway_comm.c, ported by tools/port_file.py.
// The Battle Subway's multi-battle communication with the partner: the beacon, and the commands that send the members,
// the trainers of the train and the partner's choices. The name is descriptive; the file has no assert to give it
#include "types.h"
#include "constants/pokemon.h"
#include "field/bsubway_scr.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_system.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "system/game_data.h"
#include "nitro/os.h"

static void *func_ov010_02167ac4(void *work);
static int func_ov010_02167af0(void *work);
static BOOL func_ov010_02167af4(int a, int b);
static void func_ov010_02167b44(BSubwayScrWork *bsw, GameData *gameData);
static void func_ov010_02167b9c(BSubwayScrWork *bsw);
static void func_ov010_02167bb4(BSubwayScrWork *bsw, u16 retire);
static void func_ov010_02167bcc(BSubwayScrWork *bsw);
static void func_ov010_02167bd8(BSubwayScrWork *bsw, u16 value);
static void func_ov010_02167be4(BSubwayScrWork *bsw, u16 value);
static void func_ov010_02167bf0(BSubwayScrWork *bsw, u16 value);
static void func_ov010_02167d44(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_02167db8(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_02167dec(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_02167e28(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_02167e54(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_02167e88(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_02167eac(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_02167ed0(int netId, int size, const void *data, void *work, NetHandle *handle);

static const NetCommand data_ov010_02171fd4[] = {
    {func_ov010_02167d44, NULL},
    {func_ov010_02167db8, NULL},
    {func_ov010_02167dec, NULL},
    {func_ov010_02167e28, NULL},
    {func_ov010_02167e54, NULL},
    {func_ov010_02167e88, NULL},
    {func_ov010_02167eac, NULL},
    {func_ov010_02167ed0, NULL},
};

static const GFLNetInitData data_ov010_02172014 = {
    data_ov010_02171fd4,
    NELEMS(data_ov010_02171fd4),
    NULL,
    NULL,
    NULL,
    NULL,
    func_ov010_02167ac4,
    func_ov010_02167af0,
    func_ov010_02167af4,
    NULL,
    NULL,
    NULL,
    NULL,
    {0},
    NULL,
    NULL,
    0,
    {1, 0, 0, 0, 0x80, 0x13, 0, 0},
    HEAPID_USER,
    HEAPID_TAIL(0xd),
    HEAPID_TAIL(0xf),
    HEAPID_TAIL(0xd),
    0xf0,
    0,
    2,
    0x60,
    8,
    1,
    0,
    0,
    0,
    0x2c,
    {0x2c, 1, 0, 0},
    0,
    0,
};

void func_ov010_02167ab0(BSubwayScrWork *bsw) {
    func_0203cbac(&data_ov010_02172014, NULL, bsw);
}

static void *func_ov010_02167ac4(void *work) {
    BSubwayScrWork *bsw = work;
    BSubwayBeacon *beacon = &bsw->beacon;

    func_02008494(GetGameDataPlayerInfo(bsw->gameData), &beacon->playerInfo);
    OS_GetMacAddress(beacon->mac);
    beacon->gameId = 0x3a0b;
    return beacon;
}

static int func_ov010_02167af0(void *work) {
    return sizeof(BSubwayBeacon);
}

static BOOL func_ov010_02167af4(int a, int b) {
    if (a == b) {
        return TRUE;
    }
    return FALSE;
}

void func_ov010_02167b00(BSubwayScrWork *bsw) {
    // The count passed is the end command, not the table's length
    func_0203a574(BSUBWAY_COMM_MEMBERS, data_ov010_02171fd4, BSUBWAY_COMM_MAX, bsw);
}

void func_ov010_02167b18(BSubwayScrWork *bsw) {
    func_0203a5e4(BSUBWAY_COMM_MEMBERS);
}

void func_ov010_02167b24(u8 timing) {
    func_0203d594(func_020399a8(), timing);
}

BOOL func_ov010_02167b34(u8 timing) {
    return func_0203d5a0(func_020399a8(), timing);
}

static void func_ov010_02167b44(BSubwayScrWork *bsw, GameData *gameData) {
    PlayerInfo *info = GetGameDataPlayerInfo(gameData);
    PokeParty *party = func_ov021_021ca210(bsw);
    int i;

    bsw->sendBuf[0] = getTrainerGender(info);
    for (i = 0; i < 2; i++) {
        bsw->sendBuf[1 + i] =
            PokeParty_GetParam(PokeParty_GetPkm(party, bsw->memberSlots[i]), PKM_PARAM_SPECIES, NULL);
    }
    bsw->sendBuf[3] = func_ov021_021ca2cc(func_ov021_021ca234(bsw));
}

static void func_ov010_02167b9c(BSubwayScrWork *bsw) {
    MI_CpuCopy8(bsw->unk32, bsw->sendBuf, 14 * sizeof(u16));
}

static void func_ov010_02167bb4(BSubwayScrWork *bsw, u16 retire) {
    bsw->unkC_3 = retire;
    bsw->sendBuf[0] = retire;
}

static void func_ov010_02167bcc(BSubwayScrWork *bsw) {
    *(u32 *)bsw->sendBuf = bsw->playMode;
}

static void func_ov010_02167bd8(BSubwayScrWork *bsw, u16 value) {
    *(u32 *)bsw->sendBuf = value;
}

static void func_ov010_02167be4(BSubwayScrWork *bsw, u16 value) {
    *(u32 *)bsw->sendBuf = value;
}

static void func_ov010_02167bf0(BSubwayScrWork *bsw, u16 value) {
    *(u32 *)bsw->sendBuf = value;
}

void func_ov010_02167bfc(BSubwayScrWork *bsw, u16 mode, u16 value) {
    GameData *gameData = bsw->gameData;

    switch (mode) {
    case 0:
        bsw->sendCommand = BSUBWAY_COMM_MEMBERS;
        func_ov010_02167b44(bsw, gameData);
        break;
    case 1:
        bsw->sendCommand = BSUBWAY_COMM_TRAINERS;
        func_ov010_02167b9c(bsw);
        break;
    case 2:
        bsw->sendCommand = BSUBWAY_COMM_RETIRE;
        func_ov010_02167bb4(bsw, value);
        break;
    case 3:
        bsw->sendCommand = BSUBWAY_COMM_PLAYER_INFO;
        func_02008494(GetGameDataPlayerInfo(gameData), (PlayerInfo *)bsw->sendBuf);
        break;
    case 4:
        bsw->sendCommand = BSUBWAY_COMM_PLAY_MODE;
        func_ov010_02167bcc(bsw);
        break;
    case 5:
        bsw->sendCommand = BSUBWAY_COMM_5;
        func_ov010_02167bd8(bsw, value);
        break;
    case 6:
        bsw->sendCommand = BSUBWAY_COMM_6;
        func_ov010_02167be4(bsw, value);
        break;
    case 7:
        bsw->sendCommand = BSUBWAY_COMM_7;
        func_ov010_02167bf0(bsw, value);
        break;
    default:
        bsw->sendCommand = BSUBWAY_COMM_MAX;
        break;
    }
}

BOOL func_ov010_02167cb4(BSubwayScrWork *bsw) {
    u16 command;

    if (func_0200fb24()) {
        return TRUE;
    }
    command = bsw->sendCommand;
    if (command >= BSUBWAY_COMM_MAX) {
        return TRUE;
    }
    if (func_0203d3a8(func_020399a8(), command, sizeof(bsw->sendBuf), bsw->sendBuf) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void func_ov010_02167cf4(BSubwayScrWork *bsw, u8 mode) {
    bsw->recvMode = mode;
}

BOOL func_ov010_02167d00(BSubwayScrWork *bsw, u16 *var) {
    u8 count;

    if (func_0200fb24()) {
        return TRUE;
    }
    count = 1;
    if (bsw->recvMode != 1) {
        count = 2;
    }
    if (bsw->recvCount == count) {
        bsw->recvCount = 0;
        if (var != NULL) {
            *var = bsw->recvResult;
        }
        return TRUE;
    }
    return FALSE;
}

static void func_ov010_02167d44(int netId, int size, const void *data, void *work, NetHandle *handle) {
    BSubwayScrWork *bsw = work;
    const u16 *recv = data;
    u16 result = 0;

    bsw->recvCount++;
    if (netId == func_02039338()) {
        return;
    }
    bsw->partnerGender = recv[0];
    bsw->partnerSpecies[0] = recv[1];
    bsw->partnerSpecies[1] = recv[2];
    bsw->unk18 = recv[3];
    bsw->unkC_5 = bsw->partnerGender + 5;
    if (bsw->memberSpecies[0] == bsw->partnerSpecies[0] || bsw->memberSpecies[0] == bsw->partnerSpecies[1]) {
        result += 1;
    }
    if (bsw->memberSpecies[1] == bsw->partnerSpecies[0] || bsw->memberSpecies[1] == bsw->partnerSpecies[1]) {
        result += 2;
    }
    bsw->recvResult = result;
}

static void func_ov010_02167db8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    BSubwayScrWork *bsw = work;

    bsw->recvCount++;
    if (netId == func_02039338() || func_02039338() == 0) {
        return;
    }
    MI_CpuCopy8(data, bsw->unk32, 14 * sizeof(u16));
}

static void func_ov010_02167dec(int netId, int size, const void *data, void *work, NetHandle *handle) {
    BSubwayScrWork *bsw = work;
    const u16 *recv = data;

    bsw->recvCount++;
    if (netId == func_02039338()) {
        return;
    }
    bsw->recvResult = 0;
    if (bsw->unkC_3 || recv[0]) {
        bsw->recvResult = 1;
    }
}

static void func_ov010_02167e28(int netId, int size, const void *data, void *work, NetHandle *handle) {
    BSubwayScrWork *bsw = work;

    bsw->recvCount++;
    if (netId == func_02039338()) {
        return;
    }
    MI_CpuCopy8(data, &bsw->partner, sizeof(PlayerInfo));
}

static void func_ov010_02167e54(int netId, int size, const void *data, void *work, NetHandle *handle) {
    BSubwayScrWork *bsw = work;
    const u16 *recv = data;

    bsw->recvCount++;
    if (netId == func_02039338()) {
        return;
    }
    if (bsw->playMode != recv[0]) {
        bsw->recvResult = 0;
    } else {
        bsw->recvResult = 1;
    }
}

static void func_ov010_02167e88(int netId, int size, const void *data, void *work, NetHandle *handle) {
    BSubwayScrWork *bsw = work;
    const u16 *recv = data;

    bsw->recvCount++;
    if (netId == func_02039338()) {
        return;
    }
    bsw->recvResult = recv[0];
}

static void func_ov010_02167eac(int netId, int size, const void *data, void *work, NetHandle *handle) {
    BSubwayScrWork *bsw = work;
    const u16 *recv = data;

    bsw->recvCount++;
    if (netId == func_02039338()) {
        return;
    }
    bsw->recvResult = recv[0];
}

static void func_ov010_02167ed0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    BSubwayScrWork *bsw = work;
    const u16 *recv = data;

    bsw->recvCount++;
    if (netId == func_02039338()) {
        return;
    }
    bsw->recvResult = recv[0];
}
