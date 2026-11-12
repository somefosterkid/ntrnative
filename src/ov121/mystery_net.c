// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov197/mystery_net.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mystery/mystery_net.h"
#include "constants/version.h"
#include "dwc/dwc.h"
#include "dwc/nd.h"
#include "field/delivery_beacon.h"
#include "field/delivery_irc.h"
#include "gfl/dwc_rap.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_state.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "nitro/gx.h"
#include "save/mystery_gift.h"
#include "system/version.h"

// Mystery Gift's receiving of gifts: by local wireless beacons and by infrared through overlay 12's delivery_beacon.c
// and delivery_irc.c, and by Wi-Fi through NitroDWC's download library. Our names; swan has none for this overlay

#define MYSTERY_NET_FILE_MAX 10
#define MYSTERY_NET_BUFFER_SIZE 0x1000

typedef struct MysteryNetSeq MysteryNetSeq;

typedef void (*MysteryNetSeqFunc)(MysteryNetSeq *seq, int *state, void *work);

// Black 2's, not checked against Black
struct MysteryNetSeq {
    MysteryNetSeqFunc func;
    BOOL end;
    int state;
    void *work;
};

// The states of a Wi-Fi download that wait for the library
#define DOWNLOAD_WAIT 100
#define DOWNLOAD_WAIT_CLEANUP 200

// Black 2's; twinned code confirms the offsets of nextState, cancel, timeout and initialized in Black
typedef struct {
    DWCNdFileInfo files[MYSTERY_NET_FILE_MAX];
    BOOL available[MYSTERY_NET_FILE_MAX];
    // Where a wait goes on to
    u32 nextState;
    BOOL cancel;
    BOOL cancelled;
    int count;
    u32 percent;
    u32 received;
    u32 contentLength;
    u32 selected;
    void *buffer;
    u32 timeout;
    BOOL initialized;
} MysteryNetDownload;

// Black 2's; twinned code confirms the offsets of seq, wifiWork, beaconFlags, heapId, exited, download, save, beacon,
// irc, buffer and result, and its size in Black
struct MysteryNet {
    MysteryNetSeq seq;
    void *wifiWork;
    u8 info[0x20];
    u8 unk34[0x20];
    u32 beaconFlags;
    HeapID heapId;
    BOOL exited;
    MysteryNetDownload download;
    SaveControl *save;
    void *beacon;
    void *irc;
    u32 unk7A0;
    u8 buffer[MYSTERY_NET_BUFFER_SIZE];
    u32 result;
};

// What the download library's callback last gave
// Black 2's, not checked against Black
typedef struct {
    BOOL done;
    BOOL ready;
    u32 unk8;
    u32 error;
} MysteryNetNdState;

// A hexadecimal digit and its value
// Black 2's, not checked against Black
typedef struct {
    s8 c;
    s8 value;
} MysteryHexDigit;

static u32 func_ov121_021de570(MysteryNet *net);
static u32 func_ov121_021de5d8(MysteryNet *net);
static u32 func_ov121_021de5f8(MysteryNet *net);
static void func_ov121_021de618(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021de61c(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021de690(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021de6f8(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021de70c(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021de7bc(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021de7fc(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021de830(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021de8d0(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021de910(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021de958(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021dedc8(MysteryNetSeq *seq, int *state, void *work);
static void func_ov121_021dee14(MysteryNetDownload *download, int *state, u32 nextState);
static void func_ov121_021dee38(MysteryNetDownload *download, u32 error, int *state, u32 nextState,
                                       int failState);
static void func_ov121_021dee68(MysteryNetSeq *seq, void *work, MysteryNetSeqFunc func);
static void func_ov121_021dee84(MysteryNetSeq *seq);
static void func_ov121_021dee90(MysteryNetSeq *seq);
static void func_ov121_021deea4(MysteryNetSeq *seq, MysteryNetSeqFunc func);
static BOOL func_ov121_021deeac(MysteryNetSeq *seq, MysteryNetSeqFunc func);
static void *func_ov121_021deebc(void *work);
static int func_ov121_021deec0(void *work);
static BOOL func_ov121_021deec4(u32 a, u32 b);
static void func_ov121_021deed0(void *work);
static void func_ov121_021deed8(u32 reason, u32 error, int serverError);
static void func_ov121_021deeec(void);
static u32 func_ov121_021deef8(const char *str);
static BOOL func_ov121_021def38(const u32 *data, u32 size);
static u32 func_ov121_021def54(void *work, int a1, int event, int a3);

// pokebw2's sHexDigits
static const union { u8 raw[48]; MysteryHexDigit t[23]; } data_ov121_021dfa8c_raw = { {
#include "asset/mystery_net_data_ov121_021dfa8c.inc"
} };
typedef char data_ov121_021dfa8c_fits[sizeof(data_ov121_021dfa8c_raw.t) <= sizeof(data_ov121_021dfa8c_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov121_021dfa8c (data_ov121_021dfa8c_raw.t)

// pokebw2's sNetInit
static const GFLNetInitData data_ov121_021dfabc = {
    NULL,
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    func_ov121_021deebc,
    func_ov121_021deec0,
    func_ov121_021deec4,
    NULL,
    NULL,
    NULL,
    NULL,
    { 0 },
    NULL,
    NULL,
    0,
    { 1, 0, 0, 0, 0x80, 0x13, 0, 0 },
    HEAPID_USER,
    0xd,
    0xf,
    0xd,
    0xf0,
    0,
    1,
    0x58,
    4,
    1,
    0,
    0,
    0,
    9,
    { 0x2c, 1, 0, 0 },
    0x1f4,
    0,
};

// pokebw2's sNdState
static MysteryNetNdState data_ov121_021dfc00;

// pokebw2's MysteryNet_Create
MysteryNet *func_ov121_021de2a0(SaveControl *save, HeapID heapId) {
    MysteryNet *net = GFL_HeapAllocate(heapId, sizeof(MysteryNet), FALSE, "mystery_net.c", 315);

    MI_CpuFill8(net, 0, sizeof(MysteryNet));
    net->heapId = heapId;
    net->save = save;
    func_ov121_021dee68(&net->seq, net, func_ov121_021de618);
    return net;
}

// pokebw2's MysteryNet_Delete
void func_ov121_021de2f0(MysteryNet *net) {
    func_ov121_021dee84(&net->seq);
    GFL_HeapFree(net);
}

// pokebw2's MysteryNet_Main
void func_ov121_021de300(MysteryNet *net) {
    func_ov121_021dee90(&net->seq);
}

// pokebw2's MysteryNet_ChangeState
void func_ov121_021de308(MysteryNet *net, u32 state) {
    switch (state) {
    case MYSTERY_NET_STATE_WIRELESS_START:
        if (func_ov121_021deeac(&net->seq, func_ov121_021de618)) {
            func_ov121_021deea4(&net->seq, func_ov121_021de61c);
        }
        break;
    case MYSTERY_NET_STATE_WIRELESS_END:
        if (func_ov121_021deeac(&net->seq, func_ov121_021de6f8)) {
            func_ov121_021deea4(&net->seq, func_ov121_021de690);
        }
        break;
    case MYSTERY_NET_STATE_WIFI:
        if (func_ov121_021deeac(&net->seq, func_ov121_021de618)) {
            func_ov121_021deea4(&net->seq, func_ov121_021de958);
        }
        break;
    case MYSTERY_NET_STATE_WIFI_CANCEL:
        if (func_ov121_021deeac(&net->seq, func_ov121_021de958)) {
            net->download.cancel = TRUE;
        }
        break;
    case MYSTERY_NET_STATE_WIFI_END:
        func_ov121_021deea4(&net->seq, func_ov121_021dedc8);
        break;
    case MYSTERY_NET_STATE_BEACON_START:
        func_ov121_021deea4(&net->seq, func_ov121_021de70c);
        break;
    case MYSTERY_NET_STATE_BEACON_END:
        func_ov121_021deea4(&net->seq, func_ov121_021de7bc);
        break;
    case MYSTERY_NET_STATE_IRC_START:
        func_ov121_021deea4(&net->seq, func_ov121_021de830);
        break;
    case MYSTERY_NET_STATE_IRC_END:
        func_ov121_021deea4(&net->seq, func_ov121_021de8d0);
        break;
    case MYSTERY_NET_STATE_IDLE:
        func_ov121_021deea4(&net->seq, func_ov121_021de618);
        break;
    }
}

// pokebw2's MysteryNet_GetState
u32 func_ov121_021de3e0(MysteryNet *net) {
    if (func_ov121_021deeac(&net->seq, func_ov121_021de618)) {
        return MYSTERY_NET_STATE_IDLE;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021de61c)) {
        return MYSTERY_NET_STATE_WIRELESS_START;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021de6f8)) {
        return MYSTERY_NET_STATE_WIRELESS_READY;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021de690)) {
        return MYSTERY_NET_STATE_WIRELESS_END;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021de958)) {
        return MYSTERY_NET_STATE_WIFI;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021dedc8)) {
        return MYSTERY_NET_STATE_WIFI_END;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021de70c)) {
        return MYSTERY_NET_STATE_BEACON_START;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021de7fc)) {
        return MYSTERY_NET_STATE_BEACON_WAIT;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021de7bc)) {
        return MYSTERY_NET_STATE_BEACON_END;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021de830)) {
        return MYSTERY_NET_STATE_IRC_START;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021de910)) {
        return MYSTERY_NET_STATE_IRC_WAIT;
    }
    if (func_ov121_021deeac(&net->seq, func_ov121_021de8d0)) {
        return MYSTERY_NET_STATE_IRC_END;
    }
    return MYSTERY_NET_STATE_IDLE;
}

// pokebw2's MysteryNet_GetBeaconFlags
u32 func_ov121_021de4d8(MysteryNet *net) {
    return net->beaconFlags;
}

// pokebw2's MysteryNet_GetRecvData
u32 func_ov121_021de4dc(MysteryNet *net, void *buffer, u32 size) {
    if (net->result == MYSTERY_NET_RECV_OK) {
        MI_CpuCopy8(net->buffer, buffer, size);
        return MYSTERY_NET_RECV_OK;
    }
    return net->result;
}

// pokebw2's MysteryNet_GetError
u32 func_ov121_021de500(MysteryNet *net) {
    u32 error = MYSTERY_NET_ERROR_NONE;

    if (func_0203cd98()) {
        switch (func_0203d798()->bNetType) {
        case 0:
        case 5:
            error = func_ov121_021de5d8(net);
            break;
        case 1:
            error = func_ov121_021de570(net);
            break;
        case 3:
            error = func_ov121_021de5f8(net);
            break;
        }
    }
    return error;
}

// pokebw2's MysteryNet_ClearError
void func_ov121_021de550(MysteryNet *net) {
    if (net->wifiWork != NULL) {
        func_020103e4(net->wifiWork);
        net->wifiWork = NULL;
    }
    func_ov121_021de308(net, MYSTERY_NET_STATE_IDLE);
    func_0200fba0();
}

// pokebw2's MysteryNet_CheckErrorWifi
static u32 func_ov121_021de570(MysteryNet *net) {
    u32 error = MYSTERY_NET_ERROR_NONE;

    if (func_0200fb24()) {
        switch (func_0203ca98()->unk4) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            if (net->download.initialized == TRUE) {
                return error;
            }
            func_0203ca04();
            DWC_ClearError();
            func_0200fbb0();
            func_0200f83c();
            func_0200fba0();
            error = MYSTERY_NET_ERROR_FATAL;
            break;
        case 7:
            func_0203ca04();
            GFL_NetErrAbort();
            error = 3;
            break;
        }
    }
    return error;
}

// pokebw2's MysteryNet_CheckErrorWireless
static u32 func_ov121_021de5d8(MysteryNet *net) {
    u32 error = MYSTERY_NET_ERROR_NONE;

    if (func_0200fb24()) {
        func_0200fbb0();
        func_0200f83c();
        func_0200fba0();
        error = MYSTERY_NET_ERROR_FATAL;
    }
    return error;
}

// pokebw2's MysteryNet_CheckErrorIrc
static u32 func_ov121_021de5f8(MysteryNet *net) {
    u32 error = MYSTERY_NET_ERROR_NONE;

    if (func_0200fb24()) {
        func_0200fbb0();
        func_0200f83c();
        func_0200fba0();
        error = MYSTERY_NET_ERROR_FATAL;
    }
    return error;
}

// pokebw2's MysteryNet_Idle
static void func_ov121_021de618(MysteryNetSeq *seq, int *state, void *work) {
}

// pokebw2's MysteryNet_WirelessStart
static void func_ov121_021de61c(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;

    switch (*state) {
    case 0:
        func_0203cbac(&data_ov121_021dfabc, NULL, net);
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
        *state = 1;
        break;
    case 1:
        if (func_0203cd98()) {
            func_0203d34c(TRUE, net->heapId);
            net->wifiWork = func_02010360(4, net->heapId);
            func_0203d010();
            *state = 2;
        }
        break;
    case 2:
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
        func_ov121_021deea4(seq, func_ov121_021de6f8);
        break;
    }
}

// pokebw2's MysteryNet_WirelessEnd
static void func_ov121_021de690(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;

    switch (*state) {
    case 0:
        func_020103e4(net->wifiWork);
        net->wifiWork = NULL;
        *state = 1;
        break;
    case 1:
        if (func_0203ceac(func_ov121_021deed0)) {
            *state = 2;
        } else {
            *state = 3;
        }
        break;
    case 2:
        if (net->exited) {
            net->exited = FALSE;
            *state = 3;
        }
        break;
    case 3:
        func_ov121_021deea4(seq, func_ov121_021de618);
        break;
    }
}

// pokebw2's MysteryNet_WirelessReady
static void func_ov121_021de6f8(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;

    func_02010498();
    net->beaconFlags = func_0201058c(NULL);
}

// pokebw2's MysteryNet_BeaconStart
static void func_ov121_021de70c(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;
    DeliveryInit init;

    switch (*state) {
    case 0:
        MI_CpuFill8(&init, 0, sizeof(DeliveryInit));
        init.code = 9;
        init.data[0].datasize = sizeof(MysteryGiftRecvData);
        init.data[0].pData = net->buffer;
        init.data[0].region = region;
        init.data[0].mask = 1 << GAME_VERSION;
        init.dataNum = 1;
        init.flag4 = 0;
        init.heapId = net->heapId;
        net->beacon = func_ov010_021576d0(&init);
        func_ov010_021578a0(net->beacon);
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
        net->result = MYSTERY_NET_RECV_NONE;
        *state = 1;
        break;
    case 1:
        if (func_0203cd98()) {
            func_0203d34c(TRUE, net->heapId);
            GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
            *state = 2;
        }
        break;
    case 2:
        func_ov121_021deea4(seq, func_ov121_021de7fc);
        break;
    }
}

// pokebw2's MysteryNet_BeaconEnd
static void func_ov121_021de7bc(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;

    switch (*state) {
    case 0:
        func_ov010_02157938(net->beacon);
        *state = 1;
        break;
    case 1:
        if (func_0203d210()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov121_021deea4(seq, func_ov121_021de618);
        break;
    }
}

// pokebw2's MysteryNet_BeaconWait
static void func_ov121_021de7fc(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;

    func_ov010_02157928(net->beacon);
    if (func_ov010_02157910(net->beacon)) {
        net->result = MYSTERY_NET_RECV_OK;
        func_ov121_021deea4(seq, func_ov121_021de7bc);
    }
}

// pokebw2's MysteryNet_IrcStart
static void func_ov121_021de830(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;
    DeliveryIrcInit init;

    switch (*state) {
    case 0:
        MI_CpuFill8(&init, 0, sizeof(DeliveryIrcInit));
        init.code = 9;
        init.data[0].datasize = sizeof(MysteryGiftRecvData);
        init.data[0].pData = net->buffer;
        init.unk04 = 0;
        init.heapId = net->heapId;
        init.data[0].region = region;
        init.dataNum = 1;
        init.data[0].mask = 1 << GAME_VERSION;
        net->irc = func_ov010_02157dd8(&init);
        func_ov010_02157e34(net->irc);
        net->result = MYSTERY_NET_RECV_NONE;
        *state = 1;
        break;
    case 1:
        if (func_0203cd98()) {
            func_0203d34c(TRUE, net->heapId);
            *state = 2;
        }
        break;
    case 2:
        func_ov121_021deea4(seq, func_ov121_021de910);
        break;
    }
}

// pokebw2's MysteryNet_IrcEnd
static void func_ov121_021de8d0(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;

    switch (*state) {
    case 0:
        func_ov010_02157e8c(net->irc);
        *state = 1;
        break;
    case 1:
        if (func_0203d210()) {
            *state = 2;
        }
        break;
    case 2:
        func_ov121_021deea4(seq, func_ov121_021de618);
        break;
    }
}

// pokebw2's MysteryNet_IrcWait
static void func_ov121_021de910(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;
    u8 ret;

    func_ov010_02157e78(net->irc);
    ret = func_ov010_02157e6c(net->irc);
    if (ret != 0) {
        switch (ret) {
        case 1:
            net->result = MYSTERY_NET_RECV_OK;
            break;
        case 2:
        case 3:
            net->result = MYSTERY_NET_RECV_ERROR;
            break;
        }
        func_ov121_021deea4(seq, func_ov121_021de8d0);
    }
}

// pokebw2's MysteryNet_WifiDownload
static void func_ov121_021de958(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;
    MysteryNetDownload *download = &net->download;
    int i;

    switch (*state) {
    case 0:
        func_ov009_02156de0(func_ov121_021def54, download);
        MI_CpuFill8(download, 0, sizeof(MysteryNetDownload));
        download->cancel = FALSE;
        download->cancelled = FALSE;
        download->selected = 0;
        download->buffer = net->buffer;
        net->result = MYSTERY_NET_RECV_NONE;
        if (!func_ov114_021c5928(func_ov121_021deed8, "IRAO", "WX9x7Zh6J3aBC4zQ")) {
            net->result = MYSTERY_NET_RECV_ERROR;
            *state = 9;
            return;
        }
        download->initialized = TRUE;
        func_ov121_021dee14(download, state, 1);
        break;
    case 1:
        if (download->cancelled) {
            net->result = MYSTERY_NET_RECV_ERROR;
            func_ov121_021dee38(download, 3, state, 7, 7);
            return;
        }
        if (!func_ov114_021c5c04("MYSTERY_E", "", "")) {
            net->result = MYSTERY_NET_RECV_ERROR;
            func_ov121_021dee38(download, 4, state, 9, 9);
            return;
        }
        *state = 3;
        break;
    case 3:
        if (!func_ov114_021c5c38(download->files, 0, MYSTERY_NET_FILE_MAX)) {
            net->result = MYSTERY_NET_RECV_ERROR;
            func_ov121_021dee38(download, 4, state, 9, 9);
            return;
        }
        func_ov121_021dee14(download, state, 2);
        break;
    case 2:
        for (i = 0; i < MYSTERY_NET_FILE_MAX; i++) {
            if (!func_ov121_021def38((u32 *)&download->files[i], sizeof(DWCNdFileInfo))) {
                if (func_ov121_021deef8(download->files[i].param2) & (1 << GAME_VERSION)) {
                    download->available[i] = TRUE;
                } else {
                    download->available[i] = FALSE;
                }
            } else {
                download->available[i] = FALSE;
            }
        }
        download->count = 0;
        for (i = 0; i < MYSTERY_NET_FILE_MAX; i++) {
            if (download->available[i]) {
                download->count++;
            }
        }
        *state = 4;
        break;
    case 4:
        if (download->count == 1) {
            for (i = 0; i < MYSTERY_NET_FILE_MAX; i++) {
                if (download->available[i]) {
                    download->selected = i;
                    break;
                }
            }
        } else if (download->count > 1) {
            u32 n = 0;
            u32 pick = GFL_RandomMTRange(download->count);

            for (i = 0; i < MYSTERY_NET_FILE_MAX; i++) {
                if (download->available[i]) {
                    if (n == pick) {
                        download->selected = i;
                        break;
                    }
                    n++;
                }
            }
            download->selected = i;
        } else {
            net->result = MYSTERY_NET_RECV_ERROR;
            func_ov121_021dee38(download, 1, state, 8, 8);
            return;
        }
        *state = 5;
        break;
    case 5:
        if (!func_ov114_021c5cfc(&download->files[download->selected], download->buffer, MYSTERY_NET_BUFFER_SIZE)) {
            net->result = MYSTERY_NET_RECV_ERROR;
            func_ov121_021dee38(download, 4, state, 9, 9);
            return;
        }
        download->percent = 0;
        data_ov121_021dfc00.ready = FALSE;
        data_ov121_021dfc00.error = 0;
        download->timeout = 0;
        *state = 6;
        break;
    case 6:
        func_ov114_021c5ab8();
        if (download->timeout++ > 7200) {
            func_0203c9ac(0, 0, 0, 1013);
            net->result = MYSTERY_NET_RECV_ERROR;
            func_ov121_021dee38(download, 3, state, 7, 7);
        }
        if (!data_ov121_021dfc00.ready) {
            if (download->cancel) {
                net->result = MYSTERY_NET_RECV_ERROR;
                func_ov121_021dee38(download, 3, state, 7, 7);
                return;
            }
            if (func_ov114_021c5e14(&download->received, &download->contentLength) == TRUE) {
                u32 percent = download->received * 100 / download->contentLength;

                if (download->percent != percent) {
                    download->percent = percent;
                }
            }
        } else if (data_ov121_021dfc00.error != 0) {
            net->result = MYSTERY_NET_RECV_ERROR;
            func_ov121_021dee38(download, 4, state, 9, 9);
        } else if (!download->cancelled) {
            net->result = MYSTERY_NET_RECV_OK;
            func_ov121_021dee38(download, 1, state, 8, 8);
        } else {
            net->result = MYSTERY_NET_RECV_ERROR;
            func_ov121_021dee38(download, 3, state, 8, 8);
        }
        break;
    case 7:
        func_ov114_021c5db4();
        *state = 8;
        break;
    case 8:
        *state = 9;
        break;
    case 9:
        download->initialized = FALSE;
        func_ov009_02156de0(NULL, NULL);
        func_ov121_021deea4(seq, func_ov121_021dedc8);
        break;
    case DOWNLOAD_WAIT:
        func_ov114_021c5ab8();
        if (download->timeout++ > 7200) {
            func_0203c9ac(0, 0, 0, 1013);
            net->result = MYSTERY_NET_RECV_ERROR;
            func_ov121_021dee38(download, 3, state, 9, 9);
        }
        if (data_ov121_021dfc00.ready) {
            data_ov121_021dfc00.ready = FALSE;
            if (data_ov121_021dfc00.error != 0) {
                net->result = MYSTERY_NET_RECV_ERROR;
                func_ov121_021dee38(download, 3, state, 9, 9);
                return;
            }
            *state = download->nextState;
            return;
        }
        if (download->cancel) {
            download->cancelled = TRUE;
        }
        break;
    case DOWNLOAD_WAIT_CLEANUP:
        func_ov114_021c5ab8();
        if (download->timeout++ > 7200) {
            func_0203c9ac(0, 0, 0, 1013);
            *state = download->nextState;
        }
        if (data_ov121_021dfc00.done) {
            data_ov121_021dfc00.done = FALSE;
            *state = download->nextState;
            return;
        }
        if (download->cancel) {
            download->cancelled = TRUE;
        }
        break;
    }
}

// pokebw2's MysteryNet_WifiEnd
static void func_ov121_021dedc8(MysteryNetSeq *seq, int *state, void *work) {
    MysteryNet *net = work;

    switch (*state) {
    case 0:
        if (func_0203ceac(NULL)) {
            *state = 1;
        } else {
            *state = 2;
        }
        break;
    case 1:
        if (!func_0203cd98()) {
            net->exited = FALSE;
            *state = 2;
        }
        break;
    case 2:
        func_ov121_021deea4(seq, func_ov121_021de618);
        break;
    }
}

// pokebw2's MysteryNetDownload_Wait
static void func_ov121_021dee14(MysteryNetDownload *download, int *state, u32 nextState) {
    data_ov121_021dfc00.ready = FALSE;
    data_ov121_021dfc00.error = 0;
    download->nextState = nextState;
    download->timeout = 0;
    *state = DOWNLOAD_WAIT;
}

// Ends the download library, then goes on to nextState, or to failState at once if it can't
static void func_ov121_021dee38(MysteryNetDownload *download, u32 error, int *state, u32 nextState,
                                       int failState) {
    data_ov121_021dfc00.done = FALSE;
    data_ov121_021dfc00.error = error;
    download->nextState = nextState;
    download->timeout = 0;
    *state = DOWNLOAD_WAIT_CLEANUP;
    if (!func_ov114_021c5b80()) {
        *state = failState;
    }
}

// pokebw2's MysteryNetSeq_Init
static void func_ov121_021dee68(MysteryNetSeq *seq, void *work, MysteryNetSeqFunc func) {
    MI_CpuFill8(seq, 0, sizeof(MysteryNetSeq));
    seq->work = work;
    func_ov121_021deea4(seq, func);
}

// pokebw2's MysteryNetSeq_Exit
static void func_ov121_021dee84(MysteryNetSeq *seq) {
    MI_CpuFill8(seq, 0, sizeof(MysteryNetSeq));
}

// pokebw2's MysteryNetSeq_Main
static void func_ov121_021dee90(MysteryNetSeq *seq) {
    if (!seq->end) {
        seq->func(seq, &seq->state, seq->work);
    }
}

// pokebw2's MysteryNetSeq_SetNext
static void func_ov121_021deea4(MysteryNetSeq *seq, MysteryNetSeqFunc func) {
    seq->func = func;
    seq->state = 0;
}

// pokebw2's MysteryNetSeq_IsCurrent
static BOOL func_ov121_021deeac(MysteryNetSeq *seq, MysteryNetSeqFunc func) {
    if (seq->func == func) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MysteryNet_GetInfo
static void *func_ov121_021deebc(void *work) {
    MysteryNet *net = work;

    return net->info;
}

// pokebw2's MysteryNet_GetInfoSize
static int func_ov121_021deec0(void *work) {
    return sizeof(((MysteryNet *)NULL)->info);
}

// pokebw2's MysteryNet_IsSameInfo
static BOOL func_ov121_021deec4(u32 a, u32 b) {
    if (a == b) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MysteryNet_OnExit
static void func_ov121_021deed0(void *work) {
    MysteryNet *net = work;

    net->exited = TRUE;
}

// pokebw2's MysteryNet_OnNdEvent
static void func_ov121_021deed8(u32 reason, u32 error, int serverError) {
    data_ov121_021dfc00.ready = TRUE;
    data_ov121_021dfc00.error = error;
    func_ov121_021deeec();
}

// pokebw2's MysteryNet_SetNdDone
static void func_ov121_021deeec(void) {
    data_ov121_021dfc00.done = TRUE;
}

// pokebw2's Mystery_ParseHex
static u32 func_ov121_021deef8(const char *str) {
    u32 i;
    s32 digit;
    u32 value = 0;

    while (TRUE) {
        digit = -1;
        for (i = 0; i < NELEMS(data_ov121_021dfa8c); i++) {
            if (str[0] == data_ov121_021dfa8c[i].c) {
                digit = data_ov121_021dfa8c[i].value;
                break;
            }
        }
        if (digit == -1) {
            break;
        }
        value <<= 4;
        value += digit;
        str++;
    }
    return value;
}

// pokebw2's Mystery_IsZero
static BOOL func_ov121_021def38(const u32 *data, u32 size) {
    u32 i;

    for (i = 0; i < size / 4; i++) {
        if (data[i] != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's MysteryNet_OnDwcEvent
static u32 func_ov121_021def54(void *work, int a1, int event, int a3) {
    MysteryNetDownload *download = work;

    switch (event) {
    case 3:
    case 5:
    case 6:
        return download->initialized;
    }
    return 0;
}
