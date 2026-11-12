// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/net_system.c, adapted to Black.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_queue.h"
#include "gfl/net_ring_buff.h"
#include "gfl/net_state.h"
#include "gfl/net_system.h"
#include "gfl/std.h"
#include "gfl/wm_icon.h"

// Packets between the machines: a header with a CRC, the size, the connected machines, flags and a sequence number,
// then commands from the send queue. Each machine's received commands go through a ring buffer to their handlers

// The packet header
#define PACKET_HEADER_SIZE 7
#define PACKET_CRC_LO 0
#define PACKET_CRC_HI 1
#define PACKET_SIZE 2
#define PACKET_CONNECT_BITS 3
#define PACKET_FLAGS 4
#define PACKET_SEQ 5
// A packet with nothing to send
#define PACKET_FLAGS_NONE 0xff
#define PACKET_FLAG_FIRST 1
#define PACKET_FLAG_CONTINUED 2
// The size field holds the size less 0xff
#define PACKET_FLAG_LONG 8

// Where a machine's commands are while they arrive
typedef struct {
    int pos;
    u8 *buffer;
    u16 remaining;
    u16 size;
    u16 command;
    u8 netId;
    u8 sender;
} NetRecvState;

typedef struct {
    u8 *sendData;
    u8 *sendRingData;
    u8 *mpSendData;
    u8 *mpRingData;
    u8 *recvData;
    u8 *serverRingData;
    u8 *recvTemp;
    NetRingBuff sendRing;
    NetRingBuff serverRing;
    NetRingBuff mpRing;
    NetRingBuff recvRings[GFL_NET_MACHINE_MAX];
    NetQueue sendQueue;
    NetQueue mpQueue;
    NetRecvState recvStates[GFL_NET_MACHINE_MAX];
    NetRecvState serverRecvState;
    BOOL unk1AC;
    int serverSends;
    // The packets sent to each machine that have not come back
    int sends[GFL_NET_MACHINE_MAX];
    u8 mpSeq;
    u8 sendSeq;
    u8 serverSeq;
    u8 recvSeq[GFL_NET_MACHINE_MAX];
    int recvSize;
    u16 connectBits;
    u8 unk1E6;
    u8 unk1E7;
    u8 unk1E8;
    u8 error;
    u8 unk1EA;
    u8 unk1EB;
    u8 unk1EC;
    u8 unk1ED;
} NetSystem;

// Records a network error, from the line it was found on
#define NET_ERROR(line)                                                             \
    do {                                                                            \
        GFLNetErrorInfo *err;                                                       \
        func_02039548();                                                            \
        err = func_0203ca98();                                                      \
        if (!err->type) {                                                       \
            func_0203c9ac(err->code, err->unk4, err->unk8, line);                   \
        }                                                                           \
    } while (0)

static void func_02037224(u8 state, int line);
static void func_02037234(NetRecvState *state);
static int func_02037258(void);
static BOOL func_02037288(int size, HeapID heapId, BOOL useSysSize);
static void func_02037478(void);
static void func_02037680(int netID);
static BOOL func_020377ac(u16 netId, u8 *data, u16 size);
static void func_0203781c(int netId);
static void func_02037858(int netId);
static void func_02037ba4(void);
static BOOL func_02037d98(BOOL server);
static void func_02037e60(void);
static void func_02037f48(void);
static void func_0203802c(void);
static BOOL func_020380e4(u16 netId, u8 *data, u16 size);
static void func_020381d8(u8 *data, int size);
static void func_02038298(void);
static void func_02038340(void);
static void func_02038380(void);
static BOOL func_02038524(void);
static void func_02038570(void);
static void func_020385c4(void);
static BOOL func_020387a0(u16 netId, u8 *data, u16 size);
static BOOL func_02038924(u16 netId, u8 *data, u16 size);
static BOOL func_02038a48(BOOL ok);
static BOOL func_02038a90(BOOL ok);
static BOOL func_02038ae0(u8 *packet, NetQueue *queue, int size, u8 seq);
static void func_02038d68(int command, int size, void *data, NetRecvState *state);
static void func_02038df0(NetRingBuff *ring, int netId, u8 *temp, NetRecvState *state, int size);
static void func_02039050(void);
static void func_020390bc(void);

// Two variables, where pokebw2 has a struct: as one, the stores to it are scheduled differently. MWCC lays a file's
// statics out in the reverse of their declaration order, so sSendState comes first in the ROM
static u8 sRecvState = 4;
static u8 sSendState = 4;

static NetSystem *sNetSys;

static void func_02037224(u8 state, int line) {
    sSendState = state;
}

static void func_02037234(NetRecvState *state) {
    state->command = 0;
    state->remaining = 0xffff;
    state->size = 0xffff;
    state->buffer = NULL;
    state->pos = 0;
}

// The size of the packet the parent sends
static int func_02037258(void) {
    GFLNetSys *sys = func_0203d788();

    if (sys->aNetInit.bMPMode && sys->aNetInit.unk6C) {
        return sys->aNetInit.unk6C;
    }
    return func_0203d6bc() * func_0203d680();
}

static BOOL func_02037288(int size, HeapID heapId, BOOL useSysSize) {
    if (sNetSys == NULL) {
        int machineMax = func_0203d680();
        int sendSize = func_02037258();

        sNetSys = GFL_HeapAllocate(heapId, sizeof(NetSystem), TRUE, "net_system.c", 221);
        if (useSysSize) {
            sNetSys->recvSize = sendSize + 0x40;
        } else {
            sNetSys->recvSize = size + 0x40;
        }
        sNetSys->serverRingData = GFL_HeapAllocate(heapId, sNetSys->recvSize * 3, TRUE, "net_system.c", 230);
        sNetSys->recvTemp = GFL_HeapAllocate(heapId, sNetSys->recvSize * 2, TRUE, "net_system.c", 232);
        sNetSys->recvData = GFL_HeapAllocate(heapId, machineMax * sNetSys->recvSize, TRUE, "net_system.c", 233);
        sNetSys->sendData = GFL_HeapAllocate(heapId, func_0203d6bc(), TRUE, "net_system.c", 236);
        sNetSys->sendRingData = GFL_HeapAllocate(heapId, 15 * func_0203d6bc(), TRUE, "net_system.c", 237);
        sNetSys->mpSendData = GFL_HeapAllocate(heapId, sendSize, TRUE, "net_system.c", 238);
        sNetSys->mpRingData = GFL_HeapAllocate(heapId, sendSize * 2, TRUE, "net_system.c", 239);
        func_02036af4(&sNetSys->sendQueue, 100, &sNetSys->sendRing, heapId);
        func_02036af4(&sNetSys->mpQueue, 180, &sNetSys->mpRing, heapId);
        sNetSys->unk1AC = TRUE;
        func_02037478();
    }
    sNetSys->connectBits = 0;
    sNetSys->unk1E7 = 0;
    return TRUE;
}

static void func_02037478(void) {
    int i;
    int machineMax = func_0203d680();
    int sendSize = func_02037258();
    int dataSize = func_0203d6bc();

    MI_CpuFill8(sNetSys->recvData, 0, sNetSys->recvSize * machineMax);
    for (i = 0; i < machineMax; i++) {
        func_0203640c(&sNetSys->recvRings[i], sNetSys->recvData + i * sNetSys->recvSize, sNetSys->recvSize);
        sNetSys->recvSeq[i] = 0;
    }
    MI_CpuFill8(sNetSys->mpRingData, 0, (u32)sendSize * 2);
    func_0203640c(&sNetSys->mpRing, sNetSys->mpRingData, sendSize * 2);
    for (i = 0; i < sendSize; i++) {
        sNetSys->mpSendData[i] = 0;
    }
    MI_CpuFill8(sNetSys->sendRingData, 0, (u32)dataSize * 15);
    func_0203640c(&sNetSys->sendRing, sNetSys->sendRingData, dataSize * 15);
    sNetSys->sendData[0] = 0xff;
    for (i = 1; i < dataSize; i++) {
        sNetSys->sendData[i] = 0;
    }
    MI_CpuFill8(sNetSys->serverRingData, 0, sNetSys->recvSize * 3);
    func_0203640c(&sNetSys->serverRing, sNetSys->serverRingData, sNetSys->recvSize * 3);
    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        func_02037234(&sNetSys->recvStates[i]);
        sNetSys->sends[i] = 0;
    }
    sNetSys->serverSends = 0;
    func_02037234(&sNetSys->serverRecvState);
    func_02037224(4, 314);
    sRecvState = 4;
    func_02036b4c(&sNetSys->sendQueue);
    func_02036b4c(&sNetSys->mpQueue);
    sNetSys->unk1E8 = 0;
    sNetSys->unk1EB = 1;
    sNetSys->unk1EA = 0;
    sNetSys->unk1ED = 0;
}


static void func_02037680(int netID) {
    GFL_ASSERT_AT(341, netID < GFL_NET_MACHINE_MAX);
    sNetSys->sends[netID] = 0;
    func_0203640c(&sNetSys->recvRings[netID], sNetSys->recvData + netID * sNetSys->recvSize, sNetSys->recvSize);
    sNetSys->recvSeq[netID] = 0;
    func_02037234(&sNetSys->recvStates[netID]);
}

BOOL func_02037700(int size, HeapID heapId) {
    GFLNetInitData *ini = func_0203d798();
    GFLNetSys *sys = func_0203d788();

    func_02037288(size, heapId, sys->aNetInit.bMPMode && sys->aNetInit.unk6C);
    sys->pDevTable->init(ini->heapId, sys, 0, sys->devWork);
    sys->pDevTable->unk08(0);
    sys->pDevTable->setRecvCallback(func_020387a0);
    return TRUE;
}

int func_0203778c(int a0) {
    GFLNetSys *sys = func_0203d788();

    return sys->pDevTable->unk90(a0);
}

static BOOL func_020377ac(u16 netId, u8 *data, u16 size) {
    return func_020380e4(netId, data, size);
}

BOOL func_020377b8(int a0) {
    GFLNetInitData *ini = func_0203d798();
    GFLNetSys *sys = func_0203d788();

    ini->unk66 = a0;
    sys->pDevTable->setRecvCallback(func_020377ac);
    func_02037288(0x80, ini->heapId, FALSE);
    func_02037680(0);
    func_02037680(1);
    func_02037680(2);
    return TRUE;
}

static void func_0203781c(int netId) {
    GFLNetInitData *ini = func_0203d798();
    GFLNetSys *sys = func_0203d788();

    func_02037680(netId);
    if (ini->disconnectCallback != NULL) {
        ini->disconnectCallback(sys->devWork, netId);
    }
}

static void func_02037858(int netId) {
    GFLNetInitData *ini = func_0203d798();
    GFLNetSys *sys = func_0203d788();

    func_02037680(netId);
    if (ini->disconnectCallback != NULL) {
        ini->disconnectCallback(sys->devWork, netId);
    }
}

int func_02037894(int a0, int size, int a2, BOOL (*callback)(void)) {
    GFLNetInitData *ini = func_0203d798();
    GFLNetSys *sys = func_0203d788();
    BOOL useSysSize;

    ini->unk66 = a0;
    sys->pDevTable->setDisconnectCallback(func_0203781c);
    sys->pDevTable->setRecvCallback(func_020387a0);
    useSysSize = sys->aNetInit.bMPMode && sys->aNetInit.unk6C;
    func_02037288(size, func_0203d798()->heapId, useSysSize);
    return sys->pDevTable->unk40(a2, callback);
}

BOOL func_02037928(int a0, int size) {
    GFLNetSys *sys = func_0203d788();
    BOOL result;

    sys->pDevTable->setDisconnectCallback(func_02037858);
    result = sys->pDevTable->unk44(a0, 0);
    if (result == TRUE) {
        BOOL useSysSize;

        sys->pDevTable->setRecvCallback(func_02038924);
        useSysSize = sys->aNetInit.bMPMode && sys->aNetInit.unk6C;
        func_02037288(size, func_0203d798()->heapId, useSysSize);
        sRecvState = 4;
    }
    return result;
}

BOOL func_020379c8(int size) {
    GFLNetSys *sys = func_0203d788();
    BOOL result;

    sys->pDevTable->setDisconnectCallback(func_02037858);
    result = sys->pDevTable->unkBC();
    if (result == TRUE) {
        sys->pDevTable->setRecvCallback(func_02038924);
        func_02037288(size, func_0203d798()->heapId, FALSE);
        sRecvState = 4;
    }
    return result;
}

void func_02037a44(int a0) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable != NULL) {
        sys->pDevTable->unkC0(a0);
    }
}

int func_02037a6c(BOOL a0, const u8 *mac, int size, void (*callback)(void)) {
    GFLNetSys *sys = func_0203d788();
    func_0203d798();

    sys->pDevTable->setRecvCallback(func_02038924);
    if (a0) {
        BOOL useSysSize = sys->aNetInit.bMPMode && sys->aNetInit.unk6C;

        func_02037288(size, func_0203d798()->heapId, useSysSize);
        sRecvState = 4;
    }
    return sys->pDevTable->unk48(a0, mac, 0, 0, callback);
}

void func_02037b08(void) {
    func_0203d788();
    func_0203d798();

    if (sNetSys != NULL) {
        GFL_HeapFree(sNetSys->serverRingData);
        GFL_HeapFree(sNetSys->recvTemp);
        GFL_HeapFree(sNetSys->recvData);
        func_02036b88(&sNetSys->mpQueue);
        func_02036b88(&sNetSys->sendQueue);
        GFL_HeapFree(sNetSys->sendData);
        GFL_HeapFree(sNetSys->sendRingData);
        GFL_HeapFree(sNetSys->mpSendData);
        GFL_HeapFree(sNetSys->mpRingData);
        GFL_HeapFree(sNetSys);
        sNetSys = NULL;
    }
}

static void func_02037ba4(void) {
    if (sNetSys->unk1EC) {
        if (func_02039338() == 0) {
            if (func_020393a8() == 0) {
                func_02037b08();
            }
        } else {
            func_02037b08();
        }
    }
}

void func_02037be8(u32 netId) {
    if (sNetSys != NULL && netId < GFL_NET_MACHINE_MAX) {
        func_02037680(netId);
    }
}

// Sends and receives, each frame
BOOL func_02037c10(void) {
    GFLNetSys *sys = func_0203d788();
    GFLNetInitData *ini = func_0203d798();

    if (sNetSys == NULL) {
        if (sys != NULL) {
            sys->pDevTable->update(0);
        }
        return TRUE;
    }
    if (sys->pDevTable->getSignalLevel != NULL) {
        func_020370f4(sys->pDevTable->getSignalLevel());
    }
    if (!sNetSys->unk1EA && !func_020393d0()) {
        if (!ini->bMPMode && (ini->bNetType == 0 || ini->bNetType == 5)) {
            func_02037f48();
            func_020390bc();
        } else if (ini->bNetType == 3 || ini->bNetType == 4) {
            func_0203802c();
            func_020390bc();
        } else if (ini->bNetType == 1 || ini->bNetType == 2) {
            func_02038298();
            func_020390bc();
        } else {
            func_02038340();
            func_02039050();
            if (!func_02039338() && func_02039160(0) && ini->bMPMode) {
                func_020385c4();
            }
            if (!func_02039338()) {
                func_020390bc();
            }
        }
        if (ini->bNetType != 1 && ini->bNetType != 2) {
            sys->pDevTable->update(sNetSys->connectBits);
        }
        func_0203acac();
        func_02037ba4();
    } else {
        sys->pDevTable->update(0);
    }
    return TRUE;
}

void func_02037d78(void) {
    if (sNetSys != NULL) {
        func_02037478();
    }
}

// Writes the next packet to send
static BOOL func_02037d98(BOOL server) {
    BOOL result = FALSE;

    if (sRecvState != 4) {
        return result;
    }
    if (server) {
        NetSystem *net = sNetSys;

        if (net->serverSends > 0) {
            return result;
        }
        if (func_02038ae0(net->sendData, &net->sendQueue, func_0203d6bc(), net->sendSeq)) {
            sNetSys->sendSeq++;
        }
        sRecvState = 0;
        result = TRUE;
    } else {
        NetSystem *net = sNetSys;

        if (func_02038ae0(net->sendData, &net->sendQueue, func_0203d6bc(), net->sendSeq)) {
            sNetSys->sendSeq++;
            sRecvState = 0;
            result = TRUE;
        }
    }
    return result;
}

static void func_02037e60(void) {
    GFLNetSys *sys = func_0203d788();
    int dataSize = func_0203d6bc();

    if (sys->pDevTable->isConnected() && func_02039160(func_02039338()) && sRecvState == 0) {
        if (func_02039338() != 0) {
            if (sys->pDevTable->send(sNetSys->sendData, dataSize, 0, func_02038a48)) {
                sRecvState = 2;
                sNetSys->serverSends++;
            }
        } else {
            sRecvState = 2;
            func_02038a48(TRUE);
            func_020387a0(0, sNetSys->sendData, dataSize);
            sNetSys->serverSends++;
        }
    }
}

static void func_02037f48(void) {
    int i;
    int machineMax;
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->unk58()) {
        if (sNetSys->unk1EB) {
            func_02037d98(TRUE);
        }
        if (sys->pDevTable->unk5C(sNetSys->sendData) == TRUE) {
            int dataSize;

            sRecvState = 4;
            sNetSys->unk1EB = TRUE;
            dataSize = func_0203d6bc();
            machineMax = func_0203d680();
            for (i = 0; i < machineMax; i++) {
                func_020387a0(i, sys->pDevTable->getRecvData(i), dataSize);
            }
        } else {
            sNetSys->unk1EB = FALSE;
        }
    }
}

static void func_0203802c(void) {
    GFLNetSys *sys = func_0203d788();

    if (func_020366a8(&sNetSys->sendQueue)) {
        func_0203d3a8(func_020399a8(), 0x10, 0, NULL);
    }
    if (sys->pDevTable->unk9C != NULL) {
        sys->pDevTable->unk9C();
    }
    if (sys->pDevTable->unk6C() == TRUE) {
        int dataSize = func_0203d6bc();

        if (sys->pDevTable->unkA0() == TRUE && func_02037d98(FALSE)) {
            func_020381d8(sNetSys->sendData, dataSize);
            sRecvState = 4;
        }
    }
}

static BOOL func_020380e4(u16 netId, u8 *data, u16 size) {
    int crc;
    int len;

    func_0203d798();
    func_0203d6bc();
    if (data[PACKET_FLAGS] == PACKET_FLAGS_NONE) {
        return FALSE;
    }
    if (!sNetSys->recvRings[netId].received && (data[PACKET_FLAGS] & PACKET_FLAG_CONTINUED)) {
        return FALSE;
    }
    crc = getCRC16(data + PACKET_SIZE, size - 2);
    if (data[PACKET_CRC_LO] != (u8)crc || (crc & 0xff00) >> 8 != data[PACKET_CRC_HI]) {
        return FALSE;
    }
    if (sNetSys->recvSeq[netId] != 0 && (u8)(data[PACKET_SEQ] - 1) != sNetSys->recvSeq[netId]) {
        return FALSE;
    }
    sNetSys->recvSeq[netId] = data[PACKET_SEQ];
    len = data[PACKET_SIZE];
    if (data[PACKET_FLAGS] & PACKET_FLAG_LONG) {
        len += 0xff;
    }
    func_02036428(&sNetSys->recvRings[netId], data + PACKET_HEADER_SIZE, len);
    sNetSys->recvRings[netId].received = TRUE;
    return TRUE;
}

static void func_020381d8(u8 *data, int size) {
    GFLNetSys *sys;

    func_0203d798();
    sys = func_0203d788();
    if (!sys->pDevTable->unkA4() || !sys->pDevTable->unkAC()) {
        GFL_ASSERT_AT(1024, 0);
        return;
    }
    if (sRecvState == 0) {
        sys->pDevTable->send(data, size, 0, NULL);
        func_020380e4(sys->pDevTable->getNetId(), data, size);
        func_02037224(4, 1036);
    }
}

static void func_02038298(void) {
    GFLNetSys *sys;
    int dataSize;
    NetSystem *net;

    func_0203d798();
    sys = func_0203d788();
    dataSize = func_0203d6bc();
    net = sNetSys;
    if (net->unk1E7) {
        if (!net->unk1ED) {
            if (func_02038ae0(net->sendData, &net->sendQueue, func_0203d6bc(), net->sendSeq)) {
                sNetSys->unk1ED = TRUE;
            }
        }
        if (sNetSys->unk1ED == TRUE) {
            if (sys->pDevTable->send(sNetSys->sendData, dataSize, 0, NULL)) {
                sNetSys->unk1ED = FALSE;
            }
        }
    }
}

static void func_02038340(void) {
    GFLNetSys *sys;

    func_0203d798();
    sys = func_0203d788();
    if (sys->pDevTable->isConnected() && func_02039160(func_02039338())) {
        func_02037d98(TRUE);
        func_02037e60();
    }
}

static void func_02038380(void) {
    GFLNetSys *sys = func_0203d788();

    func_0203d680();
    if (sSendState == 1 && func_020392b8() > 1) {
        func_02037224(2, 1130);
        if (sys->pDevTable->isConnected()) {
            if (!sys->pDevTable->send(sNetSys->mpSendData, func_02037258(), 0, func_02038a90)) {
                func_02037224(1, 1134);
            }
        } else if (func_0203d798()->bNetType == 3 || func_0203d798()->bNetType == 4) {
            if (!sys->pDevTable->unkA0()) {
                func_02037224(1, 1141);
            } else {
                if (sNetSys->mpSendData[PACKET_SIZE] == 0) {
                    return;
                }
                sys->pDevTable->send(sNetSys->mpSendData, func_0203d6bc(), 100, NULL);
            }
        }
    }
    if (sSendState == 2) {
        func_02038570();
        func_02038924(0, sNetSys->mpSendData, func_02037258());
        func_02037224(3, 1165);
    }
    if (!sys->pDevTable->isConnected() || func_0203d798()->bNetType == 3 || func_0203d798()->bNetType == 4) {
        func_02037224(4, 1170);
    }
}

// Whether no machine has packets that have not come back
static BOOL func_02038524(void) {
    int i;

    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        if (func_02039160(i) && sNetSys->sends[i] > 0) {
            return FALSE;
        }
    }
    return TRUE;
}

static void func_02038570(void) {
    int i;
    int machineMax = func_0203d680();

    for (i = 0; i < machineMax; i++) {
        if (func_02039160(i)) {
            sNetSys->sends[i]++;
        }
    }
}

static void func_020385c4(void) {
    GFLNetSys *sys = func_0203d788();
    GFLNetInitData *pNetIni = func_0203d798();

    if (pNetIni->bNetType == 1 || pNetIni->bNetType == 2) {
        if (!func_02039160(0)) {
            return;
        }
        GFL_ASSERT_AT(1237, !pNetIni->bMPMode);
        if (sSendState == 4 && sNetSys->unk1AC && func_02038524()) {
            func_02037224(1, 1242);
        }
        if (sSendState == 1) {
            if (sys->pDevTable->send(sNetSys->mpSendData, func_02037258(), 0, NULL)) {
                func_02037224(4, 1249);
                func_02038570();
            }
        }
    } else if (pNetIni->bNetType == 3 || pNetIni->bNetType == 4) {
        if (sSendState == 4) {
            NetSystem *net = sNetSys;

            func_02038ae0(net->mpSendData, &net->mpQueue, func_02037258(), net->mpSeq);
            sNetSys->mpSeq++;
            func_02037224(1, 1268);
        }
        func_02038380();
    } else if (sys->pDevTable->isConnected() && func_02038524()) {
        if (sSendState == 4) {
            NetSystem *net = sNetSys;

            func_02038ae0(net->mpSendData, &net->mpQueue, func_02037258(), net->mpSeq);
            sNetSys->mpSeq++;
            func_02037224(1, 1281);
        }
        func_02038380();
    }
}

static BOOL func_020387a0(u16 netId, u8 *data, u16 size) {
    int crc;
    int len;

    func_0203d6bc();
    if (data == NULL) {
        return FALSE;
    }
    sNetSys->sends[netId]--;
    if (netId == 0) {
        sNetSys->connectBits = data[PACKET_CONNECT_BITS];
    }
    if (data[PACKET_FLAGS] == PACKET_FLAGS_NONE) {
        return FALSE;
    }
    if (!sNetSys->recvRings[netId].received && (data[PACKET_FLAGS] & PACKET_FLAG_CONTINUED)) {
        return FALSE;
    }
    crc = getCRC16(data + PACKET_SIZE, size - 2);
    if (data[PACKET_CRC_LO] != (u8)crc || (crc & 0xff00) >> 8 != data[PACKET_CRC_HI]) {
        NET_ERROR(1010);
        return FALSE;
    }
    if (sNetSys->recvSeq[netId] != 0 && (u8)(data[PACKET_SEQ] - 1) != sNetSys->recvSeq[netId]) {
        NET_ERROR(1011);
        return FALSE;
    }
    sNetSys->recvSeq[netId] = data[PACKET_SEQ];
    len = data[PACKET_SIZE];
    if (data[PACKET_FLAGS] & PACKET_FLAG_LONG) {
        len += 0xff;
    }
    func_02036428(&sNetSys->recvRings[netId], data + PACKET_HEADER_SIZE, len);
    sNetSys->recvRings[netId].received = TRUE;
    return TRUE;
}

static BOOL func_02038924(u16 netId, u8 *data, u16 size) {
    int crc;
    int len;

    func_0203d798();
    func_0203d6bc();
    sNetSys->serverSends--;
    if (data == NULL) {
        return FALSE;
    }
    if (netId == 0) {
        sNetSys->connectBits = data[PACKET_CONNECT_BITS];
    }
    if (data[PACKET_FLAGS] == PACKET_FLAGS_NONE) {
        return FALSE;
    }
    if (!sNetSys->serverRing.received && (data[PACKET_FLAGS] & PACKET_FLAG_CONTINUED)) {
        return FALSE;
    }
    crc = getCRC16(data + PACKET_SIZE, size - 2);
    if (data[PACKET_CRC_LO] != (u8)crc || (crc & 0xff00) >> 8 != data[PACKET_CRC_HI]) {
        NET_ERROR(1010);
        return FALSE;
    }
    sNetSys->serverSeq = data[PACKET_SEQ];
    len = data[PACKET_SIZE];
    if (data[PACKET_FLAGS] & PACKET_FLAG_LONG) {
        len += 0xff;
    }
    func_02036428(&sNetSys->serverRing, data + PACKET_HEADER_SIZE, len);
    sNetSys->serverRing.received = TRUE;
    return TRUE;
}

static BOOL func_02038a48(BOOL ok) {
    if (ok && sRecvState == 2) {
        sRecvState = 4;
    } else {
        GFL_ASSERT_AT(1472, 0);
    }
    return TRUE;
}

static BOOL func_02038a90(BOOL ok) {
    func_0203d6bc();
    func_0203d680();
    func_0203d798();
    if (ok) {
        func_02037224(4, 1492);
    } else {
        GFL_ASSERT_AT(1496, 0);
    }
    return TRUE;
}

static BOOL func_02038ae0(u8 *packet, NetQueue *queue, int size, u8 seq) {
    GFLNetInitData *ini = func_0203d798();
    GFLNetSys *sys = func_0203d788();
    NetSendBuffer buffer;
    int len;
    int crc;

    MI_CpuFill8(packet, 0, size);
    packet[PACKET_FLAGS] = queue->pending == FALSE ? PACKET_FLAG_FIRST : PACKET_FLAG_CONTINUED;
    queue->pending = FALSE;
    packet[PACKET_CONNECT_BITS] = sys->pDevTable->getConnectBits();
    if (func_020366a8(queue) || ((ini->bNetType == 3 || ini->bNetType == 4) && sys->pDevTable->unkB0() == TRUE)) {
        packet[PACKET_FLAGS] = PACKET_FLAGS_NONE;
        return FALSE;
    }
    buffer.size = size - PACKET_HEADER_SIZE;
    buffer.ptr = packet + PACKET_HEADER_SIZE;
    if (!func_02036a50(queue, &buffer)) {
        queue->pending = TRUE;
    }
    if (buffer.size != 0xffff) {
        len = size - buffer.size - PACKET_HEADER_SIZE;
    } else {
        len = size - PACKET_HEADER_SIZE;
    }
    if (len > 0xff) {
        packet[PACKET_FLAGS] |= PACKET_FLAG_LONG;
        // The low byte of len - 0xff
        len++;
    }
    packet[PACKET_SIZE] = len;
    packet[PACKET_SEQ] = seq;
    crc = getCRC16(packet + PACKET_SIZE, size - 2);
    packet[PACKET_CRC_LO] = crc;
    packet[PACKET_CRC_HI] = (crc & 0xff00) >> 8;
    return TRUE;
}

// Queues a command to send to a machine, or every machine for GFL_NET_NETID_SERVER
BOOL func_02038c18(int command, u8 *data, int size, u32 unused, int dest, int netId, BOOL noCopy) {
    GFLNetInitData *ini;
    BOOL copy = TRUE;
    NetQueue *queue;

    if (noCopy) {
        copy = FALSE;
    }
    ini = func_0203d798();
    if (sNetSys == NULL) {
        return FALSE;
    }
    if (func_02039414() || func_0203c990()) {
        return FALSE;
    }
    if (!func_02039594()) {
        return FALSE;
    }
    if (size >= sNetSys->recvSize && copy == TRUE) {
        copy = FALSE;
    }
    if (ini->bMPMode && dest == GFL_NET_NETID_SERVER) {
        queue = &sNetSys->mpQueue;
    } else {
        queue = &sNetSys->sendQueue;
    }
    if (func_020368c0(queue, command, data, size, unused, copy, netId)) {
        return TRUE;
    }
    NET_ERROR(1012);
    return FALSE;
}

int func_02038d30(void) {
    return func_020365e8(&sNetSys->sendRing);
}

int func_02038d4c(void) {
    return func_020365e8(&sNetSys->mpRing);
}

// Hands a command that has arrived to its handler, and for the server to the server's handler too
static void func_02038d68(int command, int size, void *data, NetRecvState *state) {
    NetHandle *handle;

    func_0203d798();
    handle = func_0203996c(func_02039338());
    func_0203a770(state->netId, state->sender, command, size, data, handle);
    if (func_02039338() == 0) {
        handle = func_0203996c(GFL_NET_NETID_SERVER);
        if (command < 0x11) {
            func_0203a770(state->netId, state->sender, command, size, data, handle);
        }
    }
    func_02037234(state);
}

// Reads the commands that have arrived in a ring buffer
static void func_02038df0(NetRingBuff *ring, int netId, u8 *temp, NetRecvState *state, int size) {
    int read;
    u8 *dest;
    int command;
    int copied;
    int start;
    int savedStart;

    func_0203d798();
    if (func_020365e8(ring) == 0) {
        return;
    }
    do {
        command = state->command;
        if (command == 0) {
            command = func_02036540(ring);
            if (command == 0) {
                GFL_ASSERT_AT(1763, 0);
                continue;
            }
        }
        savedStart = ring->startPos;
        state->command = command;
        if (state->size == 0xffff) {
            if (func_020365e8(ring) < 3) {
                ring->startPos = savedStart;
                return;
            }
            u16 commandSize = func_02036540(ring);

            state->remaining = commandSize;
            state->size = commandSize;
            state->netId = netId;
            state->sender = func_02036500(ring);
        }
        start = ring->startPos;
        if (func_0203a804(command) && func_0203a634(state->sender)) {
            if (state->buffer == NULL) {
                state->buffer = func_0203a81c(command, state->netId, state->size);
            }
            dest = state->buffer + state->pos;
        } else {
            state->buffer = temp;
            dest = temp;
        }
        if (!func_0203a634(state->sender)) {
            dest = NULL;
        }
        if (!func_0203a804(command)) {
            read = func_020364d4(ring, temp, state->remaining, state->remaining);
        } else {
            read = func_020364d4(ring, temp, state->remaining, size - PACKET_HEADER_SIZE);
        }
        state->remaining -= (u16)read;
        copied = read;
        if (state->size > state->pos) {
            copied = state->size - state->pos;
            if (copied > read) {
                copied = read;
            }
            if (dest != NULL) {
                MI_CpuCopy8(temp, dest, copied);
            }
        }
        state->pos += copied;
        if (state->remaining == 0) {
            func_02038d68(command, state->size, state->buffer, state);
            continue;
        }
        if (func_0203a7ec(command) && !func_0203a804(command) && func_0203a634(state->sender)) {
            state->remaining += (u16)read;
            ring->startPos = start;
        }
        return;
    } while (func_020365e8(ring) != 0);
}

static void func_02039050(void) {
    int i;

    if (sNetSys != NULL) {
        func_0203666c(&sNetSys->serverRing);
        for (i = 0; i < 4; i++) {
            NetSystem *net;

            if (func_020365e8(&sNetSys->serverRing) <= 0) {
                break;
            }
            net = sNetSys;
            func_02038df0(&net->serverRing, GFL_NET_NETID_SERVER, net->recvTemp, &net->serverRecvState,
                func_02037258());
        }
    }
}

static void func_020390bc(void) {
    int i;
    int j;
    int machineMax;
    int dataSize = func_0203d6bc();

    if (sNetSys != NULL) {
        machineMax = func_0203d680();

        for (i = 0; i < machineMax; i++) {
            func_0203666c(&sNetSys->recvRings[i]);
            for (j = 0; j < 4; j++) {
                if (func_020365e8(&sNetSys->recvRings[i]) > 0) {
                    NetSystem *net = sNetSys;

                    func_02038df0(&net->recvRings[i], i, net->recvTemp, &net->recvStates[i], dataSize);
                }
            }
        }
    }
}

BOOL func_02039160(u16 netId) {
    GFLNetSys *sys = func_0203d788();
    GFLNetInitData *ini = func_0203d798();

    if (sNetSys == NULL) {
        return FALSE;
    }
    if (ini->bNetType == 1 || ini->bNetType == 2) {
        if (sNetSys->unk1E7 && sys->pDevTable->getNetId() != -1) {
            if (netId == 0) {
                return TRUE;
            }
            if (netId == 1) {
                return TRUE;
            }
        }
        return FALSE;
    }
    if (ini->bNetType == 3 || ini->bNetType == 4) {
        if ((netId < 2 || netId == GFL_NET_NETID_SERVER) && sys->pDevTable->isConnected()) {
            return TRUE;
        }
        return FALSE;
    }
    if (!func_020392ec()) {
        return FALSE;
    }
    if (!sys->pDevTable->isConnected()) {
        return FALSE;
    }
    if (netId == func_02039338()) {
        return TRUE;
    }
    if (func_02039338() == 0 || !ini->bMPMode) {
        u16 bits = sys->pDevTable->getConnectBits();

        if ((1 << netId) & bits) {
            return TRUE;
        }
    } else {
        u16 bits = sNetSys->connectBits;

        if (bits & (1 << netId)) {
            return TRUE;
        }
    }
    return FALSE;
}

int func_020392b8(void) {
    int count = 0;
    int i;

    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        if (func_02039160(i)) {
            count++;
        }
    }
    return count;
}

BOOL func_020392ec(void) {
    GFLNetSys *sys = func_0203d788();
    GFLNetInitData *ini = func_0203d798();

    if (sNetSys != NULL && (ini->bNetType == 1 || ini->bNetType == 2 || ini->bNetType == 3 || ini->bNetType == 4)) {
        return TRUE;
    }
    return sys->pDevTable->unk6C();
}

u16 func_02039338(void) {
    if (sNetSys != NULL) {
        GFLNetSys *sys = func_0203d788();
        GFLNetInitData *ini = func_0203d798();

        if (ini->bNetType == 1 || ini->bNetType == 2) {
            int netId = sys->pDevTable->getNetId();

            if (netId != -1) {
                return netId;
            }
        } else {
            return sys->pDevTable->getNetId();
        }
    }
    return 0;
}

u32 func_020393a8(void) {
    GFLNetSys *sys = func_0203d788();

    return (u16)sys->pDevTable->getConnectBits() & 0xfffe;
}

BOOL func_020393d0(void) {
    GFLNetSys *sys = func_0203d788();

    if (sNetSys != NULL && sNetSys->error) {
        return TRUE;
    }
    if (sys->pDevTable->isError()) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_02039414(void) {
    GFLNetSys *sys = func_0203d788();
    BOOL error = FALSE;

    if (sys != NULL && sys->pDevTable != NULL) {
        error = sys->pDevTable->isError();
    }
    if (!error && sNetSys != NULL && sNetSys->error) {
        error = TRUE;
    }
    return error;
}

BOOL func_02039468(u16 command, int dest) {
    if (func_0203d798()->bMPMode && dest == GFL_NET_NETID_SERVER) {
        return func_02036b98(&sNetSys->mpQueue, command);
    }
    return func_02036b98(&sNetSys->sendQueue, command);
}

BOOL func_020394c0(void) {
    return func_020366a8(&sNetSys->sendQueue);
}

void func_020394dc(u8 a0) {
    sNetSys->unk1E7 = a0;
}

void func_020394f0(BOOL a0) {
    GFLNetInitData *ini = func_0203d798();

    if (ini->bNetType == 1 || ini->bNetType == 2) {
        sNetSys->unk1AC = a0;
        if (a0) {
            sNetSys->serverSends = 0;
            sNetSys->sends[0] = 0;
            sNetSys->sends[1] = 0;
        }
    }
}

void func_02039548(void) {
    sNetSys->error = TRUE;
}

void func_02039560(void) {
    sNetSys->error = FALSE;
}

void func_02039578(void) {
    if (sNetSys != NULL) {
        sNetSys->unk1EA = TRUE;
    }
}

BOOL func_02039594(void) {
    if (sNetSys != NULL) {
        GFLNetInitData *ini = func_0203d798();

        if (!ini->bMPMode && (ini->bNetType == 0 || ini->bNetType == 5)) {
            return sNetSys->unk1EB;
        }
    }
    return TRUE;
}

int func_020395e0(int netId) {
    if (netId >= GFL_NET_MACHINE_MAX) {
        netId = GFL_NET_NETID_SERVER;
    }
    return netId;
}
