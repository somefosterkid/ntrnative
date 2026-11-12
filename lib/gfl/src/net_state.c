// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/net_state.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_irc_wireless.h"
#include "gfl/net_state.h"
#include "gfl/net_system.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "gfl/ui.h"
#include "gfl/wm_icon.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "nitro/rtc.h"


typedef struct NetState NetState;
typedef void (*NetStateFunc)(NetState *state);

enum {
    NET_STATE_MODE_CHILD = 1,
    NET_STATE_MODE_PARENT,
};

struct NetState {
    u8 unk0[8];
    NetStateFunc func;
    MATHRandContext32 rand;
    u8 unk24[4];
    // Called when the network has ended
    void (*endCallback)(void *work);
    // Called when the network is ready
    void (*connectCallback)(void *work);
    GFLNetErrorInfo error;
    u8 aMacAddress[6];
    // Frames to wait, which some states count down
    u16 timer;
    s8 unk48;
    u8 unk49;
    u8 unk4A;
    u8 unk4B;
    u8 unk4C;
    u8 errorFlag;
    s8 errorCode;
    u8 unk4F;
    u8 unk50;
    // Whether ending the network has been requested
    u8 ending;
    u8 unk52;
    u8 unk53;
    u8 unk54[2];
    u8 mode;
    u8 parentMac[6];
    u8 unk5D[7];
};

static void func_0203ad74(NetState *state, NetStateFunc func, int timer);
static void func_0203ad80(NetState *state, NetStateFunc func, int timer, int line);
void func_0203ad8c(HeapID heapId);
static void func_0203ae20(MATHRandContext32 *rand);
void func_0203aeec(NetState *state);
void func_0203af40(NetState *state);
void func_0203b018(NetState *state);
void func_0203b01c(void);
void func_0203b038(NetState *state);
void func_0203b084(NetState *state);
void func_0203b16c(NetState *state);
static void func_0203b170(NetState *state);
void func_0203b224(NetState *state);
void func_0203b228(NetState *state);
BOOL func_0203b22c(void);
void func_0203b258(NetState *state);
void func_0203b2a8(HeapID heapId);
u32 func_0203b2fc(NetState *state);
u32 func_0203b344(NetState *state);
void func_0203b390(NetState *state);
void func_0203b448(NetState *state);
void func_0203b48c(NetState *state);
void func_0203b4dc(NetState *state);
void func_0203b52c(NetState *state);
void func_0203b5d4(NetState *state);
void func_0203b688(NetState *state);
BOOL func_0203b6b4(void);
void func_0203b6ec(NetState *state);
void func_0203b758(NetState *state);
void func_0203b794(NetState *state);
void func_0203b7f8(NetState *state);
void func_0203b868(NetState *state);
void func_0203b90c(NetState *state);
void func_0203b950(NetState *state);
void func_0203b9d4(NetState *state);
void func_0203bbb0(NetState *state);
void func_0203bc58(int mode, int unk53, const u8 *mac);
void func_0203bce0(NetState *state);
void func_0203bdec(NetState *state);
void func_0203bec8(NetState *state);
void func_0203bf4c(NetState *state);
void func_0203c00c(NetState *state);
void func_0203c05c(NetState *state);
static void func_0203c060(NetState *state);
static void func_0203c064(NetState *state);
void func_0203c068(NetState *state);
static void func_0203c0b4(NetState *state);
void func_0203c0dc(NetState *state, int code);
void func_0203c0f4(NetState *state);
void func_0203c238(NetState *state);
static void func_0203c464(NetState *state);
void func_0203c494(NetState *state);
void func_0203c65c(NetState *state);
void func_0203c710(NetState *state);
void func_0203c7e0(NetState *state);
void func_0203c864(NetState *state);
static void func_0203c934(void);
static void func_0203cadc(NetState *state);

// pokebw2's _pNetState
static NetState *_pNetState;

static void func_0203ad74(NetState *state, NetStateFunc func, int timer) {
    state->func = func;
    state->timer = timer;
}

// Changes the state, from a line that the debug build reported
static void func_0203ad80(NetState *state, NetStateFunc func, int timer, int line) {
    func_0203ad74(state, func, timer);
}

void func_0203ad8c(HeapID heapId) {
    GFL_ASSERT_AT(184, NULL==_pNetState);
    _pNetState = GFL_HeapAllocate(heapId, sizeof(NetState), TRUE, "net_state.c", 185);
    _pNetState->timer = 50;
    _pNetState->unk4C = TRUE;
    func_0203ae20(&_pNetState->rand);
    func_0203a494(0, NULL, 0, NULL, heapId);
}

static void func_0203ae20(MATHRandContext32 *rand) {
    RTCDate date;
    RTCTime time;
    u8 mac[6];
    u64 high;
    u64 low;
    u32 seed;

    OS_GetMacAddress(mac);
    RTC_GetDateTime(&date, &time);
    seed = date.year + date.month * 0x100 * date.day * 0x10000 + time.hour * 0x10000 + (time.minute + time.second) * 0x1000000;
    seed += mac[3] * 0x10000 + mac[4] * 0x100 + mac[5];
    seed += OS_GetVBlankCount();
    GFL_RandomUpdateMT(seed);
    high = GFL_RandomMT();
    low = GFL_RandomMT();
    MATH_InitRand32(rand, (high << 32) + low);
}

void func_0203aeec(NetState *state) {
    GFLNetSys *sys = func_0203d788();
    void (*endCallback)(void *work);

    if (sys->pDevTable->unk74() == 0) {
        return;
    }
    func_0203a53c();
    func_020370c8();
    func_02037b08();
    endCallback = _pNetState->endCallback;
    GFL_HeapFree(_pNetState);
    _pNetState = NULL;
    if (endCallback != NULL) {
        endCallback(NULL);
    }
}

void func_0203af40(NetState *state) {
    GFLNetSys *sys = func_0203d788();
    GFLNetInitData *ini = func_0203d798();

    if (ini->bNetType != GFL_NET_TYPE_WIFI && ini->bNetType != GFL_NET_TYPE_WIFI_LOBBY) {
        sys->pDevTable->init(ini->heapId, sys, 0, sys->devWork);
    }
    func_02037d78();
    if (state->connectCallback != NULL) {
        state->connectCallback(func_0203d650());
        state->connectCallback = NULL;
    }
    func_0203ad80(_pNetState, func_0203c00c, 0, 303);
}



void func_0203afcc(HeapID heapId, void (*connectCallback)(void *work)) {
    func_02034f84(4);
    func_0203ad8c(heapId);
    _pNetState->connectCallback = connectCallback;
    func_0203ad80(_pNetState, func_0203af40, 0, 319);
}



void func_0203b018(NetState *state) {
}

void func_0203b01c(void) {
    _pNetState->timer += 60;
}

void func_0203b038(NetState *state) {
    if (!func_02037a6c(FALSE, state->aMacAddress, 512, func_0203b01c)) {
        return;
    }
    func_0203ad80(_pNetState, func_0203b018, 10, 361);
}


void func_0203b084(NetState *state) {
    func_0203d798();
    if (!func_02037928(1, 512)) {
        return;
    }
    func_0203ad80(_pNetState, func_0203b038, 0, 380);
}


void func_0203b0c4(const u8 *mac, BOOL parent) {
    // Written out: the ROM's text of the assert keeps the original's (u32) cast of the address
    if (!(((uintptr_t)_pNetState->aMacAddress % 2) == 0)) {
        GFL_DebugAssertFail(__FILE__, 395, "((u32)_pNetState->aMacAddress%2)==0");
    }
    if (mac != NULL) {
        MI_CpuCopy8(mac, _pNetState->aMacAddress, sizeof(_pNetState->aMacAddress));
    }
    if (parent) {
        func_0203ad80(_pNetState, func_0203b084, 0, 400);
    } else {
        func_0203ad80(_pNetState, func_0203b038, 0, 403);
    }
}

void func_0203b16c(NetState *state) {
}

static void func_0203b170(NetState *state) {
    if (func_02037928(1, 512) == 0) {
        return;
    }
    func_0203ad80(_pNetState, func_0203b16c, 0, 430);
}


void func_0203b1b0(void) {
    func_0203ad80(_pNetState, func_0203b170, 0, 444);
}

BOOL func_0203b1d8(void) {
    if (_pNetState == NULL) {
        return TRUE;
    }
    if (_pNetState->func != NULL) {
        _pNetState->func(_pNetState);
    }
    if (_pNetState == NULL) {
        return TRUE;
    }
    func_0203c934();
    return FALSE;
}

void func_0203b224(NetState *state) {
}

void func_0203b228(NetState *state) {
}

BOOL func_0203b22c(void) {
    func_0203ad80(_pNetState, func_0203b224, 0, 500);
    return TRUE;
}

void func_0203b258(NetState *state) {
    func_0203d798();
    if (func_02037894(1, 0x200, 1, func_0203b22c) != 0) {
        func_0203ad80(_pNetState, func_0203b228, 0, 517);
    }
}


void func_0203b2a8(HeapID heapId) {
    func_0203ad80(_pNetState, func_0203b228, 0, 532);
}

void func_0203b2d0(HeapID unused) {
    func_0203ad80(_pNetState, func_0203b258, 0, 546);
}

u32 func_0203b2fc(NetState *state) {
    return MATH_Rand32(&state->rand, 40) + 60;
}

u32 func_0203b344(NetState *state) {
    return MATH_Rand32(&state->rand, 32) + 180;
}

void func_0203b390(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->unk78()) {
        if (state->unk53 && state->mode != NET_STATE_MODE_CHILD) {
            if (func_02037a6c(TRUE, state->parentMac, 0x200, func_0203b01c)) {
                func_0203ad80(_pNetState, func_0203b950, func_0203b2fc(state), 581);
            }
        } else if (func_02037928(TRUE, 0x200)) {
            func_0203ad80(_pNetState, func_0203b950, func_0203b344(state), 586);
        }
    }
}


void func_0203b448(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->unk78()) {
        func_0203ad80(_pNetState, func_0203b390, 0, 605);
    }
}

void func_0203b48c(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->unk10(1, 0) != 0) {
        func_0203ad80(_pNetState, func_0203b448, 0, 613);
    }
}

void func_0203b4dc(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->unk10(1, 0) != 0) {
        func_0203ad80(_pNetState, func_0203b390, 0, 642);
    }
}

void func_0203b52c(NetState *state) {
    func_0203d798();
    if (func_020393d0()) {
        return;
    }
    if (!func_020393a8()) {
        func_0203ad80(_pNetState, func_0203b4dc, 0, 663);
        return;
    }
    if (!func_020399ec()) {
        return;
    }
    if (!func_02039acc()) {
        return;
    }
    if (state->connectCallback != NULL) {
        state->connectCallback(func_0203d650());
        state->connectCallback = NULL;
    }
    func_0203ad80(_pNetState, func_0203c00c, 0, 678);
}

void func_0203b5d4(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (func_020393a8()) {
        func_0203b2a8(func_0203d798()->heapId);
        func_0203d700();
        func_0203ad80(_pNetState, func_0203b52c, 0, 705);
        return;
    }
    if (state->mode == NET_STATE_MODE_CHILD) {
        return;
    }
    if (!sys->pDevTable->unk38(0)) {
        return;
    }
    if (state->timer != 0) {
        state->timer--;
        return;
    }
    func_0203ad80(_pNetState, func_0203b48c, 0, 723);
}






void func_0203b688(NetState *state) {
    func_0203ad80(_pNetState, func_0203b5d4, func_0203b344(state), 736);
}

BOOL func_0203b6b4(void) {
    func_0203ad80(_pNetState, func_0203b5d4, func_0203b344(_pNetState), 742);
    return TRUE;
}

void func_0203b6ec(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->unk78() == 0) {
        return;
    }
    if (func_02037894(0, 0x200, 0, func_0203b6b4) == 0) {
        return;
    }
    state->unk4C = 0;
    func_0203ad80(_pNetState, func_0203b688, 30, 764);
}



void func_0203b758(NetState *state) {
    func_0203d798();
    if (func_02039acc()) {
        func_0203ad80(_pNetState, func_0203b794, 0, 782);
    }
}

void func_0203b794(NetState *state) {
    func_0203d798();
    if (func_020399b8(func_020399a8()) == 0) {
        return;
    }
    if (state->connectCallback != NULL) {
        state->connectCallback(func_0203d650());
        state->connectCallback = NULL;
    }
    func_0203ad80(_pNetState, func_0203c00c, 0, 805);
}

void func_0203b7f8(NetState *state) {
    func_0203d798();
    func_0203996c(0xff);
    func_020399a8();
    if (func_020399ec() < 2) {
        return;
    }
    func_02039338();
    if (state->connectCallback != NULL) {
        state->connectCallback(func_0203d650());
        state->connectCallback = NULL;
    }
    func_0203ad80(_pNetState, func_0203c00c, 0, 831);
}

void func_0203b868(NetState *state) {
    GFLNetSys *sys;

    func_0203d798();
    sys = func_0203d788();
    if (sys->pDevTable->unkA8() != TRUE) {
        return;
    }
    if (!func_02039acc()) {
        return;
    }
    if (func_0203da4c() == TRUE) {
        func_0203ad80(_pNetState, func_0203b7f8, 0, 852);
        return;
    }
    if (state->connectCallback != NULL) {
        state->connectCallback(func_0203d650());
        state->connectCallback = NULL;
    }
    func_0203ad80(_pNetState, func_0203c00c, 0, 859);
}




void func_0203b90c(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->unk78()) {
        func_0203ad80(_pNetState, func_0203b6ec, 0, 881);
    }
}

void func_0203b950(NetState *state) {
    if (func_02039338() != 0) {
        func_0203ad80(_pNetState, func_0203b758, 0, 899);
        return;
    }
    if (state->mode == 2) {
        return;
    }
    if (state->timer != 0) {
        state->timer--;
        return;
    }
    func_0203ad80(_pNetState, func_0203bce0, 0, 912);
    func_0203bce0(state);
}

void func_0203b9d4(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->unkAC() == TRUE) {
        sys->pDevTable->unkB4();
        func_0203ad80(_pNetState, func_0203b868, 0, 933);
        return;
    }
    if (state->timer == 0) {
        state->timer = MATH_Rand32(&state->rand, 30) + 15;
        sys->pDevTable->unk08(0);
    } else {
        state->timer--;
    }
}

void func_0203ba98(int a0, void (*callback)(void *work), int a2) {
    GFLNetInitData *ini = func_0203d798();

    func_0203d788();
    func_0203bc58(0, a2, NULL);
    if (ini->bNetType == 3 || ini->bNetType == 4) {
        func_020377b8(0);
        _pNetState->connectCallback = callback;
        func_0203ad80(_pNetState, func_0203b9d4, MATH_Rand32(&_pNetState->rand, 30), 966);
    } else {
        if (_pNetState->unk53 && _pNetState->mode != NET_STATE_MODE_CHILD) {
            func_02037a6c(TRUE, _pNetState->parentMac, 0x200, func_0203b01c);
        } else {
            func_02037928(TRUE, 0x200);
        }
        _pNetState->connectCallback = callback;
        func_0203ad80(_pNetState, func_0203b950, 30, 978);
    }
}

void func_0203bbb0(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (_pNetState->mode == NET_STATE_MODE_PARENT && _pNetState->func == func_0203b950) {
        if (sys->pDevTable->unk10(TRUE, 0)) {
            func_0203ad80(_pNetState, func_0203b90c, 0, 992);
        }
    }
    if (_pNetState->mode == NET_STATE_MODE_CHILD && _pNetState->func == func_0203b5d4) {
        func_0203ad80(_pNetState, func_0203b48c, 0, 1001);
    }
}




void func_0203bc58(int mode, int unk53, const u8 *mac) {
    func_0203d788();
    _pNetState->unk53 = unk53;
    _pNetState->mode = mode;
    if (mac != NULL) {
        MI_CpuCopy8(mac, _pNetState->parentMac, 6);
    } else {
        MI_CpuFill8(_pNetState->parentMac, 0xff, 6);
    }
    func_0203ad80(_pNetState, func_0203bbb0, 0, 1030);
    func_0203bbb0(_pNetState);
}



void func_0203bce0(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->unk10(1, 0) != 0) {
        func_0203ad80(_pNetState, func_0203b90c, 0, 1041);
    }
}

void func_0203bd30(int mode, int unk53, const u8 *mac) {
    func_0203d788();
    _pNetState->unk53 = unk53;
    _pNetState->mode = mode;
    if (mac != NULL) {
        MI_CpuCopy8(mac, _pNetState->parentMac, 6);
    } else {
        MI_CpuFill8(_pNetState->parentMac, 0xff, 6);
    }
    if (_pNetState->mode == NET_STATE_MODE_PARENT) {
        func_0203ad80(_pNetState, func_0203b48c, 0, 1072);
    }
    if (_pNetState->mode == NET_STATE_MODE_CHILD) {
        func_0203ad80(_pNetState, func_0203bce0, 0, 1080);
        func_0203bce0(_pNetState);
    }
}


void func_0203bdec(NetState *state) {
    GFLNetInitData *init = func_0203d798();
    GFLNetSys *sys = func_0203d788();

    if (init->bNetType == 3 || init->bNetType == 4) {
        if (sys->pDevTable->unkAC() != 0) {
            return;
        }
        func_0203ceac(0);
        return;
    }
    if (func_0203d788()->pDevTable->getConnectBits() & 0xfffe) {
        return;
    }
    func_0203ceac(0);
}


void func_0203be5c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_0203d3a8(handle, 2, 0, NULL);
    if (func_02039338() == 0) {
        func_0203ad80(_pNetState, func_0203bdec, 0, 1132);
    }
}



void func_0203beac(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (func_02039338() == 0) {
        return;
    }
    func_0203ceac(NULL);
}

void func_0203bec8(NetState *state) {
    GFLNetSys *sys;

    func_0203d798();
    sys = func_0203d788();
    if (sys->pDevTable->unk78() != 0) {
        if (sys->pDevTable->unk14(0) == 0) {
            return;
        }
        func_0203aeec(state);
        return;
    }
    if (sys->pDevTable->unk6C() != 0) {
        return;
    }
    if (sys->pDevTable->unk14(0) == 0) {
        return;
    }
    func_0203aeec(state);
}


void func_0203bf4c(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->unk10(1, 0) != 0) {
        func_0203ad80(_pNetState, func_0203bec8, 0, 1188);
    }
}




void func_0203bf9c(void (*callback)(void *work)) {
    // BUG: When the network is not running, this writes through a NULL pointer
    if (_pNetState == NULL || !_pNetState->ending) {
        _pNetState->endCallback = callback;
        _pNetState->ending = TRUE;
        func_0203ad80(_pNetState, func_0203bf4c, 5, 1207);
    }
}

void func_0203bff0(void) {
    if (_pNetState != NULL) {
        _pNetState->ending = FALSE;
    }
}

void func_0203c00c(NetState *state) {
}

void func_0203c010(void) {
    func_020379c8(512);
    func_0203ad80(_pNetState, func_0203c00c, 0, 1242);
}


void func_0203c044(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_0203d798()->bMPMode = *(u8 *)data;
}

void func_0203c05c(NetState *state) {
}

static void func_0203c060(NetState *state) {
}

static void func_0203c064(NetState *state) {
}

void func_0203c068(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    if (sys->pDevTable->update(0) < 0) {
        func_0203ad80(_pNetState, func_0203c05c, 0, 1324);
    }
}



static void func_0203c0b4(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    func_020394dc(1);
    sys->pDevTable->update(0);
}


void func_0203c0dc(NetState *state, int code) {
    if ((u32)(code - 1005) > 1) {
        code = -code;
    }
    state->errorCode = code;
}

void func_0203c0f4(NetState *state) {
    GFLNetSys *sys = func_0203d788();
    int result = sys->pDevTable->update(0);

    if (result == 1000) {
        func_0203ad80(_pNetState, func_0203c0b4, 0, 1403);
        return;
    }
    if (sys->pDevTable->unkAC()) {
        func_0203ad80(_pNetState, func_0203c0b4, 0, 1407);
        return;
    }
    if (result == 1007) {
        return;
    }
    if (result == 1001) {
        func_0203ad80(_pNetState, func_0203c060, 0, 1413);
        return;
    }
    if (result == 1002) {
        func_0203ad80(_pNetState, func_0203c064, 0, 1416);
        return;
    }
    if (result >= 10) {
        func_0203ad80(_pNetState, func_0203c064, 0, 1420);
        return;
    }
    if (result < 0) {
        func_0203c0dc(state, result);
        func_0203ad80(_pNetState, func_0203c05c, 0, 1424);
    }
}


void func_0203c238(NetState *state) {
    GFLNetSys *sys = func_0203d788();
    GFLNetInitData *init = func_0203d798();

    if (sys->pDevTable->unk50(state->unk48, init->maxConnectNum, 0) != 0) {
        return;
    }
    func_02037d78();
    func_020398b4(sys);
    func_02039834(sys);
    func_0203ad80(_pNetState, func_0203c0f4, 0, 1452);
}


BOOL func_0203c2b0(int a0) {
    if (a0 != -1) {
        if (func_0203778c(a0) == 0) {
            return FALSE;
        }
    }
    _pNetState->unk48 = a0;
    func_0203ad80(_pNetState, func_0203c238, 0, 1476);
    return TRUE;
}

BOOL func_0203c304(void) {
    GFLNetSys *sys = func_0203d788();

    _pNetState->unk48 = -2;
    func_02037d78();
    func_020398b4(sys);
    func_02039834(sys);
    func_0203ad80(_pNetState, func_0203c0f4, 0, 1497);
    return TRUE;
}

BOOL func_0203c35c(void) {
    GFLNetSys *sys = func_0203d788();

    _pNetState->unk48 = -2;
    func_02037d78();
    func_020398b4(sys);
    func_02039834(sys);
    func_0203ad80(_pNetState, func_0203c0f4, 0, 1533);
    return TRUE;
}




void func_0203c3b4(void) {
    _pNetState->unk48 = -1;
    func_0203ad80(_pNetState, func_0203c238, 0, 1549);
}

int func_0203c3ec(void) {
    NetStateFunc func = _pNetState->func;

    if (func == func_0203c0f4) {
        return 0;
    }
    if (func == func_0203c0b4) {
        return 1;
    }
    if (func == func_0203c068) {
        return 3;
    }
    if (func == func_0203c060) {
        return 4;
    }
    if (func == func_0203c064) {
        return 5;
    }
    return 2;
}

static void func_0203c464(NetState *state) {
    func_0203d788();
    func_0203ad80(_pNetState, func_0203c0f4, 0, 1591);
}

void func_0203c494(NetState *state) {
    GFLNetSys *sys = func_0203d788();
    BOOL a0 = FALSE;

    func_020394dc(0);
    if (state->unk4B == 0) {
        a0 = TRUE;
    }
    if (sys->pDevTable->unk54(a0, 0)) {
        func_02037d78();
        func_020398b4(sys);
        func_02039834(sys);
        func_0203ad80(_pNetState, func_0203c464, 0, 1615);
        return;
    }
    if (sys->pDevTable->update(0) < 0) {
        func_0203ad80(_pNetState, func_0203c05c, 0, 1621);
    }
}


void func_0203c554(int a0) {
    if (_pNetState == NULL) {
        return;
    }
    if (a0 != 0) {
        if (func_02039338() == 0) {
            _pNetState->unk4B = 0;
        } else {
            _pNetState->unk4B = 1;
        }
    } else {
        _pNetState->unk4B = 0;
    }
    func_0203ad80(_pNetState, func_0203c494, 0, 1649);
    func_0203c494(_pNetState);
}



void func_0203c5c8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_0203c554(TRUE);
    _pNetState->unk4A = TRUE;
}

u8 func_0203c5ec(void) {
    return _pNetState->unk4A;
}

BOOL func_0203c600(void) {
    if (_pNetState->func == func_0203c0f4) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_0203c628(void) {
    if (_pNetState->func == func_0203c0f4 || _pNetState->func == func_0203c464) {
        return TRUE;
    }
    return FALSE;
}

void func_0203c65c(NetState *state) {
    GFLNetSys *sys;

    func_0203d798();
    sys = func_0203d788();
    sys->pDevTable->update(1);
    if (sys->pDevTable->unk78()) {
        sys->pDevTable->unk14(0);
        func_0203aeec(state);
        return;
    }
    if (!sys->pDevTable->unk6C()) {
        sys->pDevTable->unk14(0);
        func_0203aeec(state);
        return;
    }
    if (sys->pDevTable->unk74()) {
        sys->pDevTable->unk14(0);
        func_0203aeec(state);
    }
}


void func_0203c710(NetState *state) {
    GFLNetSys *sys = func_0203d788();

    sys->pDevTable->update(0);
    if (sys->pDevTable->unk10(1, 0) == 0) {
        return;
    }
    func_0203ad80(_pNetState, func_0203c65c, 0, 1794);
}

void func_0203c774(void (*callback)(void *work)) {
    GFLNetSys *sys;

    if (_pNetState == NULL) {
        return;
    }
    sys = func_0203d788();
    func_020351a0(1);
    _pNetState->endCallback = callback;
    sys->pDevTable->unk54(1, 0);
    func_0203ad80(_pNetState, func_0203c710, 5, 1817);
}

void func_0203c7e0(NetState *state) {
    int result = func_0203d788()->pDevTable->update(0);

    if (result < 0) {
        func_0203c0dc(state, result);
        func_0203ad80(_pNetState, func_0203c05c, 0, 1838);
        return;
    }
    if (result > 0) {
        func_0203ad80(_pNetState, func_0203c464, 0, 1847);
    }
}




void func_0203c864(NetState *state) {
    GFLNetInitData *init = func_0203d798();

    if (!func_02037700(512, init->heapId)) {
        return;
    }
    func_0203ad80(_pNetState, func_0203c7e0, 0, 1866);
}


void func_0203c8a8(void) {
    if (func_020392ec()) {
        return;
    }
    func_0203d798();
    func_0203516c(1);
    func_0203ad80(_pNetState, func_0203c864, 0, 1888);
}


void func_0203c8e8(u8 a0) {
    if (_pNetState != NULL) {
        _pNetState->unk4F = a0;
    }
}

void func_0203c900(int a0) {
    GFLNetSys *sys = func_0203d788();

    if (_pNetState != NULL) {
        _pNetState->unk50 = a0;
    }
    sys->pDevTable->unk8C(a0);
}


static void func_0203c934(void) {
}

void func_02039548(void);
void func_02039578(void);



BOOL func_0203c938(u8 errorFlag) {
    if (_pNetState != NULL) {
        _pNetState->errorFlag = errorFlag;
        func_02039548();
        func_02039578();
        return TRUE;
    }
    return FALSE;
}


void func_0203c968(void) {
    if (_pNetState != NULL) {
        _pNetState->errorFlag = 0;
        func_02039560();
    }
}


u8 func_0203c990(void) {
    if (_pNetState != NULL) {
        return _pNetState->errorFlag;
    }
    return 0;
}

void func_0203c9ac(int a0, u32 a1, u32 a2, int a3) {
    if (_pNetState == NULL) {
        return;
    }
    if ((u32)(a0 - 1005) > 1) {
        a0 = -a0;
    }
    _pNetState->error.code = a0;
    _pNetState->error.unk4 = a1;
    _pNetState->error.unk8 = a2;
    _pNetState->error.type = a3;
    if (a3 != 0) {
        func_02039548();
    }
}


void func_0203ca04(void) {
    if (_pNetState != NULL) {
        GFLNetInitData *pNetIni = func_0203d798();
        BOOL isWifi = pNetIni->bNetType == GFL_NET_TYPE_WIFI || pNetIni->bNetType==GFL_NET_TYPE_WIFI_LOBBY || pNetIni->bNetType==GFL_NET_TYPE_WIFI_GTS;

        if (!isWifi) {
            GFL_DebugAssertFail(__FILE__, 2231, "pNetIni->bNetType == GFL_NET_TYPE_WIFI || pNetIni->bNetType==GFL_NET_TYPE_WIFI_LOBBY || pNetIni->bNetType==GFL_NET_TYPE_WIFI_GTS");
        }
        _pNetState->error.code = 0;
        _pNetState->error.unk4 = 0;
        _pNetState->error.unk8 = 0;
        _pNetState->error.type = 0;
    }
}

GFLNetErrorInfo *func_0203ca98(void) {
    GFL_ASSERT_AT(2250, _pNetState);
    return &_pNetState->error;
}

static void func_0203cadc(NetState *state) {
    // The expression text as the original's preprocessor kept it: 0 && "_wifiBattleError<Shift-JIS>\n"
    GFL_DebugAssertFail(__FILE__, 2264, "0 && \"_wifiBattleError\x81\x46\x96\xa2\x8d\xec\x90\xac\\n\"");
}

BOOL func_0203cb00(void) {
    if (_pNetState != NULL && _pNetState->func == func_0203cadc) {
        return TRUE;
    }
    return FALSE;
}
