// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov030/net_whpipe.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "gfl/net_whpipe.h"
#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "gfl/wih.h"
#include "nitro/os.h"
#include "nitro/wm.h"
#include "system/game_beacon.h"

// net_whpipe.c is named after its embedded string. It is the GFL net's wireless pipe over wih.c: it keeps the beacons
// that the scan found, sends the game's own, and runs the steps of starting as a parent or a child and of ending.
// The names are guesses from the code, except those of NitroSDK's WM and WH

// The most that the game's data in a beacon can be
#define _BEACON_USER_SIZE_MAX 0x68

// How many beacons the pipe keeps, and for how long (in frames) after the last time one was heard
#define BEACON_MAX 16
#define BEACON_LIFETIME 600

// What the game gives to tell the beacons of its game: this machine's game command base, the beacon's and the work
typedef BOOL (*NetWhpipeGameFilter)(u8 gameCommandBase, u8 beaconCommandBase, void *work);

// What the game puts in the user part of a beacon, after a CRC-16 of the rest
typedef struct {
    u16 crc;
    // The low 16 bits of the game's ID
    u16 gameId;
    u8 gameCommandBase;
    // Must be 0
    u8 unk5;
    // How many children are connected
    u8 childCount;
    // Whether the parent no longer takes children
    u8 closed;
    u8 data[_BEACON_USER_SIZE_MAX];
} NetWhpipeBeaconInfo;

// A beacon as the scan found it: NitroSDK's WMBssDesc, with its game info after
struct NetWhpipeBeacon {
    u8 unk00[4];
    u8 mac[6];
    u8 unk0A[0x2c];
    u16 channel;
    u8 unk38[0x18];
    NetWhpipeBeaconInfo info;
};

typedef struct {
    NetWhpipeState base;
    // What the game gets each machine's data with
    GFLNetRecvFunc recvCallback;
    NetWhpipeBeacon beacons[BEACON_MAX];
    u8 unkC08[0x30];
    void (*scanCallback)(void *arg, const void *beacon, u8 gameCommandBase, u8 childCount);
    void *scanArg;
    // How many frames are left of each beacon's life, 0 when its slot is free
    u16 beaconTimer[BEACON_MAX];
    void (*onConnect)(void);
    void *gameWork;
    u16 tgid;
    u16 startCount;
    u16 holdFrames;
    // The beacon that this machine sends as a parent
    NetWhpipeBeaconInfo info;
    u8 parentMac[6];
    u8 unkCE4;
    u8 unkCE5;
    // The channel to start the parent on, 0xff for the one that WH measures the quietest
    u8 channel;
    // How to end: 1 to finalize, 2 to end
    u8 endKind;
    u8 running;
    u8 unkCE9;
    // Whether the parent's beacon says that it no longer takes children
    u8 entryClosed;
    // Whether the beacon stopped
    u8 stopped;
    // Whether to stop the beacon when the children are gone
    u8 lockEnabled;
    u8 unkCED;
    u8 unkCEE;
    u8 beaconListChanged;
    u8 beaconCounter;
    u8 channelIndex;
    u8 waitTimer;
    u8 holding;
    u8 holdPause;
    u8 shutdownTimer;
    u8 pauseSearch;
    u8 unkCF7;
} NetWhpipeWork;

// pokebw2's sWork
static NetWhpipeWork *data_ov014_021797a4 = NULL;

static void func_ov014_02177614(void *arg);
static void func_ov014_02177660(void *arg);
void func_ov014_02177d58(NetWhpipeBeaconInfo *info, int size);
static int func_ov014_02177a74(void);
static int func_ov014_02177acc(void);
static void func_ov014_02177ff0(void *arg);

// pokebw2's NetWhpipe_SetState
void func_ov014_0217727c(NetWhpipeState *work, NetWhpipeStateFunc state) {
    work->state = state;
}

BOOL func_ov014_02177280(HeapID heapId, WHCallback callback, void *gameWork, BOOL unused) {
    NetWhpipeWork *work;
    GFLNetInitData *init;

    func_0203d798();
    init = func_0203d798();
    if (data_ov014_021797a4 != NULL) {
        return FALSE;
    }
    work = GFL_HeapAllocate(heapId, sizeof(NetWhpipeWork), TRUE, "net_whpipe.c", 0xed);
    data_ov014_021797a4 = work;
    work->channel = 0xff;
    if (!func_ov014_021790e4(heapId, callback, unused)) {
        return FALSE;
    }
    func_ov014_02178e1c(init->unk4C.id.gameId);
    work->gameWork = gameWork;
    work->endKind = 0;
    work->unkCE4 = 0;
    work->tgid = WM_GetNextTgid();
    return TRUE;
}


void func_ov014_02177310(GFLNetRecvFunc callback) {
    data_ov014_021797a4->recvCallback = callback;
}

BOOL func_ov014_0217731c(const void *beacon) {
    NetWhpipeWork *work = data_ov014_021797a4;
    const NetWhpipeBeaconInfo *info;
    GFLNetInitData *init;
    int commandBase;
    u16 gameId;

    init = func_0203d798();
    commandBase = init->gameCommandBase;
    gameId = init->unk4C.id.gameId;
    info = &((const NetWhpipeBeacon *)beacon)->info;

    if (info->closed != 0) {
        return FALSE;
    }
    if (info->unk5 != 0) {
        return FALSE;
    }
    if (info->crc != getCRC16(&info->gameId, sizeof(NetWhpipeBeaconInfo) - 2)) {
        return FALSE;
    }
    if (info->gameId != gameId) {
        return FALSE;
    }
    if (data_ov014_021797a4->scanCallback != NULL) {
        data_ov014_021797a4->scanCallback(data_ov014_021797a4->scanArg, beacon, info->gameCommandBase, info->childCount);
    }
    if (init->unk20 != NULL) {
        if (!((NetWhpipeGameFilter)init->unk20)(commandBase, info->gameCommandBase, work->gameWork)) {
            return FALSE;
        }
    } else if (commandBase != info->gameCommandBase) {
        return FALSE;
    }
    if (commandBase != 0x14 && info->childCount >= init->maxConnectNum && init->maxConnectNum > 1) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov014_021773e0(const void *beacon) {
    int i;
    NetWhpipeWork *work = data_ov014_021797a4;
    func_0203d798();
    const NetWhpipeBeacon *found = beacon;

    if (!func_ov014_0217731c(beacon)) {
        return FALSE;
    }
    for (i = 0; i < BEACON_MAX; i++) {
        if (work->beaconTimer[i] != 0 && GFL_STD_MemCmp(work->beacons[i].mac, found->mac, 6) == 0) {
            work->beaconTimer[i] = BEACON_LIFETIME;
            MI_CpuCopy8(beacon, &work->beacons[i], sizeof(NetWhpipeBeacon));
            return TRUE;
        }
    }
    for (i = 0; i < BEACON_MAX; i++) {
        if (work->beaconTimer[i] == 0) {
            break;
        }
    }
    if (i >= BEACON_MAX) {
        return FALSE;
    }
    work->beaconTimer[i] = BEACON_LIFETIME;
    MI_CpuCopy8(beacon, &work->beacons[i], sizeof(NetWhpipeBeacon));
    work->beaconListChanged = TRUE;
    return TRUE;
}

BOOL func_ov014_02177494(const NetWhpipeBeacon *beacon, u32 flags) {
    NetWhpipeWork *work = data_ov014_021797a4;
    GFLNetInitData *init = func_0203d798();
    const NetWhpipeBeaconInfo *info = &beacon->info;
    u16 gameCommandBase = init->gameCommandBase;
    u16 gameId = init->unk4C.id.gameId;

    if ((flags & 1) && info->closed != 0) {
        return FALSE;
    }
    if ((flags & 2) && info->unk5 != 0) {
        return FALSE;
    }
    if ((flags & 4) && info->crc != getCRC16(&info->gameId, sizeof(NetWhpipeBeaconInfo) - 2)) {
        return FALSE;
    }
    if ((flags & 8) && info->gameId != gameId) {
        return FALSE;
    }
    if (flags & 0x10) {
        if (init->unk20 != NULL) {
            if (!((NetWhpipeGameFilter)init->unk20)(gameCommandBase, info->gameCommandBase, work->gameWork)) {
                return FALSE;
            }
        } else if (gameCommandBase != info->gameCommandBase) {
            return FALSE;
        }
    }
    if ((flags & 0x20) && info->childCount >= init->maxConnectNum) {
        return FALSE;
    }
    return TRUE;
}




void func_ov014_02177560(void) {
    NetWhpipeWork *work = data_ov014_021797a4;
    int i;

    for (i = 0; i < BEACON_MAX; i++) {
        work->beaconTimer[i] = 0;
    }
    MI_CpuFill8(work->beacons, 0, sizeof(work->beacons));
}

// pokebw2's ResetFlags
static void func_ov014_0217758c(NetWhpipeWork *work) {
    work->entryClosed = FALSE;
    work->beaconListChanged = FALSE;
    work->stopped = FALSE;
    work->lockEnabled = FALSE;
    work->unkCE9 = FALSE;
    work->running = FALSE;
}

// pokebw2's RecvCallback
static BOOL func_ov014_021775ac(u16 netId, u8 *data, u16 size) {
    return data_ov014_021797a4->recvCallback(netId, data, size);
}

BOOL func_ov014_021775bc(BOOL clearBeacons) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (work == NULL) {
        return FALSE;
    }
    func_ov014_0217758c(work);
    if (clearBeacons) {
        func_ov014_02177560();
    }
    func_ov014_0217933c(func_ov014_021775ac);
    work->running = TRUE;
    if (func_ov014_02178e4c() == WH_SYSSTATE_IDLE && func_ov014_021785b4(func_ov014_021773e0, NULL, 0)) {
        return TRUE;
    }
    return FALSE;
}


// pokebw2's StateEnd
static void func_ov014_02177614(void *arg) {
    NetWhpipeWork *work = arg;

    if (func_ov014_02178e4c() == WH_SYSSTATE_IDLE && func_ov014_02179498()) {
        work->shutdownTimer = 0xff;
        func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177660);
    }
    if (func_ov014_02178e4c() == WH_SYSSTATE_ERROR) {
        work->shutdownTimer = 0xff;
        func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177660);
    }
}

// pokebw2's StateWaitEnd
static void func_ov014_02177660(void *arg) {
    NetWhpipeWork *work = arg;

    work->shutdownTimer--;
    if (func_ov014_02178e4c() == WH_SYSSTATE_STOP || func_ov014_02178e4c() == WH_SYSSTATE_IDLE) {
        func_ov014_0217727c(&data_ov014_021797a4->base, NULL);
        return;
    }
    if (func_ov014_02178e4c() == WH_SYSSTATE_ERROR) {
        func_ov014_0217945c();
        func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177614);
        return;
    }
    if (work->shutdownTimer == 0) {
        func_ov014_0217945c();
        func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177614);
    }
}

BOOL func_ov014_021776c4(void) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (work == NULL) {
        return TRUE;
    }
    if (work->endKind == 0 && func_ov014_02179498()) {
        work->shutdownTimer = 0xff;
        func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177660);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's FreeWork
static void func_ov014_02177704(void) {
    if (data_ov014_021797a4 != NULL) {
        GFL_HeapFree(data_ov014_021797a4);
        data_ov014_021797a4 = NULL;
    }
}

BOOL func_ov014_0217771c(const void *beacon) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (func_ov014_0217731c(beacon) == 0) {
        return FALSE;
    }
    if (work->onConnect != NULL) {
        work->onConnect();
    }
    return TRUE;
}


// pokebw2's TickBeaconTimers
static void func_ov014_02177744(void) {
    NetWhpipeWork *work = data_ov014_021797a4;
    int i;

    for (i = 0; i < BEACON_MAX; i++) {
        if (work->beaconTimer[i] != 0 && work->beaconTimer[i] > 0) {
            work->beaconTimer[i]--;
            if (work->beaconTimer[i] == 0) {
                work->beaconListChanged = TRUE;
            }
        }
    }
}

void func_ov014_0217778c(void) {
    GFLNetInitData *init;
    NetWhpipeBeaconInfo *info;
    NetWhpipeWork *work;
    int size;

    if (func_0203cd98()) {
        work = data_ov014_021797a4;
        init = func_0203d798();
        if (init->unk1C != NULL) {
            size = init->unk1C(work->gameWork);
            if (size >= _BEACON_USER_SIZE_MAX) {
                OS_Terminate();
                return;
            }
            info = &work->info;
            info->gameCommandBase = init->gameCommandBase;
            info->gameId = init->unk4C.id.gameId;
            info->unk5 = 0;
            info->closed = work->entryClosed;
            MI_CpuCopy8(init->unk18(work->gameWork), info->data, size);
            func_ov014_02177d58(&work->info, 0x70);
        }
    }
}


// pokebw2's UpdateBeacon
static void func_ov014_021777fc(void) {
    NetWhpipeWork *work = data_ov014_021797a4;
    NetWhpipeBeaconInfo *info = &work->info;
    GFLNetInitData *init = func_0203d798();

    if (init->unk1C != NULL) {
        int size = init->unk1C(work->gameWork);

        GFL_ASSERT(size <= _BEACON_USER_SIZE_MAX);
        MI_CpuCopy8(init->unk18(work->gameWork), info->data, size);
    }
    if (info->childCount != func_ov014_02177a74()) {
        work->beaconCounter = 0;
        info->childCount = func_ov014_02177a74();
    }
    if (work->beaconCounter == 0) {
        func_ov014_02177d58(&work->info, sizeof(NetWhpipeBeaconInfo));
        work->beaconCounter = 2;
    }
    work->beaconCounter--;
}

// pokebw2's Update
static void func_ov014_02177888(u16 count) {
    int state = func_ov014_02178e4c();
    NetWhpipeWork *work = data_ov014_021797a4;

    if (!func_ov014_02177aec()) {
        func_ov014_021777fc();
    }
    if (work->startCount == 0xffff) {
        work->startCount = count;
    }
    if (work->lockEnabled) {
        if (!func_ov014_021795a4() && !func_ov014_02177acc()) {
            work->stopped = TRUE;
        }
        if (work->startCount > count) {
            work->stopped = TRUE;
        }
    }
    if (func_ov014_02178e68() == 0x19) {
        func_0203d720(0);
    }
    switch (state) {
    case WH_SYSSTATE_STOP:
        if (work->endKind == 1) {
            func_ov014_02177704();
        }
        break;
    case WH_SYSSTATE_IDLE:
        if (work->endKind == 1) {
            if (func_ov014_02179570(NULL)) {
                return;
            }
        }
        if (work->endKind == 2) {
            if (func_ov014_02179570(NULL)) {
                return;
            }
        }
        break;
    case WH_SYSSTATE_CONNECT_FAIL:
    case WH_SYSSTATE_ERROR:
        if (work != NULL) {
            if (!work->stopped) {
                func_0203d754(func_ov014_02178e68());
            }
            work->stopped = TRUE;
        }
        break;
    }
}

void func_ov014_0217796c(u16 count) {
    if (data_ov014_021797a4 != NULL && data_ov014_021797a4->base.state != NULL) {
        data_ov014_021797a4->base.state(data_ov014_021797a4);
    }
    if (data_ov014_021797a4 != NULL) {
        func_ov014_02177888(count);
    }
    if (data_ov014_021797a4 != NULL && func_ov014_02177c1c()) {
        func_ov014_02177744();
    }
    if (data_ov014_021797a4 != NULL) {
        if (data_ov014_021797a4->holdFrames != 0) {
            func_ov014_02179664(TRUE);
            data_ov014_021797a4->holding = TRUE;
            if (data_ov014_021797a4->holdPause == 0) {
                data_ov014_021797a4->holdFrames--;
            }
        } else {
            func_ov014_02179664(FALSE);
            data_ov014_021797a4->holding = FALSE;
        }
    }
    func_ov014_021793f8();
}

BOOL func_ov014_021779f4(void) {
    if (data_ov014_021797a4 == NULL) {
        return FALSE;
    }
    if (!func_ov014_021795a4() && !func_ov014_02177acc()) {
        return FALSE;
    }
    if (func_ov014_02178e4c() == WH_SYSSTATE_CONNECTED || func_ov014_02178e4c() == WH_SYSSTATE_DATASHARING) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IsConnected
static BOOL func_ov014_02177a34(u16 netId) {
    if (data_ov014_021797a4 == NULL) {
        return FALSE;
    }
    if (func_ov014_02178e4c() != WH_SYSSTATE_CONNECTED && func_ov014_02178e4c() != WH_SYSSTATE_DATASHARING) {
        return FALSE;
    }
    if (func_ov014_02178e38() & (1 << netId)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CountChildren
static int func_ov014_02177a74(void) {
    int count = 0;
    int i;

    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        if (func_ov014_02177a34(i)) {
            count++;
        }
    }
    return count;
}

BOOL func_ov014_02177a94(void) {
    if (data_ov014_021797a4 != NULL) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov014_02177aa8(void) {
    if (data_ov014_021797a4 != NULL) {
        if (func_ov014_02178e4c() == WH_SYSSTATE_IDLE) {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

// pokebw2's GetChildrenBitmap
static int func_ov014_02177acc(void) {
    if (data_ov014_021797a4 != NULL) {
        return func_ov014_02178e38() & 0xfffe;
    }
    return 0;
}

BOOL func_ov014_02177aec(void) {
    if (data_ov014_021797a4 != NULL && data_ov014_021797a4->stopped) {
        return TRUE;
    }
    return FALSE;
}

void func_ov014_02177b0c(int lock) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (work != NULL) {
        work->lockEnabled = lock;
        work->startCount = 0xffff;
    }
}

void *func_ov014_02177b2c(int index) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (work != NULL && work->beaconTimer[index] != 0) {
        NetWhpipeBeaconInfo *info = &work->beacons[index].info;

        return info->data;
    }
    return NULL;
}

u8 *func_ov014_02177b58(int index) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (work != NULL && work->beaconTimer[index] != 0) {
        return work->beacons[index].mac;
    }
    return NULL;
}

u8 func_ov014_02177b80(int index) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (work != NULL && work->beaconTimer[index] != 0) {
        return work->beacons[index].info.gameCommandBase;
    }
    return 0;
}

void func_ov014_02177bac(int index) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (work != NULL && work->beaconTimer[index] != 0) {
        work->beaconTimer[index] = 0;
    }
}

void func_ov014_02177bcc(void) {
    NetWhpipeWork *work = data_ov014_021797a4;
    int i;

    if (work != NULL) {
        for (i = 0; i < BEACON_MAX; i++) {
            work->beaconTimer[i] = 0;
        }
    }
}

void func_ov014_02177bf4(int index) {
    func_ov014_02177bac(index);
}


u8 *func_ov014_02177bfc(NetWhpipeBeacon *beacon, u8 *gameCommandBase) {
    NetWhpipeBeaconInfo *info = &beacon->info;
    *gameCommandBase = info->gameCommandBase;
    return info->data;
}

BOOL func_ov014_02177c08(void) {
    if (func_ov014_02179614() != 0) {
        return TRUE;
    }
    return FALSE;
}



BOOL func_ov014_02177c1c(void) {
    if (func_ov014_02178e4c() == WH_SYSSTATE_SCANNING) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov014_02177c30(u8 *data, int size, BOOL (*callback)(BOOL ok)) {
    return func_ov014_02179368(data, size, callback);
}

int func_ov014_02177c3c(void) {
    return func_ov014_02178e38();
}

int func_ov014_02177c44(void) {
    return func_ov014_021795a4();
}

// pokebw2's StateStartParent
static void func_ov014_02177c4c(void *arg) {
    NetWhpipeWork *work = arg;
    int state = func_ov014_02178e4c();
    u16 modes[2] = { 4, 0 };

    if (state == WH_SYSSTATE_MEASURECHANNEL) {
        GFLNetInitData *init = func_0203d798();
        u16 channel;

        work->entryClosed = FALSE;
        work->beaconListChanged = FALSE;
        work->unkCE9 = FALSE;
        work->running = FALSE;
        channel = func_ov014_02179024();
        if (work->channel == 0xff) {
            work->channel = channel;
        } else {
            channel = work->channel;
        }
        if (init->unk66 != 0) {
            work->tgid = WM_GetNextTgid();
        }
        func_ov014_0217778c();
        func_ov014_02179288(modes[init->bMPMode], work->tgid, channel, init->maxConnectNum - 1);
        func_ov014_0217727c(&data_ov014_021797a4->base, NULL);
    }
}

// pokebw2's StateMeasureChannel
static void func_ov014_02177ce4(void *arg) {
    if (func_ov014_02178e4c() == WH_SYSSTATE_IDLE && func_ov014_02178e84()) {
        func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177c4c);
    }
}

BOOL func_ov014_02177d0c(BOOL anyChannel) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (work->base.state != NULL) {
        return FALSE;
    }
    if (anyChannel) {
        work->channel = 0xff;
    }
    func_ov014_0217933c(func_ov014_021775ac);
    data_ov014_021797a4->running = TRUE;
    func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177ce4);
    return TRUE;
}

void func_ov014_02177d58(NetWhpipeBeaconInfo *info, int size) {
    NetWhpipeWork *work = data_ov014_021797a4;
    GFLNetInitData *init = func_0203d798();

    if (work != NULL && init != NULL) {
        info->crc = getCRC16(&info->gameId, size - 2);
        DC_FlushRange(info, size);
        if (func_ov014_02178e4c() == WH_SYSSTATE_IDLE) {
            func_ov014_02178e28(info, (u16)size);
        } else {
            func_ov014_021795d4(info, size, init->unk4C.id.gameId, work->tgid);
        }
    }
}

void func_ov014_02177db0(void *arg) {
    GFLNetInitData *init = func_0203d798();
    u16 modes[2] = { 5, 1 };

    if (func_ov014_02178e4c() == WH_SYSSTATE_IDLE || func_ov014_02178e4c() == WH_SYSSTATE_SCANNING) {
        func_ov014_02178524(modes[init->bMPMode], data_ov014_021797a4->parentMac, 0);
        func_ov014_021795c0(func_ov014_0217771c);
        func_ov014_0217933c(func_ov014_021775ac);
        data_ov014_021797a4->running = TRUE;
        func_ov014_0217727c(&data_ov014_021797a4->base, NULL);
    }
}

BOOL func_ov014_02177e20(BOOL unused, const u8 *mac, int index, void (*callback)(void)) {
    func_0203d798();
    if (data_ov014_021797a4->base.state != NULL) {
        return FALSE;
    }
    if (func_ov014_02178e4c() == WH_SYSSTATE_SCANNING) {
        data_ov014_021797a4->unkCE9 = FALSE;
        func_ov014_021788fc();
    }
    func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177db0);
    if (mac != NULL) {
        MI_CpuCopy8(mac, data_ov014_021797a4->parentMac, 6);
    } else {
        MI_CpuCopy8(data_ov014_021797a4->beacons[index].mac, data_ov014_021797a4->parentMac, 6);
    }
    data_ov014_021797a4->onConnect = callback;
    return TRUE;
}




// pokebw2's StateFinish
static void func_ov014_02177e9c(void *arg) {
    if (func_ov014_02178e4c() == WH_SYSSTATE_STOP) {
        func_ov014_02179158();
        func_ov014_02177704();
    }
}

BOOL func_ov014_02177eb0(WHCallback callback) {
    if (func_ov014_02179570(callback)) {
        func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177e9c);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's AcceptAll
static BOOL func_ov014_02177ed4(const void *info, void *work) {
    return TRUE;
}

// pokebw2's AcceptNone
static BOOL func_ov014_02177ed8(const void *info, void *work) {
    return FALSE;
}

void func_ov014_02177edc(BOOL acceptAll) {
    if (acceptAll) {
        func_ov014_02179328(func_ov014_02177ed4);
    } else {
        func_ov014_02179328(func_ov014_02177ed8);
    }
}

// pokebw2's StateParentWait
void func_ov014_02177efc(void *arg) {
    NetWhpipeWork *work = arg;

    if (!data_ov014_021797a4->holding) {
        if (work->waitTimer != 0) {
            work->waitTimer--;
        }
        if (work->waitTimer == 0 && func_ov014_02179498()) {
            func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177ff0);
        }
    }
}

void func_ov014_02177f48(void *arg) {
    NetWhpipeWork *work = arg;

    if (func_ov014_02177aa8()) {
        u8 channels[3] = { 1, 7, 13 };

        work->channelIndex = GFL_RandomLC(3);
        if (work->channelIndex >= 3) {
            work->channelIndex = 0;
        }
        func_ov014_0217778c();
        if (func_ov014_02179288(0, work->tgid, channels[work->channelIndex], 1)) {
            work->waitTimer = GFL_RandomLC(0x98) + 0x3c;
            func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177efc);
        }
    }
}


static void func_ov014_02177fc4(void *arg) {
    NetWhpipeWork *work = arg;

    work->waitTimer--;
    if (work->waitTimer == 0) {
        func_ov014_021788fc();
        func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177f48);
    }
}

static void func_ov014_02177ff0(void *arg) {
    NetWhpipeWork *work = arg;

    if (func_ov014_02178e4c() == WH_SYSSTATE_IDLE) {
        if (func_ov014_021785b4(func_ov014_021773e0, NULL, 0)) {
            work->waitTimer = GFL_RandomLC(0x28) + 0x3c;
            func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177fc4);
        }
    }
}



BOOL func_ov014_02178034(void) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (work == NULL) {
        return FALSE;
    }
    func_ov014_021780e4(0);
    func_ov014_0217758c(work);
    func_ov014_02177560();
    func_ov014_0217933c(func_ov014_021775ac);
    work->running = TRUE;
    func_ov014_0217727c(&data_ov014_021797a4->base, func_ov014_02177ff0);
    return TRUE;
}

void func_ov014_0217807c(void) {
    if (data_ov014_021797a4->lockEnabled) {
        data_ov014_021797a4->stopped = TRUE;
    }
}

u8 func_ov014_02178098(u8 index) {
    NetWhpipeWork *work = data_ov014_021797a4;

    if (work != NULL && work->beaconTimer[index] != 0) {
        return work->beacons[index].info.gameCommandBase;
    }
    return 0;
}

void func_ov014_021780c4(int index) {
    func_ov014_02178714(data_ov014_021797a4->beacons[index].channel, data_ov014_021797a4->beacons[index].mac);
}

void func_ov014_021780e4(int unused) {
    if (data_ov014_021797a4 != NULL) {
        data_ov014_021797a4->holdFrames = 20;
    }
}

void func_ov014_021780fc(u32 enabled) {
    if (data_ov014_021797a4 != NULL) {
        data_ov014_021797a4->entryClosed = enabled;
        func_ov014_0217778c();
    }
}

void func_ov014_02178118(void (*callback)(void *arg, const void *beacon, u8 gameCommandBase, u8 childCount),
                         void *arg) {
    if (data_ov014_021797a4 != NULL) {
        data_ov014_021797a4->scanCallback = callback;
        data_ov014_021797a4->scanArg = arg;
    }
}
