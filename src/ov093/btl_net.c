// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btl_net.c, ported by tools/port_file.py.
#include "types.h"
#include "battle/btl_main.h"
#include "battle/btl_net.h"
#include "battle/btl_setup.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "pml/poke_party.h"
#include "save/chatter.h"
#include "save/player_info.h"
#include "system/str_tool.h"
#include "nitro/mi.h"

// What a command that arrives whole is received into
// Black 2's; twinned code confirms the offsets of size and empty in Black
typedef struct {
    u8 data[0xbb8];
    u16 size;
    // 1 until data arrives
    u16 empty;
} BtlNetRecvBuffer;

// Command 0x100: each machine's version, the highest of which becomes the server
typedef struct {
    u16 version;
    u16 clientId;
} BtlNetVersionPacket;

typedef struct {
    u8 version;
    u8 received;
} BtlNetClientVersion;

// The versions received, by net id
typedef struct {
    BtlNetClientVersion clients[4];
    u8 count;
    u8 max;
} BtlNetVersions;

// Command 0x104: a party sent for one client, the header then the data
typedef struct {
    // 4 for none
    u32 clientId : 8;
    u32 size : 24;
    u8 data[];
} BtlNetPartyPacket;

// Black 2's; twinned code confirms the offsets of netHandle, heapId, myNetId, serverNetId, clientMask,
// syncDataReceived, numClients, serverCmdReceived, unkE, timingNo, timingPending, clientIds, syncData, serverCmd,
// clientReplies, recvBuffers, recvSizes, parties, tempBuffer, tempSize, sendBuf and versions in Black
typedef struct {
    NetHandle *netHandle;
    HeapID heapId;
    u8 myNetId;
    // 40 until decided
    u8 serverNetId;
    u16 clientMask;
    u8 syncDataReceived;
    u8 numClients;
    u8 unkC;
    u8 serverCmdReceived;
    u8 unkE;
    u8 timingNo;
    u8 timingPending;
    // By net id, 4 for none
    u8 clientIds[4];
    BtlMainSyncData syncData;
    BtlNetRecvBuffer serverCmd;
    // By net id
    BtlNetRecvBuffer clientReplies[4];
    // By net id
    void *recvBuffers[4];
    u32 recvSizes[4];
    // By client id
    PokeParty *parties[4];
    void *tempBuffer;
    u32 tempSize;
    union {
        BtlNetVersionPacket version;
        u32 noChatter;
    } sendBuf;
    BtlNetVersions versions;
} BtlNetWork;

static BtlNetWork *sWork;

void func_ov093_021d42bc(int netId, int size, const void *data, void *work, NetHandle *handle);
static BOOL func_ov093_021d4330(int netId);
static u8 func_ov093_021d4394(u8 clientId);
void func_ov093_021d43e0(int netId, int size, const void *data, void *work, NetHandle *handle);
void *func_ov093_021d446c(int netId, void *work, int size);
void func_ov093_021d44b4(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov093_021d4cec(void);
static void func_ov093_021d4a6c(void);
void *func_ov093_021d4558(int netId, void *work, int size);
void func_ov093_021d45b8(int netId, int size, const void *data, void *work, NetHandle *handle);
void *func_ov093_021d46b0(int netId, void *work, int size);
void func_ov093_021d46fc(int netId, int size, const void *data, void *work, NetHandle *handle);
void *func_ov093_021d4838(int netId, void *work, int size);
void func_ov093_021d4880(int netId, int size, const void *data, void *work, NetHandle *handle);
void *func_ov093_021d4954(int netId, void *work, int size);
void func_ov093_021d499c(int netId, int size, const void *data, void *work, NetHandle *handle);
static BtlCommTrainerData *func_ov093_021d4a0c(const BtlSetupTrainer *trainer, HeapID heapId);
static void func_ov093_021d4a64(void *data);
void *func_ov093_021d4bfc(int netId, void *work, int size);
void func_ov093_021d4c14(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov093_021d4cb8(int netId, int size, const void *data, void *work, NetHandle *handle);
void *func_ov093_021d4ce0(int netId, void *work, int size);

// Empties the receive buffers that func_ov093_021d4a6c frees, and forgets their clients
static inline void ClearRecvBuffers(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        sWork->recvBuffers[i] = NULL;
        sWork->recvSizes[i] = 0;
        sWork->clientIds[i] = 4;
    }
    for (i = 0; i < 4; i++) {
        sWork->parties[i] = NULL;
    }
    sWork->tempBuffer = NULL;
    sWork->tempSize = 0;
}

void func_ov093_021d4150(NetHandle *netHandle, u16 clientMask, HeapID heapId) {
    u32 count;
    u32 max;
    u32 i;

    if (netHandle != NULL) {
        sWork = GFL_HeapAllocate(heapId, sizeof(BtlNetWork), TRUE, "btl_net.c", 212);
        sWork->netHandle = netHandle;
        sWork->clientMask = clientMask;
        sWork->heapId = heapId;
        sWork->myNetId = func_0203d1a4(netHandle);
        sWork->serverNetId = 40;
        sWork->serverCmdReceived = FALSE;
        sWork->unkE = 0;
        for (i = 0, count = 0; i < 4; i++) {
            if ((1 << i) & clientMask) {
                count++;
            }
        }
        max = func_0203d1b4();
        if (count > max) {
            count = max;
        }
        sWork->numClients = count;
        sWork->timingNo = 0;
        sWork->timingPending = FALSE;
        sWork->syncDataReceived = FALSE;
        ClearRecvBuffers();
        func_ov093_021d4af4(1);
    } else {
        sWork = NULL;
    }
}

BOOL func_ov093_021d4230(void) {
    if (sWork != NULL) {
        return func_ov093_021d4b18(1) ? TRUE : FALSE;
    }
    return TRUE;
}

void func_ov093_021d4254(void) {
    if (sWork != NULL) {
        func_ov093_021d4a6c();
        GFL_HeapFree(sWork);
    }
}

BOOL func_ov093_021d4270(void) {
    if (sWork != NULL) {
        if (func_0200fb24()) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

// Sends this machine's version and client
BOOL func_ov093_021d4294(u8 clientId) {
    BtlNetVersionPacket *packet = &sWork->sendBuf.version;

    packet->version = 100;
    packet->clientId = clientId;
    return func_0203d3a8(sWork->netHandle, 0x100, sizeof(BtlNetVersionPacket), packet);
}

// Command 0x100: records a machine's version, and once every machine's is in, makes the highest the server
void func_ov093_021d42bc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const BtlNetVersionPacket *packet = data;
    BtlNetVersions *versions = &sWork->versions;
    int i;

    if (!versions->clients[netId].received) {
        u16 version = packet->version;

        sWork->clientIds[netId] = packet->clientId;
        versions->clients[netId].version = version;
        versions->clients[netId].received = TRUE;
        if (versions->max < version) {
            versions->max = version;
        }
        versions->count++;
        if (versions->count >= sWork->numClients) {
            sWork->serverNetId = 40;
            for (i = 0; i < 4; i++) {
                if (versions->clients[i].received && versions->max == versions->clients[i].version) {
                    sWork->serverNetId = i;
                    return;
                }
            }
        }
    }
}

static BOOL func_ov093_021d4330(int netId) {
    return sWork->versions.clients[netId].received;
}

// Whether the server is decided
BOOL func_ov093_021d4348(void) {
    if (sWork->serverNetId != 40) {
        return TRUE;
    }
    return FALSE;
}

// Whether this machine is the server
BOOL func_ov093_021d4360(void) {
    if (sWork->serverNetId == sWork->myNetId) {
        return TRUE;
    }
    return FALSE;
}

// The highest version, which every machine plays by
u8 func_ov093_021d4378(void) {
    if (sWork != NULL) {
        return sWork->versions.max;
    }
    return 100;
}

// The net id of a client, or 40
static u8 func_ov093_021d4394(u8 clientId) {
    int i;

    for (i = 0; i < 4; i++) {
        if (sWork->clientIds[i] == clientId) {
            return i;
        }
    }
    return 40;
}

// Sends the server's sync data to everyone
BOOL func_ov093_021d43b8(BtlMainSyncData *data) {
    return func_0203d3f8(sWork->netHandle, 0xff, 0x101, sizeof(BtlMainSyncData), data, 0, 0, 0);
}

// Command 0x101: keeps the first sync data that arrives
void func_ov093_021d43e0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (!sWork->syncDataReceived) {
        sWork->syncData = *(const BtlMainSyncData *)data;
        sWork->syncDataReceived = TRUE;
    }
}

// Copies the sync data once it has arrived
BOOL func_ov093_021d440c(BtlMainSyncData *data) {
    if (sWork->syncDataReceived) {
        *data = sWork->syncData;
        return TRUE;
    }
    return FALSE;
}

// Sends this machine's party to everyone
u8 func_ov093_021d4438(PokeParty *party) {
    return func_0203d3f8(sWork->netHandle, 0xff, 0x102, PokeParty_GetSaveDataSize(), party, 0, 1, 1);
}

// Command 0x102's buffer
void *func_ov093_021d446c(int netId, void *work, int size) {
    sWork->recvBuffers[netId] = GFL_HeapAllocate(HEAPID_TAIL(sWork->heapId), size, TRUE, "btl_net.c", 462);
    return sWork->recvBuffers[netId];
}

// Command 0x102: a party has arrived
void func_ov093_021d44b4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    sWork->recvSizes[netId] = size;
}

// Whether every machine's data has arrived
BOOL func_ov093_021d44cc(void) {
    int i;

    for (i = 0; i < 4; i++) {
        if (func_ov093_021d4330(i) && sWork->recvSizes[i] == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

void func_ov093_021d4500(void) {
    func_ov093_021d4cec();
    func_ov093_021d4a6c();
}

// Sends this machine's Chatot cry to everyone, or a word of zero without one
BOOL func_ov093_021d450c(void *chatter) {
    u32 size;

    if (doesChatotExist(chatter)) {
        size = func_02007b00();
    } else {
        size = sizeof(u32);
        chatter = &sWork->sendBuf.noChatter;
        sWork->sendBuf.noChatter = 0;
    }
    return func_0203d3f8(sWork->netHandle, 0xff, 0x103, size, chatter, 0, 1, 1);
}

// Command 0x103's buffer: a word for no cry, which goes in the machine's reply buffer
void *func_ov093_021d4558(int netId, void *work, int size) {
    if (size == sizeof(u32)) {
        return &sWork->clientReplies[netId];
    }
    sWork->recvBuffers[netId] = GFL_HeapAllocate(HEAPID_TAIL(sWork->heapId), size, TRUE, "btl_net.c", 543);
    return sWork->recvBuffers[netId];
}

// Command 0x103: a Chatot cry has arrived
void func_ov093_021d45b8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (size == sizeof(u32)) {
        BtlNetRecvBuffer *buffer = &sWork->clientReplies[netId];

        buffer->size = size;
        buffer->empty = FALSE;
    } else {
        sWork->recvSizes[netId] = size;
    }
}

// Whether every machine's Chatot cry has arrived
BOOL func_ov093_021d45fc(void) {
    int i;

    for (i = 0; i < 4; i++) {
        if (func_ov093_021d4330(i) && sWork->clientReplies[i].empty && sWork->recvSizes[i] == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

// A client's Chatot cry, or NULL
void *func_ov093_021d4648(u8 clientId) {
    u8 netId = func_ov093_021d4394(clientId);

    if (netId != 40 && sWork->recvSizes[netId] != 0) {
        return sWork->recvBuffers[netId];
    }
    return NULL;
}

void func_ov093_021d4674(void) {
    func_ov093_021d4cec();
    func_ov093_021d4a6c();
}

// Sends a party for one client to everyone
BOOL func_ov093_021d4680(void *data) {
    BtlNetPartyPacket *packet = data;

    return func_0203d3f8(sWork->netHandle, 0xff, 0x104, packet->size + sizeof(BtlNetPartyPacket), packet, 0, 1, 1);
}

// Command 0x104's buffer
void *func_ov093_021d46b0(int netId, void *work, int size) {
    if (sWork->tempBuffer == NULL) {
        sWork->tempBuffer = GFL_HeapAllocate(HEAPID_TAIL(sWork->heapId), size, TRUE, "btl_net.c", 633);
    }
    return sWork->tempBuffer;
}

// Command 0x104: keeps the party for its client
void func_ov093_021d46fc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const BtlNetPartyPacket *packet = data;

    if (sWork->parties[packet->clientId] == NULL) {
        sWork->parties[packet->clientId] =
            GFL_HeapAllocate(HEAPID_TAIL(sWork->heapId), packet->size, TRUE, "btl_net.c", 642);
    }
    MI_CpuCopy8(packet->data, sWork->parties[packet->clientId], packet->size);
    GFL_HeapFree(sWork->tempBuffer);
    sWork->tempBuffer = NULL;
}

// Whether a client's party has arrived
BOOL func_ov093_021d4784(u8 clientId) {
    if (sWork->parties[clientId] != NULL) {
        return TRUE;
    }
    return FALSE;
}

// A client's party, from its machine or sent for it
PokeParty *func_ov093_021d47a4(u8 clientId) {
    u8 netId = func_ov093_021d4394(clientId);
    PokeParty *party;
    u32 count;
    u32 i;

    if (netId != 40) {
        return sWork->recvBuffers[netId];
    }
    party = sWork->parties[clientId];
    if (party != NULL) {
        count = PokeParty_GetPkmCount(party);
        for (i = 0; i < count; i++) {
            PokeParty_GetPkm(party, i);
        }
        return party;
    }
    return NULL;
}

void func_ov093_021d4800(void) {
    func_ov093_021d4a6c();
}

// Sends this machine's player info to everyone
BOOL func_ov093_021d4808(PlayerInfo *info) {
    return func_0203d3f8(sWork->netHandle, 0xff, 0x105, func_02008468(), info, 0, 1, 1);
}

// Command 0x105's buffer
void *func_ov093_021d4838(int netId, void *work, int size) {
    sWork->recvBuffers[netId] = GFL_HeapAllocate(HEAPID_TAIL(sWork->heapId), size, TRUE, "btl_net.c", 724);
    return sWork->recvBuffers[netId];
}

// Command 0x105: a machine's player info has arrived
void func_ov093_021d4880(int netId, int size, const void *data, void *work, NetHandle *handle) {
    sWork->recvSizes[netId] = size;
}

// Whether every machine's player info has arrived
BOOL func_ov093_021d4898(void) {
    int i;

    for (i = 0; i < 4; i++) {
        if (func_ov093_021d4330(i) && sWork->recvSizes[i] == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

// A client's player info, from its machine or sent for it
PlayerInfo *func_ov093_021d48cc(u8 clientId) {
    u8 netId = func_ov093_021d4394(clientId);

    if (netId != 40) {
        return sWork->recvBuffers[netId];
    }
    if (sWork->tempBuffer != NULL && sWork->tempSize != 0) {
        return sWork->tempBuffer;
    }
    return NULL;
}

// Sends this machine's trainer to everyone
BOOL func_ov093_021d4908(BtlSetupTrainer *trainer) {
    BtlCommTrainerData *data = func_ov093_021d4a0c(trainer, HEAPID_TAIL(sWork->heapId));
    BOOL result = func_0203d3f8(sWork->netHandle, 0xff, 0x106, sizeof(BtlCommTrainerData), data, 0, 1, 0);

    func_ov093_021d4a64(data);
    return result;
}

// Command 0x106's buffer
void *func_ov093_021d4954(int netId, void *work, int size) {
    sWork->tempBuffer = GFL_HeapAllocate(HEAPID_TAIL(sWork->heapId), size, TRUE, "btl_net.c", 798);
    return sWork->tempBuffer;
}

// Command 0x106: a trainer has arrived
void func_ov093_021d499c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    sWork->tempSize = size;
}

// Whether the trainer has arrived
BOOL func_ov093_021d49b0(void) {
    if (sWork->tempSize != 0) {
        return TRUE;
    }
    return FALSE;
}

void *func_ov093_021d49cc(void) {
    return sWork->tempBuffer;
}

void func_ov093_021d49e0(void) {
    GFL_HeapFree(sWork->tempBuffer);
    sWork->tempBuffer = NULL;
    sWork->tempSize = 0;
}

void func_ov093_021d4a04(void) {
    func_ov093_021d4a6c();
}

// A trainer as a link battle sends it, with its name copied out of the string buffer
static BtlCommTrainerData *func_ov093_021d4a0c(const BtlSetupTrainer *trainer, HeapID heapId) {
    BtlCommTrainerData *data = GFL_HeapAllocate(heapId, sizeof(BtlCommTrainerData), TRUE, "btl_net.c", 841);

    MI_CpuCopy8(trainer, data, sizeof(BtlSetupTrainer));
    MIi_CpuClear16(GFL_StrBufGetTerminator(), data->name, sizeof(data->name));
    if (trainer->name != NULL) {
        data->nameLength = GFL_StrBufGetCharCount(trainer->name);
        wcharsncpy(GFL_StrBufGetStringPtr(trainer->name), data->name, NELEMS(data->name));
    }
    return data;
}

static void func_ov093_021d4a64(void *data) {
    GFL_HeapFree(data);
}

static void func_ov093_021d4a6c(void) {
    int i;

    for (i = 0; i < 4; i++) {
        if (sWork->recvBuffers[i] != NULL) {
            GFL_HeapFree(sWork->recvBuffers[i]);
            sWork->recvBuffers[i] = NULL;
            sWork->recvSizes[i] = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        if (sWork->parties[i] != NULL) {
            GFL_HeapFree(sWork->parties[i]);
            sWork->parties[i] = NULL;
        }
    }
    if (sWork->tempBuffer != NULL) {
        GFL_HeapFree(sWork->tempBuffer);
        sWork->tempBuffer = NULL;
        sWork->tempSize = 0;
    }
}

// Starts synchronizing every machine on a timing number
void func_ov093_021d4af4(u8 id) {
    if (sWork != NULL) {
        sWork->timingNo = id;
        sWork->timingPending = TRUE;
        func_0203a424(sWork->netHandle, id, 1, sWork->clientMask);
    }
}

// Whether every machine has reached the timing number, starting the wait if it isn't waiting
// Whether every machine has reached the timing number, starting the wait if it isn't waiting
BOOL func_ov093_021d4b18(u8 id) {
    if (sWork != NULL) {
        if (sWork->timingPending) {
            BOOL done = func_02039cb4(sWork->netHandle, sWork->timingNo, 1);

            if (done) {
                sWork->timingNo = 0;
                sWork->timingPending = FALSE;
                return done;
            }
        } else {
            func_0203a424(sWork->netHandle, id, 1, sWork->clientMask);
        }
        return FALSE;
    }
    return TRUE;
}

// Sends a server command to a client's machine
BOOL func_ov093_021d4b58(u8 clientId, void *data, u32 size) {
    u8 netId = func_ov093_021d4394(clientId);

    if (netId != 40) {
        return func_0203d3f8(sWork->netHandle, netId, 0x107, size, data, 0, 0, 1);
    }
    return TRUE;
}

// Whether every machine's reply has arrived
BOOL func_ov093_021d4b98(void) {
    int i;

    for (i = 0; i < 4; i++) {
        if (func_ov093_021d4330(i) && sWork->clientReplies[i].empty) {
            return FALSE;
        }
    }
    return TRUE;
}

// A client's reply and its size
u16 func_ov093_021d4bd4(u8 clientId, void **data) {
    u8 netId = func_ov093_021d4394(clientId);
    BtlNetRecvBuffer *buffer = &sWork->clientReplies[netId];

    *data = buffer;
    return buffer->size;
}

// Command 0x108's buffer
void *func_ov093_021d4bfc(int netId, void *work, int size) {
    return &sWork->clientReplies[netId];
}

// Command 0x108: a client's reply has arrived
void func_ov093_021d4c14(int netId, int size, const void *data, void *work, NetHandle *handle) {
    BtlNetRecvBuffer *buffer = &sWork->clientReplies[netId];

    buffer->size = size;
    buffer->empty = FALSE;
    sWork->clientReplies[netId].size = size;
    sWork->clientReplies[netId].empty = FALSE;
}

void func_ov093_021d4c58(void) {
    func_ov093_021d4cec();
}

// Whether the server's command has arrived
BOOL func_ov093_021d4c60(void) {
    return sWork->serverCmdReceived;
}

// The server's command and its size
u16 func_ov093_021d4c6c(void **data) {
    BtlNetRecvBuffer *buffer = &sWork->serverCmd;

    *data = buffer;
    return buffer->size;
}

// Sends this machine's reply to the server
BOOL func_ov093_021d4c84(void *data, u32 size) {
    BOOL result = func_0203d3f8(sWork->netHandle, sWork->serverNetId, 0x108, size, data, 0, 1, 1);

    if (result) {
        sWork->serverCmdReceived = FALSE;
    }
    return result;
}

// Command 0x107: the server's command has arrived
void func_ov093_021d4cb8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (!sWork->serverCmdReceived) {
        BtlNetRecvBuffer *buffer = &sWork->serverCmd;

        buffer->size = size;
        buffer->empty = FALSE;
        sWork->serverCmdReceived = TRUE;
    }
}

// Command 0x107's buffer
void *func_ov093_021d4ce0(int netId, void *work, int size) {
    return &sWork->serverCmd;
}

static void func_ov093_021d4cec(void) {
    int i;

    for (i = 0; i < 4; i++) {
        BtlNetRecvBuffer *buffer = &sWork->clientReplies[i];

        buffer->size = 0;
        buffer->empty = TRUE;
    }
}

// A party packet of a size, for no client yet
void *func_ov093_021d4d24(u32 size, HeapID heapId) {
    BtlNetPartyPacket *packet = GFL_HeapAllocate(heapId, size + sizeof(BtlNetPartyPacket), TRUE, "btl_net.c", 1143);

    packet->size = size;
    packet->clientId = 4;
    return packet;
}

void func_ov093_021d4d5c(void *data) {
    GFL_HeapFree(data);
}

// Puts a client's party in a party packet
void func_ov093_021d4d64(void *data, PokeParty *party, u8 clientId) {
    BtlNetPartyPacket *packet = data;

    packet->clientId = clientId;
    MI_CpuCopy8(party, packet->data, packet->size);
}


// The link battle's commands, from 0x100
const NetCommand data_ov093_021f03c4[9] = {
    { func_ov093_021d42bc, NULL },
    { func_ov093_021d43e0, NULL },
    { func_ov093_021d44b4, func_ov093_021d446c },
    { func_ov093_021d45b8, func_ov093_021d4558 },
    { func_ov093_021d46fc, func_ov093_021d46b0 },
    { func_ov093_021d4880, func_ov093_021d4838 },
    { func_ov093_021d499c, func_ov093_021d4954 },
    { func_ov093_021d4cb8, func_ov093_021d4ce0 },
    { func_ov093_021d4c14, func_ov093_021d4bfc },
};
