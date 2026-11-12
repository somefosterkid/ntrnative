// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/net.c, ported by tools/port_file.py.
#include "types.h"
#include "gfl/backup_card.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_irc_wireless.h"
#include "gfl/net_lower_data.h"
#include "gfl/net_state.h"
#include "gfl/net_system.h"
#include "gfl/std.h"
#include "gfl/ui.h"
#include "gfl/wm_icon.h"

// The infrared packets' size limit
#define GFL_NET_IRC_SEND_MAX 0x80

static struct {
    // The heap net.c makes its heaps under, once func_0203cb34 has made it
    HeapID parentHeapId;
    // The offset of BG 1 of the main engine
    int bg1Y;
    GFLNetSys *pNet;
    int bg1X;
// pokebw2's sNet
} data_021469e0;

void func_0203cdd4(void *work);
void func_0203d24c(int x, int y);
void func_0203d2cc(void);


void func_0203cb34(int a0, int a1, HeapID parentHeapId, HeapID heapId) {
    const GFLNetDevTable *pDevTable = func_0200f17c(GFL_NET_TYPE_WIFI);

    if (pDevTable != NULL) {
        pDevTable->unk00(a0, a1);
    }
    func_0200f234(GFL_NET_TYPE_WIFI);
    if (parentHeapId != heapId) {
        data_021469e0.parentHeapId = heapId;
        GFL_HeapCreateChild(parentHeapId, heapId, 0x9e80);
    }
    func_020371ac(0, 0);
    func_020370c8();
}

void func_0203cbac(const GFLNetInitData *pNetInit, void (*callback)(void *work), void *work) {
    GFLNetSys *pNet;

    if (data_021469e0.parentHeapId != 0) {
        GFL_HeapCreateChild(data_021469e0.parentHeapId, pNetInit->heapId, 0x7800);
    } else {
        GFL_HeapCreateChild(pNetInit->parentHeapId, pNetInit->heapId, 0x7800);
    }
    pNet = GFL_HeapAllocate(pNetInit->heapId, sizeof(GFLNetSys), TRUE, "net.c", 106);
    data_021469e0.pNet = pNet;
    pNet->unk352 = TRUE;
    pNet->pDevTable = func_0200f17c(pNetInit->bNetType);
    GFL_ASSERT_AT(111, pNet->pDevTable);
    if (pNetInit->bNetType == 4) {
        pNetInit = func_0203d8f0(pNetInit, work);
    }
    MI_CpuCopy8(pNetInit, &pNet->aNetInit, sizeof(GFLNetInitData));
    switch (pNetInit->bNetType) {
    case GFL_NET_TYPE_WIFI:
    case GFL_NET_TYPE_WIFI_LOBBY:
    case GFL_NET_TYPE_WIFI_GTS:
        pNet->pDevTable->unk00(pNetInit->parentHeapId, 0);
        GFL_HeapCreateChild(pNetInit->parentHeapId, pNetInit->wifiHeapId | HEAPID_TAIL_BIT, pNetInit->wifiHeapSize);
        break;
    case 3:
    case 4:
        GFL_ASSERT_AT(132, pNetInit->maxSendSize <= GFL_NET_IRC_SEND_MAX);
        if (data_021469e0.parentHeapId != 0) {
            GFL_HeapCreateChild(data_021469e0.parentHeapId, pNetInit->ircHeapId, 0x1b00);
        } else {
            GFL_HeapCreateChild(pNetInit->parentHeapId, pNetInit->ircHeapId, 0x1b00);
        }
        break;
    case 0:
    case 5:
        break;
    default:
        GFL_ASSERT_AT(142, 0);
        return;
    }
    pNet->devWork = work;
    func_0203afcc(pNetInit->heapId, callback);
    func_02039834(pNet);
    func_0203a494(pNetInit->gameCommandBase << 8, pNetInit->commandTable, pNetInit->commandNum, pNet->devWork,
                  pNetInit->heapId);
    if (pNetInit->bNetType != 3 && pNetInit->bNetType != 4) {
        BOOL isWifi = FALSE;

        if (pNetInit->bNetType == GFL_NET_TYPE_WIFI || pNetInit->bNetType == GFL_NET_TYPE_WIFI_LOBBY) {
            isWifi = TRUE;
        }
        func_02037010(pNetInit->iconX, pNetInit->iconY, isWifi, pNetInit->heapId);
    }
}


BOOL func_0203cd98(void) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet == NULL) {
        return FALSE;
    }
    return pNet->pDevTable->unk6C();
}

BOOL func_0203cdbc(void) {
    if (func_0203d788() == NULL) {
        return TRUE;
    }
    return FALSE;
}

void func_0203cdd4(void *work) {
    GFLNetSys *pNet = func_0203d788();
    HeapID heapId = pNet->aNetInit.heapId;
    HeapID wifiHeapId = pNet->aNetInit.wifiHeapId;
    HeapID ircHeapId = pNet->aNetInit.ircHeapId;
    u8 type = pNet->aNetInit.bNetType;

    if (pNet->exitCallback != NULL) {
        pNet->exitCallback(pNet->devWork);
    }
    if (pNet->aNetInit.unk2C != NULL) {
        pNet->aNetInit.unk2C(pNet->devWork);
    }
    func_020398b4(pNet);
    func_0200f234(type);
    GFL_HeapFree(pNet);
    data_021469e0.pNet = NULL;
    switch (type) {
    case 0:
    case 5:
        break;
    case 1:
    case 2:
    case 6:
        GFL_HeapDelete(wifiHeapId);
        break;
    case 3:
    case 4:
        GFL_HeapDelete(ircHeapId);
        break;
    default:
        GFL_ASSERT_AT(0xee, 0);
        return;
    }
    GFL_HeapDelete(heapId);
    func_02034fb8(4);
}


BOOL func_0203ceac(void (*callback)(void *work)) {
    GFLNetSys *pNet = func_0203d788();
    u8 netType;

    func_0203da20();
    if (pNet != NULL) {
        netType = pNet->aNetInit.bNetType;
        pNet->exitCallback = callback;
        if (netType == GFL_NET_TYPE_WIFI) {
            func_0203c774(func_0203cdd4);
        } else {
            func_0203bf9c(func_0203cdd4);
        }
        func_020370c8();
        return TRUE;
    }
    return FALSE;
}


void func_0203cf04(void) {
    func_0203bff0();
}

void *func_0203cf10(int index) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet == NULL) {
        return NULL;
    }
    return pNet->pDevTable->unk28(index);
}

u8 *func_0203cf3c(int index) {
    GFLNetSys *pNet = func_0203d788();

    return pNet->pDevTable->unk2C(index);
}

BOOL func_0203cf5c(void) {
    GFLNetSys *pNet = func_0203d788();

    func_0203ea9c();
    func_0203dc70();
    if (func_0203b1d8()) {
        return TRUE;
    }
    if (pNet == NULL) {
        return TRUE;
    }
    func_02037154();
    func_02039d94();
    func_02037c10();
    return func_02039594();
}


void func_0203cfa0(void) {
    GFLNetSys *pNet = func_0203d788();

    if (!func_0203b1d8() && pNet != NULL && pNet->pDevTable != NULL) {
        u8 type = func_0203d798()->bNetType;

        if (type != GFL_NET_TYPE_WIFI && type != GFL_NET_TYPE_WIFI_LOBBY) {
            pNet->pDevTable->update(0);
        }
    }
}

#pragma thumb off
void func_0203cff0(const u8 *mac) {
    func_0203b0c4(mac, TRUE);
}

void func_0203d000(const u8 *mac) {
    func_0203b0c4(mac, FALSE);
}

void func_0203d010(void) {
    func_0203b1b0();
}

void func_0203d01c(void) {
    GFLNetSys *pNet = func_0203d788();

    switch (pNet->aNetInit.bNetType) {
    case 1:
    case 2:
        func_0203c3b4();
        break;
    case 0:
    case 5:
        func_0203b2d0(pNet->aNetInit.heapId);
        break;
    }
}

void func_0203d060(void (*callback)(void *work), void (*a1)(void *work, BOOL a1), void (*a2)(void *work)) {
    GFLNetSys *pNet = func_0203d788();

    if (func_0203da4c() == TRUE) {
        callback = func_0203da7c(callback, a1, a2);
    }
    func_0203ba98(pNet->aNetInit.heapId, callback, 1);
}



void func_0203d0ac(int gameCommandBase) {
    func_0203e5ac(gameCommandBase);
}

void func_0203d0b8(u8 *mac, int index) {
    func_0203dbb0(mac, index);
}

void func_0203d0c4(void *a0) {
    func_0203da68(a0);
}

void func_0203d0d0(void) {
    func_0203d9ec();
}







void func_0203d0dc(void (*callback)(void *work)) {
    GFLNetSys *pNet = func_0203d788();
    func_0203ba98(pNet->aNetInit.heapId, callback, 1);
}


void func_0203d0fc(int a0) {
    func_0203d788();
    func_0203c010();
}


void func_0203d10c(void (*callback)(void *work)) {
    GFLNetSys *pNet = func_0203d788();
    func_0203ba98(pNet->aNetInit.heapId, callback, 0);
}


void func_02037a44(int a0);

void func_0203d12c(HeapID heapId) {
    if (func_0203cd98() != 0) {
        func_02037a44(heapId);
    }
}

void func_0203d14c(int mode, int a1, const u8 *mac) {
    func_0203bd30(mode, a1, mac);
}

BOOL func_0203d158(void) {
    return func_0203e4e8();
}

void func_0203d164(int netId) {
    func_0203e538(netId);
}

u8 func_0203d170(void) {
    return func_0203e590();
}






#pragma thumb reset

#pragma thumb reset


void func_0203d17c(int a0) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet->pDevTable != NULL) {
        pNet->pDevTable->unkC4(a0);
    }
}

u8 func_0203d1a4(NetHandle *handle) {
    return func_020395ec(handle);
}


int func_0203d1b4(void) {
    return func_020399ec();
}

BOOL func_0203d1c0(u8 netId) {
    if (func_0203d788() == NULL) {
        return FALSE;
    }
    return func_020399b8(func_0203996c(netId));
}

void func_0203d1e8(NetHandle *handle, int a1) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet->pDevTable->unkB8 != NULL) {
        pNet->pDevTable->unkB8(a1);
    }
}

BOOL func_0203d210(void) {
    if (data_021469e0.pNet == NULL) {
        return TRUE;
    }
    if (!func_0203d788()->pDevTable->unk6C()) {
        return TRUE;
    }
    return FALSE;
}

void func_0203d24c(int x, int y) {
    GFLNetInitData *ini = func_0203d798();

    if (ini != NULL) {
        ini->iconX = x;
        ini->iconY = y;
    }
}

BOOL func_0203d26c(void) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet != NULL && (pNet->aNetInit.bNetType == 3 || pNet->aNetInit.bNetType == 4)) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_0203d29c(void) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet != NULL
        && (pNet->aNetInit.bNetType == GFL_NET_TYPE_WIFI || pNet->aNetInit.bNetType == GFL_NET_TYPE_WIFI_LOBBY)) {
        return TRUE;
    }
    return FALSE;
}

void func_0203d2cc(void) {
    GFLNetInitData *ini = func_0203d798();
    GFLNetSys *pNet = func_0203d788();

    if (getLockIDStatus() == TRUE) {
        GFL_ASSERT_AT(0x364, 0);
        return;
    }
    if (pNet != NULL && ini->bNetType != 3) {
        BOOL isWifi = FALSE;

        if (pNet->aNetInit.bNetType == GFL_NET_TYPE_WIFI || pNet->aNetInit.bNetType == GFL_NET_TYPE_WIFI_LOBBY) {
            isWifi = TRUE;
        }
        func_02037010(pNet->aNetInit.iconX, pNet->aNetInit.iconY, isWifi, pNet->aNetInit.heapId);
    } else {
        func_020370c8();
    }
}


void func_0203d34c(BOOL top, HeapID heapId) {
    func_0203d24c(240, 0);
    func_02037118(top, heapId);
    func_0203d2cc();
}


BOOL func_0203d378(void) {
    BOOL ret = FALSE;
    if (func_02039338() == 0) {
        ret = TRUE;
    }
    return ret;
}


BOOL func_0203d394(NetHandle *handle) {
    if (handle->state == NET_HANDLE_STATE_NEGOTIATED) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_0203d3a8(NetHandle *handle, int command, u16 size, const void *data) {
    func_0203d798();

    return func_02038c18(command, data, size, 0, func_020395ec(handle), 0xff, FALSE);
}

BOOL func_0203d3f8(NetHandle *handle, u8 sendID, u16 command, u32 size, const void *data, u32 a5, BOOL a6, BOOL noCopy) {
    u8 dest;

    func_0203d798();
    dest = 0xff;
    if (!func_0203d394(handle) && command >= GFL_NET_CMD_BASE_COUNT) {
        return FALSE;
    }
    if (a6 && func_02039468(command, func_020395ec(handle))) {
        return FALSE;
    }
    if (sendID != 0xff) {
        GFL_ASSERT_AT(0x3fb, sendID < 7);
        dest = 1 << sendID;
    }
    return func_02038c18(command, data, size, a5, func_020395ec(handle), dest, noCopy);
}

BOOL func_0203d4d0(NetHandle *handle, u8 dest, u16 command, int size, const void *data, u32 a5, BOOL a6, BOOL noCopy) {
    func_0203d798();
    if (!func_0203d394(handle) && command >= GFL_NET_CMD_BASE_COUNT) {
        return FALSE;
    }
    if (a6 && func_02039468(command, func_020395ec(handle))) {
        return FALSE;
    }
    return func_02038c18(command, data, size, a5, func_020395ec(handle), dest, noCopy);
}




BOOL func_0203d56c(NetHandle *handle) {
    return func_020394c0();
}


BOOL func_0203d578(NetHandle *handle, u16 command) {
    int dest = func_020395ec(handle);
    return func_02039468(command, dest);
}

void func_0203d594(NetHandle *handle, u16 timing) {
    func_02039c3c(handle, timing);
}

BOOL func_0203d5a0(NetHandle *handle, u16 timing) {
    return func_02039ca0(handle, timing);
}




void func_0203d5ac(u8 gameCommandBase, u8 unused) {
    func_0203d788();

    func_0203d798()->gameCommandBase = gameCommandBase;
    func_0203a458(func_0203996c(GFL_NET_NETID_SERVER), gameCommandBase);
}

u8 func_0203d5d4(void) {
    GFLNetInitData *ini = func_0203d798();

    if (ini != NULL) {
        return ini->gameCommandBase;
    }
    return 0;
}

struct DWCUserData *func_0203d5ec(void) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet->aNetInit.getUserData != NULL) {
        return pNet->aNetInit.getUserData(pNet->devWork);
    }
    return NULL;
}

struct DWCFriendData *func_0203d610(void) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet->aNetInit.getFriendData != NULL) {
        return pNet->aNetInit.getFriendData(pNet->devWork);
    }
    return NULL;
}

u8 func_0203d634(void) {
    GFLNetSys *pNet = func_0203d788();

    return pNet->aNetInit.unk62;
}

void func_0203d644(BOOL a0) {
    func_020394f0(a0);
}


void *func_0203d650(void) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet != NULL) {
        return pNet->devWork;
    }
    return NULL;
}

void func_0203d668(void *work) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet != NULL) {
        pNet->devWork = work;
    }
}

int func_0203d680(void) {
    GFLNetSys *pNet = func_0203d788();

    GFL_ASSERT_AT(0x593, pNet->aNetInit.maxConnectNum <= GFL_NET_MACHINE_MAX);
    return pNet->aNetInit.maxConnectNum;
}

int func_0203d6bc(void) {
    GFLNetSys *pNet = func_0203d788();

    return pNet->aNetInit.maxSendSize;
}

u16 func_0203d6cc(void) {
    GFLNetSys *pNet = func_0203d788();

    GFL_ASSERT_AT(0x5b1, pNet);
    return pNet->aNetInit.unk6C;
}

void func_0203d700(void) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet->aNetInit.unk30 != NULL) {
        pNet->aNetInit.unk30(pNet->devWork);
    }
}

void func_0203d720(int a0) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet->aNetInit.unk28 != NULL) {
        pNet->aNetInit.unk28(func_020399a8(), a0, pNet->devWork);
    }
}

void func_0203d754(int a0) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet->aNetInit.unk24 != NULL) {
        pNet->aNetInit.unk24(func_020399a8(), a0, pNet->devWork);
    }
}



GFLNetSys *func_0203d788(void) {
    return data_021469e0.pNet;
}

GFLNetInitData *func_0203d798(void) {
    if (data_021469e0.pNet != NULL) {
        return &data_021469e0.pNet->aNetInit;
    }
    return NULL;
}

void func_0203d7b0(u8 a0) {
    func_0203c8e8(a0);
}

void func_0203d7bc(BOOL a0) {
    func_0203c900(a0);
}

// Allocates 32-byte aligned memory, keeping the allocation just before it for func_0203d7fc
void *allocConfigDSSoftwareFeature(HeapID heapId, u32 size, const char *file, u16 line) {
    void *mem = GFL_HeapAllocate(heapId, ((size + sizeof(void *) + 32) & ~31) + 32, TRUE, file, line);
    void **aligned = (void **)(((uintptr_t)mem + 32) & ~31);

    aligned[-1] = mem;
    return aligned;
}

void func_0203d7fc(void *ptr) {
    if (ptr != NULL) {
        GFL_HeapFree(((void **)ptr)[-1]);
    }
}

void func_0203d814(int x, int y) {
    data_021469e0.bg1X = x;
    data_021469e0.bg1Y = y;
}

void func_0203d828(int *x, int *y) {
    *x = data_021469e0.bg1X;
    *y = data_021469e0.bg1Y;
}

void func_0203d844(const GFLNetInitData *pNetInit) {
    MI_CpuCopy8(pNetInit, &func_0203d788()->aNetInit, sizeof(GFLNetInitData));
}

void func_0203d864(void) {
    GFLNetSys *pNet = func_0203d788();

    if (pNet != NULL) {
        pNet->aNetInit.unk2C = NULL;
        pNet->aNetInit.unk1C = NULL;
    }
}

BOOL func_0203d880(void) {
    return func_02039414();
}

void func_0203d88c(int x, int y) {
    func_020371ac(x, y);
    func_0203d2cc();
}



void func_0203d8ac(void) {
    func_020371ac(0, 0);
    func_0203d2cc();
}


void func_0203d8c4(BOOL a0) {
    if (data_021469e0.pNet != NULL) {
        if (a0 == TRUE) {
            data_021469e0.pNet->unk352 = TRUE;
        } else {
            data_021469e0.pNet->unk352 = FALSE;
        }
    }
}
