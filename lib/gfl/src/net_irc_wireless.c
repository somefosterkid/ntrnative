// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/net_irc_wireless.c. Black's asserts keep
// their file and line, and func_0203dac4 declares its empty MAC address where it first uses it.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/net_irc_wireless.h"
#include "gfl/net_state.h"
#include "gfl/net_system.h"
#include "gfl/std.h"
#include "gfl/ui.h"
#include "nitro/os.h"

// How many MAC addresses are collected over infrared
#define _COLLECTMAC_NUM 4

typedef struct {
    int num;
    u8 macs[_COLLECTMAC_NUM][8];
} NetIwCollectMac;

typedef struct {
    // The init data the networks are started with, the game's with the callbacks below swapped for this file's
    GFLNetInitData aNetInit;
    void *work;
    void *unk74;
    // The game's callbacks from its init data
    void *(*unk18)(void *work);
    int (*unk1C)(void *work);
    void (*exitCallback)(void *work);
    void (*unk84)(void *work);
    void (*unk88)(void *work, BOOL a1);
    void (*unk8C)(void *work);
    NetIwCollectMac collect;
    u8 unkB4[0x4c];
    // Frames left to wait
    u8 timer;
    u8 unk101;
    // Set when the network has connected and when it has ended
    u8 connected;
    u8 ended;
    // The game's MP mode and machine count, which the infrared network does not use
    u8 bMPMode;
    // This machine's ID once connected over infrared, 0 for the parent
    u8 netId;
    // The ID to use in place of netId, plus 1, or 0
    u8 netIdOverride;
    u8 maxConnectNum;
    u8 unk108;
    u8 state;
    u8 unk10A;
    u8 gameCommandBase;
} NetIrcWireless;

static NetIrcWireless *NetIwSys;

static void func_0203dc04(void *work);
static void func_0203dc4c(void *work);
static void func_0203dc68(NetIrcWireless *sys, int state);
static void *func_0203e420(void *work);
static int func_0203e458(void *work);

const GFLNetInitData *func_0203d8f0(const GFLNetInitData *pNetInit, void *work) {
    NetIrcWireless *sys;

    if (NetIwSys != NULL && NetIwSys->unk101 == TRUE) {
        return pNetInit;
    }
    sys = GFL_HeapAllocate(HEAPID_TAIL(pNetInit->parentHeapId), sizeof(NetIrcWireless), TRUE, "net_irc_wireless.c",
                           114);
    sys->aNetInit = *pNetInit;
    sys->work = work;
    sys->exitCallback = pNetInit->unk2C;
    sys->unk18 = pNetInit->unk18;
    sys->unk1C = pNetInit->unk1C;
    sys->aNetInit.bNetType = 3;
    sys->aNetInit.unk2C = func_0203dc4c;
    sys->aNetInit.unk18 = func_0203e420;
    sys->aNetInit.unk1C = func_0203e458;
    sys->bMPMode = sys->aNetInit.bMPMode;
    sys->aNetInit.bMPMode = FALSE;
    sys->maxConnectNum = sys->aNetInit.maxConnectNum;
    NetIwSys = sys;
    func_02034f84(0x20);
    return &NetIwSys->aNetInit;
}

void func_0203d9ec(void) {
    if (NetIwSys != NULL) {
        func_02034fb8(0x20);
        GFL_HeapFree(NetIwSys);
        NetIwSys = NULL;
    }
}

void func_0203da20(void) {
    if (NetIwSys != NULL && NetIwSys->unk101 == 0) {
        func_0203d9ec();
    }
}

BOOL func_0203da4c(void) {
    if (NetIwSys != NULL) {
        return TRUE;
    }
    return FALSE;
}

void func_0203da68(void *a0) {
    NetIwSys->unk74 = a0;
}

void (*func_0203da7c(void (*a0)(void *work), void (*a1)(void *work, BOOL a1), void (*a2)(void *work)))(void *work) {
    if (NetIwSys == NULL) {
        return a0;
    }
    if (a0 != NULL) {
        NetIwSys->unk84 = a0;
    }
    if (a1 != NULL) {
        NetIwSys->unk88 = a1;
    }
    if (a2 != NULL) {
        NetIwSys->unk8C = a2;
    }
    return func_0203dc04;
}

void func_0203dac4(const u8 *mac) {
    int i;

    if (NetIwSys == NULL) {
        return;
    }
    for (i = 0; i < _COLLECTMAC_NUM; i++) {
        if (GFL_STD_MemCmp(mac, NetIwSys->collect.macs[i], 6) == 0) {
            return;
        }
    }
    u8 empty[6] = {0};
    for (i = 0; i < _COLLECTMAC_NUM; i++) {
        if (GFL_STD_MemCmp(empty, NetIwSys->collect.macs[i], 6) == 0) {
            MI_CpuCopy8(mac, NetIwSys->collect.macs[i], 6);
            NetIwSys->collect.num = i;
            return;
        }
    }
    func_0203c938(1);
}

void func_0203dbb0(u8 *mac, int index) {
    if (NetIwSys != NULL) {
        MI_CpuCopy8(NetIwSys->collect.macs[index], mac, 6);
    } else {
        GFL_ASSERT_AT(265, 0);
    }
}

static void func_0203dc04(void *work) {
    if (NetIwSys != NULL) {
        NetIwSys->connected = TRUE;
        if (NetIwSys->netIdOverride != 0) {
            NetIwSys->netId = NetIwSys->netIdOverride - 1;
        } else {
            NetIwSys->netId = func_02039338();
        }
    }
}

static void func_0203dc4c(void *work) {
    if (NetIwSys != NULL) {
        NetIwSys->ended = TRUE;
    }
}

static void func_0203dc68(NetIrcWireless *sys, int state) {
    sys->state = state;
}

void func_0203dc70(void) {
    NetIrcWireless *sys = NetIwSys;

    if (sys == NULL) {
        return;
    }
    switch (sys->state) {
    case 0:
        if (sys->connected == TRUE) {
            sys->unk101 = TRUE;
            func_02039c68(func_020399a8(), 3, 0);
            func_0203dc68(sys, 1);
        }
        break;
    case 1:
        if (func_02039cb4(func_020399a8(), 3, 0)) {
            func_0203dc68(sys, 2);
        }
        break;
    case 2:
        if (func_02039338() == 0) {
            if (func_0203d3a8(func_020399a8(), 1, 0, NULL)) {
                func_0203dc68(sys, 3);
                sys->timer = 200;
            }
        } else if (func_0203d3a8(func_020399a8(), 16, 0, NULL)) {
            func_0203dc68(sys, 3);
        }
        break;
    case 3:
        sys->timer--;
        if (func_0203d210()) {
            if (NetIwSys->unk8C != NULL) {
                NetIwSys->unk8C(NetIwSys->work);
            }
            if (sys->unk10A && sys->aNetInit.maxConnectNum == 4 && sys->netId == 0) {
                func_0203dc68(sys, 14);
                sys->unk108++;
            } else if (sys->netId == 0 && sys->aNetInit.maxConnectNum == 4) {
                sys->unk108++;
                func_0203ceac(NULL);
                sys->unk88(sys->work, TRUE);
                func_0203dc68(sys, 4);
            } else {
                sys->unk88(sys->work, FALSE);
                func_0203dc68(sys, 9);
            }
        }
        if (sys->timer == 0) {
            func_0203ceac(NULL);
        }
        break;
    case 4:
        if (!func_0203cd98()) {
            sys->aNetInit.bNetType = 3;
            sys->aNetInit.unk2C = func_0203dc4c;
            if (sys->gameCommandBase != 0) {
                sys->aNetInit.gameCommandBase = sys->gameCommandBase;
            }
            sys->connected = FALSE;
            sys->ended = FALSE;
            func_0203cbac(&sys->aNetInit, NULL, sys->work);
            func_0203dc68(sys, 5);
        }
        break;
    case 5:
        if (func_0203cd98() == TRUE) {
            func_0203d788()->pDevTable->unkC8(NetIwSys->unk74);
            func_0203d0dc(func_0203dc04);
            func_0203dc68(sys, 6);
        }
        break;
    case 6:
        if (sys->connected == TRUE) {
            sys->unk101 = TRUE;
            if (func_0203d3a8(func_020399a8(), 9, sizeof(NetIwCollectMac), &NetIwSys->collect)) {
                func_0203dc68(sys, 7);
            }
        }
        break;
    case 7:
        if (sys->connected == TRUE && func_0203d3a8(func_020399a8(), 1, 0, NULL)) {
            func_0203d17c(200);
            func_0203dc68(sys, 8);
        }
        break;
    case 8:
        if (func_0203d210()) {
            if (NetIwSys->unk8C != NULL) {
                NetIwSys->unk8C(NetIwSys->work);
            }
            func_0203dc68(sys, 9);
        }
        break;
    case 9:
        if (func_0203d210()) {
            if (sys->gameCommandBase != 0) {
                sys->aNetInit.unk6E = 0;
                sys->aNetInit.gameCommandBase = sys->gameCommandBase;
            }
            sys->aNetInit.bNetType = 0;
            sys->aNetInit.unk2C = sys->exitCallback;
            sys->aNetInit.bMPMode = sys->bMPMode;
            func_0203cbac(&sys->aNetInit, NULL, sys->work);
            NetIwSys->unk101 = FALSE;
            func_0203dc68(sys, 10);
        }
        break;
    case 10:
        if (func_0203cd98() == TRUE) {
            if (sys->netId == 0) {
                func_0203d01c();
                func_0203dc68(sys, 12);
            } else if (sys->aNetInit.maxConnectNum == 4) {
                func_0203d010();
                func_0203dc68(sys, 11);
            } else {
                func_0203cff0(sys->collect.macs[0]);
                func_0203dc68(sys, 12);
            }
        }
        break;
    case 11: {
        int index = 0;
        u8 mac[6];
        int i;

        // Find the beacon of the machine that collected this machine's MAC address
        OS_GetMacAddress(mac);
        for (;;) {
            NetIwCollectMac *collect = func_0203cf10(index);

            if (collect == NULL) {
                break;
            }
            for (i = 0; i < _COLLECTMAC_NUM; i++) {
                if (GFL_STD_MemCmp(mac, collect->macs[i], 6) == 0) {
                    func_0203d000(func_0203cf3c(index));
                    func_0203dc68(sys, 12);
                    break;
                }
            }
            index++;
        }
        break;
    }
    case 12:
        if (sys->netId == 0) {
            if (func_020399ec() != 0 && func_02039acc() == TRUE) {
                func_0203dc68(sys, 13);
            }
        } else if (func_02039acc() == TRUE) {
            func_0203dc68(sys, 13);
        }
        break;
    case 13:
        if (sys->netId != 0) {
            if (sys->unk84 != NULL) {
                sys->unk84(sys->work);
            }
            func_0203da20();
        } else if (func_020399ec() >= sys->maxConnectNum) {
            if (sys->unk84 != NULL) {
                sys->unk84(sys->work);
            }
            func_0203da20();
        }
        break;
    case 14:
        if (!func_0203cd98()) {
            sys->aNetInit.bNetType = 3;
            sys->aNetInit.unk2C = func_0203dc4c;
            sys->connected = FALSE;
            sys->ended = FALSE;
            func_0203cbac(&sys->aNetInit, NULL, sys->work);
            func_0203dc68(sys, 15);
        }
        break;
    case 15:
        if (func_0203cd98() == TRUE) {
            func_0203d788()->pDevTable->unkC8(NetIwSys->unk74);
            func_0203d0dc(func_0203dc04);
            func_0203dc68(sys, 16);
        }
        break;
    case 16:
        if (sys->connected == TRUE) {
            func_02039c68(func_020399a8(), 3, 0);
            NetIwSys->unk101 = TRUE;
            func_0203dc68(sys, 17);
        }
        break;
    case 17:
        if (func_02039cb4(func_020399a8(), 3, 0) && func_0203d3a8(func_020399a8(), 1, 0, NULL)) {
            func_0203dc68(sys, 18);
            sys->timer = 200;
        }
        break;
    case 18:
        sys->timer--;
        if (func_0203d210() || sys->timer == 0) {
            if (NetIwSys->unk8C != NULL) {
                NetIwSys->unk8C(NetIwSys->work);
            }
            if (sys->unk10A && sys->unk108 < 3) {
                sys->unk108++;
                if (sys->unk108 >= 3) {
                    func_0203dc68(sys, 9);
                } else {
                    func_0203dc68(sys, 14);
                }
            } else {
                func_0203dc68(sys, 9);
            }
        }
        break;
    }
}

static void *func_0203e420(void *work) {
    NetIrcWireless *sys = NetIwSys;

    if (sys == NULL) {
        return NULL;
    }
    MI_CpuCopy8(sys->unk18(work), sys->unkB4, sizeof(sys->unkB4));
    return &sys->collect;
}

static int func_0203e458(void *work) {
    NetIrcWireless *sys = NetIwSys;

    if (sys == NULL) {
        return 0;
    }
    return sys->unk1C(work) + sizeof(NetIwCollectMac);
}

void func_0203e484(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const NetIwCollectMac *pTemp = data;
    int i;
    int num;

    GFL_ASSERT_AT(677, pTemp->num<_COLLECTMAC_NUM);
    num = pTemp->num;
    if (num > _COLLECTMAC_NUM) {
        num = _COLLECTMAC_NUM;
    }
    for (i = 0; i < num; i++) {
        func_0203dac4(pTemp->macs[i]);
    }
}

BOOL func_0203e4e8(void) {
    if (NetIwSys == NULL) {
        return FALSE;
    }
    if (NetIwSys->state == 16) {
        NetIwSys->maxConnectNum = NetIwSys->unk108 + 1;
        func_0203ceac(NULL);
        NetIwSys->state = 9;
        return TRUE;
    }
    return FALSE;
}

void func_0203e538(int netId) {
    GFL_ASSERT_AT(740, NetIwSys);
    if (NetIwSys != NULL) {
        NetIwSys->netIdOverride = netId + 1;
        NetIwSys->unk10A = TRUE;
    }
}

u8 func_0203e590(void) {
    if (NetIwSys == NULL) {
        return 0;
    }
    return NetIwSys->unk108;
}

void func_0203e5ac(int gameCommandBase) {
    GFL_ASSERT_AT(794, NetIwSys);
    if (NetIwSys != NULL) {
        NetIwSys->gameCommandBase = gameCommandBase;
    }
}
