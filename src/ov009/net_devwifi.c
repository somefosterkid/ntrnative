// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov011/net_devwifi.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// net_devwifi.c is the name from the ROM's own string. The GFL net's device for Wi-Fi: each slot of its table passes
// the call on to dwc_rap.c or dwc_rapcommon.c. The names are ours.

#include "gfl/net_devwifi.h"
#include "types.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_state.h"

// Black 2's; twinned code confirms the offsets of pNetInit in Black
typedef struct {
    GFLNetInitData *pNetInit;
    u32 unk4;
} NetDevWifiWork;

// pokebw2's sNetDevWifiWork
static NetDevWifiWork *data_ov009_021a8260;

static void func_ov009_02155160(int a0, int a1);
static BOOL func_ov009_02155168(HeapID heapId, void *sys, BOOL (*callback)(BOOL success), void *work);
static BOOL func_ov009_021551b0(void (*callback)(BOOL ok));
static int func_ov009_021551d4(u16 connectBits);
static BOOL func_ov009_021551dc(int a0, int a1);
static BOOL func_ov009_02155204(void (*callback)(BOOL ok));
static BOOL func_ov009_02155234(const char *key, int numEntry, BOOL a2, int a3);
static BOOL func_ov009_0215523c(int a0, int a1, int a2);
static BOOL func_ov009_02155244(BOOL a0, int a1);
static BOOL func_ov009_02155264(u8 *data, int size, int a2, GFLNetSendDoneFunc callback);
static BOOL func_ov009_02155280(GFLNetRecvFunc callback);
static BOOL func_ov009_0215528c(void);
static BOOL func_ov009_02155294(void);
static BOOL func_ov009_021552ac(void);
static BOOL func_ov009_021552c4(void);
static int func_ov009_021552cc(void);
static int func_ov009_021552d4(void);
static int func_ov009_021552dc(void);
static BOOL func_ov009_021552e8(void);
static void func_ov009_02155330(int a0);
static int func_ov009_02155338(int a0);
static int func_ov009_0215534c(void);
static int func_ov009_02155354(void);
static BOOL func_ov009_02155364(void);
static void func_ov009_0215535c(int a0);

// pokebw2's sNetDevWifiTable
static GFLNetDevTable data_ov009_021a1b80 = {
    .unk00 = func_ov009_02155160,
    .init = func_ov009_02155168,
    .unk08 = func_ov009_021551b0,
    .update = func_ov009_021551d4,
    .unk10 = func_ov009_021551dc,
    .unk14 = func_ov009_02155204,
    .unk4C = func_ov009_02155234,
    .unk50 = func_ov009_0215523c,
    .unk54 = func_ov009_02155244,
    .send = func_ov009_02155264,
    .setRecvCallback = func_ov009_02155280,
    .unk6C = func_ov009_0215528c,
    .isConnected = func_ov009_02155294,
    .unk74 = func_ov009_021552ac,
    .unk78 = func_ov009_021552c4,
    .getConnectBits = func_ov009_021552cc,
    .getNetId = func_ov009_021552d4,
    .getSignalLevel = func_ov009_021552dc,
    .isError = func_ov009_021552e8,
    .unk8C = func_ov009_02155330,
    .unk90 = func_ov009_02155338,
    .unk94 = func_ov009_0215534c,
    .unk98 = func_ov009_02155354,
    .unkAC = func_ov009_02155364,
    .unkB8 = func_ov009_0215535c,
};

// pokebw2's NetDevWifi_Prepare
static void func_ov009_02155160(int a0, int a1) {
    func_ov009_02156dfc(a0);
}

// pokebw2's NetDevWifi_Init
static BOOL func_ov009_02155168(HeapID heapId, void *sys, BOOL (*callback)(BOOL success), void *work) {
    GFLNetInitData *pNetInit;

    if (data_ov009_021a8260 != NULL) {
        return FALSE;
    }
    pNetInit = func_0203d798();
    func_ov009_02157048(pNetInit->wifiHeapId, pNetInit->wifiHeapSize);
    data_ov009_021a8260 = GFL_HeapAllocate(heapId, sizeof(NetDevWifiWork), TRUE, "net_devwifi.c", 186);
    data_ov009_021a8260->pNetInit = sys;
    return TRUE;
}

// pokebw2's NetDevWifi_Start
static BOOL func_ov009_021551b0(void (*callback)(BOOL ok)) {
    BOOL result = func_ov009_0215539c(func_0203d5ec(), func_0203d610());

    if (callback != NULL) {
        callback(TRUE);
    }
    return result;
}

// pokebw2's NetDevWifi_Update
static int func_ov009_021551d4(u16 connectBits) {
    return func_ov009_0215592c(connectBits);
}

// pokebw2's NetDevWifi_Unk10
static BOOL func_ov009_021551dc(int a0, int a1) {
    if (func_ov009_02156690(a0 == 0)) {
        func_ov009_02156730();
        return TRUE;
    }
    func_ov009_0215592c(a0);
    return TRUE;
}

// pokebw2's NetDevWifi_Exit
static BOOL func_ov009_02155204(void (*callback)(BOOL ok)) {
    if (data_ov009_021a8260 != NULL) {
        func_ov009_0215677c();
        if (callback != NULL) {
            callback(TRUE);
        }
        GFL_HeapFree(data_ov009_021a8260);
        data_ov009_021a8260 = NULL;
    }
    func_ov009_021570b8();
    return TRUE;
}

// pokebw2's NetDevWifi_Unk4C
static BOOL func_ov009_02155234(const char *key, int numEntry, BOOL a2, int a3) {
    return func_ov009_0215568c(key, numEntry, a2, a3);
}

// pokebw2's NetDevWifi_Unk50
static BOOL func_ov009_0215523c(int a0, int a1, int a2) {
    return func_ov009_02156888(a0, a1, a2);
}

// pokebw2's NetDevWifi_Unk54
static BOOL func_ov009_02155244(BOOL a0, int a1) {
    if (func_ov009_02156690(a0 == 0)) {
        func_ov009_02156730();
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NetDevWifi_Send
static BOOL func_ov009_02155264(u8 *data, int size, int a2, GFLNetSendDoneFunc callback) {
    if (func_ov009_02155b14(data, size)) {
        if (callback != NULL) {
            callback(TRUE);
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NetDevWifi_SetRecvCallback
static BOOL func_ov009_02155280(GFLNetRecvFunc callback) {
    func_ov009_02155648(callback, callback);
    return TRUE;
}

// pokebw2's NetDevWifi_Unk6C
static BOOL func_ov009_0215528c(void) {
    return func_ov009_02155388();
}

// pokebw2's NetDevWifi_IsConnected
static BOOL func_ov009_02155294(void) {
    if (func_ov009_021562ec() != -1) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NetDevWifi_Unk74
static BOOL func_ov009_021552ac(void) {
    BOOL result = func_ov009_02156d84();

    if (!func_ov009_02155388()) {
        result |= TRUE;
    }
    return result;
}

// pokebw2's NetDevWifi_Unk78
static BOOL func_ov009_021552c4(void) {
    return func_ov009_02156cac();
}

// pokebw2's NetDevWifi_GetConnectBits
static int func_ov009_021552cc(void) {
    return DWC_GetAIDBitmap();
}

// pokebw2's NetDevWifi_GetNetId
static int func_ov009_021552d4(void) {
    return func_ov009_021562ec();
}

// pokebw2's NetDevWifi_GetSignalLevel
static int func_ov009_021552dc(void) {
    return 3 - func_0205ccd8();
}

// pokebw2's NetDevWifi_IsError
static BOOL func_ov009_021552e8(void) {
    int code;
    int type;
    GFLNetErrorInfo *info;
    int result = DWC_GetLastErrorEx(&code, &type);

    if (result != 0) {
        func_0203c9ac(code, type, result, 0);
    }
    if (result == 0) {
        return 0;
    }
    if (code == 0) {
        return -10000;
    }
    info = func_0203ca98();
    return info->type != 0 ? info->type : code;
}

// pokebw2's NetDevWifi_Unk8C
static void func_ov009_02155330(int a0) {
    func_ov009_02156db0(a0);
}

// pokebw2's NetDevWifi_Unk90
static int func_ov009_02155338(int a0) {
    if (func_ov009_02156874(a0) == 6) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NetDevWifi_Unk94
static int func_ov009_0215534c(void) {
    return func_ov009_02156b70();
}

// pokebw2's NetDevWifi_Unk98
static int func_ov009_02155354(void) {
    return func_ov009_02156b8c();
}

// pokebw2's NetDevWifi_UnkB8
static void func_ov009_0215535c(int a0) {
    func_ov009_02156d70(a0);
}

// pokebw2's NetDevWifi_UnkAC
static BOOL func_ov009_02155364(void) {
    return func_ov009_02156cd0();
}

// pokebw2's NetDevWifi_GetTable
const GFLNetDevTable *func_ov009_0215536c(void) {
    return &data_ov009_021a1b80;
}
