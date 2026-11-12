// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/net_handle.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/net_irc_wireless.h"
#include "gfl/net_system.h"
#include "gfl/std.h"
#include "nitro/os.h"

// The timing numbers the server relays
typedef struct {
    u16 timings[GFL_NET_MACHINE_MAX];
    // Who each timing number is for
    u8 timingSend[GFL_NET_MACHINE_MAX];
    // Whether the server has each timing number to relay
    u8 relayPending[GFL_NET_MACHINE_MAX];
} NetHandleBaseWork;

typedef struct {
    u16 timing;
    u8 netIds;
} NetTimingData;

void func_02039658(NetHandle *handle, int netId);
void func_0203971c(NetHandle *handle, int netId);
static NetHandle *func_02039764(int netId);
static void func_02039a5c(void);
static BOOL func_02039cd8(NetHandle *handle);
void func_0203a118(NetHandle *handle);
static u16 func_0203a28c(u16 bits, int netID, BOOL set);
BOOL func_0203a2f0(u16 bits, int netID);

// pokebw2's _pBaseWork
static NetHandleBaseWork *_pBaseWork;


int func_020395ec(NetHandle *handle) {
    int i;

    func_0203d788();
    if (func_0203996c(GFL_NET_NETID_SERVER) == handle) {
        return GFL_NET_NETID_SERVER;
    }
    for (i = 0; i < GFL_NET_HANDLE_MAX; i++) {
        if (func_0203996c(i) == handle) {
            return i;
        }
    }
    GFL_ASSERT_AT(110, 0);
    return -1;
}

void func_02039658(NetHandle *handle, int netId) {
    NetNegotiation *negotiations[GFL_NET_HANDLE_MAX];
    int i;

    if (netId != GFL_NET_NETID_SERVER) {
        handle->negotiationsSent = 0;
        handle->serverNegotiationsSent = func_0203a28c(handle->serverNegotiationsSent, netId, FALSE);
    }
    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        if (handle->negotiations[i] != NULL) {
            MI_CpuFill8(handle->negotiations[i], 0, sizeof(NetNegotiation));
        }
        negotiations[i] = handle->negotiations[i];
    }
    if (handle->data != NULL) {
        GFL_HeapFree(handle->data);
        handle->data = NULL;
    }
    MI_CpuFill8(handle, 0, sizeof(NetHandle));
    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        handle->negotiations[i] = negotiations[i];
    }
    func_02037be8(netId);
}


void func_0203971c(NetHandle *handle, int netId) {
    if (netId != GFL_NET_NETID_SERVER) {
        handle->negotiationsSent = 0;
        handle->serverNegotiationsSent = func_0203a28c(handle->serverNegotiationsSent, netId, FALSE);
    }
    if (handle->negotiations[netId] != NULL) {
        MI_CpuFill8(handle->negotiations[netId], 0, sizeof(NetNegotiation));
    }
}

static NetHandle *func_02039764(int netId) {
    NetHandle *handle;
    GFLNetInitData *ini;
    int i;

    ini = func_0203d798();
    func_0203d788();
    handle = func_0203996c(netId);
    if (netId == GFL_NET_NETID_SERVER) {
        for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
            if (handle->negotiations[i] != NULL) {
                GFL_HeapFree(handle->negotiations[i]);
            }
        }
    }
    MI_CpuFill8(handle, 0, sizeof(NetHandle));
    if (netId == GFL_NET_NETID_SERVER) {
        for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
            handle->negotiations[i] = GFL_HeapAllocate(ini->heapId, sizeof(NetNegotiation), TRUE, "net_handle.c", 199);
        }
    }
    handle->state = NET_HANDLE_STATE_NONE;
    handle->timingRecv = 0xff;
    handle->timingSend = 0xff;
    handle->timingSendPending = FALSE;
    return handle;
}


void func_02039834(void *sys) {
    int i;
    GFLNetInitData *ini = func_0203d798();

    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        func_02039764(i);
    }
    func_02039764(GFL_NET_NETID_SERVER);
    GFL_ASSERT_AT(227, !_pBaseWork);
    _pBaseWork = GFL_HeapAllocate(ini->heapId, sizeof(NetHandleBaseWork), TRUE, "net_handle.c", 229);
}

void func_020398b4(void *sys) {
    NetHandle *handle;
    int i;

    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        func_02039658(func_0203996c(i), i);
    }
    handle = func_0203996c(GFL_NET_NETID_SERVER);
    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        if (handle->negotiations[i] != NULL) {
            GFL_HeapFree(handle->negotiations[i]);
            handle->negotiations[i] = NULL;
        }
    }
    func_02039658(func_0203996c(GFL_NET_NETID_SERVER), GFL_NET_NETID_SERVER);
    GFL_ASSERT_AT(256, _pBaseWork);
    GFL_HeapFree(_pBaseWork);
    _pBaseWork = NULL;
}

NetHandle *func_0203996c(int netId) {
    GFLNetSys *sys = func_0203d788();

    if (sys != NULL) {
        if (netId == GFL_NET_NETID_SERVER) {
            return &sys->handles[GFL_NET_HANDLE_MAX - 1];
        }
        if (netId < GFL_NET_HANDLE_MAX) {
            return &sys->handles[netId];
        }
    }
    return NULL;
}


NetHandle *func_020399a8(void) {
    return func_0203996c(func_02039338());
}


BOOL func_020399b8(NetHandle *handle) {
    if (handle != NULL) {
        if (handle->state == NET_HANDLE_STATE_NEGOTIATED) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

u8 func_020399dc(NetHandle *handle) {
    if (handle != NULL) {
        return handle->state;
    }
    return 8;
}

int func_020399ec(void) {
    int i;
    int count;
    GFLNetSys *sys = func_0203d788();

    count = 0;
    if (sys != NULL) {
        for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
            if (func_020399b8(func_0203996c(i))) {
                count++;
            }
        }
    }
    return count;
}

void func_02039a2c(int netId) {
    if (func_02039338() == 0) {
        NetHandle *handle = func_0203996c(GFL_NET_NETID_SERVER);

        handle->replies[netId].netId = netId;
        handle->replies[netId].pending = TRUE;
    }
}

static void func_02039a5c(void) {
    NetHandle *handle;
    int i;

    if (func_02039338() == 0) {
        handle = func_0203996c(GFL_NET_NETID_SERVER);

        for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
            if (handle->replies[i].pending && func_0203d3a8(handle, 5, 2, &handle->replies[i])) {
                handle->replies[i].pending = FALSE;
            }
        }
    }
}

BOOL func_02039acc(void) {
    NetHandle *handle = func_020399a8();
    u8 netId = func_020395ec(handle);
    GFLNetInitData *ini;

    func_0203d788();
    ini = func_0203d798();
    if (func_02039160(func_02039338())) {
        if (handle->state == NET_HANDLE_STATE_SEND_NEGOTIATION) {
            u8 data[9];

            OS_GetMacAddress(data);
            data[6] = netId;
            data[7] = ini->gameCommandBase;
            data[8] = ini->unk6E;
            return func_0203d3a8(handle, 3, sizeof(data), data);
        } else if (handle->state < NET_HANDLE_STATE_SEND_NEGOTIATION) {
            handle->state++;
        }
    }
    return FALSE;
}

void func_02039b5c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    u16 timing = ((const NetTimingData *)data)->timing;
    u8 netIds = ((const NetTimingData *)data)->netIds;
    int i;

    if (func_020395ec(handle) == GFL_NET_NETID_SERVER && netId != GFL_NET_NETID_SERVER) {
        _pBaseWork->timings[netId] = timing;
        _pBaseWork->timingSend[netId] = netIds;
        for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
            if (func_020399b8(func_0203996c(i))) {
                if (netIds == _pBaseWork->timingSend[i]) {
                    if (timing != _pBaseWork->timings[i]) {
                        return;
                    }
                } else if ((1 << i) & netIds) {
                    return;
                }
            } else if (func_02039160(i) == TRUE) {
                return;
            }
        }
        _pBaseWork->relayPending[netId] = TRUE;
    }
}






void func_02039c2c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    handle->timingRecv = *(u16 *)data;
}

void func_02039c3c(NetHandle *handle, u16 timing) {
    handle->timingSend = timing;
    handle->unk48 = 0xff;
    if (func_020395ec(handle) == GFL_NET_NETID_SERVER) {
        handle->timingSendPending = FALSE;
    } else {
        handle->timingSendPending = TRUE;
    }
}

void func_02039c68(NetHandle *handle, u32 a1, u32 a2) {
    if (handle != NULL) {
        handle->timingSend = a1 + (a2 << 8);
        handle->unk48 = 0xff;
        if (func_020395ec(handle) == GFL_NET_NETID_SERVER) {
            handle->timingSendPending = FALSE;
        } else {
            handle->timingSendPending = TRUE;
        }
    }
}

BOOL func_02039ca0(NetHandle *handle, u16 timing) {
    if (handle->timingRecv == timing) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_02039cb4(NetHandle *handle, u32 a1, u32 a2) {
    if (handle == NULL) {
        return TRUE;
    }
    if (handle->timingRecv == a1 + (a2 << 8)) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_02039cd8(NetHandle *handle) {
    int i;

    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        if (handle->negotiations[i]->valid && !func_0203a2f0(handle->negotiationsSent, i)) {
            if (func_0203d3f8(handle, GFL_NET_NETID_SERVER, 4, sizeof(NetNegotiation), handle->negotiations[i], 1, FALSE, 1)) {
                func_0203a28c(handle->negotiationsSent, i, TRUE);
            } else {
                return FALSE;
            }
        }
    }
    handle->negotiationsSent = 0;
    return TRUE;
}

void func_02039d94(void) {
    NetHandle *handle;
    GFLNetInitData *ini;
    int i;

    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        int netId = func_020395e0(i);

        handle = func_0203996c(netId);
        if (!func_02039160(netId)) {
            func_020399b8(handle);
        }
    }
    func_02039a5c();
    if (func_0203d378()) {
        handle = func_0203996c(GFL_NET_NETID_SERVER);
        for (i = 0; i < GFL_NET_HANDLE_MAX; i++) {
            if (_pBaseWork->relayPending[i] == TRUE
                && func_0203d4d0(handle, _pBaseWork->timingSend[i], 8, 2, &_pBaseWork->timings[i], 0, 0, 0)) {
                GFL_ASSERT_AT(666, _pBaseWork->timingSend[i]);
                _pBaseWork->relayPending[i] = FALSE;
            }
        }
    }
    for (i = 0; i < GFL_NET_HANDLE_MAX; i++) {
        handle = func_0203996c(func_020395e0(i));
        if (func_0203d394(handle) && handle->timingSendPending) {
            if (func_020395ec(handle) != GFL_NET_NETID_SERVER) {
                if (func_0203d3a8(handle, 7, 4, &handle->timingSend)) {
                    handle->timingSendPending = FALSE;
                }
            }
        }
    }
    if (func_02039338() == 0) {
        handle = func_0203996c(GFL_NET_NETID_SERVER);
        if (handle != NULL) {
            for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
                if (handle->negotiations[i] != NULL && handle->negotiations[i]->valid
                    && !func_0203a2f0(handle->serverNegotiationsSent, i) && func_02039cd8(handle)) {
                    handle->serverNegotiationsSent = func_0203a28c(handle->serverNegotiationsSent, i, TRUE);
                }
            }
        }
    }
    ini = func_0203d798();
    if (ini->bNetType == 4) {
        return;
    }
    handle = func_020399a8();
    if (handle->infoPending != TRUE) {
        return;
    }
    if (ini->getInfo != NULL && ini->getInfoSize != NULL) {
        int size = ini->getInfoSize(func_0203d650());

        if (func_0203d3a8(handle, 10, size, ini->getInfo(func_0203d650()))) {
            handle->infoPending = FALSE;
        }
    } else {
        handle->infoPending = FALSE;
    }
}


void func_0203a020(int netId, int size, const void *pData, void *work, NetHandle *handle) {
    const u8 *data = pData;
    GFLNetInitData *ini = func_0203d798();
    NetNegotiation *negotiation;

    if (func_0203996c(GFL_NET_NETID_SERVER) == handle) {
        negotiation = handle->negotiations[netId];
        if (negotiation->valid) {
            return;
        }
        negotiation->valid = TRUE;
        MI_CpuCopy8(data, negotiation->mac, sizeof(negotiation->mac));
        negotiation->netId = data[6];
        OS_GetMacAddress(negotiation->serverMac);
        if (data[8] != 0) {
            if (data[6] != 0 && data[7] != ini->gameCommandBase && ini->unk6E == data[8]) {
                negotiation->unkF = ini->unk6E;
                negotiation->unkE = ini->unk6E;
                return;
            }
            if (data[6] == 0 && data[7] == ini->gameCommandBase && ini->unk6E == data[8]) {
                negotiation->unkF = ini->unk6E;
                negotiation->unkE = ini->unk6E;
                return;
            }
            negotiation->unkF = 0xff;
            negotiation->unkE = 0;
        } else {
            negotiation->unkF = ini->gameCommandBase;
            negotiation->unkE = data[7];
        }
    } else {
        handle->infoPending = TRUE;
    }
}



void func_0203a118(NetHandle *handle) {
    GFLNetSys *sys = func_0203d788();

    if (sys->unk352 == 0) {
        if (handle->state != NET_HANDLE_STATE_NEGOTIATED) {
            handle->state = NET_HANDLE_STATE_REJECTED;
        }
    } else {
        handle->state = NET_HANDLE_STATE_REJECTED;
    }
}

void func_0203a150(int netId, int size, NetNegotiation *negotiation, void *work, NetHandle *handle) {
    GFLNetInitData *ini = func_0203d798();
    u8 mac[6];

    if (func_020395ec(handle) == GFL_NET_NETID_SERVER) {
        handle->state = NET_HANDLE_STATE_NEGOTIATED;
        return;
    }
    if (ini->unk6E != 0) {
        if (ini->unk6E != negotiation->unkF) {
            func_0203a118(handle);
        }
        if (ini->unk6E != negotiation->unkE) {
            func_0203a118(handle);
        }
    } else {
        if (ini->gameCommandBase != negotiation->unkF) {
            func_0203a118(handle);
        }
        if (ini->gameCommandBase != negotiation->unkE) {
            func_0203a118(handle);
        }
    }
    OS_GetMacAddress(mac);
    if (GFL_STD_MemCmp(negotiation->mac, mac, sizeof(mac)) == 0 && negotiation->netId != func_020395ec(handle)) {
        func_0203a118(handle);
    }
    if (handle->state == NET_HANDLE_STATE_REJECTED) {
        return;
    }
    func_0203996c(negotiation->netId)->state = NET_HANDLE_STATE_NEGOTIATED;
    if (ini->negotiationCallback != NULL) {
        ini->negotiationCallback(work, negotiation->netId);
    }
    func_0203dac4(negotiation->mac);
    func_0203dac4(negotiation->serverMac);
}



void func_0203a27c(int netId, int size, u8 *data) {
    func_0203a338(*data);
}

static u16 func_0203a28c(u16 bits, int netID, BOOL set) {
    u16 bit;

    GFL_ASSERT_AT(913, netID!=GFL_NET_NETID_SERVER);
    bit = 1 << netID;
    if (set) {
        bits |= bit;
    } else {
        bits = (u16)~bit & bits;
    }
    return bits;
}


BOOL func_0203a2f0(u16 bits, int netID) {
    u8 bit;

    GFL_ASSERT_AT(939, netID!=GFL_NET_NETID_SERVER);
    bit = 1 << netID;
    if (bits & bit) {
        return TRUE;
    }
    return FALSE;
}

void func_0203a338(int netID) {
    GFL_ASSERT_AT(958, netID!=GFL_NET_NETID_SERVER);
    func_02039658(func_0203996c(netID), netID);
    func_0203971c(func_0203996c(GFL_NET_NETID_SERVER), netID);
}

// Keeps data from a machine
void func_0203a388(int netID, int size, const void *data, void *work, NetHandle *handle) {
    GFLNetInitData *ini = func_0203d798();

    if (func_020395ec(handle) != GFL_NET_NETID_SERVER) {
        NetHandle *target;

        GFL_ASSERT_AT(977, netID < GFL_NET_HANDLE_MAX);
        target = func_0203996c(netID);
        if (target->data != NULL) {
            GFL_HeapFree(target->data);
        }
        target->data = GFL_HeapAllocate(ini->heapId, size, TRUE, "net_handle.c", 984);
        target->dataSize = size;
        MI_CpuCopy8(data, target->data, size);
    }
}

void func_0203a424(NetHandle *handle, int a1, int a2, int a3) {
    if (handle != NULL) {
        handle->timingSend = a1 + (a2 << 8);
        handle->unk48 = a3;
        if (func_020395ec(handle) == GFL_NET_NETID_SERVER) {
            handle->timingSendPending = FALSE;
        } else {
            handle->timingSendPending = TRUE;
        }
    }
}

void func_0203a458(NetHandle *handle, int value) {
    if (handle != NULL) {
        int i;

        for (i = 0; i < GFL_NET_HANDLE_MAX; i++) {
            NetNegotiation *negotiation = handle->negotiations[i];

            if (negotiation != NULL && negotiation->valid) {
                negotiation->unkE = value;
                negotiation->unkF = value;
            }
        }
    }
}
